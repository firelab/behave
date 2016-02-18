#ifndef PALMETTOGALLBERRY_HEADER
#define PALMETTOGALLBERRY_HEADER

#include <cmath>

class PalmettoGallberry
{
public:
	double palmettoGallberyDeadOneHourLoad(double ageOfRough, double heightOfUnderstory);
	double palmettoGallberyDeadTenHourLoad(double ageOfRough, double palmettoCoverage);
	double palmettoGallberyDeadFoliageLoad(double ageOfRough, double palmettoCoverage);
	double palmettoGallberyFuelBedDepth(double heightOfUnderstory);
	double palmettoGallberyLitterLoad(double ageOfRough, double overstoryBasalArea);
	double palmettoGallberyLiveOneHourLoad(double ageOfRough, double heightOfUnderstory);
	double palmettoGallberyLiveTenHourLoad(double ageOfRough, double heightOfUnderstory);
	double palmettoGallberyLiveFoliageLoad(double ageOfRough, double palmettoCoverage, double heightOfUnderstory);

	void setAgeOfRough(double ageOfRough);
	double getAgeOfRough() const;
	void setHeightOfUnderstory(double heightOfUnderstory);
	double getHeightOfUnderstory() const;
	void setPalmettoCoverage(double palmettoCoverage);
	double getPalmettoCoverage() const;
	void setOverstoryBasalArea(double overstoryBasalArea);
	double getOverstoryBasalArea() const;

private:
	double	ageOfRough_;
	double	heightOfUnderstory_;
	double	palmettoCoverage_;
	double	overstoryBasalArea_;
};

#endif //PALMETTOGALLBERRY_HEADER