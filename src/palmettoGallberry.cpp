#include "palmettoGallberry.h"

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead 0.0 - 0.25" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery dead 0.0 - 0.25" load (lb/ft2).
*/

double PalmettoGallberry::PalmettoGallberyDeadOneHourLoad(double age, double height)
{
	double load = -0.00121
		+ 0.00379 * log(age)
		+ 0.00118 * height * height;
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

double PalmettoGallberry::PalmettoGallberyDeadTenHourLoad(double age, double cover)
{
	double load = -0.00775
		+ 0.00021 * cover
		+ 0.00007 * age * age;
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

double PalmettoGallberry::PalmettoGallberyDeadFoliageLoad(double age, double cover)
{
	double load = 0.00221 * pow(age, 0.51263) * exp(0.02482 * cover);
	return load;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry fuel bed depth.
*
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery fuel bed depth (ft).
*/

double PalmettoGallberry::PalmettoGallberyFuelBedDepth(double height)
{
	double depth = 2.0 * height / 3.0;
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

double PalmettoGallberry::PalmettoGallberyLitterLoad(double age, double ba)
{
	double load = (0.03632 + 0.0005336 * ba) * (1.0 - pow(0.25, age));
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

double PalmettoGallberry::PalmettoGallberyLiveOneHourLoad(double age, double height)
{
	double load = 0.00546 + 0.00092 * age + 0.00212 * height * height;
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

double PalmettoGallberry::PalmettoGallberyLiveTenHourLoad(double age, double height)
{
	double load = -0.02128
		+ 0.00014 * age * age
		+ 0.00314 * height * height;
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

double PalmettoGallberry::PalmettoGallberyLiveFoliageLoad(double age, double cover,
	double height)
{
	double load = -0.0036
		+ 0.00253 * age
		+ 0.00049 * cover
		+ 0.00282 * height * height;
	if (load < 0.0)
	{
		load = 0.0;
	}
	return load;
}
