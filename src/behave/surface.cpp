#include "surface.h"

#include "surfaceTwoFuelModels.h"
#include "surfaceInputs.h"

Surface::Surface(const FuelModelSet& fuelModels, SurfaceInputs& surfaceInputs) 
    : surfaceFireSpread_(fuelModels, surfaceInputs)
{
    fuelModelSet_ = &fuelModels;
    surfaceInputs_ = &surfaceInputs;
}

// Copy Ctor
Surface::Surface(const Surface &rhs)
    : surfaceFireSpread_()
{
    surfaceFireSpread_ = rhs.surfaceFireSpread_;
}

Surface& Surface::operator= (const Surface& rhs)
{
    if (this != &rhs)
    {
        surfaceFireSpread_ = rhs.surfaceFireSpread_;
    }
    return *this;
}

double Surface::calculateSurfaceFireForwardSpreadRate(double directionOfInterest)
{
    double spreadRate = 0.0;

    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(*surfaceInputs_, surfaceFireSpread_);
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

double Surface::getSpreadRate() const
{
    return surfaceFireSpread_.getSpreadRate();
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

double Surface::getFirelineIntensity() const
{
    return surfaceFireSpread_.getFirelineIntensity();
}

double Surface::getHeatPerUnitArea() const
{
    return surfaceFireSpread_.getHeatPerUnitArea();
}

double Surface::getMidflameWindspeed() const
{
    return surfaceFireSpread_.getMidflameWindSpeed();
}

double Surface::getEllipticalA() const
{
    return surfaceFireSpread_.getEllipticalA();
}

double Surface::getEllipticalB() const
{
    return surfaceFireSpread_.getEllipticalB();
}

double Surface::getEllipticalC() const
{
    return surfaceFireSpread_.getEllipticalC();
}

bool Surface::isUsingTwoFuelModels() const
{
    return surfaceInputs_->isUsingTwoFuelModels();
}
