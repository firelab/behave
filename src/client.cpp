#include <iostream>
#include <iomanip>
#include "behavePlus.h"
#include <time.h>
#include <cmath>

int main()
{	
	BehavePlus behavePlus;

	// Surface Fire Inputs;
	int fuelModelNumber = 0;
	double moisture1h = 0.0;
	double moisture10h = 0.0;
	double moisture100h = 0.0;
	double moistureLiveHerb = 0.0;
	double moistureLiveWood = 0.0;
	double windspeed = 0;
	double windDirection = 0;
	double slope = 0;
	double slopeAspect = 0;
	double directionOfMaxSpread = 0;
	double flameLength = 0; 
	double directionOfInterest = 0;
	double spreadRate = 0;

	// Setting the wind and spread angle input mode (default is upslope)
	//behavePlus.setWindAndSpreadAnglesRelativeToUpslope();
	behavePlus.setWindAndSpreadAnglesRelativeToNorth();

	// Checking  wind and spread angle input mode
	// The method of determining angle or slope input mode is admittedly bit clunky
	std::cout << "Wind and spread direction are in degrees clockwise relative to ";
	if (behavePlus.isWindAndSpreadAngleRelativeToUpslope())
	{
		std::cout << "upslope" << std::endl << std::endl;
	}
	if (behavePlus.isWindAndSpreadAngleRelativeToNorth())
	{
		std::cout << "compass north" << std::endl << std::endl;
	}

	// Setting the slope input mode (default is percent)
	//behavePlus.setSlopeInputToPercent();
	behavePlus.setSlopeInputToDegrees();

	// Checking the slope input mode
	// The method of determining slope input mode is admittedly bit clunky
	std::wcout << "Slope input mode is ";
	if (behavePlus.isSlopeInPercent())
	{
		std::cout << "percent" << std::endl << std::endl;
	}
	if (behavePlus.isSlopeInDegrees())
	{
		std::cout << "degrees" << std::endl << std::endl;
	}

	for (int i = 101; i <= 109; i++)
	{
		fuelModelNumber = i; 
		moisture1h = 0.06;
		moisture10h = 0.07;
		moisture100h = 0.08;
		moistureLiveHerb = 0.60;
		moistureLiveWood = 0.90;
		windspeed = 3;
		windDirection = 90;
		slope = 50;
		slopeAspect = 50;
		behavePlus.updateSurfaceInputs(fuelModelNumber, moisture1h, moisture10h, moisture100h, moistureLiveHerb, moistureLiveWood, windspeed, windDirection, slope, slopeAspect);
		directionOfInterest = 0;

		spreadRate = behavePlus.calculateSurfaceFireForwardSpreadRate();
		//spreadRate = behavePlus.calculateSurfaceFireForwardSpreadRate(directionOfInterest);

		directionOfMaxSpread = behavePlus.getDirectionOfMaxSpread();
        std::cout << "Direction of maximum spread is for fuel model " << i << " is " << std::setprecision(0) << std::fixed << round(directionOfMaxSpread)
			<< " degrees" << std::endl;
		spreadRate = floor(spreadRate * 10 + 0.5) / 10;
		// std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;
		std::cout << "Spread rate in direction of interest " << directionOfInterest << " degrees for fuel model " << i << " is " << std::setprecision(1) << std::fixed
			<< spreadRate << " ch/hr" << std::endl;
		flameLength = behavePlus.getFlameLength();
		flameLength = floor(flameLength * 10 + 0.5) / 10;
		std::cout << "Flame length for fuel model " << i << " is "<< flameLength << " ft" << std::endl << std::endl;
	}

	std::cout << "Press Enter to continue";
	std::cin.get();

	return 0;
}
