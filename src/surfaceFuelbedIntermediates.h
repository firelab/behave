#ifndef SURFACEFUELBEDINTERMEDIATES_HEADER
#define SURFACEFUELBEDINTERMEDIATES_HEADER

#include <cmath>
#include "fuelModels.h"
#include "palmettoGallberry.h" 
#include "surfaceInputs.h"

class SurfaceFuelbedIntermediates
{
    // Static constants
    static const int MAX_LIFE_STATES = 2;       //< Life state (dead or alive)
    static const int MAX_PARTICLES = 4;         //< Maximum number of size classes within a life state (dead/live)
    static const int MAX_SAVR_SIZE_CLASSES = 5; //< Maximum number of SAVR size classes

public:
    SurfaceFuelbedIntermediates(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs);
    ~SurfaceFuelbedIntermediates();
    void calculateFuelbedIntermediates();

    // Public Getters
    double getFuelbedDepth() const;
    double getBulkDensity() const;
    double getPackingRatio()const;
    double getRelativePackingRatio() const;
    double getSigma() const;
    double getHeatSink() const;
    double getWeightedMoistureByLifeState(int lifeState) const;
    double getMoistureOfExtinctionByLifeState(int lifeState) const;
    double getWeightedHeatByLifeState(int lifeState) const;
    double getWeightedSilicaByLifeState(int lifeState) const;
    double getWeightedFuelLoadByLifeState(int lifeState) const;

private:
    void initializeMemberVariables();
    void setFuelLoad();
    void setMoistureContent();
    void setSAV();
    void countSizeClasses();
    void dynamicLoadTransfer();
    void calculateFractionOfTotalSurfaceArea();
    void calculateTotalSurfaceAreaForLifeState(int lifeCategory);
    void calculateFractionOfTotalSurfaceAreaForSizeClasses(int lifeCategory);
    void sumFractionOfTotalSurfaceAreaBySizeClass(const double areaWeightingFactorDeadOrLive[MAX_PARTICLES], const double savrDeadOrLive[MAX_PARTICLES],
        double summedWeightingFactors[MAX_PARTICLES]);
    void assignFractionOfTotalSurfaceAreaBySizeClass(const double savrDeadOrLive[MAX_PARTICLES], const double summedWeightingFactors[MAX_PARTICLES],
        double sizeSortedWeightingFactorsDeadOrLive[MAX_PARTICLES]);
    void setHeatOfCombustion();
    void calculateCharacteristicSAVR();
    void calculateHeatSink();
    void calculateLiveMoistureOfExtinction();
    void calculateResidenceTime();

    // Class Constants
    const int DEAD = 0;     // Dead life category index
    const int LIVE = 1;     // Live life category index

    const FuelModels* fuelModels_;          // Pointer to FuelModels object
    const SurfaceInputs* surfaceInputs_;    // Pointer to surfaceInputs object

    // Member variables
    int numberOfSizeClasses_[MAX_LIFE_STATES];                          // Number of size classes represented in the currently used fuel model (dead/live)
    double weightedMoisture_[MAX_LIFE_STATES];                          // Weighted moisture content for both life and dead fuel categories (dead/live)
    double totalSurfaceArea_[MAX_LIFE_STATES];                          // Total surface area for life state (dead/live)
    double weightedHeat_[MAX_LIFE_STATES];                              // Weighted heat content for life state (dead/live)
    double weightedSilica_[MAX_LIFE_STATES];                            // Weighted effective silica content life state(dead/live)
    double weightedFuelLoad_[MAX_LIFE_STATES];                          // Weighted fuel loading for life state (dead/live)
    double moistureOfExtinction_[MAX_LIFE_STATES];                      // Moisture of extinction for life state (dead/live)
    double fractionOfTotalSurfaceArea_[MAX_LIFE_STATES];                // Ratio of surface area for life category (dead/live) to total surface area
    double fuelDensity_[MAX_LIFE_STATES];                               // Fuel density for live and dead fuels
    double totalLoadForLifeState_[MAX_LIFE_STATES];                     // Total fuel load for live and dead fuels
    double moistureDead_[MAX_PARTICLES];                                // Moisture content in fraction of oven-dry weight for dead fuels by size class
    double moistureLive_[MAX_PARTICLES];                                // Moisture content in fraction of oven-dry weight for live fuels by size class
    double loadDead_[MAX_PARTICLES];			        			    // Fuel load of fuelbad 4 size classes for dead fuels by size class
    double loadLive_[MAX_PARTICLES];					        	    // Fuel load of fuelbad 4 size classes for live fuels by size class
    double savrDead_[MAX_PARTICLES];				    		        // Surface area to volume ratio for dead fuels by size class
    double savrLive_[MAX_PARTICLES];				        		    // Surface area to volume ratio for live fuels by size class
    double surfaceAreaDead_[MAX_PARTICLES];                             // Surface area for dead size classes 
    double surfaceAreaLive_[MAX_PARTICLES];                             // Surface area for live size classes
    double heatDead_[MAX_PARTICLES];                                    // Heat of combustion for dead size classes
    double heatLive_[MAX_PARTICLES];                                    // Heat of combustion for live size classes
    double silicaEffectiveDead_[MAX_PARTICLES];                         // Effective silica constent for dead size classes
    double silicaEffectiveLive_[MAX_PARTICLES];                         // Effective silica constent for live size classes
    double fractionOfTotalSurfaceAreaDead_[MAX_PARTICLES];              // Fraction of the total surface area in a fuel bed occupied by dead size classes
    double fractionOfTotalSurfaceAreaLive_[MAX_PARTICLES];              // Fraction of the total surface area in a fuel bed occupied by live size classes
    double sizeSortedFractionOfSurfaceAreaDead_[MAX_SAVR_SIZE_CLASSES]; // Intermediate fuel weighting values for dead fuels sorted by size class
    double sizeSortedFractionOfSurfaceAreaLive_[MAX_SAVR_SIZE_CLASSES]; // Intermediate fuel weighting values for live fuels sorted by size class

    int fuelModelNumber_;           // The number associated with the current fuel model being used
    double liveFuelMois_;           // Live fuel moisture content
    double liveFuelMext_;           // Live fuel moisture of extinction 
    double heatSink_;               // Rothermel 1972, Denominator of equation 52
    double sigma_;                  // Fuelbed characteristic SAVR, Rothermel 1972 
    double bulkDensity_;            // Ovendry bulk density in lbs/ft^2, Rothermale 1972, equation 40
    double packingRatio_;           // Packing ratio, Rothermel 1972, equation 31 
    double relativePackingRatio_;   // Packing ratio divided by the optimum packing ratio, Rothermel 1972, term in RHS equation 47
    double totalSilicaContent_;     // Total silica content in percent, Albini 1976, p. 91
};

#endif	// SURFACEFUELBEDINTERMEDIATES_HEADER
