#ifndef SURFACE_HEADER
#define SURFACE_HEADER

// The SURFACE module of BehavePlus
#include "surfaceFireSpread.h"
#include "surfaceInputs.h"

class Surface
{
public:
    Surface(const FuelModelSet& fuelModelSet);
    Surface(const Surface &rhs);
    Surface& operator= (const Surface& rhs);

    double calculateSurfaceFireForwardSpreadRate(double directionOfinterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfinterest);
    double getSpreadRate() const;
    double getDirectionOfMaxSpread() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getFirelineIntensity() const;
    double getHeatPerUnitArea() const;
    double getMidflameWindspeed() const;
    double getEllipticalA() const;
    double getEllipticalB() const;
    double getEllipticalC() const;
   
    // Surface Inputs setters
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureOneHour(double moistureOneHour);
    void setMoistureTenHour(double moistureTenHour);
    void setMoistureHundredHour(double moistureHundredHour);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous);
    void setMoistureLiveWoody(double moistureLiveWoody);
    void setSlope(double slope);
    void setAspect(double aspect);
    void setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode);
    void setWindSpeed(double windSpeed);
    void setWindDirection(double windDirection);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode);
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setFirstFuelModelNumber(int firstFuelModelNumber);
    void setSecondFuelModelNumber(int secondFuelModelNumber);
    void setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod);
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
        double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection,
        double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect,
        double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
        double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel,
        AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH, double moistureOneHour, double moistureTenHour,
        double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windSpeed, double windDirection, double slope,
        double aspect, double canopyCover, double canopyHeight, double crownRatio);

    // Surface Inputs getters
    double getCanopyHeight() const;
    double getWindSpeed() const;
    const SurfaceInputs& getSurfaceInputs() const;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum getWindAndSpreadOrientationMode() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;
    SlopeInputMode::SlopeInputModeEnum getSlopeInputMode() const;

private:
    bool isUsingTwoFuelModels() const;

    // SURFACE Module component objects
    const FuelModelSet*	fuelModelSet_;
    SurfaceFireSpread surfaceFireSpread_;
    SurfaceInputs surfaceInputs_;
};

#endif //SURFACEFIRE_HEADER
