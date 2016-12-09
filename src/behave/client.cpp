#include <cmath>
#include <iomanip>
#include <iostream>
#include <time.h>

#include "behaveRun.h"
#include "fuelModelSet.h"

#include "crown.h"

int main()
{
    //clock_t tStart = clock();

    // Surface Fire Inputs;
    int fuelModelNumber = 0;
    double moistureOneHour = 0.0;
    double moistureTenHour = 0.0;
    double moistureHundredHour = 0.0;
    double moistureLiveHerbaceous = 0.0;
    double moistureLiveWoody = 0.0;
    double windSpeed = 0.0;
    double windDirection = 0;
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

    // Setting the slope input mode (default is percent)
    //behave.setSlopeInputToPercent();
    //behave.setSlopeInputToDegrees();

    // Checking the slope input mode
    SlopeUnits::SlopeUnitsEnum slopeUnits = behave.getSlopeUnits();
    std::wcout << "Slope input mode is ";
    if (slopeUnits == SlopeUnits::PERCENT)
    {
        std::cout << "percent" << std::endl << std::endl;
    }
    if (slopeUnits == SlopeUnits::DEGREES)
    {
        std::cout << "degrees" << std::endl << std::endl;
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
    slope = 30;
    aspect = 291;
    directionOfInterest = 63;

    canopyCover = 0.50; // 50%
    canopyHeight = 6;
    crownRatio = 0.50;

    // Single fuel model test
    behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, MoistureUnits::PERCENT, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    behave.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    spreadRate = behave.getSurfaceFireSpreadRate();
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    flameLength = behave.getSurfaceFlameLength();
    std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << spreadRate << " ch/hr" << std::endl;
    std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << flameLength << " ft" << std::endl << std::endl;
    // Direction of Max Spread test
    directionOfMaxSpread = behave.getDirectionOfMaxSpread();
    std::cout << "Direction of maximum spread is " << round(directionOfMaxSpread) << " degrees" << std::endl << std::endl;

    // Palmetto-Gallbury test
    //double	ageOfRough = 0.0;
    //double	heightOfUnderstory = 0.0;
    //double	palmettoCoverage = 0.0;
    //double	overstoryBasalArea = 0.0;
    //ageOfRough = 20;
    //heightOfUnderstory = 3;
    //palmettoCoverage = 25;
    //overstoryBasalArea = 100;
    //behave.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, ageOfRough, heightOfUnderstory, palmettoCoverage, overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);

    //std::cout << "Spread rate for Palmetto-Gallberry is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //flameLength = behave.getFlameLength();
    //flameLength = floor(flameLength * 10 + 0.5) / 10;
    //std::cout << "Flame length for Palmetto-Gallberry is " << flameLength << " ft" << std::endl << std::endl;

    // Western Aspen test
    //int aspenFuelModelNumber = 0;
    //double aspenCuringLevel = 0.0;
    //double DBH = 0.0;
    //aspenFuelModelNumber = 5;
    //aspenCuringLevel = 0.0;
    //DBH = 2;
    //behave.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, AspenFireSeverity::LOW, DBH, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //std::cout << "Spread rate for Western is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //flameLength = behave.getFlameLength();
    //flameLength = floor(flameLength * 10 + 0.5) / 10;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    // Used for testing performance
    //double executionTimeInSeconds = (double)((clock() - tStart) / CLOCKS_PER_SEC);
    //std::cout << "Total execution time for " << 1000000 << " fire spread calculations is " << executionTimeInSeconds << " seconds." << std::endl;

#ifndef NDEBUG
    std::cout << "Press Enter to continue";
    std::cin.get();
#endif

    return 0;
}
