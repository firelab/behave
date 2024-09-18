/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling surface fire behavior based on the Facade OOP
*           Design Pattern and using the Rothermel spread model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in the corresponding cpp file is, in part or in
*           whole, from BehavePlus5 source originally authored by Collin D.
*           Bevins and is used with or without modification.
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

#ifndef SURFACE_H
#define SURFACE_H

// The SURFACE module of BehavePlus
#include "behaveUnits.h"
#include "fireSize.h"
#include "surfaceFire.h"
#include "surfaceInputs.h"

class Surface
{
public:
    Surface() = delete; // no default constructor
    Surface(const FuelModels& fuelModels);
    Surface(const Surface& rhs);
    Surface& operator=(const Surface& rhs);

    bool isAllFuelLoadZero(int fuelModelNumber);
    void doSurfaceRunInDirectionOfMaxSpread();
    void doSurfaceRunInDirectionOfInterest(double directionOfInterest, SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode);

    double calculateFlameLength(double firelineIntensity, FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits,
        LengthUnits::LengthUnitsEnum flameLengthUnits);

    void setFuelModels(FuelModels& fuelModels);
    void initializeMembers();

    // SurfaceFire getters
    double getSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getSpreadRateInDirectionOfInterest(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getBackingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits);
    double getFlankingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits);
    double getSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getSpreadDistanceInDirectionOfInterest(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getBackingSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits);
    double getFlankingSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits);
    double getDirectionOfMaxSpread() const;
    double getFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    double getBackingFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    double getFlankingFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    double getFlameLengthInDirectionOfInterest(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getHeadingToBackingRatio() const;
    double getFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    double getBackingFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    double getFlankingFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    double getFirelineIntensityInDirectionOfInterest(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    double getHeatPerUnitArea(HeatPerUnitAreaUnits::HeatPerUnitAreaUnitsEnum heatPerUnitAreaUnits) const;
    double getMidflameWindspeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits) const;
    double getResidenceTime(TimeUnits::TimeUnitsEnum timeUnits) const;
    double getReactionIntensity(HeatSourceAndReactionIntensityUnits::HeatSourceAndReactionIntensityUnitsEnum reactiontionIntensityUnits) const;
    double getSurfaceFireReactionIntensityForLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const;
    double getEllipticalA(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getEllipticalB(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getEllipticalC(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getFireLength(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getMaxFireWidth(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getSlopeFactor() const;
    double getBulkDensity(DensityUnits::DensityUnitsEnum densityUnits) const;
    double getHeatSink(HeatSinkUnits::HeatSinkUnitsEnum heatSinkUnits) const;
    double getHeatSource(HeatSourceAndReactionIntensityUnits::HeatSourceAndReactionIntensityUnitsEnum heatSourceUnits) const;
    double getFirePerimeter(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getFireArea(AreaUnits::AreaUnitsEnum areaUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getCharacteristicMoistureByLifeState(FuelLifeState::FuelLifeStateEnum lifeState, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getLiveFuelMoistureOfExtinction(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getCharacteristicSAVR(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;

    // SurfaceIntermediate Getters
    double getRelativePackingRatio() const;
    double getPackingRatio()const;

    // SurfaceInputs setters
    void setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCanopyCover(double canopyCover, FractionUnits::FractionUnitsEnum coverUnits);
    void setCrownRatio(double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits);
    void setElapsedTime(double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits);
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureOneHour(double moistureOneHour, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureTenHour(double moistureTenHour, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureHundredHour(double moistureHundredHour, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureDeadAggregate(double moistureDead, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureLiveWoody(double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureLiveAggregate(double moistureLive, FractionUnits::FractionUnitsEnum moistureUnits);
    void setMoistureScenarios(MoistureScenarios& moistureScenarios);
    bool setCurrentMoistureScenarioByName(std::string moistureScenarioName);
    bool setCurrentMoistureScenarioByIndex(int moistureScenarioIndex);
    void setMoistureInputMode(MoistureInputMode::MoistureInputModeEnum moistureInputMode);
    void setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits);
    void setAspect(double aspect);
    void setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor);
    void setWindDirection(double windDirection);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode);
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setFirstFuelModelNumber(int firstFuelModelNumber);
    void setSecondFuelModelNumber(int secondFuelModelNumber);
    void setTwoFuelModelsMethod(TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod);
    void setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage, FractionUnits::FractionUnitsEnum coverageUnits);
    void setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod);
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect,
        double canopyCover, FractionUnits::FractionUnitsEnum coverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits);
    void updateSurfaceInputsForTwoFuelModels(int firstFuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
        double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
        FractionUnits::FractionUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double firstFuelModelCoverage,
        FractionUnits::FractionUnitsEnum firstFuelModelCoverageUnits, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
        double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover,
        FractionUnits::FractionUnitsEnum canopyCoverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits);

    // Palmetto-Gallberry setters
    void updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
        double overstoryBasalArea, BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits, double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, FractionUnits::FractionUnitsEnum coverUnits,
        double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits);
    void setAgeOfRough(double ageOfRough);
    void setHeightOfUnderstory(double heightOfUnderstory, LengthUnits::LengthUnitsEnum heightUnits);
    void setPalmettoCoverage(double palmettoCoverage, FractionUnits::FractionUnitsEnum coverageUnits);
    void setOverstoryBasalArea(double overstoryBasalArea, BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits);
    void setIsUsingPalmettoGallberry(bool isUsingPalmettoGallberry);

    // Westerm Aspen inputs setters
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, FractionUnits::FractionUnitsEnum curingLevelUnits,
        AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double dbh, LengthUnits::LengthUnitsEnum dbhUnits, double moistureOneHour, double moistureTenHour,
        double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, FractionUnits::FractionUnitsEnum moistureUnits,
        double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
        SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, FractionUnits::FractionUnitsEnum coverUnits, double canopyHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, FractionUnits::FractionUnitsEnum crownRatioUnits);
    void setAspenFuelModelNumber(int aspenFuelModelNumber);
    void setAspenCuringLevel(double aspenCuringLevel, FractionUnits::FractionUnitsEnum curingLevelUnits);
    void setAspenDBH(double dbh, LengthUnits::LengthUnitsEnum dbhUnits);
    void setAspenFireSeverity(AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity);
    void setIsUsingWesternAspen(bool isUsingWesternAspen);

    // Chaparral inputs setters
    void setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::ChaparralFuelInputLoadModeEnum fuelLoadInputMode);
    void setChaparralFuelType(ChaparralFuelType::ChaparralFuelTypeEnum chaparralFuelType);
    void setChaparralFuelBedDepth(double chaparralFuelBedDepth, LengthUnits::LengthUnitsEnum depthUnts);
    void setChaparralFuelDeadLoadFraction(double chaparralFuelDeadLoadFraction);
    void setChaparralTotalFuelLoad(double chaparralTotalFuelLoad, LoadingUnits::LoadingUnitsEnum fuelLoadUnits);
    void setIsUsingChaparral(bool isUsingChaparral);

    // Fuel Model Getter Methods
    std::string getFuelCode(int fuelModelNumber) const;
    std::string getFuelName(int fuelModelNumber) const;
    double getFuelbedDepth(int fuelModelNumber, LengthUnits::LengthUnitsEnum lengthUnits) const;
    double getFuelMoistureOfExtinctionDead(int fuelModelNumber, FractionUnits::FractionUnitsEnum moistureUnits) const;;
    double getFuelHeatOfCombustionDead(int fuelModelNumber, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const;
    double getFuelHeatOfCombustionLive(int fuelModelNumber, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const;
    double getFuelLoadOneHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getFuelLoadTenHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getFuelLoadHundredHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getFuelLoadLiveHerbaceous(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getFuelLoadLiveWoody(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getFuelSavrOneHour(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    double getFuelSavrLiveHerbaceous(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    double getFuelSavrLiveWoody(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    bool isFuelDynamic(int fuelModelNumber) const;
    bool isFuelModelDefined(int fuelModelNumber) const;
    bool isFuelModelReserved(int fuelModelNumber) const;
    bool isAllFuelLoadZero(int fuelModelNumber) const;

    // SurfaceInputs getters
    bool isUsingTwoFuelModels() const;
    double getElapsedTime(TimeUnits::TimeUnitsEnum timeUnits) const;
    int getFuelModelNumber() const;
    double getMoistureOneHour(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureTenHour(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureHundredHour(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureDeadAggregateValue(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureLiveHerbaceous(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureLiveWoody(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureLiveAggregateValue(FractionUnits::FractionUnitsEnum moistureUnits) const;
    bool isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::MoistureClassInputEnum moistureClass) const;
    MoistureInputMode::MoistureInputModeEnum getMoistureInputMode() const;
    int getNumberOfMoistureScenarios() const;
    int getMoistureScenarioIndexByName(std::string name) const;
    bool getIsMoistureScenarioDefinedByName(std::string name) const;
    std::string getMoistureScenarioDescriptionByName(std::string name) const;
    double getMoistureScenarioOneHourByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioTenHourByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioHundredHourByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioLiveHerbaceousByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioLiveWoodyByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits) const;
    bool getIsMoistureScenarioDefinedByIndex(int index) const;
    std::string getMoistureScenarioNameByIndex(int index) const;
    std::string getMoistureScenarioDescriptionByIndex(int index) const;
    double getMoistureScenarioOneHourByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioTenHourByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioHundredHourByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioLiveHerbaceousByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getMoistureScenarioLiveWoodyByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode) const;
    double getWindDirection() const;
    double getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const;
    double getAspect() const;
    double getCanopyCover(FractionUnits::FractionUnitsEnum coverUnits) const;
    double getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const;
    double getCrownRatio(FractionUnits::FractionUnitsEnum crownRatioUnits) const;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum getWindAndSpreadOrientationMode() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum getWindAdjustmentFactorCalculationMethod() const;

    // Palmetto-Gallberry getters
    bool getIsUsingPalmettoGallberry() const;
    double getAgeOfRough() const;
    double getHeightOfUnderstory(LengthUnits::LengthUnitsEnum heightUnits) const;
    double getPalmettoGallberryCoverage(FractionUnits::FractionUnitsEnum coverageUnits) const;
    double getOverstoryBasalArea(BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits) const;
    double getPalmettoGallberryMoistureOfExtinctionDead(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getPalmettoGallberryHeatOfCombustionDead(HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const;
    double getPalmettoGallberryHeatOfCombustionLive(HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const;
    double getPalmettoGallberyDeadFineFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyDeadMediumFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyDeadFoliageLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyLitterLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyLiveFineFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyLiveMediumFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyLiveFoliageLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getPalmettoGallberyFuelBedDepth(LengthUnits::LengthUnitsEnum depthUnits) const;

    // Western Aspen getters
    bool getIsUsingWesternAspen() const;
    int getAspenFuelModelNumber() const;
    double getAspenCuringLevel(FractionUnits::FractionUnitsEnum curingLevelUnits) const;
    double getAspenDBH(LengthUnits::LengthUnitsEnum dbhUnits) const;
    AspenFireSeverity::AspenFireSeverityEnum getAspenFireSeverity() const;
    double getAspenLoadDeadOneHour(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getAspenLoadDeadTenHour(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getAspenLoadLiveHerbaceous(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getAspenLoadLiveWoody(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getAspenSavrDeadOneHour(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    double getAspenSavrDeadTenHour(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    double getAspenSavrLiveHerbaceous(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    double getAspenSavrLiveWoody(SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const;
    double getAspenMortality(FractionUnits::FractionUnitsEnum mortalityUnits) const;

    // Chaparral getters
    bool getIsUsingChaparral() const;
    ChaparralFuelType::ChaparralFuelTypeEnum getChaparralFuelType() const;
    double getChaparralFuelBedDepth(LengthUnits::LengthUnitsEnum depthUnits) const;
    double getChaparralFuelDeadLoadFraction() const;
    double getChaparralTotalFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralAge(TimeUnits::TimeUnitsEnum ageUnits) const;
    double getChaparralDaysSinceMayFirst() const;
    double getChaparralDeadFuelFraction() const;
    double getChaparralDeadMoistureOfExtinction(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getChaparralLiveMoistureOfExtinction(FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getChaparralDensity(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass, DensityUnits::DensityUnitsEnum densityUnits) const;
    double getChaparralHeatOfCombustion(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const;
    double getChaparralLoadDeadLessThanQuarterInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadDeadQuarterInchToLessThanHalfInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadDeadHalfInchToLessThanOneInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadDeadOneInchToThreeInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadLiveLeaves(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadLiveStemsLessThanQuaterInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadLiveQuarterInchToLessThanHalfInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadLiveHalfInchToLessThanOneInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralLoadLiveOneInchToThreeInch(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralMoisture(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass, FractionUnits::FractionUnitsEnum moistureUnits) const;
    double getChaparralTotalDeadFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;
    double getChaparralTotalLiveFuelLoad(LoadingUnits::LoadingUnitsEnum loadingUnits) const;

protected:
    void memberwiseCopyAssignment(const Surface& rhs);
    double calculateSpreadRateAtVector(double directionOfinterest, SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode);

    const FuelModels* fuelModels_;

    // Surface Module components
    SurfaceInputs surfaceInputs_;
    SurfaceFire surfaceFire_;

    // Size Module
    FireSize size_;
};

#endif // SURFACE_H
