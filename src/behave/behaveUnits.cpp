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
    const double INCHES_TO_FEET = 0.08333333333333;
    const double METERS_TO_FEET = 3.2808398950131;
    const double MILLIMETERS_TO_FEET = 0.003280839895;
    const double CENTIMETERS_TO_FEET = 0.03280839895;
    const double CHAINS_TO_FEET = 66.0;
    const double MILES_TO_FEET = 5280.0;
    const double KILOMETERS_TO_FEET = 3280.8398950131;

    switch (units)
    {
        case Feet:
        {
            // Already in base, nothing to do
            break;
        }
        case Inches:
        {
            value *= INCHES_TO_FEET;
            break;
        }
        case Millimeters:
        {
            value *= MILLIMETERS_TO_FEET;
            break;
        }
        case Centimeters:
        {
            value *= CENTIMETERS_TO_FEET;
            break;
        }
        case Meters:
        {
            value *= METERS_TO_FEET;
            break;
        }
        case Chains:
        {
            value *= CHAINS_TO_FEET;
            break;
        }
        case Miles:
        {
            value *= MILES_TO_FEET;
            break;
        }
        case Kilometers:
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
    const double FEET_TO_INCHES = 12;
    const double FEET_TO_CENTIMETERS = 30.480;
    const double FEET_TO_METERS = 0.3048;
    const double FEET_TO_CHAINS = 0.0151515151515;
    const double FEET_TO_MILES = 0.0001893939393939394;
    const double FEET_TO_KILOMETERS = 0.0003048;

    switch (units)
    {
        case Feet:
        {
            // Already in base, nothing to do
            break;
        }
        case Inches:
        {
            value *= FEET_TO_INCHES;
            break;
        }
        case Centimeters:
        {
            value *= FEET_TO_CENTIMETERS;
            break;
        }
        case Meters:
        {
            value *= FEET_TO_METERS;
            break;
        }
        case Chains:
        {
            value *= FEET_TO_CHAINS;
            break;
        }
        case Miles:
        {
            value *= FEET_TO_MILES;
            break;
        }
        case Kilometers:
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
    const double METERS_PER_HOUR_TO_FEET_PER_MINUTE = 0.0547;
    const double CHAINS_PER_HOUR_TO_FEET_PER_MINUTE = 1.1;
    const double MILES_PER_HOUR_TO_FEET_PER_MINUTE = 88;
    const double KILOMETERS_PER_HOUR_TO_FEET_PER_MINUTE = 54.680665;

    switch (units)
    {
        case FeetPerMinute:
        {
            // Already in base, nothing to do
            break;
        }
        case MetersPerSecond:
        {
            value *= METERS_PER_SECOND_TO_FEET_PER_MINUTE;
            break;
        }
        case   MetersPerMinute:
        {
            value *= METERS_PER_MINUTE_TO_FEET_PER_MINUTE;
            break;
        }
        case   MetersPerHour:
        {
            value *= METERS_PER_HOUR_TO_FEET_PER_MINUTE;
            break;
        }
        case ChainsPerHour:
        {
            value *= CHAINS_PER_HOUR_TO_FEET_PER_MINUTE;
            break;
        }
        case MilesPerHour:
        {
            value *= MILES_PER_HOUR_TO_FEET_PER_MINUTE;
            break;
        }
        case KilometersPerHour:
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
    const double FEET_PER_MINUTE_TO_METERS_PER_HOUR = 18.288;
    const double FEET_PER_MINUTE_TO_CHAINS_PER_HOUR = 10.0 / 11.0;
    const double FEET_PER_MINUTE_TO_MILES_PER_HOUR = 0.01136363636;
    const double FEET_PER_MINUTE_TO_KILOMETERS_PER_HOUR = 0.018288;

    switch (units)
    {
        case FeetPerMinute:
        {
            // Already in base, nothing to do
            break;
        }
        case MetersPerSecond:
        {
            value *= FEET_PER_MINUTE_TO_METERS_PER_SECOND;
            break;
        }
        case   MetersPerMinute:
        {
            value *= FEET_PER_MINUTE_TO_METERS_PER_MINUTE;
            break;
        }
        case   MetersPerHour:
        {
            value *= FEET_PER_MINUTE_TO_METERS_PER_HOUR;
            break;
        }
        case ChainsPerHour:
        {
            value *= FEET_PER_MINUTE_TO_CHAINS_PER_HOUR;
            break;
        }
        case MilesPerHour:
        {
            value *= FEET_PER_MINUTE_TO_MILES_PER_HOUR;
            break;
        }
        case KilometersPerHour:
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

double FractionUnits::toBaseUnits(double value, FractionUnitsEnum units)
{
    if (units == Percent)
    {
        value /= 100.0;
    }
    return value;
}

double FractionUnits::fromBaseUnits(double value, FractionUnitsEnum units)
{
    if (units == Percent)
    {
        value *= 100.0;
    }
    return value;
}

double SlopeUnits::toBaseUnits(double value, SlopeUnitsEnum units)
{
    static const double PI = 3.141592653589793238463;

    if (units == Percent)
    {
        value = (180 / PI) * atan(value / 100.0); // slope is now in degees
    }
    return value;
}

double SlopeUnits::fromBaseUnits(double value, SlopeUnitsEnum units)
{
    static const double PI = 3.141592653589793238463;

    if (units == Percent)
    {
        value = tan(value * (PI / 180)) * 100; // slope is now in percent
    }
    return value;
}

double DensityUnits::toBaseUnits(double value, DensityUnitsEnum units)
{
    // Denisty to base units constants
    //static const double KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT = 0.06242796051;
    static const double KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT = 0.06242781786;

    if (units == KilogramsPerCubicMeter)
    {
        value *= KG_PER_CUBIC_METER_TO_LBS_PER_CUBIC_FOOT;
    }
    return value;
}

double DensityUnits::fromBaseUnits(double value, DensityUnitsEnum units)
{
    // Denisty from base units constants
    //static const double LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER = 16.018463390932;
    static const double LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER = 16.0185;

    if (units == KilogramsPerCubicMeter)
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
        case PoundsPerSquareFoot:
        {
            // Already in base, nothing to do
            break;
        }
        case TonsPerAcre:
        {
            value *= TONS_PER_ACRE_TO_POUNDS_PER_SQUARE_FOOT;
            break;
        }
        case TonnesPerHectare:
        {
            value *= TONNES_PER_HECTARE_TO_POUNDS_PER_SQUARE_FOOT;
            break;
        }
        case KilogramsPerSquareMeter:
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
        case PoundsPerSquareFoot:
        {
            // Already in base, nothing to do
            break;
        }
        case TonsPerAcre:
        {
            value *= POUNDS_PER_SQUARE_FOOT_TO_TONS_PER_ACRE;
            break;
        }
        case TonnesPerHectare:
        {
            value *= POUNDS_PER_SQUARE_FOOT_TO_TONNES_PER_HECTARE;
            break;
        }
        case KilogramsPerSquareMeter:
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

double PressureUnits::toBaseUnits(double value, PressureUnitsEnum units) {
  // Pressure to base units constants
  const double HECTOPASCAL_TO_PASCAL     = 1e2;
  const double KILOPASCAL_TO_PASCAL      = 1e3;
  const double MEGAPASCAL_TO_PASCAL      = 1e6;
  const double GIGAPASCAL_TO_PASCAL      = 1e9;
  const double BAR_TO_PASCAL             = 1e5;
  const double ATMOSPHERE_TO_PASCAL      = 101325;
  const double TECH_ATMOSPHERE_TO_PASCAL = 98066.5;
  const double POUNDS_PER_SQUARE_INCH    = 6894.757;

  switch (units)
  {
      case Pascal:
      {
          // Already in base, nothing to do
          break;
      }
      case HectoPascal:
      {
          value /= HECTOPASCAL_TO_PASCAL;
          break;
      }
      case KiloPascal:
      {
          value /= KILOPASCAL_TO_PASCAL;
          break;
      }
      case MegaPascal:
      {
          value /= MEGAPASCAL_TO_PASCAL;
          break;
      }
      case GigaPascal:
      {
          value /= GIGAPASCAL_TO_PASCAL;
          break;
      }
      case Bar:
      {
          value /= BAR_TO_PASCAL;
          break;
      }
      case Atmosphere:
      {
          value /= ATMOSPHERE_TO_PASCAL;
          break;
      }
      case TechnicalAtmosphere:
      {
          value /= TECH_ATMOSPHERE_TO_PASCAL;
          break;
      }
      case PoundPerSquareInch:
      {
          value /= POUNDS_PER_SQUARE_INCH;
          break;
      }
  }

  return value;
}

double PressureUnits::fromBaseUnits(double value, PressureUnitsEnum units) {
  // Pressure to base units constants
  const double HECTOPASCAL_TO_PASCAL     = 1e2;
  const double KILOPASCAL_TO_PASCAL      = 1e3;
  const double MEGAPASCAL_TO_PASCAL      = 1e6;
  const double GIGAPASCAL_TO_PASCAL      = 1e9;
  const double BAR_TO_PASCAL             = 1e5;
  const double ATMOSPHERE_TO_PASCAL      = 101325;
  const double TECH_ATMOSPHERE_TO_PASCAL = 98066.5;
  const double POUNDS_PER_SQUARE_INCH    = 6894.757;

  switch (units)
  {
      case Pascal:
      {
          // Already in base, nothing to do
          break;
      }
      case HectoPascal:
      {
          value *= HECTOPASCAL_TO_PASCAL;
          break;
      }
      case KiloPascal:
      {
          value *= KILOPASCAL_TO_PASCAL;
          break;
      }
      case MegaPascal:
      {
          value *= MEGAPASCAL_TO_PASCAL;
          break;
      }
      case GigaPascal:
      {
          value *= GIGAPASCAL_TO_PASCAL;
          break;
      }
      case Bar:
      {
          value *= BAR_TO_PASCAL;
          break;
      }
      case Atmosphere:
      {
          value *= ATMOSPHERE_TO_PASCAL;
          break;
      }
      case TechnicalAtmosphere:
      {
          value *= TECH_ATMOSPHERE_TO_PASCAL;
          break;
      }
      case PoundPerSquareInch: {
          value *= POUNDS_PER_SQUARE_INCH;
          break;
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
        case SquareFeetOverCubicFeet:
        {
            // Already in base, nothing to do
            break;
        }
        case SquareMetersOverCubicMeters:
        {
            value *= SQUARE_METERS_OVER_CUBIC_METERS_TO_SQUARE_FEET_OVER_CUBIC_FEET;
            break;
        }
        case SquareInchesOverCubicInches:
        {
            value *= SQUARE_INCHES_OVER_CUBIC_INCHES_TO_SQUARE_FEET_OVER_CUBIC_FEET;
            break;
        }
        case SquareCentimetersOverCubicCentimeters:
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
        case SquareFeetOverCubicFeet:
        {
            // Already in base, nothing to do
            break;
        }
        case SquareMetersOverCubicMeters:
        {
            value *= SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_METERS_OVER_CUBIC_METERS;
            break;
        }
        case SquareInchesOverCubicInches:
        {
            value *= SQUARE_FEET_OVER_CUBIC_FEET_TO_SQUARE_INCHES_OVER_CUBIC_INCHES;
            break;
        }
        case SquareCentimetersOverCubicCentimeters:
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

double TemperatureUnits::toBaseUnits(double value, TemperatureUnitsEnum units)
{
    switch (units)
    {
        case Fahrenheit:
        {
            // Already in base, nothing to do
            break;
        }
        case Celsius:
        {
            value = ((value * 9.0) / 5.0) + 32;
            break;
        }
        case Kelvin:
        {
            value = (((value - 273.15) * 9.0) / 5.0) + 32;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double TemperatureUnits::fromBaseUnits(double value, TemperatureUnitsEnum units)
{
    switch (units)
    {
        case Fahrenheit:
        {
            // Already in base, nothing to do
            break;
        }
        case Celsius:
        {
            value = ((value - 32) * 5) / 9.0;
            break;
        }
        case Kelvin:
        {
            value = (((value - 32) * 5) / 9.0) + 273.15;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double TimeUnits::toBaseUnits(double value, TimeUnitsEnum units)
{
    switch (units)
    {
        case Minutes:
        {
            // Already in base, nothing to do
            break;
        }
        case Seconds:
        {
            value /= 60.0;
            break;
        }
        case Hours:
        {
            value *= 60;
            break;
        }
        case Days:
        {
            value *= 1440;
            break;
        }
        case Years:
        {
            value *= 525600;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double TimeUnits::fromBaseUnits(double value, TimeUnitsEnum units)
{
    switch (units)
    {
        case Minutes:
        {
            // Already in base, nothing to do
            break;
        }
        case Seconds:
        {
            value *= 60;
            break;
        }
        case Hours:
        {
            value /= 60.0;
            break;
        }
        case Days:
        {
            value /= 1440;
            break;
        }
        case Years:
        {
            value /= 525600;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double AreaUnits::toBaseUnits(double value, AreaUnitsEnum units)
{
    const double ACRES_TO_SQUARE_FEET = 43560.002160576107;
    const double HECTARES_TO_SQUARE_FEET = 107639.10416709723;
    const double SQUARE_METERS_TO_SQUARE_FEET = 10.76391041671;
    const double SQUARE_MILES_TO_SQUARE_FEET = 27878400;
    const double SQUARE_KILOMETERS_TO_SQUARE_FEET = 10763910.416709721;

    switch (units)
    {
        case SquareFeet:
        {
            // Already in base, nothing to do
            break;
        }
        case Acres:
        {
            value *= ACRES_TO_SQUARE_FEET;
            break;
        }
        case Hectares:
        {
            value *= HECTARES_TO_SQUARE_FEET;
            break;
        }
        case SquareMeters:
        {
            value *= SQUARE_METERS_TO_SQUARE_FEET;
            break;
        }
        case SquareMiles:
        {
            value *= SQUARE_MILES_TO_SQUARE_FEET;
            break;
        }
        case SquareKilometers:
        {
            value *= SQUARE_KILOMETERS_TO_SQUARE_FEET;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double AreaUnits::fromBaseUnits(double value, AreaUnitsEnum units)
{
    const double SQUARE_FEET_TO_ACRES = 2.295684e-05;
    const double SQUARE_FEET_TO_HECTARES = 0.0000092903036;
    const double SQUARE_FEET_TO_SQUARE_KILOMETERS = 9.290304e-08;
    const double SQUARE_FEET_TO_SQUARE_METERS = 0.0929030353835;
    const double SQUARE_FEET_TO_SQUARE_MILES = 3.5870064279e-08;

    switch (units)
    {
        case SquareFeet:
        {
            // Already in base, nothing to do
            break;
        }
        case Acres:
        {
            value *= SQUARE_FEET_TO_ACRES;
            break;
        }
        case Hectares:
        {
            value *= SQUARE_FEET_TO_HECTARES;
            break;
        }
        case SquareMeters:
        {
            value *= SQUARE_FEET_TO_SQUARE_METERS;
            break;
        }
        case SquareMiles:
        {
            value *= SQUARE_FEET_TO_SQUARE_MILES;
            break;
        }
        case SquareKilometers:
        {
            value *= SQUARE_FEET_TO_SQUARE_KILOMETERS;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double BasalAreaUnits::toBaseUnits(double value, BasalAreaUnitsEnum units)
{
    const double SQUARE_FEET_PER_ACRE_TO_SQUARE_METERS_PER_HECTARE = 0.229568;

    switch(units)
    {
        case SquareFeetPerAcre:
        {
            // Already in base, nothing to do
            break;
        }
        case SquareMetersPerHectare:
        {
            value *= SQUARE_FEET_PER_ACRE_TO_SQUARE_METERS_PER_HECTARE;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double BasalAreaUnits::fromBaseUnits(double value, BasalAreaUnitsEnum units)
{
    const double SQUARE_METERS_PER_HECTARE_TO_SQUARE_FEET_PER_ACRE = 4.356;
    switch(units)
    {
        case SquareFeetPerAcre:
        {
            // Already in base, nothing to do
            break;
        }
        case SquareMetersPerHectare:
        {
            value *= SQUARE_METERS_PER_HECTARE_TO_SQUARE_FEET_PER_ACRE;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double HeatOfCombustionUnits::toBaseUnits(double value, HeatOfCombustionUnitsEnum units)
{
    const double  KILOJOULES_PER_KILOGRAM_TO_BTUS_PER_POUND = 0.429592;

    switch (units)
    {
        case BtusPerPound:
        {
            // Already in base, nothing to do
            break;
        }
        case KilojoulesPerKilogram:
        {
            value *= KILOJOULES_PER_KILOGRAM_TO_BTUS_PER_POUND;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double HeatOfCombustionUnits::fromBaseUnits(double value, HeatOfCombustionUnitsEnum units)
{
    const double BTUS_PER_POUND_TO_KILOJOULES_PER_KILOGRAM = 2.32779;

    switch (units)
    {
        case BtusPerPound:
        {
            // Already in base, nothing to do
            break;
        }
        case KilojoulesPerKilogram:
        {
            value *= BTUS_PER_POUND_TO_KILOJOULES_PER_KILOGRAM;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double FirelineIntensityUnits::toBaseUnits(double value, FirelineIntensityUnitsEnum units)
{
    const double BTUS_PER_FOOT_PER_MINUTE_TO_BTUS_PER_FOOT_PER_SECOND = 0.01666666666666667;
    const double KILOJOULES_PER_METER_PER_MINUTE_TO_BTUS_PER_FOOT_PER_SECOND = 0.00481120819;
    const double KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND = 0.2886719;

    switch (units)
    {
        case BtusPerFootPerSecond:
        {
            // Already in base, nothing to do
            break;
        }
        case BtusPerFootPerMinute:
        {
            value *= BTUS_PER_FOOT_PER_MINUTE_TO_BTUS_PER_FOOT_PER_SECOND;
            break;
        }
        case KilojoulesPerMeterPerSecond:
        {
            value *= KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND;
            break;
        }
        case KilojoulesPerMeterPerMinute:
        {
            value *= KILOJOULES_PER_METER_PER_MINUTE_TO_BTUS_PER_FOOT_PER_SECOND;
            break;
        }
        case KilowattsPerMeter:
        {
            value *= KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double FirelineIntensityUnits::fromBaseUnits(double value, FirelineIntensityUnitsEnum units)
{
    const double BTUS_PER_FOOT_PER_SECOND_TO_BTUS_PER_FOOT_PER_MINUTE = 60;
    const double BTUS_PER_FOOT_PER_SECOND_TO_KILOJOULES_PER_METER_PER_MINUTE = 207.848;
    const double BTUS_PER_FOOT_PER_SECOND_TO_KILOWATTS_PER_METER = 3.464140419;

    switch (units)
    {
        case BtusPerFootPerSecond:
        {
            // Already in base, nothing to do
            break;
        }
        case BtusPerFootPerMinute:
        {
            value *= BTUS_PER_FOOT_PER_SECOND_TO_BTUS_PER_FOOT_PER_MINUTE;
            break;
        }
        case KilojoulesPerMeterPerSecond:
        {
            value *= BTUS_PER_FOOT_PER_SECOND_TO_KILOWATTS_PER_METER;
            break;
        }
        case KilojoulesPerMeterPerMinute:
        {
            value *= BTUS_PER_FOOT_PER_SECOND_TO_KILOJOULES_PER_METER_PER_MINUTE;
            break;
        }
        case KilowattsPerMeter:
        {
            value *= BTUS_PER_FOOT_PER_SECOND_TO_KILOWATTS_PER_METER;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double HeatPerUnitAreaUnits::toBaseUnits(double value, HeatPerUnitAreaUnitsEnum units)
{
    const double KILOJOULES_PER_SQUARE_METER_TO_BTUS_PER_SECOND = 0.0879872;
    const double KILOWATT_SECONDS_PER_SQUARE_METER_PER_TO_BTUS_PER_SQUARE_FOOT = 0.0879872;

    switch(units)
    {
        case BtusPerSquareFoot :
        {
            // Already in base, nothing to do
            break;
        }
        case KilojoulesPerSquareMeter:

        {
            value *= KILOJOULES_PER_SQUARE_METER_TO_BTUS_PER_SECOND;
            break;
        }
        case  KilowattSecondsPerSquareMeter:
        {
            value *= KILOWATT_SECONDS_PER_SQUARE_METER_PER_TO_BTUS_PER_SQUARE_FOOT;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double HeatPerUnitAreaUnits::fromBaseUnits(double value, HeatPerUnitAreaUnitsEnum units)
{
    const double BTUS_PER_SQUARE_FOOT_TO_KILOJOULES_PER_SQUARE_METER = 11.3653;
    const double BTUS_PER_SQUARE_FOOT_TO_KILOWATT_SECONDS_PER_SQUARE_METER = 11.3653;

    switch(units)
    {
        case BtusPerSquareFoot:
        {
            // Already in base, nothing to do
            break;
        }
        case   KilojoulesPerSquareMeter:

        {
            value *= BTUS_PER_SQUARE_FOOT_TO_KILOJOULES_PER_SQUARE_METER;
            break;
        }
        case  KilowattSecondsPerSquareMeter:
        {
            value *= BTUS_PER_SQUARE_FOOT_TO_KILOWATT_SECONDS_PER_SQUARE_METER;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double HeatSourceAndReactionIntensityUnits::toBaseUnits(double value, HeatSourceAndReactionIntensityUnitsEnum units)
{
    const double BTUS_PER_SQUARE_FOOT_PER_SECOND_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE = 60;
    const double KILOJOULES_PER_SQUARE_METER_PER_MINUTE_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE = 0.0880549963329497;
    const double KILOWATTS_PER_SQUARE_METER_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE = 5.27921783108615;

    switch (units)
    {
        case BtusPerSquareFootPerMinute:
        {
            // Already in base, nothing to do
            break;
        }
        case BtusPerSquareFootPerSecond:
        {
            value *= BTUS_PER_SQUARE_FOOT_PER_SECOND_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE;
            break;
        }
        case KilojoulesPerSquareMeterPerSecond:
        {
            value *= KILOWATTS_PER_SQUARE_METER_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE;
            break;
        }
        case KilojoulesPerSquareMeterPerMinute:
        {
            value *= KILOJOULES_PER_SQUARE_METER_PER_MINUTE_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE;
            break;
        }
        case KilowattsPerSquareMeter:
        {
            value *= KILOWATTS_PER_SQUARE_METER_TO_BTUS_PER_SQUARE_FOOT_PER_MINUTE;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double HeatSourceAndReactionIntensityUnits::fromBaseUnits(double value, HeatSourceAndReactionIntensityUnitsEnum units)
{
    const double BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_BTUS_PER_SQUARE_FOOT_PER_SECOND = 0.01666666666666667;
    const double BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_KILOJOULES_PER_SQUARE_METER_PER_MINUTE = 11.356539;
    //const double BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_KILOWATTS_PER_SQUARE_METER = 0.18927565;
    const double BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_KILOWATTS_PER_SQUARE_METER = 0.189422;

    switch (units)
    {
        case BtusPerSquareFootPerMinute:
        {
            // Already in base, nothing to do
            break;
        }
        case BtusPerSquareFootPerSecond:
        {
            value *= BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_BTUS_PER_SQUARE_FOOT_PER_SECOND;
            break;
        }
        case KilojoulesPerSquareMeterPerSecond:
        {
            value *= BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_KILOWATTS_PER_SQUARE_METER;
            break;
        }
        case KilojoulesPerSquareMeterPerMinute:
        {
            value *= BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_KILOJOULES_PER_SQUARE_METER_PER_MINUTE;
            break;
        }
        case KilowattsPerSquareMeter:
        {
            value *= BTUS_PER_SQUARE_FOOT_PER_MINUTE_TO_KILOWATTS_PER_SQUARE_METER;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }

    return value;
}

double HeatSinkUnits::toBaseUnits(double value, HeatSinkUnitsEnum units)
{
    const double KILOJOULES_PER_CUBIC_METER_TO_BTUS_PER_CUBIC_FOOT = 0.02681849745789;
    switch (units)
    {
        case BtusPerCubicFoot:
        {
            // Already in base, nothing to do
            break;
        }
        case KilojoulesPerCubicMeter:
        {
            value *= KILOJOULES_PER_CUBIC_METER_TO_BTUS_PER_CUBIC_FOOT;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}

double HeatSinkUnits::fromBaseUnits(double value, HeatSinkUnitsEnum units)
{
    const double BTUS_PER_CUBIC_FOOT_TO_KILOJOULES_PER_CUBIC_METER = 37.28769673134085;
    switch (units)
    {
        case BtusPerCubicFoot:
        {
            // Already in base, nothing to do
            break;
        }
        case KilojoulesPerCubicMeter:
        {
            value *= BTUS_PER_CUBIC_FOOT_TO_KILOJOULES_PER_CUBIC_METER;
            break;
        }
        default:
        {
            ; // TODO: Handle error
        }
    }
    return value;
}
