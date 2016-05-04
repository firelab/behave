#ifndef CROWN_HEADER
#define CROWN_HEADER

#include "crownInputs.h"
#include "surfaceInputs.h"
#include "surfaceFireSpread.h"

class FuelModels;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs, const CrownInputs& crownInputs, const double surfaceHeatPerUnitArea);
    ~Crown();

    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    double calculateCrownFireSpreadRate(double windSpeedAtTwentyFeet);
    void calculateCrownFuelLoad();

private:
    const FuelModels* fuelModels_;
    const CrownInputs* crownInputs_;
    const SurfaceInputs* surfaceInputs_;
   
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lb)

    

    SurfaceInputs crownDeepCopyOfSurfaceInputs_; // deep copy of Surface's surface inputs to allow parallel runs in Surface
    SurfaceFireSpread crownFireSpread_; // stores Crown's surface fire data to allow parallel runs in Surface

    double crownCopyOfSurfaceHeatPerUnitArea_;
    double crownFuelLoad_;
    double canopyHeatPerUnitArea_;
    double crownFireHeatPerUnitArea_;
    double crownRateOfSpread_;
};

#endif // CROWN_HEADER
