#include "surfaceInputs.h"

#include <cmath>

// Default Ctor
SurfaceInputs::SurfaceInputs()
{
    initializeMembers();
}

// Copy Ctor
SurfaceInputs::SurfaceInputs(const SurfaceInputs &rhs)
{
    fuelModelNumber_ = rhs.fuelModelNumber_;
    secondFuelModelNumber_ = rhs.secondFuelModelNumber_;
    moistureOneHour_ = rhs.moistureOneHour_;
    moistureTenHour_ = rhs.moistureTenHour_;
    moistureHundredHour_ = rhs.moistureHundredHour_;
    moistureLiveHerbaceous_ = rhs.moistureLiveHerbaceous_;
    moistureLiveWoody_ = rhs.moistureLiveWoody_;
    slope_ = rhs.slope_;
    aspect_ = rhs.aspect_;
    windSpeed_ = rhs.windSpeed_;
    windDirection_ = rhs.windDirection_;

    isUsingTwoFuelModels_ = rhs.isUsingTwoFuelModels_;
    isUsingPalmettoGallberry_ = rhs.isUsingPalmettoGallberry_;
    isUsingWesternAspen_ = rhs.isUsingWesternAspen_;

    slopeInputMode_ = rhs.slopeInputMode_;
    windAndSpreadOrientationMode_ = rhs.windAndSpreadOrientationMode_;
    windHeightInputMode_ = rhs.windHeightInputMode_;
    twoFuelModelsMethod_ = rhs.twoFuelModelsMethod_;

    firstFuelModelCoverage_ = 0.0;

    ageOfRough_ = rhs.ageOfRough_;
    heightOfUnderstory_ = rhs.heightOfUnderstory_;
    palmettoCoverage_ = rhs.palmettoCoverage_;
    overstoryBasalArea_ = rhs.overstoryBasalArea_;

    canopyCover_ = rhs.canopyCover_;
    canopyHeight_ = rhs.canopyHeight_;
    crownRatio_ = rhs.crownRatio_;

    aspenFuelModelNumber_ = rhs.aspenFuelModelNumber_;
    aspenCuringLevel_ = rhs.aspenCuringLevel_;
    aspenFireSeverity_ = rhs.aspenFireSeverity_;

    userProvidedWindAdjustmentFactor_ = -rhs.userProvidedWindAdjustmentFactor_;
}

SurfaceInputs& SurfaceInputs::operator= (const SurfaceInputs& rhs)
{
    if (this != &rhs)
    {
        fuelModelNumber_ = rhs.fuelModelNumber_;
        secondFuelModelNumber_ = rhs.secondFuelModelNumber_;
        moistureOneHour_ = rhs.moistureOneHour_;
        moistureTenHour_ = rhs.moistureTenHour_;
        moistureHundredHour_ = rhs.moistureHundredHour_;
        moistureLiveHerbaceous_ = rhs.moistureLiveHerbaceous_;
        moistureLiveWoody_ = rhs.moistureLiveWoody_;
        slope_ = rhs.slope_;
        aspect_ = rhs.aspect_;
        windSpeed_ = rhs.windSpeed_;
        windDirection_ = rhs.windDirection_;

        isUsingTwoFuelModels_ = rhs.isUsingTwoFuelModels_;
        isUsingPalmettoGallberry_ = rhs.isUsingPalmettoGallberry_;
        isUsingWesternAspen_ = rhs.isUsingWesternAspen_;

        slopeInputMode_ = rhs.slopeInputMode_;
        windAndSpreadOrientationMode_ = rhs.windAndSpreadOrientationMode_;
        windHeightInputMode_ = rhs.windHeightInputMode_;
        twoFuelModelsMethod_ = rhs.twoFuelModelsMethod_;

        firstFuelModelCoverage_ = 0.0;

        ageOfRough_ = rhs.ageOfRough_;
        heightOfUnderstory_ = rhs.heightOfUnderstory_;
        palmettoCoverage_ = rhs.palmettoCoverage_;
        overstoryBasalArea_ = rhs.overstoryBasalArea_;

        canopyCover_ = rhs.canopyCover_;
        canopyHeight_ = rhs.canopyHeight_;
        crownRatio_ = rhs.crownRatio_;

        aspenFuelModelNumber_ = rhs.aspenFuelModelNumber_;
        aspenCuringLevel_ = rhs.aspenCuringLevel_;
        aspenFireSeverity_ = rhs.aspenFireSeverity_;

        userProvidedWindAdjustmentFactor_ = -rhs.userProvidedWindAdjustmentFactor_;
    }
    return *this;
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
    windAndSpreadOrientationMode_ = WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE;
    windHeightInputMode_ = WindHeightInputMode::DIRECT_MIDFLAME;
    twoFuelModelsMethod_ = TwoFuelModels::NO_METHOD;

    firstFuelModelCoverage_ = 0.0;

    ageOfRough_ = 0.0;
    heightOfUnderstory_ = 0.0;
    palmettoCoverage_ = 0.0;
    overstoryBasalArea_ = 0.0;

    canopyCover_ = 0.0;
    canopyHeight_ = 0.0;
    crownRatio_ = 0.0;

    aspenFuelModelNumber_ = -1;
    aspenCuringLevel_ = 0.0;
    aspenFireSeverity_ = AspenFireSeverity::LOW;

    userProvidedWindAdjustmentFactor_ = -1.0;
}

void SurfaceInputs::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    //initializeMembers();

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

    setWindDirection(windDirection);

    fuelModelNumber_ = fuelModelNumber;

    setMoistureOneHour(moistureOneHour);
    setMoistureTenHour(moistureTenHour);
    setMoistureHundredHour(moistureHundredHour);
    setMoistureLiveHerbaceous(moistureLiveHerbaceous);
    setMoistureLiveWoody(moistureLiveWoody);

    windHeightInputMode_ = windHeightInputMode;
    windSpeed_ = windSpeed;
    windDirection_ = windDirection;

    isUsingTwoFuelModels_ = false;
    twoFuelModelsMethod_ = TwoFuelModels::NO_METHOD;
   
    isUsingPalmettoGallberry_ = false;
    isUsingWesternAspen_ = false;

    canopyCover_ = canopyCover;
    canopyHeight_ = canopyHeight;
    crownRatio_ = crownRatio;
}

void  SurfaceInputs::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous,
    double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed,
    double windDirection, double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod,
    double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    int fuelModelNumber = firstfuelModelNumber;
    updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody,
        windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //firstFuelModelNumber_ = firstfuelModelNumber;
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

void SurfaceInputs::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope, double aspect,
    double canopyCover, double canopyHeight, double crownRatio)
{
    updateSurfaceInputs(0, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    aspenFuelModelNumber_ = aspenFuelModelNumber;
    aspenCuringLevel_ = aspenCuringLevel;
    aspenFireSeverity_ = aspenFireSeverity;
    isUsingWesternAspen_ = true;
    DBH_ = DBH;
}

void SurfaceInputs::setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode)
{
    windAndSpreadOrientationMode_ = windAndSpreadOrientationMode;
}

void SurfaceInputs::setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    windHeightInputMode_ = windHeightInputMode;
}

double SurfaceInputs::convertWindToUpslope(double windDirectionFromNorth)
{
    // Important information: 
    // when wind is given relative to upslope, it is given as the direction the wind pushes the fire, 
    // not the direction from which is blowing - WMC 01/2016
    double windDirectionFromUpslope = windDirectionFromNorth - aspect_; // wind direction is now in degrees 
    //clockwise relative to blowing in the upslope direction
    return windDirectionFromUpslope;
}

void SurfaceInputs::setFuelModelNumber(int fuelModelNumber)
{
    fuelModelNumber_ = fuelModelNumber;
}

void SurfaceInputs::setMoistureOneHour(double moistureOneHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;
    moistureOneHour_ = moistureOneHour;
}

void SurfaceInputs::setMoistureTenHour(double moistureTenHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureTenHour /= 100.0;
    moistureTenHour_ = moistureTenHour;
}

void SurfaceInputs::setMoistureHundredHour(double moistureHundredHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureHundredHour /= 100.0;
    moistureHundredHour_ = moistureHundredHour;
}

void SurfaceInputs::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    // Convert moisture input from percent to decimal fraction
    moistureLiveHerbaceous /= 100.0;
    moistureLiveHerbaceous_ = moistureLiveHerbaceous;
}

void SurfaceInputs::setMoistureLiveWoody(double moistureLiveWoody)
{
    // Convert moisture input from percent to decimal fraction
    moistureLiveWoody /= 100.0;
    moistureLiveWoody_ = moistureLiveWoody;
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

void SurfaceInputs::setAspect(double aspect)
{
    aspect_ = aspect;
}

void SurfaceInputs::setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode)
{
    slopeInputMode_ = slopeInputMode;
}

void SurfaceInputs::setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod)
{
    twoFuelModelsMethod_ = twoFuelModelsMethod;
}

void  SurfaceInputs::setWindSpeed(double windSpeed)
{
    windSpeed_ = windSpeed;
}

void  SurfaceInputs::setWindDirection(double windDirection)
{
    if (windAndSpreadOrientationMode_ == WindAndSpreadOrientationMode::RELATIVE_TO_NORTH)
    {
        windDirection = convertWindToUpslope(windDirection);
    }
    windDirection_ = windDirection;
}

void  SurfaceInputs::setFirstFuelModelNumber(int firstFuelModelNumber)
{
    fuelModelNumber_ = firstFuelModelNumber;
}

int  SurfaceInputs::getFirstFuelModelNumber() const
{
    return fuelModelNumber_;
}

int  SurfaceInputs::getSecondFuelModelNumber() const
{
    return secondFuelModelNumber_;
}

void  SurfaceInputs::setSecondFuelModelNumber(int secondFuelModelNumber)
{
    secondFuelModelNumber_ = secondFuelModelNumber;
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

TwoFuelModels::TwoFuelModelsEnum SurfaceInputs::getTwoFuelModelsMethod() const
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

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum SurfaceInputs::getWindAndSpreadOrientationMode() const
{
    return windAndSpreadOrientationMode_;
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
