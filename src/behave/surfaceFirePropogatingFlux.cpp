#include "surfaceFirePropogatingFlux.h"

SurfaceFirePropogatingFlux::SurfaceFirePropogatingFlux()
{

}

//------------------------------------------------------------------------------
/*! \brief Calculates the fire's propagating flux.
 *
 *  \param packingRatio		Surface fuel bed packing ratio (beta) (ft3/ft3).
 *  \param sigma			Surface fuel bed's characteristic surface area-
 *                             to-volume ratio (sigma) (ft2/ft3).
 *
 *  \return Fire's propagating flux (fraction).
 */
double SurfaceFirePropogatingFlux::calculatePropagatingFlux(double sigma,
    double packingRatio)
{
    double propagatingFlux = (sigma < 1.0e-07)
        ? (0.)
        : (exp((0.792 + 0.681 * sqrt(sigma)) * (packingRatio + 0.1)) / (192. + 0.2595 * sigma));
    return propagatingFlux;
}
