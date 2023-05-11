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

Surface::Surface(const FuelModels& fuelModels)
    : surfaceInputs_(),
    surfaceFire_(fuelModels, surfaceInputs_, size_)
{
    fuelModels_ = &fuelModels;
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
    size_ = rhs.size_;
}

bool Surface::isAllFuelLoadZero(int fuelModelNumber)
{
   return fuelModels_->isAllFuelLoadZero(fuelModelNumber);
}

void Surface::doSurfaceRunInDirectionOfMaxSpread()
{
    double directionOfInterest = -1; // dummy value
    bool hasDirectionOfInterest = false;
    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(surfaceFire_);
        TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod = surfaceInputs_.getTwoFuelModelsMethod();
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
        if (isAllFuelLoadZero(fuelModelNumber) || !fuelModels_->isFuelModelDefined(fuelModelNumber))
        {
            // No fuel to burn, spread rate is zero
            surfaceFire_.skipCalculationForZeroLoad();
        }
        else
        {
            // Calculate spread rate
            surfaceFire_.calculateForwardSpreadRate(fuelModelNumber, hasDirectionOfInterest, directionOfInterest);
        }
    }
}

void Surface::doSurfaceRunInDirectionOfInterest(double directionOfInterest)
{
    bool hasDirectionOfInterest = true;
    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(surfaceFire_);
        TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod = surfaceInputs_.getTwoFuelModelsMethod();
        int firstFuelModelNumber = surfaceInputs_.getFirstFuelModelNumber();
        double firstFuelModelCoverage = surfaceInputs_.getFirstFuelModelCoverage();
        int secondFuelModelNumber = surfaceInputs_.getSecondFuelModelNumber();
        surfaceTwoFuelModels.calculateWeightedSpreadRate(twoFuelModelsMethod, firstFuelModelNumber, firstFuelModelCoverage,
            secondFuelModelNumber, hasDirectionOfInterest, directionOfInterest);
    }
    else // Use only one fuel model
    {   
        int fuelModelNumber = surfaceInputs_.getFuelModelNumber();
        if (isAllFuelLoadZero(fuelModelNumber) || !fuelModels_->isFuelModelDefined(fuelModelNumber))
        {
            // No fuel to burn, spread rate is zero
            surfaceFire_.skipCalculationForZeroLoad();
        }
        else
        {
            // Calculate spread rate
            surfaceFire_.calculateForwardSpreadRate(fuelModelNumber, hasDirectionOfInterest, directionOfInterest);
        }
    }
}

double Surface::calculateFlameLength(double firelineIntensity)
{
    return surfaceFire_.calculateFlameLength(firelineIntensity);
}

void Surface::setFuelModels(FuelModels& fuelModels)
{
    fuelModels_ = &fuelModels;
}

void Surface::initializeMembers()
{
    surfaceFire_.initializeMembers();
    surfaceInputs_.initializeMembers();
}

double Surface::calculateSpreadRateAtVector(double directionOfinterest)
{
    return surfaceFire_.calculateSpreadRateAtVector(directionOfinterest);
}

double Surface::getSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(surfaceFire_.getSpreadRate(), spreadRateUnits);
}

double Surface::getSpreadRateInDirectionOfInterest(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(surfaceFire_.getSpreadRateInDirectionOfInterest(), spreadRateUnits);
}

double Surface::getBackingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits)
{
    return SpeedUnits::fromBaseUnits(size_.getBackingSpreadRate(SpeedUnits::FeetPerMinute), spreadRateUnits);
}

double Surface::getFlankingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits)
{
    return SpeedUnits::fromBaseUnits(size_.getFlankingSpreadRate(SpeedUnits::FeetPerMinute), spreadRateUnits);
}

double Surface::getSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    double elapsedTimeInBaseUnits = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    double spreadRateInBaseUnits = surfaceFire_.getSpreadRate();
    double spreadDistanceInBaseUnits = spreadRateInBaseUnits * elapsedTimeInBaseUnits;
    return LengthUnits::fromBaseUnits(spreadDistanceInBaseUnits, lengthUnits);
}

double Surface::getSpreadDistanceInDirectionOfInterest(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    double elapsedTimeInBaseUnits = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    double spreadRateInBaseUnits = surfaceFire_.getSpreadRateInDirectionOfInterest();
    double spreadDistanceInBaseUnits = spreadRateInBaseUnits * elapsedTimeInBaseUnits;
    return LengthUnits::fromBaseUnits(spreadDistanceInBaseUnits, lengthUnits);
}

double Surface::getBackingSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits)
{
    double elapsedTimeInBaseUnits = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    double spreadRateInBaseUnits = size_.getBackingSpreadRate(SpeedUnits::FeetPerMinute);
    double spreadDistanceInBaseUnits = spreadRateInBaseUnits * elapsedTimeInBaseUnits;
    return LengthUnits::fromBaseUnits(spreadDistanceInBaseUnits, lengthUnits);
}

double Surface::getFlankingSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits)
{
    double elapsedTimeInBaseUnits = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    double spreadRateInBaseUnits = size_.getFlankingSpreadRate(SpeedUnits::FeetPerMinute);
    double spreadDistanceInBaseUnits = spreadRateInBaseUnits * elapsedTimeInBaseUnits;
    return LengthUnits::fromBaseUnits(spreadDistanceInBaseUnits, lengthUnits);
}

double Surface::getDirectionOfMaxSpread() const
{
    double directionOfMaxSpread = surfaceFire_.getDirectionOfMaxSpread();
    return directionOfMaxSpread;
}

double Surface::getFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
    return LengthUnits::fromBaseUnits(surfaceFire_.getFlameLength(), flameLengthUnits);
}

double Surface::getFireLengthToWidthRatio() const
{
    return size_.getFireLengthToWidthRatio();
}

double Surface::getFireEccentricity() const
{
    return size_.getEccentricity();
}

double Surface::getHeadingToBackingRatio() const
{
    return  size_.getHeadingToBackingRatio();
}

double Surface::getFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const
{
    return FirelineIntensityUnits::fromBaseUnits(surfaceFire_.getFirelineIntensity(), firelineIntensityUnits);
}

double Surface::getHeatPerUnitArea(HeatPerUnitAreaUnits::HeatPerUnitAreaUnitsEnum heatPerUnitAreaUnits) const
{
    return HeatPerUnitAreaUnits::fromBaseUnits(surfaceFire_.getHeatPerUnitArea(), heatPerUnitAreaUnits);
}

double Surface::getResidenceTime(TimeUnits::TimeUnitsEnum timeUnits) const
{
    return TimeUnits::fromBaseUnits(surfaceFire_.getResidenceTime(), timeUnits);
}

double Surface::getReactionIntensity(HeatSourceAndReactionIntensityUnits::HeatSourceAndReactionIntensityUnitsEnum reactiontionIntensityUnits) const
{
    return HeatSourceAndReactionIntensityUnits::fromBaseUnits(surfaceFire_.getReactionIntensity(), reactiontionIntensityUnits);
}

double Surface::getMidflameWindspeed(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(surfaceFire_.getMidflameWindSpeed(), spreadRateUnits);
}

double Surface::getEllipticalA(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getEllipticalA(lengthUnits, elapsedTime, timeUnits);
}

double Surface::getEllipticalB(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getEllipticalB(lengthUnits, elapsedTime, timeUnits);
}

double Surface::getEllipticalC(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getEllipticalC(lengthUnits, elapsedTime, timeUnits);
}

double Surface::getSlopeFactor() const
{
    return surfaceFire_.getSlopeFactor();
}

double Surface::getBulkDensity(DensityUnits::DensityUnitsEnum densityUnits) const
{
    return DensityUnits::fromBaseUnits(surfaceFire_.getBulkDensity(), densityUnits);
}

double Surface::getHeatSink(HeatSinkUnits::HeatSinkUnitsEnum heatSinkUnits) const
{
    return HeatSinkUnits::fromBaseUnits(surfaceFire_.getHeatSink(), heatSinkUnits);
}

double Surface::getFirePerimeter(LengthUnits::LengthUnitsEnum lengthUnits , double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getFirePerimeter(lengthUnits, elapsedTime, timeUnits);
}

double Surface::getFireArea(AreaUnits::AreaUnitsEnum areaUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getFireArea(areaUnits, elapsedTime, timeUnits);
}

void Surface::setCanopyCover(double canopyCover, CoverUnits::CoverUnitsEnum coverUnits)
{
    surfaceInputs_.setCanopyCover(canopyCover, coverUnits);
}

void Surface::setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    surfaceInputs_.setCanopyHeight(canopyHeight, canopyHeightUnits);
}

void Surface::setCrownRatio(double crownRatio)
{
    surfaceInputs_.setCrownRatio(crownRatio);
}

std::string Surface::getFuelCode(int fuelModelNumber) const
{
    return fuelModels_->getFuelCode(fuelModelNumber);
}

std::string Surface::getFuelName(int fuelModelNumber) const
{
    return fuelModels_->getFuelName(fuelModelNumber);
}

double Surface::getFuelbedDepth(int fuelModelNumber, LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return fuelModels_->getFuelbedDepth(fuelModelNumber, lengthUnits);
}

double Surface::getFuelMoistureOfExtinctionDead(int fuelModelNumber, MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return fuelModels_->getMoistureOfExtinctionDead(fuelModelNumber, moistureUnits);
}

double Surface::getFuelHeatOfCombustionDead(int fuelModelNumber, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return fuelModels_->getHeatOfCombustionDead(fuelModelNumber, heatOfCombustionUnits);
}

double Surface::getFuelHeatOfCombustionLive(int fuelModelNumber, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return fuelModels_->getHeatOfCombustionLive(fuelModelNumber, heatOfCombustionUnits);
}

double Surface::getFuelLoadOneHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return fuelModels_->getFuelLoadOneHour(fuelModelNumber, loadingUnits);
}

double Surface::getFuelLoadTenHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return fuelModels_->getFuelLoadTenHour(fuelModelNumber, loadingUnits);
}

double Surface::getFuelLoadHundredHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return fuelModels_->getFuelLoadHundredHour(fuelModelNumber, loadingUnits);
}

double Surface::getFuelLoadLiveHerbaceous(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return fuelModels_->getFuelLoadLiveHerbaceous(fuelModelNumber, loadingUnits);
}

double Surface::getFuelLoadLiveWoody(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return fuelModels_->getFuelLoadLiveWoody(fuelModelNumber, loadingUnits);
}

double Surface::getFuelSavrOneHour(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return fuelModels_->getSavrOneHour(fuelModelNumber, savrUnits);
}

double Surface::getFuelSavrLiveHerbaceous(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return fuelModels_->getSavrLiveHerbaceous(fuelModelNumber, savrUnits);
}

double Surface::getFuelSavrLiveWoody(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return fuelModels_->getSavrLiveWoody(fuelModelNumber, savrUnits);
}

bool Surface::isFuelDynamic(int fuelModelNumber) const
{
    return fuelModels_->getIsDynamic(fuelModelNumber);
}

bool Surface::isFuelModelDefined(int fuelModelNumber) const
{
    return fuelModels_->isFuelModelDefined(fuelModelNumber);
}

bool Surface::isFuelModelReserved(int fuelModelNumber) const
{
    return fuelModels_->isFuelModelReserved(fuelModelNumber);
}

bool Surface::isAllFuelLoadZero(int fuelModelNumber) const
{
    return fuelModels_->isAllFuelLoadZero(fuelModelNumber);
}

bool Surface::isUsingTwoFuelModels() const
{
    return surfaceInputs_.isUsingTwoFuelModels();
}

int Surface::getFuelModelNumber() const
{
	return surfaceInputs_.getFuelModelNumber();
}

double Surface::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureOneHour(moistureUnits);
}

double Surface::getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureTenHour(moistureUnits);
}

double Surface::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureHundredHour(moistureUnits);
}

double Surface::getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureLiveHerbaceous(moistureUnits);
}

double Surface::getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureLiveWoody(moistureUnits);
}

double Surface::getCanopyCover(CoverUnits::CoverUnitsEnum coverUnits) const
{
    return CoverUnits::fromBaseUnits(surfaceInputs_.getCanopyCover(), coverUnits);
}

double Surface::getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const
{
    return LengthUnits::fromBaseUnits(surfaceInputs_.getCanopyHeight(), canopyHeightUnits);
}

double Surface::getCrownRatio() const
{
    return surfaceInputs_.getCrownRatio();
}

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum Surface::getWindAndSpreadOrientationMode() const
{
    return surfaceInputs_.getWindAndSpreadOrientationMode();
}

WindHeightInputMode::WindHeightInputModeEnum Surface::getWindHeightInputMode() const
{
    return surfaceInputs_.getWindHeightInputMode();
}

WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum Surface::getWindAdjustmentFactorCalculationMethod() const
{
    return surfaceInputs_.getWindAdjustmentFactorCalculationMethod();
}

double Surface::getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits, 
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode) const
{
    double midFlameWindSpeed = surfaceFire_.getMidflameWindSpeed();
    double windSpeed = midFlameWindSpeed;
    if (windHeightInputMode == WindHeightInputMode::DirectMidflame)
    {
        windSpeed = midFlameWindSpeed;
    }
    else 
    {
        double windAdjustmentFactor = surfaceFire_.getWindAdjustmentFactor();
    
        if ((windHeightInputMode == WindHeightInputMode::TwentyFoot) && (windAdjustmentFactor > 0.0))
        {
            windSpeed = midFlameWindSpeed / windAdjustmentFactor;
        }
        else // Ten Meter
        {
            if (windAdjustmentFactor > 0.0)
            {
                windSpeed = (midFlameWindSpeed / windAdjustmentFactor) * 1.15;
            }
        }
    }
    return SpeedUnits::fromBaseUnits(windSpeed, windSpeedUnits);
}

double Surface::getWindDirection() const
{
    return surfaceInputs_.getWindDirection();
}

double Surface::getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const
{
    return SlopeUnits::fromBaseUnits(surfaceInputs_.getSlope(), slopeUnits);
}

double Surface::getAspect() const
{
    return surfaceInputs_.getAspect();
}

void Surface::setFuelModelNumber(int fuelModelNumber)
{
    surfaceInputs_.setFuelModelNumber(fuelModelNumber);
}

void Surface::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureOneHour(moistureOneHour, moistureUnits);
}

void Surface::setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureTenHour(moistureTenHour, moistureUnits);
}

void Surface::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureHundredHour(moistureHundredHour, moistureUnits);
}

void Surface::setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureLiveHerbaceous(moistureLiveHerbaceous, moistureUnits);
}

void Surface::setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureLiveWoody(moistureLiveWoody, moistureUnits);
}

void Surface::setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits)
{
    surfaceInputs_.setSlope(slope, slopeUnits);
}

void Surface::setAspect(double aspect)
{
    surfaceInputs_.setAspect(aspect);
}

void Surface::setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surfaceInputs_.setWindSpeed(windSpeed, windSpeedUnits, windHeightInputMode);
    surfaceFire_.calculateMidflameWindSpeed();
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

void Surface::setTwoFuelModelsMethod(TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod)
{
    surfaceInputs_.setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void Surface::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage, CoverUnits::CoverUnitsEnum coverUnits)
{
    surfaceInputs_.setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage, coverUnits);
}

void Surface::setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod)
{
    surfaceInputs_.setWindAdjustmentFactorCalculationMethod(windAdjustmentFactorCalculationMethod);
}

void Surface::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, 
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode,
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
    LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyHeightUnits, crownRatio);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
    double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double firstFuelModelCoverage,
    CoverUnits::CoverUnitsEnum firstFuelModelCoverageUnits, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover,
    CoverUnits::CoverUnitsEnum canopyCoverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, 
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope,
        slopeUnits, aspect, canopyCover,canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double ageOfRough,
    double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, double slope, SlopeUnits::SlopeUnitsEnum slopeUnits,
    double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        ageOfRough, heightOfUnderstory, palmettoCoverage, overstoryBasalArea, slope, slopeUnits, aspect, canopyCover, coverUnits,
        canopyHeight, canopyHeightUnits, crownRatio);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, 
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour, 
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits,
    double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
    SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
    LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits,
        windHeightInputMode, windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits,
        canopyHeight, canopyHeightUnits, crownRatio);
    surfaceFire_.calculateMidflameWindSpeed();
}
