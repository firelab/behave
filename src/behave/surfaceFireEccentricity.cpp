#include "surfaceFireEccentricity.h"
#include <math.h>

//------------------------------------------------------------------------------
/*! \brief Calculates elliptical fire perimeter eccentricity from fire
 *  length-to-width ratio.
 *
 *  \param lengthWidthRatio Elliptical fire perimeter length-to-width ratio
 *  (ft/ft).
 *
 *  \return Elliptical fire eccentricity (dl).
 */

double SurfaceFireEccentricity::calculateSurfaceFireEccentricity(double lengthWidthRatio)
{
    double x = lengthWidthRatio * lengthWidthRatio - 1.0;
    return( ( x > 0. )
          ? ( sqrt( x ) / lengthWidthRatio )
          : ( 0.0 ) );
}
