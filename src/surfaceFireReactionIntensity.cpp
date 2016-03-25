#include <cmath>
#include "surfaceFireReactionIntensity.h"

SurfaceFireReactionIntensity::SurfaceFireReactionIntensity()
{

}

SurfaceFireReactionIntensity::SurfaceFireReactionIntensity(const SurfaceFireReactionIntensity &rhs)
{
    
    for (int i = 0; i < MAX_LIFE_STATES; i++)
    {
        etaM_[i] = rhs.etaM_[i];
        etaS_[i] = rhs.etaS_[i];
        reactionIntensityForLifeState_[i] = rhs.reactionIntensityForLifeState_[i];
    }
    reactionIntensity_ = rhs.reactionIntensity_;
}

SurfaceFireReactionIntensity::SurfaceFireReactionIntensity(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates)
{
    surfaceFuelbedIntermediates_ = &surfaceFuelbedIntermediates;
    for (int i = 0; i < MAX_LIFE_STATES; i++)
    {
        etaM_[i] = 0.0;
        etaS_[i] = 0.0;
        reactionIntensityForLifeState_[i] = 0.0;
    }
    reactionIntensity_ = 0.0;
}

SurfaceFireReactionIntensity& SurfaceFireReactionIntensity::operator= (const SurfaceFireReactionIntensity& rhs)
{
    if (this != &rhs)
    {
        for (int i = 0; i < MAX_LIFE_STATES; i++)
        {
            etaM_[i] = rhs.etaM_[i];
            etaS_[i] = rhs.etaS_[i];
            reactionIntensityForLifeState_[i] = rhs.reactionIntensityForLifeState_[i];
        }
        reactionIntensity_ = rhs.reactionIntensity_;
    }
    return *this;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the fire's propagating flux.
 *
 *  \param fuelBedPackingRatio Surface fuel bed packing ratio (beta) (ft3/ft3).
 *  \param fuelBedSavr         Surface fuel bed's characteristic surface area-
 *                             to-volume ratio (sigma) (ft2/ft3).
 *
 *  \return Fire's propagating flux (fraction).
 */
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

    double weightedFuelLoad[MAX_LIFE_STATES];
    weightedFuelLoad[DEAD] = surfaceFuelbedIntermediates_->getWeightedFuelLoadByLifeState(DEAD);
    weightedFuelLoad[LIVE] = surfaceFuelbedIntermediates_->getWeightedFuelLoadByLifeState(LIVE);

    double weightedHeat[MAX_LIFE_STATES];
    weightedHeat[DEAD] = surfaceFuelbedIntermediates_->getWeightedHeatByLifeState(DEAD);
    weightedHeat[LIVE] = surfaceFuelbedIntermediates_->getWeightedHeatByLifeState(LIVE);

    calculateEtaM();
    calculateEtaS();

    for (int i = 0; i < MAX_LIFE_STATES; i++)
    {
        reactionIntensityForLifeState_[i] = gamma * weightedFuelLoad[i] * weightedHeat[i] * etaM_[i] * etaS_[i];
    }
    reactionIntensity_ = reactionIntensityForLifeState_[DEAD] + reactionIntensityForLifeState_[LIVE];

    return reactionIntensity_;
}

void SurfaceFireReactionIntensity::calculateEtaM()
{
    double relativeMoisture = 0;	// (Moisture content) / (Moisture of extinction)

    double weightedMoisture[MAX_LIFE_STATES];
    weightedMoisture[DEAD] = surfaceFuelbedIntermediates_->getWeightedMoistureByLifeState(DEAD);
    weightedMoisture[LIVE] = surfaceFuelbedIntermediates_->getWeightedMoistureByLifeState(LIVE);

    double moistureOfExtinction[MAX_LIFE_STATES];
    moistureOfExtinction[DEAD] = surfaceFuelbedIntermediates_->getMoistureOfExtinctionByLifeState(DEAD);
    moistureOfExtinction[LIVE] = surfaceFuelbedIntermediates_->getMoistureOfExtinctionByLifeState(LIVE);

    for (int i = 0; i < MAX_LIFE_STATES; i++)
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
    double weightedSilica[MAX_LIFE_STATES];
    weightedSilica[DEAD] = surfaceFuelbedIntermediates_->getWeightedSilicaByLifeState(DEAD);
    weightedSilica[LIVE] = surfaceFuelbedIntermediates_->getWeightedSilicaByLifeState(LIVE);

    double etaSDenomitator = 0;
    for (int i = 0; i < MAX_LIFE_STATES; i++)
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

double SurfaceFireReactionIntensity::getReactionIntensity() const
{
    return reactionIntensity_;
}
