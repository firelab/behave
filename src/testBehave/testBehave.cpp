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
    CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode,
        slope, slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
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
    CoverUnits::CoverUnitsEnum firstFuelModelCoverageUnits = CoverUnits::Percent;
    double slope = 30.0;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    CoverUnits::CoverUnitsEnum canopyCoverUnits = CoverUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;

    behaveRun.surface.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour, 
        moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope, 
        slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio);
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
    CoverUnits::CoverUnitsEnum coverUnits = CoverUnits::Percent;
    double canopyHeight = 30.0;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBaseHeight = 6;
    double canopyBulkDensity = 0.03;
    DensityUnits::DensityUnitsEnum canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, 
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, canopyBulkDensity, canopyBulkDensityUnits);
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

    // Test Fuel Model 4, North oriented mode, 20 foot 90 degree wind, 0 degree aspect, 40 percent canopy cover
    behaveRun.surface.setFuelModelNumber(4);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);
    behaveRun.surface.setAspect(0);
    behaveRun.surface.setWindDirection(90);
    behaveRun.surface.setWindSpeed(5, SpeedUnits::MilesPerHour, WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setSlope(30, SlopeUnits::Degrees);
    behaveRun.surface.setCanopyCover(40, CoverUnits::Percent);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 46.631688;
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
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownLengthToWidthRatio = 1.625;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    BOOST_CHECK_CLOSE(expectedCrownLengthToWidthRatio, observedCrownLengthToWidthRatio, ERROR_TOLERANCE);

    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownLengthToWidthRatio = 2.25;
    observedCrownLengthToWidthRatio = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireLengthToWidthRatio());
    BOOST_CHECK_CLOSE(expectedCrownLengthToWidthRatio, observedCrownLengthToWidthRatio, ERROR_TOLERANCE);

    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(15, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
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

    double elapsedTime = 1;

    // Area
    double expectedArea = 0.0;
    double observedArea = 0.0;

    expectedArea = 41.783821;
    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::Acres, elapsedTime, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(observedArea, expectedArea, ERROR_TOLERANCE);

    expectedArea = 0.169093;
    observedArea = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFireArea(AreaUnits::SquareKilometers, elapsedTime, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(observedArea, expectedArea, ERROR_TOLERANCE);

    // Perimeter
    double expectedPerimeter = 0.0;
    double obeservedPerimeter = 0.0;

    expectedPerimeter = 82.808915;
    obeservedPerimeter = roundToSixDecimalPlaces(observedArea = behaveRun.surface.getFirePerimeter(LengthUnits::Chains, elapsedTime, TimeUnits::Hours));
    BOOST_CHECK_CLOSE(obeservedPerimeter, expectedPerimeter, ERROR_TOLERANCE);
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
    double expectedFirelineIntensity = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);

    // Test upslope oriented mode, 20 foot uplsope wind 
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedFirelineIntensity = behaveRun.surface.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond);
    expectedFirelineIntensity = 598.339039;
    BOOST_CHECK_CLOSE(observedFirelineIntensity, expectedFirelineIntensity, ERROR_TOLERANCE);

    // Test unit conversion
    observedFirelineIntensity = behaveRun.surface.getFirelineIntensity(FirelineIntensityUnits::KilowattsPerMeter);
    expectedFirelineIntensity = 2072.730450;
    BOOST_CHECK_CLOSE(observedFirelineIntensity, expectedFirelineIntensity, ERROR_TOLERANCE);

}

BOOST_AUTO_TEST_CASE(twoFuelModelsTest)
{
    double observedSurfaceFireSpreadRate = 0.0;
    double expectedSurfaceFireSpreadRate = 0.0;

    // Test upslope oriented mode, 20 foot wind, low moisture conditions
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    setSurfaceInputsForTwoFuelModelsLowMoistureScenario(behaveRun);

    CoverUnits::CoverUnitsEnum coverUnits = CoverUnits::Percent;

    // Do runs for first fuel model coverage 0-100 with step size 10
    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(0, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 8.876216;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(10, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 10.446373;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(20, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 12.112509;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(30, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 13.825904;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(40, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 15.532700;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(50, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 17.173897;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(60, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 18.685358;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(70, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 19.997806;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(80, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.036826;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(90, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.722861;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);

    behaveRun.surface.setTwoFuelModelsFirstFuelModelCoverage(100, coverUnits);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    observedSurfaceFireSpreadRate = roundToSixDecimalPlaces(behaveRun.surface.getSpreadRate(SpeedUnits::ChainsPerHour));
    expectedSurfaceFireSpreadRate = 21.971217;
    BOOST_CHECK_CLOSE(observedSurfaceFireSpreadRate, expectedSurfaceFireSpreadRate, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(crownModuleTestRothermel)
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
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedCrownFireSpreadRate = 10.259921;
    observedCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getCrownFireSpreadRate(SpeedUnits::ChainsPerHour));
    BOOST_CHECK_CLOSE(observedCrownFireSpreadRate, expectedCrownFireSpreadRate, ERROR_TOLERANCE);

    expectedCrownFlameLength = 29.320557;
    observedCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getCrownFlameLength(LengthUnits::Feet));
    BOOST_CHECK_CLOSE(observedCrownFlameLength, expectedCrownFlameLength, ERROR_TOLERANCE);

    expectedCrownFirelineIntensity = 1775.061222;
    observedCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getCrownFirelineIntensity());
    BOOST_CHECK_CLOSE(observedCrownFirelineIntensity, expectedCrownFirelineIntensity, ERROR_TOLERANCE);

    // Test fire type, Surface fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setMoistureOneHour(20, MoistureUnits::Percent);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Surface;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Torching fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Torching;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Crowning fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    behaveRun.crown.setWindSpeed(10, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::Crowning;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);

    // Test fire type, Conditional crown fire expected
    setCrownInputsLowMoistureScenario(behaveRun);
    canopyHeight = 60;
    behaveRun.crown.setCanopyHeight(canopyHeight, LengthUnits::Feet);
    canopyBaseHeight = 30;
    behaveRun.crown.setCanopyBaseHeight(canopyBaseHeight, LengthUnits::Feet);
    canopyBulkDensity = 0.06;
    behaveRun.crown.setCanopyBulkDensity(0.06, DensityUnits::PoundsPerCubicFoot);
    behaveRun.crown.setWindSpeed(5, windSpeedUnits, windHeightInputMode);
    behaveRun.crown.doCrownRunRothermel();
    expectedFireType = (int)FireType::ConditionalCrownFire;
    observedFireType = (int)behaveRun.crown.getFireType();
    BOOST_CHECK_EQUAL(observedFireType, expectedFireType);
}

BOOST_AUTO_TEST_CASE(crownModuleTestScottAndReinhardt)
{
    // Test crown fire spread rate, flame length, intensity; Crown fire expected
    double observedFinalCrownFireSpreadRate = 0;
    double expectedFinalCrownFireSpreadRate = 0;
    double observedFinalCrownFlameLength = 0;
    double expectedFinalCrownFlameLength = 0;
    double observedFinalCrownFirelineIntensity = 0;
    double expectedFinalCrownFirelineIntensity = 0;
    double observedCriticalOpenWindSpeed = 0;
    double expectedCriticalOpenWindSpeed = 0;

    int fuelModelNumber = 10;
    double moistureOneHour = 8.0;
    double moistureTenHour = 9.0;
    double moistureHundredHour = 10.0;
    double moistureLiveHerbaceous = 0.0;
    double moistureLiveWoody = 117.0;
    double moistureFoliar = 100;
    MoistureUnits::MoistureUnitsEnum moistureUnits = MoistureUnits::Percent;
    double windSpeed = 2187.226624;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::TwentyFoot;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::FeetPerMinute;
    double windDirection = 0;
    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    double slope = 20;
    SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
    double aspect = 0;
    double canopyCover = 50;
    CoverUnits::CoverUnitsEnum coverUnits = CoverUnits::Percent;
    double canopyHeight = 38.104626;
    double canopyBaseHeight = 2.952756;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    double crownRatio = 0.50;
    double canopyBulkDensity = 0.01311;
    DensityUnits::DensityUnitsEnum canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.crown.setUserProvidedWindAdjustmentFactor(0.12);
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, canopyBulkDensity, canopyBulkDensityUnits);

    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 65.221842;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::FeetPerMinute));
    BOOST_CHECK_CLOSE(observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, ERROR_TOLERANCE);

    expectedFinalCrownFlameLength = 60.744542;
    observedFinalCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getFinalFlameLength(LengthUnits::Feet));
    BOOST_CHECK_CLOSE(observedFinalCrownFlameLength, expectedFinalCrownFlameLength, ERROR_TOLERANCE);

    expectedFinalCrownFirelineIntensity = 5293.170672;
    observedFinalCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getFinalFirelineIntesity(FirelineIntensityUnits::BtusPerFootPerSecond));
    BOOST_CHECK_CLOSE(observedFinalCrownFirelineIntensity, expectedFinalCrownFirelineIntensity, ERROR_TOLERANCE);

    expectedCriticalOpenWindSpeed = 1717.916785;
    observedCriticalOpenWindSpeed = behaveRun.crown.getCriticalOpenWindSpeed(SpeedUnits::FeetPerMinute);
    BOOST_CHECK_CLOSE(observedCriticalOpenWindSpeed, expectedCriticalOpenWindSpeed, ERROR_TOLERANCE);

    // Test crown fire spread rate, flame length, intensity; Torching fire expected
    fuelModelNumber = 5;
    moistureOneHour = 5.0;
    moistureTenHour = 6.0;
    moistureHundredHour = 8.0;
    moistureLiveHerbaceous = 0.0;
    moistureLiveWoody = 117.0;
    moistureFoliar = 100;
    moistureUnits = MoistureUnits::Percent;
    windSpeed = 24.854848;
    windHeightInputMode = WindHeightInputMode::TwentyFoot;
    windSpeedUnits = SpeedUnits::MilesPerHour;
    windDirection = 0;
    windAndSpreadOrientationMode = WindAndSpreadOrientationMode::RelativeToUpslope;
    slope = 20;
    slopeUnits = SlopeUnits::Percent;
    aspect = 0;
    canopyCover = 50;
    coverUnits = CoverUnits::Percent;
    canopyHeight = 71.631562;
    canopyBaseHeight = 4.92126;
    canopyHeightUnits = LengthUnits::Feet;
    crownRatio = 0.50;
    canopyBulkDensity = 0.003746;
    canopyBulkDensityUnits = DensityUnits::PoundsPerCubicFoot;

    behaveRun.crown.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    behaveRun.crown.setUserProvidedWindAdjustmentFactor(0.15);
    behaveRun.crown.updateCrownInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureFoliar, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyBaseHeight, canopyHeightUnits,
        crownRatio, canopyBulkDensity, canopyBulkDensityUnits);

    behaveRun.crown.doCrownRunScottAndReinhardt();
    expectedFinalCrownFireSpreadRate = 29.47538799999999;
    observedFinalCrownFireSpreadRate = roundToSixDecimalPlaces(behaveRun.crown.getFinalSpreadRate(SpeedUnits::FeetPerMinute));
    BOOST_CHECK_CLOSE(observedFinalCrownFireSpreadRate, expectedFinalCrownFireSpreadRate, ERROR_TOLERANCE);

    expectedFinalCrownFlameLength = 12.759447;
    observedFinalCrownFlameLength = roundToSixDecimalPlaces(behaveRun.crown.getFinalFlameLength(LengthUnits::Feet));
    BOOST_CHECK_CLOSE(observedFinalCrownFlameLength, expectedFinalCrownFlameLength, ERROR_TOLERANCE);

    expectedFinalCrownFirelineIntensity = 509.5687530;
    observedFinalCrownFirelineIntensity = roundToSixDecimalPlaces(behaveRun.crown.getFinalFirelineIntesity(FirelineIntensityUnits::BtusPerFootPerSecond));
    BOOST_CHECK_CLOSE(observedFinalCrownFirelineIntensity, expectedFinalCrownFirelineIntensity, ERROR_TOLERANCE);

    expectedCriticalOpenWindSpeed = 3874.421988;
    observedCriticalOpenWindSpeed = behaveRun.crown.getCriticalOpenWindSpeed(SpeedUnits::FeetPerMinute);
    BOOST_CHECK_CLOSE(observedCriticalOpenWindSpeed, expectedCriticalOpenWindSpeed, ERROR_TOLERANCE);

    behaveRun.surface.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UseCrownRatio);
}

BOOST_AUTO_TEST_CASE(spotModuleTest)
{
    double flameLength = 0.0;
    double expectedFlatSpottingDistance = 0.0;
    double expectedMountainSpottingDistance = 0.0;
    double observedMountainSpottingDistance = 0.0;
    double observedFlatSpottingDistance = 0.0;

    setSurfaceInputsForGS4LowMoistureScenario(behaveRun);
    behaveRun.surface.setWindHeightInputMode(WindHeightInputMode::TwentyFoot);
    behaveRun.surface.doSurfaceRunInDirectionOfMaxSpread();
    flameLength = behaveRun.surface.getFlameLength(LengthUnits::Feet);

    SpotFireLocation::SpotFireLocationEnum location = SpotFireLocation::RIDGE_TOP;
    double ridgeToValleyDistance = 1.0;
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits = LengthUnits::Miles;
    double ridgeToValleyElevation = 2000.0;
    LengthUnits::LengthUnitsEnum elevationUnits = LengthUnits::Feet;
    double downwindCoverHeight = 30.0;
    LengthUnits::LengthUnitsEnum coverHeightUnits = LengthUnits::Feet;
    double burningPileflameHeight = 5.0;
    LengthUnits::LengthUnitsEnum flameHeightUnits = LengthUnits::Feet;
    double windSpeedAtTwentyFeet = 5.0;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    int torchingTrees = 15;
    double DBH = 20.0;
    LengthUnits::LengthUnitsEnum DBHUnits = LengthUnits::Inches;
    double treeHeight = 30.0;
    LengthUnits::LengthUnitsEnum treeHeightUnits = LengthUnits::Feet;
    SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies = SpotTreeSpecies::ENGELMANN_SPRUCE;
   
    LengthUnits::LengthUnitsEnum spottingDistanceUnits = LengthUnits::Miles;;

	// Test spotting distance from a burning pile
	behaveRun.spot.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits, 
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits,
		burningPileflameHeight, flameHeightUnits, windSpeedAtTwentyFeet, windSpeedUnits);
	behaveRun.spot.calculateSpottingDistanceFromBurningPile();
    expectedMountainSpottingDistance = 0.021330;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
	BOOST_CHECK_CLOSE(observedMountainSpottingDistance, expectedMountainSpottingDistance, ERROR_TOLERANCE);
    expectedFlatSpottingDistance = 0.017067;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits));
    BOOST_CHECK_CLOSE(expectedFlatSpottingDistance, expectedFlatSpottingDistance, ERROR_TOLERANCE);
    // Test spotting distance from surface fire
    
	behaveRun.spot.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, windSpeedAtTwentyFeet,
        windSpeedUnits, flameLength, flameHeightUnits);
    behaveRun.spot.calculateSpottingDistanceFromSurfaceFire();
    expectedMountainSpottingDistance = 0.164401;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromSurfaceFire(spottingDistanceUnits));
    BOOST_CHECK_CLOSE(observedMountainSpottingDistance, expectedMountainSpottingDistance, ERROR_TOLERANCE);
    expectedFlatSpottingDistance = 0.132964;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromSurfaceFire(spottingDistanceUnits));
    BOOST_CHECK_CLOSE(expectedFlatSpottingDistance, expectedFlatSpottingDistance, ERROR_TOLERANCE);
    // Test spotting distance from torching trees
  
	behaveRun.spot.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits,
        ridgeToValleyElevation, elevationUnits, downwindCoverHeight, coverHeightUnits, torchingTrees, DBH, DBHUnits, 
        treeHeight, treeHeightUnits, treeSpecies, windSpeedAtTwentyFeet, windSpeedUnits);
    behaveRun.spot.calculateSpottingDistanceFromTorchingTrees();
    expectedMountainSpottingDistance = 0.222396;
    observedMountainSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxMountainousTerrainSpottingDistanceFromTorchingTrees(spottingDistanceUnits));
    BOOST_CHECK_CLOSE(observedMountainSpottingDistance, expectedMountainSpottingDistance, ERROR_TOLERANCE);
    expectedFlatSpottingDistance = 0.181449;
    observedFlatSpottingDistance = roundToSixDecimalPlaces(behaveRun.spot.getMaxFlatTerrainSpottingDistanceFromTorchingTrees(spottingDistanceUnits));
    BOOST_CHECK_CLOSE(observedFlatSpottingDistance, expectedFlatSpottingDistance, ERROR_TOLERANCE);
   
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

BOOST_AUTO_TEST_CASE(SafetyModuleTest)
{
    double flameHeight = 5; // ft
    int numberOfPersonnel = 6;
    int numberOfEquipment = 1;
    double areaPerPerson = 50; // ft^2
    double areaPerEquipment = 300; // ft^2

    LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Feet;
    AreaUnits::AreaUnitsEnum personnelAndEquipmentAreaUnits = AreaUnits::SquareFeet;
    AreaUnits::AreaUnitsEnum safetyZoneAreaUnits = AreaUnits::Acres;

    double expectedSeparationDistance = 0; 
    double expectedSafetyZoneArea = 0;
    double expectedSafetyZoneRadius = 0;

    double observedSeparationDistance = 0;
    double observedSafetyZoneArea = 0;
    double observeSafetyZoneRadius = 0;

    expectedSeparationDistance = 20; // ft
    expectedSafetyZoneArea = 0.082490356; // acres
    expectedSafetyZoneRadius = 33.819766; // ft
    
    behaveRun.safety.updateSafetyInputs(flameHeight, lengthUnits, numberOfPersonnel, numberOfEquipment, areaPerPerson, areaPerEquipment, personnelAndEquipmentAreaUnits);
    behaveRun.safety.calculateSafetyZone();

    observedSeparationDistance = behaveRun.safety.getSeparationDistance(lengthUnits);
    observedSafetyZoneArea = behaveRun.safety.getSafetyZoneArea(safetyZoneAreaUnits);
    observeSafetyZoneRadius = behaveRun.safety.getSafetyZoneRadius(lengthUnits);

    BOOST_CHECK_CLOSE(observedSeparationDistance, expectedSeparationDistance, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedSafetyZoneArea, expectedSafetyZoneArea, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observeSafetyZoneRadius, expectedSafetyZoneRadius, ERROR_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(ContainModuleTest)
{
    double observedFinalFireLineLength = 0;
    double observedPerimeterAtInitialAttack = 0;
    double observedPerimeterAtContainment = 0;
    double observedFireSizeAtInitialAttack = 0;
    double observedFinalFireSize = 0;
    double observedFinalContainmentArea = 0;
    double observedFinalTimeSinceReport = 0;
    ContainStatus::ContainStatusEnum observedContainmentStatus = ContainStatus::Unreported;

    double expectedFinalFireLineLength = 0;
    double expectedPerimeterAtInitialAttack = 0;
    double expectedPerimeterAtContainment = 0;
    double expectedFireSizeAtInitialAttack = 0;
    double expectedFinalFireSize = 0;
    double expectedFinalContainmentArea = 0;
    double expectedFinalTimeSinceReport = 0;
    ContainStatus::ContainStatusEnum expectedContainmentStatus = ContainStatus::Unreported;

    // Test case where expected result is containment
    behaveRun.contain.setAttackDistance(0, LengthUnits::Chains);
    behaveRun.contain.setLwRatio(3);
    behaveRun.contain.setReportRate(5, SpeedUnits::ChainsPerHour);
    behaveRun.contain.setReportSize(1, AreaUnits::Acres);
    behaveRun.contain.setTactic(ContainTactic::HeadAttack);
    behaveRun.contain.addResource(2, 8, TimeUnits::Hours, 20, SpeedUnits::ChainsPerHour, "test");
    behaveRun.contain.doContainRun();
    //behaveRun.contain.removeAllResources();

    expectedFinalFireLineLength = 39.539849615;
    expectedPerimeterAtInitialAttack = 36.694893;
    expectedPerimeterAtContainment = 39.539849615;
    expectedFireSizeAtInitialAttack = 7.10569878;
    expectedFinalFireSize = 9.42749714;
    expectedFinalContainmentArea = 9.42749714;
    expectedFinalTimeSinceReport = 238.75000000;
    expectedContainmentStatus = ContainStatus::Contained;

    observedFinalFireLineLength = behaveRun.contain.getFinalFireLineLength(LengthUnits::Chains);
    observedPerimeterAtInitialAttack = behaveRun.contain.getPerimiterAtInitialAttack(LengthUnits::Chains);
    observedPerimeterAtContainment = behaveRun.contain.getPerimeterAtContainment(LengthUnits::Chains);
    observedFireSizeAtInitialAttack = behaveRun.contain.getFireSizeAtInitialAttack(AreaUnits::Acres);
    observedFinalFireSize = behaveRun.contain.getFinalFireSize(AreaUnits::Acres);
    observedFinalContainmentArea = behaveRun.contain.getFinalContainmentArea(AreaUnits::Acres);
    observedFinalTimeSinceReport = behaveRun.contain.getFinalTimeSinceReport(TimeUnits::Minutes);
    observedContainmentStatus = behaveRun.contain.getContainmentStatus();

    BOOST_CHECK_CLOSE(observedFinalFireLineLength, expectedFinalFireLineLength, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedPerimeterAtContainment, expectedPerimeterAtContainment, ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedFinalContainmentArea, expectedFinalContainmentArea,ERROR_TOLERANCE);
    BOOST_CHECK_CLOSE(observedFireSizeAtInitialAttack, expectedFireSizeAtInitialAttack, ERROR_TOLERANCE);
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
