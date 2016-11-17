/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  A collection of units conversion functions and enums
* Author:   William Chatham <wchatham@fs.fed.us>
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

#include <cmath>
#include "behaveUnits.h"

double LengthUnits::toBaseUnits(double value, LengthUnits::LengthUnitsEnum units)
{
    // Length to base units constants
    static const double METERS_TO_FEET = 3.28084;
    static const double MILES_TO_FEET = 5280.0;
    static const double KILOMETERS_TO_FEET = 3280.84;

    switch (units)
    {
    case FEET:
        // Already in base, nothing to do
        break;
    case METERS:
        value *= METERS_TO_FEET;
        break;
    case MILES:
        value *= MILES_TO_FEET;
        break;
    case KILOMETERS:
        value *= KILOMETERS_TO_FEET;
        break;
    default:
        ; // TODO: Handle error
    }
    return value;
}

double LengthUnits::fromBaseUnits(double value, LengthUnits::LengthUnitsEnum units)
{
    // Length from base units constants
    static const double FEET_TO_METERS = 0.3048;
    static const double FEET_TO_MILES = 0.000189394;
    static const double FEET_TO_KILOMETERS = 0.0003048;

    switch (units)
    {
    case FEET:
        // Already in base, nothing to do
        break;
    case METERS:
        value *= FEET_TO_METERS;
        break;
    case MILES:
        value *= FEET_TO_MILES;
        break;
    case KILOMETERS:
        value *= FEET_TO_KILOMETERS;
        break;
    default:
        ; // TODO: Handle error
    }
    return value;
}

double VelocityUnits::toBaseUnits(double value, VelocityUnits::VelocityUnitsEnum units)
{
    // Velocity to base units constants
    static const double METERS_PER_SECOND_TO_FEET_PER_MINUTE = 196.8503937;
    static const double CHAINS_PER_HOUR_TO_FEET_PER_MINUTE = 1.1;
    static const double MILES_PER_HOUR_TO_FEET_PER_MINUTE = 88;
    static const double KILOMETERS_PER_HOUR_TO_FEET_PER_MINUTE = 54.680665;

    switch (units)
    {
    case FEET_PER_MINUTE:
        // Already in base, nothing to do
        break;
    case METERS_PER_SECOND:
        value *= METERS_PER_SECOND_TO_FEET_PER_MINUTE;
        break;
    case CHAINS_PER_HOUR:
        value *= CHAINS_PER_HOUR_TO_FEET_PER_MINUTE;
        break;
    case MILES_PER_HOUR:
        value *= MILES_PER_HOUR_TO_FEET_PER_MINUTE;
        break;
    case KILOMETERS_PER_HOUR:
        value *= KILOMETERS_PER_HOUR_TO_FEET_PER_MINUTE;
        break;
    default:
        ; // TODO: Handle error
    }
    return value;
}

double VelocityUnits::fromBaseUnits(double value, VelocityUnits::VelocityUnitsEnum units)
{
    // Velocity from base units constants
    static const double FEET_PER_MINUTE_TO_METERS_PER_SECOND = 0.00508;
    static const double FEET_PER_MINUTE_TO_CHAINS_PER_HOUR = 0.909091;
    static const double FEET_PER_MINUTE_TO_MILES_PER_HOUR = 0.0113636;
    static const double FEET_PER_MINUTE_TO_KILOMETERS_PER_HOUR = 0.018288;

    switch (units)
    {
    case FEET_PER_MINUTE:
        // Already in base, nothing to do
        break;
    case METERS_PER_SECOND:
        value *= FEET_PER_MINUTE_TO_METERS_PER_SECOND;
        break;
    case CHAINS_PER_HOUR:
        value *= FEET_PER_MINUTE_TO_CHAINS_PER_HOUR;
        break;
    case MILES_PER_HOUR:
        value *= FEET_PER_MINUTE_TO_MILES_PER_HOUR;
        break;
    case KILOMETERS_PER_HOUR:
        value *= FEET_PER_MINUTE_TO_KILOMETERS_PER_HOUR;
        break;
    default:
        ; // TODO: Handle error
    }
    return value;
}

double CoverUnits::toBaseUnits(double value, CoverUnitsEnum units)
{
    if (units == PERCENT)
    {
        value /= 100.0;
    }
    return value;
}

double CoverUnits::fromBaseUnits(double value, CoverUnitsEnum units)
{
    if (units == PERCENT)
    {
        value *= 100.0;
    }
    return value;
}

double SlopeUnits::toBaseUnits(double value, SlopeUnitsEnum units)
{
    static const double PI = 3.141592653589793238463;

    if (units == PERCENT)
    {    
        value = (180 / PI) * atan(value / 100.0); // slope is now in degees
    }
    return value;
}

double SlopeUnits::fromBaseUnits(double value, SlopeUnitsEnum units)
{
    static const double PI = 3.141592653589793238463;

    if (units == PERCENT)
    {
        value = tan(value * (PI / 180)) * 100; // slope is now in percent
    }
    return value;
}

double Density::toBaseUnits(double value, DensityUnitsEnum units)
{
    // Denisty to base units constants
    static const double KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT = 0.062427961;

    if (units == KILOGRAMS_PER_CUBIC_METER)
    {
        value *= KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT;
    }
    return value;
}

double Density::fromBaseUnits(double value, DensityUnitsEnum units)
{
    // Denisty from base units constants
    static const double LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER = 16.01853255;

    if (units == KILOGRAMS_PER_CUBIC_METER)
    {
        value *= LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER;
    }
    return value;
}
