#include "EventAction.hh"
#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction(RunAction* runAction)
    : G4UserEventAction(),
      fRunAction(runAction),
      fTotalEdep(0.), fPrimaryEdep(0.),
      fPrimaryX(0.), fPrimaryY(0.), fPrimaryZ(0.), fPrimaryE(0.)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    fTotalEdep = 0.;
    fPrimaryEdep = 0.;
}

void EventAction::SetPrimaryPosition(G4double x, G4double y, G4double z) {
    fPrimaryX = x; fPrimaryY = y; fPrimaryZ = z;
}

void EventAction::SetPrimaryEnergy(G4double e) {
    fPrimaryE = e;
}

void EventAction::AccumulateEdep(G4double edep, G4int parentID) {
    fTotalEdep += edep;
    if (parentID == 0) fPrimaryEdep += edep;
}

void EventAction::EndOfEventAction(const G4Event*) {
    if (fTotalEdep <= 0.) return;  // nothing deposited

    auto am = G4AnalysisManager::Instance();

    // Fill 1D summary histograms
    am->FillH1(0, fTotalEdep);     // Total edep per event
    am->FillH1(1, fPrimaryEdep);   // Primary electron edep only

    // Fill entry map for primary (one entry per event)
    am->FillH2(1, fPrimaryX, fPrimaryY, 1.0);
}