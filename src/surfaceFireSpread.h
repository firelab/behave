#ifndef SURFACEFIRE_HEADER
#define SURFACEFIRE_HEADER

#include <cmath>
#include "surfaceFuelbedIntermediates.h"
#include "surfaceFirePropogatingFlux.h"
#include "surfaceFireReactionIntensity.h"
#include "surfaceInputs.h"

class SurfaceFireSpread
{
public:
	SurfaceFireSpread(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates, const SurfaceInputs& surfaceInputs);
	double calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink);
	double calculateForwardSpreadRate(double directionOfInterest = -1.0);
	double calculateSpreadRateAtVector(double directionOfInterest);

	// Public getters
	double getDirectionOfMaxSpread() const;
	double getFlameLength() const;
	double getFireLengthToWidthRatio() const;
	double getFireEccentricity() const;
	
private:
	void initializeMembers();
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
	void calculateEffectiveWindSpeed();
	void applyWindSpeedLimit();
	double convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const;

	static const int DEAD = 0;
	static const int LIVE = 1;
	static const int MAX_LIFE_STATES = 2;

	// Pointers to other objects
	const SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;
	const SurfaceInputs* surfaceInputs_;
	SurfaceFireReactionIntensity surfaceFireReactionIntensity_; // dereferenced pointer needed for SurfaceFireReactionIntensity constructor
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
	//double etaM_[MAX_LIFE_STATES];						// Moisture damping coefficient for  i-th categort (dead/live)
	//double etaS_[MAX_LIFE_STATES];						// Mineral(silica) damping coefficient for i - th categort(dead / live)
	//double propagatingFlux_;								// Propagating flux ratio, Rothermel 1972, equation 42
	double fireLengthToWidthRatio_;
	double eccentricity_;
	double residenceTime_;
	double spreadRate_; // surface fire rate of spread in ft/min
	double reactionIntensity_;
	double firelineIntensity_;
	double flameLength_;
	double backingSpreadRate_;

	//SurfaceFireReactionIntensity surfaceFireReactionIntensity_;
	//SurfaceFirePropogatingFlux surfaceFirePropogatingFlux_;
	//SurfaceFuelbedHeatSink surfaceFuelbedHeatSink_;
};

#endif // SURFACEFIRE_HEADER
