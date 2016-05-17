#ifndef SURFACEFIRE_HEADER
#define SURFACEFIRE_HEADER

#include "surfaceFireReactionIntensity.h"
#include "surfaceFuelbedIntermediates.h"

class SurfaceFireSpread
{
    friend class SurfaceTwoFuelModels;
public:
    SurfaceFireSpread();
    SurfaceFireSpread(const SurfaceFireSpread& rhs);
    SurfaceFireSpread& operator= (const SurfaceFireSpread& rhs);
    SurfaceFireSpread(const FuelModelSet& fuelModelSet, const SurfaceInputs& surfaceInputs);
    double calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink);
    double calculateForwardSpreadRate(double directionOfInterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfInterest);
    double calculateFlameLength(double firelineIntensity);

    // Public getters
    double getFuelbedDepth() const;
    double getSpreadRate() const;
    double getDirectionOfMaxSpread() const;
    double getEffectiveWindSpeed() const;
    double getFirelineIntensity() const;
    double getFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getHeatPerUnitArea() const;
    double getResidenceTime() const;
    double getWindSpeedLimit() const;
    double getReactionIntensity() const;
    double getMidflameWindSpeed() const;
    double getEllipticalA() const;
    double getEllipticalB() const;
    double getEllipticalC() const;
    bool getIsWindLimitExceeded() const;

private:
    // Private setters
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

    void initializeMembers();
    void calculateHeatPerUnitArea();
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
    void calculateEllipticalDimensions();
    double convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const;

    // Pointers and references to other objects
    const FuelModelSet* fuelModelSet_;
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
    double residenceTime_;
    double reactionIntensity_;
    double firelineIntensity_;
    double flameLength_;
    double backingSpreadRate_;
   
    // Elliptical dimensions
    double eccentricity_;   // deviation from a perfect circle being zero in the case of a perfect circle, unitless real in interval [0,1) 
    double ellipticalA_;    // semiminor axis, commonly refered to as 'b' in mathematics and 'a' in fire literature (ft)
    double ellipticalB_;    // semimajor axis, commonly refered to as 'a' in mathematics and 'b' in fire literature (ft)
    double ellipticalC_;    // distance from center of ellipse to one of its foci, commonly referered to a 'c' in mathematics (ft)

    double midflameWindSpeed_; 
    double windAdjustmentFactor_;
    WindAdjustmentFactorMethod::WindAdjustmentFactorMethodEnum windAdjustmentFactorMethod_;
    double canopyCrownFraction_;

    double aspenMortality_;
};

#endif // SURFACEFIRE_HEADER
