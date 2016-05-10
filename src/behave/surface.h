#ifndef SURFACE_HEADER
#define SURFACE_HEADER

// The SURFACE module of BehavePlus

//	TODO: Need to add logic to handle the following wind speed input methods: Midflame height, 
//	20 ft wind and input wind adjustment factor, 20 foot wind and calculate wind adjustment factor, 
//	10 meter wind and input wind adjustment factor, 10 meter wind calculate wind adjustment factor
//	WMC - 11/2015

#include "surfaceFireSpread.h"

class SurfaceInputs;

class Surface
{
public:
    Surface(const FuelModelSet& fuelModelSet, SurfaceInputs& surfaceInputs);
    Surface(const Surface &rhs);
    Surface& operator= (const Surface& rhs);

    double calculateSurfaceFireForwardSpreadRate(double directionOfinterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfinterest);
    double getSpreadRate() const;
    double getDirectionOfMaxSpread() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getFirelineIntensity() const;

private:
    bool isUsingTwoFuelModels() const;

    // SURFACE Module component objects
    const FuelModelSet*	fuelModelSet_;
    SurfaceFireSpread surfaceFireSpread_;
    SurfaceInputs* surfaceInputs_;
};

#endif //SURFACEFIRE_HEADER
