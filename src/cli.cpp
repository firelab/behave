#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

#include <stdlib.h>
#include <stdio.h>
#include "behave.h"

#define EQUAL(a,b) (strcmp(a,b)==0)

const int REQUIRED_ARG_COUNT = 10;
enum RequiredArgs {
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
	printf("Usage:\n");
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
	printf("Arguments:\n");
	printf("\n");
	printf("--fuel-model-number <number>        Required: The fuel model number \n");
	printf("--1hr <percent>                     Required: 1-hr fuel moisture\n");
	printf("--10hr <percent>                    Required: 10-hr fuel moisture\n");
	printf("--100hr <percent>                   Required: 100-hr fuel moisture\n");
	printf("--live-herb <percent>               Required: Live herbacious fuel moisture\n");
	printf("--live-woody <percent>              Required: Live woody fuel moisture\n");
	printf("--wind-speed <speed>                Required: Mid-flame wind speed\n");
	printf("--wind-direction <degrees>          Required: Wind direction\n");
	printf("--slope <degrees>                   Required: Slope\n");
	printf("--aspect <degrees>                  Required: Aspect\n");
	printf("--direction-of-interest <degrees>   Optional: calculate spread rate in a\n");
	printf("                                       direction other than max\n");
	printf("--output-to-file                    Optional: output to a txt file\n");
	printf("                                       default file name: output.txt\n");
	printf("--file-name <name>                  Optional: specify output file name\n");
	printf("--output-to-file                    Optional: append run to output file\n");
	printf("--run-identifier <name>             Optional: specify identifier for run");
	exit(1);
}

void checkMoistureBound(double moisture)
{
	if ((moisture < 0.0) || (moisture > 300.00))
	{
		//throw std::runtime_error("ERROR: Moisture cannot negative or greater than 300.\n");
		printf("ERROR: Moisture cannot negative or greater than 300.\n");
	}
}

void checkAngleBound(double angle)
{
	if ((angle < 0.0) || (angle > 360.00))
	{
		//throw std::runtime_error("ERROR: Angles cannot negative or greater than 360.\n");
		printf("ERROR: Angles cannot negative or greater than 360.\n");
	}
}

std::string getArgumentName(int argIndex)
{
	std::string retString = "";
	if (argIndex == RequiredArgs::ASPECT)
	{
		retString = "aspect";
	}
	if (argIndex == RequiredArgs::FUEL_MODEL_NUMBER)
	{
		retString = "fuel model number";
	}
	if (argIndex == RequiredArgs::HUNDRED_HOUR)
	{
		retString = "100-hr moisture";
	}
	if (argIndex == RequiredArgs::LIVE_HERB)
	{
		retString = "live herbaceous moisture";
	}
	if (argIndex == RequiredArgs::LIVE_WOODY)
	{
		retString = "live woody moisture";
	}
	if (argIndex == RequiredArgs::ONE_HOUR)
	{
		retString = "1-hr moisture";
	}
	if (argIndex == RequiredArgs::SLOPE)
	{
		retString = "slope";
	}
	if (argIndex == RequiredArgs::TEN_HOUR)
	{
		retString = "10-hr moisture";
	}
	if (argIndex == RequiredArgs::WIND_DIRECTION)
	{
		retString = "wind direction";
	}
	if (argIndex == RequiredArgs::WIND_SPEED)
	{
		retString = "wind speed";
	}

	return retString;
}

int main(int argc, char *argv[])
{
	bool requiredArgArray[REQUIRED_ARG_COUNT];
	bool isOutputtingToFile = false;
	bool hasSpecifiedFileName = false;
	bool isAppending = false;
	bool hasDirectionOfInterest = false;
	bool hasRunIdentifier = false;
	int i;
	int test_i;
	std::string fileName = "output.txt"; // default output file name
	std::string runIdentifier = "";

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

	for (int i = 0; i < REQUIRED_ARG_COUNT; i++)
	{
		requiredArgArray[i] = false;
	}

	Behave behave;

	// Parse command line arguments
	if (argc == 1)
	{
		Usage();
	}
	i = 1;
	while(i < argc)
	{
		if (EQUAL(argv[i], "--fuel-model-number"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for fuel model number\n");
				break;
			}
			fuelModelNumber = atoi(argv[++i]);
			if (!behave.isFuelModelDefined(fuelModelNumber))
			{
				throw std::runtime_error("ERROR: Fuel model is undefined.\n");
			}
			requiredArgArray[RequiredArgs::FUEL_MODEL_NUMBER] = true;
		}
		else if(EQUAL(argv[i], "--1hr"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for 1-hr moisture\n");
				break;
			}
			moisture1h = atof(argv[++i]);
			checkMoistureBound(moisture1h);
			moisture1h /= 100;
			requiredArgArray[RequiredArgs::ONE_HOUR] = true;
		}
		else if(EQUAL(argv[i], "--10hr"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for 10-hr moisture\n");
				break;
			}
			moisture10h = atof(argv[++i]);
			checkMoistureBound(moisture10h);
			moisture10h /= 100;
			requiredArgArray[RequiredArgs::TEN_HOUR] = true;
		}
		else if(EQUAL(argv[i], "--100hr"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for 100-hr moisture\n");
				break;
			}
			moisture100h = atof(argv[++i]);
			checkMoistureBound(moisture100h);
			moisture100h /= 100;
			requiredArgArray[RequiredArgs::HUNDRED_HOUR] = true;
		}
		else if(EQUAL(argv[i], "--live-herb"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for live herbaceous moisture\n");
				break;
			}
			moistureLiveHerb = atof(argv[++i]);
			checkMoistureBound(moisture10h);
			moistureLiveHerb /= 100;
			requiredArgArray[RequiredArgs::LIVE_HERB] = true;
		}
		else if(EQUAL(argv[i], "--live-woody"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for live woody moisture\n");
				break;
			}
			moistureLiveWoody = atof(argv[++i]);
			checkMoistureBound(moisture10h);
			moistureLiveWoody /= 100;
			requiredArgArray[RequiredArgs::LIVE_WOODY] = true;
		}
		else if(EQUAL(argv[i], "--wind-speed"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for wind speed\n");
				break;
			}
			windSpeed = atof(argv[++i]);
			requiredArgArray[RequiredArgs::WIND_SPEED] = true;
		}
		else if(EQUAL(argv[i], "--wind-direction"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for wind direction\n");
				break;
			}
			windDirection = atof(argv[++i]);
			checkAngleBound(moisture10h);
			requiredArgArray[RequiredArgs::WIND_DIRECTION] = true;
		}
		else if(EQUAL(argv[i], "--slope"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for slope\n");
				break;
			}
			slope = atof(argv[++i]);
			checkAngleBound(slope);
			requiredArgArray[RequiredArgs::SLOPE] = true;
		}
		else if (EQUAL(argv[i], "--aspect"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for aspect\n");
				break;
			}
			aspect = atof(argv[++i]);
			checkAngleBound(aspect);
			requiredArgArray[RequiredArgs::ASPECT] = true;
		}
		else if (EQUAL(argv[i], "--direction-of-interest"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No value entered for direction of interest\n");
				break;
			}
			directionOfInterest = atof(argv[++i]);
			checkAngleBound(directionOfInterest);
			hasDirectionOfInterest = true;
		}
		else if(EQUAL(argv[i], "--output-to-file"))
		{
			isOutputtingToFile = true;
		}
		else if (EQUAL(argv[i], "--file-name"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No file name entered\n");
				break;
			}
			hasSpecifiedFileName = true;
			fileName = argv[++i];
		}
		else if (EQUAL(argv[i], "--append"))
		{
			isAppending = true;
		}
		else if (EQUAL(argv[i], "--run-identifier"))
		{
			test_i = i + 1;
			if (argv[test_i] == NULL)
			{
				printf("ERROR: No run identifier entered\n");
				break;
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
	int argID;
	for (int i = 0; i < REQUIRED_ARG_COUNT; i++)
	{
		if (requiredArgArray[i] == false)
		{
			std::string errorString = "ERROR: Missing required argument ";
			argID = i;
			errorString += getArgumentName(argID);
			//throw std::runtime_error(errorString);
			printf(errorString.c_str());
			printf("\n");
		}
	}

	if (!isOutputtingToFile && hasSpecifiedFileName)
	{
		//throw std::runtime_error("Must be printing a file to specify a file name\n");
	}

	if (!isOutputtingToFile && hasRunIdentifier)
	{
		//throw std::runtime_error("Must be printing a file to specify a run identifer\n");
	}

	behave.updateSurfaceInputs(fuelModelNumber, moisture1h, moisture10h, moisture100h, moistureLiveHerb, moistureLiveWoody, windSpeed, windDirection, slope, aspect);

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
		//fout = fopen(fileName.c_str(), "wt");
		if (isAppending)
		{
			fopen_s(&fout, fileName.c_str(), "at");
		}
		else
		{
			fopen_s(&fout, fileName.c_str(), "wt");
		}
		if (hasRunIdentifier)
		{
			fprintf(fout, runIdentifier.c_str());
			fprintf(fout,"\t");
		}
		fprintf(fout, "Spread_rate_(ch/hr)\t %lf\t" \
			"Flame_length(ft)\t %lf\t" \
			"Direction_of_max_spread(degrees)\t %lf\n", 
			spreadRate, flameLength, directionOfMaxSpread);
		fclose(fout);
	}

	return 0;
}

