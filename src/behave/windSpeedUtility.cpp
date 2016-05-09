#include "windSpeedUtility.h"

WindSpeedUtility::WindSpeedUtility()
{
    windSpeedAtTenMeters_ = 0;
    windSpeedAtTwentyFeet_ = 0;
    windSpeedAtMidflame_ = 0;
}

//------------------------------------------------------------------------------
/*! \brief WindSpeedAtMidflame
*
*  Dependent Variables (Outputs)
*      vWindSpeedAtMidflame (mi/h)
*
*  Independent Variables (Inputs)
*      vWindSpeedAt20Ft (mi/h)
*      vWindAdjFactor (fraction)
*/

double WindSpeedUtility::WindSpeedAtMidflame(double windSpeedAtTwentyFeet, double windAdjustmentFactor)
{
    // Calculate results
    windSpeedAtMidflame_ = windSpeedAtTwentyFeet * windAdjustmentFactor;
    return windSpeedAtMidflame_;
}

//------------------------------------------------------------------------------
/*! \brief WindSpeedAt20Ft
*
*  Dependent Variables (Outputs)
*      vWindSpeedAt20Ft (mi/h)
*
*  Independent Variables (Inputs)
*      vWindSpeedAt10M (mi/h)
*/

double WindSpeedUtility::WindSpeedAtTwentyFeetFromTenMeter(double windSpeedAtTenMeters)
{
    // Calculate results
    double windSpeedAt20Ft_ = windSpeedAtTenMeters / 1.15;
    return windSpeedAt20Ft_;
}
