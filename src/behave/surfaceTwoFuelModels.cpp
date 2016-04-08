#include "surfaceTwoFuelModels.h"

SurfaceTwoFuelModels::SurfaceTwoFuelModels(SurfaceInputs& surfaceInputs, SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates,
    SurfaceFireSpread& surfaceFireSpread)
{
    surfaceInputs_ = &surfaceInputs;
    surfaceFuelbedIntermediates_ = &surfaceFuelbedIntermediates;
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

double SurfaceTwoFuelModels::getFireFlameLength() const
{
    return fireFlameLength_;
}

double SurfaceTwoFuelModels::getFireLengthToWidthRatio() const
{
    return fireLengthToWidthRatio_;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedWeighted
*
*  Dependent Variables (Outputs)
*      SurfaceReactionIntensity (Btu/ft2/min)
*      SurfaceFireSpreadRate (ft/min)
*      SurfaceFireMaxDirFromUpslope (clockwise from upslope)
*      SurfaceFireEffectiveWind (mi/h)
*      SurfaceFireWindSpeedLimit (mi/h)
*      SurfaceFireWindSpeedFlag (flag)
*      SurfaceFireHeatPerUnitArea (Btu/ft2)
*      SurfaceFireLineIntensity (Btu/ft/s)
*      SurfaceFireFlameLength (ft)
*
*  Independent Variables (Inputs)
*      SurfaceFuelBedModel1 (item)
*      SurfaceFuelBedModel2 (item)
*      SurfaceFuelBedCoverage1 (fraction)
*      SiteSlopeFraction (rise/reach)
*      WindDirFromUpslope (degrees)
*      WindSpeedAtMidflame (mi/h)
*      SurfaceFuelMoisDead1 (fraction)
*      SurfaceFuelMoisDead10 (fraction)
*      SurfaceFuelMoisDead100 (fraction)
*      SurfaceFuelMoisDead1000 (fraction)
*      SurfaceFuelMoisLiveHerb (fraction)
*      SurfaceFuelMoisLiveWood (fraction)
*      SurfaceFuelLoadTransferFraction (fraction)
*      SurfaceFireVectorDirFromUpslope (deg)
*/

double SurfaceTwoFuelModels::calculateWeightedSpreadRate(double directionOfInterest)
{   
    fuelModelNumber_[TwoFuelModelsMethod::FIRST] = surfaceInputs_->getFirstFuelModelNumber();
    fuelModelNumber_[TwoFuelModelsMethod::SECOND] = surfaceInputs_->getSecondFuelModelNumber();

    coverageForFuelModel_[TwoFuelModelsMethod::FIRST] = surfaceInputs_->getFirstFuelModelCoverage();
    coverageForFuelModel_[TwoFuelModelsMethod::SECOND] = 1 - coverageForFuelModel_[TwoFuelModelsMethod::FIRST];

    // Calculate fire outputs for each fuel model
    calculateFireOutputsForEachModel(directionOfInterest);
    
    //------------------------------------------------
    // Determine and store combined fuel model outputs
    //------------------------------------------------
    // Fire spread rate depends upon the weighting method...
    calculateSpreadRateBasedOnMethod();

    // The following assignments are based on Pat's rules:
    // If only 1 fuel is present (whether primary or secondary), use its values exclusively
    if (coverageForFuelModel_[TwoFuelModelsMethod::FIRST] > 0.999 || coverageForFuelModel_[TwoFuelModelsMethod::SECOND] > 0.999)
    {
        int i = (coverageForFuelModel_[TwoFuelModelsMethod::FIRST] > 0.999) ? 0 : 1;
        reactionIntensity_ = reactionIntensityForFuelModel_[i];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        directionOfMaxSpread_ = dirMaxSpreadForFuelModel_[i];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        windAdjustmentFactor_ = windAdjustmentFactorForFuelModel_[i]; // TODO: Incorporate Wind Adjustment Factor model in Behave
        surfaceFireSpread_->setWindAdjustmentFactor(windAdjustmentFactor_);

        midFlameWindSpeed_ = midFlameWindSpeedForFuelModel_[i]; // TODO:  Incorporate Wind Speed at Midflame model in Behave
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

        fireFlameLength_ = flameLengthForFuelModel_[i];
        surfaceFireSpread_->setFlameLength(fireFlameLength_);

        fuelbedDepth_ = fuelbedDepthForFuelModel_[i];

    }
    // Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
    else
    {
        // Reaction intensity is the maximum of the two models
        reactionIntensity_ = (reactionIntensityForFuelModel_[TwoFuelModelsMethod::FIRST] > reactionIntensityForFuelModel_[TwoFuelModelsMethod::SECOND]) ?
            reactionIntensityForFuelModel_[TwoFuelModelsMethod::FIRST] : reactionIntensityForFuelModel_[TwoFuelModelsMethod::SECOND];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        // Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
        directionOfMaxSpread_ = dirMaxSpreadForFuelModel_[TwoFuelModelsMethod::FIRST];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        // Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
        windAdjustmentFactor_ = windAdjustmentFactorForFuelModel_[TwoFuelModelsMethod::FIRST]; // TODO: Incorporate Wind Adjustment Factor model in Behave
        //		surfaceFireSpread_->setWindAdjustmentFactor[windAdjustmentFactor_];

        // Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
        midFlameWindSpeed_ = midFlameWindSpeedForFuelModel_[TwoFuelModelsMethod::FIRST]; // TODO:  Incorporate Wind Speed at Midflame model in Behave
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        // Effective wind speed is for the FIRST (not necessarily dominant) fuel model
        effectiveWind_ = effectiveWindSpeedForFuelModel_[TwoFuelModelsMethod::FIRST];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        // Maximum reliable wind speed is the minimum of the two models
        windSpeedLimit_ = (windSpeedLimitForFuelModel_[TwoFuelModelsMethod::FIRST] < windSpeedLimitForFuelModel_[TwoFuelModelsMethod::SECOND]) ?
            windSpeedLimitForFuelModel_[TwoFuelModelsMethod::FIRST] : windSpeedLimitForFuelModel_[TwoFuelModelsMethod::SECOND];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        // If either wind limit is exceeded, set the flag
        windLimitExceeded_ = (windLimitExceededForFuelModel_[TwoFuelModelsMethod::FIRST] || windLimitExceededForFuelModel_[TwoFuelModelsMethod::SECOND]);
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        // Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
        fireLengthToWidthRatio_ = lengthToWidthRatioForFuelModel_[TwoFuelModelsMethod::FIRST];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        // Heat per unit area is the maximum of the two models
        heatPerUnitArea_ = (heatPerUnitAreaForFuelModel_[TwoFuelModelsMethod::FIRST] > heatPerUnitAreaForFuelModel_[TwoFuelModelsMethod::SECOND]) ?
            heatPerUnitAreaForFuelModel_[TwoFuelModelsMethod::FIRST] : heatPerUnitAreaForFuelModel_[TwoFuelModelsMethod::SECOND];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        // Fireline intensity is the maximum of the two models
        fireLineIntensity_ = (firelineIntensityForFuelModel_[TwoFuelModelsMethod::FIRST] > firelineIntensityForFuelModel_[TwoFuelModelsMethod::SECOND]) ?
            firelineIntensityForFuelModel_[TwoFuelModelsMethod::FIRST] : firelineIntensityForFuelModel_[1];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        // Flame length is the maximum of the two models
        fireFlameLength_ = (flameLengthForFuelModel_[TwoFuelModelsMethod::FIRST] > flameLengthForFuelModel_[TwoFuelModelsMethod::SECOND]) ?
            flameLengthForFuelModel_[TwoFuelModelsMethod::FIRST] : flameLengthForFuelModel_[TwoFuelModelsMethod::SECOND];
        surfaceFireSpread_->setFlameLength(fireFlameLength_);

        // Fuel bed depth is the maximum of the two fuel bed depths
        fuelbedDepth_ = (fuelbedDepthForFuelModel_[TwoFuelModelsMethod::FIRST] > fuelbedDepthForFuelModel_[TwoFuelModelsMethod::SECOND]) ?
            fuelbedDepthForFuelModel_[TwoFuelModelsMethod::FIRST] : fuelbedDepthForFuelModel_[TwoFuelModelsMethod::SECOND];
    }

    return spreadRate_;
}

double SurfaceTwoFuelModels::surfaceFireExpectedSpreadRate(double *ros, double *cov, int fuels,
    double lbRatio, int samples, int depth, int laterals, double *harmonicRos)
{
    // Initialize results
    double expectedRos = 0.0;
    if (harmonicRos)
    {
        *harmonicRos = 0.0;
    }

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
    // Compute the expected and harmonic spread rates
    double maximumRos;
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

    // Determine expected and harmonic spread rates.
    expectedRos *= maximumRos;
    if (harmonicRos)
    {
        *harmonicRos *= maximumRos;
    }
    return(expectedRos);
}

void SurfaceTwoFuelModels::calculateFireOutputsForEachModel(double directionOfInterest)
{
    for (int i = 0; i < TwoFuelModelsMethod::NUMBER_OF_MODELS; i++)
    {
        surfaceInputs_->setFuelModelNumber(fuelModelNumber_[i]);
        surfaceFuelbedIntermediates_->calculateFuelbedIntermediates();
        fuelbedDepthForFuelModel_[i] = surfaceFuelbedIntermediates_->getFuelbedDepth();

        rosForFuelModel_[i] = surfaceFireSpread_->calculateForwardSpreadRate(directionOfInterest);

        reactionIntensityForFuelModel_[i] = surfaceFireSpread_->getReactionIntensity();
        dirMaxSpreadForFuelModel_[i] = surfaceFireSpread_->getDirectionOfMaxSpread();
        midFlameWindSpeedForFuelModel_[i] = surfaceFireSpread_->getMidflameWindSpeed();
        effectiveWindSpeedForFuelModel_[i] = surfaceFireSpread_->getEffectiveWindSpeed();
        windSpeedLimitForFuelModel_[i] = surfaceFireSpread_->getWindSpeedLimit();
        windLimitExceededForFuelModel_[i] = surfaceFireSpread_->getIsWindLimitExceeded();

        firelineIntensityForFuelModel_[i] = surfaceFireSpread_->getFirelineIntensity();
        flameLengthForFuelModel_[i] = surfaceFireSpread_->getFlameLength();
        lengthToWidthRatioForFuelModel_[i] = surfaceFireSpread_->getFireLengthToWidthRatio();
        heatPerUnitAreaForFuelModel_[i] = surfaceFireSpread_->getHeatPerUnitArea();
    }
}

void SurfaceTwoFuelModels::calculateSpreadRateBasedOnMethod()
{
    // If area weighted spread rate ...
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod = surfaceInputs_->getTwoFuelModelsMethod();
    if (twoFuelModelsMethod == TwoFuelModelsMethod::ARITHMETIC)
    {
        spreadRate_ = (coverageForFuelModel_[TwoFuelModelsMethod::FIRST] * rosForFuelModel_[TwoFuelModelsMethod::FIRST]) +
            (coverageForFuelModel_[TwoFuelModelsMethod::SECOND] * rosForFuelModel_[TwoFuelModelsMethod::SECOND]);
    }
    // else if harmonic mean spread rate...
    else if (twoFuelModelsMethod == TwoFuelModelsMethod::HARMONIC)
    {
        if (rosForFuelModel_[TwoFuelModelsMethod::FIRST] > 0.000001 && rosForFuelModel_[TwoFuelModelsMethod::SECOND] > 0.000001)
        {
            spreadRate_ = 1.0 / ((coverageForFuelModel_[TwoFuelModelsMethod::FIRST] / rosForFuelModel_[TwoFuelModelsMethod::FIRST]) +
                (coverageForFuelModel_[TwoFuelModelsMethod::SECOND] / rosForFuelModel_[TwoFuelModelsMethod::SECOND]));
        }
    }
    // else if Finney's 2-dimensional spread rate...
    else if (twoFuelModelsMethod == TwoFuelModelsMethod::TWO_DIMENSIONAL)
    {
        //double lbRatio = lengthToWidthRatio[0]; // get first fuel model's length-to-width ratio
        double lbRatio = lengthToWidthRatioForFuelModel_[TwoFuelModelsMethod::SECOND]; // get second fuel model's length-to-width ratio, seems to agree with BehavePlus
        int samples = 2; // from behavePlus.xml
        int depth = 2; // from behavePlus.xml
        int laterals = 0; // from behavePlus.xml
        spreadRate_ = surfaceFireExpectedSpreadRate(rosForFuelModel_, coverageForFuelModel_, TwoFuelModelsMethod::NUMBER_OF_MODELS, lbRatio,
            samples, depth, laterals);
    }
}
