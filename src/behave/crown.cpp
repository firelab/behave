/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling crown fire behavior
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
*******************************************************************************
*
* THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
* MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
* IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
* OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
* PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
* LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
* PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
* RELIABILITY, OR ANY OTHER CHARACTERISTIC.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
******************************************************************************/

#include "crown.h"

#include <cmath>
#include "fuelModelSet.h"
#include "windSpeedUtility.h"

Crown::Crown(const FuelModelSet& fuelModelSet)
    : surface_(fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet;
    initializeMembers();
}

Crown::~Crown()
{

}

Crown::Crown(const Crown& rhs)
    : surface_(*rhs.fuelModelSet_)
{
    memberwiseCopyAssignment(rhs);
}

Crown& Crown::operator=(const Crown& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void Crown::memberwiseCopyAssignment(const Crown& rhs)
{
    fuelModelSet_ = rhs.fuelModelSet_;
    surface_ = rhs.surface_;
    crownInputs_ = rhs.crownInputs_;

    fireType_ = rhs.fireType_;
    crownsSurfaceHeatPerUnitArea_ = rhs.crownsSurfaceHeatPerUnitArea_;
    crownsSurfaceFirelineIntensity_ = rhs.crownsSurfaceFirelineIntensity_;
    crownFuelLoad_ = rhs.crownFuelLoad_;
    canopyHeatPerUnitArea_ = rhs.canopyHeatPerUnitArea_;
    crownFireHeatPerUnitArea_ = rhs.crownFireHeatPerUnitArea_;
    crownFirelineIntensity_ = rhs.crownFirelineIntensity_;
    crownFlameLength_ = rhs.crownFlameLength_;
    crownFireSpreadRate_ = rhs.crownFireSpreadRate_;
    crownCriticalSurfaceFireIntensity_ = rhs.crownCriticalSurfaceFireIntensity_;
    crownCriticalFireSpreadRate_ = rhs.crownCriticalFireSpreadRate_;
    crownCriticalSurfaceFlameLength_ = rhs.crownCriticalSurfaceFlameLength_;
    crownPowerOfFire_ = rhs.crownPowerOfFire_;
    crownPowerOfWind_ = rhs.crownPowerOfWind_;
    crownFirePowerRatio_ = rhs.crownFirePowerRatio_;
    crownFireActiveRatio_ = rhs.crownFireActiveRatio_;
    crownFireTransitionRatio_ = rhs.crownFireTransitionRatio_;
    windSpeedAtTwentyFeet_ = rhs.windSpeedAtTwentyFeet_;
}

void Crown::doCrownRun()
{
    // This method uses Rothermel's 1991 crown fire correlation to calculate Crown fire average spread rate (ft/min)

    // Step 1: Do surface run and store values needed for further calculations 
    surface_.doSurfaceRunInDirectionOfMaxSpread(); // Crown fire is always in direction of max spread
    crownsSurfaceHeatPerUnitArea_ = surface_.getHeatPerUnitArea();
    crownsSurfaceFirelineIntensity_ = surface_.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond);
    surface_.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    double windAdjustmentFactor = 0.4; // wind adjustment factor is assumed to be 0.4 for crown ROS
    surface_.setUserProvidedWindAdjustmentFactor(windAdjustmentFactor);

    // Step 2: Create the crown fuel model (fire behavior fuel model 10)
    surface_.setFuelModelNumber(10);    // set the fuel model used to fuel model 10
    surface_.setSlope(0.0, SlopeUnits::Degrees); // slope is always assumed to be zero in crown ROS
    surface_.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    surface_.setWindDirection(0.0);     // wind direction is assumed to be upslope in crown ROS

    // Step 3: Determine crown fire behavior
    surface_.doSurfaceRunInDirectionOfMaxSpread(); // Crown fire is always in direction of max spread
    crownFireSpreadRate_ = 3.34 * surface_.getSpreadRate(SpeedUnits::FeetPerMinute); // Rothermel 1991

    //  Step 4: Calculate remaining crown fire characteristics
    calculateCrownFuelLoad();
    calculateCanopyHeatPerUnitArea();
    calculateCrownFireHeatPerUnitArea();
    calculateCrownFirelineIntensity();
    calculateCrownFlameLength();
    
    calculateCrownCriticalFireSpreadRate();
    calculateCrownFireActiveRatio();

    calculateCrownCriticalSurfaceFireIntensity();
    calculateCrownCriticalSurfaceFlameLength();
    calculateCrownFireTransitionRatio();

    calculateCrownPowerOfFire();
    calculateWindSpeedAtTwentyFeet();
    calcuateCrownPowerOfWind();
    calculateCrownLengthToWidthRatio();
    calcualteCrownFirePowerRatio();

    // Determine if/what type of crown fire has occured
    calculateFireType();
}

double Crown::getCrownFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(crownFireSpreadRate_, spreadRateUnits);
}

double Crown::getSurfaceFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return surface_.getSpreadRate(spreadRateUnits);
}

double Crown::getCrownFirelineIntensity() const
{
    return crownFirelineIntensity_;
}

double Crown::getCrownFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
    return LengthUnits::fromBaseUnits(crownFlameLength_, flameLengthUnits);
}

FireType::FireTypeEnum Crown::getFireType() const
{
    return fireType_;
}

double Crown::getCrownFireLengthToWidthRatio() const
{
    return crownFireLengthToWidthRatio_;
}

void Crown::initializeMembers()
{
    fireType_ = FireType::Surface;
    crownsSurfaceHeatPerUnitArea_ = 0.0;
    crownsSurfaceFirelineIntensity_ = 0.0;
    crownFuelLoad_ = 0.0;
    canopyHeatPerUnitArea_ = 0.0;
    crownFireHeatPerUnitArea_ = 0.0;
    crownFirelineIntensity_ = 0.0;
    crownFlameLength_ = 0.0;
    crownFireSpreadRate_ = 0.0;
    crownCriticalSurfaceFireIntensity_ = 0.0;
    crownCriticalFireSpreadRate_ = 0.0;
    crownCriticalSurfaceFlameLength_ = 0.0;
    crownPowerOfFire_ = 0.0;
    crownPowerOfWind_ = 0.0;
    crownFirePowerRatio_ = 0.0;
    crownFireActiveRatio_ = 0.0;
    crownFireTransitionRatio_ = 0.0;
    windSpeedAtTwentyFeet_ = 0.0;;
    crownFireLengthToWidthRatio_ = 1.0;

    crownInputs_.initializeMembers();
}

void Crown::calculateCanopyHeatPerUnitArea()
{
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lbs)
    canopyHeatPerUnitArea_ = crownFuelLoad_ * LOW_HEAT_OF_COMBUSTION;
}

void Crown::calculateCrownFireHeatPerUnitArea()
{
    crownFireHeatPerUnitArea_ = crownsSurfaceHeatPerUnitArea_ + canopyHeatPerUnitArea_;
}

void Crown::calculateCrownFuelLoad()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity();
    double canopyBaseHeight = crownInputs_.getCanopyBaseHeight();
    double canopyHeight = surface_.getCanopyHeight(LengthUnits::Feet);
    crownFuelLoad_ = canopyBulkDensity * (canopyHeight - canopyBaseHeight);
}

void Crown::calculateCrownFireTransitionRatio()
{
   crownFireTransitionRatio_ = ((crownCriticalSurfaceFireIntensity_ < 1.0e-7)
        ? (0.00)
        : (crownsSurfaceFirelineIntensity_ / crownCriticalSurfaceFireIntensity_));
}

void Crown::calculateCrownFirelineIntensity()
{
    crownFirelineIntensity_ = (crownFireSpreadRate_ / 60.0) * crownFireHeatPerUnitArea_;
}

void Crown::calculateCrownCriticalSurfaceFireIntensity()
{
    // Get moisture content in percent and constrain lower limit
    double moistureFoliar = crownInputs_.getMoistureFoliar(MoistureUnits::Percent);
    moistureFoliar = (moistureFoliar < 30.0) ? 30.0 : moistureFoliar;

    double crownBaseHeight = crownInputs_.getCanopyBaseHeight();
    // Convert crown base height to meters and constrain lower limit
    crownBaseHeight = LengthUnits::fromBaseUnits(crownBaseHeight, LengthUnits::Meters);
    crownBaseHeight = (crownBaseHeight < 0.1) ? 0.1 : crownBaseHeight;

    // Critical surface fireline intensity (kW/m)
    FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits = FirelineIntensityUnits::KilowattsPerMeter;
    // Need to changed value in calculation below from 450 to 460 at some point
    crownCriticalSurfaceFireIntensity_ = pow((0.010 * crownBaseHeight * (450.0 + 25.9 * moistureFoliar)), 1.5);
    
    // Return as Btu/ft/s
    crownCriticalSurfaceFireIntensity_ = FirelineIntensityUnits::toBaseUnits(crownCriticalSurfaceFireIntensity_, firelineIntensityUnits);
}

void Crown::calculateCrownCriticalSurfaceFlameLength()
{
    crownCriticalSurfaceFlameLength_ = surface_.calculateFlameLength(crownCriticalSurfaceFireIntensity_);
}

void Crown::calculateCrownFlameLength()
{
    crownFlameLength_ = 0.2 * pow(crownFirelineIntensity_, (2.0 / 3.0));
}

void Crown::calculateCrownPowerOfFire()
{
    crownPowerOfFire_ = crownFirelineIntensity_ / 129.0;
}

void Crown::calcuateCrownPowerOfWind()
{
    const double SECONDS_PER_MINUTE = 60.0;

    double WindspeedMinusCrownROS = 0.0;

    // Eq. 7, Rothermel 1991
    WindspeedMinusCrownROS = (windSpeedAtTwentyFeet_ - crownFireSpreadRate_) / SECONDS_PER_MINUTE;
    WindspeedMinusCrownROS = (WindspeedMinusCrownROS < 1e-07) ? 0.0 : WindspeedMinusCrownROS;
    crownPowerOfWind_ = 0.00106 * (WindspeedMinusCrownROS * WindspeedMinusCrownROS * WindspeedMinusCrownROS);
}

void Crown::calcualteCrownFirePowerRatio()
{
    crownFirePowerRatio_ = (crownPowerOfWind_ > 1e-07) ? (crownPowerOfFire_ / crownPowerOfWind_) : 0.0;
}

void Crown::calculateCrownCriticalFireSpreadRate()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity();

    // Convert canopy bulk density to Kg/m3
    double convertedCanopyBulkDensity = DensityUnits::fromBaseUnits(canopyBulkDensity, DensityUnits::KilogramsPerCubicMeter);
    crownCriticalFireSpreadRate_ = (convertedCanopyBulkDensity < 1e-07) ? 0.00 : (3.0 / convertedCanopyBulkDensity);

    // Convert spread rate from m/min to ft/min
    crownCriticalFireSpreadRate_ = SpeedUnits::toBaseUnits(crownCriticalFireSpreadRate_, SpeedUnits::MetersPerMinute);
}

void Crown::calculateCrownFireActiveRatio()
{
    crownFireActiveRatio_ = (crownCriticalFireSpreadRate_ < 1e-07)
        ? (0.00)
        : (crownFireSpreadRate_ / crownCriticalFireSpreadRate_);
}

void Crown::calculateWindSpeedAtTwentyFeet()
{
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode;
    windHeightInputMode = surface_.getWindHeightInputMode();

    if (windHeightInputMode == WindHeightInputMode::TwentyFoot)
    {
        windSpeedAtTwentyFeet_ = surface_.getWindSpeed(SpeedUnits::FeetPerMinute, windHeightInputMode);
    }
    else if (windHeightInputMode == WindHeightInputMode::TenMeter)
    {
        WindSpeedUtility windSpeedUtility;
        double windSpeedAtTenMeters = surface_.getWindSpeed(SpeedUnits::FeetPerMinute, windHeightInputMode);
        windSpeedAtTwentyFeet_ = windSpeedUtility.windSpeedAtTwentyFeetFromTenMeter(windSpeedAtTenMeters);
    }
}

void Crown::calculateCrownLengthToWidthRatio()
{
    //Calculates the crown fire length-to-width ratio given the 20-ft wind speed (in mph)
    // (Rothermel 1991, Equation 10, p16)

    double windSpeed = SpeedUnits::fromBaseUnits(windSpeedAtTwentyFeet_, SpeedUnits::MilesPerHour);
    if(windSpeedAtTwentyFeet_ > 1.0e-07)
    {
        crownFireLengthToWidthRatio_ = 1.0 + 0.125 * windSpeed;
    }
    else
    {
        crownFireLengthToWidthRatio_ = 1.0;
    }
}

void Crown::calculateFireType()
{
    fireType_ = FireType::Surface;
    // If the fire CAN NOT transition to the crown ...
    if (crownFireTransitionRatio_ < 1.0)
    {
        if (crownFireActiveRatio_ < 1.0)
        {
            fireType_ = FireType::Surface; // Surface fire
        }
        else // crownFireActiveRatio_ >= 1.0 
        {
            fireType_ = FireType::ConditionalCrownFire; // Conditional crown fire
        }
    }
    // If the fire CAN transition to the crown ...
    else // crownFireTransitionRatio_ >= 1.0 )
    {
        if (crownFireActiveRatio_ < 1.0)
        {
            fireType_ = FireType::Torching; // Torching
        }
        else // crownFireActiveRatio_ >= 1.0
        {
            fireType_ = FireType::Crowning; // Crowning
        }
    }
}

void Crown::updateCrownInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, double moistureFoliar, MoistureUnits::MoistureUnitsEnum moistureUnits,
    double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
    SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight, 
    double canopyBaseHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, double canopyBulkDensity, 
    DensityUnits::DensityUnitsEnum densityUnits)
{
    surface_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
        moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode, windDirection,
        windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits, canopyHeight, canopyHeightUnits, crownRatio);
    crownInputs_.updateCrownInputs(canopyBaseHeight, canopyHeightUnits, canopyBulkDensity, densityUnits, moistureFoliar, moistureUnits);
}

void Crown::setCanopyBaseHeight(double canopyBaseHeight, LengthUnits::LengthUnitsEnum heightUnits)
{
    crownInputs_.setCanopyBaseHeight(canopyBaseHeight, heightUnits);
}

void Crown::setCanopyBulkDensity(double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits)
{
    crownInputs_.setCanopyBulkDensity(canopyBulkDensity, densityUnits);
}

void Crown::setMoistureFoliar(double moistureFoliar, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    crownInputs_.setMoistureFoliar(moistureFoliar, moistureUnits);
}

void Crown::updateCrownsSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, 
    double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody, 
    MoistureUnits::MoistureUnitsEnum moistureUnits, double windSpeed, 
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, 
    double slope, SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits,
    double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio)
{
    surface_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, 
        moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits,
        canopyHeight, canopyHeightUnits, crownRatio);
}

void  Crown::setCanopyCover(double canopyCover, CoverUnits::CoverUnitsEnum coverUnits)
{
    surface_.setCanopyCover(canopyCover, coverUnits);
}

void  Crown::setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    surface_.setCanopyHeight(canopyHeight, canopyHeightUnits);
}

void  Crown::setCrownRatio(double crownRatio)
{
    surface_.setCrownRatio(crownRatio);
}

void Crown::setFuelModelSet(FuelModelSet & fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet;
}


void  Crown::setFuelModelNumber(int fuelModelNumber)
{
    surface_.setFuelModelNumber(fuelModelNumber);
}

void  Crown::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureOneHour(moistureOneHour, moistureUnits);
}

void  Crown::setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureTenHour(moistureTenHour, moistureUnits);
}

void  Crown::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureHundredHour(moistureHundredHour, moistureUnits);
}

void  Crown::setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureLiveHerbaceous(moistureLiveHerbaceous, moistureUnits);
}

void  Crown::setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surface_.setMoistureLiveWoody(moistureLiveWoody, moistureUnits);
}

void  Crown::setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits)
{
    surface_.setSlope(slope, slopeUnits);
}

void  Crown::setAspect(double aspect)
{
    surface_.setAspect(aspect);
}

void  Crown::setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surface_.setWindSpeed(windSpeed, windSpeedUnits, windHeightInputMode);
}

void  Crown::setWindDirection(double windDirection)
{
    surface_.setWindDirection(windDirection);
}

void Crown::setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surface_.setWindHeightInputMode(windHeightInputMode);
}

void  Crown::setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadAngleMode)
{
    surface_.setWindAndSpreadOrientationMode(windAndSpreadAngleMode);
}

int Crown::getFuelModelNumber() const
{
    return surface_.getFuelModelNumber();
}

double Crown::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surface_.getMoistureOneHour(moistureUnits);
}

double Crown::getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surface_.getMoistureTenHour(moistureUnits);
}

double Crown::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surface_.getMoistureHundredHour(moistureUnits);
}

double Crown::getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surface_.getMoistureLiveHerbaceous(moistureUnits);
}

double Crown::getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surface_.getMoistureLiveWoody(moistureUnits);
}

double Crown::getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode) const
{
    return surface_.getWindSpeed(windSpeedUnits, windHeightInputMode);
}

double Crown::getWindDirection() const
{
    return surface_.getWindDirection();
}

double Crown::getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const
{
    return surface_.getSlope(slopeUnits);
}

double Crown::getAspect() const
{
    return surface_.getAspect();
}

double Crown::getCanopyCover(CoverUnits::CoverUnitsEnum canopyCoverUnits) const
{
    return surface_.getCanopyCover(canopyCoverUnits);
}

double Crown::getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeighUnits) const
{
    return surface_.getCanopyHeight(canopyHeighUnits);
}

double Crown::getCrownRatio() const
{
    return surface_.getCrownRatio();
}

double Crown::getCanopyBaseHeight() const
{
    return crownInputs_.getCanopyBaseHeight();
}

double Crown::getCanopyBulkDensity() const
{
    return crownInputs_.getCanopyBulkDensity();
}

double Crown::getMoistureFoliar(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return crownInputs_.getMoistureFoliar(moistureUnits);
}

//double Crown::getSpreadRateBaseOnFireType(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
//{
//    if (fireType_ == FireType::ConditionalCrownFire || fireType_ == FireType::Crowning)
//    {
//        return surface_.getSpreadRate(spreadRateUnits);
//    }
//    else
//    {
//        return SpeedUnits::fromBaseUnits(crownFireSpreadRate_, spreadRateUnits);
//    }
//}
