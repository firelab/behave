#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "behaveRun.h"
#include "fuelModels.h"

// Function Headers
bool areClose(const double observed, const double expected, const double epsilon);
double roundToSixDecimalPlaces(const double numberToBeRounded);
void reportTestResult(struct TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon);
void testContainModule(struct TestInfo& testInfo, BehaveRun& behaveRun);
void setCrownInputsLowMoistureScenario(BehaveRun& behaveRun);

// Define the error tolerance for double values
constexpr double error_tolerance = 1e-06;

struct TestInfo
{
  int numTotalTests = 0;
  int numFailed = 0;
  int numPassed = 0;
};

struct ContainInputs {
  double attackDistance = 0.00;
  double lwRatio = 0.00;
  double reportRate = 0.00;
  double reportSize = 0.00;
  std::string tactic = "";
  double resourceArrival = 0.00;
  double resourceProduction = 0.00;
  double resourceDuration = 0.00;
  std::string resourceFlank = "";
  double resourceBaseCost = 0.00;
  double resourceHourCost = 0.00;
  std::string desc = "";
};

struct ContainOutputs {
  double fireLineLength = 0.00;
  double perimeterAtInitialAttack = 0.00;
  double perimeterAtContainment = 0.00;
  double fireSizeAtInitialAttack = 0.00;
  double fireSize = 0.00;
  double containmentArea = 0.00;
  double timeSinceReport = 0.00;
  std::string containmentStatus = "";
};

int main()
{
    TestInfo testInfo;
    FuelModels fuelModels;
    ContainInputs containInputs;
    ContainOutputs containOutputs;
    SpeciesMasterTable mortalitySpeciesTable;
    BehaveRun behaveRun(fuelModels, containInputs, containOutputs, mortalitySpeciesTable);

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

void testContainModule(struct TestInfo& testInfo, ContainInputs& inputs, ContainOutputs& expected, BehaveRun& behaveRun)
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

    // Test case where expected result is containment
    behaveRun.contain.setAttackDistance(containInputs., LengthUnits::Chains);
    behaveRun.contain.setLwRatio(inputs.lwRatio);
    behaveRun.contain.setReportRate(inputs.reportRate, SpeedUnits::ChainsPerHour);
    behaveRun.contain.setReportSize(inputs.reportSize, AreaUnits::Acres);
    behaveRun.contain.setTactic(ContainTactic::HeadAttack);
    behaveRun.contain.addResource(inputs.resourceArrival,
                                  inputs.resourceProduction,
                                  TimeUnits::Hours,
                                  inputs.resourceDuration,
                                  SpeedUnits::ChainsPerHour,
                                  inputs.resourceDescription);

    // Perform Contain Run
    behaveRun.contain.doContainRun();

    // Compare Results
    testName = "Test final fire line length";
    expectedFinalFireLineLength = expected.FinalFireLength;
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
    reportTestResult(testInfo, testName, observedFireSizeAtInitialAttack, expected.FireSizeAtInitialAttack, error_tolerance);

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
