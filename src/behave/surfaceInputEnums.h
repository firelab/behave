#ifndef SURFACEINPUTENUMS_H
#define SURFACEINPUTENUMS_H

struct MoistureClassInput
{
    enum MoistureClassInputEnum
    {
        OneHour = 0,               // Associated with dead fuel 1 hour moisture
        TenHour = 1,               // Associated with dead fuel 10 hour moisture
        HundredHour = 2,           // Associated with dead fuel 100 hour moisture
        LiveHerbaceous = 3,        // Associated with live fuel herbaceous moisture
        LiveWoody = 4,             // Associated with live  fuel woody moisture
        DeadAggregate = 5,         // Associated with aggregate dead fuel moisture
        LiveAggregate = 6          // Associated with aggregate live fuel moisture
    };
};

struct LiveFuelMoistureInput
{
    enum LiveFuelMoistureInputEnum
    {
       
        Aggregate = 2               // Index associated with aggregate live fuel moisture
    };
};

struct AspenFireSeverity
{
    enum AspenFireSeverityEnum
    {
       Low = 0,
       Moderate = 1
    };
};

struct WindAdjustmentFactorShelterMethod
{
    enum WindAdjustmentFactorShelterMethodEnum
    {
        Unsheltered = 0,            // Wind adjustment factor was calculated using unsheltered method
        Sheltered = 1,              // Wind adjustment factor was calculated using sheltered method
    };
};

struct WindAdjustmentFactorCalculationMethod
{
    enum WindAdjustmentFactorCalculationMethodEnum
    {
        UserInput = 0,             // User enters wind adjustment factor directly
        UseCrownRatio = 1,        // Use crown ratio when calculating wind adjustment factor
        DontUseCrownRatio = 2    // Don't use crown ratio when calculating wind adjustment factor
    };
};

struct WindHeightInputMode
{
    enum WindHeightInputModeEnum
    {
        DirectMidflame = 0,    // User enters midflame wind speed directy
        TwentyFoot = 1,        // User enters the 20 foot wind speed
        TenMeter = 2           // User enters the 10 meter wind speed
    };
};

struct WindAndSpreadOrientationMode
{
    enum WindAndSpreadOrientationModeEnum
    {
        RelativeToUpslope = 0,    // Wind and spread angles I/O are clockwise relative to upslope
        RelativeToNorth = 1       // Wind direction angles I/O are clockwise relative to compass north
    };
};

struct SurfaceFireSpreadDirectionMode
{
    enum SurfaceFireSpreadDirectionModeEnum
    {
        FromIgnitionPoint = 0,
        FromPerimeter = 1
    };
};

struct TwoFuelModelsMethod
{
    enum TwoFuelModelsMethodEnum
    {
        NoMethod = 0,          // Don't use TwoFuel Models method
        Arithmetic = 1,         // Use arithmetic mean
        Harmonic = 2,           // Use harmoic mean
        TwoDimensional = 3     // Use Finney's two dimensional method
    };
};

struct TwoFuelModelsContants
{
    enum TwoFuelModelsContantsEnum
    {
        FIRST = 0,              // Index of the first fuel model
        SECOND = 1,             // Index of the second fuel model
        NUMBER_OF_MODELS = 2,   // Numbe of fuel models used in TwoFuel Models method
    };
};

struct MoistureInputMode
{
    enum MoistureInputModeEnum
    {
        BySizeClass = 0,                    // User enters moisture for each size class
        AllAggregate = 1,                   // User enters aggregate moisture for dead and live fuels
        DeadAggregateAndLiveSizeClass = 2,  // User enters aggregate moisture for dead fuels and for each live size class
        LiveAggregateAndDeadSizeClass = 3,  // User enters aggregate moisture for live fuels and for each dead size class
        MoistureScenario = 4                // User enters a moisture scenario           
    };
};

#endif // SURFACEINPUTENUMS_H
