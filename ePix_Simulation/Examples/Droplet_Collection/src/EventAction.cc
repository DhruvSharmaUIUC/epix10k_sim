#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

#include <map>
#include <set>

EventAction::EventAction(RunAction* runAction)
    : G4UserEventAction(),
      fRunAction(runAction),
      fPrimaryX(0.), fPrimaryY(0.), fPrimaryZ(0.), fPrimaryE(0.)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
    fStepHits.clear();
    fTrackMap.clear();
    fPrimarySurfaceRecorded.clear();
    // fPrimaryX = 0.; fPrimaryY = 0.; fPrimaryZ = 0.; fPrimaryE = 0.;
}

void EventAction::SetPrimaryPosition(G4double x, G4double y, G4double z) {
    fPrimaryX = x; fPrimaryY = y; fPrimaryZ = z;
    fPrimaryPositions.emplace_back(x,y,z);
}

void EventAction::SetPrimaryEnergy(G4double e) {
    fPrimaryE = e;
    fPrimaryEnergies.push_back(e);
}

void EventAction::RecordStep(const StepHit& hit) {
    // Store per-step data
    fStepHits.push_back(hit);

    // Update per-track summary
    auto it = fTrackMap.find(hit.trackID);
    if (it == fTrackMap.end()) {
        // First step of this track in the sensor
        TrackSummary ts;
        ts.trackID   = hit.trackID;
        ts.parentID  = hit.parentID;
        ts.pdgCode   = hit.pdgCode;
        ts.totalEdep = hit.edep;
        ts.entryX = hit.x; ts.entryY = hit.y; ts.entryZ = hit.z;
        ts.exitX  = hit.x; ts.exitY  = hit.y; ts.exitZ  = hit.z;
        ts.nStepsInSensor = 1;
        fTrackMap[hit.trackID] = ts;
    } else {
        it->second.totalEdep += hit.edep;
        it->second.exitX = hit.x;
        it->second.exitY = hit.y;
        it->second.exitZ = hit.z;
        it->second.nStepsInSensor++;
    }
}

void EventAction::EndOfEventAction(const G4Event* event) {
    if (fStepHits.empty()) return;  // nothing deposited

    G4int eventID = event->GetEventID();
    auto am = G4AnalysisManager::Instance();

    // --- Compute event-level quantities ---
    G4double totalEdep   = 0.;
    G4double primaryEdep = 0.;
    G4int    nSecondaries = 0;
    std::map<std::pair<G4int,G4int>, G4double> pixelMap;  // (pixX,pixY) -> edep

    // --- Fill Ntuple 0: Hits ---
    for (const auto& h : fStepHits) {
        am->FillNtupleIColumn(0, 0, eventID);
        am->FillNtupleIColumn(0, 1, h.trackID);
        am->FillNtupleIColumn(0, 2, h.parentID);
        am->FillNtupleIColumn(0, 3, h.pdgCode);
        am->FillNtupleIColumn(0, 4, h.pixelX);
        am->FillNtupleIColumn(0, 5, h.pixelY);
        am->FillNtupleDColumn(0, 6, h.x / mm);
        am->FillNtupleDColumn(0, 7, h.y / mm);
        am->FillNtupleDColumn(0, 8, h.z / mm);
        am->FillNtupleDColumn(0, 9, h.edep / keV);
        am->FillNtupleDColumn(0, 10, h.stepLength / um);
        am->FillNtupleDColumn(0, 11, h.kineticE / MeV);
        am->FillNtupleDColumn(0, 12, h.globalTime / ns);
        am->AddNtupleRow(0);

        // Accumulate totals
        totalEdep += h.edep;
        if (h.parentID == 0) primaryEdep += h.edep;

        // Accumulate pixel map
        if (h.pixelX >= 0 && h.pixelY >= 0) {
            pixelMap[{h.pixelX, h.pixelY}] += h.edep;
        }

        // Fill 2D histogram
        am->FillH2(0, h.x, h.y, h.edep / keV);
    }

    // --- Fill Ntuple 1: Tracks ---
    for (const auto& [trkID, ts] : fTrackMap) {
        am->FillNtupleIColumn(1, 0, eventID);
        am->FillNtupleIColumn(1, 1, ts.trackID);
        am->FillNtupleIColumn(1, 2, ts.parentID);
        am->FillNtupleIColumn(1, 3, ts.pdgCode);
        am->FillNtupleDColumn(1, 4, ts.totalEdep / keV);
        am->FillNtupleDColumn(1, 5, ts.entryX / mm);
        am->FillNtupleDColumn(1, 6, ts.entryY / mm);
        am->FillNtupleDColumn(1, 7, ts.entryZ / mm);
        am->FillNtupleDColumn(1, 8, ts.exitX / mm);
        am->FillNtupleDColumn(1, 9, ts.exitY / mm);
        am->FillNtupleDColumn(1, 10, ts.exitZ / mm);
        am->FillNtupleIColumn(1, 11, ts.nStepsInSensor);
        am->AddNtupleRow(1);

        if (ts.parentID != 0) nSecondaries++;
    }

    // --- Fill Ntuple 2: Events ---
    am->FillNtupleIColumn(2, 0, eventID);
    am->FillNtupleDColumn(2, 1, totalEdep / keV);
    am->FillNtupleDColumn(2, 2, primaryEdep / keV);
    am->FillNtupleDColumn(2, 3, fPrimaryX / mm);
    am->FillNtupleDColumn(2, 4, fPrimaryY / mm);
    am->FillNtupleDColumn(2, 5, fPrimaryE / MeV);
    am->FillNtupleIColumn(2, 6, (G4int)fStepHits.size());
    am->FillNtupleIColumn(2, 7, (G4int)fTrackMap.size());
    am->FillNtupleIColumn(2, 8, nSecondaries);
    am->FillNtupleIColumn(2, 9, (G4int)pixelMap.size());
    am->AddNtupleRow(2);

    // --- Fill Ntuple 3: Pixels ---
    for (const auto& [pix, edep] : pixelMap) {
        am->FillNtupleIColumn(3, 0, eventID);
        am->FillNtupleIColumn(3, 1, pix.first);
        am->FillNtupleIColumn(3, 2, pix.second);
        am->FillNtupleDColumn(3, 3, edep / keV);
        am->AddNtupleRow(3);
    }

    // --- Fill summary histograms ---
    am->FillH1(0, totalEdep);    // Total edep per event
    am->FillH1(1, primaryEdep);  // Primary electron edep only

    // Entry map for primary
    for (const auto& p : fPrimaryPositions) {
        am->FillH2(1, p.x(), p.y(), 1.0);
    }
    fPrimaryPositions.clear();
    fPrimaryEnergies.clear();
}