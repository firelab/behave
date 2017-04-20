#define BOOST_TEST_MODULE BehaveTest

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "fuelModelSet.h"
#include "behaveVector.h"

// Define the error tolerance for double values
static const double ERROR_TOLERANCE = 1e-07;

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

struct BehaveRunTest
{
    FuelModelSet fuelModelSet;
    BehaveRun behaveRun;

    BehaveRunTest()
        : behaveRun(fuelModelSet)
    {
        BOOST_TEST_MESSAGE("Setup BehaveRun test");
    }

    ~BehaveRunTest()
    {
        BOOST_TEST_MESSAGE("Teardown BehaveRun test\n");
    }
};

struct BehaveVectorTest
{
    BehaveVector behaveVector;

    BehaveVectorTest()
    {
        BOOST_TEST_MESSAGE("Setup BehaveVector test");
    }

    ~BehaveVectorTest()
    {
        BOOST_TEST_MESSAGE("Teardown BehaveVector test\n");
    }
};

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
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::PERCENT;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TWENTY_FOOT;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RELATIVE_TO_NORTH;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::PERCENT;
    double aspect = 0;
    double canopyCover = 50;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::FEET;
    double crownRatio = 0.50;

    behaveRun.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, canopyHeight, canopyHeightUnits, crownRatio);
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
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::PERCENT;
    TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod = TwoFuelModels::TWO_DIMENSIONAL;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TWENTY_FOOT;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RELATIVE_TO_NORTH;
    double firstFuelModelCoverage = 0;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::PERCENT;
    double aspect = 0;
    double canopyCover = 50;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::FEET;
    double crownRatio = 0.50;

    behaveRun.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, 
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, twoFuelModelsMethod, slope, slopeUnits, aspect, canopyCover, 
        canopyHeight, canopyHeightUnits, crownRatio);
}

BOOST_FIXTURE_TEST_SUITE(BehaveRunTestSuite, BehaveRunTest)

BOOST_AUTO_TEST_CASE(singleFuelModelTest)
{
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;

    // Test North oriented mode, 42 degree wind, 20 foot wind , 30 degree slope
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setSlope(30, SlopeUnits::DEGREES);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setWindSpeed(5, windSpeedUnits);
    behaveRun.setWindDirection(45);
    behaveRun.setAspect(95);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 19.677584;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode, 20 foot uplsope wind
    behaveRun.setFuelModelNumber(124);
    behaveRun.setWindSpeed(5, windSpeedUnits);
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindDirection(0);
    behaveRun.setSlope(30, SlopeUnits::PERCENT);
    behaveRun.setAspect(0);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode,  20 foot wind cross-slope left to right (90 degrees)
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindDirection(90);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 7.091665;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot North wind, zero aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setWindDirection(0);
    behaveRun.setAspect(0);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot North-East wind (45 degree), 215 degree aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setAspect(215);
    behaveRun.setWindDirection(45);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 4.113265;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot 45 degree wind, 95 degree aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setAspect(5);
    behaveRun.setWindDirection(45);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.503960;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test Non-Burnable Fuel
    behaveRun.setFuelModelNumber(91);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 0.0;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(ellipticalDimensionTest)
{
    double observedA = 0;
    double observedB = 0;
    double observedC = 0;
    double expectedA = 7.461858;
    double expectedB = 17.824253;
    double expectedC = 16.187176;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    SpeedUnits::SpeedUnitsEnum spreadRateUnits = SpeedUnits::CHAINS_PER_HOUR;
    // Test fire elliptical dimensions a, b and c (direct mid-flame, upslope mode)
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.setWindHeightInputMode(WindHeightInputMode::DIRECT_MIDFLAME);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
   
    observedA = roundToSixDecimalPlaces(behaveRun.getEllipticalA(spreadRateUnits));
    BOOST_CHECK_CLOSE(observedA, expectedA, ERROR_TOLERANCE);

    observedB = roundToSixDecimalPlaces(behaveRun.getEllipticalB(spreadRateUnits));
    BOOST_CHECK_CLOSE(observedB, expectedB, ERROR_TOLERANCE);

    observedC = roundToSixDecimalPlaces(observedC = behaveRun.getEllipticalC(spreadRateUnits));
    BOOST_CHECK_CLOSE(observedC, expectedC, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(directionOfInterestTest)
{
    double directionOfInterest = 0;
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
  
    // Test upslope oriented mode, 20 foot wind, direction of interest 90 degrees from upslope, 45 wind
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindDirection(45);
 
    behaveRun.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 3.016440;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot north wind, direction of interest 90 degrees from north, 45 degree aspect
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setWindDirection(0);
    behaveRun.setAspect(45);
    behaveRun.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 1.803660;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot 135 degree wind, direction of interest 90 degrees from north, 45 degree aspect
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setWindDirection(135);
    behaveRun.setAspect(45);
    behaveRun.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 1.395287;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(firelineIntensityTest)
{
    double observedFirelineIntensity = 0.0;
    double expectedFirelineIntensity = 598.339039;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind 
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
  
    observedFirelineIntensity = behaveRun.getSurfaceFirelineIntensity();
    BOOST_CHECK_CLOSE(observedFirelineIntensity, expectedFirelineIntensity, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(twoFuelModelsTest)
{
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    // Test upslope oriented mode, 20 foot wind, low moisture conditions
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    setSurfaceInputsForTwoFuelModelsLowMoistureScenario(behaveRun);

    // Do runs for first fuel model coverage 0-100 with step size 10
    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(0);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(10);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 10.446373;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(20);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 12.112509;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(30);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 13.825904;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(40);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 15.532700;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(50);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 17.173897;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(60);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 18.685358;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(70);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 19.997806;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(80);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 21.036826;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(90);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 21.722861;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(100);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 21.971217;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(crownModuleTest)
{
    double canopyHeight = 30;
    double canopyBaseHeight = 6; 
    double canopyBulkDensity = 0.03;
    double moistureFoliar = 120;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;

    double observedCrownFireSpreadRate = 0;
    double expectedCrownFireSpreadRate = 0;
    double observedCrownFlameLength = 0;
    double expectedCrownFlameLength = 0;
    double observedCrownFirelineIntensity = 0;
    double expectedCrownFirelineIntensity = 0;
    int expectedFireType = (int)FireType::SURFACE;
    int observedFireType = (int)FireType::SURFACE;

    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);

    // Test crown fire spread rate, flame length, intensity
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, moistureFoliar, MoistureUnits::PERCENT);
    behaveRun.doCrownRun();
    expectedCrownFireSpreadRate = 10.259921;
    observedCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getCrownFireSpreadRate());
    BOOST_CHECK_CLOSE(observedCrownFireSpreadRate, expectedCrownFireSpreadRate, ERROR_TOLERANCE);
    expectedCrownFlameLength = 29.320577;
    observedCrownFlameLength = roundToSixDecimalPlaces(behaveRun.getCrownFlameLength());
    BOOST_CHECK_CLOSE(observedCrownFireSpreadRate, expectedCrownFireSpreadRate, ERROR_TOLERANCE);
    expectedCrownFirelineIntensity = 1775.061222;
    observedCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.getCrownFirelineIntensity());
    BOOST_CHECK_CLOSE(observedCrownFirelineIntensity, expectedCrownFirelineIntensity, ERROR_TOLERANCE);

    // Test fire type, Surface fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.setMoistureOneHour(20, MoistureUnits::PERCENT);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, moistureFoliar, MoistureUnits::PERCENT);
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::SURFACE;
    observedFireType = (int)behaveRun.getCrownFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Torching fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, moistureFoliar, MoistureUnits::PERCENT);
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::TORCHING;
    observedFireType = (int)behaveRun.getCrownFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Crowning fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.setWindSpeed(10, windSpeedUnits);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, moistureFoliar, MoistureUnits::PERCENT);
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::CROWNING;
    observedFireType = (int)behaveRun.getCrownFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Conditional crown fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    canopyHeight = 60;
    behaveRun.setCanopyHeight(canopyHeight, LengthUnits::FEET);
    canopyBaseHeight = 30;
    canopyBulkDensity = 0.06;
    behaveRun.setWindSpeed(5, windSpeedUnits);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, moistureFoliar, MoistureUnits::PERCENT);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::CONDITIONAL_CROWN_FIRE;
    observedFireType = (int)behaveRun.getCrownFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);
}

BOOST_AUTO_TEST_CASE(spotModuleTest)
{
    double flameLength = 0.0;
    double expectedSpottingDistance = 0.0;
    double observedSpottingDistance = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    flameLength = behaveRun.getSurfaceFlameLength(LengthUnits::FEET);

    SpotFireLocation::SpotFireLocationEnum location = SpotFireLocation::RIDGE_TOP;
    double ridgeToValleyDistance = 1.0;
    double ridgeToValleyElevation = 2000.0;
    double downwindCoverHeight = 30.0;
    double windSpeedAtTwentyFeet = 5.0;
    double burningPileflameHeight = 5.0;
    double torchingTrees = 15;
    double DBH = 20;
    double treeHeight = 30;
    SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies = SpotTreeSpecies::ENGELMANN_SPRUCE;
   
	// Test spotting distance from a burning pile
	expectedSpottingDistance = 0.021330;
	behaveRun.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight, 
		burningPileflameHeight, windSpeedAtTwentyFeet);
	behaveRun.calculateSpottingDistanceFromBurningPile();
	observedSpottingDistance = roundToSixDecimalPlaces(behaveRun.getMaxSpottingDistanceFromBurningPile());
	BOOST_CHECK_CLOSE(observedSpottingDistance, expectedSpottingDistance, ERROR_TOLERANCE);

    // Test spotting distance from surface fire
    expectedSpottingDistance = 0.164401;
	behaveRun.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight, 
		flameLength, windSpeedAtTwentyFeet);
    behaveRun.calculateSpottingDistanceFromSurfaceFire();
    observedSpottingDistance = roundToSixDecimalPlaces(behaveRun.getMaxSpottingDistanceFromSurfaceFire());
    BOOST_CHECK_CLOSE(observedSpottingDistance, expectedSpottingDistance, ERROR_TOLERANCE);
    
    // Test spotting distance from torching trees
    expectedSpottingDistance = 0.222396;
	behaveRun.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight, 
		torchingTrees, DBH, treeHeight, treeSpecies, windSpeedAtTwentyFeet);
    behaveRun.calculateSpottingDistanceFromTorchingTrees();
    observedSpottingDistance = roundToSixDecimalPlaces(behaveRun.getMaxSpottingDistanceFromTorchingTrees());
    BOOST_CHECK_CLOSE(observedSpottingDistance, expectedSpottingDistance, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(speedUnitConversionTest)
{
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind
    behaveRun.setFuelModelNumber(124);
    behaveRun.setWindSpeed(5, windSpeedUnits);
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindDirection(0);
    behaveRun.setSlope(30, SlopeUnits::PERCENT);
    behaveRun.setAspect(0);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();

    // Test surface spread rate in chains per hour
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in feet per minute
    expectedSurfaceFireSpreadRate = 9.763838;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::FEET_PER_MINUTE));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in kilometers per hour
    expectedSurfaceFireSpreadRate = 0.178561;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::KILOMETERS_PER_HOUR));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in meters per minute
    expectedSurfaceFireSpreadRate = 2.976018;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::METERS_PER_MINUTE));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in meters per second
    expectedSurfaceFireSpreadRate = 0.049600;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::METERS_PER_SECOND));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in miles per hour
    expectedSurfaceFireSpreadRate = 0.110953;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate(SpeedUnits::MILES_PER_HOUR));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_SUITE_END()  // End BehaveRunTestSuite

BOOST_FIXTURE_TEST_SUITE(BehaveVectorTestSuite, BehaveVectorTest)

BOOST_AUTO_TEST_CASE(behaveVectorElementIndependenceTest)
{
    double expectedWindSpeed = 0;
    double observedWindSpeed = 0;
    double expectedSurfaceFireSpreadRate = 0;
    double observedSurfaceFireSpreadRate = 0;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MILES_PER_HOUR;

    // Make sure that changes in one behaveVector's elements surfaceInputs do not affect another's surfaceInputs
    setSurfaceInputsForGS4LowMoistureScenario(behaveVector[0]);
    behaveVector[0].setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    setSurfaceInputsForGS4LowMoistureScenario(behaveVector[1]);
    behaveVector[1].setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);

    // Change behaveVector[1]'s surfaceInputs
    behaveVector[1].setWindSpeed(10, windSpeedUnits);
  
    // Check that behaveVector[0]'s surfaceInputs is unchanged
    expectedWindSpeed = 5;
    observedWindSpeed = behaveVector[0].getWindSpeed(SpeedUnits::MILES_PER_HOUR);
    BOOST_CHECK_CLOSE(observedWindSpeed, expectedWindSpeed, ERROR_TOLERANCE);

    // Check that behaveVector[1]'s surfaceInputs has been changed correctly
    expectedWindSpeed = 10;
    observedWindSpeed = behaveVector[1].getWindSpeed(SpeedUnits::MILES_PER_HOUR);
    BOOST_CHECK_CLOSE(observedWindSpeed, expectedWindSpeed, ERROR_TOLERANCE);

    // Do a surface run in behaveVector[0] and check that surface outputs are correct
    behaveVector[0].doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveVector[0].getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Do a surface run in behaveVector[1] and check that surface outputs are correct
    behaveVector[1].doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveVector[1].getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 12.722191;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Check that behaveVector[0]'s surface outputs are unchanged
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveVector[0].getSurfaceFireSpreadRate(SpeedUnits::CHAINS_PER_HOUR));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_SUITE_END() // End BehaveVectorTestSuite

#ifndef NDEBUG
BOOST_AUTO_TEST_CASE(waitInDebug)
{
    BOOST_CHECK_EQUAL(0, 0);
    // Make Visual Studio wait while in debug mode
    std::cout << "Press Enter to continue . . .";
    std::cin.get();
}
#endif
