#ifndef SURFACEFUELBEDINTERMEDIATES_HEADER
#define SURFACEFUELBEDINTERMEDIATES_HEADER

#include "fuelModels.h"
#include "surfaceInputs.h"
#include <cmath>

class SurfaceFuelbedIntermediates
{
	// TODO: Look into the possibility of storing invariant data for a given fuel model
	// to reduce computing time in ensemble simulations -WMC 10/2015

	// Static constants
	static const int	MAX_LIFE_STATES = 2;	//< Life state (dead or alive)
	static const int	MAX_SIZES = 6;			//< Fuel moisture time lag classes
	static const int	MAX_PARTICLES = 4;
	
public:
	SurfaceFuelbedIntermediates(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs);
	~SurfaceFuelbedIntermediates();
	void calculateFuelbedIntermediates();
	
	// Public Getters
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
	void calculateFuelWeightingFactors();
	void calculateWeightedSurfaceAreas(int lifeCategory);
	void calculateAreaWeightingFactors(int lifeCategory);
	void sumAreaWeightingFactorsBySizeClass(const double areaWeightingFactorDeadOrLive[MAX_PARTICLES], const double savrDeadOrLive[MAX_PARTICLES], 
		double summedWeightingFactors[MAX_PARTICLES]);
	void assignWeightingFactorBySizeClass(const double savrDeadOrLive[MAX_PARTICLES], const double summedWeightingFactors[MAX_PARTICLES],
		double sizeSortedWeightingFactorsDeadOrLive[MAX_PARTICLES]);
	void setHeatOfCombustion();
	void sumIntermediateParametersByFuelComponent();
	void calculateHeatSink();
	void calculateLiveMoistureOfExtinction();
	void calculateResidenceTime();

	// Class Constants
	const int			DEAD = 0;								//< Dead life category index
	const int			LIVE = 1;								//< Live life category index
	const double		OVENDRY_FUEL_DENSITY = 32.0;			//< Average density of dry fuel in lbs/ft^3, Albini 1976, p. 91
	const double		TOTAL_SILICA_CONTENT = 0.0555;			//< Total silica content in percent, Albini 1976, p. 91 

	const FuelModels*		fuelModels_;						//< Pointer to FuelModels object
	const SurfaceInputs*	surfaceInputs_;						//< Pointer to surfaceInputs object
	
	// Member variables
	double	weightedMoisture_[MAX_LIFE_STATES];					// Weighted moisture content for both life and dead fuel categories */
	double	weightedSurfaceArea_[MAX_LIFE_STATES];				// Weighted surface area for i-th category (dead/live) */
	double	weightedHeat_[MAX_LIFE_STATES];						// Weighted heat content for i-th category (live/dead)
	double	weightedSilica_[MAX_LIFE_STATES];					// Weighted effective silica content for i-th categort (dead/live) */
	double	weightedFuelLoad_[MAX_LIFE_STATES];					// Weighted fuel loading for i-th category (dead/live)
	double	moistureOfExtinction_[MAX_LIFE_STATES];				// Moisture of extinction for both life and dead fuel categories */
	double	relativeWeightedSurfaceArea_[MAX_LIFE_STATES];	// Ratio of weighted surface area for life category (dead/live) to total weighted surface area */
	int		numberOfSizeClasses[MAX_LIFE_STATES];				// Number of size classes (live and dead) represented in the currently used fuel model */

	double sizeSortedWeightingFactorsDead_[MAX_PARTICLES];		// Intermediate fuel weighting values for dead fuels sorted by size class */
	double sizeSortedWeightingFactorsLive_[MAX_PARTICLES];		// Intermediate fuel weighting values for live fuels sorted by size class */
	double moistureDead_[MAX_PARTICLES];						// Moisture content in fraction of oven-dry weight for dead fuels */
	double moistureLive_[MAX_PARTICLES];						// Moisture content in fraction of oven-dry weight for live fuels */
	double loadDead_[MAX_PARTICLES];							// Fuel load of fuelbad 4 size classes for dead fuels */
	double loadLive_[MAX_PARTICLES];							// Fuel load of fuelbad 4 size classes for live fuels */
	double savrDead_[MAX_PARTICLES];							// Surface area to volume ratio for dead fuels */
	double savrLive_[MAX_PARTICLES];							// Surface area to volume ratio for live fuels */
	double surfaceAreaDead_[MAX_PARTICLES];
	double surfaceAreaLive_[MAX_PARTICLES];
	double heatDead_[MAX_PARTICLES];
	double heatLive_[MAX_PARTICLES];
	double silicaEffectiveDead_[MAX_PARTICLES];
	double silicaEffectiveLive_[MAX_PARTICLES];
	double areaWeightingFactorDead_[MAX_PARTICLES];
	double areaWeightingFactorLive_[MAX_PARTICLES];

	int		fuelModelNumber_;		// The number associated with the current fuel model being used */
	int		numberOfLive_;			// number of live size classes in the current fuel model being used */ 
	int		numberOfDead_;			// number of dead size classes in the current fuel model being used */ 
	double	liveFuelMois_;			// Live fuel moisture content */
	double	liveFuelMext_;			// Live fuel moisture of extinction */
	double	totalLoad_;				// Total fuel loading in fuel bed */
	double	heatSink_;				// Rothermel 1972, Denominator of equation 52 */
	double	sigma_;					// Particle characteristic SAVR, Rothermel 1972 
	double	bulkDensity_;			// Ovendry bulk density in lbs/ft^2, Rothermale 1972, equation 40
	double	packingRatio_;			// Packing ratio, Rothermel 1972, equation 31 
	double	relativePackingRatio_;
};

#endif	// SURFACEFUELBEDINTERMEDIATES_HEADER
