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

// TODO: Need to revisit how the Two Fuel Models module is organized and handled - WMC 02/2016

#ifndef BEHAVERUN_H
#define BEHAVERUN_H

#include "behaveUnits.h"
#include "crown.h"
#include "spot.h"
#include "surface.h"

class FuelModels;

class BehaveRun
{
public:
    BehaveRun() = delete; // There is no default constructor
    explicit BehaveRun(FuelModelSet& fuelModelSet);
    
    BehaveRun(const BehaveRun& rhs);
    BehaveRun& operator=(const BehaveRun& rhs);
    ~BehaveRun();

    // FuelModelSet Methods
    bool isFuelModelDefined(int fuelModelNumber) const;
	double getFuelLoadOneHour(int fuelModelNumber) const;
	double getFuelLoadTenHour(int fuelModelNumber) const;
	double getFuelLoadHundredHour(int fuelModelNumber) const;
	double getFuelLoadLiveHerbaceous(int fuelModelNumber) const;
	double getFuelLoadLiveWoody(int fuelModelNumber) const;

    // SURFACE 
    void doSurfaceRunInDirectionOfMaxSpread();
    void doSurfaceRunInDirectionOfInterest(double directionOfInterest);
    
    // SURFACE Module Inputs Setters
    void setCanopyCover(double canopyCover);
    void setCanopyHeight(double canopyHeight);
    void setCrownRatio(double crownRatio);
    void setFuelModelSet(FuelModelSet& fuelModelSet);
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setSlope(double slope);
    void setAspect(double aspect);
    void setWindSpeed(double windSpeed);
    void setWindSpeedUnits(SpeedUnits::SpeedUnitsEnum windSpeedUnits);
    void setSpreadRateUnits(SpeedUnits::SpeedUnitsEnum spreadRateUnits);
    void setWindDirection(double windDirection);
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadAngleMode);
    void setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod);
    void setSlopeUnits(SlopeUnits::SlopeUnitsEnum slopeUnits);
    void setTreeAndCanopyHeightUnits(LengthUnits::LengthUnitsEnum treeAndCanopyHeightUnits);
    void setCoverUnits(CoverUnits::CoverUnitsEnum coverUnits);
    void setFirstFuelModelNumber(int firstFuelModelNumber);
    void setSecondFuelModelNumber(int secondFuelModelNumber);
    void setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage);
    void setWindAdjustmentFactorCalculationMethod(
        WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod);
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
        double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
        double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, 
        MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection, double firstFuelModelCoverage,
        TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect, double canopyCover, double canopyHeight, 
        double crownRatio);
    void updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
        double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea,
        double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, 
        AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
        double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
        MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection, double slope, double aspect, 
        double canopyCover, double canopyHeight, double crownRatio);

    // SURFACE Module Getters
	int getFuelModelNumber() const;
    double getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getWindSpeed(SpeedUnits::SpeedUnitsEnum speedUnits) const;
    double getWindDirection() const;
    double getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const;
    double getAspect() const;
    CoverUnits::CoverUnitsEnum getCoverUnits() const;
    double getCanopyCover(CoverUnits::CoverUnitsEnum coverUnits) const;
    double getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const;
    double getCrownRatio() const;
    double getSurfaceFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getDirectionOfMaxSpread() const;
    double getSurfaceFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    double getSurfaceFireLengthToWidthRatio() const;
    double getSurfaceFireEccentricity() const;
    double getSurfaceFirelineIntensity() const;
    double getMidflameWindspeed(SpeedUnits::SpeedUnitsEnum midflameWindSpeedUnits) const;
    double getSurfaceFireHeatPerUnitArea() const;
    double getSurfaceFireResidenceTime() const;
    double getSurfaceFireReactionIntensity() const;
    double getEllipticalA() const;
    double getEllipticalB() const;
    double getEllipticalC() const;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum getWindAndSpreadOrientationMode() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;
    SlopeUnits::SlopeUnitsEnum getSlopeUnits() const;
    SpeedUnits::SpeedUnitsEnum getWindSpeedUnits() const;
    SpeedUnits::SpeedUnitsEnum getSpreadRateUnits() const;
    LengthUnits::LengthUnitsEnum getFlameLengthUnits() const;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum getWindAdjustmentFactorCalculationMethod() const;
   
    // CROWN Module
    void doCrownRun();

    // CROWN Module Setters
    void setCanopyBaseHeight(double canopyBaseHeight);
    void setCanopyBulkDensity(double bulkDensity);
    void setFoliarMoisture(double foliarMoisture, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setCanopyBulkDensityUnits(DensityUnits::DensityUnitsEnum densityUnits);

    // CROWN Module Getters
    double getCanopyBaseHeight() const;
    double getCanopyBulkDensity() const;
    double getMoistureFoliar() const;
    double getCrownFirelineIntensity() const;
    double getCrownFlameLength() const;
    double getCrownFireSpreadRate() const;
    FireType::FireTypeEnum getCrownFireType() const;
    DensityUnits::DensityUnitsEnum getCanopyBulkDensityUnits() const;

    // SPOT Module
    void calculateSpottingDistanceFromBurningPile();
    void calculateSpottingDistanceFromSurfaceFire();
    void calculateSpottingDistanceFromTorchingTrees();

	// SPOT Module Setters
	void updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double buringPileFlameHeight,
		double windSpeedAtTwentyFeet = SpotSurfaceInputs::NOT_SET);
	void updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double flameLength = SpotSurfaceInputs::NOT_SET,
		double windSpeedAtTwentyFeet = SpotSurfaceInputs::NOT_SET);
	void updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double torchingTrees, double DBH, double treeHeight,
		SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet = SpotSurfaceInputs::NOT_SET);

    // SPOT Module Getters
    double getMaxSpottingDistanceFromBurningPile();
    double getMaxSpottingDistanceFromSurfaceFire();
    double getMaxSpottingDistanceFromTorchingTrees();

private:
    void memberwiseCopyAssignment(const BehaveRun& rhs);

    // Fuel model set (orginal 13, 40 and custom)
    FuelModelSet* fuelModelSet_;

    // SURFACE Module
    Surface surface_;

    // CROWN Module
    Crown crown_;

    // SPOT Module
    Spot spot_;
};

#endif //BEHAVERUN_H
