#include <cmath>
#include <iomanip>
#include <iostream>

#include "behaveRun.h"
#include "fuelModelSet.h"

#include "crown.h"

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
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;
    double windDirection = 0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::DIRECT_MIDFLAME;
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
    behave.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);

    // Checking  wind and spread angle input mode
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = behave.getWindAndSpreadOrientationMode();
    std::cout << "Wind and spread direction are in degrees clockwise relative to ";
    if (windAndSpreadOrientationMode == WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE)
    {
        std::cout << "upslope" << std::endl << std::endl;
    }
    if (windAndSpreadOrientationMode == WindAndSpreadOrientationMode::RELATIVE_TO_NORTH)
    {
        std::cout << "compass north" << std::endl << std::endl;
    }

    //for (int i = 0; i <= 10; i++)
    //{
    //    fuelModelNumber = i;
    //    moistureOneHour = 6;
    //    moistureTenHour = 7;
    //    moistureHundredHour = 8;
    //    moistureLiveHerbaceous = 60;
    //    moistureLiveWoody = 90;
    //    windSpeed = 5;
    //    windDirection = 180;
    //    slope = 30;
    //    aspect = 250;
    //    directionOfInterest = 0;

    //    // std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;

    //    // Two Fuel Models test
    //    int firstFuelModelNumber = 0;
    //    int secondFuelModelNumber = 0;
    //    double coverage = 0.0;
    //    firstFuelModelNumber = 1;
    //    secondFuelModelNumber = 124;
    //    coverage = 0 + (.10 * i);
    //    behave.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, coverage, TwoFuelModels::TWO_DIMENSIONAL, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //    spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //    //spreadRate = floor(spreadRate * 10 + 0.5) / 10;
    //    std::cout << "Spread rate for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " with first fuel coverage " << coverage * 100 << "%" << std::endl;
    //    std::cout << "is " << spreadRate << " ch/hr" << std::endl;
    //    flameLength = behave.getFlameLength();
    //    //flameLength = floor(flameLength * 10 + 0.5) / 10;
    //    std::cout << "Flame length for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " is " << flameLength << " ft" << std::endl;

    //    // Direction of Max Spread test
    //    directionOfMaxSpread = behave.getDirectionOfMaxSpread();
    //    std::cout << "Direction of maximum spread is " << directionOfMaxSpread << " degrees" << std::endl << std::endl;
    //}

    fuelModelNumber = 1;
    moistureOneHour = 6;
    moistureTenHour = 7;
    moistureHundredHour = 8;
    moistureLiveHerbaceous = 60;
    moistureLiveWoody = 90;
    windSpeed = 5;
    windDirection = 42;
    windHeightInputMode = WindHeightInputMode::DIRECT_MIDFLAME;
    slope = 30;
    aspect = 291;
    directionOfInterest = 63;

    canopyCover = 0.50; // 50%
    canopyHeight = 6;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::FEET;
    crownRatio = 0.50;

    // Single fuel model test
    behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, 
        MoistureUnits::PERCENT, windSpeed, windSpeedUnits, windDirection, windHeightInputMode, slope, SlopeUnits::DEGREES, aspect, canopyCover, canopyHeight,
        canopyHeightUnits, crownRatio);
    behave.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    spreadRate = behave.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    flameLength = behave.getSurfaceFlameLength(LengthUnits::FEET);
    std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << spreadRate << " ch/hr" << std::endl;
    std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << flameLength << " ft" << std::endl << std::endl;
    // Direction of Max Spread test
    directionOfMaxSpread = behave.getDirectionOfMaxSpread();
    std::cout << "Direction of maximum spread is " << round(directionOfMaxSpread) << " degrees" << std::endl << std::endl;

#ifndef NDEBUG
    std::cout << "Press Enter to continue";
    std::cin.get();
#endif

    return 0;
}
