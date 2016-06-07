#include "surface.h"

#include "surfaceTwoFuelModels.h"
#include "surfaceInputs.h"

Surface::Surface(const FuelModelSet& fuelModels)
    : surfaceInputs_(),
    surfaceFireSpread_(fuelModels, surfaceInputs_)
{
    fuelModelSet_ = &fuelModels;
}

// Copy Ctor
Surface::Surface(const Surface &rhs)
    : surfaceFireSpread_()
{
    surfaceInputs_ = rhs.surfaceInputs_;
    surfaceFireSpread_ = rhs.surfaceFireSpread_;
}

Surface& Surface::operator= (const Surface& rhs)
{
    if (this != &rhs)
    {
        surfaceInputs_ = rhs.surfaceInputs_;
        surfaceFireSpread_ = rhs.surfaceFireSpread_;
    }
    return *this;
}

void Surface::doSurfaceRun(double directionOfInterest)
{
    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(surfaceInputs_, surfaceFireSpread_);
        surfaceTwoFuelModels.calculateWeightedSpreadRate(directionOfInterest);
    }
    else // Use only one fuel model
    {
        // Calculate spread rate
        surfaceFireSpread_.calculateForwardSpreadRate(directionOfInterest);
    }
}

double Surface::calculateFlameLength(double firelineIntensity)
{
    return surfaceFireSpread_.calculateFlameLength(firelineIntensity);
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
    return surfaceInputs_.isUsingTwoFuelModels();
}

double Surface::getCanopyHeight() const
{
    return surfaceInputs_.getCanopyHeight();
}

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum Surface::getWindAndSpreadOrientationMode() const
{
    return surfaceInputs_.getWindAndSpreadOrientationMode();
}

WindHeightInputMode::WindHeightInputModeEnum Surface::getWindHeightInputMode() const
{
    return surfaceInputs_.getWindHeightInputMode();
}

SlopeInputMode::SlopeInputModeEnum Surface::getSlopeInputMode() const
{
    return surfaceInputs_.getSlopeInputMode();
}

double Surface::getWindSpeed() const
{
    return surfaceInputs_.getWindSpeed();
}

void Surface::setFuelModelNumber(int fuelModelNumber)
{
    surfaceInputs_.setFuelModelNumber(fuelModelNumber);
}

void Surface::setMoistureOneHour(double moistureOneHour)
{
    surfaceInputs_.setMoistureOneHour(moistureOneHour);
}

void Surface::setMoistureTenHour(double moistureTenHour)
{
    surfaceInputs_.setMoistureTenHour(moistureTenHour);
}

void Surface::setMoistureHundredHour(double moistureHundredHour)
{
    surfaceInputs_.setMoistureHundredHour(moistureHundredHour);
}

void Surface::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    surfaceInputs_.setMoistureLiveHerbaceous(moistureLiveHerbaceous);
}

void Surface::setMoistureLiveWoody(double moistureLiveWoody)
{
    surfaceInputs_.setMoistureLiveWoody(moistureLiveWoody);
}

void Surface::setSlope(double slope)
{
    surfaceInputs_.setSlope(slope);
}

void Surface::setAspect(double aspect)
{
    surfaceInputs_.setAspect(aspect);
}

void Surface::setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode)
{
    surfaceInputs_.setSlopeInputMode(slopeInputMode);
}

void Surface::setWindSpeed(double windSpeed)
{
    surfaceInputs_.setWindSpeed(windSpeed);
}

void Surface::setWindDirection(double windDirection)
{
    surfaceInputs_.setWindDirection(windDirection);
}

void Surface::setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode)
{
    surfaceInputs_.setWindAndSpreadOrientationMode(windAndSpreadOrientationMode);
}

void Surface::setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surfaceInputs_.setWindHeightInputMode(windHeightInputMode);
}

void Surface::setFirstFuelModelNumber(int firstFuelModelNumber)
{
    surfaceInputs_.setFirstFuelModelNumber(firstFuelModelNumber);
}

void Surface::setSecondFuelModelNumber(int secondFuelModelNumber)
{
    surfaceInputs_.setSecondFuelModelNumber(secondFuelModelNumber);
}

void Surface::setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod)
{
    surfaceInputs_.setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void Surface::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Surface::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
    double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
    double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect,
    double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Surface::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, ageOfRough, heightOfUnderstory, palmettoCoverage,
        overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Surface::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        slope, aspect, canopyCover, canopyHeight, crownRatio);
}
