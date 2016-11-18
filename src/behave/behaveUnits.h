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

struct LengthUnits
{
    enum LengthUnitsEnum
    {
        FEET, // base length unit
        METERS,				
        MILES,
        KILOMETERS
    };

    static double toBaseUnits(double value, LengthUnitsEnum units);
    static double fromBaseUnits(double value, LengthUnitsEnum units);
};

struct VelocityUnits 
{
    enum VelocityUnitsEnum
    {
        FEET_PER_MINUTE, //base velocity unit
        CHAINS_PER_HOUR,
        METERS_PER_SECOND,	
        MILES_PER_HOUR,
        KILOMETERS_PER_HOUR
    };

    static double toBaseUnits(double value, VelocityUnitsEnum units);
    static double fromBaseUnits(double value, VelocityUnitsEnum units);
};

struct CoverUnits
{
    enum CoverUnitsEnum
    {
        FRACTION, //base cover unit
        PERCENT
    };

    static double toBaseUnits(double value, CoverUnitsEnum units);
    static double fromBaseUnits(double value, CoverUnitsEnum units);
};

struct MoistureUnits
{
    enum MoistureUnitsEnum
    {
        FRACTION, //base cover unit
        PERCENT
    };

    static double toBaseUnits(double value, MoistureUnitsEnum units);
    static double fromBaseUnits(double value, MoistureUnitsEnum units);
};


struct SlopeUnits
{
    enum SlopeUnitsEnum
    {			   
        DEGREES, //base slope unit
        PERCENT
    };

    static double toBaseUnits(double value, SlopeUnitsEnum units);
    static double fromBaseUnits(double value, SlopeUnitsEnum units);
};


struct DensityUnits
{
    enum DensityUnitsEnum
    {
        POUNDS_PER_CUBIC_FOOT, //base density unit
        KILOGRAMS_PER_CUBIC_METER
    };

    static double toBaseUnits(double value, DensityUnitsEnum units);
    static double fromBaseUnits(double value, DensityUnitsEnum units);
};

#endif // BEHAVEUNITS_H
