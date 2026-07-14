#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    // --- Materials ---
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* pixel_mat = nist->FindOrBuildMaterial("G4_Si");

    // --- World Volume ---
    G4double world_size = 25.0 * cm;
    auto solidWorld = new G4Box("World", world_size, world_size, world_size);
    auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    auto physWorld  = new G4PVPlacement(
        0, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    // --- Pixel Sensor Module (monolithic Si slab) ---
    // Full sensor: 352 pixels x 100 um = 35.2 mm in X
    //              384 pixels x 100 um = 38.4 mm in Y
    //              Thickness: 450 um (half = 225 um)
    G4double sensorHalfX = 0.5 * kNPixelsX * kPixelPitchX * um;  // 17.6 mm
    G4double sensorHalfY = 0.5 * kNPixelsY * kPixelPitchY * um;  // 19.2 mm
    G4double sensorHalfZ = kPixelHalfZ * um;                      // 225 um

    auto solidSensor = new G4Box("Sensor", sensorHalfX, sensorHalfY, sensorHalfZ);
    auto logicSensor = new G4LogicalVolume(solidSensor, pixel_mat, "Sensor");

    // Step limiter: ensure multiple steps per pixel for accurate deposition
    G4double maxStep = 10.0 * um;
    logicSensor->SetUserLimits(new G4UserLimits(maxStep));

    // Region with production cuts
    auto sensorRegion = new G4Region("SensorRegion");
    sensorRegion->AddRootLogicalVolume(logicSensor);
    auto cuts = new G4ProductionCuts();
    cuts->SetProductionCut(1.0 * um);  // low-energy secondaries
    sensorRegion->SetProductionCuts(cuts);

    // Place sensor at origin (front face at z = -225 um, back face at z = +225 um)
    new G4PVPlacement(
        0, G4ThreeVector(0, 0, 0),
        logicSensor, "Sensor", logicWorld, false, 0, true);

    return physWorld;
}