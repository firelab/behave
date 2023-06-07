/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating intermediate values used in Rothermel Model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
******************************************************************************
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

#include "surfaceFuelbedIntermediates.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "fuelModels.h"
#include "surfaceInputs.h"

SurfaceFuelbedIntermediates::SurfaceFuelbedIntermediates()
{

}

SurfaceFuelbedIntermediates::SurfaceFuelbedIntermediates(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs)
{
    fuelModels_ = &fuelModels;
    surfaceInputs_ = &surfaceInputs;
    initializeMembers();
}

SurfaceFuelbedIntermediates::SurfaceFuelbedIntermediates(const SurfaceFuelbedIntermediates& rhs)
{
    memberwiseCopyAssignment(rhs);
}

SurfaceFuelbedIntermediates& SurfaceFuelbedIntermediates::operator=(const SurfaceFuelbedIntermediates& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void SurfaceFuelbedIntermediates::memberwiseCopyAssignment(const SurfaceFuelbedIntermediates& rhs)
{
    const int NUMBER_OF_LIVE_SIZE_CLASSES = 2;

    palmettoGallberry_ = rhs.palmettoGallberry_;
    westernAspen_ = rhs.westernAspen_;

    depth_ = rhs.depth_;
    relativePackingRatio_ = rhs.relativePackingRatio_;
    fuelModelNumber_ = rhs.fuelModelNumber_;
    sigma_ = rhs.sigma_;
    bulkDensity_ = rhs.bulkDensity_;
    packingRatio_ = rhs.packingRatio_;
    heatSink_ = rhs.heatSink_;
    totalSilicaContent_ = 0.0555;

    for (int i = 0; i < FuelConstants::MAX_SAVR_SIZE_CLASSES; i++)
    {
        sizeSortedFractionOfSurfaceAreaDead_[i] = rhs.sizeSortedFractionOfSurfaceAreaDead_[i];
        sizeSortedFractionOfSurfaceAreaLive_[i] = rhs.sizeSortedFractionOfSurfaceAreaLive_[i];
    }
    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        fractionOfTotalSurfaceAreaDead_[i] = rhs.fractionOfTotalSurfaceAreaDead_[i];
        fractionOfTotalSurfaceAreaLive_[i] = rhs.fractionOfTotalSurfaceAreaLive_[i];
        surfaceAreaDead_[i] = rhs.surfaceAreaDead_[i];
        surfaceAreaLive_[i] = rhs.surfaceAreaLive_[i];
        moistureDead_[i] = rhs.moistureDead_[i];
        moistureLive_[i] = rhs.moistureLive_[i];
        loadDead_[i] = rhs.loadDead_[i];
        loadLive_[i] = rhs.loadLive_[i];
        savrDead_[i] = rhs.savrDead_[i];
        savrLive_[i] = rhs.savrLive_[i];
        heatDead_[i] = rhs.heatDead_[i];
        heatLive_[i] = rhs.heatLive_[i];
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
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
    {
        numberOfSizeClasses_[i] = rhs.numberOfSizeClasses_[i];
        totalLoadForLifeState_[i] = rhs.totalLoadForLifeState_[i];
        fractionOfTotalSurfaceArea_[i] = rhs.fractionOfTotalSurfaceArea_[i];
        moistureOfExtinction_[i] = rhs.moistureOfExtinction_[i];
        totalSurfaceArea_[i] = rhs.totalSurfaceArea_[i];
        weightedMoisture_[i] = rhs.weightedMoisture_[i];
        weightedSilica_[i] = rhs.weightedSilica_[i];
        fuelDensity_[i] = 32; // Average density of dry fuel in lbs/ft^3, Albini 1976, p. 91
    }
}

SurfaceFuelbedIntermediates::~SurfaceFuelbedIntermediates()
{

}

void SurfaceFuelbedIntermediates::calculateFuelbedIntermediates(int fuelModelNumber)
{
    // TODO: Look into the creation of two new classes, FuelBed and Particle, these
    // new classes should aid in refactoring and also improve the overall design - WMC 08/2015

    // Rothermel spread equation based on BEHAVE source code,
    // support for dynamic fuel models added 10/13/2004

    //double ovendryFuelLoad = 0.0;           // Ovendry fuel loading, Rothermel 1972
    double optimumPackingRatio = 0.0;       // Optimum packing ratio, Rothermel 1972, equation 37
    bool isDynamic = false;                 // Whether or not fuel model is dynamic

    initializeMembers(); // Reset member variables to zero to forget previous state  

    fuelModelNumber_ = fuelModelNumber;

    setFuelLoad();

    setFuelbedDepth();

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

    // Fuel surface area weighting factors
    calculateFractionOfTotalSurfaceAreaForLifeStates();

    // Moisture of extinction
    setDeadFuelMoistureOfExtinction();
    calculateLiveMoistureOfExtinction();

    // Intermediate calculations, summing parameters by fuel component
    calculateCharacteristicSAVR();

    /* final calculations */
    double totalLoad = totalLoadForLifeState_[FuelLifeState::DEAD] + totalLoadForLifeState_[FuelLifeState::LIVE];

    bulkDensity_ = totalLoad / depth_;

    for (int lifeState = 0; lifeState < FuelConstants::MAX_LIFE_STATES; lifeState++)
    {
        //packingRatio_ = totalLoad / (depth * ovendryFuelDensity);
        packingRatio_ += totalLoadForLifeState_[lifeState] / (depth_ * fuelDensity_[lifeState]);
    }

    optimumPackingRatio = 3.348 / pow(sigma_, 0.8189);
    relativePackingRatio_ = packingRatio_ / optimumPackingRatio;

    calculateHeatSink();
    calculatePropagatingFlux();
}

void SurfaceFuelbedIntermediates::setFuelLoad()
{
    if (surfaceInputs_->getIsUsingPalmettoGallberry())
    {
        // Calculate load values for Palmetto-Gallberry
        double ageOfRough = surfaceInputs_->getAgeOfRough();
        double heightOfUnderstory = surfaceInputs_->getHeightOfUnderstory(LengthUnits::Feet);
        double palmettoCoverage = surfaceInputs_->getPalmettoCoverage(CoverUnits::Fraction);
        double overstoryBasalArea = surfaceInputs_->getOverstoryBasalArea(BasalAreaUnits::SquareFeetPerAcre);

        loadDead_[0] = palmettoGallberry_.calculatePalmettoGallberyDeadOneHourLoad(ageOfRough, heightOfUnderstory);
        loadDead_[1] = palmettoGallberry_.calculatePalmettoGallberyDeadTenHourLoad(ageOfRough, palmettoCoverage);
        loadDead_[2] = palmettoGallberry_.calculatePalmettoGallberyDeadFoliageLoad(ageOfRough, palmettoCoverage);
        loadDead_[3] = palmettoGallberry_.calculatePalmettoGallberyLitterLoad(ageOfRough, overstoryBasalArea);

        loadLive_[0] = palmettoGallberry_.calculatePalmettoGallberyLiveOneHourLoad(ageOfRough, heightOfUnderstory);
        loadLive_[1] = palmettoGallberry_.calculatePalmettoGallberyLiveTenHourLoad(ageOfRough, heightOfUnderstory);
        loadLive_[2] = palmettoGallberry_.calculatePalmettoGallberyLiveFoliageLoad(ageOfRough, palmettoCoverage, heightOfUnderstory);
        loadLive_[3] = 0.0;

        for (int i = 0; i < 3; i++)
        {
            silicaEffectiveLive_[i] = 0.015;
        }
    }
    else if(surfaceInputs_->getIsUsingWesternAspen())
    {
        // Calculate load values for Western Aspen
        int aspenFuelModelNumber = surfaceInputs_->getAspenFuelModelNumber();
        double aspenCuringLevel = surfaceInputs_->getAspenCuringLevel(CuringLevelUnits::Fraction);

        loadDead_[0] = westernAspen_.calculateAspenLoadDeadOneHour(aspenFuelModelNumber, aspenCuringLevel);
        loadDead_[1] = westernAspen_.calculateAspenLoadDeadTenHour(aspenFuelModelNumber);
        loadDead_[2] = 0.0;
        loadDead_[3] = 0.0;

        loadLive_[0] = westernAspen_.calculateAspenLoadLiveHerbaceous(aspenFuelModelNumber, aspenCuringLevel);
        loadLive_[1] = westernAspen_.calculateAspenLoadLiveWoody(aspenFuelModelNumber, aspenCuringLevel);
        loadLive_[2] = 0.0;
        loadLive_[3] = 0.0;
    }
    else
    {
        // Proceed as normal
        loadDead_[0] = fuelModels_->getFuelLoadOneHour(fuelModelNumber_, LoadingUnits::PoundsPerSquareFoot);
        loadDead_[1] = fuelModels_->getFuelLoadTenHour(fuelModelNumber_, LoadingUnits::PoundsPerSquareFoot);
        loadDead_[2] = fuelModels_->getFuelLoadHundredHour(fuelModelNumber_, LoadingUnits::PoundsPerSquareFoot);
        loadDead_[3] = 0.0;

        loadLive_[0] = fuelModels_->getFuelLoadLiveHerbaceous(fuelModelNumber_, LoadingUnits::PoundsPerSquareFoot);
        loadLive_[1] = fuelModels_->getFuelLoadLiveWoody(fuelModelNumber_, LoadingUnits::PoundsPerSquareFoot);
        loadLive_[2] = 0.0;
        loadLive_[3] = 0.0;
    }
}

void SurfaceFuelbedIntermediates::setMoistureContent()
{
    if (surfaceInputs_->getIsUsingPalmettoGallberry())
    {
        moistureDead_[0] = surfaceInputs_->getMoistureOneHour(MoistureUnits::Fraction);
        moistureDead_[1] = surfaceInputs_->getMoistureTenHour(MoistureUnits::Fraction);
        moistureDead_[2] = surfaceInputs_->getMoistureOneHour(MoistureUnits::Fraction);
        moistureDead_[3] = surfaceInputs_->getMoistureHundredHour(MoistureUnits::Fraction);

        moistureLive_[0] = surfaceInputs_->getMoistureLiveWoody(MoistureUnits::Fraction);
        moistureLive_[1] = surfaceInputs_->getMoistureLiveWoody(MoistureUnits::Fraction);
        moistureLive_[2] = surfaceInputs_->getMoistureLiveHerbaceous(MoistureUnits::Fraction);
        moistureLive_[3] = 0.0;
    }
    else
    {
        for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
        {
            moistureDead_[i] = 0;
            moistureLive_[i] = 0;
        }

        moistureDead_[0] = surfaceInputs_->getMoistureOneHour(MoistureUnits::Fraction);
        moistureDead_[1] = surfaceInputs_->getMoistureTenHour(MoistureUnits::Fraction);
        moistureDead_[2] = surfaceInputs_->getMoistureHundredHour(MoistureUnits::Fraction);
        moistureDead_[3] = surfaceInputs_->getMoistureOneHour(MoistureUnits::Fraction);
  
        moistureLive_[0] = surfaceInputs_->getMoistureLiveHerbaceous(MoistureUnits::Fraction);
        moistureLive_[1] = surfaceInputs_->getMoistureLiveWoody(MoistureUnits::Fraction);
    }
}

void SurfaceFuelbedIntermediates::setDeadFuelMoistureOfExtinction()
{
    if (surfaceInputs_->getIsUsingPalmettoGallberry())
    {
        moistureOfExtinction_[FuelLifeState::DEAD] = palmettoGallberry_.getMoistureOfExtinctionDead();
    }
    else if (surfaceInputs_->getIsUsingWesternAspen())
    {
        moistureOfExtinction_[FuelLifeState::DEAD] = westernAspen_.getAspenMoistureOfExtinctionDead();
    }
    else
    {
        moistureOfExtinction_[FuelLifeState::DEAD] = fuelModels_->getMoistureOfExtinctionDead(fuelModelNumber_, MoistureUnits::Fraction);
    }
}

void SurfaceFuelbedIntermediates::setFuelbedDepth()
{
    if (surfaceInputs_->getIsUsingPalmettoGallberry())
    {
        double heightOfUnderstory = surfaceInputs_->getHeightOfUnderstory(LengthUnits::Feet);
        depth_ = palmettoGallberry_.calculatePalmettoGallberyFuelBedDepth(heightOfUnderstory);
    }
    else if (surfaceInputs_->getIsUsingWesternAspen())
    {
        int aspenFuelModelNumber = surfaceInputs_->getAspenFuelModelNumber();
        depth_ = westernAspen_.getAspenFuelBedDepth(aspenFuelModelNumber);
    }
    else
    {
        depth_ = fuelModels_->getFuelbedDepth(fuelModelNumber_, LengthUnits::Feet);
    }
}

void SurfaceFuelbedIntermediates::setSAV()
{
    if (surfaceInputs_->getIsUsingPalmettoGallberry())
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
    else if (surfaceInputs_->getIsUsingWesternAspen())
    {
        // Calculate SAVR values for Western Aspen
        int aspenFuelModelNumber = surfaceInputs_->getAspenFuelModelNumber();
        double aspenCuringLevel = surfaceInputs_->getAspenCuringLevel(CuringLevelUnits::Fraction);

        savrDead_[0] = westernAspen_.calculateAspenSavrDeadOneHour(aspenFuelModelNumber, aspenCuringLevel);
        savrDead_[1] = westernAspen_.calculateAspenSavrDeadTenHour();
        savrDead_[2] = 0.0;
        savrDead_[3] = 0.0;

        savrLive_[0] = westernAspen_.calculateAspenSavrLiveHerbaceous();
        savrLive_[1] = westernAspen_.calculateAspenSavrLiveWoody(aspenFuelModelNumber, aspenCuringLevel);
        savrLive_[2] = 0.0;
        savrLive_[3] = 0.0;
    }
    else
    {
        // Proceed as normal
        savrDead_[0] = fuelModels_->getSavrOneHour(fuelModelNumber_, SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet);
        savrDead_[1] = 109.0;
        savrDead_[2] = 30.0;
        savrDead_[3] = fuelModels_->getSavrLiveHerbaceous(fuelModelNumber_, SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet);

        savrLive_[0] = fuelModels_->getSavrLiveHerbaceous(fuelModelNumber_, SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet);
        savrLive_[1] = fuelModels_->getSavrLiveWoody(fuelModelNumber_, SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet);
        savrLive_[2] = 0.0;
        savrLive_[3] = 0.0;
    }
}

void SurfaceFuelbedIntermediates::setHeatOfCombustion()
{
    const int NUMBER_OF_LIVE_SIZE_CLASSES = 3;

    double heatOfCombustionDead = 0.0;
    double heatOfCombustionLive = 0.0;

    if (surfaceInputs_->getIsUsingPalmettoGallberry())
    {
        heatOfCombustionDead = palmettoGallberry_.getHeatOfCombustionDead();
        heatOfCombustionLive = palmettoGallberry_.getHeatOfCombustionLive();
    }
    else if (surfaceInputs_->getIsUsingWesternAspen())
    {
        heatOfCombustionDead = westernAspen_.getAspenHeatOfCombustionDead();
        heatOfCombustionLive = westernAspen_.getAspenHeatOfCombustionLive();
    }
    else
    {
        heatOfCombustionDead = fuelModels_->getHeatOfCombustionDead(fuelModelNumber_, HeatOfCombustionUnits::BtusPerPound);
        heatOfCombustionLive = fuelModels_->getHeatOfCombustionLive(fuelModelNumber_, HeatOfCombustionUnits::BtusPerPound);
    }

    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
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

void SurfaceFuelbedIntermediates::calculatePropagatingFlux()
{
    propagatingFlux_ = (sigma_ < 1.0e-07)
        ? (0.0)
        : (exp((0.792 + (0.681 * sqrt(sigma_))) * (packingRatio_ + 0.1)) / (192.0 + 0.2595 * sigma_));
}

void SurfaceFuelbedIntermediates::calculateHeatSink()
{
    double qigLive[FuelConstants::MAX_PARTICLES]; // Heat of preigintion for live fuels
    double qigDead[FuelConstants::MAX_PARTICLES]; // Heat of preigintion for dead fuels

    // Initialize variables
    heatSink_ = 0;
    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        qigLive[i] = 0.0;
        qigDead[i] = 0.0;
    }

    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        if (savrDead_[i] > 1.0e-07)
        {
            qigDead[i] = 250.0 + 1116.0 * moistureDead_[i];
            heatSink_ += fractionOfTotalSurfaceArea_[FuelLifeState::DEAD] * fractionOfTotalSurfaceAreaDead_[i] * qigDead[i] * exp(-138.0 / savrDead_[i]);
        }
        if (savrLive_[i] > 1.0e-07)
        {
            qigLive[i] = 250.0 + 1116.0 * moistureLive_[i];
            heatSink_ += fractionOfTotalSurfaceArea_[FuelLifeState::LIVE] * fractionOfTotalSurfaceAreaLive_[i] * qigLive[i] * exp(-138.0 / savrLive_[i]);
        }
    }
    heatSink_ *= bulkDensity_;
}

void SurfaceFuelbedIntermediates::calculateCharacteristicSAVR()
{
    double	wnLive[FuelConstants::MAX_PARTICLES];			// Net fuel loading for live fuels, Rothermel 1972, equation 24	
    double	wnDead[FuelConstants::MAX_PARTICLES]; 			// Net fuel loading for dead fuels, Rothermel 1972, equation 24

    double	weightedSavr[FuelConstants::MAX_LIFE_STATES];	// Weighted SAVR for i-th categort (live/dead)

    // Initialize Accumulated values
    sigma_ = 0.0;
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
    {
        totalLoadForLifeState_[i] = 0.0;
        weightedHeat_[i] = 0.0;
        weightedSilica_[i] = 0.0;
        weightedMoisture_[i] = 0.0;
        weightedSavr[i] = 0.0;
        weightedFuelLoad_[i] = 0.0;
    }
    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        wnDead[i] = 0.0;
        wnLive[i] = 0.0;
    }

    if (surfaceInputs_->getIsUsingPalmettoGallberry())
    {
        totalSilicaContent_ = 0.030;
    }

    MoistureInputMode::MoistureInputModeEnum moistureInputMode = surfaceInputs_->getMoistureInputMode();
    bool isMoistureDeadAggregated = (moistureInputMode == MoistureInputMode::AllAggregate) || (moistureInputMode == MoistureInputMode::DeadAggregateAndLiveSizeClass);
    bool isMoistureLiveAggregated = (moistureInputMode == MoistureInputMode::AllAggregate) || (moistureInputMode == MoistureInputMode::LiveAggregateAndDeadSizeClass);

    if(isMoistureDeadAggregated)
    {
        weightedMoisture_[FuelLifeState::DEAD] = surfaceInputs_->getMoistureDeadAggregateValue(MoistureUnits::Fraction);
    }
    if(isMoistureLiveAggregated)
    {
        weightedMoisture_[FuelLifeState::LIVE] = surfaceInputs_->getMoistureLiveAggregateValue(MoistureUnits::Fraction);
    }

    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        if (savrDead_[i] > 1.0e-07)
        {
            wnDead[i] = loadDead_[i] * (1.0 - totalSilicaContent_); // Rothermel 1972, equation 24
            weightedHeat_[FuelLifeState::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * heatDead_[i]; // weighted heat content
            weightedSilica_[FuelLifeState::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * silicaEffectiveDead_[i]; // weighted silica content
            if(!isMoistureDeadAggregated)
            {
                weightedMoisture_[FuelLifeState::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * moistureDead_[i]; // weighted moisture content
            }
            weightedSavr[FuelLifeState::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * savrDead_[i]; // weighted SAVR
            totalLoadForLifeState_[FuelLifeState::DEAD] += loadDead_[i];
        }
        if (savrLive_[i] > 1.0e-07)
        {
            wnLive[i] = loadLive_[i] * (1.0 - totalSilicaContent_); // Rothermel 1972, equation 24
            weightedHeat_[FuelLifeState::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * heatLive_[i]; // weighted heat content
            weightedSilica_[FuelLifeState::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * silicaEffectiveLive_[i]; // weighted silica content
            if(!isMoistureLiveAggregated)
            {
                weightedMoisture_[FuelLifeState::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * moistureLive_[i]; // weighted moisture content
            }
            weightedSavr[FuelLifeState::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * savrLive_[i]; // weighted SAVR
            totalLoadForLifeState_[FuelLifeState::LIVE] += loadLive_[i];
        }
        weightedFuelLoad_[FuelLifeState::DEAD] += sizeSortedFractionOfSurfaceAreaDead_[i] * wnDead[i];
        weightedFuelLoad_[FuelLifeState::LIVE] += sizeSortedFractionOfSurfaceAreaLive_[i] * wnLive[i];
    }

    for (int lifeState = 0; lifeState < FuelConstants::MAX_LIFE_STATES; lifeState++)
    {
        sigma_ += fractionOfTotalSurfaceArea_[lifeState] * weightedSavr[lifeState];
    }
}

void SurfaceFuelbedIntermediates::countSizeClasses()
{
    // count number of fuels
    for (int i = 0; i < FuelConstants::MAX_DEAD_SIZE_CLASSES; i++)
    {
        if (loadDead_[i])
        {
            numberOfSizeClasses_[FuelLifeState::DEAD]++;
        }
    }
    for (int i = 0; i < FuelConstants::MAX_LIVE_SIZE_CLASSES; i++)
    {
        if (loadLive_[i])
        {
            numberOfSizeClasses_[FuelLifeState::LIVE]++;
        }
    }
    if (numberOfSizeClasses_[FuelLifeState::LIVE] > 0)
    {
        numberOfSizeClasses_[FuelLifeState::LIVE] = FuelConstants::MAX_LIVE_SIZE_CLASSES;  // Boost to max number
    }
    if (numberOfSizeClasses_[FuelLifeState::DEAD] > 0)
    {
        numberOfSizeClasses_[FuelLifeState::DEAD] = FuelConstants::MAX_DEAD_SIZE_CLASSES;  // Boost to max number
    }
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

void SurfaceFuelbedIntermediates::calculateFractionOfTotalSurfaceAreaForLifeStates()
{
    double summedFractionOfTotalSurfaceArea[FuelConstants::MAX_SAVR_SIZE_CLASSES];	// Intermediate weighting factors for each size class

    for (int lifeState = 0; lifeState < FuelConstants::MAX_LIFE_STATES; lifeState++)
    {
        if (numberOfSizeClasses_[lifeState] != 0)
        {
            calculateTotalSurfaceAreaForLifeState(lifeState);
            calculateFractionOfTotalSurfaceAreaForSizeClasses(lifeState);
        }
        for (int i = 0; i < FuelConstants::MAX_SAVR_SIZE_CLASSES; i++)
        {
            summedFractionOfTotalSurfaceArea[i] = 0.0;
        }
        if (lifeState == FuelLifeState::DEAD)
        {
            sumFractionOfTotalSurfaceAreaBySizeClass(fractionOfTotalSurfaceAreaDead_, savrDead_, summedFractionOfTotalSurfaceArea);
            assignFractionOfTotalSurfaceAreaBySizeClass(savrDead_, summedFractionOfTotalSurfaceArea, sizeSortedFractionOfSurfaceAreaDead_);
        }
        if (lifeState == FuelLifeState::LIVE)
        {
            sumFractionOfTotalSurfaceAreaBySizeClass(fractionOfTotalSurfaceAreaLive_, savrLive_, summedFractionOfTotalSurfaceArea);
            assignFractionOfTotalSurfaceAreaBySizeClass(savrLive_, summedFractionOfTotalSurfaceArea, sizeSortedFractionOfSurfaceAreaLive_);
        }
    }

    fractionOfTotalSurfaceArea_[FuelLifeState::DEAD] = totalSurfaceArea_[FuelLifeState::DEAD] / (totalSurfaceArea_[FuelLifeState::DEAD] +
        totalSurfaceArea_[FuelLifeState::LIVE]);
    fractionOfTotalSurfaceArea_[FuelLifeState::LIVE] = 1.0 - fractionOfTotalSurfaceArea_[FuelLifeState::DEAD];
}

void SurfaceFuelbedIntermediates::calculateTotalSurfaceAreaForLifeState(int lifeState)
{
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
    {
        totalSurfaceArea_[lifeState] = 0.0;
    }

    bool isUsingPalmettoGallbery = surfaceInputs_->getIsUsingPalmettoGallberry();
    if (isUsingPalmettoGallbery)
    {
        fuelDensity_[FuelLifeState::DEAD] = 30.0;
        fuelDensity_[FuelLifeState::LIVE] = 46.0;
    }

    for (int i = 0; i < numberOfSizeClasses_[lifeState]; i++)
    {
        if (lifeState == FuelLifeState::DEAD)
        {
            //surfaceAreaDead_[i] = loadDead_[i] * savrDead_[i] / OVENDRY_FUEL_DENSITY;
            surfaceAreaDead_[i] = loadDead_[i] * savrDead_[i] / fuelDensity_[FuelLifeState::DEAD];
            totalSurfaceArea_[lifeState] += surfaceAreaDead_[i];
        }
        if (lifeState == FuelLifeState::LIVE)
        {
            //surfaceAreaLive_[i] = loadLive_[i] * savrLive_[i] / OVENDRY_FUEL_DENSITY;
            surfaceAreaLive_[i] = loadLive_[i] * savrLive_[i] / fuelDensity_[FuelLifeState::LIVE];
            totalSurfaceArea_[lifeState] += surfaceAreaLive_[i];
        }
    }
}

void SurfaceFuelbedIntermediates::calculateFractionOfTotalSurfaceAreaForSizeClasses(int lifeState)
{
    for (int i = 0; i < numberOfSizeClasses_[lifeState]; i++)
    {
        if (totalSurfaceArea_[lifeState] > 1.0e-7)
        {
            if (lifeState == FuelLifeState::DEAD)
            {
                fractionOfTotalSurfaceAreaDead_[i] = surfaceAreaDead_[i] / totalSurfaceArea_[FuelLifeState::DEAD];
            }
            if (lifeState == FuelLifeState::LIVE)
            {
                fractionOfTotalSurfaceAreaLive_[i] = surfaceAreaLive_[i] / totalSurfaceArea_[FuelLifeState::LIVE];
            }
        }
        else
        {
            if (lifeState == FuelLifeState::DEAD)
            {
                fractionOfTotalSurfaceAreaDead_[i] = 0.0;
            }
            if (lifeState == FuelLifeState::LIVE)
            {
                fractionOfTotalSurfaceAreaLive_[i] = 0.0;
            }
        }
    }
}

void SurfaceFuelbedIntermediates::sumFractionOfTotalSurfaceAreaBySizeClass(
    const double fractionOfTotalSurfaceAreaDeadOrLive[FuelConstants::MAX_PARTICLES],
    const double savrDeadOrLive[FuelConstants::MAX_PARTICLES], double summedFractionOfTotalSurfaceArea[FuelConstants::MAX_PARTICLES])
{
    // savrDeadOrLive[] is an alias for savrDead[] or savrLive[], which is determined by the method caller 
    // fractionOfTotalSurfaceAreaDeadOrLive  is an alias for fractionOfTotalSurfaceAreaDead[] or  fractionOfTotalSurfaceAreaLive[], 
    // which is determined by the method caller 

    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        summedFractionOfTotalSurfaceArea[i] = 0.0;
    }

    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        if (savrDeadOrLive[i] >= 1200.0)
        {
            summedFractionOfTotalSurfaceArea[0] += fractionOfTotalSurfaceAreaDeadOrLive[i];
        }
        else if (savrDeadOrLive[i] >= 192.0)
        {
            summedFractionOfTotalSurfaceArea[1] += fractionOfTotalSurfaceAreaDeadOrLive[i];
        }
        else if (savrDeadOrLive[i] >= 96.0)
        {
            summedFractionOfTotalSurfaceArea[2] += fractionOfTotalSurfaceAreaDeadOrLive[i];
        }
        else if (savrDeadOrLive[i] >= 48.0)
        {
            summedFractionOfTotalSurfaceArea[3] += fractionOfTotalSurfaceAreaDeadOrLive[i];
        }
        else if (savrDeadOrLive[i] >= 16.0)
        {
            summedFractionOfTotalSurfaceArea[4] += fractionOfTotalSurfaceAreaDeadOrLive[i];
        }
    }
}

void SurfaceFuelbedIntermediates::assignFractionOfTotalSurfaceAreaBySizeClass(const double savrDeadOrLive[FuelConstants::MAX_PARTICLES],
    const double summedFractionOfTotalSurfaceArea[FuelConstants::MAX_PARTICLES],
    double sizeSortedFractionOfSurfaceAreaDeadOrLive[FuelConstants::MAX_PARTICLES])
{
    // savrDeadOrLive[] is an alias for savrDead[] or savrLive[], which is determined by the method caller 
    // sizeSortedFractionOfSurfaceAreaDeadOrLive[] is an alias for sizeSortedFractionOfSurfaceAreaDead_[] or sizeSortedFractionOfSurfaceAreaLive_[], 
    // which is determined by the method caller

    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        if (savrDeadOrLive[i] >= 1200.0)
        {
            sizeSortedFractionOfSurfaceAreaDeadOrLive[i] = summedFractionOfTotalSurfaceArea[0];
        }
        else if (savrDeadOrLive[i] >= 192.0)
        {
            sizeSortedFractionOfSurfaceAreaDeadOrLive[i] = summedFractionOfTotalSurfaceArea[1];
        }
        else if (savrDeadOrLive[i] >= 96.0)
        {
            sizeSortedFractionOfSurfaceAreaDeadOrLive[i] = summedFractionOfTotalSurfaceArea[2];
        }
        else if (savrDeadOrLive[i] >= 48.0)
        {
            sizeSortedFractionOfSurfaceAreaDeadOrLive[i] = summedFractionOfTotalSurfaceArea[3];
        }
        else if (savrDeadOrLive[i] >= 16.0)
        {
            sizeSortedFractionOfSurfaceAreaDeadOrLive[i] = summedFractionOfTotalSurfaceArea[4];
        }
        else
        {
            sizeSortedFractionOfSurfaceAreaDeadOrLive[i] = 0.0;
        }
    }
}

void SurfaceFuelbedIntermediates::calculateLiveMoistureOfExtinction()
{
    if (numberOfSizeClasses_[FuelLifeState::LIVE] != 0)
    {
        double fineDead = 0.0;					// Fine dead fuel load
        double fineLive = 0.0;					// Fine dead fuel load
        double fineFuelsWeightingFactor = 0.0;	// Exponential weighting factors for fine fuels, Albini 1976, p. 89
        double weightedMoistureFineDead = 0.0;	// Weighted sum of find dead moisture content
        double fineDeadMoisture = 0.0;			// Fine dead moisture content, Albini 1976, p. 89
        double fineDeadOverFineLive = 0.0;		// Ratio of fine fuel loadings, dead/living, Albini 1976, p. 89

        for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
        {
            fineFuelsWeightingFactor = 0.0;
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
        for (int i = 0; i < numberOfSizeClasses_[FuelLifeState::LIVE]; i++)
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
        moistureOfExtinction_[FuelLifeState::LIVE] = (2.9 * fineDeadOverFineLive *
            (1.0 - fineDeadMoisture / moistureOfExtinction_[FuelLifeState::DEAD])) - 0.226;
        if (moistureOfExtinction_[FuelLifeState::LIVE] < moistureOfExtinction_[FuelLifeState::DEAD])
        {
            moistureOfExtinction_[FuelLifeState::LIVE] = moistureOfExtinction_[FuelLifeState::DEAD];
        }
    }
}

void SurfaceFuelbedIntermediates::initializeMembers()
{
    const int NUMBER_OF_LIVE_SIZE_CLASSES = 2;

    palmettoGallberry_.initializeMembers();
    westernAspen_.initializeMembers();

    depth_ = 0.0;
    relativePackingRatio_ = 0.0;
    fuelModelNumber_ = 0;
    sigma_ = 0.0;
    bulkDensity_ = 0.0;
    packingRatio_ = 0.0;
    heatSink_ = 0.0;
    totalSilicaContent_ = 0.0555;

    for (int i = 0; i < FuelConstants::MAX_SAVR_SIZE_CLASSES; i++)
    {
        sizeSortedFractionOfSurfaceAreaDead_[i] = 0;
        sizeSortedFractionOfSurfaceAreaLive_[i] = 0;
    }
    for (int i = 0; i < FuelConstants::MAX_PARTICLES; i++)
    {
        fractionOfTotalSurfaceAreaDead_[i] = 0.0;
        fractionOfTotalSurfaceAreaLive_[i] = 0.0;
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
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
    {
        numberOfSizeClasses_[i] = 0;
        totalLoadForLifeState_[i] = 0.0;
        fractionOfTotalSurfaceArea_[i] = 0.0;
        moistureOfExtinction_[i] = 0.0;
        totalSurfaceArea_[i] = 0.0;
        weightedMoisture_[i] = 0.0;
        weightedSilica_[i] = 0.0;
        fuelDensity_[i] = 32; // Average density of dry fuel in lbs/ft^3, Albini 1976, p. 91
    }
}

double SurfaceFuelbedIntermediates::getFuelbedDepth() const
{
    return fuelModels_->getFuelbedDepth(fuelModelNumber_, LengthUnits::Feet);
}

double SurfaceFuelbedIntermediates::getBulkDensity() const
{
    return bulkDensity_;
}

double SurfaceFuelbedIntermediates::getPackingRatio() const
{
    return packingRatio_;
}

double SurfaceFuelbedIntermediates::getPropagatingFlux() const
{
    return propagatingFlux_;
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

double SurfaceFuelbedIntermediates::getWeightedMoistureByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return weightedMoisture_[lifeState];
}

double SurfaceFuelbedIntermediates::getMoistureOfExtinctionByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return moistureOfExtinction_[lifeState];
}

double SurfaceFuelbedIntermediates::getWeightedHeatByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return weightedHeat_[lifeState];
}

double SurfaceFuelbedIntermediates::getWeightedSilicaByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return weightedSilica_[lifeState];
}

double SurfaceFuelbedIntermediates::getWeightedFuelLoadByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return weightedFuelLoad_[lifeState];
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyDeadOneHourLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyDeadOneHourLoad();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyDeadTenHourLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyDeadTenHourLoad();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyDeadFoliageLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyDeadFoliageLoad();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyFuelBedDepth() const
{
    return palmettoGallberry_.getPalmettoGallberyFuelBedDepth();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyLitterLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyLitterLoad();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyLiveOneHourLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyLiveOneHourLoad();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyLiveTenHourLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyLiveTenHourLoad();
}

double SurfaceFuelbedIntermediates::getPalmettoGallberyLiveFoliageLoad() const
{
    return palmettoGallberry_.getPalmettoGallberyLiveFoliageLoad();
}

double SurfaceFuelbedIntermediates::getAspenMortality() const
{
    return westernAspen_.getAspenMortality();
}
