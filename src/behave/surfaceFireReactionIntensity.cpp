/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating surface fire reaction intensity
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

#include "surfaceFireReactionIntensity.h"

#include <cmath>
#include "surfaceFuelbedIntermediates.h"

SurfaceFireReactionIntensity::SurfaceFireReactionIntensity()
{

}

SurfaceFireReactionIntensity::SurfaceFireReactionIntensity(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates)
{
    surfaceFuelbedIntermediates_ = &surfaceFuelbedIntermediates;
    for (int i = 0; i < FuelConstants::MaxLifeStates; i++)
    {
        etaM_[i] = 0.0;
        etaS_[i] = 0.0;
        reactionIntensityForLifeState_[i] = 0.0;
    }
    reactionIntensity_ = 0.0;
}

SurfaceFireReactionIntensity::SurfaceFireReactionIntensity(const SurfaceFireReactionIntensity& rhs)
{
    memberwiseCopyAssignment(rhs);
}

SurfaceFireReactionIntensity& SurfaceFireReactionIntensity::operator=(const SurfaceFireReactionIntensity& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void SurfaceFireReactionIntensity::memberwiseCopyAssignment(const SurfaceFireReactionIntensity& rhs)
{
    for (int i = 0; i < FuelConstants::MaxLifeStates; i++)
    {
        etaM_[i] = rhs.etaM_[i];
        etaS_[i] = rhs.etaS_[i];
        reactionIntensityForLifeState_[i] = rhs.reactionIntensityForLifeState_[i];
    }
    reactionIntensity_ = rhs.reactionIntensity_;
}

double SurfaceFireReactionIntensity::calculateReactionIntensity()
{
    double aa = 0.0; // Alternate "arbitrary variable" A value for Rothermel equations for use in computer models, Albini 1976, p. 88
    reactionIntensity_ = 0;  // Reaction Intensity, Rothermel 1972, equation 27

    double sigma = surfaceFuelbedIntermediates_->getSigma();
    double relativePackingRatio = surfaceFuelbedIntermediates_->getRelativePackingRatio();

    aa = 133.0 / pow(sigma, 0.7913);

    //double gammaMax = (sigma * sqrt(sigma)) / (495.0 + (.0594 * sigma * sqrt(sigma)));
    double sigmaToTheOnePointFive = pow(sigma, 1.5);
    double gammaMax = sigmaToTheOnePointFive / (495.0 + (0.0594 * sigmaToTheOnePointFive));
    double gamma = gammaMax * pow(relativePackingRatio, aa) * exp(aa * (1.0 - relativePackingRatio));

    double weightedFuelLoad[FuelConstants::MaxLifeStates];
    weightedFuelLoad[FuelLifeState::Dead] = surfaceFuelbedIntermediates_->getWeightedFuelLoadByLifeState(FuelLifeState::Dead);
    weightedFuelLoad[FuelLifeState::Live] = surfaceFuelbedIntermediates_->getWeightedFuelLoadByLifeState(FuelLifeState::Live);

    double weightedHeat[FuelConstants::MaxLifeStates];
    weightedHeat[FuelLifeState::Dead] = surfaceFuelbedIntermediates_->getWeightedHeatByLifeState(FuelLifeState::Dead);
    weightedHeat[FuelLifeState::Live] = surfaceFuelbedIntermediates_->getWeightedHeatByLifeState(FuelLifeState::Live);

    calculateEtaM();
    calculateEtaS();

    for (int i = 0; i < FuelConstants::MaxLifeStates; i++)
    {
        reactionIntensityForLifeState_[i] = gamma * weightedFuelLoad[i] * weightedHeat[i] * etaM_[i] * etaS_[i];
    }
    reactionIntensity_ = reactionIntensityForLifeState_[FuelLifeState::Dead] + reactionIntensityForLifeState_[FuelLifeState::Live];

    return reactionIntensity_;
}

void SurfaceFireReactionIntensity::calculateEtaM()
{
    double relativeMoisture = 0;	// (Moisture content) / (Moisture of extinction)

    double weightedMoisture[FuelConstants::MaxLifeStates];
    weightedMoisture[FuelLifeState::Dead] = surfaceFuelbedIntermediates_->getWeightedMoistureByLifeState(FuelLifeState::Dead);
    weightedMoisture[FuelLifeState::Live] = surfaceFuelbedIntermediates_->getWeightedMoistureByLifeState(FuelLifeState::Live);

    double moistureOfExtinction[FuelConstants::MaxLifeStates];
    moistureOfExtinction[FuelLifeState::Dead] = surfaceFuelbedIntermediates_->getMoistureOfExtinctionByLifeState(FuelLifeState::Dead);
    moistureOfExtinction[FuelLifeState::Live] = surfaceFuelbedIntermediates_->getMoistureOfExtinctionByLifeState(FuelLifeState::Live);

    for (int i = 0; i < FuelConstants::MaxLifeStates; i++)
    {
        if (moistureOfExtinction[i] > 0.0)
        {
            relativeMoisture = weightedMoisture[i] / moistureOfExtinction[i];
        }
        if (weightedMoisture[i] >= moistureOfExtinction[i] || relativeMoisture > 1.0)
        {
            etaM_[i] = 0;
        }
        else
        {
            etaM_[i] = 1.0 - (2.59 * relativeMoisture) + (5.11 * relativeMoisture * relativeMoisture) -
                (3.52 * relativeMoisture * relativeMoisture * relativeMoisture);
        }
    }
}

void SurfaceFireReactionIntensity::calculateEtaS()
{
    double weightedSilica[FuelConstants::MaxLifeStates];
    weightedSilica[FuelLifeState::Dead] = surfaceFuelbedIntermediates_->getWeightedSilicaByLifeState(FuelLifeState::Dead);
    weightedSilica[FuelLifeState::Live] = surfaceFuelbedIntermediates_->getWeightedSilicaByLifeState(FuelLifeState::Live);

    double etaSDenomitator = 0;
    for (int i = 0; i < FuelConstants::MaxLifeStates; i++)
    {
        etaSDenomitator = pow(weightedSilica[i], 0.19);
        if (etaSDenomitator < 1e-6)
        {
            etaS_[i] = 0;
        }
        else
        {
            etaS_[i] = 0.174 / etaSDenomitator; // 0.174 / pow(weightedSilica[i], 0.19)
        }
        if (etaS_[i] > 1.0)
        {
            etaS_[i] = 1.0;
        }
    }
}

double SurfaceFireReactionIntensity::getReactionIntensity(HeatSourceAndReactionIntensityUnits::HeatSourceAndReactionIntensityUnitsEnum reactiontionIntensityUnits) const
{
    return reactionIntensity_;
}

double SurfaceFireReactionIntensity::getSurfaceFireReactionIntensityForLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
  return  reactionIntensityForLifeState_[lifeState];
}
