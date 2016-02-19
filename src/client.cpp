#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

int main()
{	
	//clock_t tStart = clock();
	
	//for (int i = 0; i < 1000; i++)
	//{
	//	system("start C:\\Users\\wchatham\\Source\\Repos\\Behave\\build\\Debug\\behave-cli.exe --fuel-model-number 1 --1hr 1 --10hr 1 --100hr 1 --live-herb 1 --live-woody 1 --wind-speed 1 --wind-direction 1 --slope 1 --aspect 1 --output-to-file --append");
	//}

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
	double coverage = 0;

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
	behave.setSlopeInputToPercent();
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

	for (int i = 0; i < 1; i++)
	{
		fuelModelNumber = 1; 
		moistureOneHour = 0.10;
		moistureTenHour = 0.20;
		moistureHundredHour = 0.30;
		moistureLiveHerbaceous = 0.50;
		moistureLiveWoody = 1.20;
		windSpeed = 5;
		windDirection = 0;
		slope = 30;
		aspect = 0;

		directionOfInterest = -1;
		// std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;

		// Single fuel model test
		//behave.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windSpeed, windDirection, slope, aspect);
		//spreadRate = behave.calculateSurfaceFireForwardSpreadRate();
		//spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
		//std::cout << "Spread rate for fuel model " << i << " is " << std::setprecision(1) << std::fixed	<< spreadRate << " ch/hr" << std::endl;

		// Palmetto-Gallbury test
		double	ageOfRough = 6;
		double	heightOfUnderstory = 6;
		double	palmettoCoverage = 50;
		double	overstoryBasalArea = 50;
		behave.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windSpeed, windDirection, ageOfRough, heightOfUnderstory, palmettoCoverage, overstoryBasalArea, slope, aspect);
		spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
		std::cout << "Spread rate for Palmetto-Gallberry is " << std::setprecision(1) << std::fixed	<< spreadRate << " ch/hr" << std::endl;

		// Two Fuel Models test
		//firstFuelModelNumber = 3;
		//secondFuelModelNumber = 4;
		////coverage = 0 + (.10 * i);
		//coverage = .25;
		//behave.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windSpeed, windDirection, coverage, Behave::TWO_DIMENSIONAL, slope, aspect);
		//std::cout << "Spread rate for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " with first fuel coverage " << coverage << std::endl;
		//std::cout << "is " << std::setprecision(1) << std::fixed << spreadRate << " ch/hr" << std::endl;
		//spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);

		directionOfMaxSpread = behave.getDirectionOfMaxSpread();
		//std::cout << "Direction of maximum spread is for fuel model " << i << " is " << std::setprecision(0) << std::fixed << round(directionOfMaxSpread)
		//	<< " degrees" << std::endl;

		spreadRate = floor(spreadRate * 10 + 0.5) / 10;
		
		flameLength = behave.getFlameLength();
		flameLength = floor(flameLength * 10 + 0.5) / 10;
		std::cout << "Flame length for fuel model " << i << " is " << flameLength << " ft" << std::endl << std::endl;
	}

	// Used for debug
	//double executionTimeInSeconds = (double)((clock() - tStart) / CLOCKS_PER_SEC);
	//std::cout << "Total execution time for " << 1000 << " program run is " << executionTimeInSeconds << " seconds." << std::endl;
	std::cout << "Press Enter to continue";
	std::cin.get();

	return 0;
}
