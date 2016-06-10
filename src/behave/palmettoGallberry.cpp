/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the Palmetto-Gallbery special case fuel model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
*******************************************************************************
*
* THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
* MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
* IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
* OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
* PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
* LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
* PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
* RELIABILITY, OR ANY OTHER CHARACTERISTIC.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
******************************************************************************/

#include "palmettoGallberry.h"

#include <cmath>

PalmettoGallberry::PalmettoGallberry()
{
    initializeMembers();
}

PalmettoGallberry::PalmettoGallberry(const PalmettoGallberry &rhs)
{
    moistureOfExtinctionDead_ = rhs.moistureOfExtinctionDead_;
    heatOfCombustionDead_ = rhs.heatOfCombustionDead_;
    heatOfCombustionLive_ = rhs.heatOfCombustionLive_;
    palmettoGallberyDeadOneHourLoad_ = rhs.palmettoGallberyDeadOneHourLoad_;
    palmettoGallberyDeadTenHourLoad_ = rhs.palmettoGallberyDeadTenHourLoad_;
    palmettoGallberyDeadFoliageLoad_ = rhs.palmettoGallberyDeadFoliageLoad_;
    palmettoGallberyFuelBedDepth_ = rhs.palmettoGallberyFuelBedDepth_;
    palmettoGallberyLitterLoad_ = rhs.palmettoGallberyLitterLoad_;
    palmettoGallberyLiveOneHourLoad_ = rhs.palmettoGallberyLiveOneHourLoad_;
    palmettoGallberyLiveTenHourLoad_ = rhs.palmettoGallberyLiveTenHourLoad_;
    palmettoGallberyLiveFoliageLoad_ = rhs.palmettoGallberyLiveFoliageLoad_;
}

PalmettoGallberry& PalmettoGallberry::operator= (const PalmettoGallberry& rhs)
{
    if (this != &rhs)
    {
        moistureOfExtinctionDead_ = rhs.moistureOfExtinctionDead_;
        heatOfCombustionDead_ = rhs.heatOfCombustionDead_;
        heatOfCombustionLive_ = rhs.heatOfCombustionLive_;
        palmettoGallberyDeadOneHourLoad_ = rhs.palmettoGallberyDeadOneHourLoad_;
        palmettoGallberyDeadTenHourLoad_ = rhs.palmettoGallberyDeadTenHourLoad_;
        palmettoGallberyDeadFoliageLoad_ = rhs.palmettoGallberyDeadFoliageLoad_;
        palmettoGallberyFuelBedDepth_ = rhs.palmettoGallberyFuelBedDepth_;
        palmettoGallberyLitterLoad_ = rhs.palmettoGallberyLitterLoad_;
        palmettoGallberyLiveOneHourLoad_ = rhs.palmettoGallberyLiveOneHourLoad_;
        palmettoGallberyLiveTenHourLoad_ = rhs.palmettoGallberyLiveTenHourLoad_;
        palmettoGallberyLiveFoliageLoad_ = rhs.palmettoGallberyLiveFoliageLoad_;
    }
    return *this;
}

void PalmettoGallberry::initializeMembers()
{
    moistureOfExtinctionDead_ = 0.40;
    heatOfCombustionDead_ = 8300;
    heatOfCombustionLive_ = 8300;
    palmettoGallberyDeadOneHourLoad_ = 0.0;
    palmettoGallberyDeadTenHourLoad_ = 0.0;
    palmettoGallberyDeadFoliageLoad_ = 0.0;
    palmettoGallberyFuelBedDepth_ = 0.0;
    palmettoGallberyLitterLoad_ = 0.0;
    palmettoGallberyLiveOneHourLoad_ = 0.0;
    palmettoGallberyLiveTenHourLoad_ = 0.0;
    palmettoGallberyLiveFoliageLoad_ = 0.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead 0.0 - 0.25" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery dead 0.0 - 0.25" load (lb/ft2).
*/


double PalmettoGallberry::calculatePalmettoGallberyDeadOneHourLoad(double ageOfRough, double heightOfUnderstory)
{
    palmettoGallberyDeadOneHourLoad_ = -0.00121
        + 0.00379 * log(ageOfRough)
        + 0.00118 * heightOfUnderstory * heightOfUnderstory;
    if (palmettoGallberyDeadOneHourLoad_ < 0.0)
    {
        palmettoGallberyDeadOneHourLoad_ = 0.0;
    }
    return palmettoGallberyDeadOneHourLoad_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead 0.25 - 1.0 " load.
*
*  \param age      Age of rough (years).
*  \param cover    Coverage of area by palmetto (percent).
*
*  \return Palmetto-gallbery dead 0.25 - 1.0" load (lb/ft2).
*/

double PalmettoGallberry::calculatePalmettoGallberyDeadTenHourLoad(double ageOfRough, double palmettoCoverage)
{
    palmettoGallberyDeadTenHourLoad_ = -0.00775
        + 0.00021 * palmettoCoverage
        + 0.00007 * ageOfRough * ageOfRough;
    if (palmettoGallberyDeadTenHourLoad_ < 0.0)
    {
        palmettoGallberyDeadTenHourLoad_ = 0.0;
    }
    return palmettoGallberyDeadTenHourLoad_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry dead foliage load.
*
*  \param age      Age of rough (years).
*  \param cover    Coverage of area by palmetto (percent).
*
*  \return Palmetto-gallbery dead foliage load (lb/ft2).
*/

double PalmettoGallberry::calculatePalmettoGallberyDeadFoliageLoad(double ageOfRough, double palmettoCoverage)
{
    palmettoGallberyDeadFoliageLoad_ = 0.00221 * pow(ageOfRough, 0.51263) * exp(0.02482 * palmettoCoverage);
    return palmettoGallberyDeadFoliageLoad_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry fuel bed depth.
*
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery fuel bed depth (ft).
*/

double PalmettoGallberry::calculatePalmettoGallberyFuelBedDepth(double heightOfUnderstory)
{
    palmettoGallberyFuelBedDepth_ = 2.0 * heightOfUnderstory / 3.0;
    return palmettoGallberyFuelBedDepth_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry L layer load.
*
*  \param age      Age of rough (years).
*  \param ba       Overstory basal aea (ft2/ac)
*
*  \return Palmetto-gallbery L layer load (lb/ft2).
*/

double PalmettoGallberry::calculatePalmettoGallberyLitterLoad(double ageOfRough, double overstoryBasalArea)
{
    palmettoGallberyLitterLoad_ = (0.03632 + 0.0005336 * overstoryBasalArea) * (1.0 - pow(0.25, ageOfRough));
    return palmettoGallberyLitterLoad_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry live 0.0 - 0.25" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery live 0.0 - 0.25" load (lb/ft2).
*/

double PalmettoGallberry::calculatePalmettoGallberyLiveOneHourLoad(double ageOfRough, double heightOfUnderstory)
{
    palmettoGallberyLiveOneHourLoad_ = 0.00546 + 0.00092 * ageOfRough + 0.00212 * heightOfUnderstory * heightOfUnderstory;
    return palmettoGallberyLiveOneHourLoad_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the palmetto-gallberry live 0.25 - 1.0" load.
*
*  \param age      Age of rough (years).
*  \param height   Height of the understory (ft).
*
*  \return Palmetto-gallbery live 0.25 - 1.0" load (lb/ft2).
*/

double PalmettoGallberry::calculatePalmettoGallberyLiveTenHourLoad(double ageOfRough, double heightOfUnderstory)
{
    palmettoGallberyLiveTenHourLoad_ = -0.02128
        + 0.00014 * ageOfRough * ageOfRough
        + 0.00314 * heightOfUnderstory * heightOfUnderstory;
    if (palmettoGallberyLiveTenHourLoad_ < 0.0)
    {
        palmettoGallberyLiveTenHourLoad_ = 0.0;
    }
    return palmettoGallberyLiveTenHourLoad_;
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

double PalmettoGallberry::calculatePalmettoGallberyLiveFoliageLoad(double ageOfRough, double palmettoCoverage,
    double heightOfUnderstory)
{
    palmettoGallberyLiveFoliageLoad_ = -0.0036
        + 0.00253 * ageOfRough
        + 0.00049 * palmettoCoverage
        + 0.00282 * heightOfUnderstory * heightOfUnderstory;
    if (palmettoGallberyLiveFoliageLoad_ < 0.0)
    {
        palmettoGallberyLiveFoliageLoad_ = 0.0;
    }
    return palmettoGallberyLiveFoliageLoad_;
}

double PalmettoGallberry::getMoistureOfExtinctionDead() const
{
    return moistureOfExtinctionDead_;
}

double PalmettoGallberry::getHeatOfCombustionDead() const
{
    return heatOfCombustionDead_;
}

double PalmettoGallberry::getHeatOfCombustionLive() const
{
    return heatOfCombustionLive_;
}

double PalmettoGallberry::getPalmettoGallberyDeadOneHourLoad() const
{
    return palmettoGallberyDeadOneHourLoad_;
}

double PalmettoGallberry::getPalmettoGallberyDeadTenHourLoad() const
{
    return palmettoGallberyDeadTenHourLoad_;
}

double PalmettoGallberry::getPalmettoGallberyDeadFoliageLoad() const
{
    return palmettoGallberyDeadFoliageLoad_;
}

double PalmettoGallberry::getPalmettoGallberyFuelBedDepth() const
{
    return palmettoGallberyFuelBedDepth_;
}

double PalmettoGallberry::getPalmettoGallberyLitterLoad() const
{
    return palmettoGallberyLitterLoad_;
}

double PalmettoGallberry::getPalmettoGallberyLiveOneHourLoad() const
{
    return palmettoGallberyLiveOneHourLoad_;
}

double PalmettoGallberry::getPalmettoGallberyLiveTenHourLoad() const
{
    return palmettoGallberyLiveTenHourLoad_;
}

double PalmettoGallberry::getPalmettoGallberyLiveFoliageLoad() const
{
    return palmettoGallberyLiveFoliageLoad_;
}
