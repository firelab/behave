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

double SurfaceTwoFuelModels::FuelBedWeighted(double directionOfInterest)
{
    /*
        // Get the primary and secondary fuel models
        FuelModel *fm[2];
        fm[0] = currentFuelModel(1);
        fm[1] = currentFuelModel(2);

        // Get the primary and secondary fuel model coverages
        double cov[2];
        cov[0] = vSurfaceFuelBedCoverage1->m_nativeValue;
        cov[1] = 1. - cov[0];

        //----------------------------------------
        // Determine individual fuel model outputs
        //----------------------------------------

        // Intermediate outputs for each fuel model
        double ros[2];					// rate of spread
        double fli[2];					// fireline intensity
        double fl[2];					// flame length
        double ewsh[2];					// effective wind speed
        double flw[2];					// fire length-to-width ratio
        double rxi[2], hua[2], mxd[2];	// reaction intensity, heat per unit area, dir of max spread
        double waf[2], wmf[2];			// wind adjustment factor and wind speed at midflame
        double wsl[2];					// wind speed limit
        int    wsf[2];					// wind speed flag
        */

    // For all arrays below: 
    // index 0 = first fuel model, index 1 = second fuel model
    int		fuelModelNumber[2];			// fuel model number
    double	coverage[2];				// coverage
    double	ros[2];						// rate of spread
    double	firelineIntensity[2];		// fireline intensity
    double	flameLength[2];				// flame length
    double	fuelbedDepth[2];
    double	effectiveWindSpeed[2];		// effective wind speed
    double	lengthToWidthRatio[2];		// fire length-to-width ratio
    double	reactionIntensity[2];		// reaction intensity, 
    double	heatPerUnitArea[2];			// heat per unit area
    double	dirMaxSpread[2];			// direction of max spread
    double	windAdjustmentFactor[2];	// wind adjustment factor
    double	midFlameWindSpeed[2];		// wind speed at midflame
    double	windSpeedLimit[2];			// wind speed limit
    bool	windLimitExceeded[2];		// wind speed exceeded flag

    fuelModelNumber[0] = surfaceInputs_->getFirstFuelModelNumber();
    fuelModelNumber[1] = surfaceInputs_->getSecondFuelModelNumber();

    coverage[0] = surfaceInputs_->getFirstFuelModelCoverage();
    coverage[1] = 1 - coverage[0];

    // Calculate fire outputs for each fuel model
    for (int i = 0; i < 2; i++)
    {
        surfaceInputs_->setFuelModelNumber(fuelModelNumber[i]);
        surfaceFuelbedIntermediates_->calculateFuelbedIntermediates();
        fuelbedDepth[i] = surfaceFuelbedIntermediates_->getFuelbedDepth();

        ros[i] = surfaceFireSpread_->calculateForwardSpreadRate(directionOfInterest);

        reactionIntensity[i] = surfaceFireSpread_->getReactionIntensity();
        dirMaxSpread[i] = surfaceFireSpread_->getDirectionOfMaxSpread();
        midFlameWindSpeed[i] = surfaceFireSpread_->getMidflameWindSpeed();
        effectiveWindSpeed[i] = surfaceFireSpread_->getEffectiveWindSpeed();
        windSpeedLimit[i] = surfaceFireSpread_->getWindSpeedLimit();
        windLimitExceeded[i] = surfaceFireSpread_->getIsWindLimitExceeded();

        firelineIntensity[i] = surfaceFireSpread_->getFirelineIntensity();
        flameLength[i] = surfaceFireSpread_->getFlameLength();
        lengthToWidthRatio[i] = surfaceFireSpread_->getFireLengthToWidthRatio();
        heatPerUnitArea[i] = surfaceFireSpread_->getHeatPerUnitArea();
    }
    /*
    //------------------------------------------------
    // Determine and store combined fuel model outputs
    //------------------------------------------------

    // Fire spread rate depends upon the weighting method...
    // If area weighted spread rate ... */
    int method = surfaceInputs_->getTwoFuelModelsMethod();
    if (method == ARITHMETIC)
    {
        spreadRate_ = (coverage[0] * ros[0]) + (coverage[1] * ros[1]);
    }
    // else if harmonic mean spread rate...
    else if (method == HARMONIC)
    {
        if (ros[0] > 0.000001 && ros[1] > 0.000001)
        {
            spreadRate_ = 1.0 / ((coverage[0] / ros[0]) + (coverage[1] / ros[1]));
        }
    }
    // else if Finney's 2-dimensional spread rate...
    else if (method == TWO_DIMENSIONAL)
    {
        //double lbRatio = lengthToWidthRatio[0]; // get first fuel model's length-to-width ratio
        double lbRatio = lengthToWidthRatio[1]; // get second? fuel model's length-to-width ratio, seems to agree with BehavePlus
        //double lbRatio = 0;
        //if (lengthToWidthRatio[0] <= lengthToWidthRatio[1])
        //{
        //	lbRatio = lengthToWidthRatio[0]; // lbRatio is the minimum of two fuel models
        //}
        //else
        //{
        //	lbRatio = lengthToWidthRatio[1];  // lbRatio is the minimum of two fuel models
        //}
        int samples = 2; // from behavePlus.xml
        int depth = 2; // from behavePlus.xml
        int laterals = 0; // from behavePlus.xml
        spreadRate_ = FBL_SurfaceFireExpectedSpreadRate(ros, coverage, 2, lbRatio,
            samples, depth, laterals);
    }

    // The following assignments are based on Pat's rules:
    // If only 1 fuel is present (whether primary or secondary), use its values exclusively
    if (coverage[0] > 0.999 || coverage[1] > 0.999)
    {
        int i = (coverage[0] > 0.999) ? 0 : 1;
        reactionIntensity_ = reactionIntensity[i];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        directionOfMaxSpread_ = dirMaxSpread[i];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        windAdjustmentFactor_ = windAdjustmentFactor[i]; // TODO: Incorporate Wind Adjustment Factor model in Behave
        surfaceFireSpread_->setWindAdjustmentFactor(windAdjustmentFactor_);

        midFlameWindSpeed_ = midFlameWindSpeed[i]; // TODO:  Incorporate Wind Speed at Midflame model in Behave
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        effectiveWind_ = effectiveWindSpeed[i];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        windSpeedLimit_ = windSpeedLimit[i];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        windLimitExceeded_ = windLimitExceeded[i];
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        fireLengthToWidthRatio_ = lengthToWidthRatio[i];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        heatPerUnitArea_ = heatPerUnitArea[i];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        fireLineIntensity_ = firelineIntensity[i];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        fireFlameLength_ = flameLength[i];
        surfaceFireSpread_->setFlameLength(fireFlameLength_);

        fuelbedDepth_ = fuelbedDepth[i];

    }
    // Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
    else
    {
        // Reaction intensity is the maximum of the two models
        reactionIntensity_ = (reactionIntensity[0] > reactionIntensity[1]) ? reactionIntensity[0] : reactionIntensity[1];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        // Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
        directionOfMaxSpread_ = dirMaxSpread[0];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        // Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
        windAdjustmentFactor_ = windAdjustmentFactor[0]; // TODO: Incorporate Wind Adjustment Factor model in Behave
        //		surfaceFireSpread_->setWindAdjustmentFactor[windAdjustmentFactor_];

        // Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
        midFlameWindSpeed_ = midFlameWindSpeed[0]; // TODO:  Incorporate Wind Speed at Midflame model in Behave
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        // Effective wind speed is for the FIRST (not necessarily dominant) fuel model
        effectiveWind_ = effectiveWindSpeed[0];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        // Maximum reliable wind speed is the minimum of the two models
        windSpeedLimit_ = (windSpeedLimit[0] < windSpeedLimit[1]) ? windSpeedLimit[0] : windSpeedLimit[1];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        // If either wind limit is exceeded, set the flag
        windLimitExceeded_ = (windLimitExceeded[0] || windLimitExceeded[1]);
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        // Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
        fireLengthToWidthRatio_ = lengthToWidthRatio[0];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        // Heat per unit area is the maximum of the two models
        heatPerUnitArea_ = (heatPerUnitArea[0] > heatPerUnitArea[1]) ? heatPerUnitArea[0] : heatPerUnitArea[1];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        // Fireline intensity is the maximum of the two models
        fireLineIntensity_ = (firelineIntensity[0] > firelineIntensity[1]) ? firelineIntensity[0] : firelineIntensity[1];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        // Flame length is the maximum of the two models
        fireFlameLength_ = (flameLength[0] > flameLength[1]) ? flameLength[0] : flameLength[1];
        surfaceFireSpread_->setFlameLength(fireFlameLength_);

        // Fuel bed depth is the maximum of the two fuel bed depths
        fuelbedDepth_ = (fuelbedDepth[0] > fuelbedDepth[1]) ? fuelbedDepth[0] : fuelbedDepth[1];
    }

    return spreadRate_;
}

double SurfaceTwoFuelModels::FBL_SurfaceFireExpectedSpreadRate(double *ros, double *cov, int fuels,
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