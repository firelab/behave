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
#include "igniteInputs.h"

Ignite::Ignite()
{

}

Ignite::~Ignite()
{

}

double Ignite::calculateFirebrandIgnitionProbability(ProbabilityUnits::ProbabilityUnitsEnum desiredUnits)
{
    // Covert temperature to Celcius
    calculateFuelTemperature();
    double fuelTemperature = getFuelTemperature(TemperatureUnits::CELSIUS);

    // use one hour moisture in the following calculation
    double fuelMoisture = igniteInputs_.getMoistureOneHour(MoistureUnits::FRACTION);; 

    // Calculate heat of ignition
    double heatOfIgnition = 144.51
        - 0.26600 * fuelTemperature
        - 0.00058 * fuelTemperature * fuelTemperature
        - fuelTemperature * fuelMoisture
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

    return ProbabilityUnits::fromBaseUnits(probabilityOfIgnition, desiredUnits);
}

double Ignite::calculateFuelTemperature()
{
    double temperatureDifferential;

    double sunShade = igniteInputs_.getSunShade(CoverUnits::FRACTION);
    double airTemperature = igniteInputs_.getAirTemperature(TemperatureUnits::FAHRENHEIT);

    temperatureDifferential = 25.0 - (20.0 * sunShade);

    fuelTemperature_ = airTemperature + temperatureDifferential;
    return fuelTemperature_;
}

double Ignite::calculateLightningIgnitionProbability(ProbabilityUnits::ProbabilityUnitsEnum desiredUnits)
{
    /*
    *      The following assumptions are made by Latham:
    *      - 20% of negative flashes have continuing current
    *      - 90% of positive flashes have continuing current
    *      - Latham and Schlieter found a relative frequency of
    *          0.723 negative and 0.277 positive strikes
    *      - Unknown strikes are therefore p = 0.1446 neg + 0.2493 pos
    */

    // Probability of continuing current by charge type (Latham)
    static const double ccNeg = 0.2;
    static const double ccPos = 0.9;

    // Relative frequency by charge type (Latham and Schlieter)
    static const double freqNeg = 0.723;
    static const double freqPos = 0.277;

    // Convert duff depth to cm and restrict to maximum of 10 cm.
    double duffDepth = igniteInputs_.getDuffDepth(LengthUnits::CENTIMETERS);

    duffDepth *= 2.54;
    if (duffDepth > 10.0)
    {
        duffDepth = 10.0;
    }

    //  use hundred hour moisture as duff moisture and conver to percent and restrict to maximum of 40%.
    double fuelMoisture = igniteInputs_.getMoistureHundredHour(MoistureUnits::PERCENT);
    if (fuelMoisture > 40.0)
    {
        fuelMoisture = 40.0;
    }

    double pPos = 0.0;
    double pNeg = 0.0;
    double probabilityOfLightningIgnition = 0.0;

    IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelType = igniteInputs_.getIgnitionFuelBedType();
    switch (fuelType)
    {
        case IgnitionFuelBedType::PONDEROSA_PINE_LITTER:
        {
            pPos = 0.92 * exp(-0.087 * fuelMoisture);
            pNeg = 1.04 * exp(-0.054 * fuelMoisture);
            break;
        }
        case IgnitionFuelBedType::PUNKY_WOOD_ROTTEN_CHUNKY:
        {
            pPos = 0.44 * exp(-0.110 * fuelMoisture);
            pNeg = 0.59 * exp(-0.094 * fuelMoisture);
            break;
        }
        case IgnitionFuelBedType::PUNKY_WOOD_POWDER_DEEP:
        {
            pPos = 0.86 * exp(-0.060 * fuelMoisture);
            pNeg = 0.90 * exp(-0.056 * fuelMoisture);
            break;
        }
        case IgnitionFuelBedType::PUNK_WOOD_POWDER_SHALLOW:
        {
            pPos = 0.60 - (0.011 * fuelMoisture);
            pNeg = 0.73 - (0.011 * fuelMoisture);
            break;
        }
        case IgnitionFuelBedType::LODGEPOLE_PINE_DUFF:
        {
            pPos = 1.0 / (1.0 + exp(5.13 - 0.68 * duffDepth));
            pNeg = 1.0 / (1.0 + exp(3.84 - 0.60 * duffDepth));
            break;
        }
        case IgnitionFuelBedType::DOUGLAS_FIR_DUFF:
        {
            pPos = 1.0 / (1.0 + exp(6.69 - 1.39 * duffDepth));
            pNeg = 1.0 / (1.0 + exp(5.48 - 1.28 * duffDepth));
            break;
        }
        case IgnitionFuelBedType::HIGH_ALTITUDE_MIXED:
        {
            pPos = 0.62 * exp(-0.050 * fuelMoisture);
            pNeg = 0.80 - (0.014 * fuelMoisture);
            break;
        }
        case IgnitionFuelBedType::PEAT_MOSS:
        {
            pPos = 0.71 * exp(-0.070 * fuelMoisture);
            pNeg = 0.84 * exp(-0.060 * fuelMoisture);
            break;
        }
    }

    LightningCharge::LightningChargeEnum charge = igniteInputs_.getLightningChargeType();
    switch (charge)
    {
        case LightningCharge::NEGATIVE:
        {
            probabilityOfLightningIgnition = ccNeg * pNeg;
            break;
        }
        case  LightningCharge::POSITIVE:
        {
            probabilityOfLightningIgnition = ccPos * pPos;
            break;
        }
        case LightningCharge::UNKNOWN:
        {
            probabilityOfLightningIgnition = freqPos * ccPos * pPos + freqNeg * ccNeg * pNeg;
            break;
        }
    }

    // Constrain result
    if (probabilityOfLightningIgnition < 0.0)
    {
        probabilityOfLightningIgnition = 0.0;
    }
    if (probabilityOfLightningIgnition > 1.0)
    {
        probabilityOfLightningIgnition = 1.0;
    }

    return ProbabilityUnits::fromBaseUnits(probabilityOfLightningIgnition, desiredUnits);
}

void Ignite::setAirTemperature(double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnites)
{
    igniteInputs_.setAirTemperature(airTemperature, temperatureUnites);
}

void Ignite::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum desiredUnits)
{
    igniteInputs_.setMoistureOneHour(moistureOneHour, desiredUnits);
}

void Ignite::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum desiredUnits)
{
    igniteInputs_.setMoistureOneHour(moistureHundredHour, desiredUnits);
}

void Ignite::setSunShade(double sunShade, CoverUnits::CoverUnitsEnum sunShadeUnits)
{
    igniteInputs_.setSunShade(sunShade, sunShadeUnits);
}

void Ignite::setIgnitionFuelBedType(IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType_)
{
    igniteInputs_.setIgnitionFuelBedType(fuelBedType_);
}

void Ignite::setDuffDepth(double duffDepth, LengthUnits::LengthUnitsEnum lengthUnits)
{
    igniteInputs_.setDuffDepth(duffDepth, lengthUnits);
}

void Ignite::setLightningChargeType(LightningCharge::LightningChargeEnum lightningChargeType)
{
    igniteInputs_.setLightningChargeType(lightningChargeType);
}

void Ignite::updateIgniteInputs(double moistureOneHour, double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits,
    double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits, double sunShade, CoverUnits::CoverUnitsEnum sunShadeUnits,
    IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType, double duffDepth, LengthUnits::LengthUnitsEnum duffDepthUnits,
    LightningCharge::LightningChargeEnum lightningChargeType)
{
    igniteInputs_.updateIgniteInputs(moistureOneHour, moistureHundredHour, moistureUnits, airTemperature,
        temperatureUnits, sunShade, sunShadeUnits, fuelBedType, duffDepth, duffDepthUnits, lightningChargeType);
}

double Ignite::getAirTemperature(TemperatureUnits::TemperatureUnitsEnum desiredUnits)
{
    return igniteInputs_.getAirTemperature(desiredUnits);
}

double Ignite::getFuelTemperature(TemperatureUnits::TemperatureUnitsEnum desiredUnits)
{
    return TemperatureUnits::fromBaseUnits(fuelTemperature_, desiredUnits);
}

double Ignite::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum desiredUnits)
{
    return igniteInputs_.getMoistureOneHour(desiredUnits);
}

double Ignite::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum desiredUnits)
{
    return igniteInputs_.getMoistureHundredHour(desiredUnits);
}

double Ignite::getSunShade(CoverUnits::CoverUnitsEnum desiredUnits)
{
    return igniteInputs_.getSunShade(desiredUnits);
}

IgnitionFuelBedType::IgnitionFuelBedTypeEnum Ignite::getFuelBedType()
{
    return igniteInputs_.getIgnitionFuelBedType();
}

double Ignite::getDuffDepth(LengthUnits::LengthUnitsEnum desiredUnits)
{
    return igniteInputs_.getDuffDepth(desiredUnits);
}

LightningCharge::LightningChargeEnum Ignite::getLightningChargeType()
{
    return igniteInputs_.getLightningChargeType();
}

bool Ignite::isFuelDepthNeeded()
{
    IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType = igniteInputs_.getIgnitionFuelBedType();
    bool isNeeded = false;

    if (fuelBedType == IgnitionFuelBedType::LODGEPOLE_PINE_DUFF || fuelBedType == IgnitionFuelBedType::DOUGLAS_FIR_DUFF)
    {
        bool isNeeded = true;
    }
    return isNeeded;
}
