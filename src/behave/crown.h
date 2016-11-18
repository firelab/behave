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

#ifndef CROWN_H
#define CROWN_H

#include "behaveUnits.h"
#include "crownInputs.h"
#include "crownEnums.h"
#include "surface.h"

class FuelModelSet;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModelSet& fuelModelSet, const Surface& surface);
    ~Crown();

    Crown(const Crown &rhs);
    Crown& operator=(const Crown &rhs);

    void doCrownRun();
  
    void updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture);
    void setCanopyBaseHeight(double canopyBaseHeight);
    void setCanopyBulkDensity(double canopyBulkDensity);
    void setMoistureFoliar(double foliarMoisture);
    void setMoistureUnits(MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setCanopyBulkDensityUnits(DensityUnits::DensityUnitsEnum densityUnits);
    void setCanopyBaseHeightUnits(LengthUnits::LengthUnitsEnum canopyBaseHeightUnits);

    double getCanopyBaseHeight() const;
    double getCanopyBulkDensity() const;
    double getMoistureFoliar() const;
    double getCrownFireSpreadRate() const;
    double getCrownFirelineIntensity() const;
    double getCrownFlameLength() const;
    FireType::FireTypeEnum getFireType() const;
    DensityUnits::DensityUnitsEnum getCanopyBulkDensityUnits() const;
    LengthUnits::LengthUnitsEnum getCanopyBaseHeightUnits() const;

private:
    const FuelModelSet* fuelModelSet_;              // pointer to BehaveRun's FuelModelSet object
    CrownInputs crownInputs_;                       // pointer to BehaveRun's CrownInputs object
    
    // SURFACE module components
    const Surface* surface_;                        // pointer to the BehaveRun's Surface object
    Surface crownDeepCopyOfSurface_;                // deep copy of Surface's surface inputs to allow parallel runs in Surface

    // Private methods
    void initializeMemmbers();
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
    void updateDeepCopyOfSurface();
    double calculateWindSpeedAtTwentyFeet();

    // Member variables
    FireType::FireTypeEnum fireType_;               // Classification based on corwn fire active and transition ratios
    double crownCopyOfSurfaceHeatPerUnitArea_;      // Copy of surface hpua used for parallel surface runs (Btu/ft^2)
    double crownCopyOfSurfaceFirelineIntensity_;    // Copy of surface fireline intensity used for parallel surface runs
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
    double windSpeedAtTwentyFeet_;
};

#endif // CROWN_H
