/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Part of Mark Finney's EXRATE package for determining expected
*           and harmonic mean spread rate in randomly arranged fuels
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 and EXRATE source originally authored by Collin D.
*           Bevins and Mark Finney respectively, and is used with or without
*           modification.
*
*******************************************************************************
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

#include "surfaceTwoFuelModels.h"

#include "newext.h"
#include "randfuel.h"
#include "randthread.h"
#include "surfaceFire.h"
#include "surfaceFuelbedIntermediates.h"

SurfaceTwoFuelModels::SurfaceTwoFuelModels(SurfaceFire& surfaceFireSpread)
{
    surfaceFireSpread_ = &surfaceFireSpread;
}

bool SurfaceTwoFuelModels::getWindLimitExceeded() const
{
    return windLimitExceeded_;
}

double SurfaceTwoFuelModels::getReactionIntensity() const
{
    return reactionIntensity_;
}

double SurfaceTwoFuelModels::getSpreadRate() const
{
    return spreadRate_;
}

double SurfaceTwoFuelModels::getDirectionOfMaxSpread() const
{
    return directionOfMaxSpread_;
}

double SurfaceTwoFuelModels::getEffectiveWind() const
{
    return effectiveWind_;
}

double SurfaceTwoFuelModels::getFuelbedDepth() const
{
    return fuelbedDepth_;
}

double SurfaceTwoFuelModels::getHeatPerUnitArea() const
{
    return heatPerUnitArea_;
}

double SurfaceTwoFuelModels::getMidFlameWindSpeed() const
{
    return midFlameWindSpeed_;
}

double SurfaceTwoFuelModels::getWindSpeedLimit() const
{
    return windSpeedLimit_;
}

double SurfaceTwoFuelModels::WindAdjustmentFactor() const
{
    return windAdjustmentFactor_;
}

double SurfaceTwoFuelModels::getFireLineIntensity() const
{
    return fireLineIntensity_;
}

double SurfaceTwoFuelModels::getflameLength() const
{
    return flameLength_;
}

double SurfaceTwoFuelModels::getFireLengthToWidthRatio() const
{
    return fireLengthToWidthRatio_;
}

void SurfaceTwoFuelModels::calculateWeightedSpreadRate(TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
    int firstFuelModelNumber, double firstFuelModelCoverage, int secondFuelModelNumber,
    bool hasDirectionOfInterest, double directionOfInterest)
{   
    fuelModelNumber_[SurfaceInputs::TwoFuelModelsContants::FIRST] = firstFuelModelNumber;
    fuelModelNumber_[SurfaceInputs::TwoFuelModelsContants::SECOND] = secondFuelModelNumber;

    coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] = firstFuelModelCoverage;
    coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND] = 1 - coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST];

    // Calculate fire outputs for each fuel model
    calculateFireOutputsForEachModel(hasDirectionOfInterest, directionOfInterest);
    
    //------------------------------------------------
    // Determine and store combined fuel model outputs
    //------------------------------------------------
    // Fire spread rate depends upon the weighting method...
    twoFuelModelsMethod_ = twoFuelModelsMethod;
    calculateSpreadRateBasedOnMethod();

    // The following assignments are based on Pat's rules:
    // If only 1 fuel is present (whether primary or secondary), use its values exclusively
    if (coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > 0.999 || coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND] > 0.999)
    {
        int i = (coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > 0.999) ? 0 : 1;
        reactionIntensity_ = reactionIntensityForFuelModel_[i];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        directionOfMaxSpread_ = dirMaxSpreadForFuelModel_[i];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        windAdjustmentFactor_ = windAdjustmentFactorForFuelModel_[i];
        surfaceFireSpread_->setWindAdjustmentFactor(windAdjustmentFactor_);

        midFlameWindSpeed_ = midFlameWindSpeedForFuelModel_[i];
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        effectiveWind_ = effectiveWindSpeedForFuelModel_[i];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        windSpeedLimit_ = windSpeedLimitForFuelModel_[i];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        windLimitExceeded_ = windLimitExceededForFuelModel_[i];
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        fireLengthToWidthRatio_ = lengthToWidthRatioForFuelModel_[i];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        heatPerUnitArea_ = heatPerUnitAreaForFuelModel_[i];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        fireLineIntensity_ = firelineIntensityForFuelModel_[i];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        flameLength_ = flameLengthForFuelModel_[i];
        surfaceFireSpread_->setFlameLength(flameLength_);

        fuelbedDepth_ = fuelbedDepthForFuelModel_[i];
    }
    // Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
    else
    {
        // Reaction intensity is the maximum of the two models
        reactionIntensity_ = (reactionIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] >
            reactionIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            reactionIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : reactionIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND];
        surfaceFireSpread_->setReactionIntensity(reactionIntensity_);

        // Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
        directionOfMaxSpread_ = dirMaxSpreadForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST];
        surfaceFireSpread_->setDirectionOfMaxSpread(directionOfMaxSpread_);

        // Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
        windAdjustmentFactor_ = windAdjustmentFactorForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST]; // TODO: Incorporate Wind Adjustment Factor model in Behave
        //		surfaceFireSpread_->setWindAdjustmentFactor[windAdjustmentFactor_];

        // Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
        midFlameWindSpeed_ = midFlameWindSpeedForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST]; // TODO:  Incorporate Wind Speed at Midflame model in Behave
        surfaceFireSpread_->setMidflameWindSpeed(midFlameWindSpeed_);

        // Effective wind speed is for the FIRST (not necessarily dominant) fuel model
        effectiveWind_ = effectiveWindSpeedForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST];
        surfaceFireSpread_->setEffectiveWindSpeed(effectiveWind_);

        // Maximum reliable wind speed is the minimum of the two models
        windSpeedLimit_ = (windSpeedLimitForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] < windSpeedLimitForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            windSpeedLimitForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : windSpeedLimitForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND];
        surfaceFireSpread_->setWindSpeedLimit(windSpeedLimit_);

        // If either wind limit is exceeded, set the flag
        windLimitExceeded_ = (windLimitExceededForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] || windLimitExceededForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]);
        surfaceFireSpread_->setIsWindLimitExceeded(windLimitExceeded_);

        // Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
        fireLengthToWidthRatio_ = lengthToWidthRatioForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST];
        surfaceFireSpread_->setFireLengthToWidthRatio(fireLengthToWidthRatio_);

        // Heat per unit area is the maximum of the two models
        heatPerUnitArea_ = (heatPerUnitAreaForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > heatPerUnitAreaForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            heatPerUnitAreaForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : heatPerUnitAreaForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND];
        surfaceFireSpread_->setHeatPerUnitArea(heatPerUnitArea_);

        // Fireline intensity is the maximum of the two models
        fireLineIntensity_ = (firelineIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > firelineIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            firelineIntensityForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : firelineIntensityForFuelModel_[1];
        surfaceFireSpread_->setFirelineIntensity(fireLineIntensity_);

        // Flame length is the maximum of the two models
        flameLength_ = (flameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > flameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            flameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : flameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND];
        maxFlameLength_ = (maxFlameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > maxFlameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            maxFlameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : maxFlameLengthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND];
        surfaceFireSpread_->setFlameLength(flameLength_);

        // Fuel bed depth is the maximum of the two fuel bed depths
        fuelbedDepth_ = (fuelbedDepthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > fuelbedDepthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]) ?
            fuelbedDepthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] : fuelbedDepthForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND];
    }
    surfaceFireSpread_->forwardSpreadRate_ = spreadRate_;
}

double SurfaceTwoFuelModels::surfaceFireExpectedSpreadRate(double* ros, double* cov, int fuels,
    double lbRatio, int samples, int depth, int laterals)
{
    // Initialize results
    double expectedRos = 0.0;

    // Create a RandFuel instance
    RandFuel randFuel;

    // Mark says the cell size is irrelevant, but he sets it anyway.
    randFuel.setCellDimensions(10);

    // Get total fuel coverage
    double totalCov = 0.0;
    int i;
    for (i = 0; i < fuels; i++)
    {
        totalCov += cov[i];
    }
    // If no fuel coverage, we're done.
    if (totalCov <= 0.0)
    {
        return(expectedRos);
    }
    // Allocate the fuels
    if (!randFuel.allocFuels(fuels))
    {
        return(expectedRos);
    }
    // Normalize fuel coverages and store the fuel ros and cov
    for (i = 0; i < fuels; i++)
    {
        cov[i] = cov[i] / totalCov;
        randFuel.setFuelData(i, ros[i], cov[i]);
    }
    
    double maximumRos;
    double* harmonicRos;    // only exists to match computeSpread2's method signature
    harmonicRos = 0;        // point harmonicRos to null
    // Compute the expected rate
    expectedRos = randFuel.computeSpread2(
        samples,            // columns
        depth,              // rows
        lbRatio,            // fire length-to-breadth ratio
        1,                  // always use 1 thread
        &maximumRos,        // returned maximum spread rate
        harmonicRos,        // returned harmonic spread rate
        laterals,           // lateral extensions
        0);                 // less ignitions
    randFuel.freeFuels();

    // Determine expected spread rates.
    expectedRos *= maximumRos;

    return(expectedRos);
}

void SurfaceTwoFuelModels::calculateFireOutputsForEachModel(bool hasDirectionOfInterest, double directionOfInterest)
{
    for (int i = 0; i < SurfaceInputs::TwoFuelModelsContants::NUMBER_OF_MODELS; i++)
    {
        fuelbedDepthForFuelModel_[i] = surfaceFireSpread_->getFuelbedDepth();

        rosForFuelModel_[i] = surfaceFireSpread_->calculateForwardSpreadRate(fuelModelNumber_[i], hasDirectionOfInterest, directionOfInterest);

        reactionIntensityForFuelModel_[i] = surfaceFireSpread_->getReactionIntensity(HeatSourceAndReactionIntensityUnits::BtusPerSquareFootPerMinute);
        dirMaxSpreadForFuelModel_[i] = surfaceFireSpread_->getDirectionOfMaxSpread();
        midFlameWindSpeedForFuelModel_[i] = surfaceFireSpread_->getMidflameWindSpeed();
        windAdjustmentFactorForFuelModel_[i] = surfaceFireSpread_->getWindAdjustmentFactor();
        effectiveWindSpeedForFuelModel_[i] = surfaceFireSpread_->getEffectiveWindSpeed();
        windSpeedLimitForFuelModel_[i] = surfaceFireSpread_->getWindSpeedLimit();
        windLimitExceededForFuelModel_[i] = surfaceFireSpread_->getIsWindLimitExceeded();
        firelineIntensityForFuelModel_[i] = surfaceFireSpread_->getFirelineIntensity();
        maxFlameLengthForFuelModel_[i] = surfaceFireSpread_->getMaxFlameLength();
        flameLengthForFuelModel_[i] = surfaceFireSpread_->getFlameLength();
        lengthToWidthRatioForFuelModel_[i] = surfaceFireSpread_->getFireLengthToWidthRatio();
        heatPerUnitAreaForFuelModel_[i] = surfaceFireSpread_->getHeatPerUnitArea(HeatPerUnitAreaUnits::BtusPerSquareFoot);
    }
}

void SurfaceTwoFuelModels::calculateSpreadRateBasedOnMethod()
{
    // If area weighted spread rate ...
    if (twoFuelModelsMethod_ == TwoFuelModelsMethod::Arithmetic)
    {
        spreadRate_ = (coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] * rosForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST]) +
            (coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND] * rosForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]);
    }
    // else if harmonic mean spread rate...
    else if (twoFuelModelsMethod_ == TwoFuelModelsMethod::Harmonic)
    {
        if (rosForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] > 0.000001 && rosForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND] > 0.000001)
        {
            spreadRate_ = 1.0 / ((coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST] / rosForFuelModel_[SurfaceInputs::TwoFuelModelsContants::FIRST]) +
                (coverageForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND] / rosForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]));
        }
    }
    // else if Finney's 2-dimensional spread rate...
    else if (twoFuelModelsMethod_ == TwoFuelModelsMethod::TwoFimensional)
    {
        //double lbRatio = lengthToWidthRatioForFuelModel_[TwoFuelModels::FIRST]; // get first fuel model's length-to-width ratio
        double lbRatio = lengthToWidthRatioForFuelModel_[SurfaceInputs::TwoFuelModelsContants::SECOND]; // using fuel model's length-to-width ratio seems to agree with BehavePlus
        int samples = 2; // from behavePlus.xml
        int depth = 2; // from behavePlus.xml
        int laterals = 0; // from behavePlus.xml
        spreadRate_ = surfaceFireExpectedSpreadRate(rosForFuelModel_, coverageForFuelModel_, SurfaceInputs::TwoFuelModelsContants::NUMBER_OF_MODELS, lbRatio,
            samples, depth, laterals);
    }
}
