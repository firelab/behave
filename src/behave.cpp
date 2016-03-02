#include "behave.h"

Behave::Behave()
    : surfaceInputs_{}, surface_{ fuelModels_, surfaceInputs_ }
{
    // Default Constructor
}

Behave::Behave(SurfaceInputs &surfaceInputs)
    : surface_{ fuelModels_, surfaceInputs }
{
    // Constructor taking a SurfaceInputs object as a parameter
}

Behave::~Behave()
{
    // Default Destructor
}

void Behave::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    surfaceInputs_.setTwoFuelModelsMethod(TwoFuelModelsMethod::NO_METHOD);
}

void Behave::updateSurfaceInputsWithMoistureByLifeState(int fuelModelNumber, double moistureDead, double moistureLive,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    updateSurfaceInputs(fuelModelNumber, moistureDead, moistureDead, moistureDead, moistureLive,
        moistureLive, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void  Behave::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
    double firstFuelModelCoverage, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod, double slope, double aspect, 
    double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Behave::updateSurfaceInputsForTwoFuelModelsWithMoistureByLifeState(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureDead, double moistureLive, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed,
    double windDirection, double firstFuelModelCoverage, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
    double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureDead, moistureDead,
        moistureDead, moistureLive, moistureLive, windHeightInputMode, windSpeed, windDirection, firstFuelModelCoverage,
        twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Behave::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, ageOfRough, 
        heightOfUnderstory, palmettoCoverage, overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Behave::updateSurfaceInputsForPalmettoGallberyModelsWithMoistureByLifeState(double moistureDead, double moistureLive,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureDead, moistureDead,
        moistureDead, moistureLive, moistureLive, windHeightInputMode, windSpeed, windDirection, ageOfRough, heightOfUnderstory,
        palmettoCoverage, overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Behave::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void Behave::updateSurfaceInputsForWesternAspenWithMoistureByLifeState(int aspenFuelModelNumber, double aspenCuringLevel, 
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureDead, double moistureLive, 
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope, 
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureDead,
        moistureDead, moistureDead, moistureLive, moistureLive, windHeightInputMode, windSpeed, windDirection, slope, aspect,
        canopyCover, canopyHeight, crownRatio);
}

double Behave::calculateSurfaceFireForwardSpreadRate(double directionOfInterest)
{
    // Calculate Spread Rate
    return surface_.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
}

void Behave::setWindAndSpreadDirectionMode(WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode)
{
    surfaceInputs_.setWindAndSpreadAngleMode(windAndSpreadAngleMode);
}

void Behave::setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode)
{
    surfaceInputs_.setSlopeInputMode(slopeInputMode);
}

void Behave::setWindAndSpreadAnglesRelativeToNorth()
{
    surfaceInputs_.setWindAndSpreadAngleMode(WindAndSpreadAngleMode::RELATIVE_TO_NORTH);
}

void Behave::setWindAndSpreadAnglesRelativeToUpslope()
{
    surfaceInputs_.setWindAndSpreadAngleMode(WindAndSpreadAngleMode::RELATIVE_TO_UPSLOPE);
}

void Behave::setSlopeInputToPercent()
{
    surfaceInputs_.setSlopeInputMode(SlopeInputMode::SLOPE_IN_PERCENT);
}

void Behave::setSlopeInputToDegrees()
{
    surfaceInputs_.setSlopeInputMode(SlopeInputMode::SLOPE_IN_DEGREES);
}

bool Behave::isWindAndSpreadAngleRelativeToNorth() const
{
    bool isRelativeToNorth = surfaceInputs_.isWindAndSpreadAngleRelativeToNorth();
    return isRelativeToNorth;
}

bool Behave::isWindAndSpreadAngleRelativeToUpslope() const
{
    bool isRelativeToUpslope = surfaceInputs_.isWindAndSpreadAngleRelativeToUpslope();
    return isRelativeToUpslope;
}

bool Behave::isSlopeInDegrees() const
{
    bool isSlopeModeDegrees = surfaceInputs_.isSlopeInDegrees();
    return isSlopeModeDegrees;
}

bool Behave::isSlopeInPercent() const
{
    bool isSlopeInPercent = surfaceInputs_.isSlopeInPercent();
    return isSlopeInPercent;
}

double Behave::getDirectionOfMaxSpread() const
{
    return surface_.getDirectionOfMaxSpread();
}

double Behave::getFlameLength() const
{
    return surface_.getFlameLength();
}

double Behave::getFireLengthToWidthRatio() const
{
    return surface_.getFireLengthToWidthRatio();
}

double Behave::getFireEccentricity() const
{
    return surface_.getFireEccentricity();
}

bool Behave::isFuelModelDefined(int fuelModelNumber) const
{
    return fuelModels_.isFuelModelDefined(fuelModelNumber);
}
