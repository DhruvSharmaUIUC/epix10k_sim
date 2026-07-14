#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    // Sensor geometry constants (accessible from other classes)
    static constexpr G4int kNPixelsX = 352;
    static constexpr G4int kNPixelsY = 384;
    static constexpr G4double kPixelPitchX = 100.0; // um
    static constexpr G4double kPixelPitchY = 100.0; // um
    static constexpr G4double kPixelHalfZ  = 250.0; // um
};

#endif