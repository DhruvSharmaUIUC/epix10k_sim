#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    // Geometry constants accessible to other classes
    static constexpr G4int kNPixelsX = 352;
    static constexpr G4int kNPixelsY = 384;
    static constexpr G4double kPixelPitchX = 100.0; // um (set in .cc with units)
    static constexpr G4double kPixelPitchY = 100.0; // um
    static constexpr G4double kPixelHalfZ  = 250.0; // um (half-thickness)
};

#endif