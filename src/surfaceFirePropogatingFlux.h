#ifndef SURFACEFIREPROPOGATINGFLUX_HEADER
#define SURFACEFIREPROPOGATINGFLUX_HEADER

#include <cmath>

class SurfaceFirePropogatingFlux
{
public:
    SurfaceFirePropogatingFlux();
    double calculatePropagatingFlux(double sigma, double packingRatio);
};

#endif // SURFACEFIREPROPOGATINGFLUX_HEADER
