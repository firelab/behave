#define _CRT_SECURE_NO_WARNINGS // Disable warnings for fopen()

#include <iostream>
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
    printf("behave-cli [--fuel-model-number number]      Required\n");
    printf("           [--1hr percent]                   Required\n");
    printf("           [--10hr percent]                  Required\n");
    printf("           [--100hr percent]                 Required\n");
    printf("           [--live-herb percent]             Required\n");
    printf("           [--live-woody percent]            Required\n");
    printf("           [--wind-speed speed]              Required\n");
    printf("           [--wind-direction degrees]        Required\n");
    printf("           [--slope degrees]                 Required\n");
    printf("           [--aspect degrees]                Required\n");
    printf("           [--metric]                        Optional\n");
    printf("           [--english]                       Optional\n");
    printf("           [--direction-of-interest degrees] Optional\n");
    printf("           [--output-to-file]                Optional\n");
    printf("           [--file-name name]                Optional\n");
    printf("           [--append]                        Optional\n");
    printf("           [--run-identifier name]           Optional\n");
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
    printf("--metric                            Optional: In/Output is in metric units a\n");
    printf("                                       default units: English\n");
    printf("--english                           Optional: In/Output is in English units a\n");
    printf("                                       default units: English\n");
    printf("--direction-of-interest <degrees>   Optional: Calculate spread rate in a\n");
    printf("                                       direction other than maximum\n");
    printf("--output-to-file                    Optional: Output to a txt file\n");
    printf("                                       default file name: output.txt\n");
    printf("--file-name <name>                  Optional: Specify output file name\n");
    printf("--append                            Optional: Append run to output file\n");
    printf("--run-identifier <name>             Optional: Specify identifier for run\n");
    exit(1); // Exit with error code 1
}

void checkAngleBound(double angle)
{
    if ((angle < 0.0) || (angle > 360.00))
    {
        printf("ERROR: Angles cannot be negative or greater than 360.\n");
        Usage();
    }
}

void checkSlopeBound(double angle)
{
    checkAngleBound(angle);
    if (angle > 80.0)
    {
        printf("ERROR: Slope cannot exceed 80 degrees.\n");
        Usage();
    }
}

void checkMoistureBound(double moisture)
{
    if ((moisture < 0.0) || (moisture > 300.00))
    {
        printf("ERROR: Moisture cannot negative or greater than 300.\n");
        Usage();
    }
}

int parseFuelModelNumber(int i, const int MAX_ARG_INDEX, char *argv[])
{
    int parsedFuelModelNumber = -1;
    int fuelModelNumberIndex = i + 1;
    // Parse command line argument
    if ((fuelModelNumberIndex <= MAX_ARG_INDEX) && (argv[fuelModelNumberIndex] != NULL))
    {
        char* end;
        long val = strtol(argv[fuelModelNumberIndex], &end, 10);
        if (!end[0] && val >= 0)
        {
            parsedFuelModelNumber = val;
        }
    }
    if (parsedFuelModelNumber <= 0) // An error has occurred
    {
        // Report error
        printf("ERROR: No valid value entered for fuel model number");
        Usage();  // Exits program
    }

    return parsedFuelModelNumber;
}

double parseMoisture(int i, const int MAX_ARG_INDEX, char *argv[], std::string argName)
{
    double parsedMoistureValue = -1;
    int moistureValueIndex = i + 1;
    // Parse command line argument
    if ((moistureValueIndex <= MAX_ARG_INDEX) && (argv[moistureValueIndex] != NULL))
    {
        char* end;
        double val = strtod(argv[moistureValueIndex], &end);
        if (!end[0] && val >= 0)
        {
            parsedMoistureValue = val;
        }
    }
    if (parsedMoistureValue < 0) // An error has occurred
    {
        // Report error
        std::string errorString = "ERROR: No valid value entered for ";
        errorString += argName;
        printf("%s\n", errorString.c_str());
        Usage(); // Exits program
    }
    checkMoistureBound(parsedMoistureValue);
    return parsedMoistureValue;
}

double parseDouble(int i, const int MAX_ARG_INDEX, char *argv[], std::string argumentName)
{
    double parsedDouble = -1;
    int doubleValueIndex = i + 1;
    // Parse command line argument
    if ((doubleValueIndex <= MAX_ARG_INDEX) && (argv[doubleValueIndex] != NULL))
    {
        char* end;
        double val = strtod(argv[doubleValueIndex], &end);
        if (!end[0] && val >= 0)
        {
            parsedDouble = val;
        }
    }
    if (parsedDouble < 0) // An error has occured
    {
        // Report error
        std::string errorString = "ERROR: No valid value entered for ";
        errorString += argumentName;
        printf("%s\n", errorString.c_str());
        Usage(); // Exits program
    }

    return parsedDouble;
}

std::string getArgumentName(int argumentIndex)
{
    std::string argumentName = "";
    if (argumentIndex == ASPECT)
    {
        argumentName = "aspect";
    }
    if (argumentIndex == FUEL_MODEL_NUMBER)
    {
        argumentName = "fuel model number";
    }
    if (argumentIndex == HUNDRED_HOUR)
    {
        argumentName = "100-hr moisture";
    }
    if (argumentIndex == LIVE_HERB)
    {
        argumentName = "live herbaceous moisture";
    }
    if (argumentIndex == LIVE_WOODY)
    {
        argumentName = "live woody moisture";
    }
    if (argumentIndex == ONE_HOUR)
    {
        argumentName = "1-hr moisture";
    }
    if (argumentIndex == SLOPE)
    {
        argumentName = "slope";
    }
    if (argumentIndex == TEN_HOUR)
    {
        argumentName = "10-hr moisture";
    }
    if (argumentIndex == WIND_DIRECTION)
    {
        argumentName = "wind direction";
    }
    if (argumentIndex == WIND_SPEED)
    {
        argumentName = "wind speed";
    }
    return argumentName;
}

int main(int argc, char *argv[])
{
    const int MAX_ARGUMENT_INDEX = argc - 1;
    const double METERS_PER_SECOND_TO_MILES_PER_HOUR = 2.236936;
    const double CHAINS_PER_HOUSR_TO_METERS_PER_SECOND = 0.005588;
    const double FEET_TO_METERS = 0.3048;

    bool requiredArgumentArray[REQUIRED_ARG_COUNT];
    bool isOutputtingToFile = false;
    bool isMissingRequirement = false;
    bool hasSpecifiedFileName = false;
    bool isAppending = false;
    bool hasDirectionOfInterest = false;
    bool isUsingEnglish = false;
    bool isUsingMetric = false;
    int argIndex;

    std::string fileName = "output.txt"; // default output file name
    std::string runIdentifier = "no-run-identifier";
    std::string argumentName = "";

    // Surface Fire Inputs;
    int fuelModelNumber = 0;
    double moistureOneHr = 0.0;
    double moistureTenHr = 0.0;
    double moistureHundredHr = 0.0;
    double moistureLiveHerb = 0.0;
    double moistureLiveWoody = 0.0;
    double windSpeed = 0.0;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;
    double windDirection = 0.0;
    double slope = 0.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::DEGREES;
    double aspect = 0.0;
    double directionOfMaxSpread = 0.0;
    double flameLength = 0.0;
    double directionOfInterest = 0.0;

    // Wind adjustment factor parameters
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::FEET;
    double crownRatio = 0.0;

    double spreadRate = 0;

    for (int index = 0; index < REQUIRED_ARG_COUNT; index++)
    {
        requiredArgumentArray[index] = false;
    }

    FuelModelSet fuelModelSet;
    BehaveRun behave(fuelModelSet);

    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RELATIVE_TO_NORTH;

    // Check for presense command line arguments
    if (argc == 1)
    {
        Usage();
    }
    argIndex = 1;

    // Parse commandline arguments
    while (argIndex < argc)
    {
        if (EQUAL(argv[argIndex], "--fuel-model-number"))
        {
            fuelModelNumber = parseFuelModelNumber(argIndex, MAX_ARGUMENT_INDEX, argv);
            // Check if fuel model is defined
            if (!behave.isFuelModelDefined(fuelModelNumber))
            {
                // An error has occurred
                printf("ERROR: Fuel model %d is undefined\n", fuelModelNumber);
                Usage(); // Exits program
            }
            requiredArgumentArray[FUEL_MODEL_NUMBER] = true;
        }
        else if (EQUAL(argv[argIndex], "--1hr"))
        {
            argumentName = "1-hr moisture";
            moistureOneHr = parseMoisture(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            requiredArgumentArray[ONE_HOUR] = true;
        }
        else if (EQUAL(argv[argIndex], "--10hr"))
        {
            argumentName = "10-hr moisture";
            moistureTenHr = parseMoisture(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            requiredArgumentArray[TEN_HOUR] = true;
        }
        else if (EQUAL(argv[argIndex], "--100hr"))
        {
            argumentName = "100-hr moisture";
            moistureHundredHr = parseMoisture(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            requiredArgumentArray[HUNDRED_HOUR] = true;
        }
        else if (EQUAL(argv[argIndex], "--live-herb"))
        {
            argumentName = "live herbaceous moisture";
            moistureLiveHerb = parseMoisture(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            requiredArgumentArray[LIVE_HERB] = true;
        }
        else if (EQUAL(argv[argIndex], "--live-woody"))
        {
            argumentName = "live woody moisture";
            moistureLiveWoody = parseMoisture(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            requiredArgumentArray[LIVE_WOODY] = true;
        }
        else if (EQUAL(argv[argIndex], "--wind-speed"))
        {
            argumentName = "wind speed";
            windSpeed = parseDouble(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            requiredArgumentArray[WIND_SPEED] = true;
        }
        else if (EQUAL(argv[argIndex], "--wind-direction"))
        {
            argumentName = "wind direction";
            windDirection = parseDouble(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            checkAngleBound(windDirection);
            requiredArgumentArray[WIND_DIRECTION] = true;
        }
        else if (EQUAL(argv[argIndex], "--slope"))
        {
            argumentName = "slope";
            slope = parseDouble(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            checkSlopeBound(slope);
            requiredArgumentArray[SLOPE] = true;
        }
        else if (EQUAL(argv[argIndex], "--aspect"))
        {
            argumentName = "aspect";
            aspect = parseDouble(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            checkAngleBound(aspect);
            requiredArgumentArray[ASPECT] = true;
        }
        else if (EQUAL(argv[argIndex], "--metric"))
        {
            isUsingMetric = true;
        }
        else if (EQUAL(argv[argIndex], "--english"))
        {
            isUsingEnglish = true;
        }
        else if (EQUAL(argv[argIndex], "--direction-of-interest"))
        {
            argumentName = "direction of interest";
            directionOfInterest = parseDouble(argIndex, MAX_ARGUMENT_INDEX, argv, argumentName);
            checkAngleBound(directionOfInterest);
            hasDirectionOfInterest = true;
        }
        else if (EQUAL(argv[argIndex], "--output-to-file"))
        {
            isOutputtingToFile = true;
        }
        else if (EQUAL(argv[argIndex], "--file-name"))
        {
            if ((argIndex + 1) > MAX_ARGUMENT_INDEX) // An error has occurred
            {
                // Report error
                printf("ERROR: No file name entered\n");
                Usage(); // Exits program
            }
            hasSpecifiedFileName = true;
            fileName = argv[++argIndex];
            if (!(fileName.substr(fileName.find_last_of(".") + 1) == "txt")) // Output is not yet a .txt file
            {
                // Give the file a .txt extension
                fileName += ".txt";
            }
        }
        else if (EQUAL(argv[argIndex], "--append"))
        {
            isAppending = true;
        }
        else if (EQUAL(argv[argIndex], "--run-identifier"))
        {
            if ((argIndex + 1) > MAX_ARGUMENT_INDEX) // An error has occurred
            {
                // Report error
                printf("ERROR: No run identifier entered\n");
                Usage(); // Exits program
            }
            runIdentifier = argv[++argIndex];
            // in case user somehow feeds in a whitespace in the argument
            std::replace(runIdentifier.begin(), runIdentifier.end(), ' ', '-');
        }
        argIndex++;
    }

    // Check for required arguments
    for (int i = 0; i < REQUIRED_ARG_COUNT; i++)
    {
        if (requiredArgumentArray[i] == false) // An error has occurred
        {
            // Report error
            std::string errorString = "ERROR: Missing required argument ";
            errorString += getArgumentName(i);
            printf("%s\n", errorString.c_str());
            isMissingRequirement = true;
        }
    }
    if (isMissingRequirement) // An error has occurred
    {
        Usage(); // Exits program
    }


    // Check for logical consistency of parameters
    // Check for using metric XOR english input and output
    if (isUsingEnglish && isUsingMetric)
    {
        // Report error
        printf("ERROR: Cannot use both English and Metric units\n");
        Usage(); // Exits program
    }
    // Check for output to file and related parameters
    if (!isOutputtingToFile && hasSpecifiedFileName) // An error has occurred
    {
        // Report error
        printf("ERROR: Must be printing a file to specify a file name\n");
        Usage(); // Exits program
    }
    if (!isOutputtingToFile && isAppending) // An error has occurred
    {
        // Report error
        printf("ERROR: Must be printing a file to append to a file\n");
        Usage();  // Exits program
    }

    // Convert input units if necessary
    if (isUsingMetric)
    {
        windSpeedUnits = SpeedUnits::METERS_PER_SECOND;
        canopyHeightUnits = LengthUnits::METERS;
    }

    // Feed input values to behave
    behave.updateSurfaceInputs(fuelModelNumber, moistureOneHr, moistureTenHr, moistureHundredHr, moistureLiveHerb, moistureLiveWoody, MoistureUnits::PERCENT,
        windSpeed, windSpeedUnits, WindHeightInputMode::DIRECT_MIDFLAME, windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, canopyHeight, canopyHeightUnits, crownRatio);

    // Do the spread rate calculation
    if (hasDirectionOfInterest)
    {
        behave.doSurfaceRunInDirectionOfInterest(directionOfInterest);
       
    }
    else
    {
        behave.doSurfaceRunInDirectionOfMaxSpread();
    }

    // Get the surface fire spread rate
    spreadRate = behave.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR);

    // Get other required outputs
    flameLength = behave.getSurfaceFlameLength(LengthUnits::FEET);
    directionOfMaxSpread = behave.getDirectionOfMaxSpread();

    // Convert output units if necessary
    if (isUsingMetric)
    {
        spreadRate *= CHAINS_PER_HOUSR_TO_METERS_PER_SECOND;
        flameLength *= FEET_TO_METERS;
    }

    // Handle file output
    if (isOutputtingToFile)
    {
        FILE *fout;
        size_t size;
        std::string firstToken = "";
        std::string secondToken = "";
        std::string thirdToken = "";

        if (isAppending)
        {
            // Used a+ mode, otherwise file pointer for reading starts at first free block,
            // this way reading can start at first block and writing at first free block
            fout = fopen(fileName.c_str(), "a+t");

            if (!fout)
            {
                fprintf(stderr, "ERROR: Cannot open file `%s`\n", fileName.c_str());
                exit(1); // Exit with error code 1
            }

            // Scope of temporary c-strings
            {
                char firstTokenCString[128];
                char secondTokenCString[128];
                char thirdTokenCString[128];

                // Read first three tokens
                fscanf(fout, "%s", firstTokenCString);
                fscanf(fout, "%s", secondTokenCString);
                fscanf(fout, "%s", thirdTokenCString);

                // Copy c-strings to std::strings
                firstToken = firstTokenCString;
                secondToken = secondTokenCString;
                thirdToken = thirdTokenCString;
            }

            fseek(fout, 0, SEEK_END);
            size = ftell(fout);
            std::string testString = "run-identifier";
            if ((firstToken.compare(testString) != 0) && (size != 0))
            {
                fprintf(stderr, "ERROR: File format unrecognized, exiting\n");
                exit(1); // Exit with error code 1
            }

            // Test for units mismatch
            testString = "spread-rate(ch/hr)";
            if ((secondToken.compare(testString) == 0) && isUsingMetric)
            {
                fprintf(stderr, "ERROR: Cannot append to a file with mismatched units, exiting\n");
                exit(1); // Exit with error code 1
            }
            testString = "spread-rate(m/s)";
            if ((secondToken.compare(testString) == 0) && !isUsingMetric)
            {
                fprintf(stderr, "ERROR: Cannot append to a file with mismatched units, exiting\n");
                exit(1); // Exit with error code 1
            }
            testString = "spread-rate(ft)";
            if ((thirdToken.compare(testString) == 0) && isUsingMetric)
            {
                fprintf(stderr, "ERROR: Cannot append to a file with mismatched units, exiting\n");
                exit(1); // Exit with error code 1
            }
            testString = "spread-rate(m)";
            if ((thirdToken.compare(testString) == 0) && !isUsingMetric)
            {
                fprintf(stderr, "ERROR: Cannot append to a file with mismatched units, exiting\n");
                exit(1); // Exit with error code 1
            }
        }
        else
        {
            fout = fopen(fileName.c_str(), "wt");
            if (!fout)
            {
                fprintf(stderr, "ERROR: Cannot open file `%s`\n", fileName.c_str());
                exit(1); // Exit with error code 1
            }
        }

        fseek(fout, 0, SEEK_END);
        size = ftell(fout);

        if (size == 0) // File is empty, so print column headers
        {
            std::string spreadRateHeader = "";
            std::string flameLengthHeader = "";
            std::string dirMaxDHeader = "";
            std::string runIDHeader = "run-identifier";
            if (isUsingMetric)
            {
                spreadRateHeader = "spread-rate(m/s)";
                flameLengthHeader = "flame-length(m)";
            }
            else
            {
                spreadRateHeader = "spread-rate(ch/hr)";
                flameLengthHeader = "flame-length(ft)";
            }
            dirMaxDHeader = "direction-of-max-spread(degrees)";
            fprintf(fout, "%-20s\t" \
                "%-20s\t" \
                "%-20s\t" \
                "%-20s\n", runIDHeader.c_str(), spreadRateHeader.c_str(), flameLengthHeader.c_str(), dirMaxDHeader.c_str());
        }

        fprintf(fout, "%-20s\t" \
            "%-20lf\t" \
            "%-20lf\t" \
            "%-20lf\n",
            runIdentifier.c_str(), spreadRate, flameLength, directionOfMaxSpread);
        fclose(fout);

    }

    // Print to console
    printf("run-identifier\t\t\t\t%s\n", runIdentifier.c_str());
    if (isUsingMetric)
    {
        printf("spread-rate(m/s)\t\t\t%lf\n", spreadRate);
        printf("flame-Length(m)\t\t\t\t%lf\n", flameLength);
    }
    else
    {
        printf("spread-rate(ch/hr)\t\t\t%lf\n", spreadRate);
        printf("flame-Length(ft)\t\t\t%lf\n", flameLength);
    }

    printf("direction-of_max-spread(degrees)\t%lf\n", directionOfMaxSpread);

    return 0; // Success
}

