#include "surfaceFireLengthToWidthRatio.h"

//------------------------------------------------------------------------------
/*! \brief Calculates the fire length-to-width ratio given the effective
 *  wind speed (combined wind-slope effect).
 *
 *  \param effectiveWindSpeed Effective wind speed at fire's head (mi/h).
 *
 *  \return Fire length-to-width ratio (ft/ft).
 */
double SurfaceFireLengthtoWidthRatio::FBL_SurfaceFireLengthToWidthRatio( double effectiveWindSpeed )
{
    return( ( effectiveWindSpeed > SMIDGEN )
          ? ( 1. + 0.25 * effectiveWindSpeed )
          : ( 1. ) );
}