#include "surfaceTwoFuelModels.h"

SurfaceTwoFuelModels::SurfaceTwoFuelModels(SurfaceInputs& surfaceInputs, SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates, 
	SurfaceFireSpread& surfaceFireSpread)
{
	surfaceInputs_ = &surfaceInputs;
	surfaceFuelbedIntermediates_ = &surfaceFuelbedIntermediates;
	surfaceFireSpread_ = &surfaceFireSpread;
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

	coverage[0] = surfaceInputs_->getCoverage();
	coverage[0] /= 100;
	coverage[1] = 1 - coverage[0];


	// Calculate fire outputs for each fuel model
	for (int i = 0; i < 2; i++)
	{
		surfaceInputs_->setFuelModelNumber(fuelModelNumber[i]);
		surfaceFuelbedIntermediates_->calculateFuelbedIntermediates();
		fuelbedDepth[i] = surfaceFuelbedIntermediates_->getFuelbedDepth();
		midFlameWindSpeed[i] = surfaceInputs_->getMidflameWindSpeed();
		/*
		// If necessary, calculate wind adjustment factor from canopy and fuel parameters
		if (prop->boolean("surfaceConfWindSpeedAt10MCalc")
			|| prop->boolean("surfaceConfWindSpeedAt20FtCalc"))
		{
			WindAdjFactor();
		}
		waf[i] = vWindAdjFactor->m_nativeValue;

		// If necessary, calculate 20-ft wind speed from 10-m wind speed
		if (prop->boolean("surfaceConfWindSpeedAt10M")
			|| prop->boolean("surfaceConfWindSpeedAt10MCalc"))
		{
			WindSpeedAt20Ft();
		}

		// If necessary, calculate midflame wind speed from 20-ft wind speed and wind adj factor
		if (!prop->boolean("surfaceConfWindSpeedAtMidflame"))
		{
			WindSpeedAtMidflame();
		}
		wmf[i] = vWindSpeedAtMidflame->m_nativeValue;
		*/
	
		ros[i] = surfaceFireSpread_->calculateForwardSpreadRate(directionOfInterest);
		reactionIntensity[i] = surfaceFireSpread_->getReactionIntensity();
		dirMaxSpread[i] = surfaceFireSpread_->getDirectionOfMaxSpread();
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
	double wtd = 0.;	// anything
	// If area weighted spread rate ...
	if (prop->boolean("surfaceConfFuelAreaWeighted"))
	{
		wtdh = (cov[0] * rosh[0]) + (cov[1] * rosh[1]);
		wtdv = (cov[0] * rosv[0]) + (cov[1] * rosv[1]);
	}
	*/
	int method = surfaceInputs_->getTwoFuelModelsMethod();
	if (method == ARITHMETIC)
	{
		spreadRate_ = (coverage[0] * ros[0]) + (coverage[1] * ros[1]);
	}

	/*
	// else if harmonic mean spread rate...
	else if (prop->boolean("surfaceConfFuelHarmonicMean"))
	{
		wtdv = 0.0;
		wtdh = 0.0;
		if (rosh[0] > 0.000001 && rosh[1] > 0.000001)
		{
			wtdh = 1. / ((cov[0] / rosh[0]) + (cov[1] / rosh[1]));
			wtdv = 1. / ((cov[0] / rosv[0]) + (cov[1] / rosv[1]));
		}
	}
	// else if Finney's 2-dimensional spread rate...
	else if (prop->boolean("surfaceConfFuel2Dimensional"))
	{
		double lbRatio = vSurfaceFireLengthToWidth->m_nativeValue;
		int samples = prop->integer("surfaceConfFuel2DSamples");
		int depth = prop->integer("surfaceConfFuel2DDepth");
		int laterals = prop->integer("surfaceConfFuel2DLaterals");
		wtdv = FBL_SurfaceFireExpectedSpreadRate(rosv, cov, 2, lbRatio,
			samples, depth, laterals);
	}
	SurfaceFireSpread->update(wtdh);
	*/

	// The following assignments are based on Pat's rules:
	// If only 1 fuel is present (whether primary or secondary), use its values exclusively
	if (coverage[0] > 0.999 || coverage[1] > 0.999)
	{
		int i = (coverage[0] > 0.999) ? 0 : 1;
		reactionIntensity_ = reactionIntensity[i];
		directionOfMaxSpread_ = dirMaxSpread[i];
		// windAdjustmentFactor_- = windAdjustmentFactor[i]; // TODO
		//windSpeed_ = midFlameWindSpeed[i]; // TODO
		effectiveWind_ = effectiveWindSpeed[i];
		windSpeedLimit_ = windSpeedLimit[i];
		windLimitExceeded_ = windLimitExceeded[i];
		fireLengthToWidthRatio_ = lengthToWidthRatio[i];
		heatPerUnitArea_ = heatPerUnitArea[i];
		fireLineIntensity_ = firelineIntensity[i];
		fireFlameLength_ = flameLength[i];
		fuelbedDepth_ = fuelbedDepth[i];
	}
	// Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
	else
	{
		// Reaction intensity is the maximum of the two models
		reactionIntensity_ = (reactionIntensity[0] > reactionIntensity[1]) ? reactionIntensity[0] : reactionIntensity[1];
	
		// Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
		directionOfMaxSpread_ = dirMaxSpread[0];

		// Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
		//windAdjustmentFactor_ = windAdjustmentFactor[0]; // TODO

		// Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
		midFlameWindSpeed_ = midFlameWindSpeed[0];

		// Effective wind speed is for the FIRST (not necessarily dominant) fuel model
		effectiveWind_ = effectiveWindSpeed[0];
	
		// Maximum reliable wind speed is the minimum of the two models
		windSpeedLimit_ = (windSpeedLimit[0] < windSpeedLimit[1]) ? windSpeedLimit[0] : windSpeedLimit[1];

		// If either wind limit is exceeded, set the flag
		windLimitExceeded_ = (windLimitExceeded[0] || windLimitExceeded[1]);

		// Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
		fireLengthToWidthRatio_ = lengthToWidthRatio[0];

		// Heat per unit area is the maximum of the two models
		heatPerUnitArea_ = (heatPerUnitArea[0] > heatPerUnitArea[1]) ? heatPerUnitArea[0] : heatPerUnitArea[1];

		// Fireline intensity is the maximum of the two models
		fireLineIntensity_ = (firelineIntensity[0] > firelineIntensity[1]) ? firelineIntensity[0] : firelineIntensity[1];
	
		// Flame length is the maximum of the two models
		fireFlameLength_ = (flameLength[0] > flameLength[1]) ? flameLength[0] : flameLength[1];
	
		// Fuel bed depth is the maximum of the two fuel bed depths
		fuelbedDepth_ = (fuelbedDepth[0] > fuelbedDepth[1]) ? fuelbedDepth[0] : fuelbedDepth[1];
	}

	return spreadRate_;
}

