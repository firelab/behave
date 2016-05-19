#define BOOST_TEST_MODULE BehaveTest

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "fuelModelSet.h"
#include "behaveRun.h"
#include <vector>

// Define the error tolerance for double values
static const double ERROR_TOLERANCE = 1e-05;

//Inputs
int fuelModelNumber = 0;
double moistureOneHour = 0.0;
double moistureTenHour = 0.0;
double moistureHundredHour = 0.0;
double moistureLiveHerbaceous = 0.0;
double moistureLiveWoody = 0.0;
WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::DIRECT_MIDFLAME;
double windSpeed = 0.0;
double windDirection = 0;
double slope = 0.0;
double aspect = 0;
double canopyCover = 0.0;
double canopyHeight = 0.0;
double crownRatio = 0.0;

// Outputs
double observedSpreadRate = 0.0;
double expectedSpreadRate = 0.0;

struct SingleFuelModelTest
{
    FuelModelSet fuelModelSet;
    BehaveRun behaveSingleFuelModelTest;

    SingleFuelModelTest()
        : behaveSingleFuelModelTest(fuelModelSet)
    {
        BOOST_TEST_MESSAGE("Setup single fuel model test");
    }

    ~SingleFuelModelTest()
    {
        BOOST_TEST_MESSAGE("Teardown single fuel model test");
    }
};

// Setup test suite
BOOST_FIXTURE_TEST_SUITE(BehaveTest, SingleFuelModelTest)

// Setup low moisture content test case
BOOST_AUTO_TEST_CASE(lowMoistureContent)
{
    //Low Moisture Inputs 
    // 5 mph twenty ft wind, 30% slope, 50% canopy cover and crown ratio, 30 ft canopy cover
    fuelModelNumber = 124;
    moistureOneHour = 6.0;
    moistureTenHour = 7.0;
    moistureHundredHour = 8.0;
    moistureLiveHerbaceous = 60.0;
    moistureLiveWoody = 90.0;
    windHeightInputMode = WindHeightInputMode::TWENTY_FOOT;
    windSpeed = 5.0;
    windDirection = 0;
    slope = 30.0;
    aspect = 0;
    canopyCover = .50;
    canopyHeight = 30.0;
    crownRatio = 0.50;

    // Outputs
    observedSpreadRate = 0.0;
    expectedSpreadRate = 0.0;

    behaveSingleFuelModelTest.updateSurfaceInputs(fuelModelNumber, moistureOneHour,
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody,
        windHeightInputMode, windSpeed, windDirection, slope, aspect, canopyCover,
        canopyHeight, crownRatio);

    // Test upslope oriented mode, uplsope wind 
    observedSpreadRate = behaveSingleFuelModelTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode,  wind cross-slope left to right (90 degrees)
    behaveSingleFuelModelTest.setWindDirection(90);
    observedSpreadRate = behaveSingleFuelModelTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 7.091665;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, North wind, zero aspect
    behaveSingleFuelModelTest.setWindAndSpreadAnglesRelativeToNorth();
    behaveSingleFuelModelTest.setWindDirection(0);
    observedSpreadRate = behaveSingleFuelModelTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Test oriented to North mode, North-East wind (45 degree), 270 degree aspect
    behaveSingleFuelModelTest.setAspect(270);
    behaveSingleFuelModelTest.setWindDirection(45);
    observedSpreadRate = behaveSingleFuelModelTest.calculateSurfaceFireForwardSpreadRate();
    expectedSpreadRate = 5.259449;
    BOOST_CHECK_CLOSE(observedSpreadRate, expectedSpreadRate, ERROR_TOLERANCE);

    // Make Visual Studio wait while in debug mode
#ifndef NDEBUG
    std::cout << "Press Enter to continue";
    std::cin.get();
#endif
}

// Teardown test suite
BOOST_AUTO_TEST_SUITE_END()
