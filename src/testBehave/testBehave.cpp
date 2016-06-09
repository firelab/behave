#define BOOST_TEST_MODULE BehaveTest

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "fuelModelSet.h"
#include "behaveRun.h"

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

struct BehaveTest
{
    FuelModelSet fuelModelSet;
    BehaveRun behaveRun;

    BehaveTest()
        : behaveRun(fuelModelSet)
    {
        BOOST_TEST_MESSAGE("Setup Behave test");
    }

    ~BehaveTest()
    {
        BOOST_TEST_MESSAGE("Teardown Behave test");
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
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TWENTY_FOOT;
    double windSpeed = 5.0;
    double windDirection = 0;
    double slope = 30.0;
    double aspect = 0;
    double canopyCover = 0.50;
    double canopyHeight = 30.0;
    double crownRatio = 0.50;

    behaveRun.updateSurfaceInputs(fuelModelNumber, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody,
        windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover,
        canopyHeight, crownRatio);
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
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TWENTY_FOOT;
    TwoFuelModels::TwoFuelModelsEnum twoFuelModelsMethod = TwoFuelModels::TWO_DIMENSIONAL;
    double windSpeed = 5.0;
    double windDirection = 0;
    double firstFuelModelCoverage = 0;
    double slope = 30.0;
    double aspect = 0;
    double canopyCover = 0.50;
    double canopyHeight = 30.0;
    double crownRatio = 0.50;

    behaveRun.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, 
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, windHeightInputMode, windSpeed, windDirection,
        firstFuelModelCoverage, twoFuelModelsMethod, slope, aspect, canopyCover, canopyHeight, crownRatio);
}

BOOST_FIXTURE_TEST_SUITE(BehaveRunTest, BehaveTest)

BOOST_AUTO_TEST_CASE(singleFuelModelTest)
{
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind 
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode,  20 foot wind cross-slope left to right (90 degrees)
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindDirection(90);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 7.091665;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, 20 foot North wind, zero aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setWindDirection(0);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, 20 foot North-East wind (45 degree), 270 degree aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setAspect(270);
    behaveRun.setWindDirection(45);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 5.259449;
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

    // Test fire elliptical dimensions a, b and c (direct mid-flame, upslope mode)
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindHeightInputMode(WindHeightInputMode::DIRECT_MIDFLAME);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
   
    observedA = roundToSixDecimalPlaces(behaveRun.getEllipticalA());
    BOOST_CHECK_CLOSE(observedA, expectedA, ERROR_TOLERANCE);

    observedB = roundToSixDecimalPlaces(behaveRun.getEllipticalB());
    BOOST_CHECK_CLOSE(observedB, expectedB, ERROR_TOLERANCE);

    observedC = roundToSixDecimalPlaces(observedC = behaveRun.getEllipticalC());
    BOOST_CHECK_CLOSE(observedC, expectedC, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(directionOfInterestTest)
{
    double directionOfInterest = 0;
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot wind, direction of interest 90 degrees from upslope
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 2.465535;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode, 20 foot north wind, direction of interest 90 degrees from north, 45 degree aspect
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setAspect(45);
    behaveRun.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 1.632054;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(twoFuelModelsTest)
{
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    // Test upslope oriented mode, 20 foot wind, low moisture conditions
    setSurfaceInputsForTwoFuelModelsLowMoistureScenario(behaveRun);

    // Do runs for first fuel model coverage 0-100 with step size 10
    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(0);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(10);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 10.446373;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(20);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 12.112509;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(30);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 13.825904;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(40);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 15.532700;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(50);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 17.173897;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(60);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 18.685358;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(70);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 19.997806;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(80);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 21.036826;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(90);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 21.722861;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.setTwoFuelModelsFirstFuelModelCoverage(100);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSurfaceFireSpreadRate = 21.971217;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(crownModuleTest)
{
    double canopyHeight = 30;
    double canopyBaseHeight = 6; 
    double canopyBulkDensity = 0.03;
    double foliarMoisture = 120;
    double observedCrownFireSpreadRate = 0;
    double expectedCrownFireSpreadRate = 0;
    int expectedFireType = (int)FireType::SURFACE;
    int observedFireType = (int)FireType::SURFACE;

    // Test crown fire spread rate
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.doCrownRun();
    expectedCrownFireSpreadRate = 10.259921;
    observedCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.getCrownFireSpreadRate());
    BOOST_CHECK_CLOSE(observedCrownFireSpreadRate, expectedCrownFireSpreadRate, ERROR_TOLERANCE);

    // Test fire type, Surface fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.setMoistureOneHour(20);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::SURFACE;
    observedFireType = (int)behaveRun.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Torching fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::TORCHING;
    observedFireType = (int)behaveRun.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Crowning fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
    behaveRun.setWindSpeed(10);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::CROWNING;
    observedFireType = (int)behaveRun.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Conditional crown fire expected
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    canopyHeight = 60;
    behaveRun.setCanopyHeight(canopyHeight);
    canopyBaseHeight = 30;
    canopyBulkDensity = 0.06;
    behaveRun.setWindSpeed(5);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
    behaveRun.doSurfaceRunInDirectionOfMaxSpread();
    behaveRun.doCrownRun();
    expectedFireType = (int)FireType::CONDITIONAL_CROWN_FIRE;
    observedFireType = (int)behaveRun.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);
}

BOOST_AUTO_TEST_SUITE_END()

#ifndef NDEBUG
BOOST_AUTO_TEST_CASE(waitInDebug)
{
    BOOST_CHECK_EQUAL(0, 0);
    // Make Visual Studio wait while in debug mode
    std::cout << "Press Enter to continue . . .";
    std::cin.get();
}
#endif
