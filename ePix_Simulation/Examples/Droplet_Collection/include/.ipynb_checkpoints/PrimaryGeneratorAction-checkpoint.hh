#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include <vector>

class G4ParticleGun;
class G4Event;
class EventAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction(EventAction* eventAction);
    virtual ~PrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event* event) override;

    //===================================================
    G4int GetNGridPoints() const { return fGridNx * fGridNy; }
    //===================================================

private:
    G4ParticleGun* fParticleGun;
    EventAction*   fEventAction;

    //===================================================
    // ---- Grid configuration ----
    G4int    fGridNx   = 40;
    G4int    fGridNy   = 40;
    G4double fPitch    = 1000. * um;          // pixel pitch
    G4ThreeVector fGridCenter = {0., 0., 0.}; // center of scan on detector
    G4double fZstart   = -1. * mm;           // launch just above surface (adjust!)

    // ---- Per-point "recipe": one entry per electron fired at each grid point ----
    // 1 e- per pixel:
    // std::vector<G4ThreeVector> fOffsets = { {0., 0., 0.} };
    // 2 e- per pixel:
    std::vector<G4ThreeVector> fOffsets = { {0., 0., 0.} , {0., 0., 0.}};
    // Adjacent Pairs:
    // std::vector<G4ThreeVector> fOffsets = { {0., 0., 0.} , {100.*um, 0., 0.}};
    // Diagonal Pairs:
    // std::vector<G4ThreeVector> fOffsets = { {0., 0., 0.} , {100.*um, 100.*um, 0.}};
    // (see main answer for 2-electron variants)

    G4ThreeVector GridPosition(G4int index) const;
    //===================================================

};

#endif