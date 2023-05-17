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
#include "surfaceTwoFuelModels.h"

struct MoistureClassInput
{
    enum MoistureClassInputEnum
    {
        OneHour = 0,               // Associated with dead fuel 1 hour moisture
        TenHour = 1,               // Associated with dead fuel 10 hour moisture
        HundredHour = 2,           // Associated with dead fuel 100 hour moisture
        LiveHerbaceous = 3,        // Associated with live fuel herbaceous moisture
        LiveWoody = 4,             // Associated with live  fuel woody moisture
        DeadAggregate = 5,         // Associated with aggregate dead fuel moisture
        LiveAggregate = 6          // Associated with aggregate live fuel moisture
    };
};

struct LiveFuelMoistureInput
{
    enum LiveFuelMoistureInputEnum
    {
       
        Aggregate = 2               // Index associated with aggregate live fuel moisture
    };
};

struct AspenFireSeverity
{
    enum AspenFireSeverityEnum
    {
       Low = 0,
       Moderate = 1
    };
};

struct WindAdjustmentFactorShelterMethod
{
    enum WindAdjustmentFactorShelterMethodEnum
    {
        Unsheltered = 0,            // Wind adjustment factor was calculated using unsheltered method
        Sheltered = 1,              // Wind adjustment factor was calculated using sheltered method
    };
};

struct WindAdjustmentFactorCalculationMethod
{
    enum WindAdjustmentFactorCalculationMethodEnum
    {
        UserInput = 0,             // User enters wind adjustment factor directly
        UseCrownRatio = 1,        // Use crown ratio when calculating wind adjustment factor
        DontUseCrownRatio = 2    // Don't use crown ratio when calculating wind adjustment factor
    };
};

struct WindAndSpreadOrientationMode
{
    enum WindAndSpreadOrientationModeEnum
    {
        RelativeToUpslope = 0,    // Wind and spread angles I/O are clockwise relative to upslope
        RelativeToNorth = 1       // Wind direction angles I/O are clockwise relative to compass north
    };
};

struct WindHeightInputMode
{
    enum WindHeightInputModeEnum
    {
        DirectMidflame = 0,    // User enters midflame wind speed directy
        TwentyFoot = 1,        // User enters the 20 foot wind speed
        TenMeter = 2           // User enters the 10 meter wind speed
    };
};

struct MoistureInputMode
{
    enum MoistureInputModeEnum
    {
        BySizeClass = 0,                    // User enters moisture for each size class
        AllAggregate = 1,                   // User enters aggregate moisture for dead and live fuels
        DeadAggregateAndLiveSizeClass = 2,  // User enters aggregate moisture for dead fuels and for each live size class
        LiveAggregateAndDeadSizeClass = 3,  // User enters aggregate moisture for live fuels and for each dead size class
        MoistureScenario = 4                // User enters a moisture scenario           
    };
};

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
    double getWindSpeed() const;
    double getWindDirection() const;
    double getSlope() const;
    double getAspect() const;
    double getCanopyCover() const;
    double getCanopyHeight() const;
    double getCrownRatio() const;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum getWindAndSpreadOrientationMode() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;
    double getUserProvidedWindAdjustmentFactor() const;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum getWindAdjustmentFactorCalculationMethod() const;
    double getElapsedTime() const;
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
        double heightOfUnderstory, double palmettoCoverage, double overstoryBasalArea, double slope,
       SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setAgeOfRough(double ageOfRough);
    void setHeightOfUnderstory(double heightOfUnderstory);
    void setPalmettoCoverage(double palmettoCoverage);
    void setOverstoryBasalArea(double overstoryBasalArea);
   
    // Palmetto-Gallberry getters
    double getAgeOfRough() const;
    double getHeightOfUnderstory() const;
    double getPalmettoCoverage() const;
    double getOverstoryBasalArea() const;
    bool isUsingPalmettoGallberry() const;

    // Westerm Aspen inputs setters
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
        AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
        double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, 
        double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, 
        double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope, 
        SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setAspenFuelModelNumber(int aspenFuelModelNumber);
    void setAspenCuringLevel(double aspenCuringLevel);
    void setAspenDBH(double DBH);
    void setAspenFireSeverity(AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity);

    // Western Aspen getters
    bool isUsingWesternAspen() const;
    int getAspenFuelModelNumber() const;
    double getAspenCuringLevel() const;
    double getAspenDBH() const;
    AspenFireSeverity::AspenFireSeverityEnum getAspenFireSeverity() const;

    MoistureScenarios* moistureScenarios; // Moisture scenarios (optional list of moisture scenarios to simplify user input 
private:   
    void memberwiseCopyAssignment(const SurfaceInputs& rhs);
   
    int fuelModelNumber_;               // 1 to 256

    // Weather/Terrain inputs
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
    double DBH_;
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity_;

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
};

#endif // SURFACEINPUTS_H
