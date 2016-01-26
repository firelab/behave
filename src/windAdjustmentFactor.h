#ifndef WINDADJUSTMENTFACTOR_HEADER
#define WINDADJUSTMENTFACTOR_HEADER

// Standard include files
#include <math.h>

class WindAjustmentFactor
{
public:
	void WindAdjFactor(void);
	double FBL_WindAdjustmentFactor(double canopyCover, double canopyHt,
		double crownRatio, double fuelDepth, double *fraction, int *method);
private:
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero to prevent divide - by - zero, sqrt(0), etc.

	double	vTreeCanopyCover;
	double	vTreeCoverHt;
	double	vTreeCrownRatio;
	double	vSurfaceFuelBedDepth;
	double	vWindAdjFactor;
	double	vTreeCanopyCrownFraction;
	int		vWindAdjMethod;

};

#endif //WINDADJUSTMENTFACTOR_HEADER