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
std::string trim(const std::string& str);
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

struct SurfaceTestInputs {
  int fuelModelNumber = 0;
  double moistureOneHour = 0.0;
  double moistureTenHour = 0.0;
  double moistureHundredHour = 0.0;
  double moistureLiveHerbaceous = 0.0;
  double moistureLiveWoody = 0.0;
  MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
  double windSpeed = 0.0;
  WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
  SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
  double windDirection = 0;
  WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
  double slope = 0.0;
  SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
  double aspect = 0.0;
  double canopyCover = 0.0;
  CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
  double canopyHeight = 0.0;
  LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
  double crownRatio = 0.0;
};

struct SurfaceTestOutputs {
  double spreadRate = 0.00;
  double spreadRateInDirectionOfInterest = 0.00;
  double directionOfMaxSpread = 0.00;
  double flameLength = 0.00;
  double fireLengthToWidthRatio = 0.00;
  double fireEccentricity = 0.00;
  double firelineIntensity = 0.00;
  double heatPerUnitArea = 0.00;
  double midflameWindspeed = 0.00;
  double residenceTime = 0.00;
  double reactionIntensity = 0.00;
  double ellipticalA = 0.00;
  double ellipticalB = 0.00;
  double ellipticalC = 0.00;
  double slopeFactor = 0.00;
  double bulkDensity = 0.00;
  double heatSink = 0.00;
  double firePerimeter = 0.00;
  double fireArea = 0.00;
};

// Testing Function Headers
void testSurfaceModule(int row, struct TestInfo& testInfo, SurfaceTestInputs& surfaceInputs, SurfaceTestOutputs& surfaceOutputs, BehaveRun& behaveRun);

int main(int argc, char * argv[])
{
  if (argc == 1) {
    std::cout << "Unable continue tests. Please supply CSV filename.\n" << std::endl;
    return 1;
  }

  TestInfo testInfo;
  FuelModels fuelModels;
  SurfaceTestInputs surfaceInputs;
  SurfaceTestOutputs surfaceOutputs;
  SpeciesMasterTable mortalitySpeciesTable;
  BehaveRun behaveRun(fuelModels, mortalitySpeciesTable);

  std::string csvFilename{argv[1]};

  /* Units/Enum Maps */
  std::map<std::string, LengthUnits::LengthUnitsEnum> lengthUnits{
    {"Feet", LengthUnits::Feet},
    {"Inches", LengthUnits::Inches},
    {"Millimeters", LengthUnits::Millimeters},
    {"Centimeters", LengthUnits::Centimeters},
    {"Meters", LengthUnits::Meters},
    {"Chains", LengthUnits::Chains},
    {"Miles", LengthUnits::Miles},
    {"Kilometers", LengthUnits::Kilometers}
  };

  std::map<std::string, SlopeUnits::SlopeUnitsEnum> slopeUnits{{"Degrees", SlopeUnits::Degrees},
                                                               {"Percent", SlopeUnits::Percent}};

  std::map<std::string, SpeedUnits::SpeedUnitsEnum> speedUnits{
    {"FeetPerMinute", SpeedUnits::FeetPerMinute},
    {"ChainsPerHour", SpeedUnits::ChainsPerHour},
    {"MetersPerSecond", SpeedUnits::MetersPerSecond},
    {"MetersPerMinute", SpeedUnits::MetersPerMinute},
    {"MetersPerHour", SpeedUnits::MetersPerHour},
    {"MilesPerHour", SpeedUnits::MilesPerHour},
    {"KilometersPerHour", SpeedUnits::KilometersPerHour}
  };

  std::map<std::string, CoverUnits::CoverUnitsEnum> coverUnits{{"Fraction", CoverUnits::Fraction},
                                                               {"Percent", CoverUnits::Percent}};

  std::map<std::string, MoistureUnits::MoistureUnitsEnum> moistureUnits{{"Fraction", MoistureUnits::Fraction},
                                                                        {"Percent", MoistureUnits::Percent}};

  std::map<std::string, WindHeightInputMode::WindHeightInputModeEnum> windHeightInputMode{{"DirectMidflame", WindHeightInputMode::DirectMidflame},
                                                                                          {"TwentyFoot", WindHeightInputMode::TwentyFoot},
                                                                                          {"TenMeter", WindHeightInputMode::TenMeter}};

  std::map<std::string, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum> windAndSpreadOrientationMode{
    {"RelativeToUpslope", WindAndSpreadOrientationMode::RelativeToUpslope},
    {"RelativeToNorth", WindAndSpreadOrientationMode::RelativeToNorth}
  };

  // CSV Reading
  std::vector<std::string> csvHeaders;
  std::vector<std::map<std::string, std::string>> csvStringRows;
  std::vector<std::map<std::string, double>> csvDoubleRows;

  int result = parseCSVFile(csvFilename, csvHeaders, csvStringRows, csvDoubleRows);

  if (result == 0) {
    printCSVData(csvHeaders, csvStringRows, csvDoubleRows);
  }

  std::cout << "Performing tests with: " << csvStringRows.size() << " samples.\n";

  // Perform Tests using CSV Inputs
  for (int i = 0; i < csvStringRows.size(); i++) {

    std::map<std::string, double> doubleRow = csvDoubleRows[i];
    std::map<std::string, std::string> stringRow = csvStringRows[i];

    // Set up Inputs
    surfaceInputs.fuelModelNumber = doubleRow["fuelModelNumber"];
    surfaceInputs.moistureOneHour = doubleRow["moistureOneHour"];
    surfaceInputs.moistureTenHour = doubleRow["moistureTenHour"];
    surfaceInputs.moistureHundredHour = doubleRow["moistureHundredHour"];
    surfaceInputs.moistureLiveHerbaceous = doubleRow["moistureLiveHerbaceous"];
    surfaceInputs.moistureLiveWoody = doubleRow["moistureLiveWoody"];
    surfaceInputs.moistureUnits = moistureUnits[stringRow["moistureUnits"]];
    surfaceInputs.windSpeed = doubleRow["windSpeed"];
    surfaceInputs.windHeightInputMode = windHeightInputMode[stringRow["windHeightInputMode"]];
    surfaceInputs.windSpeedUnits = speedUnits[stringRow["windSpeedUnits"]];
    surfaceInputs.windDirection = doubleRow["windDirection"];
    surfaceInputs.windAndSpreadOrientationMode = windAndSpreadOrientationMode[stringRow["windAndSpreadOrientationMode"]];
    surfaceInputs.slope = doubleRow["slope"];
    surfaceInputs.slopeUnits = slopeUnits[stringRow["slopeUnits"]];
    surfaceInputs.aspect = doubleRow["aspect"];
    surfaceInputs.canopyCover = doubleRow["canopyCover"];
    surfaceInputs.canopyCoverUnits = coverUnits[stringRow["canopyCoverUnits"]];
    surfaceInputs.canopyHeight = doubleRow["canopyHeight"];
    surfaceInputs.canopyHeightUnits = lengthUnits[stringRow["canopyHeightUnits"]];
    surfaceInputs.crownRatio = doubleRow["crownRatio"];

    // Set up Outputs
    surfaceOutputs.spreadRate = doubleRow["spreadRate"];

    testSurfaceModule(i, testInfo, surfaceInputs, surfaceOutputs, behaveRun);

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
std::string trim(const std::string& str)
{
  const std::string& whitespace = " \t\"";
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

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
  if(areClose(observed, expected, epsilon)) {
    //std::cout << testName << "\npassed successfully\n";
    testInfo.numPassed++;
  } else {
    std::cout << testName << "\n(Row: " << row << ")" << "\nfailed\nobserved value " << observed << " differs from expected value " << expected << " by more than " << epsilon << "\n\n";
    testInfo.numFailed++;
  }
}

void testSurfaceModule(int row, struct TestInfo& testInfo, SurfaceTestInputs& inputs, SurfaceTestOutputs& expected, BehaveRun& behaveRun)
{
  std::cout << "Testing Surface module\n";

  string testName = "";

  double observedSpreadRate = 0;

  // Set up inputs

  behaveRun.surface.updateSurfaceInputs(inputs.fuelModelNumber,
                                        inputs.moistureOneHour,
                                        inputs.moistureTenHour,
                                        inputs.moistureHundredHour,
                                        inputs.moistureLiveHerbaceous,
                                        inputs.moistureLiveWoody,
                                        inputs.moistureUnits,
                                        inputs.windSpeed,
                                        inputs.windSpeedUnits,
                                        inputs.windHeightInputMode,
                                        inputs.windDirection,
                                        inputs.windAndSpreadOrientationMode,
                                        inputs.slope,
                                        inputs.slopeUnits,
                                        inputs.aspect,
                                        inputs.canopyCover,
                                        inputs.canopyCoverUnits,
                                        inputs.canopyHeight,
                                        inputs.canopyHeightUnits,
                                        inputs.crownRatio);

  // Perform Run
  behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

  // Compare Results
  testName = "Test fire spread rate";
  observedSpreadRate = behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour);
  reportTestResult(row, testInfo, testName, observedSpreadRate, expected.spreadRate, error_tolerance);

  std::cout << "Finished testing Surface module\n\n";
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
        strmap[header] = trim(value);
      }
      colIndex++;
    }

    csvStringRows.push_back(strmap);
    csvDoubleRows.push_back(doublemap);
    rowIndex++;
  }

  return 0; 
}
