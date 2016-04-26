#ifndef SURFACEFUELBEDINTERMEDIATES_HEADER
#define SURFACEFUELBEDINTERMEDIATES_HEADER

#include "palmettoGallberry.h" 
#include "westernAspen.h"
#include "surfaceEnums.h"

class FuelModels;
class SurfaceInputs;

class SurfaceFuelbedIntermediates
{
public:
    SurfaceFuelbedIntermediates();
    SurfaceFuelbedIntermediates(const SurfaceFuelbedIntermediates& rhs);
    SurfaceFuelbedIntermediates& operator= (const SurfaceFuelbedIntermediates& rhs);
    SurfaceFuelbedIntermediates(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs);

    ~SurfaceFuelbedIntermediates();
    void calculateFuelbedIntermediates();

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
    void initializeMemberVariables();
    void setFuelLoad();
    void setMoistureContent();
    void setDeadFuelMoistureOfExtinction();
    void setFuelbedDepth();
    void setSAV();
    void setSAV(const PalmettoGallberry& palmettoGallberry);
    void countSizeClasses();
    void dynamicLoadTransfer();
    void calculateFractionOfTotalSurfaceAreaForLifeStates();
    void calculateTotalSurfaceAreaForLifeState(int lifeCategory);
    void calculateFractionOfTotalSurfaceAreaForSizeClasses(int lifeCategory);
    void sumFractionOfTotalSurfaceAreaBySizeClass(const double areaWeightingFactorDeadOrLive[MAX_PARTICLES], 
        const double savrDeadOrLive[MAX_PARTICLES], double summedWeightingFactors[MAX_PARTICLES]);
    void assignFractionOfTotalSurfaceAreaBySizeClass(const double savrDeadOrLive[MAX_PARTICLES], 
        const double summedWeightingFactors[MAX_PARTICLES], double sizeSortedWeightingFactorsDeadOrLive[MAX_PARTICLES]);
    void setHeatOfCombustion();
    void calculateCharacteristicSAVR();
    void calculateHeatSink();
    void calculateLiveMoistureOfExtinction();
    void calculatePropagatingFlux();
    void calculateResidenceTime();

    const FuelModels* fuelModels_;          // Pointer to FuelModels object
    const SurfaceInputs* surfaceInputs_;    // Pointer to surfaceInputs object
    PalmettoGallberry palmettoGallberry_;
    WesternAspen westernAspen_;

    // Member variables
    int numberOfSizeClasses_[MAX_LIFE_STATES];                          // Number of size classes in the currently used fuel model
    double depth_;                                                      // Depth of fuelbed in feet
    double weightedMoisture_[MAX_LIFE_STATES];                          // Weighted moisture content for both live and dead fuels
    double totalSurfaceArea_[MAX_LIFE_STATES];                          // Total surface area for both live and dead fuels
    double weightedHeat_[MAX_LIFE_STATES];                              // Weighted heat content for both live and dead fuels
    double weightedSilica_[MAX_LIFE_STATES];                            // Weighted silica content for both live and dead fuels
    double weightedFuelLoad_[MAX_LIFE_STATES];                          // Weighted fuel loading for both live and dead fuels
    double moistureOfExtinction_[MAX_LIFE_STATES];                      // Moisture of extinction for both live and dead fuels
    double fractionOfTotalSurfaceArea_[MAX_LIFE_STATES];                // Ratio of surface area to total surface area
    double fuelDensity_[MAX_LIFE_STATES];                               // Fuel density for live and dead fuels
    double totalLoadForLifeState_[MAX_LIFE_STATES];                     // Total fuel load for live and dead fuels
    double moistureDead_[MAX_PARTICLES];                                // Moisture content for dead fuels by size class
    double moistureLive_[MAX_PARTICLES];                                // Moisture content for live fuels by size class
    double loadDead_[MAX_PARTICLES];			        			    // Fuel load for dead fuels by size class
    double loadLive_[MAX_PARTICLES];					        	    // Fuel load for live fuels by size class
    double savrDead_[MAX_PARTICLES];				    		        // Surface area to volume ratio for dead fuels by size class
    double savrLive_[MAX_PARTICLES];				        		    // Surface area to volume ratio for live fuels by size class
    double surfaceAreaDead_[MAX_PARTICLES];                             // Surface area for dead size classes 
    double surfaceAreaLive_[MAX_PARTICLES];                             // Surface area for live size classes
    double heatDead_[MAX_PARTICLES];                                    // Heat of combustion for dead size classes
    double heatLive_[MAX_PARTICLES];                                    // Heat of combustion for live size classes
    double silicaEffectiveDead_[MAX_PARTICLES];                         // Effective silica constent for dead size classes
    double silicaEffectiveLive_[MAX_PARTICLES];                         // Effective silica constent for live size classes
    double fractionOfTotalSurfaceAreaDead_[MAX_PARTICLES];              // Fraction of surface area for dead size classes
    double fractionOfTotalSurfaceAreaLive_[MAX_PARTICLES];              // Fraction of surface area for live size classes
    double sizeSortedFractionOfSurfaceAreaDead_[MAX_SAVR_SIZE_CLASSES]; // Intermediate fuel weighting values for dead fuels
    double sizeSortedFractionOfSurfaceAreaLive_[MAX_SAVR_SIZE_CLASSES]; // Intermediate fuel weighting values for live fuels

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

#endif	// SURFACEFUELBEDINTERMEDIATES_HEADER
