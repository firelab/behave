/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Part of Mark Finney's EXRATE package for determining expected
*           and harmonic mean spread rate in randomly arranged fuels
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in the corresponding cpp file is, in part or in
*           whole, from BehavePlus5 source originally authored by Collin D.
*           Bevins and is used with or without modification.
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

#ifndef SURFACETWOFUELMODELS_H
#define SURFACETWOFUELMODELS_H

#include "surfaceInputs.h"

class SurfaceFuelbedIntermediates;
class SurfaceFire;

class SurfaceTwoFuelModels
{
public:
    SurfaceTwoFuelModels(SurfaceFire& surfaceFireSpread);
    void calculateWeightedSpreadRate(TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod,
        int firstFuelModelNumber, double firstFuelModelCoverage, int secondFuelModelNumber,
        bool hasDirectionOfInterest, double directionOfInterest,
        SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode);

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
    double getflameLength() const;
    double getFireLengthToWidthRatio() const;

private:
    double surfaceFireExpectedSpreadRate(double* ros, double* coverage, int fuels,
        double lbRatio, int samples, int depth, int laterals);
    void calculateFireOutputsForEachModel(bool hasDirectionOfInterest, double directionOfInterest,
        SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum directionMode);
    void calculateSpreadRateBasedOnMethod();

    SurfaceFire* surfaceFireSpread_;

    // Member arrays, stores data for each of the two fuel models
    int fuelModelNumber_[TwoFuelModelsContants::NumberOfModels];                      // fuel model number
    double coverageForFuelModel_[TwoFuelModelsContants::NumberOfModels];              // percent coverage of fuel model
    double rosForFuelModel_[TwoFuelModelsContants::NumberOfModels];                   // rate of spread
    double firelineIntensityForFuelModel_[TwoFuelModelsContants::NumberOfModels];     // fireline intensity
    double maxFlameLengthForFuelModel_[TwoFuelModelsContants::NumberOfModels];        // flame length in direction of max spread
    double flameLengthForFuelModel_[TwoFuelModelsContants::NumberOfModels];           // flame length
    double fuelbedDepthForFuelModel_[TwoFuelModelsContants::NumberOfModels];          // fuel bed depth in feet
    double effectiveWindSpeedForFuelModel_[TwoFuelModelsContants::NumberOfModels];    // effective wind speed
    double lengthToWidthRatioForFuelModel_[TwoFuelModelsContants::NumberOfModels];    // fire length-to-width ratio
    double reactionIntensityForFuelModel_[TwoFuelModelsContants::NumberOfModels];     // reaction intensity, 
    double heatPerUnitAreaForFuelModel_[TwoFuelModelsContants::NumberOfModels];       // heat per unit area
    double dirMaxSpreadForFuelModel_[TwoFuelModelsContants::NumberOfModels];          // direction of max spread
    double windAdjustmentFactorForFuelModel_[TwoFuelModelsContants::NumberOfModels];  // wind adjustment factor
    double midFlameWindSpeedForFuelModel_[TwoFuelModelsContants::NumberOfModels];     // wind speed at midflame
    double windSpeedLimitForFuelModel_[TwoFuelModelsContants::NumberOfModels];        // wind speed limit
    bool windLimitExceededForFuelModel_[TwoFuelModelsContants::NumberOfModels];       // wind speed exceeded flag

    // Member variables
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum twoFuelModelsMethod_;
    bool windLimitExceeded_;        // (flag)
    double reactionIntensity_;      // (Btu / ft^2 / min)
    double spreadRate_;             // (ft / min)
    double directionOfMaxSpread_;   // (clockwise from upslope or north)
    double effectiveWind_;          // (mi / h)
    double fuelbedDepth_;           // (ft)
    double heatPerUnitArea_;        // (Btu / ft^2)
    double midFlameWindSpeed_;
    double windSpeedLimit_;         // (mi / h)
    double windAdjustmentFactor_;
    double fireLineIntensity_;      // (Btu / ft / s)
    double flameLength_;            // (ft)
    double maxFlameLength_;         // flame length in direction of maximum spread (ft)
    double fireLengthToWidthRatio_;
};

#endif // SURFACETWOFUELMODELS_H
