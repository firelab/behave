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
    backingSpreadRate_ = 0.0;
    flankingSpreadRate_ = 0.0;
    spreadRateInDirectionOfInterest_ = 0.0;
    heatPerUnitArea_ = 0.0;
    fireLengthToWidthRatio_ = 1.0;
    residenceTime_ = 0.0;
    reactionIntensity_ = 0.0;
    forwardFirelineIntensity_ = 0.0;
    backingFirelineIntensity_ = 0.0;
    flankingFirelineIntensity_ = 0.0;
    directionOfInterestFirelineIntensity_ = 0.0;
    forwardFlameLength_ = 0.0;
    backingFlameLength_ = 0.0;
    flankingFlameLength_ = 0.0;
    directionOfInterestFlameLength_ = 0.0;
    maxFlameLength_ = 0.0;
    scorchHeight_ = 0.0;

    midflameWindSpeed_ = 0.0;
    windAdjustmentFactor_ = 0.0;
    windAdjustmentFactorShelterMethod_ = WindAdjustmentFactorShelterMethod::Unsheltered;
    canopyCrownFraction_ = 0.0;

    surfaceFuelbedIntermediates_ = SurfaceFuelbedIntermediates(*fuelModels_, *surfaceInputs_);
    surfaceFireReactionIntensity_ = SurfaceFireReactionIntensity(surfaceFuelbedIntermediates_);
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
    backingSpreadRate_ = rhs.backingSpreadRate_;
    flankingSpreadRate_ = rhs.flankingSpreadRate_;
    spreadRateInDirectionOfInterest_ = rhs.spreadRateInDirectionOfInterest_;
    heatPerUnitArea_ = rhs.heatPerUnitArea_;
    fireLengthToWidthRatio_ = rhs.fireLengthToWidthRatio_;
    residenceTime_ = rhs.residenceTime_;
    reactionIntensity_ = rhs.reactionIntensity_;
    forwardFirelineIntensity_ = rhs.forwardFirelineIntensity_;
    backingFirelineIntensity_ = rhs.backingFirelineIntensity_;
    flankingFirelineIntensity_ = rhs.flankingFirelineIntensity_;
    directionOfInterestFirelineIntensity_ = rhs.directionOfInterestFirelineIntensity_;
    forwardFlameLength_ = rhs.forwardFlameLength_;
    backingFlameLength_ = rhs.backingFlameLength_;
    flankingFlameLength_ = rhs.flankingFlameLength_;
    directionOfInterestFlameLength_ = rhs.directionOfInterestFlameLength_;
    maxFlameLength_ = rhs.maxFlameLength_;
    scorchHeight_ = rhs.scorchHeight_;

    midflameWindSpeed_ = rhs.midflameWindSpeed_;
    windAdjustmentFactor_ = rhs.windAdjustmentFactor_;
    windAdjustmentFactorShelterMethod_ = rhs.windAdjustmentFactorShelterMethod_;
    canopyCrownFraction_ = rhs.canopyCrownFraction_;
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

double SurfaceFire::calculateFirelineIntensity(double spreadRate,
                                               SpeedUnits::SpeedUnitsEnum speedUnits,
                                               FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits)
{
    double secondsPerMinute = 60.0; // for converting feet per minute to feet per second
    double firelineIntensity = SpeedUnits::toBaseUnits(spreadRate, speedUnits) * reactionIntensity_ * (residenceTime_ / secondsPerMinute);
    return FirelineIntensityUnits::fromBaseUnits(firelineIntensity, firelineIntensityUnits);
}

void SurfaceFire::calculateFirelineIntensities()
{
    forwardFirelineIntensity_ = calculateFirelineIntensity(forwardSpreadRate_,
                                                           SpeedUnits::FeetPerMinute,
                                                           FirelineIntensityUnits::BtusPerFootPerSecond);

    backingFirelineIntensity_ = calculateFirelineIntensity(backingSpreadRate_,
                                                           SpeedUnits::FeetPerMinute,
                                                           FirelineIntensityUnits::BtusPerFootPerSecond);

    flankingFirelineIntensity_ = calculateFirelineIntensity(flankingSpreadRate_,
                                                            SpeedUnits::FeetPerMinute,
                                                            FirelineIntensityUnits::BtusPerFootPerSecond);

    directionOfInterestFirelineIntensity_ = calculateFirelineIntensity(spreadRateInDirectionOfInterest_,
                                                                       SpeedUnits::FeetPerMinute,
                                                                       FirelineIntensityUnits::BtusPerFootPerSecond);
}

void SurfaceFire::skipCalculationForZeroLoad()
{
    initializeMembers();
}

double SurfaceFire::calculateFlameLength(double firelineIntensity,
                                         FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits,
                                         LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    firelineIntensity = FirelineIntensityUnits::toBaseUnits(firelineIntensity, firelineIntensityUnits);


    // Byram 1959, Albini 1976
    double flameLength = ((firelineIntensity < 1.0e-07)
                          ? (0.0)
                          : (0.45 * pow(firelineIntensity, 0.46)));
    return LengthUnits::fromBaseUnits(flameLength, flameLengthUnits);
}

void SurfaceFire::calculateFlameLengths()
{
    forwardFlameLength_ = calculateFlameLength(forwardFirelineIntensity_,
                                               FirelineIntensityUnits::BtusPerFootPerSecond,
                                               LengthUnits::Feet);

    backingFlameLength_ = calculateFlameLength(backingFirelineIntensity_,
                                               FirelineIntensityUnits::BtusPerFootPerSecond,
                                               LengthUnits::Feet);

    flankingFlameLength_ = calculateFlameLength(flankingFirelineIntensity_,
                                                FirelineIntensityUnits::BtusPerFootPerSecond,
                                                LengthUnits::Feet);

    directionOfInterestFlameLength_ = calculateFlameLength(directionOfInterestFirelineIntensity_,
                                                           FirelineIntensityUnits::BtusPerFootPerSecond,
                                                           LengthUnits::Feet);
}

void SurfaceFire::calculateScorchHeight()
{
    const double airTemperature = surfaceInputs_->getAirTemperature(TemperatureUnits::Fahrenheit);
    const double windSpeed = surfaceInputs_->getWindSpeed(SpeedUnits::MilesPerHour);
    scorchHeight_ = ((forwardFirelineIntensity_ < 1.0e-07)
        ? (0.0)
        : ((63.0 / (140.0 - airTemperature))
            * pow(forwardFirelineIntensity_, 1.166667)
            / sqrt(forwardFirelineIntensity_ + (windSpeed * windSpeed * windSpeed))
            ));
}

double SurfaceFire::calculateForwardSpreadRate(int fuelModelNumber, bool hasDirectionOfInterest, double directionOfInterest, SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode)
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

    effectiveWindSpeed_ = SpeedUnits::fromBaseUnits(effectiveWindSpeed_, SpeedUnits::FeetPerMinute);
    calculateResidenceTime();

    // Calculate fire ellipse and related properties
    size_->calculateFireBasicDimensions(false, effectiveWindSpeed_, SpeedUnits::FeetPerMinute, forwardSpreadRate_, SpeedUnits::FeetPerMinute);

    fireLengthToWidthRatio_ = size_->getFireLengthToWidthRatio();

    backingSpreadRate_ = size_->getBackingSpreadRate(SpeedUnits::FeetPerMinute);
    flankingSpreadRate_ = size_->getFlankingSpreadRate(SpeedUnits::FeetPerMinute);

    if (hasDirectionOfInterest) // If needed, calculate spread rate in arbitrary direction of interest
    {
        spreadRateInDirectionOfInterest_ = calculateSpreadRateAtVector(directionOfInterest, directionMode);
    }
    else
    {
        spreadRateInDirectionOfInterest_ = forwardSpreadRate_;
    }

    calculateHeatPerUnitArea();
    calculateFirelineIntensities();
    calculateFlameLengths();

    bool isUsingWesternAspen = surfaceInputs_->getIsUsingWesternAspen();
    if (isUsingWesternAspen)
    {
        surfaceFuelbedIntermediates_.calculateWesternAspenMortality(forwardFlameLength_);
    }

    maxFlameLength_ = forwardFlameLength_; // Used by SAFETY Module
    calculateHeatSource();

    if (hasDirectionOfInterest)
    {
        return spreadRateInDirectionOfInterest_;
    }
    else
    {
        return forwardSpreadRate_;
    }
}

double SurfaceFire::calculateSpreadRateAtVector(double directionOfInterest, SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode)
{
    // Constrain direction of interest to range of [0, 359] degrees
    while(directionOfInterest < 0.0)
    {
        directionOfInterest += 360.0;
    }
    while(directionOfInterest >= 360.0)
    {
        directionOfInterest -= 360.0;
    }

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

        double radians = beta * M_PI / 180.0;

        // Equation for spread rate at angle psi along elliptical perimeter,  Catchpole et al. (1982)
        // L = forwardSpreadDistance + backingSpreadDistance
        // f = L/2
        // g = forwardSpreadDistance – f
        // h = flankingSpreadDistance
        // Rpsi = (g * cos(psi)) + sqrt((f^2 * cos^2(psi)) + (h^2 sin^2(psi)))
        double L = forwardSpreadRate_ + backingSpreadRate_;
        double f = L / 2.0;
        double g = forwardSpreadRate_ - f;
        double h = size_->getFlankingSpreadRate(SpeedUnits::FeetPerMinute);
        double cosBeta = cos(radians);
        double sinBeta = sin(radians);

        // rosVector perpendicular to perimeter at angle beta used to calculate fireline intensity and flame length
        rosVector = (g * cos(radians)) + sqrt((f * f * cosBeta * cosBeta) + (h * h * sinBeta * sinBeta));

        if (directionMode == SurfaceFireSpreadDirectionMode::FromIgnitionPoint)
        {
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

    double canopyCover = surfaceInputs_->getCanopyCover(FractionUnits::Fraction);
    double canopyHeight = surfaceInputs_->getCanopyHeight(LengthUnits::Feet);
    double crownRatio = surfaceInputs_->getCrownRatio(FractionUnits::Fraction);
    double fuelbedDepth = surfaceFuelbedIntermediates_.getFuelbedDepth();

    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod =
        surfaceInputs_->getWindAdjustmentFactorCalculationMethod();
    if(windAdjustmentFactorCalculationMethod == WindAdjustmentFactorCalculationMethod::UseCrownRatio)
    {
        windAdjustmentFactor_ = windAdjustmentFactor.calculateWindAdjustmentFactorWithCrownRatio(canopyCover, canopyHeight, crownRatio, fuelbedDepth);
    }
    else if(windAdjustmentFactorCalculationMethod == WindAdjustmentFactorCalculationMethod::DontUseCrownRatio)
    {
        windAdjustmentFactor_ = windAdjustmentFactor.calculateWindAdjustmentFactorWithoutCrownRatio(canopyCover, canopyHeight, fuelbedDepth);
    }
    windAdjustmentFactorShelterMethod_ = windAdjustmentFactor.getWindAdjustmentFactorShelterMethod();
}

void SurfaceFire::calculateMidflameWindSpeed()
{
    double windSpeed = surfaceInputs_->getWindSpeed(SpeedUnits::FeetPerMinute);

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
    double slope = surfaceInputs_->getSlope(SlopeUnits::Degrees);
    double slopex = tan((double)slope / 180.0 * M_PI); // convert from degrees to tan
    phiS_ = 5.275 * pow(packingRatio, -0.3) * (slopex * slopex);
}

void SurfaceFire::calculateHeatSource()
{
    double propagatingFlux = surfaceFuelbedIntermediates_.getPropagatingFlux();
    heatSource_ = reactionIntensity_ * propagatingFlux * (1.0 + phiS_ + phiW_);
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
    return forwardFirelineIntensity_;
}

double SurfaceFire::getBackingFirelineIntensity() const
{
  return backingFirelineIntensity_;
}

double SurfaceFire::getFlankingFirelineIntensity() const
{
  return flankingFirelineIntensity_;
}

double SurfaceFire::getFirelineIntensityInDirectionOfInterest() const
{
    return directionOfInterestFirelineIntensity_;
}

double SurfaceFire::getFlameLength() const
{
    return forwardFlameLength_;
}

double SurfaceFire::getBackingFlameLength() const
{
  return backingFlameLength_;
}

double SurfaceFire::getFlankingFlameLength() const
{
  return flankingFlameLength_;
}

double SurfaceFire::getFlameLengthInDirectionOfInterest() const
{
    return directionOfInterestFlameLength_;
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

double SurfaceFire::getHeatSource() const
{
    return heatSource_;
}

double SurfaceFire::getBulkDensity() const
{
    return surfaceFuelbedIntermediates_.getBulkDensity();
}

double SurfaceFire::getReactionIntensity() const
{
    return reactionIntensity_;
}

double SurfaceFire::getSurfaceFireReactionIntensityForLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return surfaceFireReactionIntensity_.getSurfaceFireReactionIntensityForLifeState(lifeState);
}

double SurfaceFire::getMoistureOfExtinctionByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return surfaceFuelbedIntermediates_.getMoistureOfExtinctionByLifeState(lifeState);
}

double SurfaceFire::getWeightedMoistureByLifeState(FuelLifeState::FuelLifeStateEnum lifeState) const
{
    return surfaceFuelbedIntermediates_.getWeightedMoistureByLifeState(lifeState);
}

double SurfaceFire::getWindAdjustmentFactor() const
{
    return windAdjustmentFactor_;
}

double SurfaceFire::getCharacteristicSAVR() const
{
    return surfaceFuelbedIntermediates_.getSigma();
}

bool SurfaceFire::getIsWindLimitExceeded() const
{
    return isWindLimitExceeded_;
}

double SurfaceFire::getRelativePackingRatio() const {
  return surfaceFuelbedIntermediates_.getRelativePackingRatio();
}

double SurfaceFire::getPackingRatio() const {
  return surfaceFuelbedIntermediates_.getPackingRatio();
}

double SurfaceFire::getPalmettoGallberryMoistureOfExtinctionDead() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberryMoistureOfExtinctionDead();
}

double SurfaceFire::getPalmettoGallberryHeatOfCombustionDead() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberryHeatOfCombustionDead();
}

double SurfaceFire::getPalmettoGallberryHeatOfCombustionLive() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberryHeatOfCombustionLive();
}

double SurfaceFire::getPalmettoGallberyDeadOneHourLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyDeadFineFuelLoad();
}

double SurfaceFire::getPalmettoGallberyDeadTenHourLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyDeadMediumFuelLoad();
}

double SurfaceFire::getPalmettoGallberyDeadFoliageLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyDeadFoliageLoad();
}

double SurfaceFire::getPalmettoGallberyFuelBedDepth() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyFuelBedDepth();
}

double SurfaceFire::getPalmettoGallberyLitterLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyLitterLoad();
}

double SurfaceFire::getPalmettoGallberyLiveOneHourLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyLiveFineFuelLoad();
}

double SurfaceFire::getPalmettoGallberyLiveTenHourLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyLiveTenMediumFuelLoad();
}

double SurfaceFire::getPalmettoGallberyLiveFoliageLoad() const
{
    return surfaceFuelbedIntermediates_.getPalmettoGallberyLiveFoliageLoad();
}

double SurfaceFire::getAspenMortality() const
{
    return surfaceFuelbedIntermediates_.getAspenMortality();
}

double SurfaceFire::getAspenFuelBedDepth(int typeIndex) const
{
    return surfaceFuelbedIntermediates_.getAspenFuelBedDepth(typeIndex);
}

double SurfaceFire::getAspenHeatOfCombustionDead() const
{
    return surfaceFuelbedIntermediates_.getAspenHeatOfCombustionDead();
}

double SurfaceFire::getAspenHeatOfCombustionLive() const
{
    return surfaceFuelbedIntermediates_.getAspenHeatOfCombustionLive();
}

double SurfaceFire::getAspenMoistureOfExtinctionDead() const
{
    return surfaceFuelbedIntermediates_.getAspenMoistureOfExtinctionDead();
}

double SurfaceFire::getAspenLoadDeadOneHour() const
{
    return surfaceFuelbedIntermediates_.getAspenLoadDeadOneHour();
}

double SurfaceFire::getAspenLoadDeadTenHour() const
{
    return surfaceFuelbedIntermediates_.getAspenLoadDeadTenHour();
}

double SurfaceFire::getAspenLoadLiveHerbaceous() const
{
    return surfaceFuelbedIntermediates_.getAspenLoadLiveHerbaceous();
}

double SurfaceFire::getAspenLoadLiveWoody() const
{
    return surfaceFuelbedIntermediates_.getAspenLoadLiveWoody();
}

double SurfaceFire::getAspenSavrDeadOneHour() const
{
    return surfaceFuelbedIntermediates_.getAspenSavrDeadOneHour();
}

double SurfaceFire::getAspenSavrDeadTenHour() const
{
    return surfaceFuelbedIntermediates_.getAspenSavrDeadTenHour();
}

double SurfaceFire::getAspenSavrLiveHerbaceous() const
{
    return surfaceFuelbedIntermediates_.getAspenSavrLiveHerbaceous();
}

double SurfaceFire::getAspenSavrLiveWoody() const
{
    return surfaceFuelbedIntermediates_.getAspenSavrLiveWoody();
}

double SurfaceFire::getChaparralAge() const
{
    return surfaceFuelbedIntermediates_.getChaparralAge();
}

double SurfaceFire::getChaparralDaysSinceMayFirst() const
{
    return surfaceFuelbedIntermediates_.getChaparralDaysSinceMayFirst();
}

double SurfaceFire::getChaparralDeadFuelFraction() const
{
    return surfaceFuelbedIntermediates_.getChaparralDeadFuelFraction();
}

double SurfaceFire::getChaparralDeadMoistureOfExtinction() const
{
    return surfaceFuelbedIntermediates_.getChaparralDeadMoistureOfExtinction();
}

double SurfaceFire::getChaparralLiveMoistureOfExtinction() const
{
    return surfaceFuelbedIntermediates_.getChaparralLiveMoistureOfExtinction();
}

double SurfaceFire::getChaparralDensity(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralDensity(lifeState, sizeClass);
}

double SurfaceFire::getChaparralFuelBedDepth() const
{
    return surfaceFuelbedIntermediates_.getChaparralFuelBedDepth();
}

double SurfaceFire::getChaparralHeatOfCombustion(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralHeatOfCombustion(lifeState, sizeClass);
}

double SurfaceFire::getChaparralLoad(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralLoad(lifeState, sizeClass);
}

double SurfaceFire::getChaparralMoisture(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralMoisture(lifeState, sizeClass);
}

double SurfaceFire::getChaparralSavr(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralSavr(lifeState, sizeClass);
}

double SurfaceFire::getChaparralEffectiveSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralEffectiveSilicaContent(lifeState, sizeClass);
}

double SurfaceFire::getChaparralTotalSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return surfaceFuelbedIntermediates_.getChaparralTotalSilicaContent(lifeState, sizeClass);
}

double SurfaceFire::getChaparralTotalDeadFuelLoad() const
{
    return surfaceFuelbedIntermediates_.getChaparralTotalDeadFuelLoad();
}

double SurfaceFire::getChaparralTotalFuelLoad() const
{
    return surfaceFuelbedIntermediates_.getChaparralTotalFuelLoad();
}

double SurfaceFire::getChaparralTotalLiveFuelLoad() const
{
    return surfaceFuelbedIntermediates_.getChaparralTotalLiveFuelLoad();
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
    forwardFirelineIntensity_ = firelineIntensity;
}

void SurfaceFire::setFlameLength(double flameLength)
{
    forwardFlameLength_ = flameLength;
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
