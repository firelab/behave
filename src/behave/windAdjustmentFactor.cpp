/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating wind adjustment factor
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
#include "windAdjustmentFactor.h"

WindAjustmentFactor::WindAjustmentFactor()
{
    windAdjustmentFactor_ = 0.0;
    canopyCrownFraction_ = 0.0;
    windAdjustmentFactorShelterMethod_ = WindAdjustmentFactorShelterMethod::UNSHELTERED;
}

double WindAjustmentFactor::calculateWindAdjustmentFactorWithCrownRatio(double canopyCover, double canopyHeight,
    double crownRatio, double fuelbedDepth)
{
    windAdjustmentFactor_ = 1.0;

    crownRatio = (crownRatio < 0.0) ? 0.0 : crownRatio;
    crownRatio = (crownRatio > 1.0) ? 1.0 : crownRatio;
    canopyCover = (canopyCover < 0.0) ? 0.0 : canopyCover;
    canopyCover = (canopyCover > 1.0) ? 1.0 : canopyCover;

    // canopyCrownFraction == fraction of the volume under the canopy top that is filled with
    // tree crowns (division by 3 assumes conical crown shapes).
    canopyCrownFraction_ = crownRatio * canopyCover / 3.0;

    calculateWindAdjustmentFactorShelterMethod(canopyCover, canopyHeight, fuelbedDepth);

    // Results
    windAdjustmentFactor_ = (windAdjustmentFactor_ > 1.0) ? 1.0 : windAdjustmentFactor_;
    windAdjustmentFactor_ = (windAdjustmentFactor_ < 0.0) ? 0.0 : windAdjustmentFactor_;

    return windAdjustmentFactor_;
}

double WindAjustmentFactor::calculateWindAdjustmentFactorWithoutCrownRatio(double canopyCover, double canopyHeight, double fuelbedDepth)
{
    windAdjustmentFactor_ = 1.0;
    canopyCover = (canopyCover < 0.0) ? 0.0 : canopyCover;
    canopyCover = (canopyCover > 1.0) ? 1.0 : canopyCover;

    // canopyCrownFraction == fraction of the volume under the canopy top that is filled with
    // tree crowns (division by 3 assumes conical crown shapes).
    canopyCrownFraction_ = (canopyCover * M_PI) / 1200.0; // RMRS-RP-4, eq. 45
   
    calculateWindAdjustmentFactorShelterMethod(canopyCover, canopyHeight, fuelbedDepth);

    // Results
    windAdjustmentFactor_ = (windAdjustmentFactor_ > 1.0) ? 1.0 : windAdjustmentFactor_;
    windAdjustmentFactor_ = (windAdjustmentFactor_ < 0.0) ? 0.0 : windAdjustmentFactor_;

    return windAdjustmentFactor_;
}

double WindAjustmentFactor::getCanopyCrownFraction() const
{
    return canopyCrownFraction_;
}

WindAdjustmentFactorShelterMethod::WindAdjustmentFactorShelterMethodEnum WindAjustmentFactor::getWindAdjustmentFactorShelterMethod() const
{
    return windAdjustmentFactorShelterMethod_;
}

void WindAjustmentFactor::calculateWindAdjustmentFactorShelterMethod(const double canopyCover, const double canopyHeight, const double fuelbedDepth)
{
    // Unsheltered
    if (canopyCover < 1.0e-07 || canopyCrownFraction_ < 0.05 || canopyHeight < 6.0)
    {
        if (fuelbedDepth > 1.0e-07)
        {
            windAdjustmentFactor_ = 1.83 / log((20.0 + 0.36 * fuelbedDepth) / (0.13 * fuelbedDepth));
        }
        windAdjustmentFactorShelterMethod_ = WindAdjustmentFactorShelterMethod::UNSHELTERED;
    }
    // Sheltered
    else
    {
        windAdjustmentFactor_ = 0.555 / (sqrt(canopyCrownFraction_ * canopyHeight) * log((20.0 + 0.36 * canopyHeight)
            / (0.13 * canopyHeight)));
        windAdjustmentFactorShelterMethod_ = WindAdjustmentFactorShelterMethod::SHELTERED;
    }
}
