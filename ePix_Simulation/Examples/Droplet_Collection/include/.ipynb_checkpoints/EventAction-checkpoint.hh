#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <map>
#include <vector>
#include <set>

class RunAction;

/// Per-step hit data accumulated during the event
struct StepHit {
    G4int    trackID;
    G4int    parentID;
    G4int    pdgCode;
    G4int    pixelX;      // pixel index X
    G4int    pixelY;      // pixel index Y
    G4double x, y, z;     // global position (mm)
    G4double edep;        // energy deposit (MeV)
    G4double stepLength;  // step length (mm)
    G4double kineticE;    // kinetic energy at pre-step (MeV)
    G4double globalTime;  // time since event start (ns)
};

/// Per-track summary accumulated during the event
struct TrackSummary {
    G4int    trackID;
    G4int    parentID;
    G4int    pdgCode;
    G4double totalEdep;     // total energy deposited in sensor
    G4double entryX, entryY, entryZ;  // first hit position
    G4double exitX, exitY, exitZ;     // last hit position
    G4int    nStepsInSensor;
};

class EventAction : public G4UserEventAction {
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event) override;
    virtual void EndOfEventAction(const G4Event* event) override;

    // Called by SteppingAction for each step in the sensor
    void RecordStep(const StepHit& hit);

    // Called by PrimaryGeneratorAction to store gun position
    void SetPrimaryPosition(G4double x, G4double y, G4double z);
    void SetPrimaryEnergy(G4double e);

private:
    RunAction* fRunAction;

    // Per-event accumulators
    std::vector<StepHit> fStepHits;
    std::map<G4int, TrackSummary> fTrackMap;  // trackID -> summary
    std::set<G4int> fPrimarySurfaceRecorded;

    // Primary particle info
    G4double fPrimaryX, fPrimaryY, fPrimaryZ, fPrimaryE;
    
    std::vector<G4ThreeVector> fPrimaryPositions;
    std::vector<G4double>      fPrimaryEnergies;
};

#endif