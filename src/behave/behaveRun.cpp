#include "behaveRun.h"

#include "fuelModelSet.h"

BehaveRun::BehaveRun(FuelModelSet &fuelModelSet)
    : surface_(fuelModelSet),
    crown_(fuelModelSet, surface_)
{
    fuelModelSet_ = &fuelModelSet;
}

BehaveRun::BehaveRun(const BehaveRun &rhs)
    : surface_(*rhs.fuelModelSet_),
    crown_(*rhs.fuelModelSet_, surface_)
{
    fuelModelSet_ = rhs.fuelModelSet_;
    surface_ = rhs.surface_;
    crown_ = rhs.crown_;
}

BehaveRun& BehaveRun::operator= (const BehaveRun& rhs)
{
    if (this != &rhs)
    {
        fuelModelSet_ = rhs.fuelModelSet_;
        surface_ = rhs.surface_;
        crown_ = rhs.crown_;
    }
    return *this;
}

BehaveRun::~BehaveRun()
{
    // Default Destructor
}

// setFuelModelSet(const FuelModelSet& fuelModelSet) makes this behaveRun's pointer
//  fuelModelSet_ point to the FuelModelSet given to this method as a parameter   
void BehaveRun::setFuelModelSet(FuelModelSet& fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet;
}

void BehaveRun::setFuelModelNumber(int fuelModelNumber)
{
    surface_.setFuelModelNumber(fuelModelNumber);
}

void BehaveRun::setMoistureOneHour(double moistureOneHour)
{
    surface_.setMoistureOneHour(moistureOneHour);
}

void BehaveRun::setMoistureTenHour(double moistureTenHour)
{
    surface_.setMoistureTenHour(moistureTenHour);
}

void BehaveRun::setMoistureHundredHour(double moistureHundredHour)
{
    surface_.setMoistureHundredHour(moistureHundredHour);
}

void BehaveRun::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    surface_.setMoistureLiveHerbaceous(moistureLiveHerbaceous);
}

void BehaveRun::setMoistureLiveWoody(double moistureLiveWoody)
{
    surface_.setMoistureLiveWoody(moistureLiveWoody);
}

void BehaveRun::setSlope(double slope)
{
    surface_.setSlope(slope);
}

void BehaveRun::setAspect(double aspect)
{
    surface_.setAspect(aspect);
}

void BehaveRun::setWindSpeed(double windSpeed)
{
    surface_.setWindSpeed(windSpeed);
}

void BehaveRun::setWindDirection(double windDirection)
{
    surface_.setWindDirection(windDirection);
}

void BehaveRun::setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surface_.setWindHeightInputMode(windHeightInputMode);
}

void BehaveRun::setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode)
{
    surface_.setWindAndSpreadOrientationMode(windAndSpreadOrientationMode);
}

void BehaveRun::setFirstFuelModelNumber(int firstFuelModelNumber)
{
    surface_.setFirstFuelModelNumber(firstFuelModelNumber);
}

void BehaveRun::setSecondFuelModelNumber(int secondFuelModelNumber)
{
    surface_.setSecondFuelModelNumber(secondFuelModelNumber);
}

void BehaveRun::setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod)
{
    surface_.setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void BehaveRun::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surface_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    surface_.setTwoFuelModelsMethod(TwoFuelModels::NO_METHOD);
}

void  BehaveRun::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
    double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect,
    double canopyCover, double canopyHeight, double crownRatio)
{
    surface_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surface_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, 
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, ageOfRough, heightOfUnderstory, palmettoCoverage, 
        overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surface_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::doSurfaceRun(double directionOfInterest)
{
    // Calculate SURFACE Module outputs
    surface_.doSurfaceRun(directionOfInterest);
}

void BehaveRun::setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode)
{
    surface_.setSlopeInputMode(slopeInputMode);
}

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum BehaveRun::getWindAndSpreadOrientationMode() const
{
    return surface_.getWindAndSpreadOrientationMode();
}

WindHeightInputMode::WindHeightInputModeEnum BehaveRun::getWindHeightInputMode() const
{
    return surface_.getWindHeightInputMode();
}

SlopeInputMode::SlopeInputModeEnum BehaveRun::getSlopeInputMode() const
{
    return surface_.getSlopeInputMode();
}

double BehaveRun::getSurfaceFireSpreadRate() const
{
    const double FEET_PER_MIN_TO_CHAINS_PER_HOUR = 10.0 / 11.0; // conversion factor from ft/min to chains/hr
    double surfaceFireForwardSpreadRate = surface_.getSpreadRate();
    return surfaceFireForwardSpreadRate *= FEET_PER_MIN_TO_CHAINS_PER_HOUR;
}

double BehaveRun::getDirectionOfMaxSpread() const
{
    return surface_.getDirectionOfMaxSpread();
}

double BehaveRun::getFlameLength() const
{
    return surface_.getFlameLength();
}

double BehaveRun::getFireLengthToWidthRatio() const
{
    return surface_.getFireLengthToWidthRatio();
}

double BehaveRun::getFireEccentricity() const
{
    return surface_.getFireEccentricity();
}

double BehaveRun::getSurfaceFirelineIntensity() const
{
    return surface_.getFirelineIntensity();
}

double BehaveRun::getMidflameWindspeed() const
{
    return surface_.getMidflameWindspeed();
}

double BehaveRun::getEllipticalA() const
{
    return surface_.getEllipticalA();
}

double BehaveRun::getEllipticalB() const
{
    return surface_.getEllipticalB();
}

double BehaveRun::getEllipticalC() const
{
    return surface_.getEllipticalC();
}

bool BehaveRun::isFuelModelDefined(int fuelModelNumber) const
{
    return fuelModelSet_->isFuelModelDefined(fuelModelNumber);
}

void BehaveRun::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    crown_.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
}

void BehaveRun::doCrownRun(double directionOfInterest)
{
    // Calculate Crown module outputs
    crown_.doCrownRun();
}

double BehaveRun::getCrownFireSpreadRate() const
{
    const double FEET_PER_MIN_TO_CHAINS_PER_HOUR = 10.0 / 11.0; // conversion factor from ft/min to chains/hr
    return crown_.getCrownFireSpreadRate() * FEET_PER_MIN_TO_CHAINS_PER_HOUR;
}
