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

#ifndef	BEHAVEUNITS_H
#define BEHAVEUNITS_H

struct AreaUnits
{
    enum AreaUnitsEnum
    {
        SquareFeet, // base area unit
        Acres,
        Hectares,
        SquareMeters,
        SquareMiles,
        SquareKilometers
    };

    static double toBaseUnits(double value, AreaUnitsEnum units);
    static double fromBaseUnits(double value, AreaUnitsEnum units);
};

struct BasalAreaUnits
{
    enum BasalAreaUnitsEnum
    {
        SquareFeetPerAcre, // base area unit
        SquareMetersPerHectare
    };

    static double toBaseUnits(double value, BasalAreaUnitsEnum units);
    static double fromBaseUnits(double value, BasalAreaUnitsEnum units);
};

struct LengthUnits
{
    enum LengthUnitsEnum
    {
        Feet, // base length unit
        Inches,
        Millimeters,
        Centimeters,
        Meters,
        Chains,
        Miles,
        Kilometers
    };

    static double toBaseUnits(double value, LengthUnitsEnum units);
    static double fromBaseUnits(double value, LengthUnitsEnum units);
};

struct LoadingUnits
{
    enum LoadingUnitsEnum
    {
        PoundsPerSquareFoot, // base loading unit
        TonsPerAcre,
        TonnesPerHectare,
        KilogramsPerSquareMeter
    };

    static double toBaseUnits(double value, LoadingUnitsEnum units);
    static double fromBaseUnits(double value, LoadingUnitsEnum units);
};

struct PressureUnits
{
    enum PressureUnitsEnum
    {
        Pascal,              // base loading unit
        KiloPascal,          // kPa
        MegaPascal,          // MPa
        GigaPascal,          // GPa
        Bar,                 // bar
        Atmosphere,          // atm
        TechnicalAtmosphere, // at
        PoundPerSquareInch   // psi
    };

    static double toBaseUnits(double value, PressureUnitsEnum units);
    static double fromBaseUnits(double value, PressureUnitsEnum units);
};

struct SurfaceAreaToVolumeUnits
{
    enum SurfaceAreaToVolumeUnitsEnum
    {
        SquareFeetOverCubicFeet, // base SAVR unit
        SquareMetersOverCubicMeters,
        SquareInchesOverCubicInches,
        SquareCentimetersOverCubicCentimeters
    };

    static double toBaseUnits(double value, SurfaceAreaToVolumeUnitsEnum units);
    static double fromBaseUnits(double value, SurfaceAreaToVolumeUnitsEnum units);
};

struct SpeedUnits
{
    enum SpeedUnitsEnum
    {
        FeetPerMinute, // base velocity unit
        ChainsPerHour,
        MetersPerSecond,
        MetersPerMinute,
        MetersPerHour,
        MilesPerHour,
        KilometersPerHour,
    };

    static double toBaseUnits(double value, SpeedUnitsEnum units);
    static double fromBaseUnits(double value, SpeedUnitsEnum units);
};

struct FractionUnits
{
    enum FractionUnitsEnum
    {
        Fraction, // base fraction unit
        Percent
    };

    static double toBaseUnits(double value, FractionUnitsEnum units);
    static double fromBaseUnits(double value, FractionUnitsEnum units);
};

struct SlopeUnits
{
    enum SlopeUnitsEnum
    {
        Degrees, // base slope unit
        Percent
    };

    static double toBaseUnits(double value, SlopeUnitsEnum units);
    static double fromBaseUnits(double value, SlopeUnitsEnum units);
};

struct DensityUnits
{
    enum DensityUnitsEnum
    {
        PoundsPerCubicFoot, // base density unit
        KilogramsPerCubicMeter
    };

    static double toBaseUnits(double value, DensityUnitsEnum units);
    static double fromBaseUnits(double value, DensityUnitsEnum units);
};

struct HeatOfCombustionUnits
{
    enum HeatOfCombustionUnitsEnum
    {
        BtusPerPound, // base heat of combustion unit
        KilojoulesPerKilogram
    };

    static double toBaseUnits(double value, HeatOfCombustionUnitsEnum units);
    static double fromBaseUnits(double value, HeatOfCombustionUnitsEnum units);
};

struct HeatSinkUnits
{
    enum HeatSinkUnitsEnum
    {
        BtusPerCubicFoot, // base heat sink unit
        KilojoulesPerCubicMeter
    };

    static double toBaseUnits(double value, HeatSinkUnitsEnum units);
    static double fromBaseUnits(double value, HeatSinkUnitsEnum units);
};

struct HeatPerUnitAreaUnits
{
    enum HeatPerUnitAreaUnitsEnum
    {
        BtusPerSquareFoot, // base HPUA unit
        KilojoulesPerSquareMeter,
        KilowattSecondsPerSquareMeter
    };

    static double toBaseUnits(double value, HeatPerUnitAreaUnitsEnum units);
    static double fromBaseUnits(double value, HeatPerUnitAreaUnitsEnum units);
};

struct HeatSourceAndReactionIntensityUnits
{
    enum HeatSourceAndReactionIntensityUnitsEnum
    {
        BtusPerSquareFootPerMinute, // base reaction intensity unit
        BtusPerSquareFootPerSecond,
        KilojoulesPerSquareMeterPerSecond,
        KilojoulesPerSquareMeterPerMinute,
        KilowattsPerSquareMeter
    };

    static double toBaseUnits(double value, HeatSourceAndReactionIntensityUnitsEnum units);
    static double fromBaseUnits(double value, HeatSourceAndReactionIntensityUnitsEnum units);
};

struct FirelineIntensityUnits
{
    enum FirelineIntensityUnitsEnum
    {
        BtusPerFootPerSecond,  // base fireline intensity unit
        BtusPerFootPerMinute,
        KilojoulesPerMeterPerSecond,
        KilojoulesPerMeterPerMinute,
        KilowattsPerMeter
    };

    static double toBaseUnits(double value, FirelineIntensityUnitsEnum units);
    static double fromBaseUnits(double value, FirelineIntensityUnitsEnum units);
};

struct TemperatureUnits
{
    enum TemperatureUnitsEnum
    {
        Fahrenheit, // base temperature unit
        Celsius,
        Kelvin
    };

    static double toBaseUnits(double value, TemperatureUnitsEnum units);
    static double fromBaseUnits(double value, TemperatureUnitsEnum units);
};

struct TimeUnits
{
    enum TimeUnitsEnum
    {
        Minutes, // base time unit
        Seconds,
        Hours,
        Days,
        Years
    };

    static double toBaseUnits(double value, TimeUnitsEnum units);
    static double fromBaseUnits(double value, TimeUnitsEnum units);
};

#endif // BEHAVEUNITS_H
