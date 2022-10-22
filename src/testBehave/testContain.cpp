#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "behaveRun.h"
#include "fuelModels.h"

// Helper Function Headers
bool areClose(const double observed, const double expected, const double epsilon);
double roundToSixDecimalPlaces(const double numberToBeRounded);
void reportTestResult(int row, struct TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon);
double str2double (std::string & str);
bool beginsWithNumber(std::string const & str);

// CSV Parsing
void printCSVData(std::vector<std::string> csvHeaders,
                  std::vector<std::map<std::string, std::string>> csvStringRows,
                  std::vector<std::map<std::string, double>> csvDoubleRows);
int parseCSVFile(std::string filename,
                 std::vector<std::string> & csvHeaders,
                 std::vector<std::map<std::string, std::string>> & csvStringRows,
                 std::vector<std::map<std::string, double>> & csvDoubleRows);

// Define the error tolerance for double values
constexpr double error_tolerance = 1e-06;

// Testing Structs
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
  ContainTactic::ContainTacticEnum tactic = ContainTactic::HeadAttack;
  double resourceArrival = 0.00;
  double resourceProduction = 0.00;
  double resourceDuration = 0.00;
  std::string resourceDescription = "";
};

struct ContainOutputs {
  double fireLineLength = 0.00;
  double perimeterAtInitialAttack = 0.00;
  double perimeterAtContainment = 0.00;
  double fireSizeAtInitialAttack = 0.00;
  double fireSize = 0.00;
  double containmentArea = 0.00;
  double timeSinceReport = 0.00;
  ContainStatus::ContainStatusEnum containmentStatus = ContainStatus::Unreported;
};

// Testing Function Headers
void testContainModule(int row, struct TestInfo& testInfo, ContainInputs& containInputs, ContainOutputs& containOutputs, BehaveRun& behaveRun);

int main(int argc, char * argv[])
{
  if (argc == 1) {
    std::cout << "Unable continue tests. Please supply CSV filename.\n" << std::endl;
    return 1;
  }

  
  TestInfo testInfo;
  FuelModels fuelModels;
  ContainInputs containInputs;
  ContainOutputs containOutputs;
  SpeciesMasterTable mortalitySpeciesTable;
  BehaveRun behaveRun(fuelModels, mortalitySpeciesTable);

  std::string csvFilename{argv[1]};

  std::map<std::string, ContainStatus::ContainStatusEnum> containStatus{{"Unreported", ContainStatus::Unreported},
                                                                        {"Reported", ContainStatus::Reported},
                                                                        {"Contained", ContainStatus::Contained},
                                                                        {"Overrun", ContainStatus::Overrun},
                                                                        {"Exhausted", ContainStatus::Exhausted},
                                                                        {"SizeLimitExceeded", ContainStatus::SizeLimitExceeded},
                                                                        {"TimeLimitExceeded", ContainStatus::TimeLimitExceeded}};

  std::map<std::string, ContainTactic::ContainTacticEnum> containTactic{{"HeadAttack", ContainTactic::HeadAttack},
                                                                        {"RearAttack", ContainTactic::RearAttack}};


  std::vector<std::string> csvHeaders;
  std::vector<std::map<std::string, std::string>> csvStringRows;
  std::vector<std::map<std::string, double>> csvDoubleRows;

  int result = parseCSVFile(csvFilename, csvHeaders, csvStringRows, csvDoubleRows);

  if (result == 0) {
    printCSVData(csvHeaders, csvStringRows, csvDoubleRows);
  }

  std::cout << "Performing tests with: " << csvStringRows.size() << " samples.\n";

  // Perform Tests using ContainInputs
  for (int i = 0; i < csvStringRows.size(); i++) {

      std::map<std::string, double> doubleRow = csvDoubleRows[i];
      std::map<std::string, std::string> stringRow = csvStringRows[i];

      // Set up Inputs
      containInputs.attackDistance = doubleRow["attackDistance"];
      containInputs.lwRatio = doubleRow["lwRatio"];
      containInputs.reportRate = doubleRow["reportRate"];
      containInputs.reportSize = doubleRow["reportSize"];
      containInputs.tactic = containTactic[stringRow["tactic"]];
      containInputs.resourceArrival = doubleRow["resourceArrival"];
      containInputs.resourceProduction = doubleRow["resourceProduction"];
      containInputs.resourceDuration = doubleRow["resourceDuration"];
      containInputs.resourceDescription = stringRow["resourceDescription"];

      // Set up Outputs
      containOutputs.fireLineLength = doubleRow["fireLineLength"];
      containOutputs.perimeterAtInitialAttack = doubleRow["perimeterAtInitialAttack"];
      containOutputs.perimeterAtContainment = doubleRow["perimeterAtContainment"];
      containOutputs.fireSizeAtInitialAttack = doubleRow["fireSizeAtInitialAttack"];
      containOutputs.fireSize = doubleRow["fireSize"];
      containOutputs.containmentArea = doubleRow["containmentArea"];
      containOutputs.timeSinceReport = doubleRow["timeSinceReport"];
      containOutputs.containmentStatus = containStatus[stringRow["containmentStatus"]];

      testContainModule(i, testInfo, containInputs, containOutputs, behaveRun);

  };
  std::cout << "Total tests performed: " << testInfo.numTotalTests << "\n";
  std::cout << "Total tests passed: " << testInfo.numPassed << "\n";
  std::cout << "Total tests failed: " << testInfo.numFailed << "\n\n";

  //#ifndef NDEBUG
  //    // Make Visual Studio wait while in debug mode
  //    std::cout << "Press Enter to continue . . .";
  //    std::cin.get();
  //#endif
  //    return 0;
}

/* Helper Methods */

bool areClose(const double observed, const double expected, const double epsilon)
{
    return fabs(observed - expected) < epsilon;
}

double str2double (std::string & str) {
  char * cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  char * cstr_end = cstr;
  double x = strtod(cstr, &cstr_end);
  if(cstr == cstr_end) { 
    //you have an error!
    return 0;
  } else {
    return x;
  }
}

bool beginsWithNumber(std::string const & str) {
  if (str.length() == 0) {
    return false;
  }
  return isdigit(str[0]);
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

void reportTestResult(int row, struct TestInfo& testInfo, const string testName, const double observed, const double expected, const double epsilon)
{
    testInfo.numTotalTests++;
    if(areClose(observed, expected, epsilon))
    {
        //std::cout << testName << "\npassed successfully\n";
        testInfo.numPassed++;
    }
    else
    {
      std::cout << testName << "\n(Row: " << row << ")" << "\nfailed\nobserved value " << observed << " differs from expected value " << expected << " by more than " << epsilon << "\n\n";
        testInfo.numFailed++;
    }
}

void testContainModule(int row, struct TestInfo& testInfo, ContainInputs& inputs, ContainOutputs& expected, BehaveRun& behaveRun)
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
    behaveRun.contain.setAttackDistance(inputs.attackDistance, LengthUnits::Chains);
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
    observedFinalFireLineLength = behaveRun.contain.getFinalFireLineLength(LengthUnits::Chains);
    reportTestResult(row, testInfo, testName, observedFinalFireLineLength, expected.fireLineLength, error_tolerance);

    testName = "Test observed perimeter at initial attack";
    observedPerimeterAtInitialAttack = behaveRun.contain.getPerimiterAtInitialAttack(LengthUnits::Chains);
    reportTestResult(row, testInfo, testName, observedPerimeterAtInitialAttack, expected.perimeterAtInitialAttack, error_tolerance);

    testName = "Test observed perimeter at containment";
    observedPerimeterAtContainment = behaveRun.contain.getPerimeterAtContainment(LengthUnits::Chains);
    reportTestResult(row, testInfo, testName, observedPerimeterAtContainment, expected.perimeterAtContainment, error_tolerance);

    testName = "Test observed fire size at initial attack";
    observedFireSizeAtInitialAttack = behaveRun.contain.getFireSizeAtInitialAttack(AreaUnits::Acres);
    reportTestResult(row, testInfo, testName, observedFireSizeAtInitialAttack, expected.fireSizeAtInitialAttack, error_tolerance);

    testName = "Test observed final fire size";
    observedFinalFireSize = behaveRun.contain.getFinalFireSize(AreaUnits::Acres);
    reportTestResult(row, testInfo, testName, observedFinalFireSize, expected.fireSize, error_tolerance);

    testName = "Test observed final containment area";
    observedFinalContainmentArea = behaveRun.contain.getFinalContainmentArea(AreaUnits::Acres);
    reportTestResult(row, testInfo, testName, observedFinalContainmentArea, expected.containmentArea, error_tolerance);

    testName = "Test observed final time since report";
    observedFinalTimeSinceReport = behaveRun.contain.getFinalTimeSinceReport(TimeUnits::Minutes);
    reportTestResult(row, testInfo, testName, observedFinalTimeSinceReport, expected.timeSinceReport, error_tolerance);

    testName = "Test observed containment status";
    observedContainmentStatus = behaveRun.contain.getContainmentStatus();
    reportTestResult(row, testInfo, testName, observedContainmentStatus, expected.containmentStatus, error_tolerance);

    std::cout << "Finished testing Contain module\n\n";
}

void printCSVData(std::vector<std::string> csvHeaders,
                  std::vector<std::map<std::string, std::string>> csvStringRows,
                  std::vector<std::map<std::string, double>> csvDoubleRows) {
  // Print out our table
  for (const std::string & value : csvHeaders) {
    std::cout << std::setw(10) << value;
    std::cout << "\t";
  }
  std::cout << "\n";

  // Print out our table
  for (int i = 0; i < csvStringRows.size(); i++) {
    for (std::string header : csvHeaders) {
      std::map<std::string, double> doubleRow = csvDoubleRows[i];
      std::map<std::string, std::string> strRow = csvStringRows[i];

      if (doubleRow.count(header)) {
        std::cout << std::setw(10) << doubleRow[header];
      } else if (strRow.count(header)) {
        std::cout << std::setw(10) << strRow[header];
      }
      std::cout << "\t";
    }
    std::cout << "\n";
  }
}

int parseCSVFile(std::string filename,
                 std::vector<std::string> & csvHeaders,
                 std::vector<std::map<std::string, std::string>> & csvStringRows,
                 std::vector<std::map<std::string, double>> & csvDoubleRows) {

  std::ifstream input{filename};

  if (!input.is_open()) {
    std::cerr << "Couldn't read file: " << filename << "\n";
    return 1; 
  }

  // Parse header row
  std::string line;
  std::getline(input, line);
  std::istringstream ss(std::move(line));
  std::vector<std::string> headerRow;
  for (std::string value; std::getline(ss, value, ',');) {
    csvHeaders.push_back(std::move(value));
  }

  // Parse value rows
  int rowIndex = 0;
  for (std::string line; std::getline(input, line);) {
    std::istringstream ss(std::move(line));
    std::map<std::string, std::string> strmap;
    std::map<std::string, double> doublemap;
    int colIndex = 0;
    // std::getline can split on other characters, here we use ','
    for (std::string value; std::getline(ss, value, ',');) {
      std::string header = csvHeaders[colIndex];
      if (beginsWithNumber(value)) {
        doublemap[header] = str2double(value);
      } else {
        strmap[header] = value;
      }
      colIndex++;
    }

    csvStringRows.push_back(strmap);
    csvDoubleRows.push_back(doublemap);
    rowIndex++;
  }

  return 0; 
}
