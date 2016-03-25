#include "surfaceFireFlameLength.h"
#include <cmath>

//------------------------------------------------------------------------------
/*! \brief Calculates flame length from fireline (Byram's) intensity.
 *
 *  \param firelineIntensity Fireline (Byram's) intensity (btu/ft/s).
 *
 *  \return Flame length (ft).
 */
double SurfaceFireFlameLength::FBL_SurfaceFireFlameLength( double firelineIntensity )
{
    return( ( firelineIntensity < SMIDGEN )
          ? ( 0.0 )
          : ( 0.45 * pow( firelineIntensity, 0.46 ) ) );
}


