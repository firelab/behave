#define _CRT_SECURE_NO_WARNINGS // Disable warnings for fopen()

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "fuelModels.h"
#include "behaveRun.h"

#define EQUAL(a,b) (strcmp(a,b)==0)

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
    printf("behave-raws-batch [--input-file-name name]   Optional\n");
    printf("                  [--output-file-name name]  Optional\n");
    printf("--input-file-name <name>                Optional: Specify input file name\n");
    printf("                                            default file name: input.txt\n");
    printf("--output-file-name <name>               Optional: Specify output file name\n");
    printf("                                            default file name: output.txt\n");
    printf("\nA properly formatted input file consisting of RAWS data must exist\n");
    printf("RAWS data must be comma delimited and inputs for each behave run separated\nby a new line");
    printf("Inputs must be in the following order within a line:\n");
    printf("RAWS_ID,DATE_TIME,OBSERVED_OR_PREDICTED,FUEL_MODEL_NUMBER,ONE_HOUR_MOISTURE\n");
    printf("TEN_HOUR_MOISTURE, HUNDRED_HOUR_MOISTURE, LIVE_HERB_MOISTURE,\n");
    printf("LIVE_WOODY_MOISTURE,WIND_SPEED,WIND_DIRECTION,SLOPE,ASPECT\n\n");
    printf("FUEL_MODEL_NUMBER must be an integer, all subsequent values must be\nfloating point. ");
    printf("Moisture is given in percent, midflame wind speed must be\nm/s, and wind direction,");
    printf("slope, and aspect must be in degrees. All directions\nmust be relative to compass north.");
    printf("Example of a properfly formated line of \ndata shown below:\n");
    printf("04V,2015-01-01 03:00:00,obs,101,13.7477,4.8854,4.8718,120,60,0.7333,340,0,-1\n\n");

    exit(1); // Exit with error code 1
}

int main(int argc, char *argv[])
{
    const int MAX_ARGUMENT_INDEX = argc - 1;

    std::string inputFileName = "input.txt"; // default input file name
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
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;
    double spreadRate = 0;

    FuelModels fuelModels;
    BehaveRun behave(fuelModels);

    std::string line = "";
    std::string token = "";
    std::string outputLine = "";
    std::string spreadRateString = "";
    std::string flameLengthString = "";

    int argIndex = 1;
    // Parse commandline arguments
    if (argc > 1)
    {
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
                    // Give the output file a .txt extension
                    outFileName += ".txt";
                }
            }
            else if (EQUAL(argv[argIndex], "--input-file-name"))
            {
                if ((argIndex + 1) > MAX_ARGUMENT_INDEX) // An error has occurred
                {
                    // Report error
                    printf("ERROR: No input file name entered\n");
                    Usage(); // Exits program
                }
                inputFileName = argv[++argIndex];
                if (!(inputFileName.substr(inputFileName.find_last_of(".") + 1) == "txt")) // Output is not yet a .txt file
                {
                    // Give the input file a .txt extension
                    inputFileName += ".txt";
                }
            }
            else
            {
                printf("ERROR: %s is an invalid argument\n", argv[argIndex]);
                Usage(); // Exits program
            }
            argIndex++;
        }
    }

    if (inputFileName.compare(outFileName) == 0)
    {
        // Report error
        printf("ERROR: input file name cannot be same as output file name\n");
        Usage(); // Exits program
    }

    std::ofstream outputFile(outFileName, std::ios::out);
    std::ifstream inputFile(inputFileName, std::ifstream::in);

    // Check for input file's existence
    if (!inputFile)
    {
        // Report error
        printf("ERROR: input file does not exist\n");
        Usage(); // Exits program
    }

    std::stringstream lineStream;
    bool badData = false;


    printf("Processing files please wait...\n");
    // Start reading input file
    int tokenCounter = 0;
    int lineCounter = 0;

    while(getline(inputFile, line))
    {
        // Reset variables for new loop iteration
        lineStream.str("");
        lineStream.clear();
        token = "";
        outputLine = "";
        spreadRateString = "";
        flameLengthString = "";
        tokenCounter = 0;
        runIdentifier = "";
        badData = false;

        // Parse arguments from a single line
        lineStream << line;
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
                    if (!token.compare("NA") == 0)
                    {
                        fuelModelNumber = std::stoi(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (!behave.isFuelModelDefined(fuelModelNumber))
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case ONE_HOUR:
                {
                    if (!token.compare("NA") == 0)
                    {
                        moistureOneHr = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (moistureOneHr < 0 || moistureOneHr > 1000)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case TEN_HOUR:
                {
                    if (!token.compare("NA") == 0)
                    {
                        moistureTenHr = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (moistureTenHr < 0 || moistureTenHr > 1000)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case HUNDRED_HOUR:
                {
                    if (!token.compare("NA") == 0)
                    {
                        moistureHundredHr = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (moistureHundredHr < 0 || moistureHundredHr > 1000)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case LIVE_HERB:
                {
                    if (!token.compare("NA") == 0)
                    {
                        moistureLiveHerb = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (moistureLiveHerb < 0 || moistureLiveHerb > 1000)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case LIVE_WOODY:
                {
                    if (!token.compare("NA") == 0)
                    {
                        moistureLiveWoody = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (moistureLiveWoody < 0 || moistureLiveWoody > 1000)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case WIND_SPEED:
                {
                    if (!token.compare("NA") == 0)
                    {
                        windSpeed = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (windSpeed < 0 || windSpeed > 1000)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case WIND_DIRECTION:
                {
                    if (!token.compare("NA") == 0)
                    {
                        windDirection = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (windDirection < -360 || windDirection > 360)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case SLOPE:
                {
                    if (!token.compare("NA") == 0)
                    {
                        slope = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (slope < 0 || slope > 82)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                case ASPECT:
                {
                    if (!token.compare("NA") == 0)
                    {
                        aspect = std::stod(token);
                    }
                    else
                    {
                        // Data is bad
                        badData = true;
                    }
                    if (aspect < -360|| aspect > 360)
                    {
                        // Data is bad
                        badData = true;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            tokenCounter++;
        }

        lineCounter++;
        if (lineCounter % 10000 == 0)
        {
            printf("processed %d behave runs\n", lineCounter);
        }

        // If data is not bad, do calculations
        if (!badData)
        {
            // Feed input values to behave
            behave.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHr,
                moistureTenHr, moistureHundredHr, moistureLiveHerb,
                moistureLiveWoody, MoistureUnits::Percent, windSpeed,
                SpeedUnits::MetersPerSecond,
                WindHeightInputMode::DirectMidflame, windDirection,
                WindAndSpreadOrientationMode::RelativeToNorth, slope,
                SlopeUnits::Degrees, aspect, canopyCover, CoverUnits::Percent,
                canopyHeight, LengthUnits::Feet, crownRatio);
            // Calculate spread rate and flame length
            behave.surface.doSurfaceRunInDirectionOfMaxSpread();
            // Get the surface fire spread rate
            spreadRate = behave.surface.getSpreadRate(SpeedUnits::MetersPerSecond);
            // Get other required outputs
            flameLength = behave.surface.getFlameLength(LengthUnits::Meters);
            // Convert data to string for output to file
            spreadRateString = std::to_string(spreadRate);
            flameLengthString = std::to_string(flameLength);
        }
        else
        {
            // Data is bad
            spreadRateString = "NA";
            flameLengthString = "NA";
        }

        // Print line to output file
        outputLine = runIdentifier + "," + spreadRateString + "," + flameLengthString;
        outputFile << outputLine << '\n';
    }

    // Close input and output files
    inputFile.close();
    outputFile.close();

    printf("Done!\n\n");

    return 0; // Success
}

