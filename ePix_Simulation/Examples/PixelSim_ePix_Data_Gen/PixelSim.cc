#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4StepLimiterPhysics.hh"
#include "Randomize.hh"
#include "G4RandomTools.hh"
#include <ctime>

int main(int argc, char** argv) {

    // 0. Set random seed
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    
    // Set a unique seed based on the current system time
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);

    // 1. Setup the User Interface for interactive mode
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // 2. Create the Run Manager (Automatic selection of MT or Serial)
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // 3. Set Mandatory Initialization: Geometry
    runManager->SetUserInitialization(new DetectorConstruction());

    // 4. Set Mandatory Initialization: Physics List
    // QBBC is a reliable physics list for EM and Hadronic interactions
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(physicsList);

    // 5. Set Mandatory Initialization: Action Initialization
    // This handles PrimaryGenerator, RunAction, and SteppingAction for MT
    runManager->SetUserInitialization(new ActionInitialization());

    // 6. Initialize Visualization Manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // 7. Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // 8. Process Macro or Start Interactive Session
    if (!ui) {
        // Batch mode: execute the macro file provided in arguments
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // Interactive mode: execute init_vis.mac and start UI
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    delete visManager;
    delete runManager;

    return 0;
}
