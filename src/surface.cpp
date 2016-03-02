#include "surface.h"

Surface::Surface(FuelModels& fuelModels, SurfaceInputs& surfaceInputs)
// Surface Module initialiazation list
: surfaceFuelbedIntermediates_{ fuelModels, surfaceInputs },
surfaceFireSpread_{ surfaceFuelbedIntermediates_, surfaceInputs }
{
    fuelModels_ = &fuelModels;
    surfaceInputs_ = &surfaceInputs;
}

double Surface::calculateSurfaceFireForwardSpreadRate(double directionOfInterest)
{
    double spreadRate = 0.0;

    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(*surfaceInputs_, surfaceFuelbedIntermediates_, surfaceFireSpread_);
        spreadRate = surfaceTwoFuelModels.calculateWeightedSpreadRate(directionOfInterest);
    }
    else // Use only one fuel model
    {
        // Calculate spread rate
        spreadRate = surfaceFireSpread_.calculateForwardSpreadRate(directionOfInterest);
    }
    // Return results
    return spreadRate;
}

double Surface::calculateSpreadRateAtVector(double directionOfinterest)
{
    return surfaceFireSpread_.calculateSpreadRateAtVector(directionOfinterest);
}

double Surface::getDirectionOfMaxSpread() const
{
    double directionOfMaxSpread = surfaceFireSpread_.getDirectionOfMaxSpread();
    return directionOfMaxSpread;
}

double Surface::getFlameLength() const
{
    return surfaceFireSpread_.getFlameLength();
}

double Surface::getFireLengthToWidthRatio() const
{
    return surfaceFireSpread_.getFireLengthToWidthRatio();
}

double Surface::getFireEccentricity() const
{
    return surfaceFireSpread_.getFireEccentricity();
}

bool Surface::isUsingTwoFuelModels() const
{
    return surfaceInputs_->isUsingTwoFuelModels();
}
