#include "surfaceFireSpread.h"

#include <cmath>

#include "fuelModelSet.h"
#include "surfaceEnums.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceInputs.h"
#include "windAdjustmentFactor.h"

static const double PI = 3.14159265358979323846;

SurfaceFireSpread::SurfaceFireSpread()
    : surfaceFireReactionIntensity_()
{

}

SurfaceFireSpread::SurfaceFireSpread(const FuelModelSet& fuelModelSet, const SurfaceInputs& surfaceInputs)
    : surfaceFuelbedIntermediates_(fuelModelSet, surfaceInputs),
      surfaceFireReactionIntensity_(surfaceFuelbedIntermediates_) 
{
    fuelModelSet_ = &fuelModelSet;
    surfaceInputs_ = &surfaceInputs;
    initializeMembers();
}

// Copy Ctor
SurfaceFireSpread::SurfaceFireSpread(const SurfaceFireSpread &rhs)
    : surfaceFireReactionIntensity_()
{
    surfaceFireReactionIntensity_ = rhs.surfaceFireReactionIntensity_;
    
    isWindLimitExceeded_ = rhs.isWindLimitExceeded_;
    effectiveWindSpeed_ = rhs.effectiveWindSpeed_;
    windSpeedLimit_ = rhs.windSpeedLimit_;
    phiS_ = rhs.phiS_;
    phiW_ = rhs.phiW_;
    windB_ = rhs.windB_;
    windC_ = rhs.windC_;
    windE_ = rhs.windE_;

    directionOfMaxSpread_ = rhs.directionOfMaxSpread_;
    noWindNoSlopeSpreadRate_ = rhs.noWindNoSlopeSpreadRate_;
    forwardSpreadRate_ = rhs.forwardSpreadRate_;
    heatPerUnitArea_ = rhs.heatPerUnitArea_;
    fireLengthToWidthRatio_ = rhs.fireLengthToWidthRatio_;
    eccentricity_ = rhs.eccentricity_;
    residenceTime_ = rhs.residenceTime_;
    reactionIntensity_ = rhs.reactionIntensity_;
    firelineIntensity_ = rhs.firelineIntensity_;
    flameLength_ = rhs.flameLength_;
    backingSpreadRate_ = rhs.backingSpreadRate_;

    midflameWindSpeed_ = rhs.midflameWindSpeed_;
    windAdjustmentFactor_ = rhs.windAdjustmentFactor_;
    windAdjustmentFactorMethod_ = rhs.windAdjustmentFactorMethod_;
    canopyCrownFraction_ = rhs.canopyCrownFraction_;

    aspenMortality_ = rhs.aspenMortality_;
}

SurfaceFireSpread& SurfaceFireSpread::operator= (const SurfaceFireSpread& rhs)
{
    if (this != &rhs)
    {
        surfaceFireReactionIntensity_ = rhs.surfaceFireReactionIntensity_;
   
        isWindLimitExceeded_ = rhs.isWindLimitExceeded_;
        effectiveWindSpeed_ = rhs.effectiveWindSpeed_;
        windSpeedLimit_ = rhs.windSpeedLimit_;
        phiS_ = rhs.phiS_;
        phiW_ = rhs.phiW_;
        windB_ = rhs.windB_;
        windC_ = rhs.windC_;
        windE_ = rhs.windE_;

        directionOfMaxSpread_ = rhs.directionOfMaxSpread_;
        noWindNoSlopeSpreadRate_ = rhs.noWindNoSlopeSpreadRate_;
        forwardSpreadRate_ = rhs.forwardSpreadRate_;
        heatPerUnitArea_ = rhs.heatPerUnitArea_;
        fireLengthToWidthRatio_ = rhs.fireLengthToWidthRatio_;
        eccentricity_ = rhs.eccentricity_;
        residenceTime_ = rhs.residenceTime_;
        reactionIntensity_ = rhs.reactionIntensity_;
        firelineIntensity_ = rhs.firelineIntensity_;
        flameLength_ = rhs.flameLength_;
        backingSpreadRate_ = rhs.backingSpreadRate_;

        midflameWindSpeed_ = rhs.midflameWindSpeed_;
        windAdjustmentFactor_ = rhs.windAdjustmentFactor_;
        windAdjustmentFactorMethod_ = rhs.windAdjustmentFactorMethod_;
        canopyCrownFraction_ = rhs.canopyCrownFraction_;

        aspenMortality_ = rhs.aspenMortality_;
    }
    return *this;
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
    double sigma = surfaceFuelbedIntermediates_.getSigma();
    residenceTime_ = ((sigma < 1.0e-07)
        ? (0.0)
        : (384. / sigma));
}

void SurfaceFireSpread::calculateFireFirelineIntensity()
{
    firelineIntensity_ = forwardSpreadRate_ * reactionIntensity_ * residenceTime_ / 60.0;
}

double  SurfaceFireSpread::calculateFlameLength(double firelineIntensity)
{
    double flameLength = ((firelineIntensity < 1.0e-07)
        ? (0.0)
        : (0.45 * pow(firelineIntensity, 0.46)));
    return flameLength;
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

    // Calculate fuelbed intermediates
    surfaceFuelbedIntermediates_.calculateFuelbedIntermediates();

    // Get needed fuelbed intermediates
    double sigma = surfaceFuelbedIntermediates_.getSigma();
    double packingRatio = surfaceFuelbedIntermediates_.getPackingRatio();
    double propagatingFlux = surfaceFuelbedIntermediates_.getPropagatingFlux();
    double heatSink = surfaceFuelbedIntermediates_.getHeatSink();
    reactionIntensity_ = surfaceFireReactionIntensity_.calculateReactionIntensity();

    // Calculate Wind and Slope Factors
    calculateMidflameWindSpeed();
    calculateWindFactor();
    calculateSlopeFactor();

    // No-wind no-slope spread rate and parameters
    noWindNoSlopeSpreadRate_ = calculateNoWindNoSlopeSpreadRate(reactionIntensity_, propagatingFlux, heatSink);
    forwardSpreadRate_ = noWindNoSlopeSpreadRate_;

    // Slope and wind adjusted spread rate
    calculateWindSpeedLimit();

    forwardSpreadRate_ = noWindNoSlopeSpreadRate_ * (1.0 + phiW_ + phiS_);

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
    calculateHeatPerUnitArea();

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
        double aspect = surfaceInputs_->getAspect();
        directionOfInterest -= aspect + 180.0; // Direction of interest is now relative to north
    }

    double rosVector = forwardSpreadRate_;
    if (forwardSpreadRate_) // if forward spread rate is not zero
    {
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

    double relativePackingRatio = surfaceFuelbedIntermediates_.getRelativePackingRatio();
    double phiEffectiveWind = windC_ * pow(windSpeedLimit_, windB_) * pow(relativePackingRatio, -windE_);
    forwardSpreadRate_ = noWindNoSlopeSpreadRate_ * (1 + phiEffectiveWind);
}

void SurfaceFireSpread::calculateEffectiveWindSpeed()
{
    double phiEffectiveWind = forwardSpreadRate_ / noWindNoSlopeSpreadRate_ - 1.0;
    double relativePackingRatio = surfaceFuelbedIntermediates_.getRelativePackingRatio();
    effectiveWindSpeed_ = pow(((phiEffectiveWind * pow(relativePackingRatio, windE_)) / windC_), 1.0 / windB_);
}

void SurfaceFireSpread::calculateDirectionOfMaxSpread()
{
    //Calculate directional components (direction is clockwise from upslope)
    double windDir = surfaceInputs_->getWindDirection();
    double windDirRadians = windDir * PI / 180.0;

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
    double azimuth = 0.0;
    azimuth = atan2(y, x);

    // Recalculate azimuth in degrees
    azimuth *= 180.0 / PI;

    // If angle is negative, add 360 degrees
    if (azimuth < -1.0e-20)
    {
        azimuth += 360.0;
    }

    // Undocumented hack from BehavePlus code
    if (fabs(azimuth) < 0.5)
    {
        azimuth = 0.0;
    }

    // Azimuth is the direction of maximum spread
    directionOfMaxSpread_ = azimuth;
}

void SurfaceFireSpread::calculateHeatPerUnitArea()
{
    heatPerUnitArea_ = reactionIntensity_ * residenceTime_;
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

void SurfaceFireSpread::calculateWindFactor()
{
    double sigma = surfaceFuelbedIntermediates_.getSigma();
    double relativePackingRatio = surfaceFuelbedIntermediates_.getRelativePackingRatio();
 
    windC_ = 7.47 * exp(-0.133 * pow(sigma, 0.55));
    windB_ = 0.02526 * pow(sigma, 0.54);
    windE_ = 0.715 * exp(-0.000359*sigma);

    double windSpeedInFtPerMin = midflameWindSpeed_ * 88.0;		// ft/minute
    if (windSpeedInFtPerMin < 1.0e-07)
    {
        phiW_ = 0.0;
    }
    else
    {
        phiW_ = pow(windSpeedInFtPerMin, windB_) * windC_ * pow(relativePackingRatio, -windE_);
    }
}

void SurfaceFireSpread::calculateWindAdjustmentFactor()
{
    WindAjustmentFactor windAdjustmentFactor;

    double canopyCover = surfaceInputs_->getCanopyCover();
    double canopyHeight = surfaceInputs_->getCanopyHeight();
    double crownRatio = surfaceInputs_->getCrownRatio();
    double fuelbedDepth = surfaceFuelbedIntermediates_.getFuelbedDepth();

    windAdjustmentFactor_ = windAdjustmentFactor.calculateWindAdjustmentFactor(canopyCover, canopyHeight, crownRatio, fuelbedDepth);
}

void SurfaceFireSpread::calculateMidflameWindSpeed()
{
    double windSpeed = surfaceInputs_->getWindSpeed();

    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = surfaceInputs_->getWindHeightInputMode();

    if (windHeightInputMode == WindHeightInputMode::DIRECT_MIDFLAME)
    {
        midflameWindSpeed_ = windSpeed;
    }
    else if (windHeightInputMode == WindHeightInputMode::TWENTY_FOOT || windHeightInputMode == WindHeightInputMode::TEN_METER)
    {
        if (windHeightInputMode == WindHeightInputMode::TEN_METER)
        {
            windSpeed /= 1.15;
        }
        if (surfaceInputs_->hasUserEnteredWindAdjustmentFactor())
        {
            windAdjustmentFactor_ = surfaceInputs_->getUserProvidedWindAdjustmentFactor();
        }
        else
        {
            calculateWindAdjustmentFactor();
        }
        midflameWindSpeed_ = windAdjustmentFactor_ * windSpeed;
    }
}

void SurfaceFireSpread::calculateSlopeFactor()
{
    double packingRatio = surfaceFuelbedIntermediates_.getPackingRatio();
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
    eccentricity_ = 0.0;
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

double SurfaceFireSpread::getFuelbedDepth() const
{
    int fuelModelNumber = surfaceInputs_->getFuelModelNumber();
    double fuelbedDepth = fuelModelSet_->getFuelbedDepth(fuelModelNumber);
    return fuelbedDepth;
}

double SurfaceFireSpread::getSpreadRate() const
{
    return forwardSpreadRate_;
}

double SurfaceFireSpread::getDirectionOfMaxSpread() const
{
    double localDirMaxSpread = directionOfMaxSpread_;
    if (surfaceInputs_->isWindAndSpreadAngleRelativeToNorth())
    {
        localDirMaxSpread = convertDirectionOfSpreadToRelativeToNorth(localDirMaxSpread);
        while (localDirMaxSpread >= 360.0)
        {
            localDirMaxSpread -= 360.0;
        }
    }
    return localDirMaxSpread;
}

double SurfaceFireSpread::getEffectiveWindSpeed() const
{
    return effectiveWindSpeed_;
}

double SurfaceFireSpread::convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const
{
    double dirMaxSpreadRelativeToNorth = directionOfMaxSpreadFromUpslope;
    double aspect = surfaceInputs_->getAspect();
    dirMaxSpreadRelativeToNorth += aspect + 180.0; // spread direction is now relative to north
    return dirMaxSpreadRelativeToNorth;
}

double SurfaceFireSpread::getFirelineIntensity() const
{
    return firelineIntensity_;
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

double SurfaceFireSpread::getHeatPerUnitArea() const
{
    return heatPerUnitArea_;
}

double  SurfaceFireSpread::getResidenceTime() const
{
    return residenceTime_;
}

double SurfaceFireSpread::getWindSpeedLimit() const
{
    return windSpeedLimit_;
}

double SurfaceFireSpread::getReactionIntensity() const
{
    return reactionIntensity_;
}

double SurfaceFireSpread::getMidflameWindSpeed() const
{
    return midflameWindSpeed_;
}

bool SurfaceFireSpread::getIsWindLimitExceeded() const
{
    return isWindLimitExceeded_;
}

void SurfaceFireSpread::setDirectionOfMaxSpread(double directionOFMaxSpread)
{
    directionOfMaxSpread_ = directionOFMaxSpread;
}

void SurfaceFireSpread::setEffectiveWindSpeed(double effectiveWindSpeed)
{
    effectiveWindSpeed_ = effectiveWindSpeed;
}

void SurfaceFireSpread::setFirelineIntensity(double firelineIntensity)
{
    firelineIntensity_ = firelineIntensity;
}

void SurfaceFireSpread::setFlameLength(double flameLength)
{
    flameLength_ = flameLength;
}

void SurfaceFireSpread::setFireLengthToWidthRatio(double lengthToWidthRatio)
{
    fireLengthToWidthRatio_ = lengthToWidthRatio;
}

void SurfaceFireSpread::setResidenceTime(double residenceTime)
{
    residenceTime_ = residenceTime;
}

void SurfaceFireSpread::setWindSpeedLimit(double windSpeedLimit)
{
    windSpeedLimit_ = windSpeedLimit;
}

void SurfaceFireSpread::setReactionIntensity(double reactionIntensity)
{
    reactionIntensity_ = reactionIntensity;
}

void SurfaceFireSpread::setHeatPerUnitArea(double heatPerUnitArea)
{
    heatPerUnitArea_ = heatPerUnitArea;
}

void SurfaceFireSpread::setIsWindLimitExceeded(bool isWindLimitExceeded)
{
    isWindLimitExceeded_ = isWindLimitExceeded;
}

void SurfaceFireSpread::setWindAdjustmentFactor(double windAdjustmentFactor)
{
    windAdjustmentFactor_ = windAdjustmentFactor;
}

void SurfaceFireSpread::setMidflameWindSpeed(double midflameWindSpeed)
{
    midflameWindSpeed_ = midflameWindSpeed;
}

void SurfaceFireSpread::initializeMembers()
{
    isWindLimitExceeded_ = false;
    effectiveWindSpeed_ = 0.0;
    windSpeedLimit_ = 0.0;
    phiS_ = 0.0;											
    phiW_ = 0.0;											
    windB_ = 0.0;
    windC_ = 0.0;
    windE_ = 0.0;
    directionOfMaxSpread_ = 0.0;
    noWindNoSlopeSpreadRate_ = 0.0;
    forwardSpreadRate_ = 0.0;
    heatPerUnitArea_ = 0.0;
    fireLengthToWidthRatio_ = 0.0;
    eccentricity_ = 0.0;
    residenceTime_ = 0.0;
    reactionIntensity_ = 0.0;
    firelineIntensity_ = 0.0;
    flameLength_ = 0.0;
    backingSpreadRate_ = 0.0;

    midflameWindSpeed_ = 0.0;
    windAdjustmentFactor_ = 0.0;
    windAdjustmentFactorMethod_ = WindAdjustmentFactorMethod::UNSHELTERED;
    canopyCrownFraction_ = 0.0;

    aspenMortality_ = 0.0;
}
