#ifndef SURFACETWOFUELMODELS_HEADER
#define SURFACETWOFUELMODELS_HEADER

#include "fuelModels.h"
#include "newext.h"
#include "randfuel.h"
#include "randthread.h"
#include "surfaceEnums.h"
#include "surfaceInputs.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceFireSpread.h"

class SurfaceTwoFuelModels
{
public:
    SurfaceTwoFuelModels(SurfaceInputs& surfaceInputs, SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates,
        SurfaceFireSpread& surfaceFireSpread);
    double calculateWeightedSpreadRate(double directionOfInterest);
    double surfaceFireExpectedSpreadRate(double *ros, double *coverage, int fuels,
        double lbRatio, int samples, int depth, int laterals,
        double *harmonicRos = 0);

    //public getters
    bool getWindLimitExceeded() const;
    double getReactionIntensity() const;
    double getSpreadRate() const;
    double getDirectionOfMaxSpread() const;
    double getEffectiveWind() const;
    double getFuelbedDepth() const;
    double getHeatPerUnitArea() const;
    double getMidFlameWindSpeed() const;
    double getWindSpeedLimit() const;
    double WindAdjustmentFactor() const;
    double getFireLineIntensity() const;
    double getFireFlameLength() const;
    double getFireLengthToWidthRatio() const;

private:
  
    SurfaceInputs* surfaceInputs_;
    SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;
    SurfaceFireSpread* surfaceFireSpread_;

    bool windLimitExceeded_;        // (flag)
    double reactionIntensity_;      // (Btu / ft2 / min)
    double spreadRate_;             // (ft / min)
    double directionOfMaxSpread_;   // (clockwise from upslope or north)
    double effectiveWind_;          // (mi / h)
    double fuelbedDepth_;
    double heatPerUnitArea_;
    double midFlameWindSpeed_;
    double windSpeedLimit_;         // (mi / h)
    double windAdjustmentFactor_;
    double fireHeatPerUnitArea_;    // (Btu / ft2)
    double fireLineIntensity_;      // (Btu / ft / s)
    double fireFlameLength_;        // (ft)
    double fireLengthToWidthRatio_;
};

#endif //SURFACETWOFUELMODELS_HEADER
