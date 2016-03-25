#ifndef SURFACEFIREWIDTH_HEADER
#define SURFACEFIREWIDTH_HEADER

class SurfaceFireWidth
{
public:
	double FBL_SurfaceFireWidth(
            double fireLength,
            double lengthWidthRatio);
private:
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc.
};

#endif // SURFACEFIREWIDTH_HEADERSURFACEFIREWIDTH_HEADER