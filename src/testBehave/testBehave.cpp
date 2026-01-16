#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "behaveRun.h"
#include "fuelModels.h"

// Define the error tolerance for double values
constexpr double error_tolerance = 1e-06;

struct TestInfo
{
    int numTotalTests = 0;
    int numFailed = 0;
    int numPassed = 0;

    const std::string reset_text_color = "\u001b[0m";
    const std::string red_text_color = "\u001b[31m";
    const std::string green_text_color = "\u001b[32m";
};

bool areClose(const double observed, const double expected, const double epsilon);

double roundToSixDecimalPlaces(const double numberToBeRounded);

void reportTestResult(TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon);

void setSurfaceInputsForGS4LowMoistureScenario(BehaveRun& behaveRun);
void setSurfaceInputsForTwoFuelModelsLowMoistureScenario(BehaveRun& behaveRun);
void setCrownInputsLowMoistureScenario(BehaveRun& behaveRun);

void testSurfaceSingleFuelModel(TestInfo& testInfo, BehaveRun& behaveRun);
void testCalculateScorchHeight(TestInfo& testInfo, BehaveRun& behaveRun);
void testChaparral(TestInfo& testInfo, BehaveRun& behaveRun);
void testPalmettoGallberry(TestInfo& testInfo, BehaveRun& behaveRun);
void testWesternAspen(TestInfo& testInfo, BehaveRun& behaveRun);
void testLengthToWidthRatio(TestInfo& testInfo, BehaveRun& behaveRun);
void testEllipticalDimensions(TestInfo& testInfo, BehaveRun& behaveRun);
void testDirectionOfInterest(TestInfo& testInfo, BehaveRun& behaveRun);
void testFirelineIntensity(TestInfo& testInfo, BehaveRun& behaveRun);
void testTwoFuelModels(TestInfo& testInfo, BehaveRun& behaveRun);
void testCrownModuleRothermel(TestInfo& testInfo, BehaveRun& behaveRun);
void testCrownModuleScottAndReinhardt(TestInfo& testInfo, BehaveRun& behaveRun);
void testSpotModule(TestInfo& testInfo, BehaveRun& behaveRun);
void testSpeedUnitConversion(TestInfo& testInfo, BehaveRun& behaveRun);
void testIgniteModule(TestInfo& testInfo, BehaveRun& behaveRun);
void testSafetyModule(TestInfo& testInfo, BehaveRun& behaveRun);
void testContainModule(TestInfo& testInfo, BehaveRun& behaveRun);
void testMortalityModule(TestInfo& testInfo, BehaveRun& behaveRun);
void testFineDeadFuelMoistureTool(TestInfo& testInfo, BehaveRun& behaveRun);
void testSlopeTool(TestInfo& testInfo, BehaveRun& behaveRun);
void testVaporPressureDeficitCalculator(TestInfo& testInfo, BehaveRun& behaveRun);
void testSimpleSurface(TestInfo& testInfo, BehaveRun& behaveRun);

int main()
{
    TestInfo testInfo;
    FuelModels fuelModels;
    SpeciesMasterTable mortalitySpeciesTable;
    MoistureScenarios moistureScenarios;
    BehaveRun behaveRun(fuelModels, mortalitySpeciesTable);
    behaveRun.setMoistureScenarios(moistureScenarios);

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    testSurfaceSingleFuelModel(testInfo, behaveRun);
    testChaparral(testInfo, behaveRun);
    testCalculateScorchHeight(testInfo, behaveRun);
    testPalmettoGallberry(testInfo, behaveRun);
    testWesternAspen(testInfo, behaveRun);
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
    testMortalityModule(testInfo, behaveRun);
    testFineDeadFuelMoistureTool(testInfo, behaveRun);
    testSlopeTool(testInfo, behaveRun);
    testVaporPressureDeficitCalculator(testInfo, behaveRun);
    testSimpleSurface(testInfo, behaveRun);

    std::cout << "Total tests performed: " << testInfo.numTotalTests << "\n";
    if(testInfo.numPassed > 0)
    {
        std::cout << testInfo.green_text_color;
    }
    std::cout << "Total tests passed: " << testInfo.numPassed << "\n";
    if(testInfo.numPassed > 0)
    {
        std::cout << testInfo.reset_text_color;
    }
    if(testInfo.numFailed > 0)
    {
        std::cout << testInfo.red_text_color;
    }
    std::cout << "Total tests failed: " << testInfo.numFailed << "\n\n";
    if(testInfo.numFailed > 0)
    {
        std::cout << testInfo.reset_text_color;

#ifndef DEBUG
        exit(1);
#endif
    }

#ifdef DEBUG
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
    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);
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
    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod = TwoFuelModelsMethod::TwoDimensional;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double firstFuelModelCoverage = 0;
    FractionUnits::FractionUnitsEnum firstFuelModelCoverageUnits = FractionUnits::Percent;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, 
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope, 
                                                          slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);
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
    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    FractionUnits::FractionUnitsEnum coverUnits = FractionUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBaseHeight = 6;
    double canopyBulkDensity = 0.03;
    DensityUnits::DensityUnitsEnum canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, 
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
     crownRatio, FractionUnits::Fraction, canopyBulkDensity, canopyBulkDensityUnits);
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

void reportTestResult(TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon)
{
    testInfo.numTotalTests++;
    if(areClose(observed, expected, epsilon))
    {
        std::cout << testName << "\n" << testInfo.green_text_color << "Passed successfully" << testInfo.reset_text_color << "\n\n";
        testInfo.numPassed++;
    }
    else
    {
        std::cout << testInfo.red_text_color << testName << "\n" << "Failed\nobserved value " << observed << " differs from expected value " << expected << " by more than " << epsilon << testInfo.reset_text_color << "\n\n";
        testInfo.numFailed++;
    }
}

void testSurfaceSingleFuelModel(TestInfo& testInfo, BehaveRun& behaveRun)
{
    string testName = "";
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;

    std::cout << "Testing Surface, single fuel model\n";
    testName = "Test north oriented mode, 45 degree wind, 95 degree aspect, 5 mph 20 foot wind, 30 degree slope";
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

    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;

    testName = "Test live moisture of extinction, 5 mph 20 foot uplsope wind";
    double observedLiveMoistureOfExtinction = roundToSixDecimalPlaces(behaveRun.surface.getLiveFuelMoistureOfExtinction(moistureUnits));
    double expectedLiveMoistureOfExtinction = 137.968551;
    reportTestResult(testInfo, testName, observedLiveMoistureOfExtinction, expectedLiveMoistureOfExtinction, error_tolerance);

    testName = "Test characteristic live moisture, 5 mph 20 foot uplsope wind";
    double observedCharacteristicLiveMoisture = roundToSixDecimalPlaces(behaveRun.surface.getCharacteristicMoistureByLifeState(FuelLifeState::Live, moistureUnits));
    double expectedCharacteristicLiveMoisture = 85.874007;
    reportTestResult(testInfo, testName, observedCharacteristicLiveMoisture, expectedCharacteristicLiveMoisture, error_tolerance);

    testName = "Test characteristic dead moisture, 5 mph 20 foot uplsope wind";
    double observedCharacteristicDeadMoisture = roundToSixDecimalPlaces(behaveRun.surface.getCharacteristicMoistureByLifeState(FuelLifeState::Dead, moistureUnits));
    double expectedCharacteristicDeadMoisture = 6.005463;
    reportTestResult(testInfo, testName, observedCharacteristicDeadMoisture, expectedCharacteristicDeadMoisture, error_tolerance);

    testName = "Test characteristic SAVR for north oriented mode, 45 degree wind, 95 degree aspect, 5 mph 20 foot wind, 30 degree slope";
    double observedCharacteristicSAVR = roundToSixDecimalPlaces(behaveRun.surface.getCharacteristicSAVR(SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet));
    double expectedCharacteristicSAVR = 1631.128734;
    reportTestResult(testInfo, testName, observedCharacteristicSAVR, expectedCharacteristicSAVR, error_tolerance);

    testName = "Test heat source for north oriented mode, 45 degree wind, 95 degree aspect, 5 mph 20 foot wind, 30 degree slope";
    double observedHeatSource = roundToSixDecimalPlaces(behaveRun.surface.getHeatSource(HeatSourceAndReactionIntensityUnits::BtusPerSquareFootPerMinute));
    double expectedHeatSource = 5177.248579;
    reportTestResult(testInfo, testName, observedHeatSource, expectedHeatSource, error_tolerance);

    testName = "Test heat source for north oriented mode, 0 degree wind, 0 degree aspect, 5 mph 20 foot wind, 0 degree slope";
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.setSlope(0, SlopeUnits::Degrees);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedHeatSource = roundToSixDecimalPlaces(behaveRun.surface.getHeatSource(HeatSourceAndReactionIntensityUnits::BtusPerSquareFootPerMinute));
    expectedHeatSource = 1164.267376;
    reportTestResult(testInfo, testName, observedHeatSource, expectedHeatSource, error_tolerance);

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

    testName = "Test backing spreadrate upslope oriented mode, 5 mph 20 foot uplsope wind";
    double observedBackingRate = behaveRun.surface.getBackingSpreadRate(SpeedUnits::ChainsPerHour);
    double expectedBackingRate = 2.91614659;
    reportTestResult(testInfo, testName, observedBackingRate, expectedBackingRate, error_tolerance);

    testName = "Test flanking spreadrate upslope oriented mode, 5 mph 20 foot uplsope wind";
    double observedFlankingRate = behaveRun.surface.getFlankingSpreadRate(SpeedUnits::ChainsPerHour);
    double expectedFlankingRate = 5.08766627;
    reportTestResult(testInfo, testName, observedFlankingRate, expectedFlankingRate, error_tolerance);

    testName = "Test spread distance upslope oriented mode, 5 mph 20 foot uplsope wind";
    double observedSpreadDistance = behaveRun.surface.getSpreadDistance(LengthUnits::Chains, 2.0, TimeUnits::Hours);
    double expectedSpreadDistance = 17.7524327;
    reportTestResult(testInfo, testName, observedSpreadDistance, expectedSpreadDistance, error_tolerance);
    
    testName = "Test backing distance upslope oriented mode, 5 mph 20 foot uplsope wind";
    double observedBackingDistance = behaveRun.surface.getBackingSpreadDistance(LengthUnits::Chains, 2.0, TimeUnits::Hours);
    double expectedBackingDistance = 5.8322932;
    reportTestResult(testInfo, testName, observedBackingDistance, expectedBackingDistance, error_tolerance);

    testName = "Test flanking distance upslope oriented mode, 5 mph 20 foot uplsope wind";
    double observedFlankingDistance = behaveRun.surface.getFlankingSpreadDistance(LengthUnits::Chains, 2.0, TimeUnits::Hours);
    double expectedFlankingDistance = 10.17533253;
    reportTestResult(testInfo, testName, observedFlankingDistance, expectedFlankingDistance, error_tolerance);

    testName = "Test moisture scenario input mode, D1L1 Scenario, 5 mph 20 foot uplsope wind";
    behaveRun.surface.setMoistureInputMode(MoistureInputMode::MoistureScenario);
    std::string moistureScenarioName = "D1L1";
    behaveRun.surface.setCurrentMoistureScenarioByName(moistureScenarioName);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 15.023945;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test moisture scenario input mode, D2L3 Scenario, 5 mph 20 foot uplsope wind";
    moistureScenarioName = "d2L3"; // testing case insensitivity
    behaveRun.surface.setCurrentMoistureScenarioByName(moistureScenarioName);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 1.978840;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test aggregate live and dead moisture input mode, 5 mph 20 foot uplsope wind";
    behaveRun.surface.setMoistureInputMode(MoistureInputMode::AllAggregate);
    behaveRun.surface.setMoistureDeadAggregate(3.0, moistureUnits);
    behaveRun.surface.setMoistureLiveAggregate(80.0, moistureUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.589431;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test check for 1 hour input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    bool observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::OneHour);
    bool expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for 10 hour moisture input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::TenHour);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for 100 hour moisture input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::HundredHour);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for live herb moisture input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::LiveHerbaceous);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for live woody moisture input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::LiveWoody);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for aggregate dead moisture input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::DeadAggregate);
    expectedIsMoistureClassNeeded = true;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for aggregate live moisture input requirment for current fuel and aggregate moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::LiveAggregate);
    expectedIsMoistureClassNeeded = true;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    behaveRun.surface.setMoistureInputMode(MoistureInputMode::DeadAggregateAndLiveSizeClass);

    testName = "Test check for 1 hour input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::OneHour);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for 10 hour moisture input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::TenHour);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for 100 hour moisture input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::HundredHour);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for live herb moisture input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::LiveHerbaceous);
    expectedIsMoistureClassNeeded = true;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for live woody moisture input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::LiveWoody);
    expectedIsMoistureClassNeeded = true;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for aggregate dead moisture input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::DeadAggregate);
    expectedIsMoistureClassNeeded = true;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test check for aggregate live moisture input requirment for current fuel and aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    observedIsMoistureClassNeeded = behaveRun.surface.isMoistureClassInputNeededForCurrentFuelModel(MoistureClassInput::LiveAggregate);
    expectedIsMoistureClassNeeded = false;
    reportTestResult(testInfo, testName, (double)observedIsMoistureClassNeeded, (double)observedIsMoistureClassNeeded, error_tolerance);

    testName = "Test aggregate dead and live size class moisture input mode, 5 mph 20 foot uplsope wind";
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun); // reset moisture
    behaveRun.surface.setMoistureInputMode(MoistureInputMode::DeadAggregateAndLiveSizeClass);
    behaveRun.surface.setMoistureDeadAggregate(3.0, moistureUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 9.752679;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    behaveRun.surface.setMoistureInputMode(MoistureInputMode::BySizeClass);
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun); // reset moisture
    
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
    behaveRun.surface.setCanopyCover(40, FractionUnits::Percent);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 46.631688;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test Non-Burnable Fuel";
    behaveRun.surface.setFuelModelNumber(91);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 0.0;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);
    std::cout << "Finished testing Surface, single fuel model\n\n";
}

void testCalculateScorchHeight(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::string testName = "Test calculate scorch height from 80 F air tempeature, 5 mph wind and 50 Btu/ft/s fireline intensity";
    double firelineInstensity = 50.0;
    double midflameWindspeed = 5.0;
    double airTemperature = 80.0;
    LengthUnits::LengthUnitsEnum scorchLengthUnits = LengthUnits::Feet;
    double observedScorchHeight = behaveRun.mortality.calculateScorchHeight(firelineInstensity, FirelineIntensityUnits::BtusPerFootPerSecond,
        midflameWindspeed, SpeedUnits::MilesPerHour, airTemperature, TemperatureUnits::Fahrenheit, scorchLengthUnits);
    double expectedScorchHeight = 7.617325;
    reportTestResult(testInfo, testName, observedScorchHeight, expectedScorchHeight, error_tolerance);

    testName = "Test calculate scorch height from 70 F air tempeature, 300 ft/min wind and 55 Btu/ft/s fireline intensity";
    midflameWindspeed = 300.0;
    firelineInstensity = 55;
    airTemperature = 70.0;
    observedScorchHeight = behaveRun.mortality.calculateScorchHeight(firelineInstensity, FirelineIntensityUnits::BtusPerFootPerSecond,
        midflameWindspeed, SpeedUnits::FeetPerMinute, airTemperature, TemperatureUnits::Fahrenheit, scorchLengthUnits);
    expectedScorchHeight = 9.923720;
    reportTestResult(testInfo, testName, observedScorchHeight, expectedScorchHeight, error_tolerance);
}

void testChaparral(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::string testName = "";
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    behaveRun.surface.setWindAndSpreadOrientationMode(windAndSpreadOrientationMode);
    behaveRun.surface.setWindDirection(0.0);

    testName = "Test Chaparral spread rate, direct fuel load input mode, depth 1 foot, 25% dead fuel, 0.333 lbs/ft^2, 3 mph wind,  0% slope";
    behaveRun.surface.setIsUsingChaparral(true);
    behaveRun.surface.setChaparralFuelBedDepth(1.0, LengthUnits::Feet);
    behaveRun.surface.setChaparralFuelType(ChaparralFuelType::NotSet);
    behaveRun.surface.setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::DirectFuelLoad);
    behaveRun.surface.setChaparralFuelDeadLoadFraction(0.25);
    behaveRun.surface.setChaparralTotalFuelLoad(0.333, LoadingUnits::PoundsPerSquareFoot);
    behaveRun.surface.setWindSpeed(3.0, SpeedUnits::MilesPerHour, WindHeightInputMode::DirectMidflame);
    behaveRun.surface.setSlope(0.0, SlopeUnits::Percent);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 1.792546;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);
    
    testName = "Test Chaparral spread rate, fuel load from type chamise, depth 2 foot, 33% dead fuel, 4 mph wind, 10% slope";
    behaveRun.surface.setIsUsingChaparral(true);
    behaveRun.surface.setChaparralFuelBedDepth(2.0, LengthUnits::Feet);
    behaveRun.surface.setChaparralFuelType(ChaparralFuelType::Chamise);
    behaveRun.surface.setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::FuelLoadFromDepthAndChaparralType);
    behaveRun.surface.setChaparralFuelDeadLoadFraction(0.33);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::DirectMidflame);
    behaveRun.surface.setWindSpeed(4.0, SpeedUnits::MilesPerHour, WindHeightInputMode::DirectMidflame);
    behaveRun.surface.setSlope(10.0, SlopeUnits::Percent);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 6.108717;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "Test Chaparral spread rate, fuel load from type mixed brush, depth 3 foot, 50% dead fuel,5 mph wind, 20% slope";
    behaveRun.surface.setIsUsingChaparral(true);
    behaveRun.surface.setChaparralFuelBedDepth(3.0, LengthUnits::Feet);
    behaveRun.surface.setChaparralFuelType(ChaparralFuelType::MixedBrush);
    behaveRun.surface.setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::FuelLoadFromDepthAndChaparralType);
    behaveRun.surface.setChaparralFuelDeadLoadFraction(0.50);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::DirectMidflame);
    behaveRun.surface.setWindSpeed(5.0, SpeedUnits::MilesPerHour, WindHeightInputMode::DirectMidflame);
    behaveRun.surface.setSlope(20.0, SlopeUnits::Percent);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 13.945025;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    behaveRun.surface.setIsUsingChaparral(false);
    behaveRun.surface.setWindAndSpreadOrientationMode(windAndSpreadOrientationMode);
}

void testPalmettoGallberry(TestInfo& testInfo, BehaveRun& behaveRun)
{
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    double firstFuelModelCoverage = 0;
    FractionUnits::FractionUnitsEnum firstFuelModelCoverageUnits = FractionUnits::Percent;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double ageOfRough = 10; // years
    double heightOfUnderstory = 4; // feet
    double palmetoCoverage = 50; 
    double overstoryBasalArea = 50; // sqft/ac

    std::string testName = "Test Palmetto-Gallberry spread rate";
    behaveRun.surface.setIsUsingPalmettoGallberry(true);
    BasalAreaUnits::BasalAreaUnitsEnum basalAreaUnits = BasalAreaUnits::SquareFeetPerAcre;
    behaveRun.surface.updateSurfaceInputsForPalmettoGallbery(moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits,
        windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode, ageOfRough, heightOfUnderstory, palmetoCoverage, overstoryBasalArea, basalAreaUnits,
        slope, slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    double observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    double expectedSurfaceFireSpreadRate = 12.521131;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);
    behaveRun.surface.setIsUsingPalmettoGallberry(false);
}

void testWesternAspen(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::string testName = "Test Western Aspen spread rate";
    behaveRun.surface.setIsUsingWesternAspen(true);
    int aspenFuelModel = 3;
    double aspenCuringLevel = 50.0;
    FractionUnits::FractionUnitsEnum curingLevelUnits = FractionUnits::Percent;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    AspenFireSeverity::AspenFireSeverityEnum aspenFireSeverity = AspenFireSeverity::Low;
    double dbh = 10.0;
    LengthUnits::LengthUnitsEnum dbhUnits = LengthUnits::Inches;
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    FractionUnits::FractionUnitsEnum mositureUnits = FractionUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;

    behaveRun.surface.updateSurfaceInputsForWesternAspen(aspenFuelModel, aspenCuringLevel, curingLevelUnits, aspenFireSeverity, dbh, dbhUnits,
        moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, mositureUnits,
        windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect,
        canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    double observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    double expectedSurfaceFireSpreadRate = 0.847629;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);
    
    testName = "Test Western Aspen mortality";
    double observedMortality = behaveRun.surface.getAspenMortality(FractionUnits::Fraction);
    double expectedMortality = 0.267093;
    reportTestResult(testInfo, testName, observedMortality, expectedMortality, error_tolerance);

    behaveRun.surface.setIsUsingWesternAspen(false);
}

void testLengthToWidthRatio(TestInfo& testInfo, BehaveRun& behaveRun)
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

    testName = "Test length-to-width-ratio, north oriented mode, 0 degree wind, 5 mph midflame wind, 0 degree aspect, 0 degree slope";
    windHeightInputMode = WindHeightInputMode::DirectMidflame;
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(0, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 1.590064;
    reportTestResult(testInfo, testName, observedLengthToWidthRatio, expectedLengthToWidthRatio, error_tolerance);

    testName = "Test length-to-width-ratio, north oriented mode, 45 degree wind, 5 mph 20 foot wind, 95 degree aspect, 30 degree slope";
    windHeightInputMode = WindHeightInputMode::TwentyFoot;
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.setAspect(95);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 1.375624;
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
    expectedLengthToWidthRatio = 1.519936;
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

void testEllipticalDimensions(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing elliptical dimensions\n";
    string testName = "";

    double observedA = 0;
    double observedB = 0;
    double observedC = 0;
    double expectedA = 0.628905;
    double expectedB = 1.0;
    double expectedC = 0.777482;

    double elapsedTime = 3.5869124; // 3.5869124 minutes chosen so that semimajor axis = 1.0 

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Chains;
   
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.surface.setSlope(0, SlopeUnits::Degrees);
    behaveRun.surface.setWindSpeed(5, SpeedUnits::MilesPerHour, WindHeightInputMode::DirectMidflame);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

    testName = "Test fire elliptical dimension a with 5 mph direct mid-flame, upslope mode, 3.5869124 minutes elapsed time";
    observedA = roundToSixDecimalPlaces(behaveRun.surface.getEllipticalA(lengthUnits, elapsedTime, TimeUnits::Minutes));
    reportTestResult(testInfo, testName, observedA, expectedA, error_tolerance);
   
    testName = "Test fire elliptical dimension b with 5 mph direct mid-flame, upslope mode, 3.5869124 minutes elapsed time";
    observedB = roundToSixDecimalPlaces(behaveRun.surface.getEllipticalB(lengthUnits, elapsedTime, TimeUnits::Minutes));
    reportTestResult(testInfo, testName, observedB, expectedB, error_tolerance);

    testName = "Test fire elliptical dimension c with 5 mph direct mid-flame, upslope mode, 3.5869124 minutes elapsed time";
    observedC = roundToSixDecimalPlaces(observedC = behaveRun.surface.getEllipticalC(lengthUnits, elapsedTime, TimeUnits::Minutes));
    reportTestResult(testInfo, testName, observedC, expectedC, error_tolerance);

    // Heading to Backing Ratio
    double expectedHeadingToBackingRatio = 7.988029;
    double observedHeadingToBackingRatio = 0.0;

    testName = "Test fire heading to backing ratio with 5 mph direct mid-flame, upslope mode";
    observedHeadingToBackingRatio = roundToSixDecimalPlaces(observedHeadingToBackingRatio = behaveRun.surface.getHeadingToBackingRatio());
    reportTestResult(testInfo, testName, observedHeadingToBackingRatio, expectedHeadingToBackingRatio, error_tolerance);

    // Area
    elapsedTime = 1.0;
    double expectedArea = 0.0;
    double observedArea = 0.0;

    expectedArea = 55.283555;
    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::Acres, elapsedTime, TimeUnits::Hours));
    testName = "Test fire elliptical area in acres with direct mid-flame, upslope mode, 1 hour elapsed time";
    reportTestResult(testInfo, testName, observedArea, expectedArea, error_tolerance);

    expectedArea = 0.223725;
    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::SquareKilometers, elapsedTime, TimeUnits::Hours));
    testName = "Test fire elliptical area in km^2 with direct mid-flame, upslope mode, 1 hour elapsed time";
    reportTestResult(testInfo, testName, observedArea, expectedArea, error_tolerance);

    // Perimeter
    double expectedPerimeter = 0.0;
    double obeservedPerimeter = 0.0;

    expectedPerimeter = 86.71476;
    obeservedPerimeter = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFirePerimeter(LengthUnits::Chains, elapsedTime, TimeUnits::Hours));
    testName = "Test fire elliptical perimeter in chains with direct mid-flame, upslope mode, 1 hour elapsed time";
    reportTestResult(testInfo, testName, obeservedPerimeter, expectedPerimeter, error_tolerance);

    std::cout << "Finished testing elliptical dimensions\n\n";
}

void testDirectionOfInterest(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing spread rate in direction of interest\n";
    string testName = "";
    double directionOfInterest = 0;
    double observedSpreadRateInDirectionOfInterest = 0.0;
    double expectedSpreadRateInDirectionOfInterest = 0.0;
    double observedFlameLengthInDirectionOfInterest = 0.0;
    double expectedFlameLengthInDirectionOfInterest = 0.0;
    SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum surfaceFireSpreadDirectionMode = SurfaceFireSpreadDirectionMode::FromPerimeter;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
  
    testName = "Test perimeter spread direction mode upslope oriented mode, 20 foot wind, direction of interest 90 degrees from upslope, 45 degree wind";
    directionOfInterest = 90;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::FeetPerMinute));
    expectedSpreadRateInDirectionOfInterest = 5.596433;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test perimeter spread direction mode upslope oriented mode, 20 foot wind, direction of interest 90 degrees from upslope, 45 degree wind flame length";
    observedFlameLengthInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getFlameLengthInDirectionOfInterest(LengthUnits::Feet));
    expectedFlameLengthInDirectionOfInterest = 6.5981480000000001;
    reportTestResult(testInfo, testName, observedFlameLengthInDirectionOfInterest, expectedFlameLengthInDirectionOfInterest, error_tolerance);

    testName = "Test upslope oriented mode, 20 foot wind, direction of interest 160 degrees from upslope, 290 degree wind";
    directionOfInterest = 160;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(290);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 2.766387;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test upslope oriented mode, 20 foot wind, direction of interest 215 degrees from upslope, 215 degree wind";
    directionOfInterest = 215;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(215);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 2.818063;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    surfaceFireSpreadDirectionMode = SurfaceFireSpreadDirectionMode::FromIgnitionPoint;

    testName = "Test north oriented mode, 20 foot 135 degree wind, direction of interest 30 degrees from north, 263 degree aspect";
    directionOfInterest = 30;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(280);
    behaveRun.surface.setAspect(135);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 4.180938;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test north oriented mode, 20 foot north wind, direction of interest 90 degrees from north, 45 degree aspect";
    directionOfInterest = 90;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(45);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 3.438243;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    testName = "Test north oriented mode, 20 foot 135 degree wind, direction of interest 285 degrees from north, 263 degree aspect";
    directionOfInterest = 285;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(280);
    behaveRun.surface.setAspect(263);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 2.944975;
    reportTestResult(testInfo, testName, observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, error_tolerance);

    std::cout << "Finished testing spread rate in direction of interest\n\n";
}

void testFirelineIntensity(TestInfo& testInfo, BehaveRun& behaveRun)
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

void testTwoFuelModels(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Two Fuel Models, first fuel model 1, second fuel model 124\n";
    string testName = "";
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    // Test upslope oriented mode, 20 foot wind, low moisture conditions
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    setSurfaceInputsForTwoFuelModelsLowMoistureScenario(behaveRun);

    FractionUnits::FractionUnitsEnum coverUnits = FractionUnits::Percent;

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
    expectedSurfaceFireSpreadRate = 10.470801;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 20";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(20, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 12.189713;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 30";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(30, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 13.958900;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 40";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(40, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 15.706408;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 50";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(50, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 17.362382;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 60";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(60, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 18.859066;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 70";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(70, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 20.130802;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 80";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(80, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.114030;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 90";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(90, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.747289;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    testName = "First fuel model coverage 100";
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(100, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.971217;
    reportTestResult(testInfo, testName, observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, error_tolerance);

    std::cout << "Finished testing Two Fuel Models, first fuel model 1, second fuel model 124\n\n";
}

void testCrownModuleRothermel(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Crown module, Rothermel\n";
    string testName = "";
    double canopyHeight = 30;
    double canopyBaseHeight = 6;
    double canopyBulkDensity = 0.03;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    double observedCrownFireSpreadRate = 0.0;
    double expectedCrownFireSpreadRate = 0.0;
    double observedCrownFlameLength = 0.0;
    double expectedCrownFlameLength = 0.0;
    double observedCrownFirelineIntensity = 0.0;
    double expectedCrownFirelineIntensity = 0.0;
    double observedCrownFireLengthToWidthRatio = 0.0;
    double expectedCrownFireLengthToWidthRatio = 0.0;
    double observedCrownFireArea = 0.0;
    double expectedCrownFireArea = 0.0;
    double observedCrownFirePerimeter = 0.0;
    double expectedCrownFirePerimeter = 0.0;
    int expectedFireType = (int)FireType::Surface;
    int observedFireType = (int)FireType::Surface;

    behaveRun.crown.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);

    testName = "Test crown Rothermel fire spread rate";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownFireSpreadRate = 10.259921;
    observedCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireSpreadRate(SpeedUnits::ChainsPerHour));
    reportTestResult(testInfo, testName, observedCrownFireSpreadRate, expectedCrownFireSpreadRate, error_tolerance);

    testName = "Test crown fire Rothermel length-to-width ratio";
    expectedCrownFireLengthToWidthRatio = 1.625;
    observedCrownFireLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    reportTestResult(testInfo, testName, observedCrownFireLengthToWidthRatio, expectedCrownFireLengthToWidthRatio, error_tolerance);

    testName = "Test crown fire Rothermel area";
    expectedCrownFireArea = 5.087736;
    observedCrownFireArea = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireArea(AreaUnits::Acres, 1.0, TimeUnits::Hours));
    reportTestResult(testInfo, testName, observedCrownFireArea, expectedCrownFireArea, error_tolerance);

    testName = "Test crown fire Rothermel perimeter";
    expectedCrownFirePerimeter = 26.033937;
    observedCrownFirePerimeter = roundToSixDecimalPlaces(behaveRun.crown.getCrownFirePerimeter(LengthUnits::Chains, 1.0, TimeUnits::Hours));
    reportTestResult(testInfo, testName, observedCrownFireArea, expectedCrownFireArea, error_tolerance);

    testName = "Test crown fire Rothermel flame length";
    expectedCrownFlameLength = 29.320557;
    observedCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getCrownFlameLength(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedCrownFlameLength, expectedCrownFlameLength, error_tolerance);

    testName = "Test crown Rothermel fireline intensity";
    expectedCrownFirelineIntensity = 1775.061222;
    observedCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getCrownFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond));
    reportTestResult(testInfo, testName, observedCrownFirelineIntensity, expectedCrownFirelineIntensity, error_tolerance);

    testName = "Test fire type Rothermel, Surface fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setMoistureOneHour(20, FractionUnits::Percent);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Surface;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    testName = "Test fire type Rothermel, Torching fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Torching;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    testName = "Test fire type Rothermel, Crowning fire expected";
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Crowning;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, observedFireType, expectedFireType, error_tolerance);

    testName = "Test fire type Rothermel, Conditional crown fire expected";
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

void testCrownModuleScottAndReinhardt(TestInfo& testInfo, BehaveRun& behaveRun)
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
    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;
    double windSpeed = 2187.226624;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::FeetPerMinute;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    double slope = 20;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    FractionUnits::FractionUnitsEnum coverUnits = FractionUnits::Percent;
    double canopyHeight = 38.104626;
    double canopyBaseHeight = 2.952756;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBulkDensity = 0.01311;
    DensityUnits::DensityUnitsEnum canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.crown.setUserProvidedWindAdjustmentFactor(0.4);
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, FractionUnits::Fraction, canopyBulkDensity, canopyBulkDensityUnits);

    testName = "Test Scott and Reinhardt crown fire spread rate";
    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 65.221842;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::FeetPerMinute));
    reportTestResult(testInfo, testName, observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, error_tolerance);

    testName = "Test Scott and Reinhardt crown flame length";
    expectedFinalCrownFlameLength = 60.744542;
    observedFinalCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getFinalFlameLength(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedFinalCrownFlameLength, expectedFinalCrownFlameLength, error_tolerance);

    testName = "Test Scott and Reinhardt crown fireline intensity";
    expectedFinalCrownFirelineIntensity = 5293.170672;
    observedFinalCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getFinalFirelineIntesity(FirelineIntensityUnits::BtusPerFootPerSecond));
    reportTestResult(testInfo, testName, observedFinalCrownFirelineIntensity, expectedFinalCrownFirelineIntensity, error_tolerance);

    testName = "Test Scott and Reinhardt crown fire critical open wind speed";
    expectedCriticalOpenWindSpeed = 1717.916785;
    observedCriticalOpenWindSpeed = behaveRun.crown.getCriticalOpenWindSpeed(SpeedUnits::FeetPerMinute);
    reportTestResult(testInfo, testName, observedCriticalOpenWindSpeed, expectedCriticalOpenWindSpeed, error_tolerance);

    testName = "Test Scott and Reinhardt crown fire type, crown fire expected";
    expectedFireType = (int)FireType::Crowning;
    observedFireType = (int)behaveRun.crown.getFireType();
    reportTestResult(testInfo, testName, expectedFireType, observedFireType, error_tolerance);

    testName = "Test Scott and Reinhardt crown fire spread rate with aggregate moisture";
    behaveRun.crown.setMoistureInputMode(MoistureInputMode::AllAggregate);
    behaveRun.crown.setMoistureDeadAggregate(9.0, moistureUnits);
    behaveRun.crown.setMoistureLiveAggregate(100.0, moistureUnits);
    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UseCrownRatio);
    behaveRun.crown.setWindSpeed(2187.2266239, windSpeedUnits, WindHeightInputMode::TwentyFoot);
    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 64.016394;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::ChainsPerHour));
    reportTestResult(testInfo, testName, observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, error_tolerance);

    testName = "Test Scott and Reinhardt crown fire spread rate with moisture scenario D3L2";
    behaveRun.crown.setMoistureInputMode(MoistureInputMode::MoistureScenario);
    behaveRun.crown.setCurrentMoistureScenarioByName("D3L2");
    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 68.334996;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::ChainsPerHour));
    reportTestResult(testInfo, testName, observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, error_tolerance);

    // Test crown fire spread rate, flame length, intensity; Torching fire expected
    fuelModelNumber = 5;
    moistureOneHour = 5.0;
    moistureTenHour = 6.0;
    moistureHundredHour = 8.0;
    moistureLiveHerbaceous = 0.0;
    moistureLiveWoody = 117.0;
    moistureFoliar = 100;
    moistureUnits = FractionUnits::Percent;
    windSpeed = 24.854848;
    windHeightInputMode = WindHeightInputMode::TwentyFoot;
    windSpeedUnits = SpeedUnits::MilesPerHour;
    windDirection = 0;
    windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    slope = 20;
    slopeUnits = SlopeUnits::Percent;
    aspect = 0;
    canopyCover = 50;
    coverUnits = FractionUnits::Percent;
    canopyHeight = 71.631562;
    canopyBaseHeight = 4.92126;
    canopyHeightUnits = LengthUnits::Feet;
    crownRatio = 0.50;
    canopyBulkDensity = 0.003746;
    canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.setMoistureInputMode(MoistureInputMode::BySizeClass);
    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.crown.setUserProvidedWindAdjustmentFactor(0.15);
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, FractionUnits::Fraction, canopyBulkDensity, canopyBulkDensityUnits);

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

void testSpotModule(TestInfo& testInfo, BehaveRun& behaveRun)
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
    behaveRun.surface.setUserProvidedWindAdjustmentFactor(1.0);
    behaveRun.surface.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
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

    SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downWindCanopyMode = SpotDownWindCanopyMode::CLOSED;

    behaveRun.spot.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, downWindCanopyMode,
        burningPileflameHeight, flameHeightUnits, windSpeedAtTwentyFeet, windSpeedUnits);
    behaveRun.spot.calculateSpottingDistanceFromBurningPile();
    
    testName = "Test mountain spotting distance from burning pile, closed downwind canopy";
    expectedMountainSpottingDistance = 0.021330;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);
    
    testName = "Test flat spotting distance from burning pile, closed downwind canopy";
    expectedFlatSpottingDistance = 0.017067;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);

    downWindCanopyMode = SpotDownWindCanopyMode::OPEN;

    behaveRun.spot.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, downWindCanopyMode,
        burningPileflameHeight, flameHeightUnits, windSpeedAtTwentyFeet, windSpeedUnits);
    behaveRun.spot.calculateSpottingDistanceFromBurningPile();

    testName = "Test mountain spotting distance from burning pile, open downwind canopy";
    expectedMountainSpottingDistance = 0.030863;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);

    testName = "Test flat spotting distance from burning pile, closed downwind canopy";
    expectedFlatSpottingDistance = 0.024700;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);

    downWindCanopyMode = SpotDownWindCanopyMode::CLOSED;

    behaveRun.spot.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, downWindCanopyMode,
        windSpeedAtTwentyFeet, windSpeedUnits, flameLength, flameHeightUnits);
    behaveRun.spot.calculateSpottingDistanceFromSurfaceFire();

    testName = "Test mountain spotting distance from surface fire, closed downwind canopy";
    expectedMountainSpottingDistance = 0.267467;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromSurfaceFire(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);
    
    testName = "Test flat spotting distance from surface fire, closed downwind canopy";
    expectedFlatSpottingDistance = 0.22005;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromSurfaceFire(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);
    
    behaveRun.spot.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, downWindCanopyMode,
        torchingTrees, DBH, DBHUnits, treeHeight, treeHeightUnits, treeSpecies, windSpeedAtTwentyFeet, windSpeedUnits);
    behaveRun.spot.calculateSpottingDistanceFromTorchingTrees();
    
    testName = "Test mountain spotting distance from torching trees, closed downwind canopy";
    expectedMountainSpottingDistance = 0.222396;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromTorchingTrees(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);
    
    testName = "Test flat spotting distance from from torching trees, closed downwind canopy";
    expectedFlatSpottingDistance = 0.181449;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromTorchingTrees(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedFlatSpottingDistance, expectedFlatSpottingDistance, error_tolerance);

    /// Active Crown
    testName = "Test mountain spotting distance from active crown fire, closed downwind canopy";
    double activeCrownFlameLength = 20.0;

    behaveRun.spot.updateSpotInputsForActiveCrownFire(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, treeHeight, treeHeightUnits, downWindCanopyMode,
        windSpeedAtTwentyFeet, windSpeedUnits, activeCrownFlameLength, flameHeightUnits);
    behaveRun.spot.calculateSpottingDistanceFromActiveCrown();

    expectedMountainSpottingDistance = 0.384247;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromActiveCrown(spottingDistanceUnits));
    reportTestResult(testInfo, testName, observedMountainSpottingDistance, expectedMountainSpottingDistance, error_tolerance);

    std::cout << "Finished testing Spot module\n\n";
}

void testSpeedUnitConversion(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing speed unit conversion\n";

    string testName = "";

    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    behaveRun.surface.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UseCrownRatio);
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

void testIgniteModule(TestInfo& testInfo, BehaveRun& behaveRun)
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
   
    FractionUnits::FractionUnitsEnum mositureUnits = FractionUnits::Percent;
    TemperatureUnits::TemperatureUnitsEnum temperatureUnits = TemperatureUnits::Fahrenheit;
    FractionUnits::FractionUnitsEnum sunShadeUnits = FractionUnits::Percent;
    LengthUnits::LengthUnitsEnum duffDepthUnits = LengthUnits::Inches;

    expectedFirebrandIgnitionProbability = 0.54831705;
    expectedLightningIgnitionProbability = 0.39362018;

    behaveRun.ignite.updateIgniteInputs(moistureOneHour, moistureHundredHour, mositureUnits, airTemperature,
        temperatureUnits, sunShade, sunShadeUnits, fuelBedType, duffDepth, duffDepthUnits, lightningChargeType);

    testName = "Test firebrand ignition probability for Douglas fir duff";
    observedFirebrandIgnitionProbability = behaveRun.ignite.calculateFirebrandIgnitionProbability(FractionUnits::Fraction);
    reportTestResult(testInfo, testName, observedFirebrandIgnitionProbability, expectedFirebrandIgnitionProbability, error_tolerance);

    testName = "Test lightning ignition probability for Douglas fir duff";
    observedLightningIgnitionProbability = behaveRun.ignite.calculateLightningIgnitionProbability(FractionUnits::Fraction);
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
    observedFirebrandIgnitionProbability = behaveRun.ignite.calculateFirebrandIgnitionProbability(FractionUnits::Percent);
    reportTestResult(testInfo, testName, observedFirebrandIgnitionProbability, expectedFirebrandIgnitionProbability, error_tolerance);

    testName = "Test lightning ignition probability for Lodgepole pine duff";
    observedLightningIgnitionProbability = behaveRun.ignite.calculateLightningIgnitionProbability(FractionUnits::Percent);
    reportTestResult(testInfo, testName, observedLightningIgnitionProbability, expectedLightningIgnitionProbability, error_tolerance);

    std::cout << "Finished testing Ignite module\n\n";
}

void testSafetyModule(TestInfo& testInfo, BehaveRun& behaveRun)
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

void testContainModule(TestInfo& testInfo, BehaveRun& behaveRun)
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
    expectedPerimeterAtInitialAttack = 37.51917991;
    observedPerimeterAtInitialAttack = behaveRun.contain.getPerimiterAtInitialAttack(LengthUnits::Chains);
    reportTestResult(testInfo, testName, observedPerimeterAtInitialAttack, expectedPerimeterAtInitialAttack, error_tolerance);

    testName = "Test observed perimeter at containment";
    expectedPerimeterAtContainment = 39.539849615;
    observedPerimeterAtContainment = behaveRun.contain.getPerimeterAtContainment(LengthUnits::Chains);
    reportTestResult(testInfo, testName, observedPerimeterAtContainment, expectedPerimeterAtContainment, error_tolerance);

    testName = "Test observed fire size at initial attack";
    expectedFireSizeAtInitialAttack = 8.954501709;
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

void testMortalityModule(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Mortality module\n";

    // TODO: Create tests for Mortality module
    // Stub for now

    std::cout << "Finished testing Mortality module\n\n";
}

void testFineDeadFuelMoistureTool(TestInfo& testInfo, BehaveRun& behaveRun)
{
    double observedReferenceMoisture = 0.0;
    double observedCorrectionMoisture = 0.0;
    double observedFineDeadFuelMoisture = 0.0;

    double expectedReferenceMoisture = 0;
    double expectedCorrectionMoisture = 0;
    double expectedFineDeadFuelMoisture = 0;

    int aspectIndex = 0;
    int dryBulbIndex = 0;
    int elevationIndex = 0;
    int monthIndex = 0;
    int relativeHumidityIndex = 0;
    int shadingIndex = 0;
    int slopeIndex = 0;
    int timeOfDayIndex = 0;

    string testName = "";

    std::cout << "Testing Fine Dead Fuel Moisture Tool\n";

    behaveRun.fineDeadFuelMoistureTool.calculateByIndex(aspectIndex, dryBulbIndex, elevationIndex, monthIndex, relativeHumidityIndex, shadingIndex, slopeIndex, timeOfDayIndex);
    expectedReferenceMoisture = 1;
    expectedCorrectionMoisture = 2;
    expectedFineDeadFuelMoisture = 3;
    observedReferenceMoisture = behaveRun.fineDeadFuelMoistureTool.getReferenceMoisture(FractionUnits::Percent);
    observedCorrectionMoisture = behaveRun.fineDeadFuelMoistureTool.getCorrectionMoisture(FractionUnits::Percent);
    observedFineDeadFuelMoisture = behaveRun.fineDeadFuelMoistureTool.getFineDeadFuelMoisture(FractionUnits::Percent);
    testName = "Test reference moisture for all zero index values\n";
    reportTestResult(testInfo, testName, observedReferenceMoisture, expectedReferenceMoisture, error_tolerance);
    testName = "Test correction moisture for all zero index values\n";
    reportTestResult(testInfo, testName, observedCorrectionMoisture, expectedCorrectionMoisture, error_tolerance);
    testName = "Test fine dead fuel moisture for all zero index values\n";
    reportTestResult(testInfo, testName, observedFineDeadFuelMoisture, expectedFineDeadFuelMoisture, error_tolerance);

    aspectIndex++;
    dryBulbIndex++;
    elevationIndex++;
    monthIndex++;
    relativeHumidityIndex++;
    shadingIndex++;
    slopeIndex++;
    timeOfDayIndex++;

    behaveRun.fineDeadFuelMoistureTool.calculateByIndex(aspectIndex, dryBulbIndex, elevationIndex, monthIndex, relativeHumidityIndex, shadingIndex, slopeIndex, timeOfDayIndex);
    expectedReferenceMoisture = 2;
    expectedCorrectionMoisture = 4;
    expectedFineDeadFuelMoisture = 6;
    observedReferenceMoisture = behaveRun.fineDeadFuelMoistureTool.getReferenceMoisture(FractionUnits::Percent);
    observedCorrectionMoisture = behaveRun.fineDeadFuelMoistureTool.getCorrectionMoisture(FractionUnits::Percent);
    observedFineDeadFuelMoisture = behaveRun.fineDeadFuelMoistureTool.getFineDeadFuelMoisture(FractionUnits::Percent);
    testName = "Test reference moisture for all one index values\n";
    reportTestResult(testInfo, testName, observedReferenceMoisture, expectedReferenceMoisture, error_tolerance);
    testName = "Test correction moisture for all one index values\n";
    reportTestResult(testInfo, testName, observedCorrectionMoisture, expectedCorrectionMoisture, error_tolerance);
    testName = "Test fine dead fuel moisture for all one index values\n";
    reportTestResult(testInfo, testName, observedFineDeadFuelMoisture, expectedFineDeadFuelMoisture, error_tolerance);

    aspectIndex = behaveRun.fineDeadFuelMoistureTool.getAspectIndexSize() - 1;
    dryBulbIndex = behaveRun.fineDeadFuelMoistureTool.getDryBulbTemperatureIndexSize() - 1;
    elevationIndex = behaveRun.fineDeadFuelMoistureTool.getElevationIndexSize() - 1;
    monthIndex = behaveRun.fineDeadFuelMoistureTool.getMonthIndexSize() - 1;
    relativeHumidityIndex = behaveRun.fineDeadFuelMoistureTool.getRelativeHumidityIndexSize() - 1;
    shadingIndex = behaveRun.fineDeadFuelMoistureTool.getShadingIndexSize() - 1;
    slopeIndex = behaveRun.fineDeadFuelMoistureTool.getSlopeIndexSize() - 1;
    timeOfDayIndex = behaveRun.fineDeadFuelMoistureTool.getTimeOfDayIndexSize() - 1;

    behaveRun.fineDeadFuelMoistureTool.calculateByIndex(aspectIndex, dryBulbIndex, elevationIndex, monthIndex, relativeHumidityIndex, shadingIndex, slopeIndex, timeOfDayIndex);
    expectedReferenceMoisture = 12;
    expectedCorrectionMoisture = 6;
    expectedFineDeadFuelMoisture = 18;
    observedReferenceMoisture = behaveRun.fineDeadFuelMoistureTool.getReferenceMoisture(FractionUnits::Percent);
    observedCorrectionMoisture = behaveRun.fineDeadFuelMoistureTool.getCorrectionMoisture(FractionUnits::Percent);
    observedFineDeadFuelMoisture = behaveRun.fineDeadFuelMoistureTool.getFineDeadFuelMoisture(FractionUnits::Percent);
    testName = "Test reference moisture for all max index values\n";
    reportTestResult(testInfo, testName, observedReferenceMoisture, expectedReferenceMoisture, error_tolerance);
    testName = "Test correction moisture for all max index values\n";
    reportTestResult(testInfo, testName, observedCorrectionMoisture, expectedCorrectionMoisture, error_tolerance);
    testName = "Test fine dead fuel moisture for max one index values\n";
    reportTestResult(testInfo, testName, observedFineDeadFuelMoisture, expectedFineDeadFuelMoisture, error_tolerance);

    aspectIndex++;
    dryBulbIndex++;
    elevationIndex++;
    monthIndex++;
    relativeHumidityIndex++;
    shadingIndex++;
    slopeIndex++;
    timeOfDayIndex++;

    behaveRun.fineDeadFuelMoistureTool.calculateByIndex(aspectIndex, dryBulbIndex, elevationIndex, monthIndex, relativeHumidityIndex, shadingIndex, slopeIndex, timeOfDayIndex);
    expectedReferenceMoisture = -1;
    expectedCorrectionMoisture = -1;
    expectedFineDeadFuelMoisture = -1;
    observedReferenceMoisture = behaveRun.fineDeadFuelMoistureTool.getReferenceMoisture(FractionUnits::Percent);
    observedCorrectionMoisture = behaveRun.fineDeadFuelMoistureTool.getCorrectionMoisture(FractionUnits::Percent);
    observedFineDeadFuelMoisture = behaveRun.fineDeadFuelMoistureTool.getFineDeadFuelMoisture(FractionUnits::Percent);
    testName = "Test reference moisture for all indices out of bounds\n";
    reportTestResult(testInfo, testName, observedReferenceMoisture, expectedReferenceMoisture, error_tolerance);
    testName = "Test correction moisture for all indices out of bounds\n";
    reportTestResult(testInfo, testName, observedCorrectionMoisture, expectedCorrectionMoisture, error_tolerance);
    testName = "Test fine dead fuel moisture all indices out of bounds\n";
    reportTestResult(testInfo, testName, observedFineDeadFuelMoisture, expectedFineDeadFuelMoisture, error_tolerance);

    std::cout << "Finished testing Fine Dead Fuel Moisture Tool\n\n";
}

void testSlopeTool(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Slope Tool\n";

    int mapRepresentativeFraction = 0;
    double mapDistance = 0.0;
    LengthUnits::LengthUnitsEnum mapDistanceUnits = LengthUnits::Inches;
    double contourInterval = 0.0;
    double numberOfContours = 0.0;
    LengthUnits::LengthUnitsEnum contourUnits = LengthUnits::Feet;
    LengthUnits::LengthUnitsEnum slopeDistanceUnits = LengthUnits::Feet;
    LengthUnits::LengthUnitsEnum slopeElevationUnits = LengthUnits::Feet;
    string testName = "";

    double observedSlopeSteepnessDegrees = 0.0;
    double expectedSlopeSteepnessDegrees = 0.0;
    double observedSlopeSteepnessPercent = 0.0;
    double expectedSlopeSteepnessPercent = 0.0;
    double observedSlopeDistance = 0.0;
    double expectedSlopeDistance = 0.0;
    double observedSlopeElevationChange = 0.0;
    double expectedSlopeElevationChange = 0.0;

    mapRepresentativeFraction = 1980;
    mapDistance = 3.6;
    contourInterval = 50.0;
    numberOfContours = 4.1;
    behaveRun.slopeTool.calculateSlopeFromMapMeasurements(mapRepresentativeFraction, mapDistance, mapDistanceUnits, contourInterval, numberOfContours, contourUnits);

    testName = "Test slope in degrees from map measurements, imperial units";
    expectedSlopeSteepnessDegrees = 19.0;
    observedSlopeSteepnessDegrees = std::round(behaveRun.slopeTool.getSlopeFromMapMeasurements(SlopeUnits::Degrees)); // BehavePlus 6 tool always rounds to nearest integer
    reportTestResult(testInfo, testName, observedSlopeSteepnessDegrees, expectedSlopeSteepnessDegrees, error_tolerance);

    testName = "Test slope in percent from map measurements, imperial units";
    expectedSlopeSteepnessPercent = 35.0;
    observedSlopeSteepnessPercent = std::round(behaveRun.slopeTool.getSlopeFromMapMeasurements(SlopeUnits::Percent));
    reportTestResult(testInfo, testName, observedSlopeSteepnessPercent, expectedSlopeSteepnessPercent, error_tolerance);

    testName = "Test slope elevation change from map measurements, imperial units";
    expectedSlopeElevationChange = 205.0;
    observedSlopeSteepnessPercent = std::round(behaveRun.slopeTool.getSlopeElevationChangeFromMapMeasurements(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedSlopeSteepnessPercent, expectedSlopeElevationChange, error_tolerance);

    testName = "Test slope horizontal distance from map measurements, imperial units";
    expectedSlopeDistance = 594.0;
    observedSlopeDistance = std::round(behaveRun.slopeTool.getSlopeHorizontalDistanceFromMapMeasurements(slopeDistanceUnits));
    reportTestResult(testInfo, testName, observedSlopeDistance, expectedSlopeDistance, error_tolerance);

    mapRepresentativeFraction = 3960;
    mapDistance = 3.0;
    mapDistanceUnits = LengthUnits::Centimeters;
    contourInterval = 15.0;
    numberOfContours = 5.5;
    contourUnits = LengthUnits::Meters;
    slopeDistanceUnits = LengthUnits::Meters;
    slopeElevationUnits = LengthUnits::Meters;
    behaveRun.slopeTool.calculateSlopeFromMapMeasurements(mapRepresentativeFraction, mapDistance, mapDistanceUnits, contourInterval, numberOfContours, contourUnits);

    testName = "Test slope in degrees from map measurements, metric units";
    expectedSlopeSteepnessDegrees = 35.0;
    observedSlopeSteepnessDegrees = std::round(behaveRun.slopeTool.getSlopeFromMapMeasurements(SlopeUnits::Degrees));
    reportTestResult(testInfo, testName, observedSlopeSteepnessDegrees, expectedSlopeSteepnessDegrees, error_tolerance);

    testName = "Test slope in percent from map measurements, metric units";
    expectedSlopeSteepnessPercent = 69.0;
    observedSlopeSteepnessPercent = std::round(behaveRun.slopeTool.getSlopeFromMapMeasurements(SlopeUnits::Percent));
    reportTestResult(testInfo, testName, observedSlopeSteepnessPercent, expectedSlopeSteepnessPercent, error_tolerance);

    testName = "Test slope elevation change from map measurements, metric units";
    expectedSlopeElevationChange = 82.0;
    observedSlopeSteepnessPercent = std::round(behaveRun.slopeTool.getSlopeElevationChangeFromMapMeasurements(LengthUnits::Meters));
    reportTestResult(testInfo, testName, observedSlopeSteepnessPercent, expectedSlopeElevationChange, error_tolerance);

    testName = "Test slope horizontal distance from map measurements, metric units";
    expectedSlopeDistance = 119.0;
    observedSlopeDistance = std::round(behaveRun.slopeTool.getSlopeHorizontalDistanceFromMapMeasurements(slopeDistanceUnits));
    reportTestResult(testInfo, testName, observedSlopeDistance, expectedSlopeDistance, error_tolerance);

    const int numHorirzonalDistances = behaveRun.slopeTool.getNumberOfHorizontalDistances();
    std::vector<double> observedHoriztonalDistances(numHorirzonalDistances);
    std::vector<double> expectedHoriztonalDistances(numHorirzonalDistances);
    std::vector<string> horizontalDistanceIndexNames(numHorirzonalDistances);
    horizontalDistanceIndexNames[HorizontalDistanceIndex::UPSLOPE_ZERO_DEGREES] = "UPSLOPE_ZERO_DEGREES";
    horizontalDistanceIndexNames[HorizontalDistanceIndex::FIFTEEN_DEGREES_FROM_UPSLOPE] = "FIFTEEN_DEGREES_FROM_UPSLOPE";
    horizontalDistanceIndexNames[HorizontalDistanceIndex::THIRTY_DEGREES_FROM_UPSLOPE] = "THIRTY_DEGREES_FROM_UPSLOPE";
    horizontalDistanceIndexNames[HorizontalDistanceIndex::FORTY_FIVE_DEGREES_FROM_UPSLOPE] = "FORTY_FIVE_DEGREES_FROM_UPSLOPE";
    horizontalDistanceIndexNames[HorizontalDistanceIndex::SIXTY_DEGREES_FROM_UPSLOPE] = "SIXTY_DEGREES_FROM_UPSLOPE";
    horizontalDistanceIndexNames[HorizontalDistanceIndex::SEVENTY_FIVE_DEGREES_FROM_UPSLOPE] = "SEVENTY_FIVE_DEGREES_FROM_UPSLOPE";
    horizontalDistanceIndexNames[HorizontalDistanceIndex::CROSS_SLOPE_NINETY_DEGREES] = "CROSS_SLOPE_NINETY_DEGREES";

    double calculatedMapDistance = 3.0;
    mapDistanceUnits = LengthUnits::Inches;
    double maxSlopeSteepness = 30.0;

    expectedHoriztonalDistances[HorizontalDistanceIndex::UPSLOPE_ZERO_DEGREES] = 2.9;
    expectedHoriztonalDistances[HorizontalDistanceIndex::FIFTEEN_DEGREES_FROM_UPSLOPE] = 2.9;
    expectedHoriztonalDistances[HorizontalDistanceIndex::THIRTY_DEGREES_FROM_UPSLOPE] = 2.9;
    expectedHoriztonalDistances[HorizontalDistanceIndex::FORTY_FIVE_DEGREES_FROM_UPSLOPE] = 2.9;
    expectedHoriztonalDistances[HorizontalDistanceIndex::SIXTY_DEGREES_FROM_UPSLOPE] = 3.0;
    expectedHoriztonalDistances[HorizontalDistanceIndex::SEVENTY_FIVE_DEGREES_FROM_UPSLOPE] = 3.0;
    expectedHoriztonalDistances[HorizontalDistanceIndex::CROSS_SLOPE_NINETY_DEGREES] = 3.0;

    behaveRun.slopeTool.calculateHorizontalDistance(calculatedMapDistance, mapDistanceUnits, maxSlopeSteepness, SlopeUnits::Percent);

    for (int i = 0; i < numHorirzonalDistances; i++)
    {
        testName = "Test calculateHorizontalDistance() " + horizontalDistanceIndexNames[i] + " distance in feet";
        observedHoriztonalDistances[i] = std::round(behaveRun.slopeTool.getHorizontalDistanceAtIndex(i, LengthUnits::Feet) * 10.0) / 10.0; // BehavePlus 6 tool always rounds to nearest tenths place
        reportTestResult(testInfo, testName, observedHoriztonalDistances[i], expectedHoriztonalDistances[i], error_tolerance);
    }

    std::cout << "Finished testing  Slope Tool\n\n";
}

void testVPDScenario(VaporPressureDeficitCalculator& vpdCalculator, TestInfo& testInfo, double tempF, double rhPercent, double expectedVPD) {
    std::stringstream ss;
    ss << "Test " << rhPercent << "% Relative Humidity, " << tempF << " degF Air Temperature";
    std::string testName = ss.str(); // convert stringstream to string

    vpdCalculator.setTemperature(tempF, TemperatureUnits::Fahrenheit);
    vpdCalculator.setRelativeHumidity(rhPercent, FractionUnits::Percent);
    vpdCalculator.runCalculation();
    double observedVPD = vpdCalculator.getVaporPressureDeficit(PressureUnits::HectoPascal);
    reportTestResult(testInfo, testName, observedVPD, expectedVPD, 1e-03);
}

void testVaporPressureDeficitCalculator(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Vapor Pressure Deficit Calculator\n";

    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 100.0, 0.0);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 90.0, 1.22833);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 80.0, 2.45667);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 70.0, 3.685);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 60.0, 4.91334);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 50.0, 6.14167);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 40.0, 7.37001);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 30.0, 8.59834);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 20.0, 9.82667);
    testVPDScenario(behaveRun.vpdCalculator, testInfo, 50.0, 10.0, 11.055);

    std::cout << "Finished testing  Slope Tool\n\n";
}


void testSimpleSurface(TestInfo& testInfo, BehaveRun& behaveRun)
{
    std::cout << "Testing Simple Surface\n";

    string testName = "";

    double expectedRateofSpread = 0.0;
    double observedRateofSpread = 0.0;

    int fuelModelNumber = 124; // fuel model gs4(124) chosen as it is dynamic and has values for all moisture classes
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    FractionUnits::FractionUnitsEnum moistureUnits = FractionUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 0.0;
    FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
    double canopyHeight = 0.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.0;

    behaveRun.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);

    behaveRun.surface.setUserProvidedWindAdjustmentFactor(1.0);
    behaveRun.surface.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

    testName = "Test Simple Surface Rate of Spread";
    expectedRateofSpread = 34.011429;
    observedRateofSpread = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    reportTestResult(testInfo, testName, observedRateofSpread, expectedRateofSpread, error_tolerance);


    testName = "Test Simple Surface Flame Length";
    expectedRateofSpread = 15.811421;
    observedRateofSpread = roundToSixDecimalPlaces(behaveRun.surface.getFlameLength(LengthUnits::Feet));
    reportTestResult(testInfo, testName, observedRateofSpread, expectedRateofSpread, error_tolerance);


    testName = "Test Simple Surface FirelineIntensity";
    expectedRateofSpread = 2292.684759;
    observedRateofSpread = roundToSixDecimalPlaces(behaveRun.surface.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond));
    reportTestResult(testInfo, testName, observedRateofSpread, expectedRateofSpread, error_tolerance);
}
