#ifndef PALMETTOGALLBERRY_HEADER
#define PALMETTOGALLBERRY_HEADER

#include <cmath>

class PalmettoGallberry
{
public:
	double PalmettoGallberyDeadOneHourLoad(double age, double height);
	double PalmettoGallberyDeadTenHourLoad(double age, double cover);
	double PalmettoGallberyDeadFoliageLoad(double age, double cover);
	double PalmettoGallberyFuelBedDepth(double height);
	double PalmettoGallberyLitterLoad(double age, double ba);
	double PalmettoGallberyLiveOneHourLoad(double age, double height);
	double PalmettoGallberyLiveTenHourLoad(double age, double height);
	double PalmettoGallberyLiveFoliageLoad(double age, double cover, double height);

private:

};

#endif //PALMETTOGALLBERRY_HEADER