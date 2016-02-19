#include "surfaceFuelbedIntermediates.h"

SurfaceFuelbedIntermediates::SurfaceFuelbedIntermediates(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs)
{
	fuelModels_ = &fuelModels;
	surfaceInputs_ = &surfaceInputs;
	initializeMemberVariables();
}

SurfaceFuelbedIntermediates::~SurfaceFuelbedIntermediates()
{

}

void SurfaceFuelbedIntermediates::calculateFuelbedIntermediates()
{
	
	// TODO: Basically need to refactor and break down this method, this should
	// be aided by the new OOD - WMC 08/2015

	// TODO: Look into the creation of two new classes, FuelBed and Particle, these
	// new classes should aid in refactoring and also improve the overall design - WMC 08/2015

	// Rothermel spread equation based on BEHAVE source code,
	// support for dynamic fuel models added 10/13/2004

	initializeMemberVariables(); // Reset member variables to zero to forget previous state  
	const double PI = 3.14159265358979;

	double depth = 0.0;						// Fuel bed depth in feet
	double ovendryFuelLoad = 0.0;			// Ovendry fuel loading, Rothermel 1972
	double optimumPackingRatio = 0.0;		// Optimum packing ratio, Rothermel 1972, equation 37
	double ovendryFuelDensity = 32.0;		// Average density of dry fuel in lbs/ft^3, Albini 1976, p. 91
	
	bool isDynamic = false;					// Whether or not fuel model is dynamic

	fuelModelNumber_ = surfaceInputs_->getFuelModelNumber();
	
	setFuelLoad();
	depth = fuelModels_->getFuelbedDepth(fuelModelNumber_);

	countSizeClasses();

	setMoistureContent();
	
	setSAV();

	isDynamic = fuelModels_->getIsDynamic(fuelModelNumber_);
	if (isDynamic) // do the dynamic load transfer
	{
		dynamicLoadTransfer();
	}
	
	// Heat of combustion
	setHeatOfCombustion();

	// Fuel weighting factors
	calculateFuelWeightingFactors();

	// Moisture of extinction
	moistureOfExtinction_[DEAD] = fuelModels_->getMoistureOfExtinctionDead(fuelModelNumber_);
	calculateLiveMoistureOfExtinction();
	
	// Intermediate calculations, summing parameters by fuel component
	sumIntermediateParametersByFuelComponent();

	/* final calculations */
	double totalLoad = totalLoad_[DEAD] + totalLoad_[LIVE];

	bulkDensity_ = totalLoad / depth;

	for (int lifeState = 0; lifeState < MAX_LIFE_STATES; lifeState++)
	{
		//packingRatio_ = totalLoad / (depth * ovendryFuelDensity);
		packingRatio_ += totalLoad_[lifeState] / (depth * fuelDensity_[lifeState]);
	}

	optimumPackingRatio = 3.348 / pow(sigma_, 0.8189);
	relativePackingRatio_ = packingRatio_ / optimumPackingRatio;

	calculateHeatSink();
}

void SurfaceFuelbedIntermediates::setFuelLoad()
{
	bool isUsingPalmettoGallberry = surfaceInputs_->isUsingPalmettoGallberry();

	if (isUsingPalmettoGallberry)
	{
		// Calculate load values for Palmetto-Gallberry
		PalmettoGallberry palmettoGallberry;

		double ageOfRough = surfaceInputs_->getAgeOfRough();
		double heightOfUnderstory = surfaceInputs_->getHeightOfUnderstory();
		double palmettoCoverage = surfaceInputs_->getPalmettoCoverage();
		double overstoryBasalArea = surfaceInputs_->getOverstoryBasalArea();

		loadDead_[0] = palmettoGallberry.palmettoGallberyDeadOneHourLoad(ageOfRough, heightOfUnderstory);
		loadDead_[1] = palmettoGallberry.palmettoGallberyDeadTenHourLoad(ageOfRough, palmettoCoverage);
		loadDead_[2] = palmettoGallberry.palmettoGallberyDeadFoliageLoad(ageOfRough, palmettoCoverage);
		loadDead_[3] = palmettoGallberry.palmettoGallberyLitterLoad(ageOfRough, overstoryBasalArea);

		loadLive_[0] = palmettoGallberry.palmettoGallberyLiveOneHourLoad(ageOfRough, heightOfUnderstory);
		loadLive_[1] = palmettoGallberry.palmettoGallberyLiveTenHourLoad(ageOfRough, heightOfUnderstory);
		loadLive_[2] = palmettoGallberry.palmettoGallberyLiveFoliageLoad(ageOfRough, palmettoCoverage, heightOfUnderstory);
		loadLive_[3] = 0.0;

		for (int i = 0; i < 3; i++)
		{
			silicaEffectiveLive_[i] = 0.015;
		}
	}
	else
	{
		// Proceed as normal
		loadDead_[0] = fuelModels_->getFuelLoadOneHour(fuelModelNumber_);
		loadDead_[1] = fuelModels_->getFuelLoadTenHour(fuelModelNumber_);
		loadDead_[2] = fuelModels_->getFuelLoadHundredHour(fuelModelNumber_);
		loadDead_[3] = 0.0;

		loadLive_[0] = fuelModels_->getFuelLoadLiveHerbaceous(fuelModelNumber_);
		loadLive_[1] = fuelModels_->getFuelLoadLiveWoody(fuelModelNumber_);
		loadLive_[2] = 0.0;
		loadLive_[3] = 0.0;
	}


	// Debug
	double loadDeadTonsPerAcre[4];
	double loadLiveTonsPerAcre[4];

	for (int i = 0; i < 4; i++)
	{
		loadDeadTonsPerAcre[i] = loadDead_[i] * 21.78;
		loadLiveTonsPerAcre[i] = loadLive_[i] * 21.78;
	}

	double totalDead = 0;
	double totalLive = 0;

	for (int i = 0; i < 4; i++)
	{
		totalDead += loadDeadTonsPerAcre[i];
		totalLive += loadLiveTonsPerAcre[i];
	}
}

void SurfaceFuelbedIntermediates::setMoistureContent()
{
	bool isUsingPalmettoGallberry = surfaceInputs_->isUsingPalmettoGallberry();

	if (isUsingPalmettoGallberry)
	{
		moistureDead_[0] = surfaceInputs_->getMoistureDeadAtIndex(0);
		moistureDead_[1] = surfaceInputs_->getMoistureDeadAtIndex(1);
		moistureDead_[2] = surfaceInputs_->getMoistureDeadAtIndex(0);
		moistureDead_[3] = surfaceInputs_->getMoistureDeadAtIndex(2);

		moistureLive_[0] = surfaceInputs_->getMoistureLiveAtIndex(1);
		moistureLive_[1] = surfaceInputs_->getMoistureLiveAtIndex(1);
		moistureLive_[2] = surfaceInputs_->getMoistureLiveAtIndex(0);
	}
	else
	{
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			moistureDead_[i] = 0;
			moistureLive_[i] = 0;
		}

		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			moistureDead_[i] = surfaceInputs_->getMoistureDeadAtIndex(i);
			moistureLive_[i] = surfaceInputs_->getMoistureLiveAtIndex(i);
		}
	}
}

void SurfaceFuelbedIntermediates::setSAV()
{
	bool isUsingPalmettoGallberry = surfaceInputs_->isUsingPalmettoGallberry();
	
	if (isUsingPalmettoGallberry)
	{
		// Special values for Palmetto-Gallberry
		savrDead_[0] = 350.0;
		savrDead_[1] = 140.0;
		savrDead_[2] = 2000.0;
		savrDead_[3] = 2000.0; // TODO: find appropriate savr for palmetto-gallberry litter

		savrLive_[0] = 350.0;
		savrLive_[1] = 140.0;
		savrLive_[2] = 2000.0;
		savrLive_[3] = 0.0;
	}
	else
	{
		// Proceed as normal
		savrDead_[0] = fuelModels_->getSavrOneHour(fuelModelNumber_);
		savrDead_[1] = 109.0;
		savrDead_[2] = 30.0;
		savrDead_[3] = fuelModels_->getSavrLiveHerbaceous(fuelModelNumber_);

		savrLive_[0] = fuelModels_->getSavrLiveHerbaceous(fuelModelNumber_);
		savrLive_[1] = fuelModels_->getSavrLiveWoody(fuelModelNumber_);
		savrLive_[2] = 0.0;
		savrLive_[3] = 0.0;
	}
}

void SurfaceFuelbedIntermediates::setHeatOfCombustion()
{
	const int NUMBER_OF_LIVE_SIZE_CLASSES = 3;

	double heatOfCombustionDead = fuelModels_->getHeatOfCombustionDead(fuelModelNumber_);
	double heatOfCombustionLive = fuelModels_->getHeatOfCombustionLive(fuelModelNumber_);
	
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		heatDead_[i] = heatOfCombustionDead;
		if (i < NUMBER_OF_LIVE_SIZE_CLASSES)
		{
			heatLive_[i] = heatOfCombustionLive;
		}
		else
		{
			heatLive_[i] = 0.0;
		}
	}
}

void SurfaceFuelbedIntermediates::calculateHeatSink()
{
	double qigLive[MAX_PARTICLES]; // Heat of preigintion for live fuels
	double qigDead[MAX_PARTICLES]; // Heat of preigintion for dead fuels

	// Initialize variables
	heatSink_ = 0;
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		qigLive[i] = 0.0;
		qigDead[i] = 0.0;
	}

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (savrDead_[i] > 1.0e-07)
		{
			qigDead[i] = 250.0 + 1116.0 * moistureDead_[i];
			heatSink_ += relativeWeightedSurfaceArea_[DEAD] * areaWeightingFactorDead_[i] * qigDead[i] * exp(-138.0 / savrDead_[i]);
		}
		if (savrLive_[i] > 1.0e-07)
		{
			qigLive[i] = 250.0 + 1116.0 * moistureLive_[i];
			heatSink_ += relativeWeightedSurfaceArea_[LIVE] * areaWeightingFactorLive_[i] * qigLive[i] * exp(-138.0 / savrLive_[i]);
		}
	}
	heatSink_ *= bulkDensity_;
}

void SurfaceFuelbedIntermediates::sumIntermediateParametersByFuelComponent()
{
	double	wnLive[MAX_PARTICLES];			// Net fuel loading for live fuels, Rothermel 1972, equation 24	
	double	wnDead[MAX_PARTICLES]; 			// Net fuel loading for dead fuels, Rothermel 1972, equation 24
	
	double	weightedSavr[MAX_LIFE_STATES];	// Weighted SAVR for i-th categort (live/dead)

	// Initialize Accumulated values
	//totalLoad_ = 0.0;
	sigma_ = 0.0;
	for (int i = 0; i < MAX_LIFE_STATES; i++)
	{
		totalLoad_[i] = 0.0;
		weightedHeat_[i] = 0.0;
		weightedSilica_[i] = 0.0;
		weightedMoisture_[i] = 0.0;
		weightedSavr[i] = 0.0;
		weightedFuelLoad_[i] = 0.0;
	}
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		wnDead[i] = 0.0;
		wnLive[i] = 0.0;
	}

	bool isUsingPalmettoGallbery = surfaceInputs_->isUsingPalmettoGallberry();
	if (isUsingPalmettoGallbery)
	{
		totalSilicaContent_ = 0.030;
	}

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (savrDead_[i] > 1.0e-07)
		{
			wnDead[i] = loadDead_[i] * (1.0 - totalSilicaContent_); // Rothermel 1972, equation 24
			weightedHeat_[DEAD] += areaWeightingFactorDead_[i] * heatDead_[i]; // weighted heat content
			weightedSilica_[DEAD] += areaWeightingFactorDead_[i] * silicaEffectiveDead_[i]; // weighted silica content
			weightedMoisture_[DEAD] += areaWeightingFactorDead_[i] * moistureDead_[i]; // weighted moisture content
			weightedSavr[DEAD] += areaWeightingFactorDead_[i] * savrDead_[i]; // weighted SAVR
			totalLoad_[DEAD] += loadDead_[i];
		}
		if (savrLive_[i] > 1.0e-07)
		{
			wnLive[i] = loadLive_[i] * (1.0 - totalSilicaContent_); // Rothermel 1972, equation 24
			weightedHeat_[LIVE] += areaWeightingFactorLive_[i] * heatLive_[i]; // weighted heat content
			weightedSilica_[LIVE] += areaWeightingFactorLive_[i] * silicaEffectiveLive_[i]; // weighted silica content
			weightedMoisture_[LIVE] += areaWeightingFactorLive_[i] * moistureLive_[i]; // weighted moisture content
			weightedSavr[LIVE] += areaWeightingFactorLive_[i] * savrLive_[i]; // weighted SAVR
			totalLoad_[LIVE] += loadLive_[i];
		}
		weightedFuelLoad_[DEAD] += sizeSortedWeightingFactorsDead_[i] * wnDead[i];
		weightedFuelLoad_[LIVE] += sizeSortedWeightingFactorsLive_[i] * wnLive[i];
	}
	
	for (int i = 0; i < MAX_LIFE_STATES; i++)
	{
		sigma_ += relativeWeightedSurfaceArea_[i] * weightedSavr[i];
	}
}

void SurfaceFuelbedIntermediates::countSizeClasses()
{
	const int NUMBER_OF_DEAD_SIZE_CLASSES = 4;
	const int NUMBER_OF_LIVE_SIZE_CLASSES = 3;
	const int MAX_DEAD_SIZE_CLASSES = 4;
	const int MAX_LIVE_SIZE_CLASSES = 2;

	// count number of fuels
	for (int i = 0; i < NUMBER_OF_DEAD_SIZE_CLASSES; i++)
	{
		if (loadDead_[i])
		{
			numberOfDead_++;
		}
	}
	for (int i = 0; i < NUMBER_OF_LIVE_SIZE_CLASSES; i++)
	{
		if (loadLive_[i])
		{
			numberOfLive_++;
		}
	}
	if (numberOfLive_ > 0)
	{
		numberOfLive_ = MAX_LIVE_SIZE_CLASSES;	// Boost to max number
	}
	if (numberOfDead_ > 0)
	{
		numberOfDead_ = MAX_DEAD_SIZE_CLASSES; // Boost to max number
	}

	numberOfDead_ = 4;
	numberOfLive_ = 3;

	numberOfSizeClasses[0] = numberOfDead_;
	numberOfSizeClasses[1] = numberOfLive_;
}

void SurfaceFuelbedIntermediates::dynamicLoadTransfer()
{
	if (moistureLive_[0] < 0.30)
	{
		loadDead_[3] = loadLive_[0];
		loadLive_[0] = 0.0;
	}
	else if (moistureLive_[0] <= 1.20)
	{
		//loadDead_[3] = loadLive_[0] * (1.20 - moistureLive_[0]) / 0.9;
		loadDead_[3] = loadLive_[0] * (1.333 - 1.11 * moistureLive_[0]); // To keep consistant with BehavePlus
		loadLive_[0] -= loadDead_[3];
	}
}

void SurfaceFuelbedIntermediates::calculateFuelWeightingFactors()
{
	const int SIZE_CLASSES = 5;
	double summedWeightingFactors[SIZE_CLASSES];	// Intermediate weighting factors for each size class
	
	for (int lifeState = 0; lifeState < MAX_LIFE_STATES; lifeState++)
	{
		if (numberOfSizeClasses[lifeState] != 0)
		{
			calculateWeightedSurfaceAreas(lifeState);
			calculateAreaWeightingFactors(lifeState);
		}
		for (int j = 0; j < SIZE_CLASSES; j++)
		{
			summedWeightingFactors[j] = 0.0;
		}
		if (lifeState == DEAD)
		{
			sumAreaWeightingFactorsBySizeClass(areaWeightingFactorDead_, savrDead_, summedWeightingFactors);
			assignWeightingFactorBySizeClass(savrDead_, summedWeightingFactors, sizeSortedWeightingFactorsDead_);
		}
		if (lifeState == LIVE)
		{
			sumAreaWeightingFactorsBySizeClass(areaWeightingFactorLive_, savrLive_, summedWeightingFactors);
			assignWeightingFactorBySizeClass(savrLive_, summedWeightingFactors, sizeSortedWeightingFactorsLive_);
		}
	}

	relativeWeightedSurfaceArea_[DEAD] = weightedSurfaceArea_[DEAD] / (weightedSurfaceArea_[DEAD] + weightedSurfaceArea_[LIVE]);
	relativeWeightedSurfaceArea_[LIVE] = 1.0 - relativeWeightedSurfaceArea_[DEAD];
}    

void SurfaceFuelbedIntermediates::calculateWeightedSurfaceAreas(int lifeState)
{
	for (int i = 0; i < MAX_LIFE_STATES; i++)
	{
		weightedSurfaceArea_[lifeState] = 0.0;
	}
	
	bool isUsingPalmettoGallbery = surfaceInputs_->isUsingPalmettoGallberry();
	if (isUsingPalmettoGallbery)
	{
		fuelDensity_[DEAD] = 30.0;
		fuelDensity_[LIVE] = 46.0;
	}

	for (int i = 0; i < numberOfSizeClasses[lifeState]; i++)
	{
		if (lifeState == DEAD)
		{
			//surfaceAreaDead_[i] = loadDead_[i] * savrDead_[i] / OVENDRY_FUEL_DENSITY;
			surfaceAreaDead_[i] = loadDead_[i] * savrDead_[i] / fuelDensity_[DEAD];
			weightedSurfaceArea_[lifeState] += surfaceAreaDead_[i];
		}
		if (lifeState == LIVE)
		{
			//surfaceAreaLive_[i] = loadLive_[i] * savrLive_[i] / OVENDRY_FUEL_DENSITY;
			surfaceAreaLive_[i] = loadLive_[i] * savrLive_[i] / fuelDensity_[LIVE];
			weightedSurfaceArea_[lifeState] += surfaceAreaLive_[i];
		}
	}
}

void SurfaceFuelbedIntermediates::calculateAreaWeightingFactors(int lifeState)
{
	for (int i = 0; i < numberOfSizeClasses[lifeState]; i++)
	{
		if (weightedSurfaceArea_[lifeState] > 1.0e-7)
		{
			if (lifeState == DEAD)
			{
				areaWeightingFactorDead_[i] = surfaceAreaDead_[i] / weightedSurfaceArea_[DEAD];
			}
			if (lifeState == LIVE)
			{
				areaWeightingFactorLive_[i] = surfaceAreaLive_[i] / weightedSurfaceArea_[LIVE];
			}
		}
		else
		{
			if (lifeState == DEAD)
			{
				areaWeightingFactorDead_[i] = 0.0;
			}
			if (lifeState == LIVE)
			{
				areaWeightingFactorLive_[i] = 0.0;
			}
		}
	}
}

void SurfaceFuelbedIntermediates::sumAreaWeightingFactorsBySizeClass(const double areaWeightingFactorDeadOrLive[MAX_PARTICLES], const double savrDeadOrLive[MAX_PARTICLES], 
	double summedWeightingFactors[MAX_PARTICLES])
{
	// savrDeadOrLive[] is an alias for savrDead[] or savrLive[], which is determined by the method caller 
	// areaWeightingFactorDeadOrLive  is an alias for areaWeightingFactorDead[] or areaWeightingFactorLive[], 
	// which is determined by the method caller 

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		summedWeightingFactors[i] = 0.0;
	}

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (savrDeadOrLive[i] >= 1200.0)
		{
			summedWeightingFactors[0] += areaWeightingFactorDeadOrLive[i];
		}
		else if (savrDeadOrLive[i] >= 192.0)
		{
			summedWeightingFactors[1] += areaWeightingFactorDeadOrLive[i];
		}
		else if (savrDeadOrLive[i] >= 96.0)
		{
			summedWeightingFactors[2] += areaWeightingFactorDeadOrLive[i];
		}
		else if (savrDeadOrLive[i] >= 48.0)
		{
			summedWeightingFactors[3] += areaWeightingFactorDeadOrLive[i];
		}
		else if (savrDeadOrLive[i] >= 16.0)
		{
			summedWeightingFactors[4] += areaWeightingFactorDeadOrLive[i];
		}
	}
}

void SurfaceFuelbedIntermediates::assignWeightingFactorBySizeClass(const double savrDeadOrLive[MAX_PARTICLES], const double summedWeightingFactors[MAX_PARTICLES],
	double sizeSortedWeightingFactorsDeadOrLive[MAX_PARTICLES])
{
	// savrDeadOrLive[] is an alias for savrDead[] or savrLive[], which is determined by the method caller 
	// sizeSortedWeightingFactorsDeadOrLive[] is an alias for sizeSortedWeightingFactorsDead[] or sizeSortedWeightingFactorsLive[], 
	// which is determined by the method caller

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (savrDeadOrLive[i] >= 1200.0)
		{
			sizeSortedWeightingFactorsDeadOrLive[i] = summedWeightingFactors[0];
		}
		else if (savrDeadOrLive[i] >= 192.0)
		{
			sizeSortedWeightingFactorsDeadOrLive[i] = summedWeightingFactors[1];
		}
		else if (savrDeadOrLive[i] >= 96.0)
		{
			sizeSortedWeightingFactorsDeadOrLive[i] = summedWeightingFactors[2];
		}
		else if (savrDeadOrLive[i] >= 48.0)
		{
			sizeSortedWeightingFactorsDeadOrLive[i] = summedWeightingFactors[3];
		}
		else if (savrDeadOrLive[i] >= 16.0)
		{
			sizeSortedWeightingFactorsDeadOrLive[i] = summedWeightingFactors[4];
		}
		else
		{
			sizeSortedWeightingFactorsDeadOrLive[i] = 0.0;
		}
	}
}

void SurfaceFuelbedIntermediates::calculateLiveMoistureOfExtinction()
{
	if (numberOfLive_ != 0)
	{
		double fineDead = 0.0;					// Fine dead fuel load
		double fineLive = 0.0;					// Fine dead fuel load
		double fineFuelsWeightingFactor = 0.0;	// Exponential weighting factors for fine fuels, Albini 1976, p. 89
		double weightedMoistureFineDead = 0.0;	// Weighted sum of find dead moisture content
		double fineDeadMoisture = 0.0;			// Fine dead moisture content, Albini 1976, p. 89
		double fineDeadOverFineLive = 0.0;		// Ratio of fine fuel loadings, dead/living, Albini 1976, p. 89

		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			if (savrDead_[i] > 1.0e-7)
			{
				fineFuelsWeightingFactor = loadDead_[i] * exp(-138.0 / savrDead_[i]);
			}
			fineDead += fineFuelsWeightingFactor;
			weightedMoistureFineDead += fineFuelsWeightingFactor * moistureDead_[i];
		}
		if (fineDead > 1.0e-07)
		{
			fineDeadMoisture = weightedMoistureFineDead / fineDead;
		}
		for (int i = 0; i < numberOfLive_; i++)
		{
			if (savrLive_[i] > 1.0e-07)
			{
				fineLive += loadLive_[i] * exp(-500.0 / savrLive_[i]);
			}
		}
		if (fineLive > 1.0e-7)
		{
			fineDeadOverFineLive = fineDead / fineLive;
		}
		moistureOfExtinction_[LIVE] = (2.9 * fineDeadOverFineLive * (1.0 - fineDeadMoisture / moistureOfExtinction_[DEAD])) - 0.226;
		if (moistureOfExtinction_[LIVE] < moistureOfExtinction_[DEAD])
		{
			moistureOfExtinction_[LIVE] = moistureOfExtinction_[DEAD];
		}
	}
}

void SurfaceFuelbedIntermediates::initializeMemberVariables()
{
	const int NUMBER_OF_LIVE_SIZE_CLASSES = 2;

	relativePackingRatio_ = 0.0;
	fuelModelNumber_ = 0;
	numberOfDead_ = 0;
	numberOfLive_ = 0;
	liveFuelMois_ = 0.0;
	liveFuelMext_ = 0.0;
	sigma_ = 0.0;
	bulkDensity_ = 0.0;
	packingRatio_ = 0.0;
	//totalLoad_ = 0.0;
	heatSink_ = 0.0;
	totalSilicaContent_ = 0.0555;

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		
		sizeSortedWeightingFactorsDead_[i] = 0.0;
		sizeSortedWeightingFactorsLive_[i] = 0.0;
		areaWeightingFactorDead_[i] = 0.0;
		areaWeightingFactorLive_[i] = 0.0;
		surfaceAreaDead_[i] = 0.0;
		surfaceAreaLive_[i] = 0.0;
		moistureDead_[i] = 0.0;
		moistureLive_[i] = 0.0;
		loadDead_[i] = 0.0;
		loadLive_[i] = 0.0;
		savrDead_[i] = 0.0;
		savrLive_[i] = 0.0;
		heatDead_[i] = 0.0;
		heatLive_[i] = 0.0;
		silicaEffectiveDead_[i] = 0.01;
		if (i < NUMBER_OF_LIVE_SIZE_CLASSES)
		{
			silicaEffectiveLive_[i] = 0.01;
		}
		else
		{
			silicaEffectiveLive_[i] = 0.0;
		}
	}

	for (int i = 0; i < MAX_LIFE_STATES; i++)
	{
		totalLoad_[i] = 0.0;
		relativeWeightedSurfaceArea_[i] = 0.0;
		moistureOfExtinction_[i] = 0.0;
		weightedSurfaceArea_[i] = 0.0;
		weightedMoisture_[i] = 0.0;
		weightedSilica_[i] = 0.0;
		fuelDensity_[i] = 32; // Average density of dry fuel in lbs/ft^3, Albini 1976, p. 91
	}
}

double SurfaceFuelbedIntermediates::getFuelbedDepth() const
{
	double fuelbedDepth = fuelModels_->getFuelbedDepth(fuelModelNumber_);
	return fuelbedDepth;
}

double SurfaceFuelbedIntermediates::getBulkDensity() const
{
	return bulkDensity_;
}

double SurfaceFuelbedIntermediates::getPackingRatio() const
{
	return packingRatio_;
}

double SurfaceFuelbedIntermediates::getRelativePackingRatio() const
{
	return relativePackingRatio_;
}

double SurfaceFuelbedIntermediates::getSigma() const
{
	return sigma_;
}

double SurfaceFuelbedIntermediates::getHeatSink() const
{
	return heatSink_;
}

double SurfaceFuelbedIntermediates::getWeightedMoistureByLifeState(int lifeState) const
{
	return weightedMoisture_[lifeState];
}

double SurfaceFuelbedIntermediates::getMoistureOfExtinctionByLifeState(int lifeState) const
{
	return moistureOfExtinction_[lifeState];
}

double SurfaceFuelbedIntermediates::getWeightedHeatByLifeState(int lifeState) const
{
	return weightedHeat_[lifeState];
}

double SurfaceFuelbedIntermediates::getWeightedSilicaByLifeState(int lifeState) const
{
	return weightedSilica_[lifeState];
}

double SurfaceFuelbedIntermediates::getWeightedFuelLoadByLifeState(int lifeState) const
{
	return weightedFuelLoad_[lifeState];
}
