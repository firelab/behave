#include "surfaceTwoFuelModels.h"

#include "newext.h"
#include "randfuel.h"
#include "randthread.h"
#include "surfaceFireSpread.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceInputs.h"

SurfaceTwoFuelModels::SurfaceTwoFuelModels(SurfaceInputs& surfaceInputs, SurfaceFireSpread& surfaceFireSpread)
{
    surfaceInputs_ = &surfaceInputs;
    surfaceFireSpread_ = &surfaceFireSpread;
}

bool SurfaceTwoFuelModels::getWindLimitExceeded() const
{
    return windLimitExceeded_;
}

double SurfaceTwoFuelModels::getReactionIntensity() const
{
    return reactionIntensity_;
}

double SurfaceTwoFuelModels::getSpreadRate() const
{
    return spreadRate_;
}

double SurfaceTwoFuelModels::getDirectionOfMaxSpread() const
{
    return directionOfMaxSpread_;
}

double SurfaceTwoFuelModels::getEffectiveWind() const
{
    return effectiveWind_;
}

double SurfaceTwoFuelModels::getFuelbedDepth() const
{
    return fuelbedDepth_;
}

double SurfaceTwoFuelModels::getHeatPerUnitArea() const
{
    return heatPerUnitArea_;
}

double SurfaceTwoFuelModels::getMidFlameWindSpeed() const
{
    return midFlameWindSpeed_;
}

double SurfaceTwoFuelModels::getWindSpeedLimit() const
{
    return windSpeedLimit_;
}

double SurfaceTwoFuelModels::WindAdjustmentFactor() const
{
    return windAdjustmentFactor_;
}

double SurfaceTwoFuelModels::getFireLineIntensity() const
{
    return fireLineIntensity_;
}

double SurfaceTwoFuelModels::getflameLength() const
{
    return flameLength_;
}

double SurfaceTwoFuelModels::getFireLengthToWidthRatio() const
{
    return fireLengthToWidthRatio_;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedWeighted
*
*  Dependent Variables (Outputs)
*       reactionIntensity_ (Btu/ft2/min)
*       spreadRate_ (ft/min)
*       directionOfMaxSpread_ (clockwise from upslope)
*       effectiveWind_ (mi/h)
*       windSpeedLimit_ (mi/h)
*       windLimitExceeded_ (flag)
*       heatPerUnitArea_ (Btu/ft2)
*       fireLineIntensity_ (Btu/ft/s)
*       fireFlameLength_ (ft)
*
*  Independent Variables (Inputs)
*       SurfaceInputs (object)
*/

void SurfaceTwoFuelModels::calculateWeightedSpreadRate(bool hasDirectionOfInterest, double directionOfInterest)
{   
    fuelModelNumber_[TwoFuelModels::FIRST] = surfaceInputs_->getFirstFuelModelNumber();
    fuelModelNumber_[TwoFuelModels::SECOND] = surfaceInputs_->getSecondFuelModelNumber();

    coverageForFuelModel_[TwoFuelModels::FIRST] = surfaceInputs_->getFirstFuelModelCoverage();
    coverageForFuelModel_[TwoFuelModels::SECOND] = 1 - coverageForFuelModel_[TwoFuelModels::FIRST];

    // Calculate fire outputs for each fuel model
    calculateFireOutputsForEachModel(hasDirectionOfInterest, directionOfInterest);
    
    //------------------------------------------------
    // Determine and store combined fuel model outputs
    //------------------------------------------------
    // Fire spread rate depends upon the weighting method...
    calculateSpreadRateBasedOnMethod();

    // The following assignments are based on Pat's rules:
    // If only 1 fuel is present (whether primary or secondary), use its values exclusively
    if (coverageForFuelModel_[TwoFuelModels::FIRST] > 0.999 || coverageForFuelModel_[TwoFuelModels::SECOND] > 0.999)
    {
        int i = (coverageForFuelModel_[TwoFuelModels::FIRST] > 0.999) ? 0 : 1;
        reactionIntensity_ = reactionIntensityForFuelModel_[i];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        directionOfMaxSpread_ = dirMaxSpreadForFuelModel_[i];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        windAdjustmentFactor_ = windAdjustmentFactorForFuelModel_[i];
        surfaceFireSpread_->setWindAdjustmentFactor(windAdjustmentFactor_);

        midFlameWindSpeed_ = midFlameWindSpeedForFuelModel_[i];
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        effectiveWind_ = effectiveWindSpeedForFuelModel_[i];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        windSpeedLimit_ = windSpeedLimitForFuelModel_[i];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        windLimitExceeded_ = windLimitExceededForFuelModel_[i];
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        fireLengthToWidthRatio_ = lengthToWidthRatioForFuelModel_[i];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        heatPerUnitArea_ = heatPerUnitAreaForFuelModel_[i];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        fireLineIntensity_ = firelineIntensityForFuelModel_[i];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        flameLength_ = flameLengthForFuelModel_[i];
        surfaceFireSpread_->setFlameLength(flameLength_);

        fuelbedDepth_ = fuelbedDepthForFuelModel_[i];
    }
    // Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
    else
    {
        // Reaction intensity is the maximum of the two models
        reactionIntensity_ = (reactionIntensityForFuelModel_[TwoFuelModels::FIRST] > 
            reactionIntensityForFuelModel_[TwoFuelModels::SECOND]) ?
            reactionIntensityForFuelModel_[TwoFuelModels::FIRST] : reactionIntensityForFuelModel_[TwoFuelModels::SECOND];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        // Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
        directionOfMaxSpread_ = dirMaxSpreadForFuelModel_[TwoFuelModels::FIRST];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        // Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
        windAdjustmentFactor_ = windAdjustmentFactorForFuelModel_[TwoFuelModels::FIRST]; // TODO: Incorporate Wind Adjustment Factor model in Behave
        //		surfaceFireSpread_->setWindAdjustmentFactor[windAdjustmentFactor_];

        // Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
        midFlameWindSpeed_ = midFlameWindSpeedForFuelModel_[TwoFuelModels::FIRST]; // TODO:  Incorporate Wind Speed at Midflame model in Behave
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        // Effective wind speed is for the FIRST (not necessarily dominant) fuel model
        effectiveWind_ = effectiveWindSpeedForFuelModel_[TwoFuelModels::FIRST];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        // Maximum reliable wind speed is the minimum of the two models
        windSpeedLimit_ = (windSpeedLimitForFuelModel_[TwoFuelModels::FIRST] < windSpeedLimitForFuelModel_[TwoFuelModels::SECOND]) ?
            windSpeedLimitForFuelModel_[TwoFuelModels::FIRST] : windSpeedLimitForFuelModel_[TwoFuelModels::SECOND];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        // If either wind limit is exceeded, set the flag
        windLimitExceeded_ = (windLimitExceededForFuelModel_[TwoFuelModels::FIRST] || windLimitExceededForFuelModel_[TwoFuelModels::SECOND]);
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        // Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
        fireLengthToWidthRatio_ = lengthToWidthRatioForFuelModel_[TwoFuelModels::FIRST];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        // Heat per unit area is the maximum of the two models
        heatPerUnitArea_ = (heatPerUnitAreaForFuelModel_[TwoFuelModels::FIRST] > heatPerUnitAreaForFuelModel_[TwoFuelModels::SECOND]) ?
            heatPerUnitAreaForFuelModel_[TwoFuelModels::FIRST] : heatPerUnitAreaForFuelModel_[TwoFuelModels::SECOND];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        // Fireline intensity is the maximum of the two models
        fireLineIntensity_ = (firelineIntensityForFuelModel_[TwoFuelModels::FIRST] > firelineIntensityForFuelModel_[TwoFuelModels::SECOND]) ?
            firelineIntensityForFuelModel_[TwoFuelModels::FIRST] : firelineIntensityForFuelModel_[1];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        // Flame length is the maximum of the two models
        flameLength_ = (flameLengthForFuelModel_[TwoFuelModels::FIRST] > flameLengthForFuelModel_[TwoFuelModels::SECOND]) ?
            flameLengthForFuelModel_[TwoFuelModels::FIRST] : flameLengthForFuelModel_[TwoFuelModels::SECOND];
        maxFlameLength_ = (maxFlameLengthForFuelModel_[TwoFuelModels::FIRST] > maxFlameLengthForFuelModel_[TwoFuelModels::SECOND]) ?
            maxFlameLengthForFuelModel_[TwoFuelModels::FIRST] : maxFlameLengthForFuelModel_[TwoFuelModels::SECOND];
        surfaceFireSpread_->setFlameLength(flameLength_);

        // Fuel bed depth is the maximum of the two fuel bed depths
        fuelbedDepth_ = (fuelbedDepthForFuelModel_[TwoFuelModels::FIRST] > fuelbedDepthForFuelModel_[TwoFuelModels::SECOND]) ?
            fuelbedDepthForFuelModel_[TwoFuelModels::FIRST] : fuelbedDepthForFuelModel_[TwoFuelModels::SECOND];
    }
    surfaceFireSpread_->forwardSpreadRate_ = spreadRate_;
}

double SurfaceTwoFuelModels::surfaceFireExpectedSpreadRate(double *ros, double *cov, int fuels,
    double lbRatio, int samples, int depth, int laterals)
{
    // Initialize results
    double expectedRos = 0.0;

    // Create a RandFuel instance
    RandFuel randFuel;

    // Mark says the cell size is irrelevant, but he sets it anyway.
    randFuel.setCellDimensions(10);

    // Get total fuel coverage
    double totalCov = 0.0;
    int i;
    for (i = 0; i < fuels; i++)
    {
        totalCov += cov[i];
    }
    // If no fuel coverage, we're done.
    if (totalCov <= 0.0)
    {
        return(expectedRos);
    }
    // Allocate the fuels
    if (!randFuel.allocFuels(fuels))
    {
        return(expectedRos);
    }
    // Normalize fuel coverages and store the fuel ros and cov
    for (i = 0; i < fuels; i++)
    {
        cov[i] = cov[i] / totalCov;
        randFuel.setFuelData(i, ros[i], cov[i]);
    }
    
    double maximumRos;
    double* harmonicRos; // only exists to match computeSpread2's parameter list
    harmonicRos = 0; // point harmonicRos to null
    // Compute the expected rate
    expectedRos = randFuel.computeSpread2(
        samples,        // columns
        depth,          // rows
        lbRatio,        // fire length-to-breadth ratio
        1,              // always use 1 thread
        &maximumRos,    // returned maximum spread rate
        harmonicRos,    // returned harmonic spread rate
        laterals,       // lateral extensions
        0);            // less ignitions
    randFuel.freeFuels();

    // Determine expected spread rates.
    expectedRos *= maximumRos;

    return(expectedRos);
}

void SurfaceTwoFuelModels::calculateFireOutputsForEachModel(bool hasDirectionOfInterest, double directionOfInterest)
{
    for (int i = 0; i < TwoFuelModels::NUMBER_OF_MODELS; i++)
    {
        surfaceInputs_->setFuelModelNumber(fuelModelNumber_[i]);
        fuelbedDepthForFuelModel_[i] = surfaceFireSpread_->getFuelbedDepth();

        rosForFuelModel_[i] = surfaceFireSpread_->calculateForwardSpreadRate(hasDirectionOfInterest, directionOfInterest);

        reactionIntensityForFuelModel_[i] = surfaceFireSpread_->getReactionIntensity();
        dirMaxSpreadForFuelModel_[i] = surfaceFireSpread_->getDirectionOfMaxSpread();
        midFlameWindSpeedForFuelModel_[i] = surfaceFireSpread_->getMidflameWindSpeed();
        windAdjustmentFactorForFuelModel_[i] = surfaceFireSpread_->getWindAdjustmentFactor();
        effectiveWindSpeedForFuelModel_[i] = surfaceFireSpread_->getEffectiveWindSpeed();
        windSpeedLimitForFuelModel_[i] = surfaceFireSpread_->getWindSpeedLimit();
        windLimitExceededForFuelModel_[i] = surfaceFireSpread_->getIsWindLimitExceeded();
        firelineIntensityForFuelModel_[i] = surfaceFireSpread_->getFirelineIntensity();
        maxFlameLengthForFuelModel_[i] = surfaceFireSpread_->getMaxFlameLength();
        flameLengthForFuelModel_[i] = surfaceFireSpread_->getFlameLength();
        lengthToWidthRatioForFuelModel_[i] = surfaceFireSpread_->getFireLengthToWidthRatio();
        heatPerUnitAreaForFuelModel_[i] = surfaceFireSpread_->getHeatPerUnitArea();
    }
    // Restore state of surfaceInputs
    surfaceInputs_->setFuelModelNumber(fuelModelNumber_[TwoFuelModels::FIRST]);
}

void SurfaceTwoFuelModels::calculateSpreadRateBasedOnMethod()
{
    // If area weighted spread rate ...
    TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod = surfaceInputs_->getTwoFuelModelsMethod();
    if (twoFuelModelsMethod == TwoFuelModels::ARITHMETIC)
    {
        spreadRate_ = (coverageForFuelModel_[TwoFuelModels::FIRST] * rosForFuelModel_[TwoFuelModels::FIRST]) +
            (coverageForFuelModel_[TwoFuelModels::SECOND] * rosForFuelModel_[TwoFuelModels::SECOND]);
    }
    // else if harmonic mean spread rate...
    else if (twoFuelModelsMethod == TwoFuelModels::HARMONIC)
    {
        if (rosForFuelModel_[TwoFuelModels::FIRST] > 0.000001 && rosForFuelModel_[TwoFuelModels::SECOND] > 0.000001)
        {
            spreadRate_ = 1.0 / ((coverageForFuelModel_[TwoFuelModels::FIRST] / rosForFuelModel_[TwoFuelModels::FIRST]) +
                (coverageForFuelModel_[TwoFuelModels::SECOND] / rosForFuelModel_[TwoFuelModels::SECOND]));
        }
    }
    // else if Finney's 2-dimensional spread rate...
    else if (twoFuelModelsMethod == TwoFuelModels::TWO_DIMENSIONAL)
    {
        //double lbRatio = lengthToWidthRatioForFuelModel_[TwoFuelModels::FIRST]; // get first fuel model's length-to-width ratio
        double lbRatio = lengthToWidthRatioForFuelModel_[TwoFuelModels::SECOND]; // using fuel model's length-to-width ratio seems to agree with BehavePlus
        int samples = 2; // from behavePlus.xml
        int depth = 2; // from behavePlus.xml
        int laterals = 0; // from behavePlus.xml
        spreadRate_ = surfaceFireExpectedSpreadRate(rosForFuelModel_, coverageForFuelModel_, TwoFuelModels::NUMBER_OF_MODELS, lbRatio,
            samples, depth, laterals);
    }
}
