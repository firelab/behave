/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the inputs required for surface fire behavior
*           in the Rothermel Model
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

#ifndef SURFACEINPUTS_H
#define SURFACEINPUTS_H

#include "behaveUnits.h"
#include "fuelModels.h"
#include "moistureScenarios.h"
#include "surfaceInputEnums.h"

class SurfaceInputs
{
public:
    SurfaceInputs();
    SurfaceInputs& operator=(const SurfaceInputs& rhs);

    void initializeMembers();

    // Main Surface module inputs setters 
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed,
        SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, 
        double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, 
        double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits,
        double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureDeadAggregate(double moistureDeadAggregate, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveAggregate(double moistureLiveAggregate, MoistureUnits::MoistureUnitsEnum moistureUnits);
    bool setMoistureScenarioByName(std::string moistureScenarioName);
    bool setMoistureScenarioByIndex(int moistureScenarioIndex);
    void setMoistureInputMode(MoistureInputMode::MoistureInputModeEnum moistureInputMode);
    void setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits);
    void setAspect(double aspect);
    void setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setWindDirection(double windDirection);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode);
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setCanopyCover(double canopyCover, CoverUnits::CoverUnitsEnum coverUnits);
    void setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCrownRatio(double crownRatio);
    void setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor);
    void setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod);
    void setElapsedTime(double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits);
    void setAirTemperature(double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits);
    void setIsCalculatingScorchHeight(bool IsCalculatingScorchHeight);

    // Main Surface module inputs getters 
    int getFuelModelNumber() const;
    double getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureDeadAggregateValue(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveAggregateValue(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    void updateMoisturesBasedOnInputMode();
    double getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits) const;
    double getWindDirection() const;
    double getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const;
    double getAspect() const;
    double getCanopyCover(CoverUnits::CoverUnitsEnum coverUnits) const;
    double getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const;
    double getCrownRatio() const;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum getWindAndSpreadOrientationMode() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;
    double getUserProvidedWindAdjustmentFactor() const;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum getWindAdjustmentFactorCalculationMethod() const;
    double getElapsedTime(TimeUnits::TimeUnitsEnum timeUnits) const;
    double getAirTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits) const;
    bool getIsCalculatingScorchHeight() const;
    bool isMoistureClassInputNeeded(MoistureClassInput::MoistureClassInputEnum moistureSizeClass) const;
    MoistureInputMode::MoistureInputModeEnum getMoistureInputMode() const;
    std::string getCurrentMoistureScenarioName() const;
    int getCurrentMoistureScenarioIndex() const;

    // Two fuel models inputs setters
    void updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
        double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, 
        MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, 
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection, 
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double firstFuelModelCoverage, 
        CoverUnits::CoverUnitsEnum firstFuelModelCoverageUnits, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod, 
        double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover,
        CoverUnits::CoverUnitsEnum canopyCoverUnits, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setFirstFuelModelNumber(int firstFuelModelNumber);
    void setSecondFuelModelNumber(int secondFuelModelNumber);
    void setTwoFuelModelsMethod(TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod);
    void setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage, CoverUnits::CoverUnitsEnum coverUnits);

    // Two fuel models inputs getters
    bool isUsingTwoFuelModels() const;
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum getTwoFuelModelsMethod() const;
    int getFirstFuelModelNumber() const;
    int getSecondFuelModelNumber() const;
    double getFirstFuelModelCoverage() const;

    // Palmetto-Gallberry inputs setters
    void updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
        SpeedUnits::SpeedUnitsEnum windSpeedUnits,WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double ageOfRough,
        double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits, double slope,
       SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setPalmettoGallberryAgeOfRough(double ageOfRough);
    void setPalmettoGallberryHeightOfUnderstory(double heightOfUnderstory, LengthUnits::LengthUnitsEnum heightUnits);
    void setPalmettoGallberryPalmettoCoverage(double palmettoCoverage, CoverUnits::CoverUnitsEnum coverUnits);
    void setPalmettoGallberryOverstoryBasalArea(double overstoryBasalArea, BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits);
    void setIsUsingPalmettoGallberry(bool isUsingPalmettoGallberry);

    // Palmetto-Gallberry getters
    bool getIsUsingPalmettoGallberry() const;
    double getPalmettoGallberryAgeOfRough() const;
    double getPalmettoGallberryHeightOfUnderstory(LengthUnits::LengthUnitsEnum heightUnits) const;
    double getPalmettoGallberryPalmettoCoverage(CoverUnits::CoverUnitsEnum coverUnits) const;
    double getPalmettoGallberryOverstoryBasalArea(BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits) const;
  
    // Westerm Aspen inputs setters
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, CuringLevelUnits::CuringLevelEnum curingLevelUnits,
        AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double dbh, LengthUnits::LengthUnitsEnum dbhUnits, double moistureOneHour, double moistureTenHour,
        double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, 
        double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, 
        double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope, 
        SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setAspenFuelModelNumber(int aspenFuelModelNumber);
    void setAspenCuringLevel(double aspenCuringLevel, CuringLevelUnits::CuringLevelEnum curingLevelUnits);
    void setAspenDBH(double dbh, LengthUnits::LengthUnitsEnum dbhUnits);
    void setAspenFireSeverity(AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity);
    void setIsUsingWesternAspen(bool isUsingWesternAspen);

    // Western Aspen getters
    bool getIsUsingWesternAspen() const;
    int getAspenFuelModelNumber() const;
    double getAspenCuringLevel(CuringLevelUnits::CuringLevelEnum curingLevelUnits) const;
    double getAspenDBH(LengthUnits::LengthUnitsEnum dbhUnits) const;
    AspenFireSeverity::AspenFireSeverityEnum getAspenFireSeverity() const;

    // Chaparral inputs setters
    void setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::ChaparralFuelInputLoadModeEnum fuelLoadInputMode);
    void setChaparralFuelType(ChaparralFuelType::ChaparralFuelTypeEnum chaparralFuelType);
    void setChaparralFuelBedDepth(double chaparralFuelBedDepth, LengthUnits::LengthUnitsEnum depthUnits);
    void setChaparralFuelDeadLoadFraction(double chaparralFuelDeadLoadFraction);
    void setChaparralTotalFuelLoad(double chaparralTotalFuelLoad, LoadingUnits::LoadingUnitsEnum fuelLoadUnits);
    void setIsUsingChaparral(bool isUsingChaparral);

    // Chaparral inputs getters
    ChaparralFuelLoadInputMode::ChaparralFuelInputLoadModeEnum getChaparralFuelLoadInputMode() const;
    ChaparralFuelType::ChaparralFuelTypeEnum getChaparralFuelType() const;
    double getChaparralFuelBedDepth(LengthUnits::LengthUnitsEnum depthUnits) const;
    double getChaparralFuelDeadLoadFraction() const;
    double getChaparralTotalFuelLoad(LoadingUnits::LoadingUnitsEnum fuelLoadUnits) const;
    bool getIsUsingChaparral() const;

    MoistureScenarios* moistureScenarios; // Moisture scenarios (optional list of moisture scenarios to simplify user input 
protected:   
    void memberwiseCopyAssignment(const SurfaceInputs& rhs);
   
    bool isCalculatingScorchHeight_;    // Switch to determine whether scorch height is calculated (requires air temperature to be set)
    int fuelModelNumber_;               // 1 to 256

    // Weather/Terrain inputs
    double airTemperature_;             // air temperature, degrees Fahrenheit
    double windSpeed_;                  // measured wind speed in feet per minute
    double windDirection_;              // degrees, 0-360
    double slope_;                      // gradient 0-600 or degrees 0-80  
    double aspect_;                     // aspect of slope in degrees, 0-360

    // Moisture Inputs
    MoistureInputMode::MoistureInputModeEnum moistureInputMode_;
    double moistureOneHour_;            // 1% to 60%
    double moistureTenHour_;            // 1% to 60%		
    double moistureHundredHour_;        // 1% to 60%
    double moistureLiveHerbaceous_;     // 30% to 300%
    double moistureLiveWoody_;          // 30% to 300%
    double moistureDeadAggregate_;      // Aggregate dead moisture 1% to 60%
    double moistureLiveAggregate_;      // Aggregate live moisture 30% to 300%
    std::string currentMoistureScenarioName_;  // Currently used moisture scenario name
    int currentMoistureScenarioIndex_;         // Currently used moisture scenario vector index
    std::vector<double> moistureValuesBySizeClass_; // Stores moisture values which will be used during surface and crown runs

    // Two Fuel Models inputs
    bool isUsingTwoFuelModels_;         // Whether fire spread calculation is using Two Fuel Models
    int secondFuelModelNumber_;         // 1 to 256, second fuel used in Two Fuel Models
    double firstFuelModelCoverage_;     // percent of landscape occupied by first fuel in Two Fuel Models

    // Palmetto-Gallberry inputs
    bool isUsingPalmettoGallberry_;
    double ageOfRough_;
    double heightOfUnderstory_;
    double palmettoCoverage_;
    double overstoryBasalArea_;

    // Western Aspen inputs
    bool isUsingWesternAspen_;
    int aspenFuelModelNumber_;
    double aspenCuringLevel_;
    double dbh_;
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity_;

    // Chaparral inputs
    ChaparralFuelLoadInputMode::ChaparralFuelInputLoadModeEnum chaparralFuelLoadInputMode_;
    ChaparralFuelType::ChaparralFuelTypeEnum chaparralFuelType_;
    double chaparralFuelBedDepth_;
    double chaparralFuelDeadLoadFraction_;
    double chaparralTotalFuelLoad_;
    bool isUsingChaparral_;

    // For Size Module
    double elapsedTime_;

    // Wind Adjustment Factor Parameters
    double canopyCover_;
    double canopyHeight_;
    double crownRatio_;
    double userProvidedWindAdjustmentFactor_;

    // Input Modes
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod_;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode_;  
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode_;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod_;
    SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum surfaceFireSpreadDirectionMode_;
};

#endif // SURFACEINPUTS_H
