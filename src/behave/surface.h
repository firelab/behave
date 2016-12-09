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
#include "fuelModelSet.h"
#include "surfaceFire.h"
#include "surfaceInputs.h"

class Surface
{
public:
    Surface() = delete; // no default constructor
    Surface(const FuelModelSet& fuelModelSet);
    Surface(const Surface& rhs);
    Surface& operator=(const Surface& rhs);

    bool isAllFuelLoadZero(int fuelModelNumber);
    void doSurfaceRunInDirectionOfMaxSpread();
    void doSurfaceRunInDirectionOfInterest(double directionOfinterest);
    double calculateSpreadRateAtVector(double directionOfinterest);
    double calculateFlameLength(double firelineIntensity);

    // SurfaceFire getters
    double getSpreadRate() const;
    double getDirectionOfMaxSpread() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getFirelineIntensity() const;
    double getHeatPerUnitArea() const;
    double getMidflameWindspeed() const;
    double getResidenceTime() const;
    double getReactionIntensity() const;
    double getEllipticalA() const;
    double getEllipticalB() const;
    double getEllipticalC() const;
   
    // SurfaceInputs setters
    void setTreeAndCanopyHeightUnits(LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCanopyHeight(double canopyHeight);
    void setCoverUnits(CoverUnits::CoverUnitsEnum coverUnits);
    void setCanopyCover(double canopyCover);
    void setCrownRatio(double crownRatio);
    bool isUsingTwoFuelModels() const;
    void setFuelModelNumber(int fuelModelNumber);
    void setMoistureUnits(MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setSlope(double slope);
    void setAspect(double aspect);
    void setSlopeUnits(SlopeUnits::SlopeUnitsEnum slopeInputMode);
    void setWindSpeed(double windSpeed);
    void setWindSpeedUnits(SpeedUnits::SpeedUnitsEnum windSpeedUnits);
    void setSpreadRateUnits(SpeedUnits::SpeedUnitsEnum spreadRateUnits);
    void setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor);
    void setWindDirection(double windDirection);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode);
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setFirstFuelModelNumber(int firstFuelModelNumber);
    void setSecondFuelModelNumber(int secondFuelModelNumber);
    void setTwoFuelModelsMethod(TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod);
    void setTwoFuelModelsFirstFuelModelCoverage(double firstFuelModelCoverage);
    void setFlameLengthUnits(LengthUnits::LengthUnitsEnum flameLengthUnits);
    void setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod);
    void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection, double slope,
        double aspect, double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForTwoFuelModels(int firstfuelModelNumber, int secondFuelModelNumber, double moistureOneHour,
        double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits,
        double windSpeed, double windDirection, double firstFuelModelCoverage, TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod, double slope, double aspect,
        double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForPalmettoGallbery(double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection, double ageOfRough,
        double heightOfUnderstory, double palmettoCoverage,double overstoryBasalArea, double slope, double aspect, 
        double canopyCover, double canopyHeight, double crownRatio);
    void updateSurfaceInputsForWesternAspen(int aspenFuelModelNumber, double aspenCuringLevel, AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity, double DBH,
        double moistureOneHour, double moistureTenHour, double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, 
        MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, double windDirection, double slope, double aspect, double canopyCover, double canopyHeight, 
        double crownRatio);

    // SurfaceInputs getters
	int getFuelModelNumber() const;
    MoistureUnits::MoistureUnitsEnum getMoistureUnits() const;
    double getMoistureOneHour() const;
    double getMoistureTenHour() const;
    double getMoistureHundredHour() const;
    double getMoistureLiveHerbaceous() const;
    double getMoistureLiveWoody() const;
    double getWindSpeed() const;
    double getWindDirection() const;
    double getSlope() const;
    double getAspect() const;
    CoverUnits::CoverUnitsEnum getCoverUnits() const;
    double getCanopyCover() const;
    double getCanopyHeight() const;
    double getCrownRatio() const;
    LengthUnits::LengthUnitsEnum getFlameLengthUnits() const;
    LengthUnits::LengthUnitsEnum getCanopyHeightUnits() const;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum getWindAndSpreadOrientationMode() const;
    WindHeightInputMode::WindHeightInputModeEnum getWindHeightInputMode() const;
    SlopeUnits::SlopeUnitsEnum getSlopeUnits() const;
    SpeedUnits::SpeedUnitsEnum getWindSpeedUnits() const;
    SpeedUnits::SpeedUnitsEnum getSpreadRateUnits() const;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum getWindAdjustmentFactorCalculationMethod() const;

private:
    void memberwiseCopyAssignment(const Surface& rhs);

    const FuelModelSet* fuelModelSet_;

    // Surface Module components
    SurfaceInputs surfaceInputs_;
    SurfaceFire surfaceFire_;
};

#endif // SURFACE_H
