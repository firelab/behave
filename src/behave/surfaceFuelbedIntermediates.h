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

#include "chaparralFuel.h"
#include "palmettoGallberry.h" 
#include "westernAspen.h"
#include "surfaceInputs.h"
#include "fuelModels.h"

class SurfaceFuelbedIntermediates
{
public:
    SurfaceFuelbedIntermediates();
    SurfaceFuelbedIntermediates(const SurfaceFuelbedIntermediates& rhs);
    SurfaceFuelbedIntermediates& operator=(const SurfaceFuelbedIntermediates& rhs);
    SurfaceFuelbedIntermediates(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs);

    ~SurfaceFuelbedIntermediates();
    void calculateFuelbedIntermediates(int fuelModelNumber);
    void calculateWesternAspenMortality(double flameLength);

    // Getters
    double getFuelbedDepth() const;
    double getBulkDensity() const;
    double getPackingRatio()const;
    double getPropagatingFlux() const;
    double getRelativePackingRatio() const;
    double getSigma() const;
    double getHeatSink() const;
    double getWeightedMoistureByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const;
    double getMoistureOfExtinctionByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const;
    double getWeightedHeatByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const;
    double getWeightedSilicaByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const;
    double getWeightedFuelLoadByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const;

    // Palmetto-Gallberry getters
    double getPalmettoGallberryMoistureOfExtinctionDead() const;
    double getPalmettoGallberryHeatOfCombustionDead() const;
    double getPalmettoGallberryHeatOfCombustionLive() const;
    double getPalmettoGallberyDeadFineFuelLoad() const;
    double getPalmettoGallberyDeadMediumFuelLoad() const;
    double getPalmettoGallberyDeadFoliageLoad() const;
    double getPalmettoGallberyFuelBedDepth() const;
    double getPalmettoGallberyLitterLoad() const;
    double getPalmettoGallberyLiveFineFuelLoad() const;
    double getPalmettoGallberyLiveTenMediumFuelLoad() const;
    double getPalmettoGallberyLiveFoliageLoad() const;

    // Western Aspen getters
    double getAspenMortality() const;
    double getAspenFuelBedDepth(int typeIndex) const; 
    double getAspenHeatOfCombustionDead() const;
    double getAspenHeatOfCombustionLive() const;
    double getAspenMoistureOfExtinctionDead() const;
    double getAspenLoadDeadOneHour() const;
    double getAspenLoadDeadTenHour() const;
    double getAspenLoadLiveHerbaceous() const;
    double getAspenLoadLiveWoody() const;
    double getAspenSavrDeadOneHour() const;
    double getAspenSavrDeadTenHour() const;
    double getAspenSavrLiveHerbaceous() const;
    double getAspenSavrLiveWoody() const;

    // Chaparral getters
    double getChaparralAge() const;
    double getChaparralDaysSinceMayFirst() const;
    double getChaparralDeadFuelFraction() const;
    double getChaparralDeadMoistureOfExtinction() const;
    double getChaparralLiveMoistureOfExtinction() const;
    double getChaparralDensity(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralFuelBedDepth() const;
    double getChaparralHeatOfCombustion(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralLoad(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralMoisture(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralSavr(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralEffectiveSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralTotalSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getChaparralTotalDeadFuelLoad() const;
    double getChaparralTotalFuelLoad() const;
    double getChaparralTotalLiveFuelLoad() const;
    ChaparralFuelType::ChaparralFuelTypeEnum getChaparralFuelType() const;

protected:
    void initializeMembers();
    void memberwiseCopyAssignment(const SurfaceFuelbedIntermediates& rhs);
    void setFuelLoad();
    void setMoistureContent();
    void setDeadFuelMoistureOfExtinction();
    void setFuelbedDepth();
    void setSAVR();
    void countSizeClasses();
    void dynamicLoadTransfer();
    void calculateFractionOfTotalSurfaceAreaForLifeStates();
    void calculateTotalSurfaceAreaForLifeState(int lifeCategory);
    void calculateFractionOfTotalSurfaceAreaForSizeClasses(int lifeCategory);
    void sumFractionOfTotalSurfaceAreaBySizeClass(const double areaWeightingFactorDeadOrLive[FuelConstants::MaxParticles],
        const double savrDeadOrLive[FuelConstants::MaxParticles], double summedWeightingFactors[FuelConstants::MaxParticles]);
    void assignFractionOfTotalSurfaceAreaBySizeClass(FuelLifeState::FuelLifeStateEnum lifeState, const double savrDeadOrLive[FuelConstants::MaxParticles],
        const double summedWeightingFactors[FuelConstants::MaxParticles], double sizeSortedWeightingFactorsDeadOrLive[FuelConstants::MaxParticles]);
    void setHeatOfCombustion();
    void calculateCharacteristicSAVR();
    void calculateHeatSink();
    void calculateLiveMoistureOfExtinction();
    void calculatePropagatingFlux();
  
    const FuelModels* fuelModels_;      // Pointer to FuelModels object
    const SurfaceInputs* surfaceInputs_;    // Pointer to surfaceInputs object
    ChaparralFuel chaparralFuel_;
    PalmettoGallberry palmettoGallberry_;
    WesternAspen westernAspen_;
    
    // Member variables
    int numberOfSizeClasses_[FuelConstants::MaxLifeStates];                         // Number of size classes in the currently used fuel model
    double depth_;                                                                  // Depth of fuelbed in feet
    double weightedMoisture_[FuelConstants::MaxLifeStates];                         // Weighted (characteristic) moisture content for both live and dead fuels
    double totalSurfaceArea_[FuelConstants::MaxLifeStates];                         // Total surface area for both live and dead fuels
    double weightedHeat_[FuelConstants::MaxLifeStates];                             // Weighted (characteristic) heat content for both live and dead fuels
    double weightedSilica_[FuelConstants::MaxLifeStates];                           // Weighted (characteristic) silica content for both live and dead fuels
    double weightedFuelLoad_[FuelConstants::MaxLifeStates];                         // Weighted (characteristic) fuel loading for both live and dead fuels
    double moistureOfExtinction_[FuelConstants::MaxLifeStates];                     // Moisture of extinction for both live and dead fuels
    double fractionOfTotalSurfaceArea_[FuelConstants::MaxLifeStates];               // Ratio of surface area to total surface area
    double totalLoadForLifeState_[FuelConstants::MaxLifeStates];                    // Total fuel load for live and dead fuels
    double fuelDensityDead_[FuelConstants::MaxParticles];                           // Fuel density for live fuels by size class
    double fuelDensityLive_[FuelConstants::MaxParticles];                           // Fuel density for dead fuels by size class
    double moistureDead_[FuelConstants::MaxParticles];                              // Moisture content for dead fuels by size class
    double moistureLive_[FuelConstants::MaxParticles];                              // Moisture content for live fuels by size class
    double loadDead_[FuelConstants::MaxParticles];			        			    // Fuel load for dead fuels by size class
    double loadLive_[FuelConstants::MaxParticles];					        	    // Fuel load for live fuels by size class
    double savrDead_[FuelConstants::MaxParticles];				    		        // Surface area to volume ratio for dead fuels by size class
    double savrLive_[FuelConstants::MaxParticles];                                  // Surface area to volume ratio for live fuels by size class
    double surfaceAreaDead_[FuelConstants::MaxParticles];                           // Surface area for dead size classes 
    double surfaceAreaLive_[FuelConstants::MaxParticles];                           // Surface area for live size classes
    double heatOfCombustionDead_[FuelConstants::MaxParticles];                      // Heat of combustion for dead size classes
    double heatOfCombustionLive_[FuelConstants::MaxParticles];                      // Heat of combustion for live size classes
    double silicaEffectiveDead_[FuelConstants::MaxParticles];                       // Effective silica constent for dead size classes
    double silicaEffectiveLive_[FuelConstants::MaxParticles];                       // Effective silica constent for live size classes
    double fractionOfTotalSurfaceAreaDead_[FuelConstants::MaxParticles];            // Fraction of surface area for dead size classes
    double fractionOfTotalSurfaceAreaLive_[FuelConstants::MaxParticles];            // Fraction of surface area for live size classes
    double sizeSortedFractionOfSurfaceAreaDead_[FuelConstants::MaxSavrSizeClasses]; // Intermediate fuel weighting values for dead fuels
    double sizeSortedFractionOfSurfaceAreaLive_[FuelConstants::MaxSavrSizeClasses]; // Intermediate fuel weighting values for live fuels

    int fuelModelNumber_;           // The number associated with the current fuel model being used
    double heatSink_;               // Rothermel 1972, Denominator of equation 52
    double sigma_;                  // Fuelbed characteristic SAVR, Rothermel 1972 
    double bulkDensity_;            // Ovendry bulk density in lbs/ft^2, Rothermale 1972, equation 40
    double packingRatio_;           // Packing ratio, Rothermel 1972, equation 31 
    double relativePackingRatio_;   // Packing ratio divided by the optimum packing ratio, Rothermel 1972, term in RHS equation 47
    double totalSilicaContent_;     // Total silica content (fraction), Albini 1976, p. 91
    double propagatingFlux_;
};

#endif	// SURFACEFUELBEDINTERMEDIATES_H
