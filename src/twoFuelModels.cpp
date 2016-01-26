#include "twoFuelModels.h"


//------------------------------------------------------------------------------
/*! \brief FuelBedWeighted
*
*  Dependent Variables (Outputs)
*      vSurfaceFireReactionInt (Btu/ft2/min)
*      vSurfaceFireSpreadAtHead (ft/min)
*      vSurfaceFireSpreadAtVector (ft/min)
*      vSurfaceFireMaxDirFromUpslope (clockwise from upslope)
*      vSurfaceFireEffWindAtHead (mi/h)
*      vSurfaceFireEffWindAtVector (mi/h)
*      vSurfaceFireWindSpeedLimit (mi/h)
*      vSurfaceFireWindSpeedFlag (flag)
*      vSurfaceFireHeatPerUnitArea (Btu/ft2)
*      vSurfaceFireLineIntAtHead (Btu/ft/s)
*      vSurfaceFireLineIntAtVector (Btu/ft/s)
*      vSurfaceFireFlameLengIntAtHead (ft)
*      vSurfaceFireFlameLengIntAtVector (ft)
*
*  Independent Variables (Inputs)
*      vSurfaceFuelBedModel1 (item)
*      vSurfaceFuelBedModel2 (item)
*      vSurfaceFuelBedCoverage1 (fraction)
*      vSiteSlopeFraction (rise/reach)
*      vWindDirFromUpslope (degrees)
*      vWindSpeedAtMidflame (mi/h)
*      vSurfaceFuelMoisDead1 (fraction)
*      vSurfaceFuelMoisDead10 (fraction)
*      vSurfaceFuelMoisDead100 (fraction)
*      vSurfaceFuelMoisDead1000 (fraction)
*      vSurfaceFuelMoisLiveHerb (fraction)
*      vSurfaceFuelMoisLiveWood (fraction)
*      vSurfaceFuelLoadTransferFraction (fraction)
*      vSurfaceFireVectorDirFromUpslope (deg)
*/


void TwoFuelModels::FuelBedWeighted(void)
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
	double rosh[2], rosv[2];	// ros at head and vector
	double flih[2], fliv[2];	// fireline intensity at head and vector
	double flh[2], flv[2];		// flame length at head and vector
	double ewsh[2], ewsv[2];	// effective wind speed at head and vector
	double flw[2];				// fire length-to-width ratio
	double rxi[2], hua[2], mxd[2];	// reaction intensity, heat per unit area, dir of max spread
	double waf[2], wmf[2];		// wind adjustment factor and wind speed at midflame
	double wsl[2];				// wind speed limit
	int    wsf[2];				// wind speed flag

	// Calculate fire outputs for each fuel model
	for (int i = 0; i<2; i++)
	{
		// Load the fuel attributes into the equation tree's fuel model
		// This replaces the call to FuelBedModel()
		vSurfaceFuelBedDepth->update(fm[i]->m_depth);
		vSurfaceFuelBedMextDead->update(fm[i]->m_mext);
		vSurfaceFuelHeatDead->update(fm[i]->m_heatDead);
		vSurfaceFuelHeatLive->update(fm[i]->m_heatLive);
		vSurfaceFuelLoadDead1->update(fm[i]->m_load1);
		vSurfaceFuelLoadDead10->update(fm[i]->m_load10);
		vSurfaceFuelLoadDead100->update(fm[i]->m_load100);
		vSurfaceFuelLoadLiveHerb->update(fm[i]->m_loadHerb);
		vSurfaceFuelLoadLiveWood->update(fm[i]->m_loadWood);
		vSurfaceFuelSavrDead1->update(fm[i]->m_savr1);
		vSurfaceFuelSavrLiveHerb->update(fm[i]->m_savrHerb);
		vSurfaceFuelSavrLiveWood->update(fm[i]->m_savrWood);
		vSurfaceFuelLoadTransferEq->updateItem(fm[i]->m_transfer);

		// Load the equation tree's fuel model into fuel parameter arrays
		FuelBedParms();
		FuelLoadTransferFraction();		// vSurfaceFuelLoadTransferFraction

		// Calculate the fuel bed intermediates
		FuelBedIntermediates();

		// Calculate vSurfaceFireResidenceTime
		FireResidenceTime();			// vSurfaceFireResidenceTime

		// Load life class moistures into the equation tree time-lag classes
		if (prop->boolean("surfaceConfMoisLifeCat"))
		{
			FuelMoisLifeClass();		// vSurfaceFuelMoisLifeDead, vSurfaceFuelMoisLifeLive
		}
		// or load moisture scenario into the equation tree time-lag classes
		else if (prop->boolean("surfaceConfMoisScenario"))
		{
			FuelMoisScenarioModel();
		}
		// Load equation tree time-lag classes into fuel parameter arrays
		FuelMoisTimeLag();

		// Calculate heat sink outputs
		FuelBedHeatSink();

		// Calculate vSurfaceFirePropagatingFlux
		FirePropagatingFlux();

		// Calculate and store vSurfaceFireReactionInt
		FireReactionInt();
		rxi[i] = vSurfaceFireReactionInt->m_nativeValue;

		// Calculate vSurfaceFireNoWindRate
		FireNoWindRate();

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

		// Calculate and store fire spread outputs
		FireSpreadAtHead();
		rosh[i] = vSurfaceFireSpreadAtHead->m_nativeValue;
		mxd[i] = vSurfaceFireMaxDirFromUpslope->m_nativeValue;
		ewsh[i] = vSurfaceFireEffWindAtHead->m_nativeValue;
		wsl[i] = vSurfaceFireWindSpeedLimit->m_nativeValue;
		wsf[i] = (wmf[i] > wsl[i]) ? 1 : 0;

		// Calculate vSurfaceFireLineIntAtHead
		FireLineIntAtHead();
		flih[i] = vSurfaceFireLineIntAtHead->m_nativeValue;

		// Calculate vSurfaceFireFlameLengAtHead
		FireFlameLengAtHead();
		flh[i] = vSurfaceFireFlameLengAtHead->m_nativeValue;

		// Calculate vSurfaceFireLengthToWidth
		FireLengthToWidth();
		flw[i] = vSurfaceFireLengthToWidth->m_nativeValue;

		// Calculate vSurfaceFireEccentricity
		FireEccentricity();

		// If spread direction is upslope, set vector to 0
		if (prop->boolean("surfaceConfSpreadDirMax"))
		{
			vSurfaceFireVectorBeta->update(0.);
		}
		else
		{
			FireVectorBeta();
		}

		// Calculate vSurfaceFireSpreadAtVector
		FireSpreadAtBeta();
		rosv[i] = vSurfaceFireSpreadAtVector->m_nativeValue;

		// Calculate vSurfaceFireLineIntAtVector
		FireLineIntAtVector();
		fliv[i] = vSurfaceFireLineIntAtVector->m_nativeValue;

		// Calculate vSurfaceFireFlameLengAtVector
		FireFlameLengAtVector();
		flv[i] = vSurfaceFireFlameLengAtVector->m_nativeValue;

		// Calculate vSurfaceFireEffWindAtVector
		FireEffWindAtVector();
		ewsv[i] = vSurfaceFireEffWindAtVector->m_nativeValue;

		// Calculate and store vSurfaceFireHeatPerUnitArea
		FireHeatPerUnitArea();
		hua[i] = vSurfaceFireHeatPerUnitArea->m_nativeValue;
	}

	//------------------------------------------------
	// Determine and store combined fuel model outputs
	//------------------------------------------------

	// Fire spread rate depends upon the weighting method...
	double wtdh = 0.;	// value at head
	double wtdv = 0.;	// value at vector
	double wtd = 0.;	// anything
	// If area weighted spread rate ...
	if (prop->boolean("surfaceConfFuelAreaWeighted"))
	{
		wtdh = (cov[0] * rosh[0]) + (cov[1] * rosh[1]);
		wtdv = (cov[0] * rosv[0]) + (cov[1] * rosv[1]);
	}
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
		wtdh = FBL_SurfaceFireExpectedSpreadRate(rosh, cov, 2, lbRatio,
			samples, depth, laterals);
		wtdv = FBL_SurfaceFireExpectedSpreadRate(rosv, cov, 2, lbRatio,
			samples, depth, laterals);
	}
	vSurfaceFireSpreadAtHead->update(wtdh);
	vSurfaceFireSpreadAtVector->update(wtdv);

	// The following assignments are based on Pat's rules:
	// If only 1 fuel is present (whether primary or secondary), use its values exclusively
	if (cov[0] > 0.999 || cov[1] > 0.999)
	{
		int i = (cov[0] > 0.999) ? 0 : 1;
		vSurfaceFireReactionInt->update(rxi[i]);
		vSurfaceFireMaxDirFromUpslope->update(mxd[i]);
		vWindAdjFactor->update(waf[i]);
		vWindSpeedAtMidflame->update(wmf[i]);
		vSurfaceFireEffWindAtHead->update(ewsh[i]);
		vSurfaceFireEffWindAtVector->update(ewsv[i]);
		vSurfaceFireWindSpeedLimit->update(wsl[i]);
		vSurfaceFireWindSpeedFlag->updateItem(wsf[i]);
		vSurfaceFireLengthToWidth->update(flw[i]);
		vSurfaceFireHeatPerUnitArea->update(hua[i]);
		vSurfaceFireLineIntAtHead->update(flih[i]);
		vSurfaceFireLineIntAtVector->update(fliv[i]);
		vSurfaceFireFlameLengAtHead->update(flh[i]);
		vSurfaceFireFlameLengAtVector->update(flv[i]);
		vSurfaceFuelBedDepth->update(fm[i]->m_depth);
	}
	// Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
	else
	{
		// Reaction intensity is the maximum of the two models
		wtd = (rxi[0] > rxi[1]) ? rxi[0] : rxi[1];
		vSurfaceFireReactionInt->update(wtd);

		// Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
		vSurfaceFireMaxDirFromUpslope->update(mxd[0]);

		// Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
		vWindAdjFactor->update(waf[0]);

		// Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
		vWindSpeedAtMidflame->update(wmf[0]);

		// Effective wind speed is for the FIRST (not necessarily dominant) fuel model
		vSurfaceFireEffWindAtHead->update(ewsh[0]);
		vSurfaceFireEffWindAtVector->update(ewsv[0]);

		// Maximum reliable wind speed is the minimum of the two models
		wtd = (wsl[0] < wsl[1]) ? wsl[0] : wsl[1];
		vSurfaceFireWindSpeedLimit->update(wtd);

		// If either wind limit is exceeded, set the flag
		vSurfaceFireWindSpeedFlag->updateItem(wsf[0] || wsf[1]);

		// Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
		vSurfaceFireLengthToWidth->update(flw[0]);

		// Heat per unit area is the maximum of the two models
		wtd = (hua[0] > hua[1]) ? hua[0] : hua[1];
		vSurfaceFireHeatPerUnitArea->update(wtd);

		// Fireline intensity is the maximum of the two models
		wtdh = (flih[0] > flih[1]) ? flih[0] : flih[1];
		wtdv = (fliv[0] > fliv[1]) ? fliv[0] : fliv[1];
		vSurfaceFireLineIntAtHead->update(wtdh);
		vSurfaceFireLineIntAtVector->update(wtdv);

		// Flame length is the maximum of the two models
		wtdh = (flh[0] > flh[1]) ? flh[0] : flh[1];
		wtdv = (flv[0] > flv[1]) ? flv[0] : flv[1];
		vSurfaceFireFlameLengAtHead->update(wtdh);
		vSurfaceFireFlameLengAtVector->update(wtdv);

		// Fuel bed depth is the maximum of the two fuel bed depths
		wtd = (fm[0]->m_depth > fm[1]->m_depth) ? fm[0]->m_depth : fm[1]->m_depth;
		vSurfaceFuelBedDepth->update(wtd);
	}
	//------------
	// Log results
	//------------

	if (m_log)
	{
		fprintf(m_log, "%sbegin proc FuelBedWeighted() 6 13\n", Margin);
		fprintf(m_log, "%s  i vSurfaceFuelBedModel1 %d %s\n", Margin,
			vSurfaceFuelBedModel1->activeItemDataIndex(),
			vSurfaceFuelBedModel1->activeItemName().latin1());
		fprintf(m_log, "%s  i vSurfaceFuelBedModel2 %d %s\n", Margin,
			vSurfaceFuelBedModel2->activeItemDataIndex(),
			vSurfaceFuelBedModel2->activeItemName().latin1());
		fprintf(m_log, "%s  i vSurfaceFuelBedCoverage1 %g %s\n", Margin,
			vSurfaceFuelBedCoverage1->m_nativeValue,
			vSurfaceFuelBedCoverage1->m_nativeUnits.latin1());
		fprintf(m_log, "%s  i vSiteSlopeFraction %g %s\n", Margin,
			vSiteSlopeFraction->m_nativeValue,
			vSiteSlopeFraction->m_nativeUnits.latin1());
		fprintf(m_log, "%s  i vWindDirFromUpslope %g %s\n", Margin,
			vWindDirFromUpslope->m_nativeValue,
			vWindDirFromUpslope->m_nativeUnits.latin1());
		fprintf(m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
			vWindSpeedAtMidflame->m_nativeValue,
			vWindSpeedAtMidflame->m_nativeUnits.latin1());

		fprintf(m_log, "%s  o vSurfaceFireReactionInt %g %s\n", Margin,
			vSurfaceFireReactionInt->m_nativeValue,
			vSurfaceFireReactionInt->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireSpreadAtHead %g %s\n", Margin,
			vSurfaceFireSpreadAtHead->m_nativeValue,
			vSurfaceFireSpreadAtHead->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireSpreadAtVector %g %s\n", Margin,
			vSurfaceFireSpreadAtVector->m_nativeValue,
			vSurfaceFireSpreadAtVector->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireMaxDirFromUpslope %g %s\n", Margin,
			vSurfaceFireMaxDirFromUpslope->m_nativeValue,
			vSurfaceFireMaxDirFromUpslope->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireEffWindAtHead %g %s\n", Margin,
			vSurfaceFireEffWindAtHead->m_nativeValue,
			vSurfaceFireEffWindAtHead->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireEffWindAtVector %g %s\n", Margin,
			vSurfaceFireEffWindAtVector->m_nativeValue,
			vSurfaceFireEffWindAtVector->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireWindSpeedLimit %g %s\n", Margin,
			vSurfaceFireWindSpeedLimit->m_nativeValue,
			vSurfaceFireWindSpeedLimit->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireWindSpeedFlag %g %s\n", Margin,
			vSurfaceFireWindSpeedFlag->m_nativeValue,
			vSurfaceFireWindSpeedFlag->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireHeatPerUnitArea %g %s\n", Margin,
			vSurfaceFireHeatPerUnitArea->m_nativeValue,
			vSurfaceFireHeatPerUnitArea->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireLineIntAtHead %g %s\n", Margin,
			vSurfaceFireLineIntAtHead->m_nativeValue,
			vSurfaceFireLineIntAtHead->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireLineIntAtVector %g %s\n", Margin,
			vSurfaceFireLineIntAtVector->m_nativeValue,
			vSurfaceFireLineIntAtVector->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireFlameLengAtHead %g %s\n", Margin,
			vSurfaceFireFlameLengAtHead->m_nativeValue,
			vSurfaceFireFlameLengAtHead->m_nativeUnits.latin1());
		fprintf(m_log, "%s  o vSurfaceFireFlameLengAtVector %g %s\n", Margin,
			vSurfaceFireFlameLengAtVector->m_nativeValue,
			vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1());
	}
	return;
	*/
}

