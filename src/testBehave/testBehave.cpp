#define BOOST_TEST_MODULE BehaveTest

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "fuelModelSet.h"
#include "behaveRun.h"

static const double ERROR_TOLERANCE = 1e-05;

struct SingleFuelModelTest
{
    FuelModelSet fuelModelSet;
    BehaveRun behaveSingleFuelModelTest;

    SingleFuelModelTest()
        : behaveSingleFuelModelTest(fuelModelSet)
    {
        


    }
};

BOOST_FIXTURE_TEST_SUITE(BehaveTest, SingleFuelModelTest)

BOOST_AUTO_TEST_CASE(lowMoistureContent)
{
    int fuelModelNumber = 1;
    double moistureOneHour = 6.0;
    double moistureTenHour = 7.0;
    double moistureHundredHour = 8.0;
    double moistureLiveHerbaceous = 60.0;
    double moistureLiveWoody = 90.0;
    double windSpeed = 5.0;
    double windDirection = 0;
    double slope = 30.0;
    double aspect = 0;
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;

    behaveSingleFuelModelTest.updateSurfaceInputs(fuelModelNumber, moistureOneHour, 
        moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, 
        WindHeightInputMode::DIRECT_MIDFLAME, windSpeed, windDirection, slope, aspect, 
        canopyCover, canopyHeight, crownRatio);

    BOOST_CHECK_CLOSE(behaveSingleFuelModelTest.calculateSurfaceFireForwardSpreadRate(), 109.394614, ERROR_TOLERANCE);

    // Make Visual Studio wait while in debug mode
#ifndef NDEBUG
    std::cout << "Press Enter to continue";
    std::cin.get();
#endif
}

BOOST_AUTO_TEST_SUITE_END()
