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

    double calculateCanopyHeatPerUnitArea();
    double calculateCrownFireHeatPerUnitArea();
    double calculateCrownFireSpreadRate();
    double calculateCrownFuelLoad();
    double calculateCrownFireTransitionRatio();
    double calculateCrownFireFirelineIntensity();
    double calculateCrownCriticalFireSpreadRate();
    double calculateCrownCriticalSurfaceFireIntensity();
    double calculateCrownCriticalSurfaceFlameLength();
    double calculateCrownFireFlameLength();
    double calculateCrownFirePowerOfFire();
    double calcuateCrownFirePowerOfWind();
    double calcualteCrownFirePowerRatio();
    double calculateCrownFireActiveRatio();

private:
    const FuelModelSet* fuelModelSet_;
    const CrownInputs* crownInputs_;
    const SurfaceInputs* surfaceInputs_;
   
    SurfaceInputs crownDeepCopyOfSurfaceInputs_; // deep copy of Surface's surface inputs to allow parallel runs in Surface
    SurfaceFireSpread crownFireSpread_; // stores and operates on Crown's surface fire data to allow parallel runs in Surface

    double calculateWindSpeedAtTwentyFeet();

    double crownCopyOfSurfaceHeatPerUnitArea_;
    double crownCopyOfSurfaceFirelineIntensity_;
    double crownFuelLoad_;
    double canopyHeatPerUnitArea_;
    double crownFireHeatPerUnitArea_;
    double crownFireFirelineIntensity_;
    double crownFireFlameLength_;
    double crownFireSpreadRate_;
    double crownCriticalSurfaceFireIntensity_;
    double crownCriticalFireSpreadRate_;
    double crownFirePowerOfFire_;
    double crownFirePowerOfWind_;
    double crownFirePowerRatio_;
    double crownFireActiveRatio_;
    double windSpeedAtTwentyFeet_;
};

#endif // CROWN_HEADER
