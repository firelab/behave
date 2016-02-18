#include "palmettoGallberry.h"

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead 0.0 - 0.25" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery dead 0.0 - 0.25" load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyDeadOneHourLoad(double ageOfRough, double heightOfUnderstory)
{
	double load = -0.00121
		+ 0.00379 * log(ageOfRough)
		+ 0.00118 * heightOfUnderstory * heightOfUnderstory;
	if (load < 0.0)
	{
		load = 0.0;
	}
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead 0.25 - 1.0 " load.
*
*  \param age      Age of rough (years).
*  \param cover    Coverage of area by palmetto (percent).
*
*  \return Palmetto-gallbery dead 0.25 - 1.0" load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyDeadTenHourLoad(double ageOfRough, double palmettoCoverage)
{
	double load = -0.00775
		+ 0.00021 * palmettoCoverage
		+ 0.00007 * ageOfRough * ageOfRough;
	if (load < 0.0)
	{
		load = 0.0;
	}
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead foliage load.
*
*  \param age      Age of rough (years).
*  \param cover    Coverage of area by palmetto (percent).
*
*  \return Palmetto-gallbery dead foliage load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyDeadFoliageLoad(double ageOfRough, double palmettoCoverage)
{
	double load = 0.00221 * pow(ageOfRough, 0.51263) * exp(0.02482 * palmettoCoverage);
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry fuel bed depth.
*
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery fuel bed depth (ft).
*/

double PalmettoGallberry::palmettoGallberyFuelBedDepth(double heightOfUnderstory)
{
	double depth = 2.0 * heightOfUnderstory / 3.0;
	return depth;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry L layer load.
*
*  \param age      Age of rough (years).
*  \param ba       Overstory basal aea (ft2/ac)
*
*  \return Palmetto-gallbery L layer load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyLitterLoad(double ageOfRough, double overstoryBasalArea)
{
	double load = (0.03632 + 0.0005336 * overstoryBasalArea) * (1.0 - pow(0.25, ageOfRough));
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry live 0.0 - 0.25" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery live 0.0 - 0.25" load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyLiveOneHourLoad(double ageOfRough, double heightOfUnderstory)
{
	double load = 0.00546 + 0.00092 * ageOfRough + 0.00212 * heightOfUnderstory * heightOfUnderstory;
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry live 0.25 - 1.0" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery live 0.25 - 1.0" load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyLiveTenHourLoad(double ageOfRough, double heightOfUnderstory)
{
	double load = -0.02128
		+ 0.00014 * ageOfRough * ageOfRough
		+ 0.00314 * heightOfUnderstory * heightOfUnderstory;
	if (load < 0.0)
	{
		load = 0.0;
	}
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry live foliage load.
*
*  \param age      Age of rough (years).
*  \param cover    Coverage of area by palmetto (percent).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery live foliage load (lb/ft2).
*/

double PalmettoGallberry::palmettoGallberyLiveFoliageLoad(double ageOfRough, double palmettoCoverage,
	double heightOfUnderstory)
{
	double load = -0.0036
		+ 0.00253 * ageOfRough
		+ 0.00049 * palmettoCoverage
		+ 0.00282 * heightOfUnderstory * heightOfUnderstory;
	if (load < 0.0)
	{
		load = 0.0;
	}
	return load;
}

void PalmettoGallberry::setAgeOfRough(double ageOfRough)
{
	ageOfRough_ = ageOfRough;
}

double PalmettoGallberry::getAgeOfRough() const
{
	return ageOfRough_;
}

void PalmettoGallberry::setHeightOfUnderstory(double heightOfUnderstory)
{
	heightOfUnderstory_ = heightOfUnderstory;
}

double PalmettoGallberry::getHeightOfUnderstory() const
{
	return heightOfUnderstory_;
}

void PalmettoGallberry::setPalmettoCoverage(double palmettoCoverage)
{
	palmettoCoverage_ = palmettoCoverage;
}

double PalmettoGallberry::getPalmettoCoverage() const
{
	return palmettoCoverage_;
}

void PalmettoGallberry::setOverstoryBasalArea(double overstoryBasalArea)
{
	overstoryBasalArea_ = overstoryBasalArea;
}

double PalmettoGallberry::getOverstoryBasalArea() const
{
	return overstoryBasalArea_;
}
