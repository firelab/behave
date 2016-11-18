/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the inputs required for surface fire behavior 
*           in the Rothermel Model
* Author:   William Chatham <wchatham@fs.fed.us>
*
******************************************************************************
*
* THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
* MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
* IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
* OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
* PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
* LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
* PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
* RELIABILITY, OR ANY OTHER CHARACTERISTIC.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
******************************************************************************/

#include "surfaceInputs.h"

#include <cmath>

// Default Ctor
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

    slopeUnits_ = SlopeUnits::PERCENT;
    flameLengthUnits_ = LengthUnits::FEET;
    windSpeedUnits_ = VelocityUnits::MILES_PER_HOUR;
    windAndSpreadOrientationMode_ = WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE;
    windHeightInputMode_ = WindHeightInputMode::DIRECT_MIDFLAME;
    twoFuelModelsMethod_ = TwoFuelModels::NO_METHOD;
    canopyHeightUnits_ = LengthUnits::FEET;
    canopyCoverUnits_ = CoverUnits::PERCENT;
    moistureUnits_ = MoistureUnits::PERCENT;

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

    setFuelModelNumber(fuelModelNumber);
   
    setMoistureOneHour(moistureOneHour);
    setMoistureTenHour(moistureTenHour);
    setMoistureHundredHour(moistureHundredHour);
    setMoistureLiveHerbaceous(moistureLiveHerbaceous);
    setMoistureLiveWoody(moistureLiveWoody);

    setWindHeightInputMode(windHeightInputMode);
    setWindSpeed(windSpeed);
    setWindDirection(windDirection);

    isUsingTwoFuelModels_ = false;
    setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum::NO_METHOD);
   
    isUsingPalmettoGallberry_ = false;
    isUsingWesternAspen_ = false;

    setCanopyCover(canopyCover);
    setCanopyHeight(canopyHeight);
    setCrownRatio(crownRatio);
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

    setSecondFuelModelNumber(secondFuelModelNumber);
    setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage);
    isUsingTwoFuelModels_ = true;
    setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void  SurfaceInputs::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
    double ageOfRough, double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, double slope,
    double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    updateSurfaceInputs(0, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    setAgeOfRough(ageOfRough);
    setHeightOfUnderstory(heightOfUnderstory);
    setPalmettoCoverage(palmettoCoverage);
    setOverstoryBasalArea(overstoryBasalArea);
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

    setAspenFuelModelNumber(aspenFuelModelNumber);
    setAspenCuringLevel(aspenCuringLevel);
    setAspenFireSeverity(aspenFireSeverity);
    setAspenDBH(DBH);
    isUsingWesternAspen_ = true;
}

void SurfaceInputs::setAspenFuelModelNumber(int aspenFuelModelNumber)
{
    aspenFuelModelNumber_ = aspenFuelModelNumber;
}

void SurfaceInputs::setAspenCuringLevel(double aspenCuringLevel)
{
    aspenCuringLevel_ = aspenCuringLevel;
}

void SurfaceInputs::setAspenDBH(double DBH)
{
    DBH_ = DBH;
}

void SurfaceInputs::setAspenFireSeverity(AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity)
{
    aspenFireSeverity_ = aspenFireSeverity;
}

void SurfaceInputs::setCanopyCover(double canopyCover)
{
    canopyCover_ = CoverUnits::toBaseUnits(canopyCover, canopyCoverUnits_);
}

void SurfaceInputs::setCanopyHeight(double canopyHeight)
{
    canopyHeight_ = LengthUnits::toBaseUnits(canopyHeight, canopyHeightUnits_);
}

void SurfaceInputs::setCrownRatio(double crownRatio)
{
    crownRatio_ = crownRatio;
}

void SurfaceInputs::setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode)
{
    windAndSpreadOrientationMode_ = windAndSpreadOrientationMode;
}

void SurfaceInputs::setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    windHeightInputMode_ = windHeightInputMode;
}

void SurfaceInputs::setCanopyHeightUnits(LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    canopyHeightUnits_ = canopyHeightUnits;
}

void SurfaceInputs::setCanopyCoverUnits(CoverUnits::CoverUnitsEnum canopyCoverUnits)
{
    canopyCoverUnits_ = canopyCoverUnits;
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

void SurfaceInputs::setMoistureUnits(MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureUnits_ = moistureUnits;
}

void SurfaceInputs::setMoistureOneHour(double moistureOneHour)
{
    moistureOneHour_ = MoistureUnits::toBaseUnits(moistureOneHour, moistureUnits_);
}

void SurfaceInputs::setMoistureTenHour(double moistureTenHour)
{
    moistureTenHour_ = MoistureUnits::toBaseUnits(moistureTenHour, moistureUnits_);
}

void SurfaceInputs::setMoistureHundredHour(double moistureHundredHour)
{
    moistureHundredHour_ = MoistureUnits::toBaseUnits(moistureHundredHour, moistureUnits_);
}

void SurfaceInputs::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    moistureLiveHerbaceous_ = MoistureUnits::toBaseUnits(moistureLiveHerbaceous, moistureUnits_);
}

void SurfaceInputs::setMoistureLiveWoody(double moistureLiveWoody)
{
    moistureLiveWoody_ = MoistureUnits::toBaseUnits(moistureLiveWoody, moistureUnits_);
}

void SurfaceInputs::setSlope(double slope)
{
    slope_ = SlopeUnits::toBaseUnits(slope, slopeUnits_);   
}

void SurfaceInputs::setAspect(double aspect)
{
    aspect_ = aspect;
}

void SurfaceInputs::setFlameLengthUnits(LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    flameLengthUnits_ = flameLengthUnits;
}

void SurfaceInputs::setSlopeUnits(SlopeUnits::SlopeUnitsEnum slopeUnits)
{
    slopeUnits_ = slopeUnits;
}

void SurfaceInputs::setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod)
{
    twoFuelModelsMethod_ = twoFuelModelsMethod;
}

void SurfaceInputs::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage)
{
    firstFuelModelCoverage_ = firstFuelModelCoverage;
}

void  SurfaceInputs::setWindSpeed(double windSpeed)
{
    windSpeed_ = VelocityUnits::toBaseUnits(windSpeed, windSpeedUnits_);
}

void SurfaceInputs::setWindSpeedUnits(VelocityUnits::VelocityUnitsEnum windSpeedUnits)
{
    windSpeedUnits_ = windSpeedUnits;
}

void  SurfaceInputs::setWindDirection(double windDirection)
{
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

MoistureUnits::MoistureUnitsEnum SurfaceInputs::getMoistureUnits() const
{
    return moistureUnits_;
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

    if (userProvidedWindAdjustmentFactor_ >= 0.0) // Negative 1 indicates nothing has been entered
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

SlopeUnits::SlopeUnitsEnum SurfaceInputs::getSlopeUnits() const
{
    return slopeUnits_;
}

VelocityUnits::VelocityUnitsEnum SurfaceInputs::getWindSpeedUnits() const
{
    return windSpeedUnits_;
}

LengthUnits::LengthUnitsEnum SurfaceInputs::getFlameLengthUnits() const
{
    return flameLengthUnits_;
}

CoverUnits::CoverUnitsEnum SurfaceInputs::getCanopyCoverUnits() const
{
    return canopyCoverUnits_;
}

LengthUnits::LengthUnitsEnum SurfaceInputs::getCanopyHeightUnits() const
{
    return canopyHeightUnits_;
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

double SurfaceInputs::getAspenDBH() const
{
    return DBH_;
}

AspenFireSeverity::AspenFireSeverityEnum SurfaceInputs::getAspenFireSeverity() const
{
    return aspenFireSeverity_;
}

void SurfaceInputs::setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor)
{
    userProvidedWindAdjustmentFactor_ = userProvidedWindAdjustmentFactor;
}

double SurfaceInputs::getUserProvidedWindAdjustmentFactor() const
{
    return userProvidedWindAdjustmentFactor_;
}
