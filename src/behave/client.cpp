#include <iomanip>
#include <iostream>
#include <math.h>

#include "behaveRun.h"
#include "fuelModels.h"

int main()
{
    // Surface Fire Inputs;
    int fuelModelNumber = 0;
    double moistureOneHour = 0.0;
    double moistureTenHour = 0.0;
    double moistureHundredHour = 0.0;
    double moistureLiveHerbaceous = 0.0;
    double moistureLiveWoody = 0.0;
    double windSpeed = 0.0;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MilesPerHour;
    double windDirection = 0;
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode = WindHeightInputMode::DirectMidflame;
    double slope = 0.0;
    double aspect = 0.0;
    double directionOfMaxSpread = 0;
    double flameLength = 0;
    double directionOfInterest = 0;
    double spreadRate = 0;

    // Wind adjustment factor parameters
    double canopyCover = 0.0;
    double canopyHeight = 0.0;
    double crownRatio = 0.0;

    FuelModels fuelModels;
    SpeciesMasterTable mortalitySpeciesTable;
    BehaveRun behave(fuelModels, mortalitySpeciesTable);

    fuelModelNumber = 1;
    moistureOneHour = 6;
    moistureTenHour = 7;
    moistureHundredHour = 8;
    moistureLiveHerbaceous = 60;
    moistureLiveWoody = 90;
    windSpeed = 5;
    windDirection = 42;

    slope = 30;
    aspect = 291;
    directionOfInterest = 63;
    canopyCover = 0.50; // 50%
    FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
    canopyHeight = 6;
    LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;
    crownRatio = 0.50;

    WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode = behave.surface.getWindAndSpreadOrientationMode();
    SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum surfaceFireSpreadDirectionMode = SurfaceFireSpreadDirectionMode::FromIgnitionPoint;

    // Single fuel model test
    behave.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody,
        FractionUnits::Percent, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode, slope, SlopeUnits::Degrees,
                                       aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);

    behave.surface.setIsUsingChaparral(true);
    behave.surface.setChaparralFuelBedDepth(3, LengthUnits::Feet);
    behave.surface.setChaparralFuelType(ChaparralFuelType::Chamise);
    behave.surface.setChaparralFuelLoadInputMode(ChaparralFuelLoadInputMode::FuelLoadFromDepthAndChaparralType);
    behave.surface.setChaparralFuelDeadLoadFraction(0.25);
    behave.surface.setChaparralTotalFuelLoad(1.0, LoadingUnits::PoundsPerSquareFoot);
    behave.surface.setWindHeightInputMode(WindHeightInputMode::DirectMidflame);
    behave.surface.setSlope(0, SlopeUnits::Percent);

    behave.surface.doSurfaceRunInDirectionOfMaxSpread();
    spreadRate = behave.surface.getSpreadRate(SpeedUnits::ChainsPerHour);

    // Setting the wind and spread angle input mode (default is upslope)
    behave.surface.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToNorth);

    // Checking  wind and spread angle input mode
   
    std::cout << "Wind and spread direction are in degrees clockwise relative to ";
    if (windAndSpreadOrientationMode == WindAndSpreadOrientationMode::RelativeToUpslope)
    {
        std::cout << "upslope" << std::endl << std::endl;
    }
    if (windAndSpreadOrientationMode == WindAndSpreadOrientationMode::RelativeToNorth)
    {
        std::cout << "compass north" << std::endl << std::endl;
    }

    for (int i = 0; i <= 10; i++)
    {
        fuelModelNumber = i;
        moistureOneHour = 6;
        moistureTenHour = 7;
        moistureHundredHour = 8;
        moistureLiveHerbaceous = 60;
        moistureLiveWoody = 90;
        TwoFuelModelsMethod::TwoFuelModelsMethodEnum  twoFuelModelsMethod = TwoFuelModelsMethod::TwoDimensional;
        windSpeed = 5;
        windHeightInputMode = WindHeightInputMode::DirectMidflame;
        windDirection = 180;
        slope = 30;
        SlopeUnits::SlopeUnitsEnum slopeUnits = SlopeUnits::Percent;
        aspect = 250;
        directionOfInterest = 0;
        SurfaceFireSpreadDirectionMode::SurfaceFireSpreadDirectionModeEnum surfaceFireSpreadDirectionMode = SurfaceFireSpreadDirectionMode::FromIgnitionPoint;

        // std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;

        // Two Fuel Models test
        int firstFuelModelNumber = 0;
        int secondFuelModelNumber = 0;
        double firstFuelModelCoverage = 0.0;
        firstFuelModelNumber = 1;
        secondFuelModelNumber = 124;
        firstFuelModelCoverage = 0 + (.10 * i);
        FractionUnits::FractionUnitsEnum firstFuelModelCoverageUnits = FractionUnits::Percent;
        FractionUnits::FractionUnitsEnum canopyCoverUnits = FractionUnits::Percent;
        LengthUnits::LengthUnitsEnum canopyHeightUnits = LengthUnits::Feet;

        behave.surface.updateSurfaceInputsForTwoFuelModels(firstFuelModelNumber, secondFuelModelNumber, moistureOneHour, moistureTenHour,
            moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, FractionUnits::Percent, windSpeed, windSpeedUnits, windHeightInputMode,
            windDirection, windAndSpreadOrientationMode, firstFuelModelCoverage, firstFuelModelCoverageUnits, twoFuelModelsMethod, slope,
                                                           slopeUnits, aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);
        behave.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
        spreadRate = behave.surface.getSpreadRate(SpeedUnits::ChainsPerHour);
        //spreadRate = floor(spreadRate * 10 + 0.5) / 10;
        std::cout << "Spread rate for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " with first fuel coverage " << firstFuelModelCoverage * 100 << "%" << std::endl;
        std::cout << "is " << spreadRate << " ch/hr" << std::endl;
        flameLength = behave.surface.getFlameLength(LengthUnits::Feet);
        //flameLength = floor(flameLength * 10 + 0.5) / 10;
        std::cout << "Flame length for the two fuel models " << firstFuelModelNumber << " and " << secondFuelModelNumber << " is " << flameLength << " ft" << std::endl;

        // Direction of Max Spread test
        directionOfMaxSpread = behave.surface.getDirectionOfMaxSpread();
        std::cout << "Direction of maximum spread is " << directionOfMaxSpread << " degrees" << std::endl << std::endl;
    }

    fuelModelNumber = 1;
    moistureOneHour = 6;
    moistureTenHour = 7;
    moistureHundredHour = 8;
    moistureLiveHerbaceous = 60;
    moistureLiveWoody = 90;
    windSpeed = 5;
    windDirection = 42;
  
    slope = 30;
    aspect = 291;
    directionOfInterest = 63;
    canopyCover = 0.50; // 50%
    canopyCoverUnits = FractionUnits::Percent;
    canopyHeight = 6;
    canopyHeightUnits = LengthUnits::Feet;
    crownRatio = 0.50;

    surfaceFireSpreadDirectionMode = SurfaceFireSpreadDirectionMode::FromIgnitionPoint;

    // Single fuel model test
    behave.surface.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous, moistureLiveWoody, 
        FractionUnits::Percent, windSpeed, windSpeedUnits, windHeightInputMode, windDirection, windAndSpreadOrientationMode, slope, SlopeUnits::Degrees,
                                       aspect, canopyCover, canopyCoverUnits, canopyHeight, canopyHeightUnits, crownRatio, FractionUnits::Fraction);
    behave.surface.doSurfaceRunInDirectionOfInterest(directionOfInterest, surfaceFireSpreadDirectionMode);
    spreadRate = behave.surface.getSpreadRate(SpeedUnits::ChainsPerHour);
    //flameLength = floor((behave.getFlameLength()) * 10 + 0.5) / 10;
    flameLength = behave.surface.getFlameLength(LengthUnits::Feet);
    std::cout << "Spread rate for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << spreadRate << " ch/hr" << std::endl;
    std::cout << "Flame length for fuel model " << fuelModelNumber << " is " << std::setprecision(8) << flameLength << " ft" << std::endl << std::endl;
    // Direction of Max Spread test
    directionOfMaxSpread = behave.surface.getDirectionOfMaxSpread();
    std::cout << "Direction of maximum spread is " << round(directionOfMaxSpread) << " degrees" << std::endl << std::endl;

    // Example usage of Mortality module
    double probalilityOfMortality = 0;

    vector<SpeciesMasterTableRecord> speciesInRegion;

    vector<bool> requiredFieldVector;

    vector<std::string> fieldNameStrings =
    {
        "region",
        "flame_length_or_scorch_height_switch",
        "flame_length_or_scorch_height_value",
        "equation_type",
        "dbh",
        "tree_height",
        "crown_ratio",
        "crown_damage",
        "cambium_kill_rating",
        "beetle_damage",
        "bole_char_height",
        "fire_severity"
    };   // This vecctor is for getting the name of an input from its enum value

    GACC region = GACC::EasternArea;
    EquationType equationType = EquationType::crown_scorch;

    behave.mortality.setGACCRegion(region);
    behave.mortality.setEquationType(equationType);

    // Get species lists by region or by region and equation type to narrow down further
    speciesInRegion = behave.mortality.getSpeciesRecordVectorForGACCRegion(region);
    speciesInRegion = behave.mortality.getSpeciesRecordVectorForGACCRegionAndEquationType(region, equationType);

    string speciesCode = "ABBA";
    behave.mortality.setSpeciesCode(speciesCode);

    // Query the species master table for various info
    int numSpeciesRecords = behave.mortality.getNumberOfRecordsInSpeciesTable();
    int speciesIndex = behave.mortality.getSpeciesTableIndexFromSpeciesCode(speciesCode);
    bool isInRegion = behave.mortality.checkIsInGACCRegionAtSpeciesTableIndex(speciesIndex, region);
   
    // With species and equation type selected, find out the required inputs
    bool isInputOk = false;
    if(isInRegion)
    {
        isInputOk = behave.mortality.updateInputsForSpeciesCodeAndEquationType(speciesCode, equationType);
    }

    if(isInputOk)
    {
        behave.mortality.setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch::flame_length);
        behave.mortality.setFlameLengthOrScorchHeightValue(4.0, LengthUnits::Feet);

        // Not all of inputs are needed for every equation type, requirements vary, inpsect requiredFieldVector to see which are needed
        behave.mortality.setTreeDensityPerUnitArea(100, AreaUnits::Acres);
        double testGetTreeDensityByArea = behave.mortality.getTreeDensityPerUnitArea(AreaUnits::Acres);
        behave.mortality.setDBH(5.0, LengthUnits::Inches);
        double testGetDBH = behave.mortality.getDBH(LengthUnits::Feet);
        behave.mortality.setTreeHeight(35.0, LengthUnits::Feet);
        double testGetTreeHeight = behave.mortality.getTreeHeight(LengthUnits::Feet);
        behave.mortality.setCrownRatio(0.30, FractionUnits::Fraction); // A value between 0.0 and 1.0 is valid
        behave.mortality.setCrownDamage(25.0);
        behave.mortality.setCambiumKillRating(3.2);
        behave.mortality.setBeetleDamage(BeetleDamage::yes);
        behave.mortality.setBoleCharHeight(2.0, LengthUnits::Feet);
        double testGetBoleCharHeight = behave.mortality.getBoleCharHeight(LengthUnits::Feet);

        requiredFieldVector = behave.mortality.getRequiredFieldVector();

        for(int i = (int)RequiredFieldNames::dbh; i < requiredFieldVector.size(); i++)
        {
            if(requiredFieldVector[i] == true)
            {
                RequiredFieldNames currentRequiredFieldName = static_cast<RequiredFieldNames>(i);
                if((currentRequiredFieldName == RequiredFieldNames::dbh) && behave.mortality.getDBH(LengthUnits::Inches) < 0)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::tree_height) && behave.mortality.getTreeHeight(LengthUnits::Feet) < 0)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::crown_ratio) && behave.mortality.getCrownRatio(FractionUnits::Fraction) < 0)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::crown_damage) && behave.mortality.getCrownDamage() < 0)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::cambium_kill_rating) && behave.mortality.getCambiumKillRating() < 0)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::beetle_damage) && behave.mortality.getBeetleDamage() == BeetleDamage::not_set)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::bole_char_height) && behave.mortality.getBoleCharHeight(LengthUnits::Feet) < 0)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
                if((currentRequiredFieldName == RequiredFieldNames::fire_severity) && behave.mortality.getFireSeverity() == FireSeverity::not_set)
                {
                    std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                }
            }
        }

        probalilityOfMortality = behave.mortality.calculateMortality(FractionUnits::Fraction);

        probalilityOfMortality = behave.mortality.getProbabilityOfMortality(FractionUnits::Percent);

        std::cout << "Probability of behave.mortality: " << std::setprecision(3) << probalilityOfMortality << "%\n";
    }
    else
    {
        std::cout << "Error, species not found\n";
    }

#ifndef NDEBUG
    std::cout << "Press Enter to continue";
    std::cin.get();
#endif

    return 0;
}
