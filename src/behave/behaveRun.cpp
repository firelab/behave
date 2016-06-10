/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Interface for Behave application based on the Facade OOP Design
Pattern used to tie together the modules and objects used by Behave
* Author:   William Chatham <wchatham@fs.fed.us>
*
*******************************************************************************
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

void BehaveRun::setCanopyCover(double canopyCover)
{
    surface_.setCanopyCover(canopyCover);
}

void BehaveRun::setCanopyHeight(double canopyHeight)
{
    surface_.setCanopyHeight(canopyHeight);
}

void BehaveRun::setCrownRatio(double crownRatio)
{
    surface_.setCrownRatio(crownRatio);
}

void BehaveRun::setFuelModelSet(FuelModelSet& fuelModelSet)
{
    // makes this behaveRun's fuelModelSet_ point to the FuelModelSet given to this method as a parameter
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

void BehaveRun::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage)
{
    surface_.setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage);
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

void BehaveRun::doSurfaceRunInDirectionOfMaxSpread()
{
    surface_.doSurfaceRunInDirectionOfMaxSpread();
}

void BehaveRun::doSurfaceRunInDirectionOfInterest(double directionOfInterest)
{
    // Calculate SURFACE Module outputs
    surface_.doSurfaceRunInDirectionOfInterest(directionOfInterest);
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

double BehaveRun::getCanopyCover() const
{
    return surface_.getCanopyCover();
}

double BehaveRun::getCanopyHeight() const
{
    return surface_.getCanopyHeight();
}

double BehaveRun::getCrownRatio() const
{
    return surface_.getCrownRatio();
}

void BehaveRun::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    crown_.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
}

void BehaveRun::doCrownRun()
{
    // Calculate Crown module outputs
    crown_.doCrownRun();
}

double BehaveRun::getCrownFireSpreadRate() const
{
    const double FEET_PER_MIN_TO_CHAINS_PER_HOUR = 10.0 / 11.0; // conversion factor from ft/min to chains/hr
    return crown_.getCrownFireSpreadRate() * FEET_PER_MIN_TO_CHAINS_PER_HOUR;
}

FireType::FireTypeEnum BehaveRun::getFireType() const
{
    return crown_.getFireType();
}
