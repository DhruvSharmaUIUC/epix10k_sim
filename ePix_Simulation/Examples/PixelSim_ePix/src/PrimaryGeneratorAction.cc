#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* eventAction)
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(nullptr),
      fEventAction(eventAction)
{
    fParticleGun = new G4ParticleGun(1);

    auto particleTable = G4ParticleTable::GetParticleTable();
    auto particle = particleTable->FindParticle("e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(3.12 * MeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // Random position over the sensor face
    G4double halfX = 0.5 * DetectorConstruction::kNPixelsX
                     * DetectorConstruction::kPixelPitchX * um;
    G4double halfY = 0.5 * DetectorConstruction::kNPixelsY
                     * DetectorConstruction::kPixelPitchY * um;

    G4double xpos = CLHEP::RandFlat::shoot(-halfX, halfX);
    G4double ypos = CLHEP::RandFlat::shoot(-halfY, halfY);
    G4double zpos = -250.0 * um;  // just above sensor front face

    fParticleGun->SetParticlePosition(G4ThreeVector(xpos, ypos, zpos));
    fParticleGun->GeneratePrimaryVertex(anEvent);

    // Store primary info for the event record
    fEventAction->SetPrimaryPosition(xpos, ypos, zpos);
    fEventAction->SetPrimaryEnergy(fParticleGun->GetParticleEnergy());
}