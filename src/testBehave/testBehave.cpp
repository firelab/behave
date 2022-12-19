#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "behaveRun.h"
#include "fuelModels.h"

// Define the error tolerance for double values
constexpr double error_tolerance = 1e-06;

using namespace std;
using ofstream = std::basic_ofstream<char>;

struct TestInfo
{
    int numTotalTests = 0;
    int numFailed = 0;
    int numPassed = 0;
};

ofstream file{ "/home/tdog/Documents/BehavePlusOutput/SB2_SB3/behaveOutput_SB2_SB3_Fuels.csv", ios::out|ios::app };
ofstream moisturefile{ "/home/tdog/Documents/BehavePlusOutput/moistureOutput.csv", ios::out|ios::app };
ofstream fuelmodelfile{ "/home/tdog/Documents/BehavePlusOutput/fuelModelOutput.csv", ios::out|ios::app };

bool areClose(const double observed, const double expected, const double epsilon);

double roundToSixDecimalPlaces(const double numberToBeRounded);

void reportTestResult(struct TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon);

void writeToOutput(BehaveRun& behaveRun, double ROS, string);
bool writeToMoistureOutput( void );
bool addMoisScenario(const string &file,
                           const string &name, const string &desc,
                           double dead1,    double dead10,   double dead100,  double dead1000,
                           double liveHerb, double liveWood);
bool writeToFuelModelOutput( void );

void setSurfaceInputsForGS4LowMoistureScenario(BehaveRun& behaveRun);
void setSurfaceInputsForTwoFuelModelsLowMoistureScenario(BehaveRun& behaveRun);
void setCrownInputsLowMoistureScenario(BehaveRun& behaveRun);

void testSurfaceSingleFuelModel(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testLengthToWidthRatio(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testEllipticalDimensions(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testDirectionOfInterest(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testFirelineIntensity(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testTwoFuelModels(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testCrownModuleRothermel(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testCrownModuleScottAndReinhardt(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testSpotModule(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testSpeedUnitConversion(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testIgniteModule(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testSafetyModule(struct TestInfo& testInfo, BehaveRun& behaveRun);
void testContainModule(struct TestInfo& testInfo, BehaveRun& behaveRun);

void generateSurfaceSingleFuelModelData(struct TestInfo& testInfo, BehaveRun& behaveRun);

int main()
{
    TestInfo testInfo;
    FuelModels fuelModels;
    BehaveRun behaveRun(fuelModels);

//    file << "FuelModel,FuelDescription,MoistureScenario,Dead1hr,Dead10hr,Dead100hr,Herbaceous,Woody,WindSpeed,"
//            "20ftWindSpeed,Slope(%),HeatPerUnitArea(BtusPerSquareFoot),FireLineIntensity(BtusPerFootPerSecond),"
//            "FlameLength(Feet),CanopyCover(%),ROS(ChainsPerHour)" << endl;

    file << "FuelModel,FuelDescription,MoistureScenario,Dead1hr,Dead10hr,Dead100hr,Herbaceous,Woody,WindSpeed,"
            "20ftWindSpeed,Slope(%),HeatPerUnitArea(BtusPerSquareFoot),FireLineIntensity(BtusPerFootPerSecond),"
            "FL,CC,ROS(ChainsPerHour),WeightedMoistureDead,WeightedMoistureLive,MoEDead, MoELive,"
            "etaMDead,etaMLive,ReactionIntensity,NoWindNoSlopeSpreadRate" << endl;

    moisturefile <<"Name|Description|Dead1hr|Dead10hr|Dead100hr|Dead1000hr|LiveHerbaceous|LiveWoody" << endl;
    fuelmodelfile << "FuelModelNumber|Code|Name|FuelBedDepth|MoEDead|HeatOfCombustionDead|HeatOfCombustionLive|"
                  << "FuelLoad1Hr|FuelLoad10Hr|FuelLoad100Hr|"
                  << "FuelLoadHerbaceous|FuelLoadWoody|SAVR1Hr|SAVRHerbaceous|SAVRWoody|Dynamic|Reserved" << endl;

    generateSurfaceSingleFuelModelData(testInfo, behaveRun);
    writeToMoistureOutput();
    writeToFuelModelOutput();

    testSurfaceSingleFuelModel(testInfo, behaveRun);
    testLengthToWidthRatio(testInfo, behaveRun);
    testEllipticalDimensions(testInfo, behaveRun);
    testDirectionOfInterest(testInfo, behaveRun);
    testFirelineIntensity(testInfo, behaveRun);
    testTwoFuelModels(testInfo, behaveRun);
    testCrownModuleRothermel(testInfo, behaveRun);
    testCrownModuleScottAndReinhardt(testInfo, behaveRun);
    testSpotModule(testInfo, behaveRun);
    testSpeedUnitConversion(testInfo, behaveRun);
    testIgniteModule(testInfo, behaveRun);
    testSafetyModule(testInfo, behaveRun);
    testContainModule(testInfo, behaveRun);

    std::cout << "Total tests performed: " << testInfo.numTotalTests << "\n";
    std::cout << "Total tests passed: " << testInfo.numPassed << "\n";
    std::cout << "Total tests failed: " << testInfo.numFailed << "\n\n";

#ifndef NDEBUG
    // Make Visual Studio wait while in debug mode
    std::cout << "Press Enter to continue . . .";
    std::cin.get();
#endif
    return 0;
}

void setSurfaceInputsForGS4LowMoistureScenario(BehaveRun& behaveRun)
{
    //Low Moisture Inputs 
    // 5 mph twenty ft wind, 30% slope, 50% canopy cover and crown ratio, 30 ft canopy cover
    int fuelModelNumber = 124; // fuel model gs4(124) chosen as it is dynamic and has values for all moisture classes
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
}

void setSurfaceInputsForTwoFuelModelsLowMoistureScenario(BehaveRun& behaveRun)
{
    int firstFuelModelNumber = 1; // fuel model 1 chosen arbitrarily
    int secondFuelModelNumber = 124; // fuel model gs4(124) chosen as it is dynamic and has values for all moisture classes
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod = TwoFuelModelsMethod::TwoDimensional;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double firstFuelModelCoverage = 0;
    CoverUnits::CoverUnitsEnum firstFuelModelCoverageUnits = CoverUnits::Percent;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, 
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope, 
        slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
}

void setCrownInputsLowMoistureScenario(BehaveRun& behaveRun)
{
    //Low Moisture Inputs 
    // 5 mph twenty ft wind, 30% slope, 50% canopy cover and crown ratio, 30 ft canopy cover
    int fuelModelNumber = 124; // fuel model gs4(124) chosen as it is dynamic and has values for all moisture classes
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    double moistureFoliar = 120;
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    CoverUnits::CoverUnitsEnum coverUnits = CoverUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBaseHeight = 6;
    double canopyBulkDensity = 0.03;
    DensityUnits::DensityUnitsEnum canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, 
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, canopyBulkDensity, canopyBulkDensityUnits);
}

bool areClose(const double observed, const double expected, const double epsilon)
{
    return fabs(observed - expected) < epsilon;
}

double roundToSixDecimalPlaces(const double numberToBeRounded)
{
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(6); // set to 6 places after decimal
    ss << numberToBeRounded; // put number to be rounded into the stringstream
    std::string s = ss.str(); // convert stringstream to string
    double roundedValue = stod(s); // convert string to double
    return roundedValue;
}

void reportTestResult(struct TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon)
{
    testInfo.numTotalTests++;
    if(areClose(observed, expected, epsilon))
    {
        //std::cout << testName << "\npassed successfully\n";
        testInfo.numPassed++;
    }
    else
    {
        std::cout << testName << "\nfailed\nobserved value " << observed << " differs from expected value " << expected << " by more than " << epsilon << "\n";
        testInfo.numFailed++;
    }
}

void generateSurfaceSingleFuelModelData(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    FuelModels _fuelModels;
    string testName = "";
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;
    double windSpeedAddition = 0.0;
//    const int fuelModelNumbers[]{121, 122, 123, 124, 142, 162, 182};
    const int fuelModelNumbers[]{202, 203, 171, 172, 173, 174, 175, 176, 177, 178, 179};
//    const int fuelModelNumbers[]{181, 182, 183, 184, 185, 186, 187, 188, 189};
    const double canopyCover[]{0, 20, 40, 60, 80};
    const char label[]{'A', 'B', 'C', 'D', 'E', 'F',
                       'G', 'H', 'I', 'J', 'K', 'L',
                       'M', 'N', 'O', 'P', 'Q', 'R',
                       'S', 'T', 'U', 'V', 'W','X', 'Y', 'Z'};
    CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
    Surface surfaceObj = behaveRun.surface;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;

    std::cout << "Testing Surface, single fuel model\n";
    testName = "Test north oriented mode, 45 degree wind, 5 mph 20 foot wind, 30 degree slope";
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    for (const auto element : canopyCover)
    {
        behaveRun.surface.setCanopyCover(element, canopyCoverUnits);

        // Vary wind speeds by 1 mph increments
        for (int i = 0; i < 31; i++)
        {
            windSpeedAddition = (double) i * 1;
            int idx = 0;

            behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
            behaveRun.surface.setSlope(0, SlopeUnits::Degrees);
            behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
            behaveRun.surface.setWindSpeed(5 + windSpeedAddition, windSpeedUnits, windHeightInputMode);
            behaveRun.surface.setWindDirection(0);
            behaveRun.surface.setAspect(95);
            behaveRun.surface.getFuelLoadOneHour(123, LoadingUnits::PoundsPerSquareFoot);

            for (int j = 0; j < 31; j++)
            {
                behaveRun.surface.setMoistureOneHour(2.0 + (double) j * 0.5,  MoistureUnits::Percent);
                behaveRun.surface.setMoistureTenHour(3.0 + (double) j * 0.5,  MoistureUnits::Percent);
                behaveRun.surface.setMoistureHundredHour(4.0 + (double) j * 0.5,  MoistureUnits::Percent);
                behaveRun.surface.setMoistureLiveHerbaceous(20.0 + (double) j * 5,  MoistureUnits::Percent);
                behaveRun.surface.setMoistureLiveWoody(50.0 + (double) j * 5,  MoistureUnits::Percent);

//                string moistureScenario = "\"MS" + to_string(idx) + " ("
//                                          + to_string((int)behaveRun.surface.getMoistureOneHour(MoistureUnits::Percent)) + "-"
//                                          + to_string((int)behaveRun.surface.getMoistureTenHour(MoistureUnits::Percent)) + "-"
//                                          + to_string((int)behaveRun.surface.getMoistureHundredHour(MoistureUnits::Percent)) + "-"
//                                          + to_string((int)behaveRun.surface.getMoistureLiveHerbaceous(MoistureUnits::Percent)) + "-"
//                                          + to_string((int)behaveRun.surface.getMoistureLiveWoody(MoistureUnits::Percent))
//                                          + ")\"";

                string moistureScenario = to_string(idx);
                idx++;
//                for (const auto fuelModelNumber : _fuelModels.FuelModelArray_)
//                for (const auto fuelModelNumber : _fuelModels.FuelModelArray_)
//                {
//                    if ( fuelModelNumber.fuelModelNumber_ != 0 )
//                    {
//                        behaveRun.surface.setFuelModelNumber(fuelModelNumber.fuelModelNumber_);
//                        behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
//                        observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
//                        writeToOutput(behaveRun, observedSurfaceFireSpreadRate, moistureScenario);
//                    }
//                }
                for (const auto fuelModelNumber : fuelModelNumbers)
                {
                    behaveRun.surface.setFuelModelNumber(fuelModelNumber);
                    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
                    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
                    writeToOutput(behaveRun, observedSurfaceFireSpreadRate, moistureScenario);
                }
            }

//            for (int l=0; l < 10; l++)
//            {
//
//            }

        }
    }


    std::cout << "Finished generating Surface, single fuel model data \n\n";
}

void testSurfaceSingleFuelModel(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    string testName = "";
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;

    std::cout << "Testing Surface, single fuel model\n";
    testName = "Test north oriented mode, 45 degree wind, 5 mph 20 foot wind, 30 degree slope";
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.setAspect(95);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 19.677584;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test upslope oriented mode, 5 mph 20 foot uplsope wind";
    behaveRun.surface.setFuelModelNumber(124);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setSlope(30, SlopeUnits::Percent);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test upslope oriented mode, 5 mph 20 foot wind cross-slope left to right (90 degrees)";
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(90);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 7.091665;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test north oriented mode, 20 foot North wind, zero aspect";
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test north oriented mode, 20 foot north-east wind (45 degree), 215 degree aspect";
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setAspect(215);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 4.113265;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test north oriented mode, 20 foot 45 degree wind, 95 degree aspect";
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setAspect(5);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.503960;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test Fuel Model 4, north oriented mode, 20 foot 90 degree wind, 0 degree aspect, 40 percent canopy cover";
    behaveRun.surface.setFuelModelNumber(4);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.setWindDirection(90);
    behaveRun.surface.setWindSpeed(5, SpeedUnits::MilesPerHour, WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setCanopyCover(40, CoverUnits::Percent);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 46.631688;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test Non-Burnable Fuel";
    behaveRun.surface.setFuelModelNumber(91);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 0.0;
    reportTestResult(testInfo, "Test Non-Burnable Fuel", observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);
    std::cout << "Finished testing Surface, single fuel model\n\n";
}

void testLengthToWidthRatio(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing length-to-width-ratio\n";
    string testName = "";
    // Observed and expected output
    double observedLengthToWidthRatio = 0.0;
    double expectedLengthToWidthRatio = 0.0;

    double observedCrownLengthToWidthRatio = 0;
    double expectedCrownLengthToWidthRatio = 0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    testName = "Test length-to-width-ratio, north oriented mode, north wind, 0 mph 20 foot wind, 0 degree aspect, 0 degree slope";
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(0, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(0, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 1.0;
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    testName = "Test length-to-width-ratio, north oriented mode, 45 degree wind, 5 mph 20 foot wind, 95 degree aspect, 30 degree slope";
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.setAspect(95);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 1.897769;
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    testName = "Test length-to-width-ratio, north oriented mode, 45 degree wind, 15 mph 20 foot wind, 95 degree aspect, 30 degree slope";
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(15, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.setAspect(95);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 2.142422;
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    testName = "Test crown fire length-to-width-ratio, 0 mph 20 foot wind";
    behaveRun.crown.setWindSpeed(0, windSpeedUnits, windHeightInputMode);
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownLengthToWidthRatio = 1.625;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    testName = "Test crown fire length-to-width-ratio, 10 mph 20 foot wind";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownLengthToWidthRatio = 2.25;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    testName = "Test crown fire length-to-width-ratio, 15 mph 20 foot wind";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(15, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownLengthToWidthRatio = 2.875;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    std::cout << "Finished testing length-to-width-ratio\n\n";
}

void testEllipticalDimensions(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing elliptical dimensions\n";
    string testName = "";

    double observedA = 0;
    double observedB = 0;
    double observedC = 0;
    double expectedA = 7.4618580;
    double expectedB = 17.824253;
    double expectedC = 16.187176;

    double elapsedTime = 1;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Chains;
   
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::DirectMidflame);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

    testName = "Test fire elliptical dimension a with direct mid-flame, upslope mode, 1 hour elapsed time";
    observedA = roundToSixDecimalPlaces(behaveRun.surface.getEllipticalA(lengthUnits, elapsedTime, TimeUnits::Hours));
    reportTestResult(testInfo, testName, observedA, expectedA, error_tolerance);
   
    testName = "Test fire elliptical dimension b with direct mid-flame, upslope mode, 1 hour elapsed time";
    observedB = roundToSixDecimalPlaces(behaveRun.surface.getEllipticalB(lengthUnits, elapsedTime, TimeUnits::Hours));
    reportTestResult(testInfo, testName, observedB, expectedB, error_tolerance);

    testName = "Test fire elliptical dimension c with direct mid-flame, upslope mode, 1 hour elapsed time";
    observedC = roundToSixDecimalPlaces(observedC = behaveRun.surface.getEllipticalC(lengthUnits, elapsedTime, TimeUnits::Hours));
    reportTestResult(testInfo, testName, observedC, expectedC, error_tolerance);

    // Area
    double expectedArea = 0.0;
    double observedArea = 0.0;

    expectedArea = 41.783821;
    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::Acres, elapsedTime, TimeUnits::Hours));
    testName = "Test fire elliptical area in acres with direct mid-flame, upslope mode, 1 hour elapsed time";
    reportTestResult(testInfo, testName, observedArea, expectedArea, error_tolerance);

    expectedArea = 0.169093;
    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::SquareKilometers, elapsedTime, TimeUnits::Hours));
    testName = "Test fire elliptical area in km^2 with direct mid-flame, upslope mode, 1 hour elapsed time";
    reportTestResult(testInfo, testName, observedArea, expectedArea, error_tolerance);

    // Perimeter
    double expectedPerimeter = 0.0;
    double obeservedPerimeter = 0.0;

    expectedPerimeter = 82.808915;
    obeservedPerimeter = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFirePerimeter(LengthUnits::Chains, elapsedTime, TimeUnits::Hours));
    testName = "Test fire elliptical perimeter in chains with direct mid-flame, upslope mode, 1 hour elapsed time";
    reportTestResult(testInfo, testName, obeservedPerimeter, expectedPerimeter, error_tolerance);

    std::cout << "Finished testing elliptical dimensions\n\n";
}

void testDirectionOfInterest(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing spread rate in direction of interest\n";
    string testName = "";
    double directionOfInterest = 0;
    double observedSpreadRateInDirectionOfInterest = 0.0;
    double expectedSpreadRateInDirectionOfInterest = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
  
    testName = "Test upslope oriented mode, 20 foot wind, direction of interest 90 degrees from upslope, 45 degree wind";
    directionOfInterest = 90;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 3.016440;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test upslope oriented mode, 20 foot wind, direction of interest 160 degrees from upslope, 290 degree wind";
    directionOfInterest = 160;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(290);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.399262;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test upslope oriented mode, 20 foot wind, direction of interest 215 degrees from upslope, 215 degree wind";
    directionOfInterest = 215;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(215);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.648579;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test north oriented mode, 20 foot 135 degree wind, direction of interest 30 degrees from north, 263 degree aspect";
    directionOfInterest = 30;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(280);
    behaveRun.surface.setAspect(135);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 3.504961;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test north oriented mode, 20 foot north wind, direction of interest 90 degrees from north, 45 degree aspect";
    directionOfInterest = 90;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(45);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.803660;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test north oriented mode, 20 foot 135 degree wind, direction of interest 285 degrees from north, 263 degree aspect";
    directionOfInterest = 285;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(280);
    behaveRun.surface.setAspect(263);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.452856;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    std::cout << "Finished testing spread rate in direction of interest\n\n";
}

void testFirelineIntensity(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing fireline instensity\n";
    string testName = "";
    double observedFirelineIntensity = 0.0;
    double expectedFirelineIntensity = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    testName = "Test upslope oriented mode, 20 foot uplsope wind";
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedFirelineIntensity = behaveRun.surface.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond);
    expectedFirelineIntensity = 598.339039;
    reportTestResult(testInfo, testName, observedFirelineIntensity, expectedFirelineIntensity, error_tolerance);
   
    // Test unit conversion
    observedFirelineIntensity = behaveRun.surface.getFirelineIntensity(FirelineIntensityUnits::KilowattsPerMeter);
    expectedFirelineIntensity = 2072.730450;
    reportTestResult(testInfo, testName, observedFirelineIntensity, expectedFirelineIntensity, error_tolerance);

    std::cout << "Finished testing fireline instensity\n\n";
}

void testTwoFuelModels(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Two Fuel Models, first fuel model 1, second fuel model 124\n";
    string testName = "";
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    // Test upslope oriented mode, 20 foot wind, low moisture conditions
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    setSurfaceInputsForTwoFuelModelsLowMoistureScenario(behaveRun);

    CoverUnits::CoverUnitsEnum coverUnits = CoverUnits::Percent;

    testName = "First fuel model coverage 0";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(0, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 10";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(10, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 10.446373;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 20";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(20, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 12.112509;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 30";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(30, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 13.825904;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 40";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(40, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 15.532700;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 50";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(50, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 17.173897;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 60";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(60, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 18.685358;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 70";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(70, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 19.997806;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 80";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(80, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.036826;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 90";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(90, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.722861;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 100";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(100, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.971217;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    std::cout << "Finished testing Two Fuel Models, first fuel model 1, second fuel model 124\n\n";
}

void testCrownModuleRothermel(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Crown module, Rothermel\n";
    string testName = "";
    double canopyHeight = 30;
    double canopyBaseHeight = 6;
    double canopyBulkDensity = 0.03;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    double observedCrownFireSpreadRate = 0;
    double expectedCrownFireSpreadRate = 0;
    double observedCrownFlameLength = 0;
    double expectedCrownFlameLength = 0;
    double observedCrownFirelineIntensity = 0;
    double expectedCrownFirelineIntensity = 0;
    int expectedFireType = (int)FireType::Surface;
    int observedFireType = (int)FireType::Surface;

    behaveRun.crown.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);

    testName = "Test crown fire spread rate";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownFireSpreadRate = 10.259921;
    observedCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireSpreadRate(SpeedUnits::ChainsPerHour));
    reportTestResult(testInfo, testName, observedCrownFireSpreadRate, expectedCrownFireSpreadRate, error_tolerance);

    testName = "Test crown fire flame length";
    expectedCrownFlameLength = 29.320557;
    observedCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getCrownFlameLength(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedCrownFlameLength, expectedCrownFlameLength, error_tolerance);

    testName = "Test crown fireline intensity";
    expectedCrownFirelineIntensity = 1775.061222;
    observedCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getCrownFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond));
    reportTestResult(testInfo, testName, observedCrownFirelineIntensity, expectedCrownFirelineIntensity, error_tolerance);

    testName = "Test fire type, Surface fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setMoistureOneHour(20, MoistureUnits::Percent);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Surface;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    testName = "Test fire type, Torching fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Torching;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    testName = "Test fire type, Crowning fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Crowning;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    testName = "Test fire type, Conditional crown fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    canopyHeight = 60;
    behaveRun.crown.setCanopyHeight(canopyHeight, LengthUnits::Feet);
    canopyBaseHeight = 30;
    behaveRun.crown.setCanopyBaseHeight(canopyBaseHeight, LengthUnits::Feet);
    canopyBulkDensity = 0.06;
    behaveRun.crown.setCanopyBulkDensity(0.06, DensityUnits::PoundsPerCubicFoot);
    behaveRun.crown.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::ConditionalCrownFire;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    std::cout << "Finished testing Crown module, Rothermel\n\n";
}

void testCrownModuleScottAndReinhardt(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Crown module, Scott and Reinhardt\n";
    string testName = "";

    double observedFinalCrownFireSpreadRate = 0;
    double expectedFinalCrownFireSpreadRate = 0;
    double observedFinalCrownFlameLength = 0;
    double expectedFinalCrownFlameLength = 0;
    double observedFinalCrownFirelineIntensity = 0;
    double expectedFinalCrownFirelineIntensity = 0;
    double observedCriticalOpenWindSpeed = 0;
    double expectedCriticalOpenWindSpeed = 0;
    int expectedFireType = (int)FireType::Surface;
    int observedFireType = (int)FireType::Surface;

    int fuelModelNumber = 10;
    double moistureOneHour = 8.0;
    double moistureTenHour = 9.0;
    double moistureHundredHour = 10.0;
    double moistureLiveHerbaceous = 0.0;
    double moistureLiveWoody = 117.0;
    double moistureFoliar = 100;
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    double windSpeed = 2187.226624;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::FeetPerMinute;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    double slope = 20;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    CoverUnits::CoverUnitsEnum coverUnits = CoverUnits::Percent;
    double canopyHeight = 38.104626;
    double canopyBaseHeight = 2.952756;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBulkDensity = 0.01311;
    DensityUnits::DensityUnitsEnum canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.crown.setUserProvidedWindAdjustmentFactor(0.12);
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, canopyBulkDensity, canopyBulkDensityUnits);

    testName = "Test crown fire spread rate";
    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 65.221842;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::FeetPerMinute));
    reportTestResult(testInfo, testName, observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, error_tolerance);

    testName = "Test crown flame length";
    expectedFinalCrownFlameLength = 60.744542;
    observedFinalCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getFinalFlameLength(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedFinalCrownFlameLength, expectedFinalCrownFlameLength, error_tolerance);

    testName = "Test crown fireline intensity";
    expectedFinalCrownFirelineIntensity = 5293.170672;
    observedFinalCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getFinalFirelineIntesity(FirelineIntensityUnits::BtusPerFootPerSecond));
    reportTestResult(testInfo, testName, observedFinalCrownFirelineIntensity, expectedFinalCrownFirelineIntensity, error_tolerance);

    testName = "Test crown fire critical open wind speed";
    expectedCriticalOpenWindSpeed = 1717.916785;
    observedCriticalOpenWindSpeed = behaveRun.crown.getCriticalOpenWindSpeed(SpeedUnits::FeetPerMinute);
    reportTestResult(testInfo, testName, observedCriticalOpenWindSpeed, expectedCriticalOpenWindSpeed, error_tolerance);

    testName = "Test crown fire type, crown fire expected";
    expectedFireType = (int)FireType::Crowning;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, expectedFireType, observedFireType, error_tolerance);

    // Test crown fire spread rate, flame length, intensity; Torching fire expected
    fuelModelNumber = 5;
    moistureOneHour = 5.0;
    moistureTenHour = 6.0;
    moistureHundredHour = 8.0;
    moistureLiveHerbaceous = 0.0;
    moistureLiveWoody = 117.0;
    moistureFoliar = 100;
    moistureUnits = MoistureUnits::Percent;
    windSpeed = 24.854848;
    windHeightInputMode = WindHeightInputMode::TwentyFoot;
    windSpeedUnits = SpeedUnits::MilesPerHour;
    windDirection = 0;
    windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    slope = 20;
    slopeUnits = SlopeUnits::Percent;
    aspect = 0;
    canopyCover = 50;
    coverUnits = CoverUnits::Percent;
    canopyHeight = 71.631562;
    canopyBaseHeight = 4.92126;
    canopyHeightUnits = LengthUnits::Feet;
    crownRatio = 0.50;
    canopyBulkDensity = 0.003746;
    canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.crown.setUserProvidedWindAdjustmentFactor(0.15);
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, canopyBulkDensity, canopyBulkDensityUnits);

    testName = "Test crown fire spread rate";
    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 29.47538799999999;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::FeetPerMinute));
    reportTestResult(testInfo, testName, observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, error_tolerance);

    testName = "Test crown flame length";
    expectedFinalCrownFlameLength = 12.759447;
    observedFinalCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getFinalFlameLength(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedFinalCrownFlameLength, expectedFinalCrownFlameLength, error_tolerance);

    testName = "Test crown fireline intensity";
    expectedFinalCrownFirelineIntensity = 509.5687530;
    observedFinalCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getFinalFirelineIntesity(FirelineIntensityUnits::BtusPerFootPerSecond));
    reportTestResult(testInfo, testName, observedFinalCrownFirelineIntensity, expectedFinalCrownFirelineIntensity, error_tolerance);

    testName = "Test crown fire critical open wind speed";
    expectedCriticalOpenWindSpeed = 3874.421988;
    observedCriticalOpenWindSpeed = behaveRun.crown.getCriticalOpenWindSpeed(SpeedUnits::FeetPerMinute);
    reportTestResult(testInfo, testName, observedCriticalOpenWindSpeed, expectedCriticalOpenWindSpeed, error_tolerance);

    testName = "Test crown fire type, torching fire expected expected";
    expectedFireType = (int)FireType::Torching;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, expectedFireType, observedFireType, error_tolerance);

    behaveRun.surface.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UseCrownRatio);

    std::cout << "Finished testing Crown module, Scott and Reinhardt\n\n";
}

void testSpotModule(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Spot module\n";

    string testName = "";

    double flameLength = 0.0;
    double expectedFlatSpottingDistance = 0.0;
    double expectedMountainSpottingDistance = 0.0;
    double observedMountainSpottingDistance = 0.0;
    double observedFlatSpottingDistance = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    flameLength = behaveRun.surface.getFlameLength(LengthUnits::Feet);

    SpotFireLocation::SpotFireLocationEnum location = SpotFireLocation::RIDGE_TOP;
    double ridgeToValleyDistance = 1.0;
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits = LengthUnits::Miles;
    double ridgeToValleyElevation = 2000.0;
    LengthUnits::LengthUnitsEnum elevationUnits = LengthUnits::Feet;
    double downwindCoverHeight = 30.0;
    LengthUnits::LengthUnitsEnum coverHeightUnits = LengthUnits::Feet;
    double burningPileflameHeight = 5.0;
    LengthUnits::LengthUnitsEnum flameHeightUnits = LengthUnits::Feet;
    double windSpeedAtTwentyFeet = 5.0;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    int torchingTrees = 15;
    double DBH = 20.0;
    LengthUnits::LengthUnitsEnum DBHUnits = LengthUnits::Inches;
    double treeHeight = 30.0;
    LengthUnits::LengthUnitsEnum treeHeightUnits = LengthUnits::Feet;
    SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies = SpotTreeSpecies::ENGELMANN_SPRUCE;
    LengthUnits::LengthUnitsEnum spottingDistanceUnits = LengthUnits::Miles;;

    behaveRun.spot.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits,
        burningPileflameHeight, flameHeightUnits, windSpeedAtTwentyFeet, windSpeedUnits);
    behaveRun.spot.calculateSpottingDistanceFromBurningPile();
    
    testName = "Test mountain spotting distance from burning pile";
    expectedMountainSpottingDistance = 0.021330;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);
    
    testName = "Test flat spotting distance from burning pile";
    expectedFlatSpottingDistance = 0.017067;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);

    behaveRun.spot.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, windSpeedAtTwentyFeet,
        windSpeedUnits, flameLength, flameHeightUnits);
    behaveRun.spot.calculateSpottingDistanceFromSurfaceFire();

    testName = "Test mountain spotting distance from surface fire";
    expectedMountainSpottingDistance = 0.164401;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromSurfaceFire(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);
    
    testName = "Test flat spotting distance from surface fire";
    expectedFlatSpottingDistance = 0.132964;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromSurfaceFire(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);
    
    behaveRun.spot.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, torchingTrees, DBH, DBHUnits,
        treeHeight, treeHeightUnits, treeSpecies, windSpeedAtTwentyFeet, windSpeedUnits);
    behaveRun.spot.calculateSpottingDistanceFromTorchingTrees();
    
    testName = "Test mountain spotting distance from torching trees";
    expectedMountainSpottingDistance = 0.222396;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromTorchingTrees(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);
    
    testName = "Test flat spotting distance from from torching trees";
    expectedFlatSpottingDistance = 0.181449;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromTorchingTrees(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);

    std::cout << "Finished testing Spot module\n\n";
}

void testSpeedUnitConversion(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing speed unit conversion\n";

    string testName = "";

    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test using upslope oriented mode, 20 foot uplsope wind
    behaveRun.surface.setFuelModelNumber(124);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setSlope(30, SlopeUnits::Percent);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

    testName = "Test surface spread rate in chains per hour";
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test surface spread rate in feet per minute";
    expectedSurfaceFireSpreadRate = 9.763838;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::FeetPerMinute));
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test surface spread rate in kilometers per hour";
    expectedSurfaceFireSpreadRate = 0.178561;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::KilometersPerHour));
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test surface spread rate in meters per minute";
    expectedSurfaceFireSpreadRate = 2.976018;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::MetersPerMinute));
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test surface spread rate in meters per second";
    expectedSurfaceFireSpreadRate = 0.049600;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::MetersPerSecond));
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test surface spread rate in miles per hour";
    expectedSurfaceFireSpreadRate = 0.110953;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::MilesPerHour));
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    std::cout << "Finished testing speed unit conversion\n\n";
}

void testIgniteModule(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Ignite module\n";

    string testName = "";

    double moistureOneHour = 6.0;
    double moistureHundredHour = 8.0;
    double airTemperature = 80; // Fahrenheit
    double sunShade = 50.0; // Percent 
    IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType = IgnitionFuelBedType::DouglasFirDuff;
    double duffDepth = 6.0; // inches
    LightningCharge::LightningChargeEnum lightningChargeType = LightningCharge::Unknown;

    double observedFirebrandIgnitionProbability = 0;
    double observedLightningIgnitionProbability = 0;
    double expectedFirebrandIgnitionProbability = 0;
    double expectedLightningIgnitionProbability = 0;
   
    MoistureUnits::MoistureUnitsEnum mositureUnits = MoistureUnits::Percent;
    TemperatureUnits::TemperatureUnitsEnum temperatureUnits = TemperatureUnits::Fahrenheit;
    CoverUnits::CoverUnitsEnum sunShadeUnits = CoverUnits::Percent;
    LengthUnits::LengthUnitsEnum duffDepthUnits = LengthUnits::Inches;

    expectedFirebrandIgnitionProbability = 0.54831705;
    expectedLightningIgnitionProbability = 0.39362018;

    behaveRun.ignite.updateIgniteInputs(moistureOneHour, moistureHundredHour, mositureUnits, airTemperature,
        temperatureUnits, sunShade, sunShadeUnits, fuelBedType, duffDepth, duffDepthUnits, lightningChargeType);

    testName = "Test firebrand ignition probability for Douglas fir duff";
    observedFirebrandIgnitionProbability = behaveRun.ignite.calculateFirebrandIgnitionProbability(ProbabilityUnits::Fraction);
    reportTestResult(testInfo, testName, observedFirebrandIgnitionProbability, expectedFirebrandIgnitionProbability, error_tolerance);

    testName = "Test lightning ignition probability for Douglas fir duff";
    observedLightningIgnitionProbability = behaveRun.ignite.calculateLightningIgnitionProbability(ProbabilityUnits::Fraction);
    reportTestResult(testInfo, testName, observedLightningIgnitionProbability, expectedLightningIgnitionProbability, error_tolerance);

    moistureOneHour = 7.0;
    moistureHundredHour = 9.0;
    airTemperature = 90; // Fahrenheit
    sunShade = 25.0; // Percent 
    fuelBedType = IgnitionFuelBedType::LodgepolePineDuff;
    duffDepth = 8.0; // inches
    lightningChargeType = LightningCharge::Negative;

    expectedFirebrandIgnitionProbability = 50.717573;
    expectedLightningIgnitionProbability = 17.931991;

    behaveRun.ignite.updateIgniteInputs(moistureOneHour, moistureHundredHour, mositureUnits, airTemperature,
        temperatureUnits, sunShade, sunShadeUnits, fuelBedType, duffDepth, duffDepthUnits, lightningChargeType);

    testName = "Test firebrand ignition probability for Lodgepole pine duff";
    observedFirebrandIgnitionProbability = behaveRun.ignite.calculateFirebrandIgnitionProbability(ProbabilityUnits::Percent);
    reportTestResult(testInfo, testName, observedFirebrandIgnitionProbability, expectedFirebrandIgnitionProbability, error_tolerance);

    testName = "Test lightning ignition probability for Lodgepole pine duff";
    observedLightningIgnitionProbability = behaveRun.ignite.calculateLightningIgnitionProbability(ProbabilityUnits::Percent);
    reportTestResult(testInfo, testName, observedLightningIgnitionProbability, expectedLightningIgnitionProbability, error_tolerance);

    std::cout << "Finished testing Ignite module\n\n";
}

void testSafetyModule(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Safety module\n";

    string testName = "";

    double flameHeight = 5; // ft
    int numberOfPersonnel = 6;
    int numberOfEquipment = 1;
    double areaPerPerson = 50; // ft^2
    double areaPerEquipment = 300; // ft^2

    LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Feet;
    AreaUnits::AreaUnitsEnum personnelAndEquipmentAreaUnits = AreaUnits::SquareFeet;
    AreaUnits::AreaUnitsEnum safetyZoneAreaUnits = AreaUnits::Acres;

    double expectedSeparationDistance = 0; 
    double expectedSafetyZoneArea = 0;
    double expectedSafetyZoneRadius = 0;

    double observedSeparationDistance = 0;
    double observedSafetyZoneArea = 0;
    double observeSafetyZoneRadius = 0;

    expectedSeparationDistance = 20; // ft
    expectedSafetyZoneArea = 0.082490356; // acres
    expectedSafetyZoneRadius = 33.819766; // ft
    
    behaveRun.safety.updateSafetyInputs(flameHeight, lengthUnits, numberOfPersonnel, numberOfEquipment, areaPerPerson, areaPerEquipment, personnelAndEquipmentAreaUnits);
    behaveRun.safety.calculateSafetyZone();

    observedSeparationDistance = behaveRun.safety.getSeparationDistance(lengthUnits);
    observedSafetyZoneArea = behaveRun.safety.getSafetyZoneArea(safetyZoneAreaUnits);
    observeSafetyZoneRadius = behaveRun.safety.getSafetyZoneRadius(lengthUnits);

    testName = "Test separation distance";
    reportTestResult(testInfo, testName, observedSeparationDistance, expectedSeparationDistance, error_tolerance);

    testName = "Test safety zone area";
    reportTestResult(testInfo, testName, observedSafetyZoneArea, expectedSafetyZoneArea, error_tolerance);

    testName = "Test safety zone radius";
    reportTestResult(testInfo, testName, observeSafetyZoneRadius, expectedSafetyZoneRadius, error_tolerance);

    std::cout << "Finished testing Safety module\n\n";
}

void testContainModule(struct TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Contain module\n";

    string testName = "";

    double observedFinalFireLineLength = 0;
    double observedPerimeterAtInitialAttack = 0;
    double observedPerimeterAtContainment = 0;
    double observedFireSizeAtInitialAttack = 0;
    double observedFinalFireSize = 0;
    double observedFinalContainmentArea = 0;
    double observedFinalTimeSinceReport = 0;
    ContainStatus::ContainStatusEnum observedContainmentStatus = ContainStatus::Unreported;

    double expectedFinalFireLineLength = 0;
    double expectedPerimeterAtInitialAttack = 0;
    double expectedPerimeterAtContainment = 0;
    double expectedFireSizeAtInitialAttack = 0;
    double expectedFinalFireSize = 0;
    double expectedFinalContainmentArea = 0;
    double expectedFinalTimeSinceReport = 0;
    ContainStatus::ContainStatusEnum expectedContainmentStatus = ContainStatus::Unreported;

    // Test case where expected result is containment
    behaveRun.contain.setAttackDistance(0, LengthUnits::Chains);
    behaveRun.contain.setLwRatio(3);
    behaveRun.contain.setReportRate(5, SpeedUnits::ChainsPerHour);
    behaveRun.contain.setReportSize(1, AreaUnits::Acres);
    behaveRun.contain.setTactic(ContainTactic::HeadAttack);
    behaveRun.contain.addResource(2, 8, TimeUnits::Hours, 20, SpeedUnits::ChainsPerHour, "test");
    behaveRun.contain.doContainRun();
    //behaveRun.contain.removeAllResources();

    testName = "Test final fire line length";
    expectedFinalFireLineLength = 39.539849615;
    observedFinalFireLineLength = behaveRun.contain.getFinalFireLineLength(LengthUnits::Chains);
    reportTestResult(testInfo, testName, observedFinalFireLineLength, expectedFinalFireLineLength, error_tolerance);

    testName = "Test observed perimeter at initial attack";
    expectedPerimeterAtInitialAttack = 36.694893;
    observedPerimeterAtInitialAttack = behaveRun.contain.getPerimiterAtInitialAttack(LengthUnits::Chains);
    reportTestResult(testInfo, testName, observedPerimeterAtInitialAttack, expectedPerimeterAtInitialAttack, error_tolerance);

    testName = "Test observed perimeter at containment";
    expectedPerimeterAtContainment = 39.539849615;
    observedPerimeterAtContainment = behaveRun.contain.getPerimeterAtContainment(LengthUnits::Chains);
    reportTestResult(testInfo, testName, observedPerimeterAtContainment, expectedPerimeterAtContainment, error_tolerance);

    testName = "Test observed fire size at initial attack";
    expectedFireSizeAtInitialAttack = 7.10569878;
    observedFireSizeAtInitialAttack = behaveRun.contain.getFireSizeAtInitialAttack(AreaUnits::Acres);
    reportTestResult(testInfo, testName, observedFireSizeAtInitialAttack, expectedFireSizeAtInitialAttack, error_tolerance);

    testName = "Test observed final fire size";
    expectedFinalFireSize = 9.42749714;
    observedFinalFireSize = behaveRun.contain.getFinalFireSize(AreaUnits::Acres);
    reportTestResult(testInfo, testName, observedFinalFireSize, expectedFinalFireSize, error_tolerance);

    testName = "Test observed final containment area";
    expectedFinalContainmentArea = 9.42749714;
    observedFinalContainmentArea = behaveRun.contain.getFinalContainmentArea(AreaUnits::Acres);
    reportTestResult(testInfo, testName, observedFinalContainmentArea, expectedFinalContainmentArea, error_tolerance);

    testName = "Test observed final time since report";
    expectedFinalTimeSinceReport = 238.75000000;
    observedFinalTimeSinceReport = behaveRun.contain.getFinalTimeSinceReport(TimeUnits::Minutes);
    reportTestResult(testInfo, testName, observedFinalTimeSinceReport, expectedFinalTimeSinceReport, error_tolerance);

    testName = "Test observed containment status";
    expectedContainmentStatus = ContainStatus::Contained;
    observedContainmentStatus = behaveRun.contain.getContainmentStatus();
    reportTestResult(testInfo, testName, observedContainmentStatus, expectedContainmentStatus, error_tolerance);

    std::cout << "Finished testing Contain module\n\n";
}

void writeToOutput(BehaveRun& behaveRun, double ROS, string moistureScenario)
{
    file << behaveRun.surface.getFuelCode(behaveRun.surface.getFuelModelNumber()) << ","
         << "\"" << behaveRun.surface.getFuelName(behaveRun.surface.getFuelModelNumber()) << "\","
         << moistureScenario << ","
         << behaveRun.surface.getMoistureOneHour(MoistureUnits::MoistureUnitsEnum::Percent) << ","
         << behaveRun.surface.getMoistureTenHour(MoistureUnits::MoistureUnitsEnum::Percent) << ","
         << behaveRun.surface.getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum::Percent) << ","
         << behaveRun.surface.getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum::Percent) << ","
         << behaveRun.surface.getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum::Percent) << ","
         << behaveRun.surface.getWindSpeed(SpeedUnits::SpeedUnitsEnum::MilesPerHour,WindHeightInputMode::WindHeightInputModeEnum::TwentyFoot) << ","
         << behaveRun.surface.getMidflameWindspeed(SpeedUnits::SpeedUnitsEnum::MilesPerHour) << ","
         << behaveRun.surface.getSlope(SlopeUnits::SlopeUnitsEnum::Percent) << ","
         << behaveRun.surface.getHeatPerUnitArea(HeatPerUnitAreaUnits::BtusPerSquareFoot) << ","
         << behaveRun.surface.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond) << ","
         << behaveRun.surface.getFlameLength(LengthUnits::Feet) << ","
         << behaveRun.surface.getCanopyCover(CoverUnits::Percent) << ","
         << ROS << ","
         << behaveRun.surface.surfaceFire_.getWeightedMoisture_Dead() << ","
         << behaveRun.surface.surfaceFire_.getWeightedMoisture_Live() << ","
         << behaveRun.surface.surfaceFire_.getMoE_Dead() << ","
         << behaveRun.surface.surfaceFire_.getMoE_Live() << ","
         << behaveRun.surface.surfaceFire_.getetaMDead() << ","
         << behaveRun.surface.surfaceFire_.getetaMLive() << ","
         << behaveRun.surface.surfaceFire_.getReactionIntensity() << ","
         << behaveRun.surface.surfaceFire_.getNoWindNoSlopeSpreadRate()
         << endl;
}

bool writeToMoistureOutput( void )
{
    if ( ! ( addMoisScenario ( "", "1-low",
                               "TSTMDL (3,4,5,70,70)",
                               0.03, 0.04, 0.05, 0.08, 0.70, 0.70 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "2-med",
                               "TSTMDL (6,7,8,120,120)",
                               0.06, 0.07, 0.08, 0.14, 1.20, 1.20 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "3-high",
                               "TSTMDL (12,13,14,170,170)",
                               0.12, 0.13, 0.14, 0.24, 1.70, 1.70 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d1l1",
                               "Very low dead, fully-cured herb",
                               0.03, 0.04, 0.05, 0.07, 0.30, 0.60 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d1l2",
                               "Very low dead, 2/3-cured herb",
                               0.03, 0.04, 0.05, 0.07, 0.60, 0.90 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d1l3",
                               "Very low dead, 1/3-cured herb",
                               0.03, 0.04, 0.05, 0.07, 0.90, 1.20 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d1l4",
                               "Very low dead, fully-green herb",
                               0.03, 0.04, 0.05, 0.07, 1.20, 1.50 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d2l1",
                               "Low dead, fully-cured herb",
                               0.06, 0.07, 0.08, 0.10, 0.30, 0.60 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d2l2",
                               "Low dead, 2/3-cured herb",
                               0.06, 0.07, 0.08, 0.10, 0.60, 0.90 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d2l3",
                               "Low dead, 1/3-cured herb",
                               0.06, 0.07, 0.08, 0.10, 0.90, 1.20 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d2l4",
                               "Low dead, fully-green herb",
                               0.06, 0.07, 0.08, 0.10, 1.20, 1.50 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d3l1",
                               "Moderate dead, fully-cured herb",
                               0.09, 0.10, 0.11, 0.12, 0.30, 0.60 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d3l2",
                               "Moderate dead, 2/3-cured herb",
                               0.09, 0.10, 0.11, 0.12, 0.60, 0.90 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d3l3",
                               "Moderate dead, 1/3-cured herb",
                               0.09, 0.10, 0.11, 0.12, 0.90, 1.20 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d3l4",
                               "Moderate dead, fully-green herb",
                               0.09, 0.10, 0.11, 0.12, 1.20, 1.50 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d4l1",
                               "High dead, fully-cured herb",
                               0.12, 0.13, 0.14, 0.16, 0.30, 0.60 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d4l2",
                               "High dead, 2/3-cured herb",
                               0.12, 0.13, 0.14, 0.16, 0.60, 0.90 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d4l3",
                               "High dead, 1/3-cured herb",
                               0.12, 0.13, 0.14, 0.16, 0.90, 1.20 ) ) )
        return( false );

    if ( ! ( addMoisScenario ( "", "d4l4",
                               "High dead, fully-green herb",
                               0.12, 0.13, 0.14, 0.16, 1.20, 1.50 ) ) )
        return( false );

    return( true );
}

bool addMoisScenario(const string &file,
                     const string &name, const string &desc,
                     double dead1,    double dead10,   double dead100,  double dead1000,
                     double liveHerb, double liveWood)
{
    moisturefile << name << "| "
            << desc << "| "
            << dead1 << "| "
            << dead10 << "| "
            << dead100 << "| "
            << dead1000 << "| "
            << liveHerb << "| "
            << liveWood << endl;

    return true;
}

bool writeToFuelModelOutput( void )
{
    FuelModels _fuelModels;

    for (const auto fuelModelNumber : _fuelModels.FuelModelArray_)
    {
        if (fuelModelNumber.fuelModelNumber_ != 0 && fuelModelNumber.code_ != "NO CODE")
        {
            fuelmodelfile << fuelModelNumber.fuelModelNumber_ << "| "
                          << fuelModelNumber.code_ << "| "
                          << fuelModelNumber.name_ << "| "
                          << fuelModelNumber.fuelbedDepth_ << "| "
                          << fuelModelNumber.moistureOfExtinctionDead_ << "| "
                          << fuelModelNumber.heatOfCombustionDead_ << "| "
                          << fuelModelNumber.heatOfCombustionLive_ << "| "
                          << fuelModelNumber.fuelLoadOneHour_ << "| "
                          << fuelModelNumber.fuelLoadTenHour_ << "| "
                          << fuelModelNumber.fuelLoadHundredHour_ << "| "
                          << fuelModelNumber.fuelLoadLiveHerbaceous_ << "| "
                          << fuelModelNumber.fuelLoadLiveWoody_ << "| "
                          << fuelModelNumber.savrOneHour_ << "| "
                          << fuelModelNumber.savrLiveHerbaceous_ << "| "
                          << fuelModelNumber.savrLiveWoody_ << "| "
                          << fuelModelNumber.isDynamic_ << "| "
                          << fuelModelNumber.isReserved_ << "| "
                          <<  endl;
        }
    }
}