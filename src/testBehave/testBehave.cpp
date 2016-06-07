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

BOOST_FIXTURE_TEST_SUITE(BehaveRunTest, BehaveTest)

BOOST_AUTO_TEST_CASE(singleFuelModelTest)
{
    // Observed and expected output
    double observedSpreadRate = 0.0;
    double expectedSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind 
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.doSurfaceRun();
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode,  20 foot wind cross-slope left to right (90 degrees)
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.setWindDirection(90);
    behaveRun.doSurfaceRun();
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSpreadRate = 7.091665;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, 20 foot North wind, zero aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setWindDirection(0);
    behaveRun.doSurfaceRun();
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, 20 foot North-East wind (45 degree), 270 degree aspect
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setAspect(270);
    behaveRun.setWindDirection(45);
    behaveRun.doSurfaceRun();
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSpreadRate = 5.259449;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(ellipticalDimensionTest)
{
    double observedSpreadRate = 0.0;
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
    behaveRun.doSurfaceRun();
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());

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
    double observedSpreadRate = 0.0;
    double expectedSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind, direction of interest 90 degrees from upslope
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_UPSLOPE);
    behaveRun.doSurfaceRun(directionOfInterest);
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSpreadRate = 2.465535;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode, 20 foot uplsope wind, direction of interest 90 degrees from north, 45 degree aspect
    directionOfInterest = 90;
    behaveRun.setWindHeightInputMode(WindHeightInputMode::TWENTY_FOOT);
    behaveRun.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RELATIVE_TO_NORTH);
    behaveRun.setAspect(45);
    behaveRun.doSurfaceRun(directionOfInterest);
    observedSpreadRate = roundToSixDecimalPlaces(behaveRun.getSurfaceFireSpreadRate());
    expectedSpreadRate = 1.632054;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(crownModuleTest)
{
    double canopyBaseHeight = 6; 
    double canopyBulkDensity = 0.03;
    double foliarMoisture = 120;
    double observedCrownROS = 0;
    double expectedCrownROS = 10.259921;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);

    behaveRun.doSurfaceRun();
    behaveRun.doCrownRun();

    observedCrownROS = roundToSixDecimalPlaces(behaveRun.getCrownFireSpreadRate());
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
