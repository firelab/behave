#ifndef PALMETTOGALLBERRY_HEADER
#define PALMETTOGALLBERRY_HEADER

#include <cmath>

class PalmettoGallberry
{
public:
	PalmettoGallberry();

	double palmettoGallberyDeadOneHourLoad(double ageOfRough, double heightOfUnderstory);
	double palmettoGallberyDeadTenHourLoad(double ageOfRough, double palmettoCoverage);
	double palmettoGallberyDeadFoliageLoad(double ageOfRough, double palmettoCoverage);
	double palmettoGallberyFuelBedDepth(double heightOfUnderstory);
	double palmettoGallberyLitterLoad(double ageOfRough, double overstoryBasalArea);
	double palmettoGallberyLiveOneHourLoad(double ageOfRough, double heightOfUnderstory);
	double palmettoGallberyLiveTenHourLoad(double ageOfRough, double heightOfUnderstory);
	double palmettoGallberyLiveFoliageLoad(double ageOfRough, double palmettoCoverage, double heightOfUnderstory);

private:
};

#endif //PALMETTOGALLBERRY_HEADER