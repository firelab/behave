/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Various constant values used by the Surface class
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

#ifndef SURFACEENUMS_H
#define SURFACEENUMS_H

struct FuelConstants
{
    enum FuelConstantsEnum
    {
        DEAD = 0,                   // Index associated with dead fuels
        LIVE = 1,                   // Index associated with live fuels
        MAX_LIFE_STATES = 2,        // Number of life states, live and dead
        MAX_LIVE_SIZE_CLASSES = 3,  // Maximum number of live size classes
        MAX_DEAD_SIZE_CLASSES = 4,  // Maximum number of dead size classes
        MAX_PARTICLES = 4,          // Maximum number of size classes within a life state (dead/live)
        MAX_SAVR_SIZE_CLASSES = 5,  // Maximum number of SAVR size classes
        NUM_FUEL_MODELS = 267       // Maximum number of fuel models
    };
};

struct AspenFireSeverity
{
    enum AspenFireSeverityEnum
    {
        LOW = 0,
        MODERATE = 1
    };
};

struct TwoFuelModels
{
    enum TwoFuelModelsEnum
    {
        FIRST = 0,              // Index of the first fuel model
        NO_METHOD = 0,          // Don't use TwoFuel Models method
        ARITHMETIC = 1,         // Use arithmetic mean
        SECOND = 1,             // Index of the second fuel model
        NUMBER_OF_MODELS = 2,   // Numbe of fuel models used in TwoFuel Models method
        HARMONIC = 2,           // Use harmoic mean
        TWO_DIMENSIONAL = 3     // Use Finney's two dimensional method
    };
};

struct WindAdjustmentFactorShelterMethod
{
    enum WindAdjustmentFactorShelterMethodEnum
    {
        UNSHELTERED = 0,            // Wind adjustment factor was calculated using unsheltered method
        SHELTERED = 1,              // Wind adjustment factor was calculated using sheltered method
    };  
};

struct WindAdjustmentFactorCalculationMethod
{
    enum WindAdjustmentFactorCalculationMethodEnum
    {
        USER_INPUT = 0,             // User enters wind adjustment factor directly
        USE_CROWN_RATIO = 1,        // Use crown ratio when calculating wind adjustment factor
        DONT_USE_CROWN_RATIO = 2    // Don't use crown ratio when calculating wind adjustment factor
    };
};

struct WindAndSpreadOrientationMode
{
    enum WindAndSpreadOrientationModeEnum
    {
        RELATIVE_TO_UPSLOPE = 0,    // Wind and spread angles I/O are clockwise relative to upslope
        RELATIVE_TO_NORTH = 1       // Wind direction angles I/O are clockwise relative to compass north
    };
};

struct WindHeightInputMode
{
    enum WindHeightInputModeEnum
    {
        DIRECT_MIDFLAME = 0,    // User enters midflame wind speed directy
        TWENTY_FOOT = 1,        // User enters the 20 foot wind speed
        TEN_METER = 2           // User enters the 10 meter wind speed
    };
};

#endif // SURFACEENUMS_H
