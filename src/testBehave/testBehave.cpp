#define BOOST_TEST_MODULE BehaveTest

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "behaveRun.h"
#include "fuelModelSet.h"

// Define the error tolerance for double values
static const double ERROR_TOLERANCE = 1e-06;

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
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
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
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod = TwoFuelModelsMethod::TwoFimensional;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double firstFuelModelCoverage = 0;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, 
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, twoFuelModelsMethod, slope, slopeUnits, aspect, canopyCover, 
        canopyHeight, canopyHeightUnits, crownRatio);
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
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    double windSpeed = 5.0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToNorth;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBaseHeight = 6;
    double canopyBulkDensity = 0.03;
    
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, canopyHeight, canopyHeightUnits,
        crownRatio, canopyBaseHeight, canopyBulkDensity);
}

BOOST_FIXTURE_TEST_SUITE(BehaveRunTestSuite, BehaveRunTest)

BOOST_AUTO_TEST_CASE(singleFuelModelTest)
{
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;

    // Test North oriented mode, 45 degree wind, 5 mph 20 foot wind, 30 degree slope
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
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode, 5 mph 20 foot uplsope wind
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
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test upslope oriented mode, 5 mph 20 foot wind cross-slope left to right (90 degrees)
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(90);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 7.091665;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot North wind, zero aspect
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot North-East wind (45 degree), 215 degree aspect
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setAspect(215);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 4.113265;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot 45 degree wind, 95 degree aspect
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setAspect(5);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.503960;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test Non-Burnable Fuel
    behaveRun.surface.setFuelModelNumber(91);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 0.0;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(lengthToWidthRatioTest)
{
    // Observed and expected output
    double observedLengthToWidthRatio = 0.0;
    double expectedLengthToWidthRatio = 0.0;

    double observedCrownLengthToWidthRatio = 0;
    double expectedCrownLengthToWidthRatio = 0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    // Test North oriented mode, north wind, 0 mph 20 foot wind, 0 degree aspect, 0 degree slope
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(0, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(0, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 1.0;
    BOOST_CHECK_CLOSE(observedLengthToWidthRatio, expectedLengthToWidthRatio, ERROR_TOLERANCE);

    // Test North oriented mode, 45 degree wind, 5 mph 20 foot wind, 95 degree aspect, 30 degree slope
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.setAspect(95);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 1.897769;
    BOOST_CHECK_CLOSE(observedLengthToWidthRatio, expectedLengthToWidthRatio, ERROR_TOLERANCE);

    // Test North oriented mode, 45 degree wind, 15 mph 20 foot wind, 95 degree aspect, 30 degree slope
    behaveRun.surface.setWindHeightInputMode(windHeightInputMode);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindSpeed(15, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.setAspect(95);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.surface.getFireLengthToWidthRatio());
    expectedLengthToWidthRatio = 2.142422;
    BOOST_CHECK_CLOSE(observedLengthToWidthRatio, expectedLengthToWidthRatio, ERROR_TOLERANCE);

    // Test Crown fire length-to-width-ratio
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRun();
    expectedCrownLengthToWidthRatio = 1.625;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    BOOST_CHECK_CLOSE(expectedCrownLengthToWidthRatio, observedCrownLengthToWidthRatio, ERROR_TOLERANCE);

    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRun();
    expectedCrownLengthToWidthRatio = 2.25;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    BOOST_CHECK_CLOSE(expectedCrownLengthToWidthRatio, observedCrownLengthToWidthRatio, ERROR_TOLERANCE);

    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(15, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRun();
    expectedCrownLengthToWidthRatio = 2.875;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    BOOST_CHECK_CLOSE(expectedCrownLengthToWidthRatio, observedCrownLengthToWidthRatio, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(ellipticalDimensionTest)
{
    double observedA = 0;
    double observedB = 0;
    double observedC = 0;
    double expectedA = 7.4618580;
    double expectedB = 17.824253;
    double expectedC = 16.187176;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Chains;
    // Test fire elliptical dimensions a, b and c (direct mid-flame, upslope mode, 1 hour elapsed time)
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::DirectMidflame);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

    observedA = roundToSixDecimalPlaces(behaveRun.surface.getEllipticalA(lengthUnits, 1, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(observedA, expectedA, ERROR_TOLERANCE);

    observedB = roundToSixDecimalPlaces(behaveRun.surface.getEllipticalB(lengthUnits, 1, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(observedB, expectedB, ERROR_TOLERANCE);

    observedC = roundToSixDecimalPlaces(observedC = behaveRun.surface.getEllipticalC(lengthUnits, 1, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(observedC, expectedC, ERROR_TOLERANCE);

    double expectedArea = 41.783821;
    double observedArea = 0.0;

    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::Acres, 1, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(observedArea, expectedArea, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(directionOfInterestTest)
{
    double directionOfInterest = 0;
    double observedSpreadRateInDirectionOfInterest = 0.0;
    double expectedSpreadRateInDirectionOfInterest = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
  
    // Test upslope oriented mode, 20 foot wind, direction of interest 90 degrees from upslope, 45 degree wind
    directionOfInterest = 90;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(45);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 3.016440;
    BOOST_CHECK_CLOSE(observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, ERROR_TOLERANCE);

    // Test upslope oriented mode, 20 foot wind, direction of interest 160 degrees from upslope, 290 degree wind
    directionOfInterest = 160;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(290);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.399262;
    BOOST_CHECK_CLOSE(observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, ERROR_TOLERANCE);

    // Test upslope oriented mode, 20 foot wind, direction of interest 215 degrees from upslope, 215 degree wind
    directionOfInterest = 215;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(215);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.648579;
    BOOST_CHECK_CLOSE(observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, ERROR_TOLERANCE);
    
    // Test North oriented mode, 20 foot 135 degree wind, direction of interest 30 degrees from north, 263 degree aspect
    directionOfInterest = 30;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(280);
    behaveRun.surface.setAspect(135);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 3.504961;
    BOOST_CHECK_CLOSE(observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot north wind, direction of interest 90 degrees from north, 45 degree aspect
    directionOfInterest = 90;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setAspect(45);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.803660;
    BOOST_CHECK_CLOSE(observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, ERROR_TOLERANCE);

    // Test North oriented mode, 20 foot 135 degree wind, direction of interest 285 degrees from north, 263 degree aspect
    directionOfInterest = 285;
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setWindDirection(280);
    behaveRun.surface.setAspect(263);
    behaveRun.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest);
    observedSpreadRateInDirectionOfInterest = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRateInDirectionOfInterest(SpeedUnits::ChainsPerHour));
    expectedSpreadRateInDirectionOfInterest = 1.452856;
    BOOST_CHECK_CLOSE(observedSpreadRateInDirectionOfInterest, expectedSpreadRateInDirectionOfInterest, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(firelineIntensityTest)
{
    double observedFirelineIntensity = 0.0;
    double expectedFirelineIntensity = 598.339039;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind 
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedFirelineIntensity = behaveRun.surface.getFirelineIntensity();
    BOOST_CHECK_CLOSE(observedFirelineIntensity, expectedFirelineIntensity, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(twoFuelModelsTest)
{
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    // Test upslope oriented mode, 20 foot wind, low moisture conditions
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    setSurfaceInputsForTwoFuelModelsLowMoistureScenario(behaveRun);

    // Do runs for first fuel model coverage 0-100 with step size 10
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(10);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 10.446373;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(20);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 12.112509;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(30);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 13.825904;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(40);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 15.532700;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(50);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 17.173897;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(60);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 18.685358;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(70);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 19.997806;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(80);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.036826;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(90);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.722861;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(100);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.971217;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(crownModuleTest)
{
    double canopyHeight = 30;
    double canopyBaseHeight = 6; 
    double canopyBulkDensity = 0.03;
    
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    double observedCrownFireSpreadRate = 0;
    double expectedCrownFireSpreadRate = 0;
    double observedCrownFlameLength = 0;
    double expectedCrownFlameLength = 0;
    double observedCrownFirelineIntensity = 0;
    double expectedCrownFirelineIntensity = 0;
    int expectedFireType = (int)FireType::Surface;
    int observedFireType = (int)FireType::Surface;

    behaveRun.crown.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);

    // Test crown fire spread rate, flame length, intensity
    //setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    //behaveRun.crown.doSurfaceRunInDirectionOfMaxSpread();
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRun();
    expectedCrownFireSpreadRate = 10.259921;
    observedCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireSpreadRate(SpeedUnits::ChainsPerHour));
    BOOST_CHECK_CLOSE(observedCrownFireSpreadRate, expectedCrownFireSpreadRate, ERROR_TOLERANCE);

    expectedCrownFlameLength = 29.320557;
    observedCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getCrownFlameLength());
    BOOST_CHECK_CLOSE(observedCrownFlameLength, expectedCrownFlameLength, ERROR_TOLERANCE);

    expectedCrownFirelineIntensity = 1775.061222;
    observedCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getCrownFirelineIntensity());
    BOOST_CHECK_CLOSE(observedCrownFirelineIntensity, expectedCrownFirelineIntensity, ERROR_TOLERANCE);

    // Test fire type, Surface fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setMoistureOneHour(20, MoistureUnits::Percent);
    behaveRun.crown.doCrownRun();
    expectedFireType = (int)FireType::Surface;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Torching fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRun();
    expectedFireType = (int)FireType::Torching;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Crowning fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRun();
    expectedFireType = (int)FireType::Crowning;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Conditional crown fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    canopyHeight = 60;
    behaveRun.crown.setCanopyHeight(canopyHeight, LengthUnits::Feet);
    canopyBaseHeight = 30;
    behaveRun.crown.setCanopyBaseHeight(canopyBaseHeight);
    canopyBulkDensity = 0.06;
    behaveRun.crown.setCanopyBulkDensity(0.06);
    behaveRun.crown.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRun();
    expectedFireType = (int)FireType::ConditionalCrownFire;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);
}

BOOST_AUTO_TEST_CASE(spotModuleTest)
{
    double flameLength = 0.0;
    double expectedSpottingDistance = 0.0;
    double observedSpottingDistance = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    flameLength = behaveRun.surface.getFlameLength(LengthUnits::Feet);

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
	behaveRun.spot.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight, 
		burningPileflameHeight, windSpeedAtTwentyFeet);
	behaveRun.spot.calculateSpottingDistanceFromBurningPile();
	observedSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxSpottingDistanceFromBurningPile());
	BOOST_CHECK_CLOSE(observedSpottingDistance, expectedSpottingDistance, ERROR_TOLERANCE);

    // Test spotting distance from surface fire
    expectedSpottingDistance = 0.164401;
	behaveRun.spot.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight, 
		flameLength, windSpeedAtTwentyFeet);
    behaveRun.spot.calculateSpottingDistanceFromSurfaceFire();
    observedSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxSpottingDistanceFromSurfaceFire());
    BOOST_CHECK_CLOSE(observedSpottingDistance, expectedSpottingDistance, ERROR_TOLERANCE);
    
    // Test spotting distance from torching trees
    expectedSpottingDistance = 0.222396;
	behaveRun.spot.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyElevation, downwindCoverHeight,
		torchingTrees, DBH, treeHeight, treeSpecies, windSpeedAtTwentyFeet);
    behaveRun.spot.calculateSpottingDistanceFromTorchingTrees();
    observedSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxSpottingDistanceFromTorchingTrees());
    BOOST_CHECK_CLOSE(observedSpottingDistance, expectedSpottingDistance, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(speedUnitConversionTest)
{
    // Observed and expected output
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind
    behaveRun.surface.setFuelModelNumber(124);
    behaveRun.surface.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.setWindDirection(0);
    behaveRun.surface.setSlope(30, SlopeUnits::Percent);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();

    // Test surface spread rate in chains per hour
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in feet per minute
    expectedSurfaceFireSpreadRate = 9.763838;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::FeetPerMinute));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in kilometers per hour
    expectedSurfaceFireSpreadRate = 0.178561;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::KilometersPerHour));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in meters per minute
    expectedSurfaceFireSpreadRate = 2.976018;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::MetersPerMinute));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in meters per second
    expectedSurfaceFireSpreadRate = 0.049600;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::MetersPerSecond));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    // Test surface spread rate in miles per hour
    expectedSurfaceFireSpreadRate = 0.110953;
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::MilesPerHour));
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(igniteModuleTest)
{
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
   
    MoistureUnits::MoistureUnitsEnum mositureUnits = MoistureUnits::Percent;
    TemperatureUnits::TemperatureUnitsEnum temperatureUnits = TemperatureUnits::Fahrenheit;
    CoverUnits::CoverUnitsEnum sunShadeUnits = CoverUnits::Percent;
    LengthUnits::LengthUnitsEnum duffDepthUnits = LengthUnits::Inches;

    expectedFirebrandIgnitionProbability = 0.54831705;
    expectedLightningIgnitionProbability = 0.39362018;

    behaveRun.ignite.updateIgniteInputs(moistureOneHour, moistureHundredHour, mositureUnits, airTemperature,
        temperatureUnits, sunShade, sunShadeUnits, fuelBedType, duffDepth, duffDepthUnits, lightningChargeType);
    observedFirebrandIgnitionProbability = behaveRun.ignite.calculateFirebrandIgnitionProbability(ProbabilityUnits::Fraction);
    BOOST_CHECK_CLOSE(observedFirebrandIgnitionProbability, expectedFirebrandIgnitionProbability, ERROR_TOLERANCE);

    observedLightningIgnitionProbability = behaveRun.ignite.calculateLightningIgnitionProbability(ProbabilityUnits::Fraction);
    BOOST_CHECK_CLOSE(expectedLightningIgnitionProbability, observedLightningIgnitionProbability, ERROR_TOLERANCE);

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
    observedFirebrandIgnitionProbability = behaveRun.ignite.calculateFirebrandIgnitionProbability(ProbabilityUnits::Percent);
    BOOST_CHECK_CLOSE(observedFirebrandIgnitionProbability, expectedFirebrandIgnitionProbability, ERROR_TOLERANCE);

    observedLightningIgnitionProbability = behaveRun.ignite.calculateLightningIgnitionProbability(ProbabilityUnits::Percent);
    BOOST_CHECK_CLOSE(expectedLightningIgnitionProbability, observedLightningIgnitionProbability, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(ContainModuleTest)
{
    double observedFinalFireLineLength = 0;
    double observedtPerimeterAtContainment = 0;
    double observedFinalFireSize = 0;
    double observedFinalContainmentArea = 0;
    double observedFinalTimeSinceReport = 0;
    ContainStatus::ContainStatusEnum observedContainmentStatus = ContainStatus::Unreported;

    double expectedFinalFireLineLength = 0;
    double expectedPerimeterAtContainment = 0;
    double expectedFinalFireSize = 0;
    double expectedFinalContainmentArea = 0;
    double expectedFinalTimeSinceReport = 0;
    ContainStatus::ContainStatusEnum expectedContainmentStatus = ContainStatus::Unreported;

    // Test case when expected result is containment
    behaveRun.contain.setAttackDistance(0);
    behaveRun.contain.setFireStartTime(0);
    behaveRun.contain.setLwRatio(3);
    behaveRun.contain.setReportRate(5);
    behaveRun.contain.setReportSize(1);
    behaveRun.contain.setTactic(ContainTactic::HeadAttack);
    behaveRun.contain.addResource(0, 20, 480, ContainFlank::LeftFlank, "test");
    behaveRun.contain.doContainRun();
    behaveRun.contain.removeAllResources();

    expectedFinalFireLineLength = 14.8329956;
    expectedPerimeterAtContainment = 14.8329956;
    expectedFinalFireSize = 1.32673918;
    expectedFinalContainmentArea = 1.32673918;
    expectedFinalTimeSinceReport = 44.5000000;
    expectedContainmentStatus = ContainStatus::Contained;

    observedFinalFireLineLength = behaveRun.contain.getFinalFireLineLength();
    observedtPerimeterAtContainment = behaveRun.contain.getPerimeterAtContainment();
    observedFinalFireSize = behaveRun.contain.getFinalFireSize();
    observedFinalContainmentArea = behaveRun.contain.getFinalContainmentArea();
    observedFinalTimeSinceReport = behaveRun.contain.getFinalTimeSinceReport();
    observedContainmentStatus = behaveRun.contain.getContainmentStatus();

    BOOST_CHECK_CLOSE(observedFinalFireLineLength, expectedFinalFireLineLength, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedtPerimeterAtContainment, expectedPerimeterAtContainment, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedFinalContainmentArea, expectedFinalContainmentArea,ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedFinalFireSize, expectedFinalFireSize, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedFinalTimeSinceReport, expectedFinalTimeSinceReport, ERROR_TOLERANCE);
    BOOST_CHECK_EQUAL(observedContainmentStatus, expectedContainmentStatus);

}

BOOST_AUTO_TEST_SUITE_END()  // End BehaveRunTestSuite

#ifndef NDEBUG
BOOST_AUTO_TEST_CASE(waitInDebug)
{
    BOOST_CHECK_EQUAL(0, 0);
    // Make Visual Studio wait while in debug mode
    std::cout << "Press Enter to continue . . .";
    std::cin.get();
}
#endif
