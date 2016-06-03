#ifndef CROWN_HEADER
#define CROWN_HEADER

#include "crownInputs.h"
#include "surface.h"

class FuelModelSet;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModelSet& fuelModelSet, const Surface& surface);
    ~Crown();

    Crown(const Crown &rhs);
    Crown& operator= (const Crown& rhs);

    double calculateCrownFireSpreadRate();
    void updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture);

private:
    const FuelModelSet* fuelModelSet_;  // pointer to BehaveRun's FuelModelSet object
    CrownInputs crownInputs_;    // pointer to BehaveRun's CrownInputs object
    
    // SURFACE module objects and pointers
    const Surface* surface_;                              // pointer to the BehaveRun's Surface object
    Surface crownDeepCopyOfSurface_;    // deep copy of Surface's surface inputs to allow parallel runs in Surface
    //SurfaceFireSpread crownFireSpread_;             // stores and operates on Crown's surface fire data to allow parallel runs in Surface

    // Private methods
    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    void calculateCrownFuelLoad();
    void calculateCrownFirelineIntensity();
    void calculateCrownFlameLength();
    void calculateCrownPowerOfFire();
    void calcuateCrownPowerOfWind();
    void calculateCrownCriticalFireSpreadRate();
    void calculateCrownCriticalSurfaceFireIntensity();
    void calculateCrownCriticalSurfaceFlameLength();
    void calculateCrownFireTransitionRatio();
    void calcualteCrownFirePowerRatio();
    void calculateCrownFireActiveRatio();
    double calculateWindSpeedAtTwentyFeet();

    // Member variables
    double crownCopyOfSurfaceHeatPerUnitArea_;
    double crownCopyOfSurfaceFirelineIntensity_;
    double crownFuelLoad_;                          // return Crown fire fuel load (lb / ft2)
    double canopyHeatPerUnitArea_;
    double crownFireHeatPerUnitArea_;
    double crownFirelineIntensity_;                 // Crown fire fireline intensity (Btu / ft / s)
    double crownFlameLength_;                       // Crown fire flame length (ft)
    double crownFireSpreadRate_;
    double crownCriticalSurfaceFireIntensity_;      // Critical surface fire intensity (Btu / ft / s)
    double crownCriticalFireSpreadRate_;            // Critical crown fire spread rate (ft / min)
    double crownCriticalSurfaceFlameLength_;        // Critical surface fire flame length (ft)
    double crownPowerOfFire_;                       // Crown fire 'power of the fire' ( ft*lb / s / ft2)
    double crownPowerOfWind_;                       // Crown fire 'power of the wind' ( ft*lb / s / ft2)
    double crownFirePowerRatio_;
    double crownFireActiveRatio_;                   // Crown fire active ratio
    double crownFireTransitionRatio_;
    double windSpeedAtTwentyFeet_;
};

#endif // CROWN_HEADER
