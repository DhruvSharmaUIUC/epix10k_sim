#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;

class EventAction : public G4UserEventAction {
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event) override;
    virtual void EndOfEventAction(const G4Event* event) override;

    // Called by SteppingAction for each step in the sensor
    void AccumulateEdep(G4double edep, G4int parentID);

    // Called by PrimaryGeneratorAction to store gun position
    void SetPrimaryPosition(G4double x, G4double y, G4double z);
    void SetPrimaryEnergy(G4double e);

private:
    RunAction* fRunAction;

    // Per-event accumulators (minimal)
    G4double fTotalEdep;
    G4double fPrimaryEdep;

    // Primary particle info
    G4double fPrimaryX, fPrimaryY, fPrimaryZ, fPrimaryE;
};

#endif