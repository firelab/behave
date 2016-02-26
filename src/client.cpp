#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

int main()
{
    //clock_t tStart = clock();
    //double executionTimeInSeconds = (double)((clock() - tStart) / CLOCKS_PER_SEC);
    //std::cout << "Total execution time for " << 1000 << " program run is " << executionTimeInSeconds << " seconds." << std::endl;

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

    // Two Fuel Models
    int firstFuelModelNumber = 0;
    int secondFuelModelNumber = 0;
    double coverage = 0.0;

    // Wind adjustment factor parameters
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;

    Behave behave;

    // Setting the wind and spread angle input mode (default is upslope)
    //behavePlus.setWindAndSpreadAnglesRelativeToUpslope();
    //behave.setWindAndSpreadAnglesRelativeToNorth();

    // Checking  wind and spread angle input mode
    std::cout << "Wind and spread direction are in degrees clockwise relative to ";
    if (behave.isWindAndSpreadAngleRelativeToUpslope())
    {
        std::cout << "upslope" << std::endl << std::endl;
    }
    if (behave.isWindAndSpreadAngleRelativeToNorth())
    {
        std::cout << "compass north" << std::endl << std::endl;
    }

    // Setting the slope input mode (default is percent)
    //behave.setSlopeInputToPercent();
    //behave.setSlopeInputToDegrees();

    // Checking the slope input mode
    std::wcout << "Slope input mode is ";
    if (behave.isSlopeInPercent())
    {
        std::cout << "percent" << std::endl << std::endl;
    }
    if (behave.isSlopeInDegrees())
    {
        std::cout << "degrees" << std::endl << std::endl;
    }

    for (int i = 0; i < 11; i++)
    {
        fuelModelNumber = i;
        moistureOneHour = 0.06;
        moistureTenHour = 0.07;
        moistureHundredHour = 0.08;
        moistureLiveHerbaceous = 0.60;
        moistureLiveWoody = 0.90;
        windSpeed = 5;
        windDirection = 0;
        slope = 30;
        aspect = 0;
        directionOfInterest = -1;

        canopyCover = .25;
        canopyHeight = 6.0;
        crownRatio = 0.5;

        // std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;

        // Single fuel model test
        //behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
        //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
        //std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(1) << std::fixed	<< spreadRate << " ch/hr" << std::endl;
        //std::cout << "Flame length for fuel model " << fuelModelNumber << " ft" << std::endl << std::endl;

        // Palmetto-Gallbury test
        //double	ageOfRough = 20;
        //double	heightOfUnderstory = 3;
        //double	palmettoCoverage = 25;
        //double	overstoryBasalArea = 100;
        //behave.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, ageOfRough, heightOfUnderstory, palmettoCoverage, overstoryBasalArea, slope, aspect, canopyCover, canopyHeight, crownRatio);
        //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
        //std::cout << "Spread rate for Palmetto-Gallberry is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
        //flameLength = behave.getFlameLength();
        //flameLength = floor(flameLength * 10 + 0.5) / 10;
        //std::cout << "Flame length for Palmetto-Gallberry is " << flameLength << " ft" << std::endl << std::endl;

        // Two Fuel Models test
        firstFuelModelNumber = 1;
        secondFuelModelNumber = 124;
        coverage = 0 + (.10 * i);
        behave.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, coverage, TwoFuelModelsMethod::TWO_DIMENSIONAL, slope, aspect, canopyCover, canopyHeight, crownRatio);
        spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
        spreadRate = floor(spreadRate * 10 + 0.5) / 10;
        std::cout << "Spread rate for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " with first fuel coverage " << std::setprecision(0) << coverage * 100 << "%" << std::endl;
        std::cout << "is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
        flameLength = behave.getFlameLength();
        flameLength = floor(flameLength * 10 + 0.5) / 10;
        std::cout << "Flame length for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

        // Direction of Max Spread test
        //directionOfMaxSpread = behave.getDirectionOfMaxSpread();
        //std::cout << "Direction of maximum spread is for fuel model " << i << " is " << std::setprecision(0) << std::fixed << round(directionOfMaxSpread)
        //	<< " degrees" << std::endl;
    }

    // Used for debug
    //double executionTimeInSeconds = (double)((clock() - tStart) / CLOCKS_PER_SEC);
    //std::cout << "Total execution time for " << 1000 << " program run is " << executionTimeInSeconds << " seconds." << std::endl;
    std::cout << "Press Enter to continue";
    std::cin.get();

    return 0;
}
