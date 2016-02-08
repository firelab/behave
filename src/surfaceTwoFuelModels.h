#ifndef SURFACETWOFUELMODELS_HEADER
#define SURFACETWOFUELMODELS_HEADER

#include "fuelModels.h"
#include "surfaceInputs.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceFireSpread.h"

class SurfaceTwoFuelModels
{
public:
	SurfaceTwoFuelModels(SurfaceInputs& surfaceInputs, SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates, SurfaceFireSpread& surfaceFireSpread);
	double FuelBedWeighted(double directionOfInterest);

private:
	enum MEAN_METHOD
	{
		NO_METHOD = 0,
		ARITHMETIC = 1,
		HARMONIC = 2,
		TWO_DIMENSIONAL = 3
	};

	SurfaceInputs* surfaceInputs_;
	SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;
	SurfaceFireSpread* surfaceFireSpread_;

	bool windLimitExceeded_;			// (flag)
	double reactionIntensity_;			// (Btu / ft2 / min)
	double spreadRate_;					// (ft / min)
	double directionOfMaxSpread_;		// (clockwise from upslope or north)
	double effectiveWind_;				// (mi / h)
	double fuelbedDepth_;
	double heatPerUnitArea_;
	double midFlameWindSpeed_;
	double windSpeedLimit_;				// (mi / h)
	double windAdjustmentFactor_;
	double fireHeatPerUnitArea_;		// (Btu / ft2)
	double fireLineIntensity_;			// (Btu / ft / s)
	double fireFlameLength_;			// (ft)
	double fireLengthToWidthRatio_;
};

#endif //SURFACETWOFUELMODELS_HEADER
