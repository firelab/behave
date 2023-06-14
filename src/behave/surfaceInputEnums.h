#ifndef SURFACEINPUTENUMS_H
#define SURFACEINPUTENUMS_H

struct MoistureClassInput
{
    enum MoistureClassInputEnum
    {
        OneHour = 0,            // Associated with dead fuel 1 hour moisture
        TenHour = 1,            // Associated with dead fuel 10 hour moisture
        HundredHour = 2,        // Associated with dead fuel 100 hour moisture
        LiveHerbaceous = 3,     // Associated with live fuel herbaceous moisture
        LiveWoody = 4,          // Associated with live fuel woody moisture
        DeadAggregate = 5,      // Associated with aggregate dead fuel moisture
        LiveAggregate = 6       // Associated with aggregate live fuel moisture
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
        Unsheltered = 0,        // Wind adjustment factor was calculated using unsheltered method
        Sheltered = 1,          // Wind adjustment factor was calculated using sheltered method
    };
};

struct WindAdjustmentFactorCalculationMethod
{
    enum WindAdjustmentFactorCalculationMethodEnum
    {
        UserInput = 0,          // User enters wind adjustment factor directly
        UseCrownRatio = 1,      // Use crown ratio when calculating wind adjustment factor
        DontUseCrownRatio = 2   // Don't use crown ratio when calculating wind adjustment factor
    };
};

struct WindHeightInputMode
{
    enum WindHeightInputModeEnum
    {
        DirectMidflame = 0,     // User enters midflame wind speed directy
        TwentyFoot = 1,         // User enters the 20 foot wind speed
        TenMeter = 2            // User enters the 10 meter wind speed
    };
};

struct WindAndSpreadOrientationMode
{
    enum WindAndSpreadOrientationModeEnum
    {
        RelativeToUpslope = 0,  // Wind and spread angles I/O are clockwise relative to upslope
        RelativeToNorth = 1     // Wind direction angles I/O are clockwise relative to compass north
    };
};

struct FuelLifeState
{
    enum FuelLifeStateEnum
    {
        Dead = 0,               // Index associated with dead fuels
        Live = 1,               // Index associated with live fuels
    };
};

struct FuelConstants
{
    enum FuelConstantsEnum
    {
        MaxLifeStates = 2,      // Number of life states, live and dead
        MaxLiveSizeClasses = 5, // Maximum number of live size classes
        MaxDeadSizeClasses = 4, // Maximum number of dead size classes
        MaxParticles = 5,       // Maximum number of size classes within a life state (dead/live)
        MaxSavrSizeClasses = 5, // Maximum number of SAVR size classes
        MaxFuelModels = 256     // Maximum number of fuel models
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
        Arithmetic = 1,        // Use arithmetic mean
        Harmonic = 2,          // Use harmoic mean
        TwoDimensional = 3     // Use Finney's two dimensional method
    };
};

struct TwoFuelModelsContants
{
    enum TwoFuelModelsContantsEnum
    {
        First = 0,            // Index of the first fuel model
        Second = 1,           // Index of the second fuel model
        NumberOfModels = 2,   // Numbe of fuel models used in TwoFuel Models method
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

struct ChaparralFuelType
{
    enum ChaparralFuelTypeEnum
    {
        NotSet = 0,
        Chamise = 1,
        MixedBrush = 2
    };
};

struct ChaparralContants
{
    enum ChaparralContantsEnum
    {
        NumFuelClasses = 5
    };
};

struct ChaparralFuelLoadInputMode
{
    enum ChaparralFuelInputLoadModeEnum
    {
        DirectFuelLoad = 1,
        FuelLoadFromDepthAndChaparralType = 2
    };
};

#endif // SURFACEINPUTENUMS_H
