#ifndef SURFACEFIRE_HEADER
#define SURFACEFIRE_HEADER

#include <cmath>
#include "surfaceEnums.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceFirePropogatingFlux.h"
#include "surfaceFireReactionIntensity.h"
#include "surfaceInputs.h"
#include "windAdjustmentFactor.h"

class SurfaceFireSpread
{
public:
    SurfaceFireSpread(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates, const SurfaceInputs& surfaceInputs);
    double calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink);
    double calculateForwardSpreadRate(double directionOfInterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfInterest);

    // Public getters

    double getDirectionOfMaxSpread() const;
    double getEffectiveWindSpeed() const;
    double getFirelineIntensity() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getResidenceTime() const;
    double getHeatPerUnitArea() const;
    double getWindSpeedLimit() const;
    double getReactionIntensity() const;
    double getMidflameWindSpeed() const;
    bool getIsWindLimitExceeded() const;

    // public setters
    void setDirectionOfMaxSpread(double directionOFMaxSpread);
    void setEffectiveWindSpeed(double effectiveWindSpeed);
    void setFirelineIntensity(double firelineIntensity);
    void setFlameLength(double flameLength);
    void setFireLengthToWidthRatio(double lengthToWidthRatio);
    void setFireEccentricity(double eccentricity);
    void setResidenceTime(double residenceTime);
    void setWindSpeedLimit(double windSpeedLimit);
    void setReactionIntensity(double reactionIntensity);
    void setHeatPerUnitArea(double heatPerUnitArea);
    void setIsWindLimitExceeded(bool isWindLimitExceeded);
    void setWindAdjustmentFactor(double windAdjustmentFactor);
    void setMidflameWindSpeed(double midflameWindSpeed);

private:
    void initializeMembers();
    void calculateWindAdjustmentFactor();
    void calculateWindFactors();
    void calculateSlopeFactor();
    void calculateFireLengthToWidthRatio();
    void calculateSurfaceFireEccentricity();
    void calculateResidenceTime();
    void calculateFireFirelineIntensity();
    void calculateFlameLength();
    void calculateWindSpeedLimit();
    void calculateDirectionOfMaxSpread();
    void calculateBackingSpreadRate();
    void calculateMidflameWindSpeed();
    void calculateEffectiveWindSpeed();
    void applyWindSpeedLimit();
    double convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const;

    static const int DEAD = 0;
    static const int LIVE = 1;
    static const int MAX_LIFE_STATES = 2;

    // Pointers to other objects
    const SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;
    const SurfaceInputs* surfaceInputs_;
    SurfaceFireReactionIntensity surfaceFireReactionIntensity_;
    SurfaceFirePropogatingFlux surfaceFirePropogatingFlux_;

    // Member variables
    bool   isWindLimitExceeded_;
    double effectiveWindSpeed_;
    double windSpeedLimit_;
    double phiS_;											// Slope factor, Rothermel 1972, equation 51
    double phiW_;											// Wind coefficient, Rothermel 1972, equation 47
    double windB_;											// Rothermel 1972, Equation 49
    double windC_;											// Rothermel 1972, Equation 48
    double windE_;											// Rothermel 1972, Equation 50
    double windDirectionFromUpslope_;						// Wind direction in degrees clockwise from upslope
    double directionOfMaxSpread_;							// Direction of max fire spread in degrees clockwise from upslope
    double noWindNoSlopeSpreadRate_;						// No-wind-no-slope fire spread rate, Rothermel 1972, equation 52
    double forwardSpreadRate_;								// Maximum rate of fire spread rate, Rothermel 1972, equation 52
    double heatPerUnitArea_;
    double fireLengthToWidthRatio_;
    double eccentricity_;
    double residenceTime_;
    double spreadRate_; // surface fire rate of spread in ft/min
    double reactionIntensity_;
    double firelineIntensity_;
    double flameLength_;
    double backingSpreadRate_;

    double midflameWindSpeed_;
    double windAdjustmentFactor_;
    WindAdjustmentFactorMethod::WindAdjustmentFactorMethodEnum windAdjustmentFactorMethod_;
    double canopyCrownFraction_;

    double aspenMortality_;
};

#endif // SURFACEFIRE_HEADER
