#ifndef SURFACEFIREFLAMELENGTH_HEADER
#define SURFACEFIREFLAMELENGTH_HEADER

//dependencies: none

//optional dependencies: surfaceFirelineIntensity

class SurfaceFireFlameLength
{
public:
	double FBL_SurfaceFireFlameLength(double firelineIntensity);

private:
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc.
};

#endif // SURFACEFIREFLAMELENGTH_HEADER