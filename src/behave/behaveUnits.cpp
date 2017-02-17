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
    const double METERS_TO_FEET = 3.2808398950131;
    const double MILES_TO_FEET = 5280.0;
    const double KILOMETERS_TO_FEET = 3280.8398950131;

    switch (units)
    {
        case FEET:
        {
            // Already in base, nothing to do
            break;
        }
        case METERS:
        {
            value *= METERS_TO_FEET;
            break;
        }
        case MILES:
        {
            value *= MILES_TO_FEET;
            break;
        }
        case KILOMETERS:
        {
            value *= KILOMETERS_TO_FEET;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double LengthUnits::fromBaseUnits(double value, LengthUnits::LengthUnitsEnum units)
{
    // Length from base units constants
    const double FEET_TO_METERS = 0.3048;
    const double FEET_TO_MILES = 0.000189394;
    const double FEET_TO_KILOMETERS = 0.0003048;

    switch (units)
    {
        case FEET:
        {
            // Already in base, nothing to do
            break;
        }
        case METERS:
        {
            value *= FEET_TO_METERS;
            break;
        }
        case MILES:
        {
            value *= FEET_TO_MILES;
            break;
        }
        case KILOMETERS:
        {
            value *= FEET_TO_KILOMETERS;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double SpeedUnits::toBaseUnits(double value, SpeedUnits::SpeedUnitsEnum units)
{
    // Velocity to base units constants
    const double METERS_PER_SECOND_TO_FEET_PER_MINUTE = 196.8503937;
    const double METERS_PER_MINUTE_TO_FEET_PER_MINUTE = 3.28084;
    const double CHAINS_PER_HOUR_TO_FEET_PER_MINUTE = 1.1;
    const double MILES_PER_HOUR_TO_FEET_PER_MINUTE = 88;
    const double KILOMETERS_PER_HOUR_TO_FEET_PER_MINUTE = 54.680665;

    switch (units)
    {
        case FEET_PER_MINUTE:
        {
            // Already in base, nothing to do
            break;
        }
        case METERS_PER_SECOND:
        {
            value *= METERS_PER_SECOND_TO_FEET_PER_MINUTE;
            break;
        }
        case   METERS_PER_MINUTE:
        {
            value *= METERS_PER_MINUTE_TO_FEET_PER_MINUTE;
            break;
        }
        case CHAINS_PER_HOUR:
        {
            value *= CHAINS_PER_HOUR_TO_FEET_PER_MINUTE;
            break;
        }
        case MILES_PER_HOUR:
        {
            value *= MILES_PER_HOUR_TO_FEET_PER_MINUTE;
            break;
        }
        case KILOMETERS_PER_HOUR:
        {
            value *= KILOMETERS_PER_HOUR_TO_FEET_PER_MINUTE;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double SpeedUnits::fromBaseUnits(double value, SpeedUnits::SpeedUnitsEnum units)
{
    // Velocity from base units constants
    const double FEET_PER_MINUTE_TO_METERS_PER_SECOND = 0.00508;
    const double FEET_PER_MINUTE_TO_METERS_PER_MINUTE = 0.3048;
    const double FEET_PER_MINUTE_TO_CHAINS_PER_HOUR = 10.0 / 11.0;
    const double FEET_PER_MINUTE_TO_MILES_PER_HOUR = 0.01136363636;
    const double FEET_PER_MINUTE_TO_KILOMETERS_PER_HOUR = 0.018288;

    switch (units)
    {
        case FEET_PER_MINUTE:
        {
            // Already in base, nothing to do
            break;
        }
        case METERS_PER_SECOND:
        {
            value *= FEET_PER_MINUTE_TO_METERS_PER_SECOND;
            break;
        }
        case   METERS_PER_MINUTE:
        {
            value *= FEET_PER_MINUTE_TO_METERS_PER_MINUTE;
            break;
        }
        case CHAINS_PER_HOUR:
        {
            value *= FEET_PER_MINUTE_TO_CHAINS_PER_HOUR;
            break;
        }
        case MILES_PER_HOUR:
        {
            value *= FEET_PER_MINUTE_TO_MILES_PER_HOUR;
            break;
        }
        case KILOMETERS_PER_HOUR:
        {
            value *= FEET_PER_MINUTE_TO_KILOMETERS_PER_HOUR;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
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

double MoistureUnits::toBaseUnits(double value, MoistureUnitsEnum units)
{
    if (units == PERCENT)
    {
        value /= 100.0;
    }
    return value;
}

double MoistureUnits::fromBaseUnits(double value, MoistureUnitsEnum units)
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

double DensityUnits::toBaseUnits(double value, DensityUnitsEnum units)
{
    // Denisty to base units constants
    static const double KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT = 0.06242796051;

    if (units == KILOGRAMS_PER_CUBIC_METER)
    {
        value *= KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT;
    }
    return value;
}

double DensityUnits::fromBaseUnits(double value, DensityUnitsEnum units)
{
    // Denisty from base units constants
    static const double LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER = 16.018463390932;

    if (units == KILOGRAMS_PER_CUBIC_METER)
    {
        value *= LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER;
    }
    return value;
}

double LoadingUnits::toBaseUnits(double value, LoadingUnitsEnum units)
{
    // Velocity to base units constants
    const double KILOGRAMS_PER_SQUARE_METER_TO_POUNDS_PER_SQUARE_FOOT = 0.2048161436225217;
    const double TONS_PER_ACRE_TO_POUNDS_PER_SQUARE_FOOT = 0.045913682277318638;
    const double TONNES_PER_HECTARE_TO_POUNDS_PER_SQUARE_FOOT = 0.02048161436225217;
 
    switch (units)
    {
        case POUNDS_PER_SQUARE_FOOT:
        {
            // Already in base, nothing to do
            break;
        }
        case TONS_PER_ACRE:
        {
            value *= TONNES_PER_HECTARE_TO_POUNDS_PER_SQUARE_FOOT;
            break;
        }
        case TONNES_PER_HECTARE:
        {
            value *= TONNES_PER_HECTARE_TO_POUNDS_PER_SQUARE_FOOT;
            break;
        }
        case KILOGRAMS_PER_SQUARE_METER:
        {
            value *= KILOGRAMS_PER_SQUARE_METER_TO_POUNDS_PER_SQUARE_FOOT;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double LoadingUnits::fromBaseUnits(double value, LoadingUnitsEnum units)
{
    // Velocity to base units constants
    const double POUNDS_PER_SQUARE_FOOT_TO_KILOGRAMS_PER_SQUARE_METER = 4.88242763638305;
    const double POUNDS_PER_SQUARE_FOOT_TO_TONS_PER_ACRE = 21.78;
    const double POUNDS_PER_SQUARE_FOOT_TO_TONNES_PER_HECTARE = 48.8242763638305;

    switch (units)
    {
        case POUNDS_PER_SQUARE_FOOT:
        {
            // Already in base, nothing to do
            break;
        }
        case TONS_PER_ACRE:
        {
            value *= POUNDS_PER_SQUARE_FOOT_TO_TONS_PER_ACRE;
            break;
        }
        case TONNES_PER_HECTARE:
        {
            value *= POUNDS_PER_SQUARE_FOOT_TO_TONNES_PER_HECTARE;
            break;
        }
        case KILOGRAMS_PER_SQUARE_METER:
        {
            value *= POUNDS_PER_SQUARE_FOOT_TO_KILOGRAMS_PER_SQUARE_METER;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double SurfaceAreaToVolumeUnits::toBaseUnits(double value, SurfaceAreaToVolumeUnitsEnum units)
{
    const double SQUARE_METERS_OVER_CUBIC_METERS_TO_SQUARE_FEET_OVER_CUBIC_FEET = 3.280839895013123;
    
    const double SQUARE_INCHES_OVER_CUBIC_INCHES_TO_SQUARE_FEET_OVER_CUBIC_FEET = 0.083333333333333;
    const double SQUARE_CENTIMETERS_OVER_CUBIC_CENTIMERS_TO_SQUARE_FEET_OVER_CUBIC_FEET = 0.03280839895013123;

    switch (units)
    {
        case SQUARE_FEET_OVER_CUBIC_FEET:
        {
            // Already in base, nothing to do
            break;
        }
        case SQUARE_METERS_OVER_CUBIC_METERS:
        {
            value *= SQUARE_METERS_OVER_CUBIC_METERS_TO_SQUARE_FEET_OVER_CUBIC_FEET;
            break;
        }
        case SQUARE_INCHES_OVER_CUBIC_INCHES:
        {
            value *= SQUARE_INCHES_OVER_CUBIC_INCHES_TO_SQUARE_FEET_OVER_CUBIC_FEET;
            break;
        }
        case SQUARE_CENTIMETERS_OVER_CUBIC_CENTIMERS:
        {
            value *= SQUARE_CENTIMETERS_OVER_CUBIC_CENTIMERS_TO_SQUARE_FEET_OVER_CUBIC_FEET;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double SurfaceAreaToVolumeUnits::fromBaseUnits(double value, SurfaceAreaToVolumeUnitsEnum units)
{
    const double SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_METERS_OVER_CUBIC_METERS = 0.3048;
    const double SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_INCHES_OVER_CUBIC_INCHES = 12;
    const double SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_CENTIMETERS_OVER_CUBIC_CENTIMERS = 30.48;

    switch (units)
    {
        case SQUARE_FEET_OVER_CUBIC_FEET:
        {
            // Already in base, nothing to do
            break;
        }
        case SQUARE_METERS_OVER_CUBIC_METERS:
        {
            value *= SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_METERS_OVER_CUBIC_METERS;
            break;
        }
        case SQUARE_INCHES_OVER_CUBIC_INCHES:
        {
            value *= SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_INCHES_OVER_CUBIC_INCHES;
            break;
        }
        case SQUARE_CENTIMETERS_OVER_CUBIC_CENTIMERS:
        {
            value *= SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_CENTIMETERS_OVER_CUBIC_CENTIMERS;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}
