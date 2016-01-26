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

double WindSpeed::WindSpeedAt20Ft(double windSpeedAt10MIn)
{
	// Calculate results
	double windSpeedAt20Ft = FBL_WindSpeedAt20Ft(windSpeedAt10MIn);	
	return windSpeedAt20Ft;
}


//------------------------------------------------------------------------------
/*! \brief Calculates the wind speed at 20 ft from the wind speed at 10 m.
*
*  \param windSpeedAt10M Wind speed at 10 meter (mi/h).
*
*  \return Wind speed at 20 ft (mi/h).
*/

double WindSpeed::FBL_WindSpeedAt20Ft(double windSpeedAt10MIn)
{
	return( windSpeedAt10MIn / 1.15 );
}
