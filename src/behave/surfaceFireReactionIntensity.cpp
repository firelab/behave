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
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
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
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
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

    double weightedFuelLoad[FuelConstants::MAX_LIFE_STATES];
    weightedFuelLoad[FuelConstants::DEAD] = surfaceFuelbedIntermediates_->getWeightedFuelLoadByLifeState(FuelConstants::DEAD);
    weightedFuelLoad[FuelConstants::LIVE] = surfaceFuelbedIntermediates_->getWeightedFuelLoadByLifeState(FuelConstants::LIVE);

    double weightedHeat[FuelConstants::MAX_LIFE_STATES];
    weightedHeat[FuelConstants::DEAD] = surfaceFuelbedIntermediates_->getWeightedHeatByLifeState(FuelConstants::DEAD);
    weightedHeat[FuelConstants::LIVE] = surfaceFuelbedIntermediates_->getWeightedHeatByLifeState(FuelConstants::LIVE);

    calculateEtaM();
    calculateEtaS();

    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
    {
        reactionIntensityForLifeState_[i] = gamma * weightedFuelLoad[i] * weightedHeat[i] * etaM_[i] * etaS_[i];
    }
    reactionIntensity_ = reactionIntensityForLifeState_[FuelConstants::DEAD] + reactionIntensityForLifeState_[FuelConstants::LIVE];

    return reactionIntensity_;
}

void SurfaceFireReactionIntensity::calculateEtaM()
{
    double relativeMoisture = 0;	// (Moisture content) / (Moisture of extinction)

    double weightedMoisture[FuelConstants::MAX_LIFE_STATES];
    weightedMoisture[FuelConstants::DEAD] = surfaceFuelbedIntermediates_->getWeightedMoistureByLifeState(FuelConstants::DEAD);
    weightedMoisture[FuelConstants::LIVE] = surfaceFuelbedIntermediates_->getWeightedMoistureByLifeState(FuelConstants::LIVE);

    double moistureOfExtinction[FuelConstants::MAX_LIFE_STATES];
    moistureOfExtinction[FuelConstants::DEAD] = surfaceFuelbedIntermediates_->getMoistureOfExtinctionByLifeState(FuelConstants::DEAD);
    moistureOfExtinction[FuelConstants::LIVE] = surfaceFuelbedIntermediates_->getMoistureOfExtinctionByLifeState(FuelConstants::LIVE);

    MoE_Dead_ = moistureOfExtinction[FuelConstants::DEAD];
    MoE_Live_ = moistureOfExtinction[FuelConstants::LIVE];
    weightedMoistureDead_ = weightedMoisture[FuelConstants::DEAD];
    weightedMoistureLive_ = weightedMoisture[FuelConstants::LIVE];

    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
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
    double weightedSilica[FuelConstants::MAX_LIFE_STATES];
    weightedSilica[FuelConstants::DEAD] = surfaceFuelbedIntermediates_->getWeightedSilicaByLifeState(FuelConstants::DEAD);
    weightedSilica[FuelConstants::LIVE] = surfaceFuelbedIntermediates_->getWeightedSilicaByLifeState(FuelConstants::LIVE);

    double etaSDenomitator = 0;
    for (int i = 0; i < FuelConstants::MAX_LIFE_STATES; i++)
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

/*  These gets were added by Thomas DeVera for SIG research puproses June 24, 2022
 *
*/
double SurfaceFireReactionIntensity::getetaMDead() const
{
    return  etaM_[FuelConstants::DEAD];
}

double SurfaceFireReactionIntensity::getetaMLive() const
{
    return  etaM_[FuelConstants::LIVE];
}

double SurfaceFireReactionIntensity::getWeightedMoistureDead() const
{
    return weightedMoistureDead_;
};

double SurfaceFireReactionIntensity::getWeightedMoistureLive() const
{
    return weightedMoistureLive_;
};

double SurfaceFireReactionIntensity::getMoistureExtinctionDead() const
{
    return MoE_Dead_;
};

double SurfaceFireReactionIntensity::getMoistureExtinctionLive() const
{
    return MoE_Live_;
};