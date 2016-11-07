/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the Western Aspen special case fuel model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
******************************************************************************
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

#include "westernAspen.h"

#include <cmath>

WesternAspen::WesternAspen()
{

}

WesternAspen::WesternAspen(const WesternAspen& rhs)
{
    mortality_ = rhs.mortality_;
}

WesternAspen& WesternAspen::operator = (const WesternAspen& rhs)
{
    if (this != &rhs)
    {
        mortality_ = rhs.mortality_;
    }
    return *this;
}

WesternAspen::~WesternAspen()
{

}

void WesternAspen::initializeMembers()
{
    mortality_ = 0.0;
}

double WesternAspen::getAspenMortality () const
{
    return mortality_;
}

//------------------------------------------------------------------------------
/*! \brief Returns the interpolated/extrapolated value based upon curing.
*
*  \param curing        Curing level (fraction)
*  \param valueArray    Array of 6 boundary values.
*
*  \return value        Interpolated value
*/
double WesternAspen::aspenInterpolate(double curing, double* valueArray)
{
    static double curingArray[] = { 0.0, 0.3, 0.5, 0.7, 0.9, 1.000000001 };
    curing = (curing < 0.0) ? 0.0 : curing;
    curing = (curing > 1.0) ? 1.0 : curing;
    double fraction = 0.0;
    unsigned i = 1;
    for (i = 1; i<sizeof(curingArray) - 1; i++)
    {
        if (curing < curingArray[i])
        {
            fraction = 1.0 - (curingArray[i] - curing) / (curingArray[i] - curingArray[i - 1]);
            break;
        }
    }
    double value = valueArray[i - 1] + fraction * (valueArray[i] - valueArray[i - 1]);
    return value;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen fuel bed depth.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \return Aspen fuel bed depth (ft).
*/

double WesternAspen::getAspenFuelBedDepth(int aspenFuelModelNumber)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    double Depth[] = { 0.65, 0.30, 0.18, 0.50, 0.18 };
    return Depth[aspenFuelModelIndex];
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen dead fuel moisture of extinction
*
*  \return dead fuel moisture of extinction.
*/

double WesternAspen::getAspenMoistureOfExtinctionDead()
{
    return 0.25;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen dead fuel heat of combustion
*
*  \return dead fuel heat of combustion.
*/

double WesternAspen::getAspenHeatOfCombustionDead()
{
    return 8000.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen live fuel heat of combustion
*
*  \return live fuel heat of combustion.
*/

double WesternAspen::getAspenHeatOfCombustionLive()
{
    return 8000.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen dead 0.0 - 0.25" load.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \param aspenCuringLevel      Curing level (fraction)
*
*  \return load                 Aspen dead 0.0 - 0.25" load (lb/ft2).
*/

double WesternAspen::getAspenLoadDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    static double Load[5][6] = 
    {
        { 0.800, 0.893, 1.056, 1.218, 1.379, 1.4595 },
        { 0.738, 0.930, 1.056, 1.183, 1.309, 1.3720 },
        { 0.601, 0.645, 0.671, 0.699, 0.730, 0.7455 },
        { 0.880, 0.906, 1.037, 1.167, 1.300, 1.3665 },
        { 0.754, 0.797, 0.825, 0.854, 0.884, 0.8990 }
    };
    double load = 0.0;
    if (aspenFuelModelIndex >= 0 && aspenFuelModelIndex < 5)
    {
        load = aspenInterpolate(aspenCuringLevel, Load[aspenFuelModelIndex]);
    }
    return load * 2000.0 / 43560.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen dead 0.25 - 1.0" load.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \return load                 Aspen dead 0.25 - 1.0" load (lb/ft2).
*/

double WesternAspen::getAspenLoadDeadTenHour(int aspenFuelModelNumber)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    static double Load[] = { 0.975, 0.475, 1.035, 1.340, 1.115 };
    double load = 0.0;
    if (aspenFuelModelIndex >= 0 && aspenFuelModelIndex < 5)
    {
        load = Load[aspenFuelModelIndex];
    }
    return load * 2000.0 / 43560.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen live herbaceous load.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \param aspenCuringLevel      Curing level (fraction)
*
*  \return load                 Aspen live herbaceous load (lb/ft2).
*/

double WesternAspen::getAspenLoadLiveHerbaceous(int aspenFuelModelNumber, double aspenCuringLevel)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    static double Load[5][6] = 
    {
        { 0.335, 0.234, 0.167, 0.100, 0.033, 0.000 },
        { 0.665, 0.465, 0.332, 0.199, 0.067, 0.000 },
        { 0.150, 0.105, 0.075, 0.045, 0.015, 0.000 },
        { 0.100, 0.070, 0.050, 0.030, 0.010, 0.000 },
        { 0.150, 0.105, 0.075, 0.045, 0.015, 0.000 }
    };
    double load = 0.0;
    if (aspenFuelModelIndex >= 0 && aspenFuelModelIndex < 5)
    {
        load = aspenInterpolate(aspenCuringLevel, Load[aspenFuelModelIndex]);
    }
    return load * 2000.0 / 43560.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen live woody load.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \param aspenCuringLevel      Curing level (fraction)
*
*  \return load                 Aspen live woody load (lb/ft2).
*/

double WesternAspen::getAspenLoadLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    static double Load[5][6] = 
    {
        { 0.403, 0.403, 0.333, 0.283, 0.277, 0.274 },
        { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000 },
        { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000 },
        { 0.455, 0.455, 0.364, 0.290, 0.261, 0.2465 },
        { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000 }
    };
    double load = 0.0;
    if (aspenFuelModelIndex >= 0 && aspenFuelModelIndex < 5)
    {
        load = aspenInterpolate(aspenCuringLevel, Load[aspenFuelModelIndex]);
    }
    return load * 2000. / 43560.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen mortality rate.
*
*  \param severity Fire severity level: 0 = low severity, 1= moderate+ severity
*  \param flameLength Flame length of the fire at the tree (ft).
*  \param dbh          Aspen diameter at breast height (in).
*
*  \return Aspen mortality rate (fraction).
*/

double WesternAspen::calculateAspenMortality(int severity, double flameLength, double DBH)
{
    double mortality = 1.0;
    double charHeight = flameLength / 1.8;
    if (severity == 0)
    {
        mortality = 1.0 / (1.0 + exp(-4.407 + 0.638 * DBH - 2.134 * charHeight));
    }
    else if (severity == 1)
    {
        mortality = 1.0 / (1.0 + exp(-2.157 + 0.218 * DBH - 3.600 * charHeight));
    }
    mortality_ = (mortality < 0.0) ? 0.0 : mortality;
    mortality_ = (mortality > 1.0) ? 1.0 : mortality;
    return mortality_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen dead 0.0 - 0.25" savr.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \param aspenCuringLevel      Curing level (fraction)
*
*  \return Aspen dead 0.0 - 0.25" savr (ft2/ft3).
*/

double WesternAspen::getAspenSavrDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    static double Savr[5][6] = 
    {
        { 1440., 1620., 1910., 2090., 2220., 2285. },
        { 1480., 1890., 2050., 2160., 2240., 2280. },
        { 1400., 1540., 1620., 1690., 1750., 1780. },
        { 1350., 1420., 1710., 1910., 2060., 2135. },
        { 1420., 1540., 1610., 1670., 1720., 1745. }
    };
    double savr = 1440.;
    if (aspenFuelModelIndex >= 0 && aspenFuelModelIndex < 5)
    {
        savr = aspenInterpolate(aspenCuringLevel, Savr[aspenFuelModelIndex]);
    }
    return savr;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen dead 0.25 - 1.0" savr.
*
*  \return Aspen dead 0.25 - 1.0" savr (ft2/ft3).
*/

double WesternAspen::getAspenSavrDeadTenHour()
{
    return 109.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen live herbaceous savr.
*
*  \return Aspen live herbaceous savr (ft2/ft3).
*/

double WesternAspen::getAspenSavrLiveHerbaceous()
{
    return 2800.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen live woody savr.
*
*  \param aspenFuelModelNumber  ID number of the aspen fuel type:
*                               1 = Aspen/shrub
*                               2 = Aspen/tall forb
*                               3 = Aspen/low forb
*                               4 = Mixed/forb
*                               5 = Mixed/shrub
*
*  \param aspenCuringLevel      Curing level (fraction)
*
*  \return Aspen live woody savr (ft2/ft3).
*/
double WesternAspen::getAspenSavrLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel)
{
    int aspenFuelModelIndex = aspenFuelModelNumber - 1;
    double Savr[5][6] = 
    {
        { 2440., 2440., 2310., 2090., 1670., 1670. },
        { 2440., 2440., 2440., 2440., 2440., 2440. },
        { 2440., 2440., 2440., 2440., 2440., 2440. },
        { 2530., 2530., 2410., 2210., 1800., 1800. },
        { 2440., 2440., 2440., 2440., 2440., 2440. }
    };
    double savr = 2440.;
    if (aspenFuelModelIndex >= 0 && aspenFuelModelIndex < 5)
    {
        savr = aspenInterpolate(aspenCuringLevel, Savr[aspenFuelModelIndex]);
    }
    return savr;
}
