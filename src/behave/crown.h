/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling crown fire behavior
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

// TODO: Add unit conversions for energy and incorporate into calculateCrownCriticalSurfaceFireIntensity() - WMC 11/16
// TODO: Allow for use case in which Crown is run completely without Surface, will involve allowing direct input of HPUA 
//       and surface flame length, as well as setting all other pertinent surface inputs in Crown's copy of Surface - WMC 11/16

#ifndef CROWN_H
#define CROWN_H

#include "behaveUnits.h"
#include "crownInputs.h"
#include "surface.h"

class FuelModels;

struct FireType
{
    enum FireTypeEnum
    {
        Surface = 0,    // surface fire with no torching or crown fire spread.
        Torching = 1,   // surface fire with torching.
        ConditionalCrownFire = 2, // active crown fire possible if the fire transitions to the overstory        
        Crowning = 3    // active crown fire, fire is spreading through the canopy.
    };
};

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(FuelModels& fuelModels);
    ~Crown();

    Crown(const Crown& rhs);
    Crown& operator=(const Crown& rhs);

    void doCrownRunRothermel();
    void doCrownRunScottAndReinhardt();
    void initializeMembers();

    void setFuelModels(FuelModels& fuelModels);

    // CROWN Module Setters
    void updateCrownInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, double moistureFoliar,
        MoistureUnits::MoistureUnitsEnum  moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode,
        double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits,
        double canopyHeight, double canopyBaseHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio,
        double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits);
    void setCanopyBaseHeight(double canopyBaseHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCanopyBulkDensity(double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits);
    void setMoistureFoliar(double foliarMoisture, MoistureUnits::MoistureUnitsEnum moistureUnits);

    // Crown Module Getters
    double getCanopyBaseHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const;
    double getCanopyBulkDensity(DensityUnits::DensityUnitsEnum canopyBulkDensityUnits) const;
    double getMoistureFoliar(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getCrownFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getCrownFireSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getSurfaceFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getSurfaceFireSpreadDistance(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getCrownFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    double getCrownFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    FireType::FireTypeEnum getFireType() const;

    double getFinalSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getFinalHeatPerUnitArea(HeatPerUnitAreaUnits::HeatPerUnitAreaUnitsEnum heatPerUnitAreaUnits) const;
    double getFinalFirelineIntesity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    double getFinalFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;

    double getCrownFireLengthToWidthRatio() const;
    double getCrownFireArea(AreaUnits::AreaUnitsEnum areaUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getCrownFirePerimeter(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getCriticalOpenWindSpeed(SpeedUnits::SpeedUnitsEnum speedUnits) const;
    double getCrownFractionBurned() const;

    // Fuel Model Getter Methods
    std::string getFuelCode(int fuelModelNumber) const;
    std::string getFuelName(int fuelModelNumber) const;
    double getFuelbedDepth(int fuelModelNumber, LengthUnits::LengthUnitsEnum lengthUnits) const;
    double getFuelMoistureOfExtinctionDead(int fuelModelNumber, MoistureUnits::MoistureUnitsEnum moistureUnits) const;;
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

    // SURFACE Module Inputs Setters
    void updateCrownsSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed,
        SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
        SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setCanopyCover(double canopyCover, CoverUnits::CoverUnitsEnum coverUnits);
    void setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCrownRatio(double crownRatio);
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureDeadAggregate(double moistureDead, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveAggregate(double moistureLive, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureScenarios(MoistureScenarios& moistureScenarios);
    bool setCurrentMoistureScenarioByName(std::string moistureScenarioName);
    bool setCurrentMoistureScenarioByIndex(int moistureScenarioIndex);
    void setMoistureInputMode(MoistureInputMode::MoistureInputModeEnum moistureInputMode);
    void setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits);
    void setAspect(double aspect);
    void setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setWindDirection(double windDirection);
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadAngleMode);
    void setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor);
    void setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod);

    // SurfaceInputs getters
    int getFuelModelNumber() const;
    double getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    int getNumberOfMoistureScenarios();
    int getMoistureScenarioIndexByName(const std::string name);
    bool getIsMoistureScenarioDefinedByName(const std::string name);
    std::string getMoistureScenarioDescriptionByName(const std::string name);
    double getMoistureScenarioOneHourByName(const std::string name);
    double getMoistureScenarioTenHourByName(const std::string name);
    double getMoistureScenarioHundredHourByName(const std::string name);
    double getMoistureScenarioLiveHerbaceousByName(const std::string name);
    double getMoistureScenarioLiveWoodyByName(const std::string name);
    bool getIsMoistureScenarioDefinedByIndex(const int index);
    std::string getMoistureScenarioNameByIndex(const int index);
    std::string getMoistureScenarioDescriptionByIndex(const int index);
    double getMoistureScenarioOneHourByIndex(const int index);
    double getMoistureScenarioTenHourByIndex(const int index);
    double getMoistureScenarioHundredHourByIndex(const int index);
    double getMoistureScenarioLiveHerbaceousByIndex(const int index);
    double getMoistureScenarioLiveWoodyByIndex(const int index);
    double getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode) const;
    double getWindDirection() const;
    double getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const;
    double getAspect() const;
    double getCanopyCover(CoverUnits::CoverUnitsEnum canopyCoverUnits) const;
    double getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeighUnits) const;
    double getCrownRatio() const;

protected:
    struct CrownModelType
    {
        enum CrownModelTypeEnum
        {
            rothermel,
            scott_and_reinhardt
        };
    };
    FuelModels* fuelModels_;
    CrownInputs crownInputs_;

    // SURFACE module components
    Surface surfaceFuel_;
    Surface crownFuel_;

    // SIZE
    FireSize crownFireSize_;

    // Private methods
    void memberwiseCopyAssignment(const Crown& rhs);
    void calculateCrownFireActiveWindSpeed();
    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    void calculateCrownFuelLoad();
    void calculateCrownFirelineIntensity();
    void calculateCrownFlameLength();
    void calculatePassiveCrownFlameLength();
    void calculateSurfaceFireCriticalSpreadRateScottAndReinhardt();
    void calculateCrownCriticalFireSpreadRate();
    void calculateCrownCriticalSurfaceFireIntensity();
    void calculateCrownCriticalSurfaceFlameLength();
    void calculateCrownFireTransitionRatio();
    void calculateCrownFireActiveRatio();
    void calculateFireTypeRothermel();
    void calculateFireTypeScottAndReinhardt();
    void calculateWindSpeedAtTwentyFeet();
    void calculateCrowningSurfaceFireRateOfSpread();
    void calculateCrownFractionBurned();
    void assignFinalFireBehaviorBasedOnFireType(CrownModelType::CrownModelTypeEnum);

    // Member variables
    FireType::FireTypeEnum fireType_;               // Classification based on corwn fire active and transition ratios
    double surfaceFireHeatPerUnitArea_;             // Surface fire hpua used for parallel surface runs (Btu/ft^2)
    double surfaceFirelineIntensity_;               // Surface fireline intensity used for parallel surface runs
    double surfaceFireSpreadRate_;
    double surfaceFireFlameLength_;
    double surfaceFireCriticalSpreadRate_;
    double crownFuelLoad_;                          // Crown fire fuel load (lb / ft^2)
    double canopyHeatPerUnitArea_;                  // Canopy heat per unit area (Btu/ft^2)
    double crownFireHeatPerUnitArea_;               // Crown fire heat per unit area (Btu/ft^2)
    double crownFirelineIntensity_;                 // Crown fire fireline intensity (Btu / ft / s)
    double crownFlameLength_;                       // Crown fire flame length (ft)
    double crownFireSpreadRate_;
    double crownCriticalSurfaceFirelineIntensity_;  // Crown fire's critical surface fire intensity (Btu / ft / s)
    double crownCriticalFireSpreadRate_;            // Crown fire's critical crown fire spread rate (ft / min)
    double crownCriticalSurfaceFlameLength_;        // Crown fire's critical surface fire flame length (ft)
    double crownFireActiveRatio_;                   // Crown fire active ratio
    double crownFireTransitionRatio_;               // Crown fire transition ratio
    double crownFireLengthToWidthRatio_;            // Crown fire length-to-width ratio
    double crownFireActiveWindSpeed_;               // 20 ft windspeed at which active crowning is possible (ft/min)
    double crownFractionBurned_;
    double crowningSurfaceFireRos_;                 // Surface fire spread rate at which the active crown fire spread rate is fully achieved (ft/min)
    double windSpeedAtTwentyFeet_;

    double finalSpreadRate_;                        // "Actual" spread rate of the fire, depends on fire type
    double finalHeatPerUnitArea_;                   // "Actual" fire heat per unit area, depends on fire type
    double finalFirelineIntesity_;                  // "Actual" fireline intensity, depends on fire type
    double finalFlameLength_;                       // "Actual" flame length, depends on fire type

    double passiveCrownFireSpreadRate_;
    double passiveCrownFireHeatPerUnitArea_;
    double passiveCrownFireLineIntensity_;
    double passiveCrownFireFlameLength_;

    bool isSurfaceFire_;
    bool isPassiveCrownFire_;
    bool isActiveCrownFire_;
    bool isCrownFire_;
};

#endif // CROWN_H
