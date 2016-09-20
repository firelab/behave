#define _CRT_SECURE_NO_WARNINGS // Disable warnings for fopen()

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "fuelModelSet.h"
#include "behaveRun.h"

#define EQUAL(a,b) (strcmp(a,b)==0)

const int REQUIRED_ARG_COUNT = 10;

enum
{
    RAWS_ID,
    DATE_TIME,
    OBSERVED_OR_PREDICTED,
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
    printf("behave-batch [--input-file-name name]   Optional\n");
    printf("             [--output-file-name name]  Optional\n");
    printf("--output-file-name <name>               Optional: Specify input file name\n");
    printf("                                            default file name: input.txt\n");
    printf("--output-file-name <name>               Optional: Specify output file name\n");
    printf("                                            default file name: output.txt\n");
    printf("A properly formatted input file consisting of RAWS data must exist\n");
    printf("RAWS data must be comma delimited and each behave run separated by a new line");
    printf("Inputs must be in the following order : ");
    printf("RAWS_ID,DATE_TIME,OBSERVED_OR_PREDICTED,FUEL_MODEL_NUMBER,ONE_HOUR_MOISTURE\n");
    printf("TEN_HOUR_MOISTURE, HUNDRED_HOUR_MOISTURE, LIVE_HERB_MOISTURE,\n");
    printf("LIVE_WOODY_MOISTURE,WIND_SPEED,WIND_DIRECTION,SLOPE,ASPECT   \n");
    printf("Example of a properfly formated line of data shown below:\n");
    printf("04V,2015-01-01 03:00:00,obs,101,13.7477,4.8854,4.8718,120,60,0.733349036493112,340,0,-1\n\n");
    exit(1); // Exit with error code 1
}

int main(int argc, char *argv[])
{
    const int MAX_ARGUMENT_INDEX = argc - 1;

    const double METERS_PER_SECOND_TO_MILES_PER_HOUR = 2.236936;
    const double CHAINS_PER_HOUSR_TO_METERS_PER_SECOND = 0.005588;
    const double FEET_TO_METERS = 0.3048;

    std::string inFileName = "input.txt"; // default input file name
    std::string outFileName = "output.txt"; // default output file name
    std::string runIdentifier = "";
  
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

    // Wind adjustment factor parameters
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;
    double spreadRate = 0;

    FuelModelSet fuelModelSet;
    BehaveRun behave(fuelModelSet);
    behave.setSlopeInputMode(SlopeInputMode::SLOPE_IN_DEGREES);
    behave.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);

    
    std::string line = "";
  
    std::string token = "";
    std::string outputLine = "";
    std::string spreadRateString = "";
    std::string flameLengthString = "";

    int argIndex = 1;
    // Parse commandline arguments
    while (argIndex < argc)
    {
        if (EQUAL(argv[argIndex], "--output-file-name"))
        {
            if ((argIndex + 1) > MAX_ARGUMENT_INDEX) // An error has occurred
            {
                // Report error
                printf("ERROR: No output file name entered\n");
                Usage(); // Exits program
            }
            outFileName = argv[++argIndex];
            if (!(outFileName.substr(outFileName.find_last_of(".") + 1) == "txt")) // Output is not yet a .txt file
            {
                // Give the file a .txt extension
                outFileName += ".txt";
            }
        }
        if (EQUAL(argv[argIndex], "--input-file-name"))
        {
            if ((argIndex + 1) > MAX_ARGUMENT_INDEX) // An error has occurred
            {
                // Report error
                printf("ERROR: No input file name entered\n");
                Usage(); // Exits program
            }
            inFileName = argv[++argIndex];
            if (!(inFileName.substr(inFileName.find_last_of(".") + 1) == "txt")) // Output is not yet a .txt file
            {
                // Give the file a .txt extension
                inFileName += ".txt";
            }
        }
        argIndex++;
    }

    if (inFileName.compare(outFileName) == 0)
    {
        // Report error
        printf("ERROR: input file name cannot be same as output file name\n");
        Usage(); // Exits program
    }

    std::ofstream outFile(outFileName, std::ios::out);
    std::ifstream inputFileStream(inFileName, std::ifstream::in);

    int tokenCounter = 0;

    if (!inputFileStream)
    {
        printf("ERROR: input file does not exist\n");
        Usage(); // Exits program
    }

    while(getline(inputFileStream, line))
    {
        std::stringstream lineStream(line);
        token = "";
        outputLine = "";
        spreadRateString = "";
        flameLengthString = "";
        tokenCounter = 0;
        runIdentifier = "";

        // Parse arguments from a single line
        while(std::getline(lineStream, token, ','))
        {
            switch (tokenCounter)
            {
                case RAWS_ID:
                {
                    runIdentifier += token + ",";
                    break;
                }
                case DATE_TIME:
                {
                    runIdentifier += token + ",";
                    break;
                }
                case OBSERVED_OR_PREDICTED:
                {
                    runIdentifier += token;
                    break;
                }
                case FUEL_MODEL_NUMBER:
                {
                    fuelModelNumber = std::stoi(token);
                    break;
                }
                case ONE_HOUR:
                {
                    moistureOneHr = std::stod(token);
                    break;
                }
                case TEN_HOUR:
                {
                    moistureTenHr = std::stod(token);
                    break;
                }
                case HUNDRED_HOUR:
                {
                    moistureHundredHr = std::stod(token);
                    break;
                }
                case LIVE_HERB:
                {
                    moistureLiveHerb = std::stod(token);              
                    break;
                }
                case LIVE_WOODY:
                {
                    moistureLiveWoody = std::stod(token);
                    break;
                }
                case WIND_SPEED:
                {
                    windSpeed = std::stod(token);
                    break;
                }
                case WIND_DIRECTION:
                {
                    windDirection = std::stod(token);
                    break;
                }
                case SLOPE:
                {
                    slope = std::stod(token);
                    break;
                }
                case ASPECT:
                {
                    aspect = std::stod(token);
                    break;
                }
                default:
                {
                    break;
                }
            }
            tokenCounter++;
        }

        // Convert windspeed from m/s to mi/hr for Behave's internal calculations
        windSpeed *= METERS_PER_SECOND_TO_MILES_PER_HOUR;

        // Feed input values to behave
        behave.updateSurfaceInputs(fuelModelNumber, moistureOneHr, moistureTenHr, moistureHundredHr, moistureLiveHerb, moistureLiveWoody, WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, canopyCover, canopyHeight, crownRatio);

        // Calculate spread rate and flame length
        behave.doSurfaceRunInDirectionOfMaxSpread();

        // Get the surface fire spread rate
        spreadRate = behave.getSurfaceFireSpreadRate();

        // Get other required outputs
        flameLength = behave.getFlameLength();
        directionOfMaxSpread = behave.getDirectionOfMaxSpread();

        // Convert output to metric
        spreadRate *= CHAINS_PER_HOUSR_TO_METERS_PER_SECOND;
        flameLength *= FEET_TO_METERS;

        spreadRateString = std::to_string(spreadRate);
        flameLengthString = std::to_string(flameLength);

        outputLine = runIdentifier + "," + spreadRateString + "," + flameLengthString;
        outFile << outputLine << std::endl;
    }

    outFile.close();

    return 0; // Success
}

