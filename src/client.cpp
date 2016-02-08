#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

int main()
{	
    // Surface Fire Inputs;
    int fuelModelNumber = 0;
    double moisture1h = 0.0;
    double moisture10h = 0.0;
    double moisture100h = 0.0;
    double moistureLiveHerb = 0.0;
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
	int firstFuelModelNumber = 2;
	int secondFuelModelNumber = 4;
	double coverage = 33;

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

	for (int i = 1; i <= 1; i++)
	{
		fuelModelNumber = i; 
		moisture1h = 0.06;
		moisture10h = 0.07;
		moisture100h = 0.08;
		moistureLiveHerb = 0.60;
		moistureLiveWoody = 0.90;
		windSpeed = 3;
		windDirection = 0;
		slope = 20;
		aspect = 50;

		//behave.updateSurfaceInputs(fuelModelNumber, moisture1h, moisture10h, moisture100h, moistureLiveHerb, moistureLiveWoody, windSpeed, windDirection, slope, aspect);
		behave.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moisture1h, moisture10h, moisture100h, moistureLiveHerb, moistureLiveWoody, windSpeed, windDirection, slope, coverage, Behave::ARITHMETIC, aspect);

		spreadRate = behave.calculateSurfaceFireForwardSpreadRate();

		//directionOfInterest = 0;
		//spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
	

		directionOfMaxSpread = behave.getDirectionOfMaxSpread();
		std::cout << "Direction of maximum spread is for fuel model " << i << " is " << std::setprecision(0) << std::fixed << round(directionOfMaxSpread)
			<< " degrees" << std::endl;
		spreadRate = floor(spreadRate * 10 + 0.5) / 10;
		// std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;
		std::cout << "Spread rate in direction of interest " << directionOfInterest << " degrees for fuel model " << i << " is " << std::setprecision(1) << std::fixed
			<< spreadRate << " ch/hr" << std::endl;
		flameLength = behave.getFlameLength();
		flameLength = floor(flameLength * 10 + 0.5) / 10;
		std::cout << "Flame length for fuel model " << i << " is "<< flameLength << " ft" << std::endl << std::endl;
	}
	// Used for debug
	std::cout << "Press Enter to continue";
	std::cin.get();

	return 0;
}
