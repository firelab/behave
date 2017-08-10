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
#include "fuelModelSet.h"
#include "surfaceInputs.h"

SurfaceFuelbedIntermediates::SurfaceFuelbedIntermediates()
{

}

SurfaceFuelbedIntermediates::SurfaceFuelbedIntermediates(const FuelModelSet& fuelModelSet, const SurfaceInputs& surfaceInputs)
{
    fuelModelSet_ = &fuelModelSet;
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

    isUsingPalmettoGallberry_ = rhs.isUsingPalmettoGallberry_;
    isUsingWesternAspen_ = rhs.isUsingWesternAspen_;

    depth_ = rhs.depth_;
    relativePackingRatio_ = rhs.relativePackingRatio_;
    fuelModelNumber_ = rhs.fuelModelNumber_;
    liveFuelMois_ = rhs.liveFuelMois_;
    liveFuelMext_ = rhs.liveFuelMext_;
    sigma_ = rhs.sigma_;
    bulkDensity_ = rhs.bulkDensity_;
    packingRatio_ = rhs.packingRatio_;
    heatSink_ = rhs.heatSink_;
    totalSilicaContent_ = 0.0555;

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_SAVR_SIZE_CLASSES; i++)
    {
        sizeSortedFractionOfSurfaceAreaDead_[i] = rhs.sizeSortedFractionOfSurfaceAreaDead_[i];
        sizeSortedFractionOfSurfaceAreaLive_[i] = rhs.sizeSortedFractionOfSurfaceAreaLive_[i];
    }
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; i++)
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

    isDynamic = fuelModelSet_->getIsDynamic(fuelModelNumber_);
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
    double totalLoad = totalLoadForLifeState_[SurfaceInputs::FuelConstants::DEAD] + totalLoadForLifeState_[SurfaceInputs::FuelConstants::LIVE];

    bulkDensity_ = totalLoad / depth_;

    for (int lifeState = 0; lifeState < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; lifeState++)
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
    if (isUsingPalmettoGallberry_)
    {
        // Calculate load values for Palmetto-Gallberry
        double ageOfRough = surfaceInputs_->getAgeOfRough();
        double heightOfUnderstory = surfaceInputs_->getHeightOfUnderstory();
        double palmettoCoverage = surfaceInputs_->getPalmettoCoverage();
        double overstoryBasalArea = surfaceInputs_->getOverstoryBasalArea();

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
    else if (isUsingWesternAspen_)
    {
        // Calculate load values for Western Aspen
        int aspenFuelModelNumber = surfaceInputs_->getAspenFuelModelNumber();
        double aspenCuringLevel = surfaceInputs_->getAspenCuringLevel();

        loadDead_[0] = westernAspen_.getAspenLoadDeadOneHour(aspenFuelModelNumber, aspenCuringLevel);
        loadDead_[1] = westernAspen_.getAspenLoadDeadTenHour(aspenFuelModelNumber);
        loadDead_[2] = 0.0;
        loadDead_[3] = 0.0;

        loadLive_[0] = westernAspen_.getAspenLoadLiveHerbaceous(aspenFuelModelNumber, aspenCuringLevel);
        loadLive_[1] = westernAspen_.getAspenLoadLiveWoody(aspenFuelModelNumber, aspenCuringLevel);
        loadLive_[2] = 0.0;
        loadLive_[3] = 0.0;
    }
    else
    {
        // Proceed as normal
        loadDead_[0] = fuelModelSet_->getFuelLoadOneHour(fuelModelNumber_);
        loadDead_[1] = fuelModelSet_->getFuelLoadTenHour(fuelModelNumber_);
        loadDead_[2] = fuelModelSet_->getFuelLoadHundredHour(fuelModelNumber_);
        loadDead_[3] = 0.0;

        loadLive_[0] = fuelModelSet_->getFuelLoadLiveHerbaceous(fuelModelNumber_);
        loadLive_[1] = fuelModelSet_->getFuelLoadLiveWoody(fuelModelNumber_);
        loadLive_[2] = 0.0;
        loadLive_[3] = 0.0;
    }
}

void SurfaceFuelbedIntermediates::setMoistureContent()
{
    if (isUsingPalmettoGallberry_)
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
        for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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
    if (isUsingPalmettoGallberry_)
    {
        moistureOfExtinction_[SurfaceInputs::FuelConstants::DEAD] = palmettoGallberry_.getMoistureOfExtinctionDead();
    }
    else if (isUsingWesternAspen_)
    {
        moistureOfExtinction_[SurfaceInputs::FuelConstants::DEAD] = westernAspen_.getAspenMoistureOfExtinctionDead();
    }
    else
    {
        moistureOfExtinction_[SurfaceInputs::FuelConstants::DEAD] = fuelModelSet_->getMoistureOfExtinctionDead(fuelModelNumber_);
    }
}

void SurfaceFuelbedIntermediates::setFuelbedDepth()
{
    if (isUsingPalmettoGallberry_)
    {
        double heightOfUnderstory = surfaceInputs_->getHeightOfUnderstory();
        depth_ = palmettoGallberry_.calculatePalmettoGallberyFuelBedDepth(heightOfUnderstory);
    }
    else if (isUsingWesternAspen_)
    {
        int aspenFuelModelNumber = surfaceInputs_->getAspenFuelModelNumber();
        depth_ = westernAspen_.getAspenFuelBedDepth(aspenFuelModelNumber);
    }
    else
    {
        depth_ = fuelModelSet_->getFuelbedDepth(fuelModelNumber_);
    }
}

void SurfaceFuelbedIntermediates::setSAV()
{
    if (isUsingPalmettoGallberry_)
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
    else if (isUsingWesternAspen_)
    {
        // Calculate SAVR values for Western Aspen
        int aspenFuelModelNumber = surfaceInputs_->getAspenFuelModelNumber();
        double aspenCuringLevel = surfaceInputs_->getAspenCuringLevel();

        savrDead_[0] = westernAspen_.getAspenSavrDeadOneHour(aspenFuelModelNumber, aspenCuringLevel);
        savrDead_[1] = westernAspen_.getAspenSavrDeadTenHour();
        savrDead_[2] = 0.0;
        savrDead_[3] = 0.0;

        savrLive_[0] = westernAspen_.getAspenSavrLiveHerbaceous();
        savrLive_[1] = westernAspen_.getAspenSavrLiveWoody(aspenFuelModelNumber, aspenCuringLevel);
        savrLive_[2] = 0.0;
        savrLive_[3] = 0.0;
    }
    else
    {
        // Proceed as normal
        savrDead_[0] = fuelModelSet_->getSavrOneHour(fuelModelNumber_);
        savrDead_[1] = 109.0;
        savrDead_[2] = 30.0;
        savrDead_[3] = fuelModelSet_->getSavrLiveHerbaceous(fuelModelNumber_);

        savrLive_[0] = fuelModelSet_->getSavrLiveHerbaceous(fuelModelNumber_);
        savrLive_[1] = fuelModelSet_->getSavrLiveWoody(fuelModelNumber_);
        savrLive_[2] = 0.0;
        savrLive_[3] = 0.0;
    }
}

void SurfaceFuelbedIntermediates::setHeatOfCombustion()
{
    const int NUMBER_OF_LIVE_SIZE_CLASSES = 3;

    double heatOfCombustionDead = 0.0;
    double heatOfCombustionLive = 0.0;

    if (isUsingPalmettoGallberry_)
    {
        heatOfCombustionDead = palmettoGallberry_.getHeatOfCombustionDead();
        heatOfCombustionLive = palmettoGallberry_.getHeatOfCombustionLive();
    }
    else if (isUsingWesternAspen_)
    {
        heatOfCombustionDead = westernAspen_.getAspenHeatOfCombustionDead();
        heatOfCombustionLive = westernAspen_.getAspenHeatOfCombustionLive();
    }
    else
    {
        heatOfCombustionDead = fuelModelSet_->getHeatOfCombustionDead(fuelModelNumber_);
        heatOfCombustionLive = fuelModelSet_->getHeatOfCombustionLive(fuelModelNumber_);
    }

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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
        ? (0.)
        : (exp((0.792 + 0.681 * sqrt(sigma_)) * (packingRatio_ + 0.1)) / (192. + 0.2595 * sigma_));
}

void SurfaceFuelbedIntermediates::calculateHeatSink()
{
    double qigLive[SurfaceInputs::FuelConstants::MAX_PARTICLES]; // Heat of preigintion for live fuels
    double qigDead[SurfaceInputs::FuelConstants::MAX_PARTICLES]; // Heat of preigintion for dead fuels

    // Initialize variables
    heatSink_ = 0;
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
    {
        qigLive[i] = 0.0;
        qigDead[i] = 0.0;
    }

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
    {
        if (savrDead_[i] > 1.0e-07)
        {
            qigDead[i] = 250.0 + 1116.0 * moistureDead_[i];
            heatSink_ += fractionOfTotalSurfaceArea_[SurfaceInputs::FuelConstants::DEAD] * fractionOfTotalSurfaceAreaDead_[i] * qigDead[i] * exp(-138.0 / savrDead_[i]);
        }
        if (savrLive_[i] > 1.0e-07)
        {
            qigLive[i] = 250.0 + 1116.0 * moistureLive_[i];
            heatSink_ += fractionOfTotalSurfaceArea_[SurfaceInputs::FuelConstants::LIVE] * fractionOfTotalSurfaceAreaLive_[i] * qigLive[i] * exp(-138.0 / savrLive_[i]);
        }
    }
    heatSink_ *= bulkDensity_;
}

void SurfaceFuelbedIntermediates::calculateCharacteristicSAVR()
{
    double	wnLive[SurfaceInputs::FuelConstants::MAX_PARTICLES];			// Net fuel loading for live fuels, Rothermel 1972, equation 24	
    double	wnDead[SurfaceInputs::FuelConstants::MAX_PARTICLES]; 			// Net fuel loading for dead fuels, Rothermel 1972, equation 24

    double	weightedSavr[SurfaceInputs::FuelConstants::MAX_LIFE_STATES];	// Weighted SAVR for i-th categort (live/dead)

    // Initialize Accumulated values
    sigma_ = 0.0;
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; i++)
    {
        totalLoadForLifeState_[i] = 0.0;
        weightedHeat_[i] = 0.0;
        weightedSilica_[i] = 0.0;
        weightedMoisture_[i] = 0.0;
        weightedSavr[i] = 0.0;
        weightedFuelLoad_[i] = 0.0;
    }
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
    {
        wnDead[i] = 0.0;
        wnLive[i] = 0.0;
    }

    if (isUsingPalmettoGallberry_)
    {
        totalSilicaContent_ = 0.030;
    }

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
    {
        if (savrDead_[i] > 1.0e-07)
        {
            wnDead[i] = loadDead_[i] * (1.0 - totalSilicaContent_); // Rothermel 1972, equation 24
            weightedHeat_[SurfaceInputs::FuelConstants::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * heatDead_[i]; // weighted heat content
            weightedSilica_[SurfaceInputs::FuelConstants::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * silicaEffectiveDead_[i]; // weighted silica content
            weightedMoisture_[SurfaceInputs::FuelConstants::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * moistureDead_[i]; // weighted moisture content
            weightedSavr[SurfaceInputs::FuelConstants::DEAD] += fractionOfTotalSurfaceAreaDead_[i] * savrDead_[i]; // weighted SAVR
            totalLoadForLifeState_[SurfaceInputs::FuelConstants::DEAD] += loadDead_[i];
        }
        if (savrLive_[i] > 1.0e-07)
        {
            wnLive[i] = loadLive_[i] * (1.0 - totalSilicaContent_); // Rothermel 1972, equation 24
            weightedHeat_[SurfaceInputs::FuelConstants::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * heatLive_[i]; // weighted heat content
            weightedSilica_[SurfaceInputs::FuelConstants::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * silicaEffectiveLive_[i]; // weighted silica content
            weightedMoisture_[SurfaceInputs::FuelConstants::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * moistureLive_[i]; // weighted moisture content
            weightedSavr[SurfaceInputs::FuelConstants::LIVE] += fractionOfTotalSurfaceAreaLive_[i] * savrLive_[i]; // weighted SAVR
            totalLoadForLifeState_[SurfaceInputs::FuelConstants::LIVE] += loadLive_[i];
        }
        weightedFuelLoad_[SurfaceInputs::FuelConstants::DEAD] += sizeSortedFractionOfSurfaceAreaDead_[i] * wnDead[i];
        weightedFuelLoad_[SurfaceInputs::FuelConstants::LIVE] += sizeSortedFractionOfSurfaceAreaLive_[i] * wnLive[i];
    }

    for (int lifeState = 0; lifeState < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; lifeState++)
    {
        sigma_ += fractionOfTotalSurfaceArea_[lifeState] * weightedSavr[lifeState];
    }
}

void SurfaceFuelbedIntermediates::countSizeClasses()
{
    // count number of fuels
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_DEAD_SIZE_CLASSES; i++)
    {
        if (loadDead_[i])
        {
            numberOfSizeClasses_[SurfaceInputs::FuelConstants::DEAD]++;
        }
    }
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_LIVE_SIZE_CLASSES; i++)
    {
        if (loadLive_[i])
        {
            numberOfSizeClasses_[SurfaceInputs::FuelConstants::LIVE]++;
        }
    }
    if (numberOfSizeClasses_[SurfaceInputs::FuelConstants::LIVE] > 0)
    {
        numberOfSizeClasses_[SurfaceInputs::FuelConstants::LIVE] = SurfaceInputs::FuelConstants::MAX_LIVE_SIZE_CLASSES;  // Boost to max number
    }
    if (numberOfSizeClasses_[SurfaceInputs::FuelConstants::DEAD] > 0)
    {
        numberOfSizeClasses_[SurfaceInputs::FuelConstants::DEAD] = SurfaceInputs::FuelConstants::MAX_DEAD_SIZE_CLASSES;  // Boost to max number
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
    double summedFractionOfTotalSurfaceArea[SurfaceInputs::FuelConstants::MAX_SAVR_SIZE_CLASSES];	// Intermediate weighting factors for each size class

    for (int lifeState = 0; lifeState < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; lifeState++)
    {
        if (numberOfSizeClasses_[lifeState] != 0)
        {
            calculateTotalSurfaceAreaForLifeState(lifeState);
            calculateFractionOfTotalSurfaceAreaForSizeClasses(lifeState);
        }
        for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_SAVR_SIZE_CLASSES; i++)
        {
            summedFractionOfTotalSurfaceArea[i] = 0.0;
        }
        if (lifeState == SurfaceInputs::FuelConstants::DEAD)
        {
            sumFractionOfTotalSurfaceAreaBySizeClass(fractionOfTotalSurfaceAreaDead_, savrDead_, summedFractionOfTotalSurfaceArea);
            assignFractionOfTotalSurfaceAreaBySizeClass(savrDead_, summedFractionOfTotalSurfaceArea, sizeSortedFractionOfSurfaceAreaDead_);
        }
        if (lifeState == SurfaceInputs::FuelConstants::LIVE)
        {
            sumFractionOfTotalSurfaceAreaBySizeClass(fractionOfTotalSurfaceAreaLive_, savrLive_, summedFractionOfTotalSurfaceArea);
            assignFractionOfTotalSurfaceAreaBySizeClass(savrLive_, summedFractionOfTotalSurfaceArea, sizeSortedFractionOfSurfaceAreaLive_);
        }
    }

    fractionOfTotalSurfaceArea_[SurfaceInputs::FuelConstants::DEAD] = totalSurfaceArea_[SurfaceInputs::FuelConstants::DEAD] / (totalSurfaceArea_[SurfaceInputs::FuelConstants::DEAD] +
        totalSurfaceArea_[SurfaceInputs::FuelConstants::LIVE]);
    fractionOfTotalSurfaceArea_[SurfaceInputs::FuelConstants::LIVE] = 1.0 - fractionOfTotalSurfaceArea_[SurfaceInputs::FuelConstants::DEAD];
}

void SurfaceFuelbedIntermediates::calculateTotalSurfaceAreaForLifeState(int lifeState)
{
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; i++)
    {
        totalSurfaceArea_[lifeState] = 0.0;
    }

    bool isUsingPalmettoGallbery = surfaceInputs_->isUsingPalmettoGallberry();
    if (isUsingPalmettoGallbery)
    {
        fuelDensity_[SurfaceInputs::FuelConstants::DEAD] = 30.0;
        fuelDensity_[SurfaceInputs::FuelConstants::LIVE] = 46.0;
    }

    for (int i = 0; i < numberOfSizeClasses_[lifeState]; i++)
    {
        if (lifeState == SurfaceInputs::FuelConstants::DEAD)
        {
            //surfaceAreaDead_[i] = loadDead_[i] * savrDead_[i] / OVENDRY_FUEL_DENSITY;
            surfaceAreaDead_[i] = loadDead_[i] * savrDead_[i] / fuelDensity_[SurfaceInputs::FuelConstants::DEAD];
            totalSurfaceArea_[lifeState] += surfaceAreaDead_[i];
        }
        if (lifeState == SurfaceInputs::FuelConstants::LIVE)
        {
            //surfaceAreaLive_[i] = loadLive_[i] * savrLive_[i] / OVENDRY_FUEL_DENSITY;
            surfaceAreaLive_[i] = loadLive_[i] * savrLive_[i] / fuelDensity_[SurfaceInputs::FuelConstants::LIVE];
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
            if (lifeState == SurfaceInputs::FuelConstants::DEAD)
            {
                fractionOfTotalSurfaceAreaDead_[i] = surfaceAreaDead_[i] / totalSurfaceArea_[SurfaceInputs::FuelConstants::DEAD];
            }
            if (lifeState == SurfaceInputs::FuelConstants::LIVE)
            {
                fractionOfTotalSurfaceAreaLive_[i] = surfaceAreaLive_[i] / totalSurfaceArea_[SurfaceInputs::FuelConstants::LIVE];
            }
        }
        else
        {
            if (lifeState == SurfaceInputs::FuelConstants::DEAD)
            {
                fractionOfTotalSurfaceAreaDead_[i] = 0.0;
            }
            if (lifeState == SurfaceInputs::FuelConstants::LIVE)
            {
                fractionOfTotalSurfaceAreaLive_[i] = 0.0;
            }
        }
    }
}

void SurfaceFuelbedIntermediates::sumFractionOfTotalSurfaceAreaBySizeClass(
    const double fractionOfTotalSurfaceAreaDeadOrLive[SurfaceInputs::FuelConstants::MAX_PARTICLES],
    const double savrDeadOrLive[SurfaceInputs::FuelConstants::MAX_PARTICLES], double summedFractionOfTotalSurfaceArea[SurfaceInputs::FuelConstants::MAX_PARTICLES])
{
    // savrDeadOrLive[] is an alias for savrDead[] or savrLive[], which is determined by the method caller 
    // fractionOfTotalSurfaceAreaDeadOrLive  is an alias for fractionOfTotalSurfaceAreaDead[] or  fractionOfTotalSurfaceAreaLive[], 
    // which is determined by the method caller 

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
    {
        summedFractionOfTotalSurfaceArea[i] = 0.0;
    }

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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

void SurfaceFuelbedIntermediates::assignFractionOfTotalSurfaceAreaBySizeClass(const double savrDeadOrLive[SurfaceInputs::FuelConstants::MAX_PARTICLES],
    const double summedFractionOfTotalSurfaceArea[SurfaceInputs::FuelConstants::MAX_PARTICLES],
    double sizeSortedFractionOfSurfaceAreaDeadOrLive[SurfaceInputs::FuelConstants::MAX_PARTICLES])
{
    // savrDeadOrLive[] is an alias for savrDead[] or savrLive[], which is determined by the method caller 
    // sizeSortedFractionOfSurfaceAreaDeadOrLive[] is an alias for sizeSortedFractionOfSurfaceAreaDead_[] or sizeSortedFractionOfSurfaceAreaLive_[], 
    // which is determined by the method caller

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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
    if (numberOfSizeClasses_[SurfaceInputs::FuelConstants::LIVE] != 0)
    {
        double fineDead = 0.0;					// Fine dead fuel load
        double fineLive = 0.0;					// Fine dead fuel load
        double fineFuelsWeightingFactor = 0.0;	// Exponential weighting factors for fine fuels, Albini 1976, p. 89
        double weightedMoistureFineDead = 0.0;	// Weighted sum of find dead moisture content
        double fineDeadMoisture = 0.0;			// Fine dead moisture content, Albini 1976, p. 89
        double fineDeadOverFineLive = 0.0;		// Ratio of fine fuel loadings, dead/living, Albini 1976, p. 89

        for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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
        for (int i = 0; i < numberOfSizeClasses_[SurfaceInputs::FuelConstants::LIVE]; i++)
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
        moistureOfExtinction_[SurfaceInputs::FuelConstants::LIVE] = (2.9 * fineDeadOverFineLive *
            (1.0 - fineDeadMoisture / moistureOfExtinction_[SurfaceInputs::FuelConstants::DEAD])) - 0.226;
        if (moistureOfExtinction_[SurfaceInputs::FuelConstants::LIVE] < moistureOfExtinction_[SurfaceInputs::FuelConstants::DEAD])
        {
            moistureOfExtinction_[SurfaceInputs::FuelConstants::LIVE] = moistureOfExtinction_[SurfaceInputs::FuelConstants::DEAD];
        }
    }
}

void SurfaceFuelbedIntermediates::initializeMembers()
{
    const int NUMBER_OF_LIVE_SIZE_CLASSES = 2;

    palmettoGallberry_.initializeMembers();
    westernAspen_.initializeMembers();

    isUsingPalmettoGallberry_ = surfaceInputs_->isUsingPalmettoGallberry();
    isUsingWesternAspen_ = surfaceInputs_->isUsingWesternAspen();

    depth_ = 0.0;
    relativePackingRatio_ = 0.0;
    fuelModelNumber_ = 0;
    liveFuelMois_ = 0.0;
    liveFuelMext_ = 0.0;
    sigma_ = 0.0;
    bulkDensity_ = 0.0;
    packingRatio_ = 0.0;
    heatSink_ = 0.0;
    totalSilicaContent_ = 0.0555;

    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_SAVR_SIZE_CLASSES; i++)
    {
        sizeSortedFractionOfSurfaceAreaDead_[i] = 0;
        sizeSortedFractionOfSurfaceAreaLive_[i] = 0;
    }
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_PARTICLES; i++)
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
    for (int i = 0; i < SurfaceInputs::FuelConstants::MAX_LIFE_STATES; i++)
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
    double fuelbedDepth = fuelModelSet_->getFuelbedDepth(fuelModelNumber_);
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
