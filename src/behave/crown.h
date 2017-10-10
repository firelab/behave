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

class FuelModelSet;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModelSet& fuelModelSet);
    ~Crown();

    Crown(const Crown &rhs);
    Crown& operator=(const Crown &rhs);

    void doCrownRun();
  
    void initializeMembers();

    // CROWN Module Setters
    void updateCrownInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, double moistureFoliar,
        MoistureUnits::MoistureUnitsEnum  moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode,
        double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, double canopyHeight, double canopyBaseHeight,
        LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio,  double canopyBulkDensity,
        DensityUnits::DensityUnitsEnum densityUnits);
    void setCanopyBaseHeight(double canopyBaseHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCanopyBulkDensity(double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits);
    void setMoistureFoliar(double foliarMoisture, MoistureUnits::MoistureUnitsEnum moistureUnits);

    // Crown Module Getters
    double getCanopyBaseHeight() const;
    double getCanopyBulkDensity() const;
    double getMoistureFoliar(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    //double getSpreadRateBaseOnFireType(SpeedUnits::SpeedUnitsEnum desiredUnits) const;
    double getCrownFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getSurfaceFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getCrownFirelineIntensity() const;
    double getCrownFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const;
    FireType::FireTypeEnum getFireType() const;

    double getCrownFireLengthToWidthRatio() const;

    // SURFACE Module Inputs Setters
    void updateCrownsSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits,
        WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode, double windDirection,
        WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect,
        double canopyCover, double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio);
    void setCanopyCover(double canopyCover);
    void setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits);
    void setCrownRatio(double crownRatio);
    void setFuelModelSet(FuelModelSet& fuelModelSet);
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
    void setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode);
    void setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadAngleMode);

    // SurfaceInputs getters
    int getFuelModelNumber() const;
    double getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const;
    double getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode) const;
    double getWindDirection() const;
    double getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const;
    double getAspect() const;
    double getCanopyCover(CoverUnits::CoverUnitsEnum canopyCoverUnits) const;
    double getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeighUnits) const;
    double getCrownRatio() const;

private:
    const FuelModelSet* fuelModelSet_;
    CrownInputs crownInputs_;
    
    // SURFACE module components
    Surface surface_;

    // Private methods
    void memberwiseCopyAssignment(const Crown& rhs);
    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    void calculateCrownFuelLoad();
    void calculateCrownFirelineIntensity();
    void calculateCrownFlameLength();
    void calculateCrownPowerOfFire();
    void calcuateCrownPowerOfWind();
    void calculateCrownCriticalFireSpreadRate();
    void calculateCrownCriticalSurfaceFireIntensity();
    void calculateCrownCriticalSurfaceFlameLength();
    void calculateCrownFireTransitionRatio();
    void calcualteCrownFirePowerRatio();
    void calculateCrownFireActiveRatio();
    void calculateFireType();
    void calculateWindSpeedAtTwentyFeet();
    void calculateCrownLengthToWidthRatio();

    // Member variables
    FireType::FireTypeEnum fireType_;               // Classification based on corwn fire active and transition ratios
    double crownsSurfaceHeatPerUnitArea_;      // Copy of surface hpua used for parallel surface runs (Btu/ft^2)
    double crownsSurfaceFirelineIntensity_;    // Copy of surface fireline intensity used for parallel surface runs
    double crownFuelLoad_;                          // Crown fire fuel load (lb / ft^2)
    double canopyHeatPerUnitArea_;                  // Canopy heat per unit area (Btu/ft^2)
    double crownFireHeatPerUnitArea_;               // Crown fire heat per unit area (Btu/ft^2)
    double crownFirelineIntensity_;                 // Crown fire fireline intensity (Btu / ft / s)
    double crownFlameLength_;                       // Crown fire flame length (ft)
    double crownFireSpreadRate_;
    double crownCriticalSurfaceFireIntensity_;      // Critical surface fire intensity (Btu / ft / s)
    double crownCriticalFireSpreadRate_;            // Critical crown fire spread rate (ft / min)
    double crownCriticalSurfaceFlameLength_;        // Critical surface fire flame length (ft)
    double crownPowerOfFire_;                       // Crown fire 'power of the fire' ( ft*lb / s / ft^2)
    double crownPowerOfWind_;                       // Crown fire 'power of the wind' ( ft*lb / s / ft^2)
    double crownFirePowerRatio_;                    // Crown fire power ratio
    double crownFireActiveRatio_;                   // Crown fire active ratio
    double crownFireTransitionRatio_;               // Crown fire transition ratio
    double crownFireLengthToWidthRatio_;               // Crown fire transition ratio
    double windSpeedAtTwentyFeet_;
};

#endif // CROWN_H
