/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating the probability of ignition from a firebrand
*           and from lightning
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this cpp file is, in part or in
*           whole, from BehavePlus5 source originally authored by Collin D.
*           Bevins and is used with or without modification.
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

#include "ignite.h"

#include <cmath>
#include "igniteEnums.h"

Ignite::Ignite()
{

}

Ignite::~Ignite()
{

}

//------------------------------------------------------------------------------
/*! \brief Calculates the probability of a firebrand starting a fire.
*
*  \param fuelTemperature  Dead surface fuel temperature (oF).
*  \param fuelMoisture     Dead surface fuel moisture content (lb/lb).
*
*  \return Probability of a firebrand starting a fire [0..1].
*/

double Ignite::calculateFirebrandIgnitionProbability(double fuelTemperature,
    double fuelMoisture)
{
    // Covert Fahrenheit to Celcius
    double localFuelTemperature = (fuelTemperature - 32.0) * 5.0 / 9.0;
    double heatOfIgnition = 144.51
        - 0.26600 * localFuelTemperature
        - 0.00058 * localFuelTemperature * localFuelTemperature
        - localFuelTemperature * fuelMoisture
        + 18.5400 * (1.0 - exp(-15.1 * fuelMoisture))
        + 640.000 * fuelMoisture;
    if (heatOfIgnition > 400.0)
    {
        heatOfIgnition = 400.0;
    }

    double x = 0.1 * (400.0 - heatOfIgnition);
    double probabilityOfIgnition = (0.000048 * pow(x, 4.3)) / 50.0;
    if (probabilityOfIgnition > 1.0)
    {
        probabilityOfIgnition = 1.0;
    }
    else if (probabilityOfIgnition < 0.0)
    {
        probabilityOfIgnition = 0.0;
    }
    return(probabilityOfIgnition);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the fuel temperature using the BEHAVE FIRE2 subroutine
*  CAIGN() algorithm.
*
*  \param airTemperature Air temperature (oF).
*  \param sunShade       Fraction of sun shaded from the fuel.
*
*  \return Fuel temperature (oF).
*/

double Ignite::calculateFuelTemperature(double airTemperature, double sunShade)
{
    double localAirTemperature;
    double temperatureDifferential;

    // Temperature differential depends upon shading
#ifdef __DEPRECATED__
    temperatureDifferential = 5.0;
    if (sunShade <= 0.10)
    {
        temperatureDifferential = 25.0;
    }
    else if (sunShade <= 0.50)
    {
        temperatureDifferential = 19.0;
    }
    else if (sunShade <= 0.90)
    {
        temperatureDifferential = 12.0;
    }
    else
    {
        temperatureDifferential = 5.0;
    }

    // FIRE2 SUBROUTINE CAIGN() restricts air temp to 5-degree intervals
    int intervalAirTemperature = (int)(airTemperature / 10.0);
    localAirTemperature = 5.0 + (10. * intervalAirTemperature);
#endif

    // But we are going to use the continuum
    localAirTemperature = airTemperature;

    // This could be approximated by amountIncrease = 25.0 - (20.0 * sunShade);
    temperatureDifferential = 25.0 - (20.0 * sunShade);
    return(localAirTemperature + temperatureDifferential);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the probability of a lightning strike starting a fire.
*
*  \param fuelType Ignition fuel bed type:
*                      0 == Ponderosa Pine Litter
*                      1 == Punky wood, rotten, chunky
*                      2 == Punky wood powder, deep (4.8 cm)
*                      3 == Punk wood powder, shallow (2.4 cm)
*                      4 == Lodgepole pine duff
*                      5 == Douglas-fir duff
*                      6 == High altitude mixed (mainly Engelmann spruce)
*                      7 == Peat moss (commercial)
*  \param depth    Ignition fuel bed depth (inches).
*  \param moisture Ignition fuel moisture content, uses 100-hr fuel moisture (lb/lb). 
*  \param charge   Lightning charge:
*                      0 == negative,
*                      1 == positive,
*                      2 == unknown
*
*  \note  The following assumptions are made by Latham:
*      - 20% of negative flashes have continuing current
*      - 90% of positive flashes have continuing current
*      - Latham and Schlieter found a relative frequency of
*          0.723 negative and 0.277 positive strikes
*      - Unknown strikes are therefore p = 0.1446 neg + 0.2493 pos
*
*  \return Probability of the lightning strike starting a fire [0..1].
*/

double Ignite::calculateLightningIgnitionProbability(int fuelType, double depth,
    double moisture, int charge)
{
    // Probability of continuing current by charge type (Latham)
    static const double ccNeg = 0.2;
    static const double ccPos = 0.9;

    // Relative frequency by charge type (Latham and Schlieter)
    static const double freqNeg = 0.723;
    static const double freqPos = 0.277;

    // Convert duff depth to cm and restrict to maximum of 10 cm.
    depth *= 2.54;
    if (depth > 10.0)
    {
        depth = 10.0;
    }

    // Convert duff moisture to percent and restrict to maximum of 40%.
    moisture *= 100.0;
    if (moisture > 40.0)
    {
        moisture = 40.0;
    }

    // Ponderosa Pine Litter
    double pPos = 0.0;
    double pNeg = 0.0;
    double prob = 0.0;
    if (fuelType == IgnitionFuelBedType::PONDEROSA_PINE_LITTER)
    {
        pPos = 0.92 * exp(-0.087 * moisture);
        pNeg = 1.04 * exp(-0.054 * moisture);
    }
    // Punky wood, rotten, chunky
    else if (fuelType == IgnitionFuelBedType::PUNKY_WOOD_ROTTEN_CHUNKY)
    {
        pPos = 0.44 * exp(-0.110 * moisture);
        pNeg = 0.59 * exp(-0.094 * moisture);
    }
    // Punky wood powder, deep (4.8 cm)
    else if (fuelType == IgnitionFuelBedType::PUNKY_WOOD_POWDER_DEEP)
    {
        pPos = 0.86 * exp(-0.060 * moisture);
        pNeg = 0.90 * exp(-0.056 * moisture);
    }
    // Punk wood powder, shallow (2.4 cm)
    else if (fuelType == IgnitionFuelBedType::PUNK_WOOD_POWDER_SHALLOW)
    {
        pPos = 0.60 - (0.011 * moisture);
        pNeg = 0.73 - (0.011 * moisture);
    }
    // Lodgepole pine duff
    else if (fuelType == IgnitionFuelBedType::LODGEPOLE_PINE_DUFF)
    {
        pPos = 1.0 / (1.0 + exp(5.13 - 0.68 * depth));
        pNeg = 1.0 / (1.0 + exp(3.84 - 0.60 * depth));
    }
    // Douglas-fir duff
    else if (fuelType == IgnitionFuelBedType::DOUGLAS_FIR_DUFF)
    {
        pPos = 1.0 / (1.0 + exp(6.69 - 1.39 * depth));
        pNeg = 1.0 / (1.0 + exp(5.48 - 1.28 * depth));
    }
    // High altitude mixed (mainly Engelmann spruce)
    else if (fuelType == IgnitionFuelBedType::HIGH_ALTITUDE_MIXED)
    {
        pPos = 0.62 * exp(-0.050 * moisture);
        pNeg = 0.80 - (0.014 * moisture);
    }
    // Peat moss (commercial)
    else if (fuelType == IgnitionFuelBedType::PEAT_MOSS)
    {
        pPos = 0.71 * exp(-0.070 * moisture);
        pNeg = 0.84 * exp(-0.060 * moisture);
    }

    // Return requested result
    if (charge == LightningCharge::NEGATIVE)
    {
        prob = ccNeg * pNeg;
    }
    else if (charge == LightningCharge::POSITIVE)
    {
        prob = ccPos * pPos;
    }
    else if (charge == LightningCharge::UNKNOWN)
    {
        prob = freqPos * ccPos * pPos
            + freqNeg * ccNeg * pNeg;
    }

    // Constrain result
    if (prob < 0.0)
    {
        prob = 0.0;
    }
    if (prob > 1.0)
    {
        prob = 1.0;
    }
    return(prob);
}
