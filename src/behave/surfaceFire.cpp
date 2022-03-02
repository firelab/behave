/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating values associated with surface fires used 
*           in Rothermel Model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
******************************************************************************
*
* THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
* MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
* IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
* OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
* PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
* LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
* PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
* RELIABILITY, OR ANY OTHER CHARACTERISTIC.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
******************************************************************************/

#define _USE_MATH_DEFINES
#include <cmath>

#include "surfaceFire.h"
#include "surfaceFuelbedIntermediates.h"
#include "surfaceInputs.h"
#include "windAdjustmentFactor.h"

SurfaceFire::SurfaceFire()
    : surfaceFireReactionIntensity_()
{

}

SurfaceFire::SurfaceFire(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs, 
    FireSize& size)
    : surfaceFuelbedIntermediates_(fuelModels, surfaceInputs),
      surfaceFireReactionIntensity_(surfaceFuelbedIntermediates_) 
{
    fuelModels_ = &fuelModels;
    size_ = &size;
    surfaceInputs_ = &surfaceInputs;
    initializeMembers();
}

// Copy Ctor
SurfaceFire::SurfaceFire(const SurfaceFire& rhs)
    : surfaceFireReactionIntensity_()
{
    memberwiseCopyAssignment(rhs);
}

SurfaceFire& SurfaceFire::operator=(const SurfaceFire& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void SurfaceFire::memberwiseCopyAssignment(const SurfaceFire& rhs)
{
    surfaceFireReactionIntensity_ = rhs.surfaceFireReactionIntensity_;
    surfaceFuelbedIntermediates_ = rhs.surfaceFuelbedIntermediates_;

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
    spreadRateInDirectionOfInterest_ = rhs.spreadRateInDirectionOfInterest_;
    heatPerUnitArea_ = rhs.heatPerUnitArea_;
    fireLengthToWidthRatio_ = rhs.fireLengthToWidthRatio_;
    residenceTime_ = rhs.residenceTime_;
    reactionIntensity_ = rhs.reactionIntensity_;
    firelineIntensity_ = rhs.firelineIntensity_;
    flameLength_ = rhs.flameLength_;
    maxFlameLength_ = rhs.maxFlameLength_;
    backingSpreadRate_ = rhs.backingSpreadRate_;

    midflameWindSpeed_ = rhs.midflameWindSpeed_;
    windAdjustmentFactor_ = rhs.windAdjustmentFactor_;
    windAdjustmentFactorShelterMethod_ = rhs.windAdjustmentFactorShelterMethod_;
    canopyCrownFraction_ = rhs.canopyCrownFraction_;

    aspenMortality_ = rhs.aspenMortality_;
}

double SurfaceFire::calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink)
{
    noWindNoSlopeSpreadRate_ = (heatSink < 1.0e-07)
        ? (0.0)
        : (reactionIntensity * propagatingFlux / heatSink);
    return noWindNoSlopeSpreadRate_;
}

void SurfaceFire::calculateResidenceTime()
{
    double sigma = surfaceFuelbedIntermediates_.getSigma();
    residenceTime_ = ((sigma < 1.0e-07)
        ? (0.0)
        : (384. / sigma));
}

void SurfaceFire::calculateFireFirelineIntensity(double forwardSpreadRate)
{
    double secondsPerMinute = 60.0; // for converting feet per minute to feet per second
    firelineIntensity_ = forwardSpreadRate * reactionIntensity_ * (residenceTime_ / secondsPerMinute);
}

double  SurfaceFire::calculateFlameLength(double firelineIntensity)
{
    double flameLength = ((firelineIntensity < 1.0e-07)
        ? (0.0)
        : (0.45 * pow(firelineIntensity, 0.46)));
    return flameLength;
}

void SurfaceFire::skipCalculationForZeroLoad()
{
    initializeMembers();
}

void SurfaceFire::calculateFlameLength()
{
    // Byram 1959, Albini 1976
    flameLength_ = ((firelineIntensity_ < 1.0e-07)
        ? (0.0)
        : (0.45 * pow(firelineIntensity_, 0.46)));
}

double SurfaceFire::calculateForwardSpreadRate(int fuelModelNumber, bool hasDirectionOfInterest, double directionOfInterest)
{
    // Reset member variables to prepare for next calculation
    initializeMembers();

    // Calculate fuelbed intermediates
    surfaceFuelbedIntermediates_.calculateFuelbedIntermediates(fuelModelNumber);

    // Get needed fuelbed intermediates
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

    effectiveWindSpeed_ = SpeedUnits::fromBaseUnits(effectiveWindSpeed_, SpeedUnits::MilesPerHour);    
    calculateResidenceTime();

    double elapsedTime = surfaceInputs_->getElapsedTime();

    // Calculate fire ellipse and related properties
    size_->calculateFireBasicDimensions(effectiveWindSpeed_, SpeedUnits::MilesPerHour, forwardSpreadRate_, SpeedUnits::FeetPerMinute);

    fireLengthToWidthRatio_ = size_->getFireLengthToWidthRatio();
   
    backingSpreadRate_ = size_->getBackingSpreadRate(SpeedUnits::FeetPerMinute);

    calculateFireFirelineIntensity(forwardSpreadRate_);
    calculateFlameLength();
    maxFlameLength_ = getFlameLength(); // Used by SAFETY Module
    if (hasDirectionOfInterest) // If needed, calculate spread rate in arbitrary direction of interest
    {
        spreadRateInDirectionOfInterest_ = calculateSpreadRateAtVector(directionOfInterest);
        calculateFireFirelineIntensity(spreadRateInDirectionOfInterest_);
        calculateFlameLength();
    }

    calculateHeatPerUnitArea();

    return forwardSpreadRate_;
}

double SurfaceFire::calculateSpreadRateAtVector(double directionOfInterest)
{
    double rosVector = forwardSpreadRate_;
    double eccentricity_ = size_->getEccentricity();
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
            double radians = beta * M_PI / 180.0;
            rosVector = forwardSpreadRate_ * (1.0 - eccentricity_) / (1.0 - eccentricity_ * cos(radians));
        }
    }
    return rosVector;
}

void SurfaceFire::applyWindSpeedLimit()
{
    isWindLimitExceeded_ = true;
    effectiveWindSpeed_ = windSpeedLimit_;

    double relativePackingRatio = surfaceFuelbedIntermediates_.getRelativePackingRatio();
    double phiEffectiveWind = windC_ * pow(windSpeedLimit_, windB_) * pow(relativePackingRatio, -windE_);
    forwardSpreadRate_ = noWindNoSlopeSpreadRate_ * (1 + phiEffectiveWind);
}

void SurfaceFire::calculateEffectiveWindSpeed()
{
    double phiEffectiveWind = forwardSpreadRate_ / noWindNoSlopeSpreadRate_ - 1.0;
    double relativePackingRatio = surfaceFuelbedIntermediates_.getRelativePackingRatio();
    effectiveWindSpeed_ = pow(((phiEffectiveWind * pow(relativePackingRatio, windE_)) / windC_), 1.0 / windB_);
}

void SurfaceFire::calculateDirectionOfMaxSpread()
{
    //Calculate directional components (direction is clockwise from upslope)
    double correctedWindDirection = surfaceInputs_->getWindDirection();

    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientation = surfaceInputs_->getWindAndSpreadOrientationMode();
    if (windAndSpreadOrientation == WindAndSpreadOrientationMode::RelativeToNorth)
    {
        double aspect = surfaceInputs_->getAspect();
        correctedWindDirection -= aspect;
    }

    double windDirRadians = correctedWindDirection * M_PI / 180.0;

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
    azimuth *= 180.0 / M_PI;

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

    // Convert azimuth to be relative to North if necessary
    if (windAndSpreadOrientation == WindAndSpreadOrientationMode::RelativeToNorth)
    {
        azimuth = convertDirectionOfSpreadToRelativeToNorth(azimuth);
        while (azimuth >= 360.0)
        {
            azimuth -= 360.0;
        }
    }

    // Azimuth is the direction of maximum spread
    directionOfMaxSpread_ = azimuth;
}

void SurfaceFire::calculateHeatPerUnitArea()
{
    heatPerUnitArea_ = reactionIntensity_ * residenceTime_;
}

void  SurfaceFire::calculateWindSpeedLimit()
{
    windSpeedLimit_ = 0.9 * reactionIntensity_; 
    if (phiS_ > 0.0)
    {
        if (phiS_ > windSpeedLimit_)
        {
            // Enforce wind speed limit
            phiS_ = windSpeedLimit_;
        }
    }
}

void SurfaceFire::calculateWindFactor()
{
    double sigma = surfaceFuelbedIntermediates_.getSigma();
    double relativePackingRatio = surfaceFuelbedIntermediates_.getRelativePackingRatio();
 
    windC_ = 7.47 * exp(-0.133 * pow(sigma, 0.55));
    windB_ = 0.02526 * pow(sigma, 0.54);
    windE_ = 0.715 * exp(-0.000359*sigma);

    // midflameWindSpeed is in ft/min
    if (midflameWindSpeed_ < 1.0e-07) 
    {
        phiW_ = 0.0;
    }
    else
    {
        phiW_ = pow(midflameWindSpeed_, windB_) * windC_ * pow(relativePackingRatio, -windE_);
    }
}

void SurfaceFire::calculateWindAdjustmentFactor()
{
    WindAjustmentFactor windAdjustmentFactor;

    double canopyCover = surfaceInputs_->getCanopyCover();
    double canopyHeight = surfaceInputs_->getCanopyHeight();
    double crownRatio = surfaceInputs_->getCrownRatio();
    double fuelbedDepth = surfaceFuelbedIntermediates_.getFuelbedDepth();

    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod = 
        surfaceInputs_->getWindAdjustmentFactorCalculationMethod();
    if(windAdjustmentFactorCalculationMethod == WindAdjustmentFactorCalculationMethod::UseCrownRatio)
    {
        windAdjustmentFactor_ = windAdjustmentFactor.calculateWindAdjustmentFactorWithCrownRatio(canopyCover, canopyHeight, crownRatio, fuelbedDepth);
    }
    else if((windAdjustmentFactorCalculationMethod == WindAdjustmentFactorCalculationMethod::DontUseCrownRatio))
    {
        windAdjustmentFactor_ = windAdjustmentFactor.calculateWindAdjustmentFactorWithoutCrownRatio(canopyCover, canopyHeight, fuelbedDepth);
    }
    windAdjustmentFactorShelterMethod_ = windAdjustmentFactor.getWindAdjustmentFactorShelterMethod();
}

void SurfaceFire::calculateMidflameWindSpeed()
{
    double windSpeed = surfaceInputs_->getWindSpeed();

    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = surfaceInputs_->getWindHeightInputMode();

    if (windHeightInputMode == WindHeightInputMode::DirectMidflame)
    {
        midflameWindSpeed_ = windSpeed;
    }
    else if (windHeightInputMode == WindHeightInputMode::TwentyFoot || windHeightInputMode == WindHeightInputMode::TenMeter)
    {
        if (windHeightInputMode == WindHeightInputMode::TenMeter)
        {
            windSpeed /= 1.15;
        }
        WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod =
            surfaceInputs_->getWindAdjustmentFactorCalculationMethod();
        if (windAdjustmentFactorCalculationMethod == WindAdjustmentFactorCalculationMethod::UserInput)
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

void SurfaceFire::calculateSlopeFactor()
{
    double packingRatio = surfaceFuelbedIntermediates_.getPackingRatio();
    // Slope factor
    double slope = surfaceInputs_->getSlope();
    double slopex = tan((double)slope / 180.0 * M_PI); // convert from degrees to tan
    phiS_ = 5.275 * pow(packingRatio, -0.3) * (slopex * slopex);
}

double SurfaceFire::getFuelbedDepth() const
{
    int fuelModelNumber = surfaceInputs_->getFuelModelNumber();
    double fuelbedDepth = fuelModels_->getFuelbedDepth(fuelModelNumber, LengthUnits::Feet);
    return fuelbedDepth;
}

double SurfaceFire::getSpreadRate() const
{
    return forwardSpreadRate_;
}

double SurfaceFire::getSpreadRateInDirectionOfInterest() const
{
    return spreadRateInDirectionOfInterest_;
}

double SurfaceFire::getDirectionOfMaxSpread() const
{
    return directionOfMaxSpread_;
}

double SurfaceFire::getEffectiveWindSpeed() const
{
    return effectiveWindSpeed_;
}

double SurfaceFire::convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const
{
    double dirMaxSpreadRelativeToNorth = directionOfMaxSpreadFromUpslope;
    double aspect = surfaceInputs_->getAspect();
    dirMaxSpreadRelativeToNorth += aspect + 180.0; // spread direction is now relative to north
    return dirMaxSpreadRelativeToNorth;
}

double SurfaceFire::getFirelineIntensity() const
{
    return firelineIntensity_;
}

double SurfaceFire::getFlameLength() const
{
    return flameLength_;
}

double SurfaceFire::getMaxFlameLength() const
{
    return maxFlameLength_;
}

double SurfaceFire::getFireLengthToWidthRatio() const
{
    return size_->getFireLengthToWidthRatio();
}

double SurfaceFire::getFireEccentricity() const
{
    return  size_->getEccentricity();
}

double SurfaceFire::getHeatPerUnitArea() const
{
    return heatPerUnitArea_;
}

double  SurfaceFire::getResidenceTime() const
{
    return residenceTime_;
}

double SurfaceFire::getWindSpeedLimit() const
{
    return windSpeedLimit_;
}

double SurfaceFire::getMidflameWindSpeed() const
{
    return midflameWindSpeed_;
}

double SurfaceFire::getSlopeFactor() const
{
    return phiS_;
}

double SurfaceFire::getHeatSink() const
{
    return surfaceFuelbedIntermediates_.getHeatSink();
}

double SurfaceFire::getBulkDensity() const
{
    return surfaceFuelbedIntermediates_.getBulkDensity();
}

double SurfaceFire::getReactionIntensity() const
{
    return reactionIntensity_;
}

double SurfaceFire::getWindAdjustmentFactor() const
{
    return windAdjustmentFactor_;
}

bool SurfaceFire::getIsWindLimitExceeded() const
{
    return isWindLimitExceeded_;
}

void SurfaceFire::setDirectionOfMaxSpread(double directionOFMaxSpread)
{
    directionOfMaxSpread_ = directionOFMaxSpread;
}

void SurfaceFire::setEffectiveWindSpeed(double effectiveWindSpeed)
{
    effectiveWindSpeed_ = effectiveWindSpeed;
}

void SurfaceFire::setFirelineIntensity(double firelineIntensity)
{
    firelineIntensity_ = firelineIntensity;
}

void SurfaceFire::setFlameLength(double flameLength)
{
    flameLength_ = flameLength;
}

void SurfaceFire::setFireLengthToWidthRatio(double lengthToWidthRatio)
{
    fireLengthToWidthRatio_ = lengthToWidthRatio;
}

void SurfaceFire::setResidenceTime(double residenceTime)
{
    residenceTime_ = residenceTime;
}

void SurfaceFire::setWindSpeedLimit(double windSpeedLimit)
{
    windSpeedLimit_ = windSpeedLimit;
}

void SurfaceFire::setReactionIntensity(double reactionIntensity)
{
    reactionIntensity_ = reactionIntensity;
}

void SurfaceFire::setHeatPerUnitArea(double heatPerUnitArea)
{
    heatPerUnitArea_ = heatPerUnitArea;
}

void SurfaceFire::setIsWindLimitExceeded(bool isWindLimitExceeded)
{
    isWindLimitExceeded_ = isWindLimitExceeded;
}

void SurfaceFire::setWindAdjustmentFactor(double windAdjustmentFactor)
{
    windAdjustmentFactor_ = windAdjustmentFactor;
}

void SurfaceFire::setMidflameWindSpeed(double midflameWindSpeed)
{
    midflameWindSpeed_ = midflameWindSpeed;
}

void SurfaceFire::initializeMembers()
{
    isWindLimitExceeded_ = false;
    effectiveWindSpeed_ = 0.0;
    windSpeedLimit_ = 0.0;
    phiS_ = 0.0;											
    phiW_ = 0.0;											
    windB_ = 0.0;
    windC_ = 0.0;
    windE_ = 0.0;
    directionOfInterest_ = 0.0;
    directionOfMaxSpread_ = 0.0;
    noWindNoSlopeSpreadRate_ = 0.0;
    forwardSpreadRate_ = 0.0;
    heatPerUnitArea_ = 0.0;
    fireLengthToWidthRatio_ = 1.0;
    residenceTime_ = 0.0;
    reactionIntensity_ = 0.0;
    firelineIntensity_ = 0.0;
    flameLength_ = 0.0;
    maxFlameLength_ = 0.0;
    backingSpreadRate_ = 0.0;

    midflameWindSpeed_ = 0.0;
    windAdjustmentFactor_ = 0.0;
    windAdjustmentFactorShelterMethod_ = WindAdjustmentFactorShelterMethod::Unsheltered;
    canopyCrownFraction_ = 0.0;

    aspenMortality_ = 0.0;
}
