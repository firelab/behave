#include "surfaceFireSpread.h"

SurfaceFireSpread::SurfaceFireSpread(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates, const SurfaceInputs& surfaceInputs)
	: surfaceFireReactionIntensity_{ surfaceFuelbedIntermediates }, surfaceFirePropogatingFlux_{}
{
	surfaceFuelbedIntermediates_ = &surfaceFuelbedIntermediates;
	surfaceInputs_ = &surfaceInputs;
	initializeMembers();
}

double SurfaceFireSpread::calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink)
{
	noWindNoSlopeSpreadRate_ = (heatSink < 1.0e-07)
		? (0.0)
		: (reactionIntensity * propagatingFlux / heatSink);
	return(noWindNoSlopeSpreadRate_);
}

void SurfaceFireSpread::calculateResidenceTime()
{
	double sigma = surfaceFuelbedIntermediates_->getSigma();
	residenceTime_ = ((sigma < 1.0e-07)
		? (0.0)
		: (384. / sigma));
}

void SurfaceFireSpread::calculateFireFirelineIntensity()
{
	firelineIntensity_ = forwardSpreadRate_ * reactionIntensity_ * residenceTime_ / 60.0;
}

void SurfaceFireSpread::calculateFlameLength()
{
	flameLength_ = ((firelineIntensity_ < 1.0e-07)
		? (0.0)
		: (0.45 * pow(firelineIntensity_, 0.46)));
}

double SurfaceFireSpread::calculateForwardSpreadRate(double directionOfInterest)
{
	// Reset member variables to prepare for next calculation
	initializeMembers();

	const double PI = 3.141592653589793238462643383279;

	// Get needed fuelbed intermediates
	double sigma = surfaceFuelbedIntermediates_->getSigma();
	double packingRatio = surfaceFuelbedIntermediates_->getPackingRatio();
	double propagatingFlux = surfaceFirePropogatingFlux_.calculatePropagatingFlux(sigma, packingRatio);
	double heatSink = surfaceFuelbedIntermediates_->getHeatSink();
	reactionIntensity_ = surfaceFireReactionIntensity_.calculateReactionIntensity();

	// Calculate Wind and Slope Factors
	calculateWindFactors();
	calculateSlopeFactor();

	// No-wind no-slope spread rate and parameters
	noWindNoSlopeSpreadRate_ = calculateNoWindNoSlopeSpreadRate(reactionIntensity_, propagatingFlux, heatSink);
	forwardSpreadRate_ = noWindNoSlopeSpreadRate_;

	// Slope and wind adjusted spread rate
	calculateWindSpeedLimit();

	forwardSpreadRate_ = noWindNoSlopeSpreadRate_ * (1 + phiW_ + phiS_);

	// Calculate spread rate in optimal direction.
	calculateDirectionOfMaxSpread();
	calculateEffectiveWindSpeed();
	// maximum windspeed effect on ros
	if (effectiveWindSpeed_ > windSpeedLimit_)
	{
		applyWindSpeedLimit();
	}

	// Convert wind speeds to mi/hr
	effectiveWindSpeed_ /= 88.0; // effective wind speed is not in mi/hr
	windSpeedLimit_ /= 88.0; // wind speed limit is not in mi/hr

	calculateResidenceTime();

	// Calculate fire ellipse
	calculateFireLengthToWidthRatio();
	calculateSurfaceFireEccentricity();

	// If needed, calculate spread rate in arbitrary direction of interest
	if (directionOfInterest != -1.0)
	{
		forwardSpreadRate_ = calculateSpreadRateAtVector(directionOfInterest);
	}

	calculateFireFirelineIntensity();
	calculateFlameLength();

	const double FEET_PER_MIN_TO_CHAINS_PER_HOUR = 10.0 / 11.0; // conversion factor from ft/min to chains/hr
	forwardSpreadRate_ *= FEET_PER_MIN_TO_CHAINS_PER_HOUR;
	return forwardSpreadRate_;
}

//------------------------------------------------------------------------------
// \brief Calculates the fire spread rate at 'beta' degrees from the
//		direction of maximum spread.
//
//  \param directionOfInterest The direction for which the spread rate of the fire
//		is desired               
//
//  \return Fire spread rate along the specified vector (ft/min).

double SurfaceFireSpread::calculateSpreadRateAtVector(double directionOfInterest)
{
	if (surfaceInputs_->isWindAndSpreadAngleRelativeToNorth())
	{
		double slopeAspect = surfaceInputs_->getSlopeAspect();
		directionOfInterest -= slopeAspect + 180; // Direction of interest is now relative to north
	}

	double rosVector = forwardSpreadRate_;
	if (forwardSpreadRate_) // if forward spread rate is not zero
	{
		const double PI = 3.141592653589793238462643383279;
		// Calculate the fire spread rate in this azimuth
		// if it deviates more than a tenth degree from the maximum azimuth

		// Calcualte beta: the angle between the direction of max spread and the direction of interest
		double beta = fabs(directionOfMaxSpread_ - directionOfInterest);
		// Calculate the fire spread rate in this azimuth
		// if it deviates more than a tenth degree from the maximum azimuth
		if (beta > 180.0)
		{
			beta = (360.0 - beta);
		}
		if (fabs(beta) > 0.1)
		{
			double radians = beta * PI / 180.0;
			rosVector = forwardSpreadRate_ * (1.0 - eccentricity_) / (1.0 - eccentricity_ * cos(radians));
		}
	}
	return rosVector;
}

void SurfaceFireSpread::applyWindSpeedLimit()
{
	isWindLimitExceeded_ = true;
	effectiveWindSpeed_ = windSpeedLimit_;

	double relativePackingRatio = surfaceFuelbedIntermediates_->getRelativePackingRatio();
	double phiEffectiveWind = windC_ * pow(windSpeedLimit_, windB_) * pow(relativePackingRatio, -windE_);
	forwardSpreadRate_ = noWindNoSlopeSpreadRate_ * (1 + phiEffectiveWind);
}

void SurfaceFireSpread::calculateEffectiveWindSpeed()
{
	double phiEffectiveWind = forwardSpreadRate_ / noWindNoSlopeSpreadRate_ - 1.0;
	double relativePackingRatio = surfaceFuelbedIntermediates_->getRelativePackingRatio();
	effectiveWindSpeed_ = pow(((phiEffectiveWind * pow(relativePackingRatio, windE_)) / windC_), 1.0 / windB_);
}

void SurfaceFireSpread::calculateDirectionOfMaxSpread()
{
	const double PI = 3.141592653589793238462643383279;

	//Calculate directional components (direction is clockwise from upslope)
	double windDir = surfaceInputs_->getWindDirection();
	double windDirRadians = windDir * PI / 180.;

	// Calculate wind and slope rate
	double slopeRate = noWindNoSlopeSpreadRate_ * phiS_;
	double windRate = noWindNoSlopeSpreadRate_ * phiW_;

	// Calculate coordinate components
	double x = slopeRate + (windRate * cos(windDirRadians));
	double y = windRate * sin(windDirRadians);
	double rateVector = sqrt((x * x) + (y * y));

	// Apply wind and slope rate to spread rate
	forwardSpreadRate_ = noWindNoSlopeSpreadRate_ + rateVector;

	// Calculate azimuth
	double azimuth = 0;
	azimuth = atan2(y, x);

	// Recalculate azimuth in degrees
	azimuth *= 180.0 / PI;

	// If angle is negative, add 360 degrees
	if (azimuth < -1.0e-20)
	{
		azimuth += 360;
	}

	// Undocumented hack from BehavePlus code
	if (fabs(azimuth) < 0.5)
	{
		azimuth = 0.0;
	}

	// Azimuth is the direction of maximum spread
	directionOfMaxSpread_ = azimuth;
}

void  SurfaceFireSpread::calculateWindSpeedLimit()
{
	windSpeedLimit_ = 0.9 * reactionIntensity_; // windSpeedLimit is in ft/min
	if (phiS_ > 0.0)
	{
		if (phiS_ > windSpeedLimit_)
		{
			// can't have inifinite windspeed
			phiS_ = windSpeedLimit_;
		}
	}
}

void SurfaceFireSpread::calculateWindFactors()
{
	double sigma = surfaceFuelbedIntermediates_->getSigma();
	double relativePackingRatio = surfaceFuelbedIntermediates_->getRelativePackingRatio();
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc

	// Wind factors
	windC_ = 7.47 * exp(-0.133 * pow(sigma, 0.55));
	windB_ = 0.02526 * pow(sigma, 0.54);
	windE_ = 0.715 * exp(-0.000359*sigma);

	// Wind coefficient
	double windSpeedInFtPerMin = surfaceInputs_->getMidflameWindSpeed() * 88.0;		// ft/minute
	if (windSpeedInFtPerMin < SMIDGEN)
	{
		phiW_ = 0.0;
	}
	else
	{
		phiW_ = pow(windSpeedInFtPerMin, windB_) * windC_ * pow(relativePackingRatio, -windE_);
	}
}

void SurfaceFireSpread::calculateSlopeFactor()
{
	const double PI = 3.141592653589793238462643383279;
	double packingRatio = surfaceFuelbedIntermediates_->getPackingRatio();
	// Slope factor
	double slope = surfaceInputs_->getSlope();
	double slopex = tan((double)slope / 180.0 * PI); // convert from degrees to tan
	phiS_ = 5.275 * pow(packingRatio, -0.3) * (slopex * slopex);
}

void SurfaceFireSpread::calculateFireLengthToWidthRatio()
{
	if (effectiveWindSpeed_ > 1.0e-07)
	{
		fireLengthToWidthRatio_ = 1.0 + (0.25 * effectiveWindSpeed_);
	}
	else
	{
		fireLengthToWidthRatio_ = 1.0;
	}
}

void SurfaceFireSpread::calculateSurfaceFireEccentricity()
{
	eccentricity_ = 0;
	double x = (fireLengthToWidthRatio_ * fireLengthToWidthRatio_) - 1.0;
	if (x > 0.0)
	{
		eccentricity_ = sqrt(x) / fireLengthToWidthRatio_;
	}
}

void SurfaceFireSpread::calculateBackingSpreadRate()
{
	backingSpreadRate_ = forwardSpreadRate_ * (1.0 - eccentricity_) / (1.0 + eccentricity_);
}

double  SurfaceFireSpread::getDirectionOfMaxSpread() const
{
	double localDirMaxSpread = directionOfMaxSpread_;
	if (surfaceInputs_->isWindAndSpreadAngleRelativeToNorth())
	{
		localDirMaxSpread = convertDirectionOfSpreadToRelativeToNorth(localDirMaxSpread);
		while (localDirMaxSpread >= 360)
		{
			localDirMaxSpread -= 360;
		}
	}
	return localDirMaxSpread;
}

double  SurfaceFireSpread::convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const
{
	double dirMaxSpreadRelativeToNorth = directionOfMaxSpreadFromUpslope;
	double slopeAspect = surfaceInputs_->getSlopeAspect();
	dirMaxSpreadRelativeToNorth += slopeAspect + 180; // spread direction is now relative to north
	return dirMaxSpreadRelativeToNorth;
}

double SurfaceFireSpread::getFlameLength() const
{
	return flameLength_;
}

double SurfaceFireSpread::getFireLengthToWidthRatio() const
{
	return fireLengthToWidthRatio_;
}

double SurfaceFireSpread::getFireEccentricity() const
{
	return eccentricity_;
}

void SurfaceFireSpread::initializeMembers()
{
	isWindLimitExceeded_ = false;
	directionOfMaxSpread_ = 0.0;
	effectiveWindSpeed_ = 0.0;
	windSpeedLimit_ = 0.0;
	phiS_ = 0.0;
	phiW_ = 0.0;
	windB_ = 0.0;
	windC_ = 0.0;
	windE_ = 0.0;
	noWindNoSlopeSpreadRate_ = 0.0;
	forwardSpreadRate_ = 0.0;
	fireLengthToWidthRatio_ = 0.0;
	eccentricity_ = 0.0;
	residenceTime_ = 0.0;
	spreadRate_ = 0.0; // surface fire rate of spread in ft/min
	reactionIntensity_ = 0.0;
	firelineIntensity_ = 0.0;
}
