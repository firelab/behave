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
	void FuelBedWeighted(int firstFuelModelNumber, int secondFuelModelNumber);

private:
	enum MEAN_METHOD
	{
		ARITHMETIC = 0,
		HARMONIC = 1,
		TWO_DIMENSIONAL = 2
	};

	SurfaceInputs*		surfaceInputs_;
	SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;
	SurfaceFireSpread* surfaceFireSpread_;

	double reactionIntensity;		// (Btu / ft2 / min)
	double spreadRate;				// (ft / min)
	double maxDirectionFromUpslope;	// (clockwise from upslope)
	double effectiveWind;			// (mi / h)
	double windSpeedLimit;			// (mi / h)
	double fireWindSpeedFlag;		// (flag)
	double fireHeatPerUnitArea;		// (Btu / ft2)
	double fireLineIntensity;		// (Btu / ft / s)
	double fireFlameLength;			// (ft)
};

#endif //SURFACETWOFUELMODELS_HEADER
