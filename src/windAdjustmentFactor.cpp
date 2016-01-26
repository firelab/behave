#include "windAdjustmentFactor.h"

//------------------------------------------------------------------------------
/*! \brief Calculates the wind adjustment factor for scaling wind speed from
* 20-ft to midflame height.
*
*  For sheltered conditions under a canopy, Albini and Baughman (1979)
*  equation 21 (page 9) is used for the wind adjustment factor.
*
*  For unsheltered conditions, wind adjustment factor is calculated as an
*	average from the top of the fuel bed to twice the fuel bed depth,
*	using Albini and Baughman (1979) equation 9 (page 5).
*
*  \param canopyCover  Canopy cover projected onto ground [0..1].
*  \param canopyHt     Tree canopy height from the ground (ft).
*  \param crownRatio   Tree crown length-to-tree height ratio [0..1].
*  \param fuelDepth    Fuel bed depth (ft).
*  \param[out] fraction Pointer to a real that on return holds the fraction
*                      of crown volume filled with tree crowns
*  \param[out] method  Pointer to an integer that on return holds the method
*                      used: 0=unsheltered, 1=sheltered
*
*  \return Wind adjustment factor [0..1].
*/

double WindAjustmentFactor::FBL_WindAdjustmentFactor(double canopyCover, double canopyHt,
	double crownRatio, double fuelDepth, double *fraction, int *method)
{
	double windAdjustmentFactor = 1.0;
	crownRatio = (crownRatio < 0.0) ? 0.0 : crownRatio;
	crownRatio = (crownRatio > 1.0) ? 1.0 : crownRatio;
	canopyCover = (canopyCover < 0.0) ? 0.0 : canopyCover;
	canopyCover = (canopyCover > 1.0) ? 1.0 : canopyCover;
	// Pat asked to delete this line on 11/9/07
	//canopyHt    = ( canopyHt < 10.0 ) ? 10.0 : canopyHt;

	// f == fraction of the volume under the canopy top that is filled with
	// tree crowns (division by 3 assumes conical crown shapes).
	double f = crownRatio * canopyCover / 3.0;

	// Unsheltered
	// The following line was modified by Pat Andrews, 11/9/07
	//if ( canopyCover < SMIDGEN || f < 0.05 )
	int mthd = 0;
	if (canopyCover < SMIDGEN || f < 0.05 || canopyHt < 6.0)
	{
		if (fuelDepth > SMIDGEN)
		{
			windAdjustmentFactor = 1.83 / log((20. + 0.36 * fuelDepth) / (0.13 * fuelDepth));
		}
		mthd = 0;
	}
	// Sheltered
	else
	{
		windAdjustmentFactor = 0.555 / (sqrt(f * canopyHt) * log((20 + 0.36 * canopyHt)
			/ (0.13 * canopyHt)));
		mthd = 1;
	}
	// Constrain the result
	windAdjustmentFactor = (windAdjustmentFactor > 1.0) ? 1.0 : windAdjustmentFactor;
	windAdjustmentFactor = (windAdjustmentFactor < 0.0) ? 0.0 : windAdjustmentFactor;
	*fraction = f;
	*method = mthd;
	return(windAdjustmentFactor);
}

//------------------------------------------------------------------------------
/*! \brief WindAdjFactor
*
*  Dependent Variables (Outputs)
*      vTreeCanopyCrownFraction (ft^3/ft^3).
*      vWindAdjFactor (dl)
*      vWindAdjMethod (discrete)
*
*  Independent Variables (Inputs)
*      vTreeCanopyCover (fraction)
*      vTreeCoverHt (ft)
*      vTreeCrownRatio (fraction)
*      vSurfaceFuelBedDepth (ft)
*/

void WindAjustmentFactor::WindAdjFactor(void)
{
	// Access current input values
	double cc = vTreeCanopyCover;
	double ch = vTreeCoverHt;
	double cr = vTreeCrownRatio;
	double fd = vSurfaceFuelBedDepth;
	// Calculate results
	int method = 0;
	double fraction = 0.0;
	double waf = FBL_WindAdjustmentFactor(cc, ch, cr, fd, &fraction, &method);
	// Store results
	vWindAdjFactor = waf;
	vWindAdjMethod = method;
	vTreeCanopyCrownFraction = fraction;
	
	return;
}
