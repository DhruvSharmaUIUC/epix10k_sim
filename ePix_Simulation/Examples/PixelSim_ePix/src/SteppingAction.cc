#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"

#include <cmath>

SteppingAction::SteppingAction(EventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    // Only record steps inside the sensor volume
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    if (!volume) return;
    if (volume->GetName() != "Sensor") return;

    // Get energy deposit
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return;  // skip zero-deposit steps

    // Compute midpoint position
    G4ThreeVector prePos  = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector postPos = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector midPos  = 0.5 * (prePos + postPos);

    // Compute pixel indices from position
    // Sensor spans [-17.6mm, +17.6mm] in X and [-19.2mm, +19.2mm] in Y
    G4double sensorHalfX = 0.5 * DetectorConstruction::kNPixelsX
                           * DetectorConstruction::kPixelPitchX * um;
    G4double sensorHalfY = 0.5 * DetectorConstruction::kNPixelsY
                           * DetectorConstruction::kPixelPitchY * um;

    G4int pixelX = (G4int)std::floor(
        (midPos.x() + sensorHalfX) / (DetectorConstruction::kPixelPitchX * um));
    G4int pixelY = (G4int)std::floor(
        (midPos.y() + sensorHalfY) / (DetectorConstruction::kPixelPitchY * um));

    // Clamp to valid range
    pixelX = std::max(0, std::min(pixelX, DetectorConstruction::kNPixelsX - 1));
    pixelY = std::max(0, std::min(pixelY, DetectorConstruction::kNPixelsY - 1));

    // Get track info
    const G4Track* track = step->GetTrack();

    // Build the hit struct
    StepHit hit;
    hit.trackID    = track->GetTrackID();
    hit.parentID   = track->GetParentID();
    hit.pdgCode    = track->GetDefinition()->GetPDGEncoding();
    hit.pixelX     = pixelX;
    hit.pixelY     = pixelY;
    hit.x          = midPos.x();
    hit.y          = midPos.y();
    hit.z          = midPos.z();
    hit.edep       = edep;
    hit.stepLength = step->GetStepLength();
    hit.kineticE   = step->GetPreStepPoint()->GetKineticEnergy();
    hit.globalTime = step->GetPreStepPoint()->GetGlobalTime();

    // Pass to EventAction for accumulation
    fEventAction->RecordStep(hit);
}