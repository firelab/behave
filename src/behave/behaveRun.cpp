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
    fuelModelSet_ = rhs.fuelModelSet_;
    surface_ = rhs.surface_;
    crown_ = rhs.crown_;
    spot_ = rhs.spot_;
}

BehaveRun& BehaveRun::operator= (const BehaveRun& rhs)
{
    if (this != &rhs)
    {
        fuelModelSet_ = rhs.fuelModelSet_;
        surface_ = rhs.surface_;
        crown_ = rhs.crown_;
        spot_ = rhs.spot_;
    }
    return *this;
}

BehaveRun::~BehaveRun()
{
    // Default Destructor
}

void BehaveRun::setCanopyCover(double canopyCover)
{
    // Convert canopy cover input from percent to decimal fraction
    canopyCover /= 100.0;
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
    // Convert moisture input from percent to decimal fraction
    moistureOneHour /= 100.0;
    surface_.setMoistureOneHour(moistureOneHour);
}

void BehaveRun::setMoistureTenHour(double moistureTenHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureTenHour /= 100.0;
    surface_.setMoistureTenHour(moistureTenHour);
}

void BehaveRun::setMoistureHundredHour(double moistureHundredHour)
{
    // Convert moisture input from percent to decimal fraction
    moistureHundredHour /= 100.0;
    surface_.setMoistureHundredHour(moistureHundredHour);
}

void BehaveRun::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
    // Convert moisture input from percent to decimal fraction
    moistureLiveHerbaceous /= 100.0;
    surface_.setMoistureLiveHerbaceous(moistureLiveHerbaceous);
}

void BehaveRun::setMoistureLiveWoody(double moistureLiveWoody)
{
    // Convert moisture input from percent to decimal fraction
    moistureLiveWoody /= 100.0;
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
    //Convert fuel model coverage input from percent to decimal fraction
    firstFuelModelCoverage /= 100;
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
    //Convert relevant inputs from percent to decimal fraction
    moistureOneHour /= 100;
    moistureTenHour /= 100;
    moistureHundredHour /= 100;
    moistureLiveHerbaceous /= 100;
    moistureLiveWoody /= 100;
    canopyCover /= 100;

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
    //Convert relevant inputs from percent to decimal fraction
    moistureOneHour /= 100;
    moistureTenHour /= 100;
    moistureHundredHour /= 100;
    moistureLiveHerbaceous /= 100;
    moistureLiveWoody /= 100;
    firstFuelModelCoverage /= 100;
    canopyCover /= 100;

    surface_.updateSurfaceInputsForTwoFuelModels(firstfuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

void BehaveRun::updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
    double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio)
{
    //Convert relevant inputs from percent to decimal fraction
    moistureOneHour /= 100;
    moistureTenHour /= 100;
    moistureHundredHour /= 100;
    moistureLiveHerbaceous /= 100;
    moistureLiveWoody /= 100;
    canopyCover /= 100;

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
    //Convert relevant inputs from percent to decimal fraction
    moistureOneHour /= 100;
    moistureTenHour /= 100;
    moistureHundredHour /= 100;
    moistureLiveHerbaceous /= 100;
    moistureLiveWoody /= 100;
    canopyCover /= 100;

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

double BehaveRun::getWindSpeed() const
{
    return surface_.getWindSpeed();
}

double BehaveRun::getWindDirection() const
{
    return surface_.getWindDirection();
}

double BehaveRun::getSlope() const
{
    return surface_.getSlope();
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

double BehaveRun::getMoistureOneHour() const
{
    // Convert moisture from decimal fraction to percent
    return surface_.getMoistureOneHour() * 100;
}

double BehaveRun::getMoistureTenHour() const
{
    // Convert moisture from decimal fraction to percent
    return surface_.getMoistureTenHour() * 100;
}

double BehaveRun::getMoistureHundredHour() const
{
    // Convert moisture from decimal fraction to percent
    return surface_.getMoistureHundredHour() * 100;
}

double BehaveRun::getMoistureLiveHerbaceous() const
{
    // Convert moisture from decimal fraction to percent
    return surface_.getMoistureLiveHerbaceous() * 100;
}

double BehaveRun::getMoistureLiveWoody() const
{
    // Convert moisture from decimal fraction to percent
    return surface_.getMoistureLiveWoody() * 100;
}

double BehaveRun::getCanopyCover() const
{
    // Convert canopy cover from decimal fraction to percent
    return surface_.getCanopyCover() * 100;
}

double BehaveRun::getCanopyHeight() const
{
    return surface_.getCanopyHeight();
}

double BehaveRun::getCrownRatio() const
{
    return surface_.getCrownRatio();
}

void BehaveRun::updateDeepCopyOfSurface(const Surface & surface)
{
    crown_.updateDeepCopyOfSurface(surface);
}

void BehaveRun::setCanopyBaseHeight(double canopyBaseHeight)
{
    crown_.setCanopyBaseHeight(canopyBaseHeight);
}

void BehaveRun::setCanopyBulkDensity(double bulkDensity)
{
    crown_.setCanopyBulkDensity(bulkDensity);
}

void BehaveRun::setFoliarMoisture(double foliarMoisture)
{
    crown_.setFoliarMoisture(foliarMoisture);
}

void BehaveRun::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    crown_.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
}

double BehaveRun::getCanopyBaseHeight() const
{
    return crown_.getCanopyBaseHeight();
}

double BehaveRun::getCanopyBulkDensity() const
{
    return crown_.getCanopyBulkDensity();
}

double BehaveRun::getFoliarMoisture() const
{
    return crown_.getFoliarMoisture();
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
    // Update Crown's copy of surface
    crown_.updateDeepCopyOfSurface(surface_);
    // Calculate Crown module outputs
    crown_.doCrownRun();
}

double BehaveRun::getCrownFireSpreadRate() const
{
    const double FEET_PER_MIN_TO_CHAINS_PER_HOUR = 10.0 / 11.0; // conversion factor from ft/min to chains/hr
    return crown_.getCrownFireSpreadRate() * FEET_PER_MIN_TO_CHAINS_PER_HOUR;
}

FireType::FireTypeEnum BehaveRun::getCrownFireType() const
{
    return crown_.getFireType();
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
