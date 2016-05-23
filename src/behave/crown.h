#ifndef CROWN_HEADER
#define CROWN_HEADER

#include "crownInputs.h"
#include "surfaceInputs.h"
#include "surface.h"

class FuelModelSet;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModelSet& fuelModelSet, const CrownInputs& crownInputs, const SurfaceInputs& surfaceInputs, const Surface& surface);
    ~Crown();

    Crown(const Crown &rhs);
    Crown& operator= (const Crown& rhs);

    double calculateCrownFireSpreadRate();

   
    double calculateCrownFireTransitionRatio();
    
    double calculateCrownCriticalFireSpreadRate();
    double calculateCrownCriticalSurfaceFireIntensity();
    double calculateCrownCriticalSurfaceFlameLength();
    double calculateCrownFlameLength();
    
    double calcualteCrownFirePowerRatio();
    double calculateCrownFireActiveRatio();

private:
    const FuelModelSet* fuelModelSet_;
    const CrownInputs* crownInputs_;
    const SurfaceInputs* surfaceInputs_;
    const Surface* surface_;

    SurfaceInputs crownDeepCopyOfSurfaceInputs_; // deep copy of Surface's surface inputs to allow parallel runs in Surface
    SurfaceFireSpread crownFireSpread_; // stores and operates on Crown's surface fire data to allow parallel runs in Surface

    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    void calculateCrownFuelLoad();
    void calculateCrownFirelineIntensity();
    void calculateCrownPowerOfFire();
    void calcuateCrownPowerOfWind();

    double calculateWindSpeedAtTwentyFeet();

    double crownCopyOfSurfaceHeatPerUnitArea_;
    double crownCopyOfSurfaceFirelineIntensity_;
    double crownFuelLoad_;
    double canopyHeatPerUnitArea_;
    double crownFireHeatPerUnitArea_;
    double crownFirelineIntensity_;
    double crownFlameLength_;
    double crownFireSpreadRate_;
    double crownCriticalSurfaceFireIntensity_;
    double crownCriticalFireSpreadRate_;
    double crownPowerOfFire_;
    double crownPowerOfWind_;
    double crownFirePowerRatio_;
    double crownFireActiveRatio_;
    double windSpeedAtTwentyFeet_;
};

#endif // CROWN_HEADER
