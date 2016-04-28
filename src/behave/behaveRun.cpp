#include "behaveRun.h"

#include "fuelModels.h"

BehaveRun::BehaveRun(FuelModels &fuelModels)
    : surfaceInputs_(),
    surface_(fuelModels, surfaceInputs_)
{
    fuelModels_ = &fuelModels;
}

BehaveRun::BehaveRun(const BehaveRun &rhs)
    : surfaceInputs_(), 
      surface_(*rhs.fuelModels_, surfaceInputs_)
{
    fuelModels_ = rhs.fuelModels_;
    surface_ = rhs.surface_;
    surfaceInputs_ = rhs.surfaceInputs_;
}

BehaveRun& BehaveRun::operator= (const BehaveRun& rhs)
{
    if (this != &rhs)
    {
        fuelModels_ = rhs.fuelModels_;
        surface_ = rhs.surface_;
        surfaceInputs_ = rhs.surfaceInputs_;
    }
    return *this;
}

BehaveRun::~BehaveRun()
{
    // Default Destructor
}

void BehaveRun::setFuelModelNumber(int fuelModelNumber)
{
    surfaceInputs_.setFuelModelNumber(fuelModelNumber);
}

void BehaveRun::setMoistureOneHour(double moistureOneHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;

    surfaceInputs_.setMoistureOneHour(moistureOneHour);
}

void BehaveRun::setMoistureTenHour(double moistureTenHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureTenHour /= 100.0;

    surfaceInputs_.setMoistureTenHour(moistureTenHour);
}

void BehaveRun::setMoistureHundredHour(double moistureHundredHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureHundredHour /= 100.0;

    surfaceInputs_.setMoistureHundredHour(moistureHundredHour);
}

void BehaveRun::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    // Convert moisture input from percent to decimal fraction
    moistureLiveHerbaceous /= 100.0;

    surfaceInputs_.setMoistureLiveHerbaceous(moistureLiveHerbaceous);
}

void BehaveRun::setMoistureLiveWoody(double moistureLiveWoody)
{
    // Convert moisture input from percent to decimal fraction
    moistureLiveWoody /= 100.0;

    surfaceInputs_.setMoistureLiveWoody(moistureLiveWoody);
}

void BehaveRun::setSlope(double slope)
{
    surfaceInputs_.setSlope(slope);
}

void BehaveRun::setSlopeAspect(double slopeAspect)
{
    surfaceInputs_.setSlopeAspect(slopeAspect);
}

void BehaveRun::setWindSpeed(double windSpeed)
{
    surfaceInputs_.setWindSpeed(windSpeed);
}

void BehaveRun::setWindDirection(double windDirection)
{
    surfaceInputs_.setWindDirection(windDirection);
}

void BehaveRun::setWindAndSpreadAngleMode(WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode)
{
    surfaceInputs_.setWindAndSpreadAngleMode(windAndSpreadAngleMode);
}

void BehaveRun::setFirstFuelModelNumber(int firstFuelModelNumber)
{
    surfaceInputs_.setFirstFuelModelNumber(firstFuelModelNumber);
}

void BehaveRun::setSecondFuelModelNumber(int secondFuelModelNumber)
{
    surfaceInputs_.setSecondFuelModelNumber(secondFuelModelNumber);
}

void BehaveRun::setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod)
{
    surfaceInputs_.setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void BehaveRun::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;
    moistureTenHour /= 100.0;
    moistureHundredHour /= 100.0;
    moistureLiveHerbaceous /= 100.0;
    moistureLiveHerbaceous /= 100.0;

    surfaceInputs_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    surfaceInputs_.setTwoFuelModelsMethod(TwoFuelModels::NO_METHOD);
}

void BehaveRun::updateSurfaceInputsWithMoistureByLifeState(int fuelModelNumber, double moistureDead, double moistureLive,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Don't covert moisture input from percent to decimal fraction, this will done in function call below
    updateSurfaceInputs(fuelModelNumber, moistureDead, moistureDead, moistureDead, moistureLive,
        moistureLive, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void  BehaveRun::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
    double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect,
    double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;
    moistureTenHour /= 100.0;
    moistureHundredHour /= 100.0;
    moistureLiveHerbaceous /= 100.0;
    moistureLiveHerbaceous /= 100.0;

    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForTwoFuelModelsWithMoistureByLifeState(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureDead, double moistureLive, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed,
    double windDirection, double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod,
    double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureDead /= 100.0;
    moistureLive /= 100.0;

    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureDead, moistureDead,
        moistureDead, moistureLive, moistureLive, windHeightInputMode, windSpeed, windDirection, firstFuelModelCoverage,
        twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;
    moistureTenHour /= 100.0;
    moistureHundredHour /= 100.0;
    moistureLiveHerbaceous /= 100.0;
    moistureLiveHerbaceous /= 100.0;

    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, ageOfRough, 
        heightOfUnderstory, palmettoCoverage, overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForPalmettoGallberyModelsWithMoistureByLifeState(double moistureDead, double moistureLive,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureDead /= 100.0;
    moistureLive /= 100.0;

    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureDead, moistureDead,
        moistureDead, moistureLive, moistureLive, windHeightInputMode, windSpeed, windDirection, ageOfRough, heightOfUnderstory,
        palmettoCoverage, overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;
    moistureTenHour /= 100.0;
    moistureHundredHour /= 100.0;
    moistureLiveHerbaceous /= 100.0;
    moistureLiveHerbaceous /= 100.0;

    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForWesternAspenWithMoistureByLifeState(int aspenFuelModelNumber, double aspenCuringLevel, 
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureDead, double moistureLive, 
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope, 
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    // Convert moisture input from percent to decimal fraction
    moistureDead /= 100.0;
    moistureLive /= 100.0;

    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureDead,
        moistureDead, moistureDead, moistureLive, moistureLive, windHeightInputMode, windSpeed, windDirection, slope, aspect,
        canopyCover, canopyHeight, crownRatio);
}

double BehaveRun::calculateSurfaceFireForwardSpreadRate(double directionOfInterest)
{
    // Calculate Spread Rate
    const double FEET_PER_MIN_TO_CHAINS_PER_HOUR = 10.0 / 11.0; // conversion factor from ft/min to chains/hr
    double surfaceFireForwardSpreadRate = surface_.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    surfaceFireForwardSpreadRate *= FEET_PER_MIN_TO_CHAINS_PER_HOUR;
    return surfaceFireForwardSpreadRate;
}

void BehaveRun::setWindAndSpreadDirectionMode(WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode)
{
    surfaceInputs_.setWindAndSpreadAngleMode(windAndSpreadAngleMode);
}

void BehaveRun::setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode)
{
    surfaceInputs_.setSlopeInputMode(slopeInputMode);
}

void BehaveRun::setWindAndSpreadAnglesRelativeToNorth()
{
    surfaceInputs_.setWindAndSpreadAngleMode(WindAndSpreadAngleMode::RELATIVE_TO_NORTH);
}

void BehaveRun::setWindAndSpreadAnglesRelativeToUpslope()
{
    surfaceInputs_.setWindAndSpreadAngleMode(WindAndSpreadAngleMode::RELATIVE_TO_UPSLOPE);
}

void BehaveRun::setSlopeInputToPercent()
{
    surfaceInputs_.setSlopeInputMode(SlopeInputMode::SLOPE_IN_PERCENT);
}

void BehaveRun::setSlopeInputToDegrees()
{
    surfaceInputs_.setSlopeInputMode(SlopeInputMode::SLOPE_IN_DEGREES);
}

bool BehaveRun::isWindAndSpreadAngleRelativeToNorth() const
{
    bool isRelativeToNorth = surfaceInputs_.isWindAndSpreadAngleRelativeToNorth();
    return isRelativeToNorth;
}

bool BehaveRun::isWindAndSpreadAngleRelativeToUpslope() const
{
    bool isRelativeToUpslope = surfaceInputs_.isWindAndSpreadAngleRelativeToUpslope();
    return isRelativeToUpslope;
}

bool BehaveRun::isSlopeInDegrees() const
{
    bool isSlopeModeDegrees = surfaceInputs_.isSlopeInDegrees();
    return isSlopeModeDegrees;
}

bool BehaveRun::isSlopeInPercent() const
{
    bool isSlopeInPercent = surfaceInputs_.isSlopeInPercent();
    return isSlopeInPercent;
}

double BehaveRun::getSpreadRate() const
{
    return surface_.getSpreadRate();
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

bool BehaveRun::isFuelModelDefined(int fuelModelNumber) const
{
    return fuelModels_->isFuelModelDefined(fuelModelNumber);
}