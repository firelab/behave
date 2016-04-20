#include "windSpeed.h"

WindSpeed::WindSpeed()
{
	windSpeedAt10M = 0;
	windSpeedAt20Ft = 0;
	windSpeedAtMidflame = 0;
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

double WindSpeed::WindSpeedAtMidflame(double windSpeedAt20FtIn, double windAdjFactor)
{
	// Calculate results
	windSpeedAtMidflame = windSpeedAt20FtIn * windAdjFactor;
	
	return windSpeedAtMidflame;
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

double WindSpeed::WindSpeedAtTwentyFtFromTenMeter(double windSpeedAt10MIn)
{
	// Calculate results
    double windSpeedAt20Ft = (windSpeedAt10MIn / 1.15);
	return windSpeedAt20Ft;
}

