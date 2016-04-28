#ifndef SURFACEFIRE_HEADER
#define SURFACEFIRE_HEADER

#include "surfaceFireReactionIntensity.h"
#include "surfaceFuelbedIntermediates.h"

class SurfaceFireSpread
{
public:
    SurfaceFireSpread();
    SurfaceFireSpread(const SurfaceFireSpread& rhs);
    SurfaceFireSpread& operator= (const SurfaceFireSpread& rhs);
    SurfaceFireSpread(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs);
    double calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink);
    double calculateForwardSpreadRate(double directionOfInterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfInterest);

    // Public getters
    double getFuelbedDepth() const;
    double getSpreadRate() const;
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
    void calculateWindFactor();
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

    // Pointers and references to other objects
    const FuelModels* fuelModels_;
    const SurfaceInputs* surfaceInputs_;
    SurfaceFuelbedIntermediates surfaceFuelbedIntermediates_;
    SurfaceFireReactionIntensity surfaceFireReactionIntensity_;
  
    // Member variables
    bool   isWindLimitExceeded_;
    double effectiveWindSpeed_;
    double windSpeedLimit_;
    double phiS_;											// Slope factor, Rothermel 1972, equation 51
    double phiW_;											// Wind factor, Rothermel 1972, equation 47
    double windB_;											// Rothermel 1972, Equation 49
    double windC_;											// Rothermel 1972, Equation 48
    double windE_;											// Rothermel 1972, Equation 50
    double directionOfMaxSpread_;							// Direction of max fire spread in degrees clockwise from upslope
    double noWindNoSlopeSpreadRate_;						// No-wind-no-slope fire spread rate, Rothermel 1972, equation 52
    double forwardSpreadRate_;								// Maximum rate of fire spread rate, Rothermel 1972, equation 52
    double heatPerUnitArea_;
    double fireLengthToWidthRatio_;
    double eccentricity_;
    double residenceTime_;
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
