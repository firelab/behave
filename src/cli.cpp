#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

#include <stdlib.h>
#include <stdio.h>
#include "behave.h"

#define EQUAL(a,b) (strcmp(a,b)==0)

void Usage()
{
	printf("Usage:\n");
	printf("behave_cli [--fuel_model_number number]\n");
	printf("           [--1hr percent]\n");
	printf("           [--10hr percent]\n");
	printf("           [--100hr percent]\n");
	printf("           [--live_herb percent]\n");
	printf("           [--live_woody percent]\n");
	printf("           [--wind_speed speed]\n");
	printf("           [--wind_direction degrees]\n");
	printf("           [--slope degrees]\n");
	printf("           [--aspect degrees]\n");
	printf("           [--direction-of-interest degrees]\n");
	printf("           [--output-to-file]\n");
	printf("           [--file-name name]\n");
	printf("           [--append]\n");
	printf("           [--run-identifier name]\n");
	//printf("           [--verbose]\n");
	printf("\n");
	printf("Arguments:\n");
	printf("\n");
	printf("--fuel-model-number <number>        The fuel model number \n");
	printf("--1hr <percent>                     1-hr fuel moisture\n");
	printf("--10hr <percent>                    10-hr fuel moisture\n");
	printf("--100hr <percent>                   100-hr fuel moisture\n");
	printf("--live-herb <percent>               Live herbacious fuel moisture\n");
	printf("--live-woody <percent>              Live woody fuel moisture\n");
	printf("--wind-speed <speed>                Mid-flame wind speed\n");
	printf("--wind-direction <degrees>          Wind direction\n");
	printf("--slope <degrees>                   Slope\n");
	printf("--aspect <degrees>                  Aspect\n");
	printf("--direction-of-interest <degrees>   Optional: calculate spread\n");
	printf("                                    rate in direction other than max\n");
	printf("--output-to-file                    Optional: output to a txt file\n");
	printf("                                    default file name: output.txt\n");
	printf("--file-name <name>                  Optional: specify output file name\n");
	printf("--output-to-file                    Optional: append run to output file\n");
	printf("--run-identifier <name>             Optional: specify identifier for run");
	exit(1);
}

void checkMoistureBound(double moisture)
{
	if ((moisture < 0.0) || (moisture > 300.00))
	{
		throw std::runtime_error("ERROR: Moisture cannot negative or greater than 300.\n");
	}
}

void checkAngleBound(double angle)
{
	if ((angle < 0.0) || (angle > 360.00))
	{
		throw std::runtime_error("ERROR: Angles cannot negative or greater than 360.\n");
	}
}

int main(int argc, char *argv[])
{
	bool isOutputtingToFile = false;
	bool hasSpecifiedFileName = false;
	bool isAppending = false;
	bool hasDirectionOfInterest = false;
	bool hasRunIdentifier = false;
	int i;
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

	Behave behave;

	if (argc == 1)
	{
		Usage();
	}

	i = 1;
	while(i < argc)
	{
		if (EQUAL(argv[i], "--fuel-model-number"))
		{
			fuelModelNumber = atoi(argv[++i]);
			if (!behave.isFuelModelDefined(fuelModelNumber))
			{
				throw std::runtime_error("ERROR: Fuel model is undefined.\n");
			}
		}
		else if(EQUAL(argv[i], "--1hr"))
		{
			moisture1h = atof(argv[++i]);
			checkMoistureBound(moisture1h);
			moisture1h /= 100;
		}
		else if(EQUAL(argv[i], "--10hr"))
		{
			moisture10h = atof(argv[++i]);
			checkMoistureBound(moisture10h);
			moisture10h /= 100;
		}
		else if(EQUAL(argv[i], "--100hr"))
		{
			moisture100h = atof(argv[++i]);
			moisture100h /= 100;
			checkMoistureBound(moisture10h);
		}
		else if(EQUAL(argv[i], "--live-herb"))
		{
			moistureLiveHerb = atof(argv[++i]);
			checkMoistureBound(moisture10h);
			moistureLiveHerb /= 100;
		}
		else if(EQUAL(argv[i], "--live-woody"))
		{
			moistureLiveWoody = atof(argv[++i]);
			checkMoistureBound(moisture10h);
			moistureLiveWoody /= 100;
		}
		else if(EQUAL(argv[i], "--wind-speed"))
		{
			windSpeed = atof(argv[++i]);
			checkMoistureBound(moisture10h);
		}
		else if(EQUAL(argv[i], "--wind-direction"))
		{
			windDirection = atof(argv[++i]);
			checkMoistureBound(moisture10h);
		}
		else if(EQUAL(argv[i], "--slope"))
		{
			slope = atof(argv[++i]);
			checkAngleBound(slope);
		}
		else if (EQUAL(argv[i], "--aspect"))
		{
			aspect = atof(argv[++i]);
			checkAngleBound(aspect);
		}
		else if (EQUAL(argv[i], "--direction-of-interest"))
		{
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
			hasSpecifiedFileName = true;
			fileName = argv[++i];
		}
		else if (EQUAL(argv[i], "--append"))
		{
			isAppending = true;
		}
		else if (EQUAL(argv[i], "--run-identifier"))
		{
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
	
	if (!isOutputtingToFile && hasSpecifiedFileName)
	{
		throw std::runtime_error("Must be printing a file to specify a file name\n");
		Usage();
	}

	if (!isOutputtingToFile && hasRunIdentifier)
	{
		throw std::runtime_error("Must be printing a file to specify a run identifer\n");
		Usage();
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

