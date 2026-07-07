#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class EventAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction(EventAction* eventAction);
    virtual ~PrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event* event) override;

private:
    G4ParticleGun* fParticleGun;
    EventAction*   fEventAction;
};

#endif