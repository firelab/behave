#ifndef CROWN_HEADER
#define CROWN_HEADER

#include "crownInputs.h"
#include "surfaceInputs.h"
#include "surfaceFireSpread.h"

class FuelModelSet;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModelSet& fuelModelSet, const CrownInputs& crownInputs, const SurfaceInputs& surfaceInputs, double surfaceHeatPerUnitArea, double surfaceFirelineIntensity);
    ~Crown();

    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    double calculateCrownFireSpreadRate(double windSpeedAtTwentyFeet);
    void calculateCrownFuelLoad();
    double Crown::calculateCrownFireTransitionRatio(double surfaceFireIntensity, double criticalFireIntensity);
    double Crown::calculateCrownFireFirelineIntensity();

private:
    const FuelModelSet* fuelModelSet_;
    const CrownInputs* crownInputs_;
    const SurfaceInputs* surfaceInputs_;
   
    SurfaceInputs crownDeepCopyOfSurfaceInputs_; // deep copy of Surface's surface inputs to allow parallel runs in Surface
    SurfaceFireSpread crownFireSpread_; // stores and operates on Crown's surface fire data to allow parallel runs in Surface

    double crownCopyOfSurfaceHeatPerUnitArea_;
    double crownFuelLoad_;
    double canopyHeatPerUnitArea_;
    double crownFireHeatPerUnitArea_;
    double crownFireSpreadRate_;
};

#endif // CROWN_HEADER
