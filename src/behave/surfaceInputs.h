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
//#include "surfaceEnums.h"

struct AspenFireSeverity
{
    enum AspenFireSeverityEnum
    {
       Low = 0,
       Mmoderate = 1
    };
};

struct TwoFuelModelsMethod
{
    enum TwoFuelModelsMethodEnum
    {
        NoMethod = 0,          // Don't use TwoFuel Models method
        Arithmetic = 1,         // Use arithmetic mean
        Harmonic = 2,           // Use harmoic mean
        TwoFimensional = 3     // Use Finney's two dimensional method
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

class SurfaceInputs
{
public:
    SurfaceInputs();
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
    double getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
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

    struct FuelConstants
    {
        enum FuelConstantsEnum
        {
            DEAD = 0,                   // Index associated with dead fuels
            LIVE = 1,                   // Index associated with live fuels
            MAX_LIFE_STATES = 2,        // Number of life states, live and dead
            MAX_LIVE_SIZE_CLASSES = 3,  // Maximum number of live size classes
            MAX_DEAD_SIZE_CLASSES = 4,  // Maximum number of dead size classes
            MAX_PARTICLES = 4,          // Maximum number of size classes within a life state (dead/live)
            MAX_SAVR_SIZE_CLASSES = 5,  // Maximum number of SAVR size classes
            NUM_FUEL_MODELS = 267       // Maximum number of fuel models
        };
    };

    struct TwoFuelModelsContants
    {
        enum TwoFuelModelsContantsEnum
        {
            FIRST = 0,              // Index of the first fuel model
            SECOND = 1,             // Index of the second fuel model
            NUMBER_OF_MODELS = 2,   // Numbe of fuel models used in TwoFuel Models method
        };
    };

private:   
    // Main Suface module inputs
    int fuelModelNumber_;               // 1 to 256
    double moistureOneHour_;            // 1% to 60%
    double moistureTenHour_;            // 1% to 60%		
    double moistureHundredHour_;        // 1% to 60%
    double moistureLiveHerbaceous_;     // 30% to 300%
    double moistureLiveWoody_;          // 30% to 300%
    double windSpeed_;                  // measured wind speed in feet per minute
    double windDirection_;              // degrees, 0-360
    double slope_;                      // gradient 0-600 or degrees 0-80  
    double aspect_;                     // aspect of slope in degrees, 0-360

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
