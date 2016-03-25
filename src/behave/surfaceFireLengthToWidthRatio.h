#ifndef SURFACEFIRELENGTHTOWIDTHRATIO_HEADER
#define SURFACEFIRELENGTHTOWIDTHRATIO_HEADER

class SurfaceFireLengthtoWidthRatio
{
public:
	double FBL_SurfaceFireLengthToWidthRatio(double effectiveWindSpeed);
private:
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc.
};

#endif // SURFACEFIRELENGTHTOWIDTHRATIO_HEADER