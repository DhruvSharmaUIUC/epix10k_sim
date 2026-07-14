#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

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
    if (edep <= 0.) return;

    // Compute midpoint position
    G4ThreeVector prePos  = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector postPos = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector midPos  = 0.5 * (prePos + postPos);

    // Fill the 2D energy deposition histogram directly (no buffering)
    auto am = G4AnalysisManager::Instance();
    am->FillH2(0, midPos.x(), midPos.y(), edep / keV);

    // Accumulate event-level totals for H1 histograms
    G4int parentID = step->GetTrack()->GetParentID();
    fEventAction->AccumulateEdep(edep, parentID);
}