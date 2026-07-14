#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Event.hh"

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

G4ThreeVector PrimaryGeneratorAction::GridPosition(G4int index) const
{
    const G4int ix = index % fGridNx;   // column
    const G4int iy = index / fGridNx;   // row

    // Centered coordinates: for Nx=10, x runs over -4.5,...,+4.5 (times pitch)
    // => ±50, ±150, ..., ±450 um, i.e. pixel centers. 
    const G4double x = (ix - (fGridNx - 1) / 2.0) * fPitch;
    const G4double y = (iy - (fGridNy - 1) / 2.0) * fPitch;

    return fGridCenter + G4ThreeVector(x, y, fZstart);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) 
{
    const G4int evtID = anEvent->GetEventID();

    // Safety: if more events are requested than grid points, wrap around.
    const G4int index = evtID % GetNGridPoints();

    const G4ThreeVector base = GridPosition(index);

    G4double xbase = base[0];
    G4double ybase = base[1];
    G4double zbase = base[2];
    
    for (const auto& off : fOffsets) {
        G4double xoff = off[0];
        G4double yoff = off[1];
        G4double zoff = off[2];
        
        fParticleGun->SetParticlePosition(G4ThreeVector(xbase + xoff, ybase + yoff, zbase + zoff));
        fParticleGun->GeneratePrimaryVertex(anEvent);
    
        // Store primary info for the event record
        fEventAction->SetPrimaryPosition(xbase + xoff, ybase + yoff, zbase + zoff);
        fEventAction->SetPrimaryEnergy(fParticleGun->GetParticleEnergy());
    }
}