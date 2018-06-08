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

SurfaceInputs & SurfaceInputs::operator=(const SurfaceInputs & rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
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

    windAndSpreadOrientationMode_ = WindAndSpreadOrientationMode::RelativeToUpslope;
    windHeightInputMode_ = WindHeightInputMode::DirectMidflame;
    twoFuelModelsMethod_ = TwoFuelModelsMethod::NoMethod;
    windAdjustmentFactorCalculationMethod_ = WindAdjustmentFactorCalculationMethod::UseCrownRatio;

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
    aspenFireSeverity_ = AspenFireSeverity::Low;
    DBH_ = 0.0;

    elapsedTime_ = TimeUnits::toBaseUnits(1, TimeUnits::Hours);

    userProvidedWindAdjustmentFactor_ = -1.0;
}

void SurfaceInputs::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, 
    MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, 
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection, 
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
    SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits,
    double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    setSlope(slope, slopeUnits);
    aspect_ = aspect;

 

    setFuelModelNumber(fuelModelNumber);
   
    setMoistureOneHour(moistureOneHour, moistureUnits);
    setMoistureTenHour(moistureTenHour, moistureUnits);
    setMoistureHundredHour(moistureHundredHour, moistureUnits);
    setMoistureLiveHerbaceous(moistureLiveHerbaceous, moistureUnits);
    setMoistureLiveWoody(moistureLiveWoody, moistureUnits);

    setWindSpeed(windSpeed, windSpeedUnits, windHeightInputMode);
    setWindHeightInputMode(windHeightInputMode);

    if (windDirection < 0.0)
    {
        windDirection += 360.0;
    }
    while (windDirection >= 360.0)
    {
        windDirection -= 360.0;
    }

    setWindDirection(windDirection);
    setWindAndSpreadOrientationMode(windAndSpreadOrientationMode);
    isUsingTwoFuelModels_ = false;
    setTwoFuelModelsMethod(TwoFuelModelsMethod::NoMethod);
   
    isUsingPalmettoGallberry_ = false;
    isUsingWesternAspen_ = false;

    setCanopyCover(canopyCover, coverUnits);
    setCanopyHeight(canopyHeight, canopyHeightUnits);
    setCrownRatio(crownRatio);
}

void  SurfaceInputs::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous,
    double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, 
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double firstFuelModelCoverage, 
    CoverUnits::CoverUnitsEnum firstFuelModelCoverageUnits, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod, double slope, SlopeUnits::SlopeUnitsEnum slopeUnits,
    double aspect, double canopyCover, CoverUnits::CoverUnitsEnum canopyCoverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    int fuelModelNumber = firstfuelModelNumber;
    updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, 
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits,
        aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
    setSecondFuelModelNumber(secondFuelModelNumber);
    setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage, firstFuelModelCoverageUnits);
    isUsingTwoFuelModels_ = true;
    setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void  SurfaceInputs::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, 
    double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, 
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode,
    double ageOfRough, double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, 
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
    LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    updateSurfaceInputs(0, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyHeightUnits,
        crownRatio);

    setAgeOfRough(ageOfRough);
    setHeightOfUnderstory(heightOfUnderstory);
    setPalmettoCoverage(palmettoCoverage);
    setOverstoryBasalArea(overstoryBasalArea);
    isUsingPalmettoGallberry_ = true;
}

void SurfaceInputs::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, 
    double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, 
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode,
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
    LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    updateSurfaceInputs(0, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyHeightUnits,
        crownRatio);

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

void SurfaceInputs::setCanopyCover(double canopyCover, CoverUnits::CoverUnitsEnum coverUnits)
{
    canopyCover_ = CoverUnits::toBaseUnits(canopyCover, coverUnits);
}

void SurfaceInputs::setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    canopyHeight_ = LengthUnits::toBaseUnits(canopyHeight, canopyHeightUnits);
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

void SurfaceInputs::setFuelModelNumber(int fuelModelNumber)
{
    fuelModelNumber_ = fuelModelNumber;
}

void SurfaceInputs::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureOneHour_ = MoistureUnits::toBaseUnits(moistureOneHour, moistureUnits);
}

void SurfaceInputs::setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureTenHour_ = MoistureUnits::toBaseUnits(moistureTenHour, moistureUnits);
}

void SurfaceInputs::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureHundredHour_ = MoistureUnits::toBaseUnits(moistureHundredHour, moistureUnits);
}

void SurfaceInputs::setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureLiveHerbaceous_ = MoistureUnits::toBaseUnits(moistureLiveHerbaceous, moistureUnits);
}

void SurfaceInputs::setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureLiveWoody_ = MoistureUnits::toBaseUnits(moistureLiveWoody, moistureUnits);
}

void SurfaceInputs::setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits)
{
    slope_ = SlopeUnits::toBaseUnits(slope, slopeUnits);
}

void SurfaceInputs::setAspect(double aspect)
{
    aspect_ = aspect;
}

void SurfaceInputs::setTwoFuelModelsMethod(TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod)
{
    twoFuelModelsMethod_ = twoFuelModelsMethod;
}

void SurfaceInputs::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage, CoverUnits::CoverUnitsEnum coverUnits)
{
    firstFuelModelCoverage_ = CoverUnits::toBaseUnits(firstFuelModelCoverage, coverUnits);
}

void  SurfaceInputs::setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    windHeightInputMode_ = windHeightInputMode;
    windSpeed_ = SpeedUnits::toBaseUnits(windSpeed, windSpeedUnits);
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

WindHeightInputMode::WindHeightInputModeEnum SurfaceInputs::getWindHeightInputMode() const
{
    return windHeightInputMode_;
}

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum SurfaceInputs::getWindAndSpreadOrientationMode() const
{
    return windAndSpreadOrientationMode_;
}

double SurfaceInputs::getWindDirection() const
{
    return windDirection_;
}

double SurfaceInputs::getWindSpeed() const
{
    return windSpeed_;
}

double SurfaceInputs::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(moistureOneHour_, moistureUnits);
}

double SurfaceInputs::getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
     return MoistureUnits::fromBaseUnits(moistureTenHour_, moistureUnits);
}

double SurfaceInputs::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(moistureHundredHour_, moistureUnits);
}

double SurfaceInputs::getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(moistureLiveHerbaceous_, moistureUnits);
}

double SurfaceInputs::getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(moistureLiveWoody_, moistureUnits);
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

void SurfaceInputs::memberwiseCopyAssignment(const SurfaceInputs & rhs)
{
    fuelModelNumber_ = rhs.fuelModelNumber_;
    moistureOneHour_ = rhs.moistureOneHour_;
    moistureTenHour_ = rhs.moistureTenHour_;
    moistureHundredHour_ = rhs.moistureHundredHour_;
    moistureLiveHerbaceous_ = rhs.moistureLiveHerbaceous_;
    moistureLiveWoody_ = rhs.moistureLiveWoody_;
    windSpeed_ = rhs.windSpeed_;
    windDirection_ = rhs.windDirection_;
    slope_ = rhs.slope_;
    aspect_ = rhs.aspect_;

    isUsingTwoFuelModels_ = rhs.isUsingTwoFuelModels_;
    secondFuelModelNumber_ = rhs.secondFuelModelNumber_;
    firstFuelModelCoverage_ = rhs.firstFuelModelCoverage_;

    isUsingPalmettoGallberry_ = rhs.isUsingPalmettoGallberry_;
    ageOfRough_ = rhs.ageOfRough_;
    heightOfUnderstory_ = rhs.heightOfUnderstory_;
    palmettoCoverage_ = rhs.palmettoCoverage_;
    overstoryBasalArea_ = rhs.overstoryBasalArea_;

    isUsingWesternAspen_ = rhs.isUsingWesternAspen_;
    aspenFuelModelNumber_ = rhs.aspenFuelModelNumber_;
    aspenCuringLevel_ = rhs.aspenCuringLevel_;
    DBH_ = rhs.DBH_;
    aspenFireSeverity_ = rhs.aspenFireSeverity_;

    elapsedTime_ = rhs.elapsedTime_;

    canopyCover_ = rhs.canopyCover_;
    canopyHeight_ = rhs.canopyHeight_;
    crownRatio_ = rhs.crownRatio_;
    userProvidedWindAdjustmentFactor_ = rhs.userProvidedWindAdjustmentFactor_;

    twoFuelModelsMethod_ = rhs.twoFuelModelsMethod_;
    windHeightInputMode_ = rhs.windHeightInputMode_;
    windAndSpreadOrientationMode_ = rhs.windAndSpreadOrientationMode_;
    windAdjustmentFactorCalculationMethod_ = rhs.windAdjustmentFactorCalculationMethod_;
}

void SurfaceInputs::setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor)
{
    userProvidedWindAdjustmentFactor_ = userProvidedWindAdjustmentFactor;
}

void SurfaceInputs::setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod)
{
    windAdjustmentFactorCalculationMethod_ = windAdjustmentFactorCalculationMethod;
}

void SurfaceInputs::setElapsedTime(double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits)
{
    elapsedTime_ = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
}

double SurfaceInputs::getUserProvidedWindAdjustmentFactor() const
{
    return userProvidedWindAdjustmentFactor_;
}

WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum SurfaceInputs::getWindAdjustmentFactorCalculationMethod() const
{
    return windAdjustmentFactorCalculationMethod_;
}

double SurfaceInputs::getElapsedTime() const
{
    return elapsedTime_;
}
