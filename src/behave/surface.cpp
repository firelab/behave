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

#include <cmath>

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
    surfaceInputs_.updateMoisturesBasedOnInputMode();
    double directionOfInterest = 0.0;
    bool hasDirectionOfInterest = false;
    SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode = SurfaceFireSpreadDirectionMode::FromIgnitionPoint;

    if (isUsingTwoFuelModels())
    {
        // Calculate spread rate for Two Fuel Models
        SurfaceTwoFuelModels surfaceTwoFuelModels(surfaceFire_);
        TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod = surfaceInputs_.getTwoFuelModelsMethod();
        int firstFuelModelNumber = surfaceInputs_.getFirstFuelModelNumber();
        double firstFuelModelCoverage = surfaceInputs_.getFirstFuelModelCoverage();
        int secondFuelModelNumber = surfaceInputs_.getSecondFuelModelNumber();
        surfaceTwoFuelModels.calculateWeightedSpreadRate(twoFuelModelsMethod, firstFuelModelNumber, firstFuelModelCoverage,
            secondFuelModelNumber, hasDirectionOfInterest, directionOfInterest, directionMode);
    }
    else // Use only one fuel model
    {
        // Calculate spread rate
        int fuelModelNumber = surfaceInputs_.getFuelModelNumber();
        bool isUsingChaparralOrPalmettoGallberryOrWesternAspen = surfaceInputs_.getIsUsingPalmettoGallberry() || surfaceInputs_.getIsUsingWesternAspen() ||
            surfaceInputs_.getIsUsingChaparral();
        if (!isUsingChaparralOrPalmettoGallberryOrWesternAspen && (isAllFuelLoadZero(fuelModelNumber) || !fuelModels_->isFuelModelDefined(fuelModelNumber)))
        {
            // No fuel to burn, spread rate is zero
            surfaceFire_.skipCalculationForZeroLoad();
        }
        else
        {
            // Calculate spread rate
            surfaceFire_.calculateForwardSpreadRate(fuelModelNumber, hasDirectionOfInterest, directionOfInterest, directionMode);
        }
    }
}

void Surface::doSurfaceRunInDirectionOfInterest(double directionOfInterest, SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode)
{
    surfaceInputs_.updateMoisturesBasedOnInputMode();
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
            secondFuelModelNumber, hasDirectionOfInterest, directionOfInterest, directionMode);
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
            surfaceFire_.calculateForwardSpreadRate(fuelModelNumber, hasDirectionOfInterest, directionOfInterest, directionMode);
        }
    }
}

//------------------------------------------------------------------------------
/*! \brief Calculates flame length from fireline (Byram's) intensity.
 *
 *  \param firelineIntensity Fireline (Byram's) intensity (btu/ft/s).
 *
 *  \return Flame length.
 */

double Surface::calculateFlameLength(double firelineIntensity, FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits,
    LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    firelineIntensity = FirelineIntensityUnits::toBaseUnits(firelineIntensity, firelineIntensityUnits);
    double flameLength = ((firelineIntensity < 1.0e-07)
        ? (0.0)
        : (0.45 * pow(firelineIntensity, 0.46)));
    return LengthUnits::fromBaseUnits(flameLength, flameLengthUnits);
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

double Surface::calculateSpreadRateAtVector(double directionOfinterest, SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode)
{
    return surfaceFire_.calculateSpreadRateAtVector(directionOfinterest, directionMode);
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

double Surface::getDirectionOfInterestFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
    return LengthUnits::fromBaseUnits(surfaceFire_.getDirectionOfInterestFlameLength(), flameLengthUnits);
}

double Surface::getBackingFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
  return LengthUnits::fromBaseUnits(surfaceFire_.getBackingFlameLength(), flameLengthUnits);
}

double Surface::getFlankingFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
  return LengthUnits::fromBaseUnits(surfaceFire_.getFlankingFlameLength(), flameLengthUnits);
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

double Surface::getDirectionOfInterestFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const
{
    return FirelineIntensityUnits::fromBaseUnits(surfaceFire_.getDirectionOfInterestFirelineIntensity(),
                                                 firelineIntensityUnits);
}

double Surface::getBackingFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const
{
  return FirelineIntensityUnits::fromBaseUnits(surfaceFire_.getBackingFirelineIntensity(), firelineIntensityUnits);
}

double Surface::getFlankingFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const
{
  return FirelineIntensityUnits::fromBaseUnits(surfaceFire_.getFlankingFirelineIntensity(), firelineIntensityUnits);
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

double Surface::getSurfaceFireReactionIntensityForLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return surfaceFire_.getSurfaceFireReactionIntensityForLifeState(lifeState);
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

double Surface::getFireLength(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
  return size_.getFireLength(lengthUnits, elapsedTime, timeUnits);
}

double Surface::getMaxFireWidth(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
  return size_.getMaxFireWidth(lengthUnits, elapsedTime, timeUnits);
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

double Surface::getHeatSource(HeatSourceAndReactionIntensityUnits::HeatSourceAndReactionIntensityUnitsEnum heatSourceUnits) const
{
    return HeatSourceAndReactionIntensityUnits::fromBaseUnits(surfaceFire_.getHeatSource(), heatSourceUnits);
}

double Surface::getFirePerimeter(LengthUnits::LengthUnitsEnum lengthUnits , double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getFirePerimeter(false, lengthUnits, elapsedTime, timeUnits);
}

double Surface::getFireArea(AreaUnits::AreaUnitsEnum areaUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    return size_.getFireArea(false, areaUnits, elapsedTime, timeUnits);
}

double Surface::getCharacteristicMoistureByLifeState(FuelLifeState::FuelLifeStateEnum lifeState, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return FractionUnits::fromBaseUnits(surfaceFire_.getWeightedMoistureByLifeState(lifeState), moistureUnits);
}

double Surface::getLiveFuelMoistureOfExtinction(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return FractionUnits::fromBaseUnits(surfaceFire_.getMoistureOfExtinctionByLifeState(FuelLifeState::Live), moistureUnits);
}

double Surface::getCharacteristicSAVR(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(surfaceFire_.getCharacteristicSAVR(), savrUnits);
}

double Surface::getRelativePackingRatio() const {
  return surfaceFire_.getRelativePackingRatio();
}

double Surface::getPackingRatio() const {
  return surfaceFire_.getPackingRatio();
}

void Surface::setCanopyCover(double canopyCover, FractionUnits::FractionUnitsEnum canopyUnits)
{
    surfaceInputs_.setCanopyCover(canopyCover, canopyUnits);
}

void Surface::setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    surfaceInputs_.setCanopyHeight(canopyHeight, canopyHeightUnits);
}

void Surface::setCrownRatio(double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits)
{
    surfaceInputs_.setCrownRatio(crownRatio, crownRatioUnits);
}

void Surface::setElapsedTime(double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) {
  surfaceInputs_.setElapsedTime(elapsedTime, timeUnits);
}

double Surface::getElapsedTime(TimeUnits::TimeUnitsEnum timeUnits) const {
  return surfaceInputs_.getElapsedTime(timeUnits);
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

double Surface::getFuelMoistureOfExtinctionDead(int fuelModelNumber, FractionUnits::FractionUnitsEnum moistureUnits) const
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

double Surface::getMoistureOneHour(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureOneHour(moistureUnits);
}

double Surface::getMoistureTenHour(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureTenHour(moistureUnits);
}

double Surface::getMoistureHundredHour(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureHundredHour(moistureUnits);
}

double Surface::getMoistureDeadAggregateValue(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureDeadAggregateValue(moistureUnits);
}

double Surface::getMoistureLiveHerbaceous(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureLiveHerbaceous(moistureUnits);
}

double Surface::getMoistureLiveWoody(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureLiveWoody(moistureUnits);
}

double Surface::getMoistureLiveAggregateValue(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureLiveAggregateValue(moistureUnits);
}

bool Surface::isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::MoistureClassInputEnum moistureClass) const
{
    int currentFuelModel = surfaceInputs_.getFuelModelNumber();
    bool isMoistureClassInputNeeded = surfaceInputs_.isMoistureClassInputNeeded(moistureClass);
    bool isFuelLoadNonZeroForSizeClass = false;

    double fuelLoad = 0.0;

    LoadingUnits::LoadingUnitsEnum loadingUnits = LoadingUnits::PoundsPerSquareFoot;
    if(moistureClass == MoistureClassInput::OneHour)
    {
        fuelLoad = fuelModels_->getFuelLoadOneHour(currentFuelModel, loadingUnits);
        if(fuelLoad > 0.0)
        {
            isFuelLoadNonZeroForSizeClass = true;
        }
    }
    else if(moistureClass == MoistureClassInput::TenHour)
    {
        fuelLoad = fuelModels_->getFuelLoadTenHour(currentFuelModel, loadingUnits);
        if(fuelLoad > 0.0)
        {
            isFuelLoadNonZeroForSizeClass = true;
        }
    }
    else if(moistureClass == MoistureClassInput::HundredHour)
    {
        fuelLoad = fuelModels_->getFuelLoadHundredHour(currentFuelModel, loadingUnits);
        if(fuelLoad > 0.0)
        {
            isFuelLoadNonZeroForSizeClass = true;
        }
    }
    else if(moistureClass == MoistureClassInput::LiveHerbaceous)
    {
        fuelLoad = fuelModels_->getFuelLoadLiveHerbaceous(currentFuelModel, loadingUnits);
        if(fuelLoad > 0.0)
        {
            isFuelLoadNonZeroForSizeClass = true;
        }
    }
    else if(moistureClass == MoistureClassInput::LiveWoody)
    {
        fuelLoad = fuelModels_->getFuelLoadLiveWoody(currentFuelModel, loadingUnits);
        if(fuelLoad > 0.0)
        {
            isFuelLoadNonZeroForSizeClass = true;
        }
    }
    else if(moistureClass == MoistureClassInput::DeadAggregate)
    {
        // All fuels have at least one non-zero dead component
        isFuelLoadNonZeroForSizeClass = true;
    }
    else if(moistureClass == MoistureClassInput::LiveAggregate)
    {
        double fuelHerbaceous = fuelModels_->getFuelLoadLiveHerbaceous(currentFuelModel, loadingUnits);
        double fuelWoody = fuelModels_->getFuelLoadLiveWoody(currentFuelModel, loadingUnits);

        if((fuelHerbaceous > 0.0) || (fuelWoody > 0.0))
        {
            isFuelLoadNonZeroForSizeClass = true;
        }
    }

    return isMoistureClassInputNeeded && isFuelLoadNonZeroForSizeClass;
}

MoistureInputMode::MoistureInputModeEnum Surface::getMoistureInputMode() const
{
    return surfaceInputs_.getMoistureInputMode();
}

int Surface::getNumberOfMoistureScenarios() const
{
    return surfaceInputs_.getNumberOfMoistureScenarios();
}

int Surface::getMoistureScenarioIndexByName(const std::string name) const
{
    return  surfaceInputs_.getMoistureScenarioIndexByName(name);
}

bool Surface::getIsMoistureScenarioDefinedByName(const std::string name) const
{
    return surfaceInputs_.getIsMoistureScenarioDefinedByName(name);
}

std::string Surface::getMoistureScenarioDescriptionByName(const std::string name) const
{
    return surfaceInputs_.getMoistureScenarioDescriptionByName(name);
}

double Surface::getMoistureScenarioOneHourByName(const std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioOneHourByName(name, moistureUnits);
}

double Surface::getMoistureScenarioTenHourByName(const std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioTenHourByName(name, moistureUnits);
}

double Surface::getMoistureScenarioHundredHourByName(const std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioHundredHourByName(name, moistureUnits);
}

double Surface::getMoistureScenarioLiveHerbaceousByName(const std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioLiveHerbaceousByName(name, moistureUnits);
}

double Surface::getMoistureScenarioLiveWoodyByName(const std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioLiveWoodyByName(name, moistureUnits);
}

bool Surface::getIsMoistureScenarioDefinedByIndex(const int index) const
{
    return surfaceInputs_.getIsMoistureScenarioDefinedByIndex(index);
}

std::string Surface::getMoistureScenarioNameByIndex(const int index) const
{
    return surfaceInputs_.getMoistureScenarioNameByIndex(index);
}

std::string Surface::getMoistureScenarioDescriptionByIndex(const int index) const
{
    return surfaceInputs_.getMoistureScenarioDescriptionByIndex(index);
}

double Surface::getMoistureScenarioOneHourByIndex(const int index, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioOneHourByIndex(index, moistureUnits);
}

double Surface::getMoistureScenarioTenHourByIndex(const int index, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioTenHourByIndex(index, moistureUnits);
}

double Surface::getMoistureScenarioHundredHourByIndex(const int index, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioHundredHourByIndex(index, moistureUnits);
}

double Surface::getMoistureScenarioLiveHerbaceousByIndex(const int index, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioLiveHerbaceousByIndex(index, moistureUnits);
}

double Surface::getMoistureScenarioLiveWoodyByIndex(const int index, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return surfaceInputs_.getMoistureScenarioLiveWoodyByIndex(index, moistureUnits);
}

double Surface::getCanopyCover(FractionUnits::FractionUnitsEnum canopyUnits) const
{
    return surfaceInputs_.getCanopyCover(canopyUnits);
}

double Surface::getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const
{
    return surfaceInputs_.getCanopyHeight(canopyHeightUnits);
}

double Surface::getCrownRatio(FractionUnits::FractionUnitsEnum crownRatioUnits) const
{
    return surfaceInputs_.getCrownRatio(crownRatioUnits);
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

bool Surface::getIsUsingPalmettoGallberry() const
{
    return surfaceInputs_.getIsUsingPalmettoGallberry();
}

double Surface::getAgeOfRough() const
{
    return surfaceInputs_.getPalmettoGallberryAgeOfRough();
}

double Surface::getHeightOfUnderstory(const LengthUnits::LengthUnitsEnum heightUnits) const
{
    return surfaceInputs_.getPalmettoGallberryHeightOfUnderstory(heightUnits);
}

double Surface::getPalmettoGallberryCoverage(FractionUnits::FractionUnitsEnum coverageUnits) const
{
    return surfaceInputs_.getPalmettoGallberryPalmettoCoverage(coverageUnits);
}

double Surface::getOverstoryBasalArea(BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits) const
{
    return surfaceInputs_.getPalmettoGallberryOverstoryBasalArea(basalAreaUnits);
}

double Surface::getPalmettoGallberryMoistureOfExtinctionDead(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return FractionUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberryMoistureOfExtinctionDead(), moistureUnits);
}

double Surface::getPalmettoGallberryHeatOfCombustionDead(HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return HeatOfCombustionUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberryHeatOfCombustionDead(), heatOfCombustionUnits);
}

double Surface::getPalmettoGallberryHeatOfCombustionLive(HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return  HeatOfCombustionUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberryHeatOfCombustionLive(), heatOfCombustionUnits);
}

double Surface::getPalmettoGallberyDeadFineFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyDeadOneHourLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyDeadMediumFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyDeadTenHourLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyDeadFoliageLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyDeadFoliageLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyLitterLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyLitterLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyLiveFineFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyLiveOneHourLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyLiveMediumFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyLiveTenHourLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyLiveFoliageLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyLiveFoliageLoad(), loadingUnits);
}

double Surface::getPalmettoGallberyFuelBedDepth(LengthUnits::LengthUnitsEnum depthUnits) const
{
    return LengthUnits::fromBaseUnits(surfaceFire_.getPalmettoGallberyFuelBedDepth(), depthUnits);
}

bool Surface::getIsUsingWesternAspen() const
{
    return surfaceInputs_.getIsUsingWesternAspen();
}

int Surface::getAspenFuelModelNumber() const
{
    return surfaceInputs_.getAspenFuelModelNumber();
}

double Surface::getAspenCuringLevel(FractionUnits::FractionUnitsEnum curingLevelUnits) const
{
    return surfaceInputs_.getAspenCuringLevel(curingLevelUnits);
}

double Surface::getAspenDBH(LengthUnits::LengthUnitsEnum dbhUnits) const
{
    return surfaceInputs_.getAspenDBH(dbhUnits);
}

AspenFireSeverity::AspenFireSeverityEnum Surface::getAspenFireSeverity() const
{
    return surfaceInputs_.getAspenFireSeverity();
}

double Surface::getAspenLoadDeadOneHour(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getAspenLoadDeadOneHour(), loadingUnits);
}

double Surface::getAspenLoadDeadTenHour(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getAspenLoadDeadTenHour(), loadingUnits);
}

double Surface::getAspenLoadLiveHerbaceous(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getAspenLoadLiveHerbaceous(), loadingUnits);
}

double Surface::getAspenLoadLiveWoody(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getAspenLoadLiveWoody(), loadingUnits);
}

double Surface::getAspenSavrDeadOneHour(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(surfaceFire_.getAspenSavrDeadOneHour(), savrUnits);
}

double Surface::getAspenSavrDeadTenHour(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(surfaceFire_.getAspenSavrDeadTenHour(), savrUnits);
}

double Surface::getAspenSavrLiveHerbaceous(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(surfaceFire_.getAspenSavrLiveHerbaceous(), savrUnits);
}

double Surface::getAspenSavrLiveWoody(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(surfaceFire_.getAspenSavrLiveWoody(), savrUnits);
}

double Surface::getAspenMortality(FractionUnits::FractionUnitsEnum mortalityUnits) const
{
    return FractionUnits::fromBaseUnits(surfaceFire_.getAspenMortality(), mortalityUnits);
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
    return surfaceInputs_.getSlope(slopeUnits);
}

double Surface::getAspect() const
{
    return surfaceInputs_.getAspect();
}

void Surface::setFuelModelNumber(int fuelModelNumber)
{
    surfaceInputs_.setFuelModelNumber(fuelModelNumber);
}

void Surface::setMoistureOneHour(double moistureOneHour, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureOneHour(moistureOneHour, moistureUnits);
}

void Surface::setMoistureTenHour(double moistureTenHour, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureTenHour(moistureTenHour, moistureUnits);
}

void Surface::setMoistureHundredHour(double moistureHundredHour, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureHundredHour(moistureHundredHour, moistureUnits);
}

void Surface::setMoistureDeadAggregate(double moistureDead, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureDeadAggregate(moistureDead, moistureUnits);
}

void Surface::setMoistureLiveHerbaceous(double moistureLiveHerbaceous, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureLiveHerbaceous(moistureLiveHerbaceous, moistureUnits);
}

void Surface::setMoistureLiveWoody(double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureLiveWoody(moistureLiveWoody, moistureUnits);
}

void Surface::setMoistureLiveAggregate(double moistureLive, FractionUnits::FractionUnitsEnum moistureUnits)
{
    surfaceInputs_.setMoistureLiveAggregate(moistureLive, moistureUnits);
}

void Surface::setMoistureScenarios(MoistureScenarios& moistureScenarios)
{
    surfaceInputs_.setMoistureScenarios(moistureScenarios);
}

bool Surface::setCurrentMoistureScenarioByName(std::string moistureScenarioName)
{
    return surfaceInputs_.setCurrentMoistureScenarioByName(moistureScenarioName);
}

bool Surface::setCurrentMoistureScenarioByIndex(int moistureScenarioIndex)
{
    return surfaceInputs_.setCurrentMoistureScenarioByIndex(moistureScenarioIndex);
}

void Surface::setMoistureInputMode(MoistureInputMode::MoistureInputModeEnum moistureInputMode)
{
    surfaceInputs_.setMoistureInputMode(moistureInputMode);
    surfaceInputs_.updateMoisturesBasedOnInputMode();
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

void Surface::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage, FractionUnits::FractionUnitsEnum coverageUnits)
{
    surfaceInputs_.setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage, coverageUnits);
}

void Surface::setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod)
{
    surfaceInputs_.setWindAdjustmentFactorCalculationMethod(windAdjustmentFactorCalculationMethod);
}

void Surface::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits, double windSpeed,
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode,
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, FractionUnits::FractionUnitsEnum canopyUnits, double canopyHeight,
    LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits)
{
    surfaceInputs_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, canopyUnits, canopyHeight, canopyHeightUnits, crownRatio, crownRatioUnits);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
    double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
    FractionUnits::FractionUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double firstFuelModelCoverage,
    FractionUnits::FractionUnitsEnum firstFuelModelCoverageUnits, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover,
    FractionUnits::FractionUnitsEnum canopyCoverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits,
    double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits)
{
    surfaceInputs_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope,
        slopeUnits, aspect, canopyCover,canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, crownRatioUnits);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits, double windSpeed,
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double ageOfRough,
    double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits,
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, FractionUnits::FractionUnitsEnum canopyUnits,
    double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits ) {
    surfaceInputs_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        ageOfRough, heightOfUnderstory, palmettoCoverage, overstoryBasalArea, basalAreaUnits, slope, slopeUnits, aspect, canopyCover, canopyUnits,
        canopyHeight, canopyHeightUnits, crownRatio, crownRatioUnits);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, FractionUnits::FractionUnitsEnum curingLevelUnits,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double dbh, LengthUnits::LengthUnitsEnum dbhUnits, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits,
    double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
    SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, FractionUnits::FractionUnitsEnum canopyUnits, double canopyHeight,
    LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio,  FractionUnits::FractionUnitsEnum crownRatioUnits)
{
    surfaceInputs_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, curingLevelUnits, aspenFireSeverity,
        dbh, dbhUnits, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits,
        windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect,
        canopyCover, canopyUnits, canopyHeight, canopyHeightUnits, crownRatio, crownRatioUnits);
    surfaceFire_.calculateMidflameWindSpeed();
}

void Surface::setAspenFuelModelNumber(int aspenFuelModelNumber)
{
    surfaceInputs_.setAspenFuelModelNumber(aspenFuelModelNumber);
}

void Surface::setAspenCuringLevel(double aspenCuringLevel, FractionUnits::FractionUnitsEnum curingLevelUnits)
{
    surfaceInputs_.setAspenCuringLevel(aspenCuringLevel, curingLevelUnits);
}

void Surface::setAspenDBH(double dbh, LengthUnits::LengthUnitsEnum dbhUnits)
{
    surfaceInputs_.setAspenDBH(dbh, dbhUnits);
}

void Surface::setAspenFireSeverity(AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity)
{
    surfaceInputs_.setAspenFireSeverity(aspenFireSeverity);
}

void Surface::setIsUsingWesternAspen(bool isUsingWesternAspen)
{
    surfaceInputs_.setIsUsingWesternAspen(isUsingWesternAspen);
}

void Surface::setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::ChaparralFuelInputLoadModeEnum fuelLoadInputMode)
{
    surfaceInputs_.setChaparralFuelLoadInputMode(fuelLoadInputMode);
}

void Surface::setChaparralFuelType(ChaparralFuelType::ChaparralFuelTypeEnum chaparralFuelType)
{
    surfaceInputs_.setChaparralFuelType(chaparralFuelType);
}

void Surface::setChaparralFuelBedDepth(double chaparralFuelBedDepth, LengthUnits::LengthUnitsEnum depthUnts)
{
    surfaceInputs_.setChaparralFuelBedDepth(chaparralFuelBedDepth, depthUnts);
}

void Surface::setChaparralFuelDeadLoadFraction(double chaparralFuelDeadLoadFraction)
{
    surfaceInputs_.setChaparralFuelDeadLoadFraction(chaparralFuelDeadLoadFraction);
}

void Surface::setChaparralTotalFuelLoad(double chaparralTotalFuelLoad, LoadingUnits::LoadingUnitsEnum fuelLoadUnits)
{
    surfaceInputs_.setChaparralTotalFuelLoad(chaparralTotalFuelLoad, fuelLoadUnits);
}

void Surface::setIsUsingChaparral(bool isUsingChaparral)
{
    surfaceInputs_.setIsUsingChaparral(isUsingChaparral);
}

ChaparralFuelType::ChaparralFuelTypeEnum Surface::getChaparralFuelType() const
{
    return surfaceInputs_.getChaparralFuelType();
}

double Surface::getChaparralFuelBedDepth(LengthUnits::LengthUnitsEnum depthUnits) const
{
    return surfaceInputs_.getChaparralFuelBedDepth(depthUnits);
}

double Surface::getChaparralFuelDeadLoadFraction() const
{
    return surfaceInputs_.getChaparralFuelDeadLoadFraction();
}

double Surface::getChaparralTotalFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return surfaceInputs_.getChaparralTotalFuelLoad(loadingUnits);
}

double Surface::getChaparralAge(TimeUnits::TimeUnitsEnum ageUnits) const
{
    return surfaceFire_.getChaparralAge();
}

double Surface::getChaparralDaysSinceMayFirst() const
{
    return surfaceFire_.getChaparralDaysSinceMayFirst();
}

double Surface::getChaparralDeadFuelFraction() const
{
    return surfaceFire_.getChaparralDeadFuelFraction();
}

double Surface::getChaparralDeadMoistureOfExtinction(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return FractionUnits::fromBaseUnits(surfaceFire_.getChaparralDeadMoistureOfExtinction(), moistureUnits);
}

double Surface::getChaparralLiveMoistureOfExtinction(FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return  FractionUnits::fromBaseUnits(surfaceFire_.getChaparralLiveMoistureOfExtinction(), moistureUnits);
}

double Surface::getChaparralDensity(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass, DensityUnits::DensityUnitsEnum densityUnits) const
{
    return DensityUnits::fromBaseUnits(surfaceFire_.getChaparralDensity(lifeState, sizeClass), densityUnits);
}

double Surface::getChaparralHeatOfCombustion(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return HeatOfCombustionUnits::fromBaseUnits(surfaceFire_.getChaparralHeatOfCombustion(lifeState, sizeClass), heatOfCombustionUnits);
}

double Surface::getChaparralLoadDeadLessThanQuarterInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Dead, 0), loadingUnits);
}

double Surface::getChaparralLoadDeadQuarterInchToLessThanHalfInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Dead, 1), loadingUnits);
}

double Surface::getChaparralLoadDeadHalfInchToLessThanOneInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Dead, 2), loadingUnits);
}

double Surface::getChaparralLoadDeadOneInchToThreeInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Dead, 3), loadingUnits);
}

double Surface::getChaparralLoadLiveLeaves(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Live, 0), loadingUnits);
}

double Surface::getChaparralLoadLiveStemsLessThanQuaterInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Live, 1), loadingUnits);
}

double Surface::getChaparralLoadLiveQuarterInchToLessThanHalfInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Live, 2), loadingUnits);
}

double Surface::getChaparralLoadLiveHalfInchToLessThanOneInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Live, 3), loadingUnits);
}

double Surface::getChaparralLoadLiveOneInchToThreeInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralLoad(FuelLifeState::Live, 4), loadingUnits);
}

double Surface::getChaparralMoisture(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return FractionUnits::fromBaseUnits(surfaceFire_.getChaparralMoisture(lifeState, sizeClass), moistureUnits);
}

double Surface::getChaparralTotalDeadFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralTotalDeadFuelLoad(), loadingUnits);
}

double Surface::getChaparralTotalLiveFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(surfaceFire_.getChaparralTotalLiveFuelLoad(), loadingUnits);
}

bool Surface::getIsUsingChaparral() const
{
    return surfaceInputs_.getIsUsingChaparral();
}

void Surface::setAgeOfRough(double ageOfRough)
{
    surfaceInputs_.setPalmettoGallberryAgeOfRough(ageOfRough);
}

void Surface::setHeightOfUnderstory(double heightOfUnderstory, LengthUnits::LengthUnitsEnum heightUnits)
{
    surfaceInputs_.setPalmettoGallberryHeightOfUnderstory(heightOfUnderstory, heightUnits);
}

void Surface::setPalmettoCoverage(double palmettoCoverage, FractionUnits::FractionUnitsEnum coverageUnits)
{
    surfaceInputs_.setPalmettoGallberryPalmettoCoverage(palmettoCoverage, coverageUnits);
}

void Surface::setOverstoryBasalArea(double overstoryBasalArea, BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits)
{
    surfaceInputs_.setPalmettoGallberryOverstoryBasalArea(overstoryBasalArea, basalAreaUnits);
}

void Surface::setIsUsingPalmettoGallberry(bool isUsingPalmettoGallberry)
{
    surfaceInputs_.setIsUsingPalmettoGallberry(isUsingPalmettoGallberry);
}
