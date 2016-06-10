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

#include <cmath>
#include "windAdjustmentFactor.h"

WindAjustmentFactor::WindAjustmentFactor()
{
    windAdjustmentFactor_ = 0.0;
    canopyCrownFraction_ = 0.0;
    windAdjustmentFactorMethod_ = WindAdjustmentFactorMethod::UNSHELTERED;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the wind adjustment factor for scaling wind speed from
*   20-ft to midflame height.
*
*   For sheltered conditions under a canopy, Albini and Baughman (1979)
*   equation 21 (page 9) is used for the wind adjustment factor.
*
*   For unsheltered conditions, wind adjustment factor is calculated as an
*	average from the top of the fuel bed to twice the fuel bed depth,
*	using Albini and Baughman (1979) equation 9 (page 5).
*
*   \param canopyCover  Canopy cover projected onto ground [0..1].
*   \param canopyHeight Tree canopy height from the ground (ft).
*   \param crownRatio   Tree crown length-to-tree height ratio [0..1].
*   \param fuelbedDepth Fuel bed depth (ft).
*
*   \return Wind adjustment factor [0..1].
*/

double WindAjustmentFactor::calculateWindAdjustmentFactor(double canopyCover, double canopyHeight,
    double crownRatio, double fuelbedDepth)
{
    double windAdjustmentFactor = 1.0;

    crownRatio = (crownRatio < 0.0) ? 0.0 : crownRatio;
    crownRatio = (crownRatio > 1.0) ? 1.0 : crownRatio;
    canopyCover = (canopyCover < 0.0) ? 0.0 : canopyCover;
    canopyCover = (canopyCover > 1.0) ? 1.0 : canopyCover;

    // canopyCrownFraction == fraction of the volume under the canopy top that is filled with
    // tree crowns (division by 3 assumes conical crown shapes).
    canopyCrownFraction_ = crownRatio * canopyCover / 3.0;

    // Unsheltered
    if (canopyCover < 1.0e-07 || canopyCrownFraction_ < 0.05 || canopyHeight < 6.0)
    {
        if (fuelbedDepth > 1.0e-07)
        {
            windAdjustmentFactor = 1.83 / log((20.0 + 0.36 * fuelbedDepth) / (0.13 * fuelbedDepth));
        }
        windAdjustmentFactorMethod_ = WindAdjustmentFactorMethod::UNSHELTERED;
    }
    // Sheltered
    else
    {
        windAdjustmentFactor = 0.555 / (sqrt(canopyCrownFraction_ * canopyHeight) * log((20.0 + 0.36 * canopyHeight)
            / (0.13 * canopyHeight)));
        windAdjustmentFactorMethod_ = WindAdjustmentFactorMethod::SHELTERED;
    }
    // Results
    windAdjustmentFactor_ = (windAdjustmentFactor > 1.0) ? 1.0 : windAdjustmentFactor;
    windAdjustmentFactor_ = (windAdjustmentFactor < 0.0) ? 0.0 : windAdjustmentFactor;

    return windAdjustmentFactor_;
}

double WindAjustmentFactor::getCanopyCrownFraction() const
{
    return canopyCrownFraction_;
}

WindAdjustmentFactorMethod::WindAdjustmentFactorMethodEnum WindAjustmentFactor::getWindAdjustmentFactorMethod() const
{
    return windAdjustmentFactorMethod_;
}
