// Interface for Behave application based on the Facade OOP Design Pattern
// It is used to tie together the modules and objects used by Behave - WMC 10/2015

// TODO: Need to revisit how the Two Fuel Models module is organized and handled - WMC 02/2016


#ifndef BEHAVE_HEADER
#define BEHAVE_HEADER

#include "fuelModels.h"
#include "surface.h"

class Behave
{
public:
    Behave();
    explicit Behave(SurfaceInputs &surfaceInputs);
    ~Behave();

    // SURFACE Module Inputs 
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber,
        double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double firstFuelModelCoverage, TwoFuelModelsMethod::TwoFuelModelsMethodEnum method,
        double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void  updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
        double windSpeed, double windDirection, double ageOfRough, double heightOfUnderstory, double palmettoCoverage,
        double overstoryBasalArea, double slope, double aspect, double canopyCover, double canopyHeight, double crownRatio);
    double calculateSurfaceFireForwardSpreadRate(double directionOfInterest = -1.0);

    // SURFACE Module Getters
    double getDirectionOfMaxSpread() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    bool isFuelModelDefined(int fuelModelNumber) const;
    bool isWindAndSpreadAngleRelativeToNorth() const;
    bool isWindAndSpreadAngleRelativeToUpslope() const;
    bool isSlopeInDegrees() const;
    bool isSlopeInPercent() const;

    // SURFACE Module Setters
    void setWindAndSpreadAnglesRelativeToNorth();
    void setWindAndSpreadAnglesRelativeToUpslope();
    void setSlopeInputToPercent();
    void setSlopeInputToDegrees();

private:
    // SURFACE Module Private Setters
    void setSlopeInputMode(SlopeInputMode::SlopeInputModeEnum slopeInputMode);
    void setWindAndSpreadDirectionMode(WindAndSpreadAngleMode::WindAndSpreadAngleModeEnum windAndSpreadAngleMode);

    // SURFACE Module Component Objects
    FuelModels fuelModels_;         // Object containing data for fuel models
    Surface surface_;               // SURFACE Moduel object
    SurfaceInputs surfaceInputs_;   // Object that manages user input to SURFACE Module
};

#endif //BEHAVE_HEADER
