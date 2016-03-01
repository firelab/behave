#include "surfaceInputs.h"

SurfaceInputs::SurfaceInputs()
{
    initializeMembers();
}

void SurfaceInputs::initializeMembers()
{
    fuelModelNumber_ = 0;
    secondFuelModelNumber_ = 0;
    moistureOneHour_ = 0.0;
    moistureTenHour_ = 0.0;
    moistureHundredHour_ = 0.0;
    moistureLiveHerbaceous_ = 0.0;
    moistureLiveWoody_ = 0.0;
    slope_ = 0.0;
    aspect_ = 0.0;
    windSpeed_ = 0.0;
    windDirection_ = 0.0;

    isUsingTwoFuelModels_ = false;
    isUsingPalmettoGallberry_ = false;
    isUsingWesternAspen_ = false;

    slopeInputMode_ = SlopeInputMode::SLOPE_IN_PERCENT;
    windAndSpreadAngleMode_ = WindAndSpreadAngleMode::RELATIVE_TO_UPSLOPE;
    windHeightInputMode_ = WindHeightInputMode::DIRECT_MIDFLAME;
    twoFuelModelsMethod_ = TwoFuelModelsMethod::NO_METHOD;

    canopyCover_ = 0.0;
    canopyHeight_ = 0.0;
    crownRatio_ = 0.0;

    aspenFuelModelNumber_ = -1;
    aspenCuringLevel_ = 0.0;

    for (int i = 0; i < MAX_SIZES; i++)
    {
        moistureDead_[i] = 0.0;
        moistureLive_[i] = 0.0;
    }

    userProvidedWindAdjustmentFactor_ = -1.0;
}

void SurfaceInputs::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    initializeMembers();

    for (int i = 0; i < MAX_SIZES; i++)
    {
        moistureDead_[i] = 0.0;
        moistureLive_[i] = 0.0;
    }

    setSlope(slope);
    aspect_ = aspect;

    //	To Fix Original BehavePlus's occasional reporting 360 as direction of max 
    //	spread, just add an equaul sign after the less than or greater than sign 
    //	in the next 2 conditional statements 
    if (windDirection < 0.0)
    {
        windDirection += 360.0;
    }
    while (windDirection > 360.0)
    {
        windDirection -= 360.0;
    }

    if (windAndSpreadAngleMode_ == WindAndSpreadAngleMode::RELATIVE_TO_NORTH)
    {
        windDirection = convertWindToUpslope(windDirection);
    }

    fuelModelNumber_ = fuelModelNumber;
    moistureOneHour_ = moistureOneHour;
    moistureTenHour_ = moistureTenHour;
    moistureHundredHour_ = moistureHundredHour;
    moistureLiveHerbaceous_ = moistureLiveHerbaceous;
    moistureLiveWoody_ = moistureLiveWoody;

    windHeightInputMode_ = windHeightInputMode;
    windSpeed_ = windSpeed;
    windDirection_ = windDirection;

    setMoistureDead();
    setMoistureLive();

    isUsingTwoFuelModels_ = false;
    twoFuelModelsMethod_ = TwoFuelModelsMethod::NO_METHOD;

    isUsingPalmettoGallberry_ = false;

    isUsingWesternAspen_ = false;
   
    canopyCover_ = canopyCover;
    canopyHeight_ = canopyHeight;
    crownRatio_ = crownRatio;
}

void  SurfaceInputs::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous,
    double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed,
    double windDirection, double firstFuelModelCoverage, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
    double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    int fuelModelNumber = firstfuelModelNumber;
    updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody,
        windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    firstFuelModelNumber_ = firstfuelModelNumber;
    secondFuelModelNumber_ = secondFuelModelNumber;
    firstFuelModelCoverage_ = firstFuelModelCoverage;

    isUsingTwoFuelModels_ = true;

    twoFuelModelsMethod_ = twoFuelModelsMethod;
}

void  SurfaceInputs::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
    double ageOfRough, double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    updateSurfaceInputs(0, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    ageOfRough_ = ageOfRough;
    heightOfUnderstory_ = heightOfUnderstory;
    palmettoCoverage_ = palmettoCoverage;
    overstoryBasalArea_ = overstoryBasalArea;

    isUsingPalmettoGallberry_ = true;
}

void SurfaceInputs::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    updateSurfaceInputs(0, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    aspenFuelModelNumber_ = aspenFuelModelNumber;
    aspenCuringLevel_ = aspenCuringLevel;
    aspenFireSeverity_ = aspenFireSeverity;
    isUsingWesternAspen_ = true;
}

void SurfaceInputs::setWindAndSpreadAngleMode(WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode)
{
    windAndSpreadAngleMode_ = windAndSpreadAngleMode;
}

double SurfaceInputs::convertWindToUpslope(double windDirectionFromNorth)
{
    // Important information: 
    // when wind is given relative to upslope, it is given as the direction the wind pushes the fire, not the direction from which is blowing - WMC 01/2016
    double windDirectionFromUpslope = windDirectionFromNorth - aspect_; // wind direction is now in degrees clockwise relative to blowing in the upslope direction
    return windDirectionFromUpslope;
}

void SurfaceInputs::setFuelModelNumber(int fuelModelNumber)
{
    fuelModelNumber_ = fuelModelNumber;
}

void SurfaceInputs::setMoistureDead()
{
    moistureDead_[0] = moistureOneHour_;
    moistureDead_[1] = moistureTenHour_;
    moistureDead_[2] = moistureHundredHour_;
    moistureDead_[3] = moistureOneHour_;
}

void SurfaceInputs::setMoistureLive()
{
    moistureLive_[0] = moistureLiveHerbaceous_;
    moistureLive_[1] = moistureLiveWoody_;
}

void SurfaceInputs::setMoistureDead(double moistureOneHour, double moistureTenHour, double moistureHundredHour)
{
    moistureDead_[0] = moistureOneHour;
    moistureDead_[1] = moistureTenHour;
    moistureDead_[2] = moistureHundredHour;
    moistureDead_[3] = moistureOneHour;
}

void SurfaceInputs::setMoistureLive(double moistureLiveHerbaceous, double moistureLiveWoody)
{
    moistureLive_[0] = moistureLiveHerbaceous;
    moistureLive_[1] = moistureLiveWoody;
}

void SurfaceInputs::setMoistureOneHour(double moistureOneHour)
{
    moistureDead_[0] = moistureOneHour;
    moistureDead_[3] = moistureOneHour;
}

void SurfaceInputs::setMoistureTenHour(double moistureTenHour)
{
    moistureDead_[1] = moistureTenHour;
}

void SurfaceInputs::setMoistureHundredHour(double moistureHundredHour)
{
    moistureDead_[2] = moistureHundredHour;
}

void SurfaceInputs::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    moistureLive_[0] = moistureLiveHerbaceous_;
}

void SurfaceInputs::setMoistureLiveWoody(double moistureLiveWoody)
{
    moistureLive_[1] = moistureLiveWoody_;
}

void SurfaceInputs::setSlope(double slope)
{
    if (slopeInputMode_ == SlopeInputMode::SLOPE_IN_PERCENT)
    {
        const double PI = 3.141592653589793238463;
        slope = (180 / PI) * atan(slope / 100); // slope is now in degees
    }
    slope_ = slope;
}

void SurfaceInputs::setSlopeAspect(double aspect)
{
    aspect_ = aspect;
}

void SurfaceInputs::setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode)
{
    slopeInputMode_ = slopeInputMode;
}

void SurfaceInputs::setTwoFuelModelsMethod(TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod)
{
    twoFuelModelsMethod_ = twoFuelModelsMethod;
}

void  SurfaceInputs::setWindSpeed(double windSpeed)
{
    windSpeed_ = windSpeed;
}

void  SurfaceInputs::setFirstFuelModelNumber(int firstFuelModelNumber)
{
    firstFuelModelNumber_ = firstFuelModelNumber;
}

int  SurfaceInputs::getFirstFuelModelNumber() const
{
    return firstFuelModelNumber_;
}

int  SurfaceInputs::getSecondFuelModelNumber() const
{
    return secondFuelModelNumber_;
}

void  SurfaceInputs::setSecondFuelModelNumber(int secondFuelModelNumber)
{
    secondFuelModelNumber_ = secondFuelModelNumber;
}

double SurfaceInputs::getMoistureDeadAtIndex(int index) const
{
    return moistureDead_[index];
}
double SurfaceInputs::getMoistureLiveAtIndex(int index) const
{
    return moistureLive_[index];
}

int SurfaceInputs::getFuelModelNumber() const
{
    return fuelModelNumber_;
}

double SurfaceInputs::getSlope() const
{
    return slope_;
}

double SurfaceInputs::getAspect() const
{
    return aspect_;
}

double SurfaceInputs::getFirstFuelModelCoverage() const
{
    return firstFuelModelCoverage_;
}

TwoFuelModelsMethod::TwoFuelModelsMethodEnum SurfaceInputs::getTwoFuelModelsMethod() const
{
    return twoFuelModelsMethod_;
}

bool SurfaceInputs::isUsingTwoFuelModels() const
{
    return isUsingTwoFuelModels_;
}

bool SurfaceInputs::isUsingPalmettoGallberry() const
{
    return isUsingPalmettoGallberry_;
}

bool SurfaceInputs::hasUserEnteredWindAdjustmentFactor() const
{
    bool userHasEnteredWindAdjustmentFactor = false;

    if (userProvidedWindAdjustmentFactor_ != -1.0)
    {
        userHasEnteredWindAdjustmentFactor = true;
    }
    return userHasEnteredWindAdjustmentFactor;
}

WindHeightInputMode::WindHeightInputModeEnum SurfaceInputs::getWindHeightInputMode() const
{
    return windHeightInputMode_;
}

bool SurfaceInputs::isWindAndSpreadAngleRelativeToNorth() const
{
    bool isRelativeToNorth = (windAndSpreadAngleMode_ == WindAndSpreadAngleMode::RELATIVE_TO_NORTH);
    return isRelativeToNorth;
}

bool SurfaceInputs::isWindAndSpreadAngleRelativeToUpslope() const
{
    bool isRelativeToUpslope = (windAndSpreadAngleMode_ == WindAndSpreadAngleMode::RELATIVE_TO_UPSLOPE);
    return isRelativeToUpslope;
}

bool SurfaceInputs::isSlopeInDegrees() const
{
    bool isSlopeModeDegrees = (slopeInputMode_ == SlopeInputMode::SLOPE_IN_DEGREES);
    return isSlopeModeDegrees;
}

bool SurfaceInputs::isSlopeInPercent() const
{
    bool isSlopeInPercent = (slopeInputMode_ == SlopeInputMode::SLOPE_IN_PERCENT);
    return isSlopeInPercent;
}

double SurfaceInputs::getWindDirection() const
{
    return windDirection_;
}

double SurfaceInputs::getWindSpeed() const
{
    return windSpeed_;
}

double SurfaceInputs::getMoistureOneHour() const
{
    return moistureOneHour_;
}

double SurfaceInputs::getMoistureTenHour() const
{
    return moistureTenHour_;
}

double SurfaceInputs::getMoistureHundredHour() const
{
    return moistureHundredHour_;
}

double SurfaceInputs::getMoistureLiveHerbaceous() const
{
    return moistureLiveHerbaceous_;
}

double SurfaceInputs::getMoistureLiveWoody() const
{
    return moistureLiveWoody_;
}

void SurfaceInputs::setAgeOfRough(double ageOfRough)
{
    ageOfRough_ = ageOfRough;
}

double SurfaceInputs::getAgeOfRough() const
{
    return ageOfRough_;
}

void SurfaceInputs::setHeightOfUnderstory(double heightOfUnderstory)
{
    heightOfUnderstory_ = heightOfUnderstory;
}

double SurfaceInputs::getHeightOfUnderstory() const
{
    return heightOfUnderstory_;
}

void SurfaceInputs::setPalmettoCoverage(double palmettoCoverage)
{
    palmettoCoverage_ = palmettoCoverage;
}

double SurfaceInputs::getPalmettoCoverage() const
{
    return palmettoCoverage_;
}

void SurfaceInputs::setOverstoryBasalArea(double overstoryBasalArea)
{
    overstoryBasalArea_ = overstoryBasalArea;
}

double SurfaceInputs::getOverstoryBasalArea() const
{
    return overstoryBasalArea_;
}

double SurfaceInputs::getCanopyCover() const
{
    return canopyCover_;
}

double SurfaceInputs::getCanopyHeight() const
{
    return canopyHeight_;
}

double SurfaceInputs::getCrownRatio() const
{
    return crownRatio_;
}

bool SurfaceInputs::isUsingWesternAspen() const
{
    return isUsingWesternAspen_;
}

int SurfaceInputs::getAspenFuelModelNumber() const
{
    return aspenFuelModelNumber_;
}

double SurfaceInputs::getAspenCuringLevel() const
{
    return aspenCuringLevel_;
}

void SurfaceInputs::setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor)
{
    userProvidedWindAdjustmentFactor_ = userProvidedWindAdjustmentFactor;
}

double SurfaceInputs::getUserProvidedWindAdjustmentFactor() const
{
    return userProvidedWindAdjustmentFactor_;
}
