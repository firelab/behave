#ifndef SURFACEINPUTS_HEADER
#define SURFACEINPUTS_HEADER

#include <string>
#include <cmath>
#include "surfaceEnums.h"

class SurfaceInputs
{
public:
    SurfaceInputs();

    void initializeMembers();
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous,
        double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
        double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour, double moistureTenHour,
        double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double firstFuelModelCoverage, TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod, double slope, double aspect,
        double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory,
        double palmettoCoverage, double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureDead(double moistureOneHour, double moistureTenHour, double moistureHundredHour);
    void setMoistureLive(double moistureLiveHerbaceous, double moistureLiveWoody);
    void setMoistureOneHour(double moistureOneHour);
    void setMoistureTenHour(double moistureTenHour);
    void setMoistureHundredHour(double moistureHundredHour);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous);
    void setMoistureLiveWoody(double moistureLiveWoody);
    void setSlope(double slope);
    void setSlopeAspect(double slopeAspect);
    void setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode);
    void setWindSpeed(double windSpeed);
    void setWindDirection(double windDirection);
    void setWindAndSpreadAngleMode(WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode);
    void setFirstFuelModelNumber(int firstFuelModelNumber);
    void setSecondFuelModelNumber(int secondFuelModelNumber);
    void setTwoFuelModelsMethod(TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod);

//    void setIsUsingPalmettoGallberry(bool isUsingPalmettoGallberry);
    bool isUsingPalmettoGallberry() const;
    void setAgeOfRough(double ageOfRough);
    double getAgeOfRough() const;
    void setHeightOfUnderstory(double heightOfUnderstory);
    double getHeightOfUnderstory() const;
    void setPalmettoCoverage(double palmettoCoverage);
    double getPalmettoCoverage() const;
    void setOverstoryBasalArea(double overstoryBasalArea);
    double getOverstoryBasalArea() const;

    bool isUsingWesternAspen() const;
    int getAspenFuelModelNumber() const;
    double getAspenCuringLevel() const;

    int getFuelModelNumber() const;
    int getFirstFuelModelNumber() const;
    int getSecondFuelModelNumber() const;
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum getTwoFuelModelsMethod() const;

    bool isWindAndSpreadAngleRelativeToNorth() const;
    bool isWindAndSpreadAngleRelativeToUpslope() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;

    bool isSlopeInDegrees() const;
    bool isSlopeInPercent() const;
    bool isUsingTwoFuelModels() const;
    bool hasUserEnteredWindAdjustmentFactor() const;
    double getMoistureDeadAtIndex(int index) const;
    double getMoistureLiveAtIndex(int index) const;
    double getSlope() const;
    double getAspect() const;
    double getWindDirection() const;
    double getWindSpeed() const;
    double getMoistureOneHour() const;
    double getMoistureTenHour() const;
    double getMoistureHundredHour() const;
    double getMoistureLiveHerbaceous() const;
    double getMoistureLiveWoody() const;
    double getFirstFuelModelCoverage() const;

    double getCanopyCover() const;
    double getCanopyHeight() const;
    double getCrownRatio() const;
    void setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor);
    double getUserProvidedWindAdjustmentFactor() const;

private:
    double convertWindToUpslope(double windDirectionFromNorth);
    void setMoistureDead();
    void setMoistureLive();

    static const int MAX_SIZES = 4;     // Max number of fuel size classes

    int fuelModelNumber_;               // 1 to 256
    double moistureOneHour_;            // 1% to 60%
    double moistureTenHour_;            // 1% to 60%		
    double moistureHundredHour_;        // 1% to 60%
    double moistureLiveHerbaceous_;     // 30% to 300%
    double moistureLiveWoody_;          // 30% to 300%
    double windSpeed_;                  // measured wind speed in miles per hour
    double windDirection_;              // degrees, 0-360
    double slope_;                      // gradient 0-600 or degrees 0-80  
    double aspect_;                     // aspect of slope in degrees, 0-360
    double moistureDead_[MAX_SIZES];    // dead fuel moisture content
    double moistureLive_[MAX_SIZES];    // live fuel moisture content

    // Two Fuel Models
    bool isUsingTwoFuelModels_;         // Whether fuel spread calculation is using Two Fuel Models
    int firstFuelModelNumber_;          // 1 to 256, first fuel used in Two Fuel Models
    int secondFuelModelNumber_;         // 1 to 256, second fuel used in Two Fuel Models
    double firstFuelModelCoverage_;     // percent of landscape occupied by first fuel in Two Fuel Models

    // Palmetto-Gallberry
    bool isUsingPalmettoGallberry_;     // Whether fuel spread calculation is using Palmetto-Gallbery
    double  ageOfRough_;
    double  heightOfUnderstory_;
    double  palmettoCoverage_;
    double  overstoryBasalArea_;

    // Western Aspen
    bool isUsingWesternAspen_;          // Whether fuel spread calculation is using Western Aspen
    int aspenFuelModelNumber_;
    double aspenCuringLevel_;
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity_;

    // Wind Adjustment Factor Parameters
    double canopyCover_;
    double canopyHeight_;
    double crownRatio_;
    double userProvidedWindAdjustmentFactor_;

    // Input Modes
    SlopeInputMode::SlopeInputModeEnum slopeInputMode_;                         // Specifies whether slope is input as percent or degrees
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod_;          // Specifies method used in Two Fuel Models calculations
    WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode_; // Specifies how wind and spread directions are referenced
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode_;          // Specifies the height above canopy from which wind speed is measured
};

#endif // SURFACEINPUTS_HEADER
