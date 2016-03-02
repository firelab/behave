#ifndef SURFACE_HEADER
#define SURFACE_HEADER

// The SURFACE module of BehavePlus

//	TODO: Need to add logic to handle the following wind speed input methods: Midflame height, 
//	20 ft wind and input wind adjustment factor, 20 foot wind and calculate wind adjustment factor, 
//	10 meter wind and input wind adjustment factor, 10 meter wind calculate wind adjustment factor
//	WMC - 11/2015

#include "surfaceEnums.h"
#include "surfaceInputs.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceFireSpread.h"
#include "surfaceTwoFuelModels.h"

class Surface
{
    static const int MAX_SIZES = 4;
    static const int MAX_LIFE_STATES = 2;
    static const int DEAD = 0;
    static const int LIVE = 1;

public:
    Surface(FuelModels& fuelModels, SurfaceInputs& surfaceInputs);
    double calculateSurfaceFireForwardSpreadRate(double directionOfinterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfinterest);
    double getDirectionOfMaxSpread() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;

private:
    bool isUsingTwoFuelModels() const;

    // SURFACE Module component objects
    FuelModels*	fuelModels_;
    SurfaceFuelbedIntermediates surfaceFuelbedIntermediates_;
    SurfaceFireSpread surfaceFireSpread_;
    SurfaceInputs* surfaceInputs_;

    void initializeMembers();
};

#endif //SURFACEFIRE_HEADER
