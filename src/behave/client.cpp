#include <iostream>
#include <iomanip>
#include "behaveRun.h"
#include <time.h>
#include <cmath>

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

    // Two Fuel Models
    int firstFuelModelNumber = 0;
    int secondFuelModelNumber = 0;
    double coverage = 0.0;

    // Palmetto-Gallberry
    double	ageOfRough = 0.0;
    double	heightOfUnderstory = 0.0;
    double	palmettoCoverage = 0.0;
    double	overstoryBasalArea = 0.0;

    // Western Aspen
    int aspenFuelModelNumber = 0;
    double aspenCuringLevel = 0.0;
    double DBH = 0.0;

    // Wind adjustment factor parameters
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;

    FuelModels fuelModels;
    BehaveRun behave(fuelModels);

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

    for (int i = 0; i <= 10; i++)
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
        //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
        //std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(1) << std::fixed	<< spreadRate << " ch/hr" << std::endl;
        //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

        // Palmetto-Gallbury test
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

        // Western Apsen test
        //aspenFuelModelNumber = 5;
        //aspenCuringLevel = 0.0;
        //DBH = 2;
        //behave.updateSurfaceInputsForWesternAspen(aspenFuelModelNumber, aspenCuringLevel, AspenFireSeverity::LOW, DBH, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
        //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
        //std::cout << "Spread rate for Western is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
        //flameLength = behave.getFlameLength();
        //flameLength = floor(flameLength * 10 + 0.5) / 10;
        //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

        //// Two Fuel Models test
        firstFuelModelNumber = 1;
        secondFuelModelNumber = 124;
        coverage = 0 + (.10 * i);
        behave.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, coverage, TwoFuelModels::TWO_DIMENSIONAL, slope, aspect, canopyCover, canopyHeight, crownRatio);
        spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
        spreadRate = floor(spreadRate * 10 + 0.5) / 10;
        std::cout << "Spread rate for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " with first fuel coverage " << std::setprecision(0) << coverage * 100 << "%" << std::endl;
        std::cout << "is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
        flameLength = behave.getFlameLength();
        flameLength = floor(flameLength * 10 + 0.5) / 10;
        std::cout << "Flame length for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

        // Direction of Max Spread test
        //directionOfMaxSpread = behave.getDirectionOfMaxSpread();
        //std::cout << "Direction of maximum spread is for fuel model " << i << " is " << std::setprecision(0) << std::fixed << round(directionOfMaxSpread) << " degrees" << std::endl;
    }

    //fuelModelNumber = 1;
    //moistureOneHour = 0.06;
    //moistureTenHour = 0.07;
    //moistureHundredHour = 0.08;
    //moistureLiveHerbaceous = 0.60;
    //moistureLiveWoody = 0.90;
    //windSpeed = 5;
    //windDirection = 0;
    //slope = 30;
    //aspect = 0;
    //directionOfInterest = -1;

    //// Copy Constructor Test
    //behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //Behave behaveCopy = behave;

    //spreadRate = behaveCopy.getSpreadRate();
    //flameLength = floor((behaveCopy.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "bevaheCopy's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //fuelModelNumber = 2;
    //
    //std::cout << "Calculating behave's spead rate for fuel model " << fuelModelNumber << std::endl;
    //behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behave's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //spreadRate = behaveCopy.getSpreadRate();
    //flameLength = floor((behaveCopy.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behaveCopy's spread rate for fuel model 1 is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model 1 is " << flameLength << " ft" << std::endl << std::endl;

    //fuelModelNumber = 3;
    //std::cout << "Calculating behave's spead rate for fuel model " << fuelModelNumber << std::endl;
    //behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behave's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //spreadRate = behaveCopy.getSpreadRate();
    //flameLength = floor((behaveCopy.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behaveCopy's spread rate for fuel model 1 is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model 1 is " << flameLength << " ft" << std::endl << std::endl;

    //fuelModelNumber = 5;
 
    //std::cout << "Calculating behaveCopy's spead rate for fuel model " << fuelModelNumber << std::endl;
    //behaveCopy.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behaveCopy.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behaveCopy.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behaveCopy's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //fuelModelNumber = 7;
    //behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    //fuelModelNumber = 6;
    //std::cout << "Calculating behaveCopy's spead rate for fuel model " << fuelModelNumber << std::endl;
    //behaveCopy.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);
    //spreadRate = behaveCopy.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behaveCopy.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behaveCopy's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //fuelModelNumber = 2;
    //std::cout << "Calculating behaveCopy's spead rate for fuel model " << fuelModelNumber << std::endl;
    //behaveCopy.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    //fuelModelNumber = 7;
    //std::cout << "Changing behave's fuel model to " << fuelModelNumber << std::endl;
    //behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

    //spreadRate = behaveCopy.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behaveCopy.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "behaveCopy's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    //spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    //std::cout << "bevahe's spread rate for fuel model " << fuelModelNumber << " is now " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
    //std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << flameLength << " ft" << std::endl << std::endl;

    // Used for debug
    //double executionTimeInSeconds = (double)((clock() - tStart) / CLOCKS_PER_SEC);
    //std::cout << "Total execution time for " << 1000000 << " fire spread calculations is " << executionTimeInSeconds << " seconds." << std::endl;
    std::cout << "Press Enter to continue";
    std::cin.get();

    return 0;
}
