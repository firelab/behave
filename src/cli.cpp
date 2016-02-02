#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "behave.h"

#define EQUAL(a,b) (strcmp(a,b)==0)

const int REQUIRED_ARG_COUNT = 10;

enum
{
	FUEL_MODEL_NUMBER,
	ONE_HOUR,
	TEN_HOUR,
	HUNDRED_HOUR,
	LIVE_HERB,
	LIVE_WOODY,
	WIND_SPEED,
	WIND_DIRECTION,
	SLOPE,
	ASPECT
};

void Usage()
{
	printf("\nUsage:\n");
	printf("behave_cli [--fuel_model_number number]      Required\n");
	printf("           [--1hr percent]                   Required\n");
	printf("           [--10hr percent]                  Required\n");
	printf("           [--100hr percent]                 Required\n");
	printf("           [--live_herb percent]             Required\n");
	printf("           [--live_woody percent]            Required\n");
	printf("           [--wind_speed speed]              Required\n");
	printf("           [--wind_direction degrees]        Required\n");
	printf("           [--slope degrees]                 Required\n");
	printf("           [--aspect degrees]                Required\n");
	printf("           [--direction-of-interest degrees] Optional\n");
	printf("           [--output-to-file]                Optional\n");
	printf("           [--file-name name]                Optional\n");
	printf("           [--append]                        Optional\n");
	printf("           [--run-identifier name]           Optional\n");
	//printf("           [--verbose]\n");
	printf("\n");
	printf("All parameter values must be non-negative and angles must be between\n");
	printf("0-360 degrees, and all angles are relative to compass north\n\n");
	printf("Arguments:\n");
	printf("\n");
	printf("--fuel-model-number <number>        Required: The fuel model number \n");
	printf("--1hr <percent>                     Required: 1-hr fuel moisture\n");
	printf("--10hr <percent>                    Required: 10-hr fuel moisture\n");
	printf("--100hr <percent>                   Required: 100-hr fuel moisture\n");
	printf("--live-herb <percent>               Required: Live herbacious fuel moisture\n");
	printf("--live-woody <percent>              Required: Live woody fuel moisture\n");
	printf("--wind-speed <speed>                Required: Mid-flame wind speed(mi/hr)\n");
	printf("--wind-direction <degrees>          Required: Wind direction\n");
	printf("--slope <degrees>                   Required: Slope steepness\n");
	printf("--aspect <degrees>                  Required: Aspect of the slope\n");
	printf("--direction-of-interest <degrees>   Optional: Calculate spread rate in a\n");
	printf("                                       direction other than maximum\n");
	printf("--output-to-file                    Optional: Output to a txt file\n");
	printf("                                       default file name: output.txt\n");
	printf("--file-name <name>                  Optional: Specify output file name\n");
	printf("--append                            Optional: Append run to output file\n");
	printf("--run-identifier <name>             Optional: Specify identifier for run\n");
	exit(1);
}

int parseFuelModelNumber(int i, const int MAX_ARG_INDEX, std::string argString)
{
	int retVal = -1;
	if (argString.c_str() != NULL)
	{
		char* end;
		long val = strtol(argString.c_str(), &end, 10);
		if (!end[0] && val >= 0)
		{
			retVal = val;
		}
	}
	if (retVal <= 0)
	{
		printf("ERROR: No valid value entered for fuel model number");
		Usage();
	}
	return retVal;
}

void checkAngleBound(double angle)
{
	if ((angle < 0.0) || (angle > 360.00))
	{
		//throw std::runtime_error("ERROR: Angles cannot negative or greater than 360.\n");
		printf("ERROR: Angles cannot negative or greater than 360.\n");
		exit(1);
	}
}

void checkSlopeBound(double angle)
{
	checkAngleBound(angle);
	if (angle > 80.0)
	{
		printf("ERROR: Slope cannot exceed 80 degrees.\n");
		exit(1);
	}
}

void checkMoistureBound(double moisture)
{
	if ((moisture < 0.0) || (moisture > 300.00))
	{
		//throw std::runtime_error("ERROR: Moisture cannot negative or greater than 300.\n");
		printf("ERROR: Moisture cannot negative or greater than 300.\n");
		exit(1);
	}
}

double parseMoisture(int i, const int MAX_ARG_INDEX, std::string argString, std::string argName)
{
	double retVal = -1;
	if (argString.c_str() != NULL)
	{
		char* end;
		double val = strtod(argString.c_str(), &end);
		if (!end[0] && val >= 0)
		{
			retVal = val;
		}
	}
	if (retVal < 0)
	{
		std::string errorString = "ERROR: No valid value entered for ";
		errorString += argName;
		printf(errorString.c_str());
		Usage();
	}
	checkMoistureBound(retVal);
	retVal /= 100;

	return retVal;
}

double parseDouble(int i, const int MAX_ARG_INDEX, std::string argString, std::string argName)
{
	double retVal = -1;
	if (argString.c_str() != NULL)
	{
		char* end;
		double val = strtod(argString.c_str(), &end);
		if (!end[0] && val >= 0)
		{
			retVal = val;
		}
	}
	if (retVal < 0)
	{
		std::string errorString = "ERROR: No valid value entered for ";
		errorString += argName;
		printf(errorString.c_str());
		Usage();
	}

	return retVal;
}

std::string getArgumentName(int argIndex)
{
	std::string retString = "";
	if (argIndex == ASPECT)
	{
		retString = "aspect";
	}
	if (argIndex == FUEL_MODEL_NUMBER)
	{
		retString = "fuel model number";
	}
	if (argIndex == HUNDRED_HOUR)
	{
		retString = "100-hr moisture";
	}
	if (argIndex == LIVE_HERB)
	{
		retString = "live herbaceous moisture";
	}
	if (argIndex == LIVE_WOODY)
	{
		retString = "live woody moisture";
	}
	if (argIndex == ONE_HOUR)
	{
		retString = "1-hr moisture";
	}
	if (argIndex == SLOPE)
	{
		retString = "slope";
	}
	if (argIndex == TEN_HOUR)
	{
		retString = "10-hr moisture";
	}
	if (argIndex == WIND_DIRECTION)
	{
		retString = "wind direction";
	}
	if (argIndex == WIND_SPEED)
	{
		retString = "wind speed";
	}
	return retString;
}

int main(int argc, char *argv[])
{
	const int MAX_ARG_INDEX = argc - 1;

	bool requiredArgArray[REQUIRED_ARG_COUNT];
	bool isOutputtingToFile = false;
	bool isMissingReq = false;
	bool hasSpecifiedFileName = false;
	bool isAppending = false;
	bool hasDirectionOfInterest = false;
	bool hasRunIdentifier = false;
	int i;
	std::string argString = "";
	std::string fileName = "output.txt"; // default output file name
	std::string runIdentifier = "";
	std::string argName = "";

	// Surface Fire Inputs;
	int fuelModelNumber = 0;
	double moistureOneHr = 0.0;
	double moistureTenHr = 0.0;
	double moistureHundredHr = 0.0;
	double moistureLiveHerb = 0.0;
	double moistureLiveWoody = 0.0;
	double windSpeed = 0.0;
	double windDirection = 0.0;
	double slope = 0.0;
	double aspect = 0.0;
	double directionOfMaxSpread = 0.0;
	double flameLength = 0.0;
	double directionOfInterest = 0.0;
	double spreadRate = 0;

	for (int index = 0; index < REQUIRED_ARG_COUNT; index++)
	{
		requiredArgArray[index] = false;
	}

	Behave behave;
	behave.setSlopeInputToDegrees();
	behave.setWindAndSpreadAnglesRelativeToNorth();

	// Parse command line arguments
	if (argc == 1)
	{
		Usage();
	}
	i = 1;

	while (i < argc)
	{
		if (EQUAL(argv[i], "--fuel-model-number"))
		{
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				fuelModelNumber = parseFuelModelNumber(i, MAX_ARG_INDEX, argString);
			}
			else
			{
				printf("ERROR: No valid value entered for fuel model number\n");
				Usage();
			}
			if (!behave.isFuelModelDefined(fuelModelNumber))
			{
				printf("ERROR: Fuel model %d is undefined\n", fuelModelNumber);
				Usage();
			}
			requiredArgArray[FUEL_MODEL_NUMBER] = true;
		}
		else if (EQUAL(argv[i], "--1hr"))
		{
			argName = "1-hr moisture";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				moistureOneHr = parseMoisture(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for 1-hr moisture\n");
				Usage();
			}
			requiredArgArray[ONE_HOUR] = true;
		}
		else if (EQUAL(argv[i], "--10hr"))
		{
			argName = "10-hr moisture";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				moistureTenHr = parseMoisture(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for 10-hr moisture\n");
				Usage();
			}
			requiredArgArray[TEN_HOUR] = true;
		}
		else if (EQUAL(argv[i], "--100hr"))
		{
			argName = "100-hr moisture";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				moistureHundredHr = parseMoisture(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for 100-hr moisture\n");
				Usage();
			}
			requiredArgArray[HUNDRED_HOUR] = true;
		}
		else if (EQUAL(argv[i], "--live-herb"))
		{
			argName = "live herbaceous moisture";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				moistureLiveHerb = parseMoisture(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for live herbaceous moisture\n");
				Usage();
			}
			requiredArgArray[LIVE_HERB] = true;
		}
		else if (EQUAL(argv[i], "--live-woody"))
		{
			argName = "live woody moisture";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				moistureLiveWoody = parseMoisture(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for live woody moisture\n");
				Usage();
			}
			requiredArgArray[LIVE_WOODY] = true;
		}
		else if (EQUAL(argv[i], "--wind-speed"))
		{
			argName = "wind speed";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				windSpeed = parseDouble(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for wind speed\n");
				Usage();
			}
			requiredArgArray[WIND_SPEED] = true;
		}
		else if (EQUAL(argv[i], "--wind-direction"))
		{
			argName = "wind direction";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				windDirection = parseDouble(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for wind direction\n");
				Usage();
			}
			checkAngleBound(windDirection);
			requiredArgArray[WIND_DIRECTION] = true;
		}
		else if (EQUAL(argv[i], "--slope"))
		{
			argName = "slope";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				slope = parseDouble(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for slope\n");
				Usage();
			}
			checkSlopeBound(slope);
			requiredArgArray[SLOPE] = true;
		}
		else if (EQUAL(argv[i], "--aspect"))
		{
			argName = "aspect";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				aspect = parseDouble(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for aspect\n");
				Usage();
			}
			checkAngleBound(aspect);
			requiredArgArray[ASPECT] = true;
		}
		else if (EQUAL(argv[i], "--direction-of-interest"))
		{
			argName = "direction of interest";
			if ((i + 1) <= MAX_ARG_INDEX)
			{
				argString = argv[++i];
				directionOfInterest = parseDouble(i, MAX_ARG_INDEX, argString, argName);
			}
			else
			{
				printf("ERROR: No valid value entered for direction of interest\n");
				Usage();
			}
			checkAngleBound(directionOfInterest);
			hasDirectionOfInterest = true;
		}
		else if (EQUAL(argv[i], "--output-to-file"))
		{
			isOutputtingToFile = true;
		}
		else if (EQUAL(argv[i], "--file-name"))
		{
			if ((i + 1) > MAX_ARG_INDEX)
			{
				printf("ERROR: No file name entered\n");
				Usage();
			}
			hasSpecifiedFileName = true;
			fileName = argv[++i];
			if (!(fileName.substr(fileName.find_last_of(".") + 1) == "txt"))
			{
				fileName += ".txt";
			}
		}
		else if (EQUAL(argv[i], "--append"))
		{
			isAppending = true;
		}
		else if (EQUAL(argv[i], "--run-identifier"))
		{
			if ((i + 1) > MAX_ARG_INDEX)
			{
				printf("ERROR: No run identifier entered\n");
				Usage();
			}
			hasRunIdentifier = true;
			runIdentifier = argv[++i];
		}
		//else if(EQUAL(argv[i], "--verbose"))
		//{
		//	verbose = true;
		//}
		else
		{
			Usage();
		}
		i++;
	}

	// Check for required arguments
	for (int i = 0; i < REQUIRED_ARG_COUNT; i++)
	{
		if (requiredArgArray[i] == false)
		{
			std::string errorString = "ERROR: Missing required argument ";
			errorString += getArgumentName(i);
			//throw std::runtime_error(errorString);
			printf(errorString.c_str());
			printf("\n");
			isMissingReq = true;
		}
	}
	if (isMissingReq)
	{
		exit(1);
	}

	if (!isOutputtingToFile && hasSpecifiedFileName)
	{
		printf("ERROR: Must be printing a file to specify a file name\n");
		Usage();
	}

	if (!isOutputtingToFile && isAppending)
	{
		printf("ERROR: Must be printing a file to append to a file\n");
		Usage();
	}

	if (!isOutputtingToFile && hasRunIdentifier)
	{
		printf("ERROR: Must be printing a file to specify a run identifer\n");
		Usage();
	}

	behave.updateSurfaceInputs(fuelModelNumber, moistureOneHr, moistureTenHr, moistureHundredHr, moistureLiveHerb, moistureLiveWoody, windSpeed, windDirection, slope, aspect);

	if (hasDirectionOfInterest)
	{
		spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
	}
	else
	{
		spreadRate = behave.calculateSurfaceFireForwardSpreadRate();
	}

	flameLength = behave.getFlameLength();
	directionOfMaxSpread = behave.getDirectionOfMaxSpread();

	if (isOutputtingToFile)
	{
		FILE *fout;
		if (isAppending)
		{
			//fopen_s(&fout, fileName.c_str(), "at");
			fout = fopen(fileName.c_str(), "at");
		}
		else
		{
			//fopen_s(&fout, fileName.c_str(), "wt");
			fout = fopen(fileName.c_str(), "wt");
		}
		if (hasRunIdentifier)
		{
			fprintf(fout, runIdentifier.c_str());
			fprintf(fout, "\t");
		}
		fprintf(fout, "Spread_rate_(ch/hr)\t %lf\t" \
			"Flame_length(ft)\t %lf\t" \
			"Direction_of_max_spread(degrees)\t %lf\n",
			spreadRate, flameLength, directionOfMaxSpread);
		fclose(fout);
	}

	printf("Spread_rate_(ch/hr)\t\t\t%lf\n", spreadRate);
	printf("Flame_length(ft)\t\t\t%lf\n", flameLength);
	printf("Direction_of_max_spread(degrees)\t%lf\n", directionOfMaxSpread);

	return 0;
}

