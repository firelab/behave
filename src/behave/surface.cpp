/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling surface fire behavior based on the Facade OOP
*           Design Pattern and using the Rothermel spread model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
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

#include "surface.h"

#include "surfaceTwoFuelModels.h"
#include "surfaceInputs.h"

Surface::Surface(const FuelModelSet& fuelModels)
    : surfaceInputs_(),
    surfaceFire_(fuelModels, surfaceInputs_)
{
    fuelModelSet_ = &fuelModels;
}

// Copy Ctor
Surface::Surface(const Surface& rhs)
    : surfaceFire_()
{
    memberwiseCopyAssignment(rhs);
}

Surface& Surface::operator=(const Surface& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void Surface::memberwiseCopyAssignment(const Surface& rhs)
{
    surfaceInputs_ = rhs.surfaceInputs_;
    surfaceFire_ = rhs.surfaceFire_;
}

void Surface::doSurfaceRunInDirectionOfMaxSpread()
{
    double directionOfInterest = -1; // dummy value
    bool hasDirectionOfInterest = false;
    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(surfaceFire_);
        TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod = surfaceInputs_.getTwoFuelModelsMethod();
        int firstFuelModelNumber = surfaceInputs_.getFirstFuelModelNumber();
        double firstFuelModelCoverage = surfaceInputs_.getFirstFuelModelCoverage();
        int secondFuelModelNumber = surfaceInputs_.getSecondFuelModelNumber();
        surfaceTwoFuelModels.calculateWeightedSpreadRate(twoFuelModelsMethod, firstFuelModelNumber, firstFuelModelCoverage, 
            secondFuelModelNumber, hasDirectionOfInterest, directionOfInterest);
    }
    else // Use only one fuel model
    {
        // Calculate spread rate
        int fuelModelNumber = surfaceInputs_.getFuelModelNumber();
        surfaceFire_.calculateForwardSpreadRate(fuelModelNumber, hasDirectionOfInterest, directionOfInterest);
    }
}

void Surface::doSurfaceRunInDirectionOfInterest(double directionOfInterest)
{
    bool hasDirectionOfInterest = true;
    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(surfaceFire_);
        TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod = surfaceInputs_.getTwoFuelModelsMethod();
        int firstFuelModelNumber = surfaceInputs_.getFirstFuelModelNumber();
        double firstFuelModelCoverage = surfaceInputs_.getFirstFuelModelCoverage();
        int secondFuelModelNumber = surfaceInputs_.getSecondFuelModelNumber();
        surfaceTwoFuelModels.calculateWeightedSpreadRate(twoFuelModelsMethod, firstFuelModelNumber, firstFuelModelCoverage,
            secondFuelModelNumber, hasDirectionOfInterest, directionOfInterest);
    }
    else // Use only one fuel model
    {
        // Calculate spread rate
        int fuelModelNumber = surfaceInputs_.getFuelModelNumber();
        surfaceFire_.calculateForwardSpreadRate(fuelModelNumber, hasDirectionOfInterest, directionOfInterest);
    }
}

double Surface::calculateFlameLength(double firelineIntensity)
{
    return surfaceFire_.calculateFlameLength(firelineIntensity);
}

double Surface::calculateSpreadRateAtVector(double directionOfinterest)
{
    return surfaceFire_.calculateSpreadRateAtVector(directionOfinterest);
}

double Surface::getSpreadRate() const
{
    return surfaceFire_.getSpreadRate();
}

double Surface::getDirectionOfMaxSpread() const
{
    double directionOfMaxSpread = surfaceFire_.getDirectionOfMaxSpread();
    return directionOfMaxSpread;
}

double Surface::getFlameLength() const
{
    return surfaceFire_.getFlameLength();
}

double Surface::getFireLengthToWidthRatio() const
{
    return surfaceFire_.getFireLengthToWidthRatio();
}

double Surface::getFireEccentricity() const
{
    return surfaceFire_.getFireEccentricity();
}

double Surface::getFirelineIntensity() const
{
    return surfaceFire_.getFirelineIntensity();
}

double Surface::getHeatPerUnitArea() const
{
    return surfaceFire_.getHeatPerUnitArea();
}

double Surface::getResidenceTime() const
{
    return surfaceFire_.getResidenceTime();
}

double Surface::getReactionIntensity() const
{
    return surfaceFire_.getReactionIntensity();
}

double Surface::getMidflameWindspeed() const
{
    return surfaceFire_.getMidflameWindSpeed();
}

double Surface::getEllipticalA() const
{
    return surfaceFire_.getEllipticalA();
}

double Surface::getEllipticalB() const
{
    return surfaceFire_.getEllipticalB();
}

double Surface::getEllipticalC() const
{
    return surfaceFire_.getEllipticalC();
}

void Surface::setCanopyHeightUnits(LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    surfaceInputs_.setCanopyHeightUnits(canopyHeightUnits);
}

void Surface::setCanopyCover(double canopyCover)
{
    surfaceInputs_.setCanopyCover(canopyCover);
}

void Surface::setCanopyHeight(double canopyHeight)
{
    surfaceInputs_.setCanopyHeight(canopyHeight);
}

void Surface::setCrownRatio(double crownRatio)
{
    surfaceInputs_.setCrownRatio(crownRatio);
}

bool Surface::isUsingTwoFuelModels() const
{
    return surfaceInputs_.isUsingTwoFuelModels();
}

int Surface::getFuelModelNumber() const
{
	return surfaceInputs_.getFuelModelNumber();
}

double Surface::getMoistureOneHour() const
{
    return surfaceInputs_.getMoistureOneHour();
}

double Surface::getMoistureTenHour() const
{
    return surfaceInputs_.getMoistureTenHour();
}

double Surface::getMoistureHundredHour() const
{
    return surfaceInputs_.getMoistureHundredHour();
}

double Surface::getMoistureLiveHerbaceous() const
{
    return surfaceInputs_.getMoistureLiveHerbaceous();
}

double Surface::getMoistureLiveWoody() const
{
    return surfaceInputs_.getMoistureLiveWoody();
}

double Surface::getCanopyCover() const
{
    return surfaceInputs_.getCanopyCover();
}

double Surface::getCanopyHeight() const
{
    return surfaceInputs_.getCanopyHeight();
}

double Surface::getCanopyHeightInDesiredUnits(LengthUnits::LengthUnitsEnum desiredUnits) const
{
    return LengthUnits::fromBaseUnits(surfaceInputs_.getCanopyHeight(), desiredUnits);
}

double Surface::getCrownRatio() const
{
    return surfaceInputs_.getCrownRatio();
}

LengthUnits::LengthUnitsEnum Surface::getFlameLengthUnits() const
{
    return surfaceInputs_.getFlameLengthUnits();
}

LengthUnits::LengthUnitsEnum Surface::getCanopyHeightUnits() const
{
    return  surfaceInputs_.getCanopyHeightUnits();
}

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum Surface::getWindAndSpreadOrientationMode() const
{
    return surfaceInputs_.getWindAndSpreadOrientationMode();
}

WindHeightInputMode::WindHeightInputModeEnum Surface::getWindHeightInputMode() const
{
    return surfaceInputs_.getWindHeightInputMode();
}

SlopeUnits::SlopeUnitsEnum Surface::getSlopeUnits() const
{
    return surfaceInputs_.getSlopeUnits();
}

SpeedUnits::SpeedUnitsEnum Surface::getWindSpeedUnits() const
{
    return surfaceInputs_.getWindSpeedUnits();
}

SpeedUnits::SpeedUnitsEnum Surface::getSpreadRateUnits() const
{
    return surfaceInputs_.getSpreadRateUnits();
}

WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum Surface::getWindAdjustmentFactorCalculationMethod() const
{
    return surfaceInputs_.getWindAdjustmentFactorCalculationMethod();
}

double Surface::getWindSpeed() const
{
    return surfaceInputs_.getWindSpeed();
}

double Surface::getWindDirection() const
{
    return surfaceInputs_.getWindDirection();
}

double Surface::getSlope() const
{
    return surfaceInputs_.getSlope();
}

double Surface::getAspect() const
{
    return surfaceInputs_.getAspect();
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

void Surface::setSlopeUnits(SlopeUnits::SlopeUnitsEnum slopeInputMode)
{
    surfaceInputs_.setSlopeUnits(slopeInputMode);
}

void Surface::setWindSpeed(double windSpeed)
{
    surfaceInputs_.setWindSpeed(windSpeed);
}

void Surface::setWindSpeedUnits(SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
    surfaceInputs_.setWindSpeedUnits(windSpeedUnits);
}

void Surface::setSpreadUnits(SpeedUnits::SpeedUnitsEnum spreadRateUnits)
{
    surfaceInputs_.setSpreadRateUnits(spreadRateUnits);
}

void Surface::setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor)
{
    surfaceInputs_.setUserProvidedWindAdjustmentFactor(userProvidedWindAdjustmentFactor);
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

void Surface::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage)
{
    surfaceInputs_.setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage);
}

void Surface::setFlameLengthUnits(LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    surfaceInputs_.setFlameLengthUnits(flameLengthUnits);
}

void Surface::setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod)
{
    surfaceInputs_.setWindAdjustmentFactorCalculationMethod(windAdjustmentFactorCalculationMethod);
}

void Surface::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, double windSpeed, double windDirection, double slope, double aspect, 
    double canopyCover, double canopyHeight, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
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
