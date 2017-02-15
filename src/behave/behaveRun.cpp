/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Interface for Behave application based on the Facade OOP Design
*           Pattern used to tie together the modules and objects used by Behave
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

BehaveRun::BehaveRun(FuelModelSet& fuelModelSet)
    : surface_(fuelModelSet),
    crown_(fuelModelSet, surface_)
{
    fuelModelSet_ = &fuelModelSet;
}

BehaveRun::BehaveRun(const BehaveRun& rhs)
    : surface_(*rhs.fuelModelSet_),
    crown_(*rhs.fuelModelSet_, surface_) 
{
    memberwiseCopyAssignment(rhs);
}

BehaveRun& BehaveRun::operator=(const BehaveRun& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void BehaveRun::memberwiseCopyAssignment(const BehaveRun& rhs)
{
    fuelModelSet_ = rhs.fuelModelSet_;
    surface_ = rhs.surface_;
    crown_ = rhs.crown_;
    spot_ = rhs.spot_;
}

BehaveRun::~BehaveRun()
{

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
    if (fuelModelSet_->isFuelModelDefined(fuelModelNumber))
    {
        surface_.setFuelModelNumber(fuelModelNumber);
    }
    else
    {
        surface_.setFuelModelNumber(0);
        // TODO: Handle error
    }
}

void BehaveRun::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureOneHour(moistureOneHour, moistureUnits);
}

void BehaveRun::setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureTenHour(moistureTenHour, moistureUnits);
}

void BehaveRun::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureHundredHour(moistureHundredHour, moistureUnits);
}

void BehaveRun::setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureLiveHerbaceous(moistureLiveHerbaceous, moistureUnits);
}

void BehaveRun::setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureLiveWoody(moistureLiveWoody, moistureUnits);
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

void BehaveRun::setWindSpeedUnits(SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
    surface_.setWindSpeedUnits(windSpeedUnits);
}

void BehaveRun::setSpreadRateUnits(SpeedUnits::SpeedUnitsEnum spreadRateUnits)
{
    surface_.setSpreadRateUnits(spreadRateUnits);
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
    if (fuelModelSet_->isFuelModelDefined(firstFuelModelNumber))
    {
        surface_.setFirstFuelModelNumber(firstFuelModelNumber);
    }
    else
    {
        surface_.setFuelModelNumber(0);
        // TODO: Handle error
    }
}

void BehaveRun::setSecondFuelModelNumber(int secondFuelModelNumber)
{
    if (fuelModelSet_->isFuelModelDefined(secondFuelModelNumber))
    {
        surface_.setSecondFuelModelNumber(secondFuelModelNumber);
    }
    else
    {
        surface_.setFuelModelNumber(0);
        // TODO: Handle error
    }
}

void BehaveRun::setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage)
{
    surface_.setTwoFuelModelsFirstFuelModelCoverage(firstFuelModelCoverage);
}

void BehaveRun::setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod)
{
    surface_.setWindAdjustmentFactorCalculationMethod(windAdjustmentFactorCalculationMethod);
}

void BehaveRun::setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod)
{
    surface_.setTwoFuelModelsMethod(twoFuelModelsMethod);
}

void BehaveRun::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection, double slope, double aspect,
    double canopyCover, double canopyHeight, double crownRatio)
{  
    surface_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    setFuelModelNumber(fuelModelNumber);
    surface_.setTwoFuelModelsMethod(TwoFuelModels::NO_METHOD);
}

void  BehaveRun::updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
    double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits,
    double windSpeed, double windDirection, double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect,
    double canopyCover, double canopyHeight, double crownRatio)
{   
    surface_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
    setFirstFuelModelNumber(firstfuelModelNumber);
    setSecondFuelModelNumber(secondFuelModelNumber);
}

void BehaveRun::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
    double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, 
    double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{ 
    surface_.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, 
        moistureLiveWoody, moistureUnits, windSpeed, windDirection, ageOfRough, heightOfUnderstory, palmettoCoverage,
        overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection,
    double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    surface_.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, aspenFireSeverity, DBH, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windDirection,
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

void BehaveRun::setSlopeUnits(SlopeUnits::SlopeUnitsEnum slopeInputMode)
{
    surface_.setSlopeUnits(slopeInputMode);
}

void BehaveRun::setTreeAndCanopyHeightUnits(LengthUnits::LengthUnitsEnum treeAndCanopyHeightUnits)
{
    surface_.setTreeAndCanopyHeightUnits(treeAndCanopyHeightUnits);
    crown_.setCanopyBaseHeightUnits(treeAndCanopyHeightUnits);
}

void BehaveRun::setCoverUnits(CoverUnits::CoverUnitsEnum coverUnits)
{
    surface_.setCoverUnits(coverUnits);
}

WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum BehaveRun::getWindAndSpreadOrientationMode() const
{
    return surface_.getWindAndSpreadOrientationMode();
}

WindHeightInputMode::WindHeightInputModeEnum BehaveRun::getWindHeightInputMode() const
{
    return surface_.getWindHeightInputMode();
}

SpeedUnits::SpeedUnitsEnum BehaveRun::getWindSpeedUnits() const
{
    return surface_.getWindSpeedUnits();
}

SpeedUnits::SpeedUnitsEnum BehaveRun::getSpreadRateUnits() const
{
    return surface_.getSpreadRateUnits();
}

SlopeUnits::SlopeUnitsEnum BehaveRun::getSlopeUnits() const
{
    return surface_.getSlopeUnits();
}

LengthUnits::LengthUnitsEnum BehaveRun::getFlameLengthUnits() const
{
    return surface_.getFlameLengthUnits();
}

WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum BehaveRun::getWindAdjustmentFactorCalculationMethod() const
{
    return surface_.getWindAdjustmentFactorCalculationMethod();
}

double BehaveRun::getSurfaceFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return  SpeedUnits::fromBaseUnits(surface_.getSpreadRate(), spreadRateUnits);
}

double BehaveRun::getDirectionOfMaxSpread() const
{
    return surface_.getDirectionOfMaxSpread();
}

double BehaveRun::getSurfaceFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
    return LengthUnits::fromBaseUnits(surface_.getFlameLength(), flameLengthUnits);
}

double BehaveRun::getSurfaceFireLengthToWidthRatio() const
{
    return surface_.getFireLengthToWidthRatio();
}

double BehaveRun::getSurfaceFireEccentricity() const
{
    return surface_.getFireEccentricity();
}

double BehaveRun::getSurfaceFirelineIntensity() const
{
    return surface_.getFirelineIntensity();
}

double BehaveRun::getMidflameWindspeed(SpeedUnits::SpeedUnitsEnum midflameWindSpeedUnits) const
{
    return surface_.getMidflameWindspeed();
}

double BehaveRun::getSurfaceFireHeatPerUnitArea() const
{
    return surface_.getHeatPerUnitArea();
}

double BehaveRun::getSurfaceFireResidenceTime() const
{
    return surface_.getResidenceTime();
}

double BehaveRun::getSurfaceFireReactionIntensity() const
{
    return surface_.getReactionIntensity();
}

double BehaveRun::getEllipticalA() const
{
    SpeedUnits::SpeedUnitsEnum desiredUnits = surface_.getSpreadRateUnits();
    return SpeedUnits::fromBaseUnits(surface_.getEllipticalA(), desiredUnits);
}

double BehaveRun::getEllipticalB() const
{
    SpeedUnits::SpeedUnitsEnum desiredUnits = surface_.getSpreadRateUnits();
    return SpeedUnits::fromBaseUnits(surface_.getEllipticalB(), desiredUnits);
}

double BehaveRun::getEllipticalC() const
{
    SpeedUnits::SpeedUnitsEnum desiredUnits = surface_.getSpreadRateUnits();
    return SpeedUnits::fromBaseUnits(surface_.getEllipticalC(), desiredUnits);
}

double BehaveRun::getWindSpeed(SpeedUnits::SpeedUnitsEnum speedUnits) const
{
    return SpeedUnits::fromBaseUnits(surface_.getWindSpeed(), speedUnits);
}

double BehaveRun::getWindDirection() const
{
    return surface_.getWindDirection();
}

double BehaveRun::getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const
{
    return SlopeUnits::fromBaseUnits(surface_.getSlope(), slopeUnits);
}

double BehaveRun::getAspect() const
{
    return surface_.getAspect();
}

bool BehaveRun::isFuelModelDefined(int fuelModelNumber) const
{
    return fuelModelSet_->isFuelModelDefined(fuelModelNumber);
}

double BehaveRun::getFuelLoadOneHour(int fuelModelNumber) const
{
	return fuelModelSet_->getFuelLoadOneHour(fuelModelNumber);
}

double BehaveRun::getFuelLoadTenHour(int fuelModelNumber) const
{
	return fuelModelSet_->getFuelLoadTenHour(fuelModelNumber);
}

double BehaveRun::getFuelLoadHundredHour(int fuelModelNumber) const
{
	return fuelModelSet_->getFuelLoadHundredHour(fuelModelNumber);
}

double BehaveRun::getFuelLoadLiveHerbaceous(int fuelModelNumber) const
{
	return fuelModelSet_->getFuelLoadLiveHerbaceous(fuelModelNumber);
}

double BehaveRun::getFuelLoadLiveWoody(int fuelModelNumber) const
{
	return fuelModelSet_->getFuelLoadLiveWoody(fuelModelNumber);
}

int BehaveRun::getFuelModelNumber() const
{
	return surface_.getFuelModelNumber();
}

double BehaveRun::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(surface_.getMoistureOneHour(), moistureUnits);
}

double BehaveRun::getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(surface_.getMoistureTenHour(), moistureUnits);
}

double BehaveRun::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(surface_.getMoistureHundredHour(), moistureUnits);
}

double BehaveRun::getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(surface_.getMoistureLiveHerbaceous(), moistureUnits);
}

double BehaveRun::getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return MoistureUnits::fromBaseUnits(surface_.getMoistureLiveWoody(), moistureUnits);
}

CoverUnits::CoverUnitsEnum BehaveRun::getCoverUnits() const
{
    return surface_.getCoverUnits();
}

double BehaveRun::getCanopyCover(CoverUnits::CoverUnitsEnum coverUnits) const
{
    return CoverUnits::fromBaseUnits(surface_.getCoverUnits(), coverUnits);
}

double BehaveRun::getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const
{
    return LengthUnits::fromBaseUnits(surface_.getCanopyHeight(), canopyHeightUnits);
}

double BehaveRun::getCrownRatio() const
{
    return surface_.getCrownRatio();
}

void BehaveRun::setCanopyBaseHeight(double canopyBaseHeight)
{
    crown_.setCanopyBaseHeight(canopyBaseHeight);
}

void BehaveRun::setCanopyBulkDensity(double bulkDensity)
{
    crown_.setCanopyBulkDensity(bulkDensity);
}

void BehaveRun::setFoliarMoisture(double moistureFoliar, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    crown_.setMoistureFoliar(moistureFoliar, moistureUnits);
}

void BehaveRun::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    crown_.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture, moistureUnits);
}

void BehaveRun::setCanopyBulkDensityUnits(DensityUnits::DensityUnitsEnum densityUnits)
{
    crown_.setCanopyBulkDensityUnits(densityUnits);
}

double BehaveRun::getCanopyBaseHeight() const
{
    LengthUnits::LengthUnitsEnum desiredUnits = crown_.getCanopyBaseHeightUnits();
    return LengthUnits::fromBaseUnits(crown_.getCanopyBaseHeight(),desiredUnits);
}

double BehaveRun::getCanopyBulkDensity() const
{
    DensityUnits::DensityUnitsEnum desiredUnits = crown_.getCanopyBulkDensityUnits();
    return DensityUnits::fromBaseUnits(crown_.getCanopyBulkDensity(), desiredUnits);
}

double BehaveRun::getMoistureFoliar() const
{
    return crown_.getMoistureFoliar();
}

double BehaveRun::getCrownFirelineIntensity() const
{
    return crown_.getCrownFirelineIntensity();
}

double BehaveRun::getCrownFlameLength() const
{
    return crown_.getCrownFlameLength();
}

void BehaveRun::doCrownRun()
{
    // Calculate Crown module outputs
    crown_.doCrownRun();
}

double BehaveRun::getCrownFireSpreadRate() const
{
    SpeedUnits::SpeedUnitsEnum desiredUnits = surface_.getSpreadRateUnits();
    return SpeedUnits::fromBaseUnits(crown_.getCrownFireSpreadRate(), desiredUnits);
}

FireType::FireTypeEnum BehaveRun::getCrownFireType() const
{
    return crown_.getFireType();
}

DensityUnits::DensityUnitsEnum BehaveRun::getCanopyBulkDensityUnits() const
{
    return  crown_.getCanopyBulkDensityUnits();
}

void BehaveRun::calculateSpottingDistanceFromBurningPile()
{
    spot_.calculateSpottingDistanceFromBurningPile();
}

void BehaveRun::calculateSpottingDistanceFromSurfaceFire()
{
    spot_.calculateSpottingDistanceFromSurfaceFire();
}

void BehaveRun::calculateSpottingDistanceFromTorchingTrees()
{
    spot_.calculateSpottingDistanceFromTorchingTrees();
}

void BehaveRun::updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
	double ridgeToValleyElevation, double downwindCoverHeight, double buringPileFlameHeight, double windSpeedAtTwentyFeet)
{
	spot_.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight,
		buringPileFlameHeight, windSpeedAtTwentyFeet);
}

void BehaveRun::updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
	double ridgeToValleyElevation, double downwindCoverHeight, double flameLength, double windSpeedAtTwentyFeet)
{
	spot_.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight,
		flameLength, windSpeedAtTwentyFeet);
}

void BehaveRun::updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
	double ridgeToValleyElevation, double downwindCoverHeight, double torchingTrees, double DBH, double treeHeight,
	SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet)
{
	spot_.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight,
		torchingTrees, DBH, treeHeight, treeSpecies, windSpeedAtTwentyFeet);
}

double BehaveRun::getMaxSpottingDistanceFromBurningPile()
{
    return spot_.getMaxSpottingDistanceFromBurningPile();
}

double BehaveRun::getMaxSpottingDistanceFromSurfaceFire()
{
    return spot_.getMaxSpottingDistanceFromSurfaceFire();
}

double BehaveRun::getMaxSpottingDistanceFromTorchingTrees()
{
    return spot_.getMaxSpottingDistanceFromTorchingTrees();
}
