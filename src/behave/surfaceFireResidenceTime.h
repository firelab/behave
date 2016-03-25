#ifndef SURFACEFIRERESIDENCETIME_HEADER
#define SURFACEFIRERESIDENCETIME_HEADER

class SurfaceFireResidenceTime
{
public:
	double FBL_SurfaceFireResidenceTime( double fuelBedSavr );

private:
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc.
};

#endif //SURFACEFIRERESIDENCETIME_HEADER
