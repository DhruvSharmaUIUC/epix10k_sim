#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;
class EventAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction(EventAction*);
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override;

private:
    G4ParticleGun* fParticleGun;
    EventAction* fEventAction;
};

#endif