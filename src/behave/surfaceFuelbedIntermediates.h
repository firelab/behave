/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating intermediate values used in Rothermel Model
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

#ifndef SURFACEFUELBEDINTERMEDIATES_H
#define SURFACEFUELBEDINTERMEDIATES_H

#include "palmettoGallberry.h" 
#include "westernAspen.h"
#include "surfaceInputs.h"

class FuelModelSet;
class SurfaceInputs;

class SurfaceFuelbedIntermediates
{
public:
    SurfaceFuelbedIntermediates();
    SurfaceFuelbedIntermediates(const SurfaceFuelbedIntermediates& rhs);
    SurfaceFuelbedIntermediates& operator=(const SurfaceFuelbedIntermediates& rhs);
    SurfaceFuelbedIntermediates(const FuelModelSet& fuelModelSet, const SurfaceInputs& surfaceInputs);

    ~SurfaceFuelbedIntermediates();
    void calculateFuelbedIntermediates(int fuelModelNumber);

    // Public Getters
    double getFuelbedDepth() const;
    double getBulkDensity() const;
    double getPackingRatio()const;
    double getPropagatingFlux() const;
    double getRelativePackingRatio() const;
    double getSigma() const;
    double getHeatSink() const;
    double getWeightedMoistureByLifeState(int lifeState) const;
    double getMoistureOfExtinctionByLifeState(int lifeState) const;
    double getWeightedHeatByLifeState(int lifeState) const;
    double getWeightedSilicaByLifeState(int lifeState) const;
    double getWeightedFuelLoadByLifeState(int lifeState) const;

    // Palmetto-Gallberry
    double getPalmettoGallberyDeadOneHourLoad() const;
    double getPalmettoGallberyDeadTenHourLoad() const;
    double getPalmettoGallberyDeadFoliageLoad() const;
    double getPalmettoGallberyFuelBedDepth() const;
    double getPalmettoGallberyLitterLoad() const;
    double getPalmettoGallberyLiveOneHourLoad() const;
    double getPalmettoGallberyLiveTenHourLoad() const;
    double getPalmettoGallberyLiveFoliageLoad() const;

    // Western Aspen
    double getAspenMortality() const;

private:
    void initializeMembers();
    void memberwiseCopyAssignment(const SurfaceFuelbedIntermediates& rhs);
    void setFuelLoad();
    void setMoistureContent();
    void setDeadFuelMoistureOfExtinction();
    void setFuelbedDepth();
    void setSAV();
    void countSizeClasses();
    void dynamicLoadTransfer();
    void calculateFractionOfTotalSurfaceAreaForLifeStates();
    void calculateTotalSurfaceAreaForLifeState(int lifeCategory);
    void calculateFractionOfTotalSurfaceAreaForSizeClasses(int lifeCategory);
    void sumFractionOfTotalSurfaceAreaBySizeClass(const double areaWeightingFactorDeadOrLive[FuelConstants::MAX_PARTICLES],
        const double savrDeadOrLive[FuelConstants::MAX_PARTICLES], double summedWeightingFactors[FuelConstants::MAX_PARTICLES]);
    void assignFractionOfTotalSurfaceAreaBySizeClass(const double savrDeadOrLive[FuelConstants::MAX_PARTICLES],
        const double summedWeightingFactors[FuelConstants::MAX_PARTICLES], double sizeSortedWeightingFactorsDeadOrLive[FuelConstants::MAX_PARTICLES]);
    void setHeatOfCombustion();
    void calculateCharacteristicSAVR();
    void calculateHeatSink();
    void calculateLiveMoistureOfExtinction();
    void calculatePropagatingFlux();

    const FuelModelSet* fuelModelSet_;      // Pointer to FuelModelSet object
    const SurfaceInputs* surfaceInputs_;    // Pointer to surfaceInputs object
    PalmettoGallberry palmettoGallberry_;
    WesternAspen westernAspen_;

    // Member variables
    int numberOfSizeClasses_[FuelConstants::MAX_LIFE_STATES];                           // Number of size classes in the currently used fuel model
    double depth_;                                                                      // Depth of fuelbed in feet
    double weightedMoisture_[FuelConstants::MAX_LIFE_STATES];                           // Weighted moisture content for both live and dead fuels
    double totalSurfaceArea_[FuelConstants::MAX_LIFE_STATES];                           // Total surface area for both live and dead fuels
    double weightedHeat_[FuelConstants::MAX_LIFE_STATES];                               // Weighted heat content for both live and dead fuels
    double weightedSilica_[FuelConstants::MAX_LIFE_STATES];                             // Weighted silica content for both live and dead fuels
    double weightedFuelLoad_[FuelConstants::MAX_LIFE_STATES];                           // Weighted fuel loading for both live and dead fuels
    double moistureOfExtinction_[FuelConstants::MAX_LIFE_STATES];                       // Moisture of extinction for both live and dead fuels
    double fractionOfTotalSurfaceArea_[FuelConstants::MAX_LIFE_STATES];                 // Ratio of surface area to total surface area
    double fuelDensity_[FuelConstants::MAX_LIFE_STATES];                                // Fuel density for live and dead fuels
    double totalLoadForLifeState_[FuelConstants::MAX_LIFE_STATES];                      // Total fuel load for live and dead fuels
    double moistureDead_[FuelConstants::MAX_PARTICLES];                                 // Moisture content for dead fuels by size class
    double moistureLive_[FuelConstants::MAX_PARTICLES];                                 // Moisture content for live fuels by size class
    double loadDead_[FuelConstants::MAX_PARTICLES];			        			        // Fuel load for dead fuels by size class
    double loadLive_[FuelConstants::MAX_PARTICLES];					        	        // Fuel load for live fuels by size class
    double savrDead_[FuelConstants::MAX_PARTICLES];				    		            // Surface area to volume ratio for dead fuels by size class
    double savrLive_[FuelConstants::MAX_PARTICLES];                                     // Surface area to volume ratio for live fuels by size class
    double surfaceAreaDead_[FuelConstants::MAX_PARTICLES];                              // Surface area for dead size classes 
    double surfaceAreaLive_[FuelConstants::MAX_PARTICLES];                              // Surface area for live size classes
    double heatDead_[FuelConstants::MAX_PARTICLES];                                     // Heat of combustion for dead size classes
    double heatLive_[FuelConstants::MAX_PARTICLES];                                     // Heat of combustion for live size classes
    double silicaEffectiveDead_[FuelConstants::MAX_PARTICLES];                          // Effective silica constent for dead size classes
    double silicaEffectiveLive_[FuelConstants::MAX_PARTICLES];                          // Effective silica constent for live size classes
    double fractionOfTotalSurfaceAreaDead_[FuelConstants::MAX_PARTICLES];               // Fraction of surface area for dead size classes
    double fractionOfTotalSurfaceAreaLive_[FuelConstants::MAX_PARTICLES];               // Fraction of surface area for live size classes
    double sizeSortedFractionOfSurfaceAreaDead_[FuelConstants::MAX_SAVR_SIZE_CLASSES];  // Intermediate fuel weighting values for dead fuels
    double sizeSortedFractionOfSurfaceAreaLive_[FuelConstants::MAX_SAVR_SIZE_CLASSES];  // Intermediate fuel weighting values for live fuels

    bool isUsingPalmettoGallberry_;
    bool isUsingWesternAspen_;

    int fuelModelNumber_;           // The number associated with the current fuel model being used
    double liveFuelMois_;           // Live fuel moisture content
    double liveFuelMext_;           // Live fuel moisture of extinction 
    double heatSink_;               // Rothermel 1972, Denominator of equation 52
    double sigma_;                  // Fuelbed characteristic SAVR, Rothermel 1972 
    double bulkDensity_;            // Ovendry bulk density in lbs/ft^2, Rothermale 1972, equation 40
    double packingRatio_;           // Packing ratio, Rothermel 1972, equation 31 
    double relativePackingRatio_;   // Packing ratio divided by the optimum packing ratio, Rothermel 1972, term in RHS equation 47
    double totalSilicaContent_;     // Total silica content in percent, Albini 1976, p. 91
    double propagatingFlux_;
};

#endif	// SURFACEFUELBEDINTERMEDIATES_H
