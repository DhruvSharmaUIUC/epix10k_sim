#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto am = G4AnalysisManager::Instance();
    am->SetDefaultFileType("root");
    am->SetNtupleMerging(true);
    am->SetVerboseLevel(1);

    // =============================================
    // HISTOGRAMS (for quick sanity checks)
    // =============================================

    // H1 ID 0: Total energy deposited per event in sensor
    am->CreateH1("TotalEdep", "Total Edep per Event;E_{dep} [MeV];Counts",
                 500, 0.0, 1.0 * MeV);

    // H1 ID 1: Energy deposited per primary electron (should show Landau)
    am->CreateH1("PrimaryEdep", "Primary Electron Edep;E_{dep} [MeV];Counts",
                 500, 0.0, 1.0 * MeV);

    // H2 ID 0: 2D energy deposition map
    am->CreateH2("EdepMap", "Energy Deposition Map;X [mm];Y [mm]",
                 352, -17.6 * mm, 17.6 * mm,
                 384, -19.2 * mm, 19.2 * mm);

    // H2 ID 1: 2D primary electron entry map (ground truth)
    am->CreateH2("EntryMap", "Primary Entry Map;X [mm];Y [mm]",
                 352, -17.6 * mm, 17.6 * mm,
                 384, -19.2 * mm, 19.2 * mm);

    // =============================================
    // NTUPLE 0: Per-step hits (full detail)
    // =============================================
    am->CreateNtuple("Hits", "Per-step energy deposits in sensor");
    am->CreateNtupleIColumn("eventID");       // col 0
    am->CreateNtupleIColumn("trackID");       // col 1
    am->CreateNtupleIColumn("parentID");      // col 2
    am->CreateNtupleIColumn("pdgCode");       // col 3
    am->CreateNtupleIColumn("pixelX");        // col 4
    am->CreateNtupleIColumn("pixelY");        // col 5
    am->CreateNtupleDColumn("x_mm");          // col 6
    am->CreateNtupleDColumn("y_mm");          // col 7
    am->CreateNtupleDColumn("z_mm");          // col 8
    am->CreateNtupleDColumn("edep_keV");      // col 9
    am->CreateNtupleDColumn("stepLen_um");    // col 10
    am->CreateNtupleDColumn("kineticE_MeV"); // col 11
    am->CreateNtupleDColumn("time_ns");       // col 12
    am->FinishNtuple(0);

    // =============================================
    // NTUPLE 1: Per-track summary
    // =============================================
    am->CreateNtuple("Tracks", "Per-track summary in sensor");
    am->CreateNtupleIColumn("eventID");       // col 0
    am->CreateNtupleIColumn("trackID");       // col 1
    am->CreateNtupleIColumn("parentID");      // col 2
    am->CreateNtupleIColumn("pdgCode");       // col 3
    am->CreateNtupleDColumn("totalEdep_keV"); // col 4
    am->CreateNtupleDColumn("entryX_mm");     // col 5
    am->CreateNtupleDColumn("entryY_mm");     // col 6
    am->CreateNtupleDColumn("entryZ_mm");     // col 7
    am->CreateNtupleDColumn("exitX_mm");      // col 8
    am->CreateNtupleDColumn("exitY_mm");      // col 9
    am->CreateNtupleDColumn("exitZ_mm");      // col 10
    am->CreateNtupleIColumn("nSteps");        // col 11
    am->FinishNtuple(1);

    // =============================================
    // NTUPLE 2: Per-event summary
    // =============================================
    am->CreateNtuple("Events", "Per-event summary");
    am->CreateNtupleIColumn("eventID");           // col 0
    am->CreateNtupleDColumn("totalEdep_keV");     // col 1
    am->CreateNtupleDColumn("primaryEdep_keV");   // col 2
    am->CreateNtupleDColumn("primaryX_mm");       // col 3
    am->CreateNtupleDColumn("primaryY_mm");       // col 4
    am->CreateNtupleDColumn("primaryE_MeV");      // col 5
    am->CreateNtupleIColumn("nHits");             // col 6
    am->CreateNtupleIColumn("nTracks");           // col 7
    am->CreateNtupleIColumn("nSecondaries");      // col 8
    am->CreateNtupleIColumn("nPixelsFired");      // col 9
    am->FinishNtuple(2);

    // =============================================
    // NTUPLE 3: Per-pixel deposits per event
    // =============================================
    am->CreateNtuple("Pixels", "Per-pixel energy per event");
    am->CreateNtupleIColumn("eventID");       // col 0
    am->CreateNtupleIColumn("pixelX");        // col 1
    am->CreateNtupleIColumn("pixelY");        // col 2
    am->CreateNtupleDColumn("edep_keV");      // col 3
    am->FinishNtuple(3);
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto am = G4AnalysisManager::Instance();
    am->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto am = G4AnalysisManager::Instance();
    am->Write();
    am->CloseFile();
}