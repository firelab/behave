#include "surfaceFireResidenceTime.h"
	
//------------------------------------------------------------------------------
/*! \brief Calculates the fire's residence time.
	*
	*  \param fuelBedSavr  Surface fuel bed characteristic surface area-to-volume
	*                      ratio (sigma) (ft2/ft3).
	*
	*  \return Fire residence time (min).
*/

double SurfaceFireResidenceTime::FBL_SurfaceFireResidenceTime( double fuelBedSavr )
{
    return( ( fuelBedSavr < SMIDGEN )
          ? ( 0.0 )
          : ( 384. / fuelBedSavr ) );
}


