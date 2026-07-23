#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto am = G4AnalysisManager::Instance();
    am->SetDefaultFileType("root");
    am->SetVerboseLevel(1);

    // =============================================
    // HISTOGRAMS ONLY (minimal output)
    // =============================================

    // H1 ID 0: Total energy deposited per event in sensor
    am->CreateH1("TotalEdep", "Total Edep per Event;E_{dep} [MeV];Counts",
                 500, 0.0, 1.0 * MeV);

    // H1 ID 1: Energy deposited per primary electron (should show Landau)
    am->CreateH1("PrimaryEdep", "Primary Electron Edep;E_{dep} [MeV];Counts",
                 500, 0.0, 1.0 * MeV);

    // H2 ID 0: 2D energy deposition map (Edeps per pixel)
    am->CreateH2("EdepMap", "Energy Deposition Map;X [mm];Y [mm]",
                 352, -17.6 * mm, 17.6 * mm,
                 384, -19.2 * mm, 19.2 * mm);

    // H2 ID 1: 2D primary electron entry map (ground truth)
    am->CreateH2("EntryMap", "Primary Entry Map;X [mm];Y [mm]",
                 352, -17.6 * mm, 17.6 * mm,
                 384, -19.2 * mm, 19.2 * mm);
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