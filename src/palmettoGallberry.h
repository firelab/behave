#ifndef PALMETTOGALLBERRY_HEADER
#define PALMETTOGALLBERRY_HEADER

#include <cmath>

class PalmettoGallberry
{
public:
	double PalmettoGallberyDead1HrLoad(double age, double height);
	double PalmettoGallberyDead10HrLoad(double age, double cover);
	double PalmettoGallberyDeadFoliageLoad(double age, double cover);
	double PalmettoGallberyFuelBedDepth(double height);
	double PalmettoGallberyLitterLoad(double age, double ba);
	double PalmettoGallberyLive1HrLoad(double age, double height);
	double PalmettoGallberyLive10HrLoad(double age, double height);
	double PalmettoGallberyLiveFoliageLoad(double age, double cover, double height);

private:

};

#endif //PALMETTOGALLBERRY_HEADER