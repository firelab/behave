#define BOOST_TEST_MODULE BehaveTest

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "fuelModelSet.h"
#include "behaveRun.h"
#include <vector>

// Define the error tolerance for double values
static const double ERROR_TOLERANCE = 1e-05;

struct BehaveTest
{
    FuelModelSet fuelModelSet;
    BehaveRun behaveTest;

    BehaveTest()
        : behaveTest(fuelModelSet)
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
    int fuelModelNumber = 124; // fuel model gs4(124) chosen as it is dynamic and has all moisture classes
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

BOOST_FIXTURE_TEST_SUITE(BehaveRunTest, BehaveTest)

// Setup low moisture content test case
BOOST_AUTO_TEST_CASE(singleFuelModelTest)
{
    // Observed and expected output
    double observedSpreadRate = 0.0;
    double expectedSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveTest);

    // Test upslope oriented mode, 20 foot uplsope wind 
    behaveTest.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveTest.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    observedSpreadRate = behaveTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode,  20 foot wind cross-slope left to right (90 degrees)
    behaveTest.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveTest.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveTest.setWindDirection(90);
    observedSpreadRate = behaveTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 7.091665;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, 20 foot North wind, zero aspect
    behaveTest.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveTest.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveTest.setWindDirection(0);
    observedSpreadRate = behaveTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, 20 foot North-East wind (45 degree), 270 degree aspect
    behaveTest.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveTest.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveTest.setAspect(270);
    behaveTest.setWindDirection(45);
    observedSpreadRate = behaveTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 5.259449;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    double observedA = 0;
    double observedB = 0;
    double observedC = 0;
    double expectedA = 7.461858;
    double expectedB = 17.824253;
    double expectedC = 16.187176;

    // Test fire elliptical dimensions a, b and c (direct mid-flame, upslope mode)
    setSurfaceInputsForGS4LowMoistureScenario(behaveTest);
    behaveTest.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveTest.setWindHeightInputMode(WindHeightInputMode::DIRECT_MIDFLAME);
    behaveTest.calculateSurfaceFireForwardSpreadRate();
    observedA = behaveTest.getEllipticalA();
    BOOST_CHECK_CLOSE(observedA, expectedA, ERROR_TOLERANCE);
    observedB = behaveTest.getEllipticalB();
    BOOST_CHECK_CLOSE(observedB, expectedB, ERROR_TOLERANCE);
    observedC = behaveTest.getEllipticalC();
    BOOST_CHECK_CLOSE(observedC, expectedC, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(crownModuleTest)
{
    double canopyBaseHeight = 6; 
    double canopyBulkDensity = 0.03;
    double foliarMoisture = 120;

    setSurfaceInputsForGS4LowMoistureScenario(behaveTest);
    behaveTest.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);

    double observedCrownROS = 0;
    double expectedCrownROS = 10.259921;
    observedCrownROS = behaveTest.calculateCrownFireSpreadRate();
    BOOST_CHECK_CLOSE(observedCrownROS, expectedCrownROS , ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_SUITE_END()

#ifndef NDEBUG
BOOST_AUTO_TEST_CASE(waitInDebug)
{
    BOOST_CHECK_CLOSE(0.0, 0.0, ERROR_TOLERANCE);
    // Make Visual Studio wait while in debug mode
    std::cout << "Press Enter to continue . . .";
    std::cin.get();
}
#endif
