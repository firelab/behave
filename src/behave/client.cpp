#include <iomanip>
#include <iostream>
#include <math.h>

#include "behaveRun.h"
#include "fuelModelSet.h"

int main()
{
    // Surface Fire Inputs;
    int fuelModelNumber = 0;
    double moistureOneHour = 0.0;
    double moistureTenHour = 0.0;
    double moistureHundredHour = 0.0;
    double moistureLiveHerbaceous = 0.0;
    double moistureLiveWoody = 0.0;
    double windSpeed = 0.0;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::DirectMidflame;
    double slope = 0.0;
    double aspect = 0.0;
    double directionOfMaxSpread = 0;
    double flameLength = 0;
    double directionOfInterest = 0;
    double spreadRate = 0;

    // Wind adjustment factor parameters
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;

    FuelModelSet fuelModelSet;
    BehaveRun behave(fuelModelSet);

    // Setting the wind and spread angle input mode (default is upslope)
    behave.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);

    // Checking  wind and spread angle input mode
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = behave.surface.getWindAndSpreadOrientationMode();
    std::cout << "Wind and spread direction are in degrees clockwise relative to ";
    if (windAndSpreadOrientationMode == WindAndSpreadOrientationMode::RelativeToUpslope)
    {
        std::cout << "upslope" << std::endl << std::endl;
    }
    if (windAndSpreadOrientationMode == WindAndSpreadOrientationMode::RelativeToNorth)
    {
        std::cout << "compass north" << std::endl << std::endl;
    }

   

    for (int i = 0; i <= 10; i++)
    {
        fuelModelNumber = i;
        moistureOneHour = 6;
        moistureTenHour = 7;
        moistureHundredHour = 8;
        moistureLiveHerbaceous = 60;
        moistureLiveWoody = 90;
        TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod = TwoFuelModelsMethod::TwoFimensional;
        windSpeed = 5;
        windHeightInputMode = WindHeightInputMode::DirectMidflame;
        windDirection = 180;
        slope = 30;
        SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
        aspect = 250;
        directionOfInterest = 0;

        // std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;

        // Two Fuel Models test
        int firstFuelModelNumber = 0;
        int secondFuelModelNumber = 0;
        double firstFuelModelCoverage = 0.0;
        firstFuelModelNumber = 1;
        secondFuelModelNumber = 124;
        firstFuelModelCoverage = 0 + (.10 * i);
        CoverUnits::CoverUnitsEnum firstFuelModelCoverageUnits = CoverUnits::Percent;
        CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
        LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;

        behave.surface.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
            moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, MoistureUnits::Percent, windSpeed, windSpeedUnits, windHeightInputMode,
            windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope,
            slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
        behave.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
        spreadRate = behave.surface.getSpreadRate(SpeedUnits::ChainsPerHour);
        //spreadRate = floor(spreadRate * 10 + 0.5) / 10;
        std::cout << "Spread rate for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " with first fuel coverage " << firstFuelModelCoverage * 100 << "%" << std::endl;
        std::cout << "is " << spreadRate << " ch/hr" << std::endl;
        flameLength = behave.surface.getFlameLength(LengthUnits::Feet);
        //flameLength = floor(flameLength * 10 + 0.5) / 10;
        std::cout << "Flame length for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " is " << flameLength << " ft" << std::endl;

        // Direction of Max Spread test
        directionOfMaxSpread = behave.surface.getDirectionOfMaxSpread();
        std::cout << "Direction of maximum spread is " << directionOfMaxSpread << " degrees" << std::endl << std::endl;
    }

    fuelModelNumber = 1;
    moistureOneHour = 6;
    moistureTenHour = 7;
    moistureHundredHour = 8;
    moistureLiveHerbaceous = 60;
    moistureLiveWoody = 90;
    windSpeed = 5;
    windDirection = 42;
  
    slope = 30;
    aspect = 291;
    directionOfInterest = 63;
    canopyCover = 0.50; // 50%
    CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
    canopyHeight = 6;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    crownRatio = 0.50;

    // Single fuel model test
    behave.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, 
        MoistureUnits::Percent, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode, slope, SlopeUnits::Degrees,
        aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
    behave.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    spreadRate = behave.surface.getSpreadRate(SpeedUnits::ChainsPerHour);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    flameLength = behave.surface.getFlameLength(LengthUnits::Feet);
    std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << spreadRate << " ch/hr" << std::endl;
    std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << flameLength << " ft" << std::endl << std::endl;
    // Direction of Max Spread test
    directionOfMaxSpread = behave.surface.getDirectionOfMaxSpread();
    std::cout << "Direction of maximum spread is " << round(directionOfMaxSpread) << " degrees" << std::endl << std::endl;

#ifndef NDEBUG
    std::cout << "Press Enter to continue";
    std::cin.get();
#endif

    return 0;
}
