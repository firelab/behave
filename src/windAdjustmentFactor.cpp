#include "windAdjustmentFactor.h"

WindAjustmentFactor::WindAjustmentFactor()
{
    windAdjustmentFactor_ = 0.0;
    canopyCrownFraction_ = 0.0;
    windAdjustmentFactorMethod_ = WindAdjustmentFactorMethod::UNSHELTERED;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the wind adjustment factor for scaling wind speed from
* 20-ft to midflame height.
*
*  Original function used by BehavePlus by Collin Bevins
*  For sheltered conditions under a canopy, Albini and Baughman (1979)
*  equation 21 (page 9) is used for the wind adjustment factor.
*
*  For unsheltered conditions, wind adjustment factor is calculated as an
*	average from the top of the fuel bed to twice the fuel bed depth,
*	using Albini and Baughman (1979) equation 9 (page 5).
*
*  \param canopyCover  Canopy cover projected onto ground [0..1].
*  \param canopyHt     Tree canopy height from the ground (ft).
*  \param crownRatio   Tree crown length-to-tree height ratio [0..1].
*  \param fuelDepth    Fuel bed depth (ft).
*
*  \return Wind adjustment factor [0..1].
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
        windAdjustmentFactor = 0.555 / (sqrt(canopyCrownFraction_ * canopyHeight) * log((20.0 + 0.36 * canopyHeight) / (0.13 * canopyHeight)));
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
