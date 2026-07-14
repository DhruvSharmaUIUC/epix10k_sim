#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
    auto runAction   = new RunAction;
    auto eventAction = new EventAction(runAction);

    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new PrimaryGeneratorAction(eventAction));
    SetUserAction(new SteppingAction(eventAction));
}