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

    double surfaceFireExpectedSpreadRate(double *ros, double *coverage, int fuels,
        double lbRatio, int samples, int depth, int laterals,
        double *harmonicRos = 0);
    void calculateFireOutputsForEachModel(double directionOfInterest);
    void calculateSpreadRateBasedOnMethod();

    // Member arrays
    int fuelModelNumber_[TwoFuelModels::NUMBER_OF_MODELS];            // fuel model number
    double coverageForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];                // percent coverage of fuel model
    double rosForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];                     // rate of spread
    double firelineIntensityForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];       // fireline intensity
    double flameLengthForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];             // flame length
    double fuelbedDepthForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];            // fuel bed depth in feet
    double effectiveWindSpeedForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];      // effective wind speed
    double lengthToWidthRatioForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];      // fire length-to-width ratio
    double reactionIntensityForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];       // reaction intensity, 
    double heatPerUnitAreaForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];         // heat per unit area
    double dirMaxSpreadForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];            // direction of max spread
    double windAdjustmentFactorForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];    // wind adjustment factor
    double midFlameWindSpeedForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];       // wind speed at midflame
    double windSpeedLimitForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];          // wind speed limit
    bool windLimitExceededForFuelModel_[TwoFuelModels::NUMBER_OF_MODELS];         // wind speed exceeded flag

    // Member variables
    bool windLimitExceeded_;        // (flag)
    double reactionIntensity_;      // (Btu / ft2 / min)
    double spreadRate_;             // (ft / min)
    double directionOfMaxSpread_;   // (clockwise from upslope or north)
    double effectiveWind_;          // (mi / h)
    double fuelbedDepth_;           // (ft)
    double heatPerUnitArea_;        // (Btu / ft2)
    double midFlameWindSpeed_;
    double windSpeedLimit_;         // (mi / h)
    double windAdjustmentFactor_;
    double fireLineIntensity_;      // (Btu / ft / s)
    double fireFlameLength_;        // (ft)
    double fireLengthToWidthRatio_;
};

#endif //SURFACETWOFUELMODELS_HEADER
