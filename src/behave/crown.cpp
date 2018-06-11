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
    : surfaceFuel_(fuelModelSet), crownFuel_(fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet;
    initializeMembers();
}

Crown::~Crown()
{

}

Crown::Crown(const Crown& rhs)
    : surfaceFuel_(*rhs.fuelModelSet_), crownFuel_(*rhs.fuelModelSet_)
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
    surfaceFuel_ = rhs.surfaceFuel_;
    crownFuel_ = rhs.crownFuel_;
    crownInputs_ = rhs.crownInputs_;

    fireType_ = rhs.fireType_;
    surfaceFireHeatPerUnitArea_ = rhs.surfaceFireHeatPerUnitArea_;
    surfaceFirelineIntensity_ = rhs.surfaceFirelineIntensity_;
    crownFuelLoad_ = rhs.crownFuelLoad_;
    canopyHeatPerUnitArea_ = rhs.canopyHeatPerUnitArea_;
    crownFireHeatPerUnitArea_ = rhs.crownFireHeatPerUnitArea_;
    crownFirelineIntensity_ = rhs.crownFirelineIntensity_;
    crownFlameLength_ = rhs.crownFlameLength_;
    crownFireSpreadRate_ = rhs.crownFireSpreadRate_;
    crownCriticalSurfaceFirelineIntensity_ = rhs.crownCriticalSurfaceFirelineIntensity_;
    crownCriticalFireSpreadRate_ = rhs.crownCriticalFireSpreadRate_;
    crownCriticalSurfaceFlameLength_ = rhs.crownCriticalSurfaceFlameLength_;
    crownPowerOfFire_ = rhs.crownPowerOfFire_;
    crownPowerOfWind_ = rhs.crownPowerOfWind_;
    crownFirePowerRatio_ = rhs.crownFirePowerRatio_;
    crownFireActiveRatio_ = rhs.crownFireActiveRatio_;
    crownFireTransitionRatio_ = rhs.crownFireTransitionRatio_;
    windSpeedAtTwentyFeet_ = rhs.windSpeedAtTwentyFeet_;
    crownFireLengthToWidthRatio_ = rhs.crownFireLengthToWidthRatio_;

    surfaceFireSpreadRate_ = rhs.surfaceFireSpreadRate_;
    surfaceFireCriticalSpreadRate_ = rhs.surfaceFireCriticalSpreadRate_;

    passiveCrownFireSpreadRate_ = rhs.passiveCrownFireSpreadRate_;
    passiveCrownFireHeatPerUnitArea_ = rhs.passiveCrownFireHeatPerUnitArea_;
    passiveCrownFireLineIntensity_ = rhs.passiveCrownFireLineIntensity_;
    passiveCrownFireFlameLength_ = rhs.passiveCrownFireFlameLength_;

    finalSpreadRate_ = rhs.finalSpreadRate_;
    finalHeatPerUnitArea_ = rhs.finalHeatPerUnitArea_;
    finalFirelineIntesity_ = rhs.finalFirelineIntesity_;
    finalFlameLength_ = rhs.finalFlameLength_;

    isSurfaceFire_ = rhs.isSurfaceFire_;
    isPassiveCrownFire_ = rhs.isPassiveCrownFire_;
    isActiveCrownFire_ = rhs.isActiveCrownFire_;

    crownFireActiveWindSpeed_ = rhs.crownFireActiveWindSpeed_;
    crownFractionBurned_ = rhs.crownFractionBurned_;
}

void Crown::doCrownRunRothermel()
{
    // This method uses Rothermel's 1991 crown fire correlation to calculate Crown fire average spread rate (ft/min)

    double canopyHeight = surfaceFuel_.getCanopyHeight(LengthUnits::Feet);
    double canopyBaseHeight = crownInputs_.getCanopyBaseHeight(LengthUnits::Feet);
    double crownRatio = (canopyHeight - canopyBaseHeight) / canopyHeight;
    surfaceFuel_.setCrownRatio(crownRatio);

    // Step 1: Do surface run and store values needed for further calculations 
    surfaceFuel_.doSurfaceRunInDirectionOfMaxSpread(); // Crown ROS output given in direction of max spread 
    surfaceFireHeatPerUnitArea_ = surfaceFuel_.getHeatPerUnitArea();
    surfaceFirelineIntensity_ = surfaceFuel_.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond);
    surfaceFuel_.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    double windAdjustmentFactor = 0.4; // Wind adjustment factor is assumed to be 0.4
    surfaceFuel_.setUserProvidedWindAdjustmentFactor(windAdjustmentFactor);

    // Step 2: Create the crown fuel model (fire behavior fuel model 10)
    surfaceFuel_.setFuelModelNumber(10); // Set the fuel model used to fuel model 10
    surfaceFuel_.setSlope(0.0, SlopeUnits::Degrees); // Slope is assumed to be zero
    surfaceFuel_.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    surfaceFuel_.setWindDirection(0.0); // Wind direction is assumed to be upslope

    // Step 3: Determine crown fire behavior
    surfaceFuel_.doSurfaceRunInDirectionOfMaxSpread();
    crownFireSpreadRate_ = 3.34 * surfaceFuel_.getSpreadRate(SpeedUnits::FeetPerMinute); // Rothermel 1991

    // Step 4: Calculate remaining crown fire characteristics
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
    calculateFireTypeRothermel();
}

void Crown::doCrownRunScottAndReinhardt()
{
    // Scott and Reinhardt (2001) linked models method for crown fire

    double canopyHeight = surfaceFuel_.getCanopyHeight(LengthUnits::Feet);
    double canopyBaseHeight = crownInputs_.getCanopyBaseHeight(LengthUnits::Feet);
    double crownRatio = (canopyHeight - canopyBaseHeight) / canopyHeight;

    surfaceFuel_.setCrownRatio(crownRatio);
    
    // Step 1: Do surface run and store values needed for further calculations
    surfaceFuel_.doSurfaceRunInDirectionOfMaxSpread();
    surfaceFireSpreadRate_ = surfaceFuel_.getSpreadRate(SpeedUnits::FeetPerMinute); // Rothermel 1991
    surfaceFireHeatPerUnitArea_ = surfaceFuel_.getHeatPerUnitArea();
    surfaceFirelineIntensity_ = surfaceFuel_.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond);
    surfaceFireFlameLength_ = surfaceFuel_.getFlameLength(LengthUnits::Feet); // Byram

    // Step 2: Create the crown fuel model (fire behavior fuel model 10)
    crownFuel_ = surfaceFuel_;
    crownFuel_.setFuelModelNumber(10); // Set the crown fuel model used to fuel model 10
    crownFuel_.setUserProvidedWindAdjustmentFactor(0.4); // Wind adjustment factor is assumed to be 0.4
    crownFuel_.setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::UserInput);
    crownFuel_.setSlope(0, SlopeUnits::Degrees);
    crownFuel_.setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::RelativeToUpslope);
    crownFuel_.setWindDirection(0.0); // Wind direction is assumed to be upslope
    crownFuel_.setWindSpeed(surfaceFuel_.getWindSpeed(SpeedUnits::FeetPerMinute, WindHeightInputMode::TwentyFoot), SpeedUnits::FeetPerMinute, WindHeightInputMode::TwentyFoot);
    
    // Step 3: Determine crown fire behavior
    crownFuel_.doSurfaceRunInDirectionOfMaxSpread();    
    crownFireSpreadRate_ = 3.34 * crownFuel_.getSpreadRate(SpeedUnits::FeetPerMinute); // Rothermel 1991

    // Step 4: Calculate remaining crown fire characteristics
    calculateCrownFireActiveWindSpeed();
    calculateCrownFuelLoad();
    calculateCanopyHeatPerUnitArea();
    calculateCrownFireHeatPerUnitArea();
    calculateCrownFirelineIntensity();
    calculateCrownFlameLength();
    calculateCrownCriticalFireSpreadRate();
    calculateCrownCriticalSurfaceFireIntensity();
    calculateCrownCriticalSurfaceFlameLength();
    calculateCrownFireActiveRatio();
    calculateCrownFireTransitionRatio();

    calculateCrownPowerOfFire();
    calculateWindSpeedAtTwentyFeet();
    calcuateCrownPowerOfWind();
    calculateCrownLengthToWidthRatio();
    calcualteCrownFirePowerRatio();

    // Determine if/what type of crown fire has occured
    calculateFireTypeRothermel();
    calculateFireTypeScottAndReinhardt();
 
    // Scott & Reinhardt's critical surface fire spread rate (R'initiation) (ft/min)
    calculateSurfaceFireCriticalSpreadRateScottAndReinhardt();

    // Scott & Reinhardt crown fraction burned
    surfaceFuel_.setWindSpeed(crownFireActiveWindSpeed_, SpeedUnits::FeetPerMinute, WindHeightInputMode::TwentyFoot);
    surfaceFuel_.doSurfaceRunInDirectionOfMaxSpread(); // Do crown run with crowning fire active wind speed
    crowningSurfaceFireRos_ = surfaceFuel_.getSpreadRate(SpeedUnits::FeetPerMinute);
    calculateCrownFractionBurned();

    // Scott & Reinhardt torching (passive crown) spread rate, hpua, fireline intensity
    passiveCrownFireSpreadRate_ = surfaceFireSpreadRate_
        + crownFractionBurned_ * (crownFireSpreadRate_ - surfaceFireSpreadRate_);
    passiveCrownFireHeatPerUnitArea_ = surfaceFireHeatPerUnitArea_ + canopyHeatPerUnitArea_ * crownFractionBurned_;
    passiveCrownFireLineIntensity_ = passiveCrownFireHeatPerUnitArea_ * passiveCrownFireSpreadRate_ / 60.0;

    // Scott & Reinhardt torching (passive) flame length
    calculatePassiveCrownFlameLength();
    
    // Determine final fire behavior
    if (isSurfaceFire_)
    {
        finalSpreadRate_ = surfaceFireSpreadRate_;
        finalHeatPerUnitArea_ = surfaceFireHeatPerUnitArea_;
        finalFirelineIntesity_ = surfaceFirelineIntensity_;
        finalFlameLength_ = surfaceFireFlameLength_;
    }
    else if (isPassiveCrownFire_)
    {
        finalSpreadRate_ = passiveCrownFireSpreadRate_;
        finalHeatPerUnitArea_ = passiveCrownFireHeatPerUnitArea_;
        finalFirelineIntesity_ = passiveCrownFireLineIntensity_;
        finalFlameLength_ = passiveCrownFireFlameLength_;
    }
    else if (isActiveCrownFire_)
    {
        finalSpreadRate_ = crownFireSpreadRate_;
        finalHeatPerUnitArea_ = crownFireHeatPerUnitArea_;
        finalFirelineIntesity_ = crownFirelineIntensity_;
        finalFlameLength_ = crownFlameLength_;
    }
}

void Crown::calculateCrownFractionBurned()
{
    // Calculates the crown fraction burned as per Scott & Reinhardt.
    // Using these parameters:
    // surfaceFireSpreadRate_: the "actual" surface fire spread rate (ft/min).
    // surfaceFireCriticalSpreadRate_: surface fire spread rate required to initiate torching/crowning (ft/min).
    // crowningSurfaceFireRos_: Surface fire spread rate at which the active crown fire spread rate is fully achieved 
    // and the crown fraction burned is 1.
    
    double numerator = surfaceFireSpreadRate_ - surfaceFireCriticalSpreadRate_;
    double denominator = crowningSurfaceFireRos_ - surfaceFireCriticalSpreadRate_;

    crownFractionBurned_ = (denominator > 1e-07) ? (numerator / denominator) : 0.0;
    crownFractionBurned_ = (crownFractionBurned_ > 1.0) ? 1.0 : crownFractionBurned_;
    crownFractionBurned_ = (crownFractionBurned_ < 0.0) ? 0.0 : crownFractionBurned_;
}

void Crown::calculateCrownFireActiveWindSpeed()
{
    // O'active is the 20-ft wind speed at which the crown canopy becomes fully available for active fire spread and:
    // the crown fraction burned approaches 1, Ractive == R'active, and the surface fire spread rate would equal R'sa.
    // See Scott & Reinhardt(2001) equation 20 on page 19.

    double cbd = 16.0185 * getCanopyBulkDensity(DensityUnits::PoundsPerCubicFoot);
    double ractive = 3.28084 * (3.0 / cbd);         // R'active, ft/min
    double r10 = ractive / 3.34;                    // R'active = 3.324 * R10
    double propFlux = 0.048317062998571636;         // Fuel model 10 actual propagating flux ratio
    double reactionIntensity = crownFuel_.getReactionIntensity(HeatSourceAndReactionIntensityUnits::BtusPerSquareFootPerMinute);
    double heatSink = crownFuel_.getHeatSink(HeatSinkUnits::BtusPerCubicFoot);
    double ros0 = reactionIntensity * propFlux / heatSink;
    double windB = 1.4308256324729873;              // Fuel model 10 actual wind factor B
    double windBInv = 1.0 / windB;                  // Fuel model 10 actual inverse of wind factor B
    double windK = 0.0016102128596515481;           // Fuel model 10 actual K = C*pow((beta/betOpt),-E)
    double slopeFactor = 0.0;
    double a = ((r10 / ros0) - 1.0 - slopeFactor) / windK;
    double uMid = pow(a, windBInv);                 // midflame wind speed (ft/min)
    crownFireActiveWindSpeed_ = uMid / 0.4;         // 20-ft wind speed (ft/min) for waf=0.4
}

double Crown::getCrownFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(crownFireSpreadRate_, spreadRateUnits);
}

double Crown::getSurfaceFireSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return surfaceFuel_.getSpreadRate(spreadRateUnits);
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

double Crown::getFinalSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(finalSpreadRate_, spreadRateUnits);
}

double Crown::getFinalHeatPerUnitArea() const
{
    return finalHeatPerUnitArea_;
}

double Crown::getFinalFirelineIntesity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const
{
    return FirelineIntensityUnits::fromBaseUnits(finalFirelineIntesity_, firelineIntensityUnits);
}

double Crown::getFinalFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
    return LengthUnits::fromBaseUnits(finalFlameLength_, flameLengthUnits);
}

double Crown::getCrownFireLengthToWidthRatio() const
{
    return crownFireLengthToWidthRatio_;
}

double Crown::getCriticalOpenWindSpeed(SpeedUnits::SpeedUnitsEnum speedUnits) const
{
    return SpeedUnits::fromBaseUnits(crownFireActiveWindSpeed_, speedUnits);
}

void Crown::initializeMembers()
{
    fireType_ = FireType::Surface;
    surfaceFireHeatPerUnitArea_ = 0.0;
    surfaceFirelineIntensity_ = 0.0;
    crownFuelLoad_ = 0.0;
    canopyHeatPerUnitArea_ = 0.0;
    crownFireHeatPerUnitArea_ = 0.0;
    crownFirelineIntensity_ = 0.0;
    crownFlameLength_ = 0.0;
    crownFireSpreadRate_ = 0.0;
    crownCriticalSurfaceFirelineIntensity_ = 0.0;
    crownCriticalFireSpreadRate_ = 0.0;
    crownCriticalSurfaceFlameLength_ = 0.0;
    crownPowerOfFire_ = 0.0;
    crownPowerOfWind_ = 0.0;
    crownFirePowerRatio_ = 0.0;
    crownFireActiveRatio_ = 0.0;
    crownFireTransitionRatio_ = 0.0;
    windSpeedAtTwentyFeet_ = 0.0;;
    crownFireLengthToWidthRatio_ = 1.0;

    surfaceFireSpreadRate_ = 0.0;
    surfaceFireCriticalSpreadRate_ = 0.0;

    passiveCrownFireSpreadRate_ = 0.0;
    passiveCrownFireHeatPerUnitArea_ = 0.0;
    passiveCrownFireLineIntensity_ = 0.0;
    passiveCrownFireFlameLength_ = 0.0;

    isSurfaceFire_ = false;
    isPassiveCrownFire_ = false;
    isActiveCrownFire_ = false;

    crownFireActiveWindSpeed_ = 0.0;
    
    crownInputs_.initializeMembers();
}

void Crown::calculateCanopyHeatPerUnitArea()
{
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lbs)
    canopyHeatPerUnitArea_ = crownFuelLoad_ * LOW_HEAT_OF_COMBUSTION;
}

void Crown::calculateCrownFireHeatPerUnitArea()
{
    crownFireHeatPerUnitArea_ = surfaceFireHeatPerUnitArea_ + canopyHeatPerUnitArea_;
}

void Crown::calculateCrownFuelLoad()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity(DensityUnits::PoundsPerCubicFoot);
    double canopyBaseHeight = crownInputs_.getCanopyBaseHeight(LengthUnits::Feet);
    double canopyHeight = surfaceFuel_.getCanopyHeight(LengthUnits::Feet);
    crownFuelLoad_ = canopyBulkDensity * (canopyHeight - canopyBaseHeight);
}

void Crown::calculateCrownFireTransitionRatio()
{
   crownFireTransitionRatio_ = ((crownCriticalSurfaceFirelineIntensity_ < 1.0e-7)
        ? (0.00)
        : (surfaceFirelineIntensity_ / crownCriticalSurfaceFirelineIntensity_));
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

    // Convert crown base height to meters and constrain lower limit
    double crownBaseHeight = crownInputs_.getCanopyBaseHeight(LengthUnits::Meters);
    crownBaseHeight = (crownBaseHeight < 0.1) ? 0.1 : crownBaseHeight;

    // Critical surface fireline intensity (kW/m)
    FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits = FirelineIntensityUnits::KilowattsPerMeter;
    crownCriticalSurfaceFirelineIntensity_ = pow((0.010 * crownBaseHeight * (460.0 + 25.9 * moistureFoliar)), 1.5);

    // Return as Btu/ft/s
    crownCriticalSurfaceFirelineIntensity_ = FirelineIntensityUnits::toBaseUnits(crownCriticalSurfaceFirelineIntensity_, firelineIntensityUnits);
}

void Crown::calculateCrownCriticalSurfaceFlameLength()
{
    crownCriticalSurfaceFlameLength_ = surfaceFuel_.calculateFlameLength(crownCriticalSurfaceFirelineIntensity_);
}

void Crown::calculateCrownFlameLength()
{
    // Uses Thomas's (1963) flame length (ft) given a fireline intensity (Btu/ft/s).
    if (crownFirelineIntensity_ <= 0.0)
    {
        crownFlameLength_ = 0.0;
    }
    else
    {
        crownFlameLength_ = 0.2 * pow(crownFirelineIntensity_, (2.0 / 3.0));
    }
}

void Crown::calculatePassiveCrownFlameLength()
{
    // Uses Thomas's (1963) flame length (ft) given a fireline intensity (Btu/ft/s).
    if (passiveCrownFireLineIntensity_ <= 0.0)
    {
        passiveCrownFireFlameLength_ = 0.0;
    }
    else
    {
        passiveCrownFireFlameLength_ = 0.2 * pow(passiveCrownFireLineIntensity_, (2.0 / 3.0));
    }
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


void Crown::calculateSurfaceFireCriticalSpreadRateScottAndReinhardt()
{
    // Scott & Reinhardt's critical surface fire spread rate (R'initiation) (ft/min)
    surfaceFireCriticalSpreadRate_ = (60. * crownCriticalSurfaceFirelineIntensity_) / surfaceFireHeatPerUnitArea_;
}

void Crown::calculateCrownCriticalFireSpreadRate()
{
    // Convert canopy bulk density to Kg/m3
    double convertedCanopyBulkDensity = crownInputs_.getCanopyBulkDensity(DensityUnits::KilogramsPerCubicMeter);
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
    windHeightInputMode = surfaceFuel_.getWindHeightInputMode();

    if (windHeightInputMode == WindHeightInputMode::TwentyFoot)
    {
        windSpeedAtTwentyFeet_ = surfaceFuel_.getWindSpeed(SpeedUnits::FeetPerMinute, windHeightInputMode);
    }
    else if (windHeightInputMode == WindHeightInputMode::TenMeter)
    {
        WindSpeedUtility windSpeedUtility;
        double windSpeedAtTenMeters = surfaceFuel_.getWindSpeed(SpeedUnits::FeetPerMinute, windHeightInputMode);
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

void Crown::calculateFireTypeRothermel()
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

void Crown::calculateFireTypeScottAndReinhardt()
{
    // Final fire type
    isSurfaceFire_ = fireType_ == FireType::Surface || fireType_ == FireType::ConditionalCrownFire;
    isPassiveCrownFire_ = fireType_ == FireType::Torching;
    isActiveCrownFire_ = fireType_ == FireType::Crowning;
    isCrownFire_ = isActiveCrownFire_ || isPassiveCrownFire_;
}

void Crown::updateCrownInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour, double moistureHundredHour,
    double moistureLiveHerbaceous, double moistureLiveWoody, double moistureFoliar, MoistureUnits::MoistureUnitsEnum moistureUnits,
    double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode,
    double windDirection, WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadOrientationMode, double slope,
    SlopeUnits::SlopeUnitsEnum slopeUnits, double aspect, double canopyCover, CoverUnits::CoverUnitsEnum coverUnits, double canopyHeight, 
    double canopyBaseHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits, double crownRatio, double canopyBulkDensity, 
    DensityUnits::DensityUnitsEnum densityUnits)
{
    surfaceFuel_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
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
    surfaceFuel_.updateSurfaceInputs(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, 
        moistureLiveHerbaceous, moistureLiveWoody, moistureUnits, windSpeed, windSpeedUnits, windHeightInputMode,
        windDirection, windAndSpreadOrientationMode, slope, slopeUnits, aspect, canopyCover, coverUnits,
        canopyHeight, canopyHeightUnits, crownRatio);
}

void  Crown::setCanopyCover(double canopyCover, CoverUnits::CoverUnitsEnum coverUnits)
{
    surfaceFuel_.setCanopyCover(canopyCover, coverUnits);
}

void  Crown::setCanopyHeight(double canopyHeight, LengthUnits::LengthUnitsEnum canopyHeightUnits)
{
    surfaceFuel_.setCanopyHeight(canopyHeight, canopyHeightUnits);
}

void  Crown::setCrownRatio(double crownRatio)
{
    surfaceFuel_.setCrownRatio(crownRatio);
}

void Crown::setFuelModelSet(FuelModelSet & fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet;
}


void  Crown::setFuelModelNumber(int fuelModelNumber)
{
    surfaceFuel_.setFuelModelNumber(fuelModelNumber);
}

void  Crown::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceFuel_.setMoistureOneHour(moistureOneHour, moistureUnits);
}

void  Crown::setMoistureTenHour(double moistureTenHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceFuel_.setMoistureTenHour(moistureTenHour, moistureUnits);
}

void  Crown::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceFuel_.setMoistureHundredHour(moistureHundredHour, moistureUnits);
}

void  Crown::setMoistureLiveHerbaceous(double moistureLiveHerbaceous, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceFuel_.setMoistureLiveHerbaceous(moistureLiveHerbaceous, moistureUnits);
}

void  Crown::setMoistureLiveWoody(double moistureLiveWoody, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    surfaceFuel_.setMoistureLiveWoody(moistureLiveWoody, moistureUnits);
}

void  Crown::setSlope(double slope, SlopeUnits::SlopeUnitsEnum slopeUnits)
{
    surfaceFuel_.setSlope(slope, slopeUnits);
}

void  Crown::setAspect(double aspect)
{
    surfaceFuel_.setAspect(aspect);
}

void  Crown::setWindSpeed(double windSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surfaceFuel_.setWindSpeed(windSpeed, windSpeedUnits, windHeightInputMode);
}

void  Crown::setWindDirection(double windDirection)
{
    surfaceFuel_.setWindDirection(windDirection);
}

void Crown::setWindHeightInputMode(WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode)
{
    surfaceFuel_.setWindHeightInputMode(windHeightInputMode);
}

void  Crown::setWindAndSpreadOrientationMode(WindAndSpreadOrientationMode::WindAndSpreadOrientationModeEnum windAndSpreadAngleMode)
{
    surfaceFuel_.setWindAndSpreadOrientationMode(windAndSpreadAngleMode);
}

void Crown::setUserProvidedWindAdjustmentFactor(double userProvidedWindAdjustmentFactor)
{
    surfaceFuel_.setUserProvidedWindAdjustmentFactor(userProvidedWindAdjustmentFactor);
}

void Crown::setWindAdjustmentFactorCalculationMethod(WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod)
{
    surfaceFuel_.setWindAdjustmentFactorCalculationMethod(windAdjustmentFactorCalculationMethod);
}

int Crown::getFuelModelNumber() const
{
    return surfaceFuel_.getFuelModelNumber();
}

double Crown::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceFuel_.getMoistureOneHour(moistureUnits);
}

double Crown::getMoistureTenHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceFuel_.getMoistureTenHour(moistureUnits);
}

double Crown::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceFuel_.getMoistureHundredHour(moistureUnits);
}

double Crown::getMoistureLiveHerbaceous(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceFuel_.getMoistureLiveHerbaceous(moistureUnits);
}

double Crown::getMoistureLiveWoody(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return surfaceFuel_.getMoistureLiveWoody(moistureUnits);
}

double Crown::getWindSpeed(SpeedUnits::SpeedUnitsEnum windSpeedUnits, WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode) const
{
    return surfaceFuel_.getWindSpeed(windSpeedUnits, windHeightInputMode);
}

double Crown::getWindDirection() const
{
    return surfaceFuel_.getWindDirection();
}

double Crown::getSlope(SlopeUnits::SlopeUnitsEnum slopeUnits) const
{
    return surfaceFuel_.getSlope(slopeUnits);
}

double Crown::getAspect() const
{
    return surfaceFuel_.getAspect();
}

double Crown::getCanopyCover(CoverUnits::CoverUnitsEnum canopyCoverUnits) const
{
    return surfaceFuel_.getCanopyCover(canopyCoverUnits);
}

double Crown::getCanopyHeight(LengthUnits::LengthUnitsEnum canopyHeighUnits) const
{
    return surfaceFuel_.getCanopyHeight(canopyHeighUnits);
}

double Crown::getCrownRatio() const
{
    return surfaceFuel_.getCrownRatio();
}

double Crown::getCanopyBaseHeight(LengthUnits::LengthUnitsEnum canopyHeightUnits) const
{
    return crownInputs_.getCanopyBaseHeight(canopyHeightUnits);
}

double Crown::getCanopyBulkDensity(DensityUnits::DensityUnitsEnum canopyBulkDensityUnits) const
{
    return crownInputs_.getCanopyBulkDensity(canopyBulkDensityUnits);
}

double Crown::getMoistureFoliar(MoistureUnits::MoistureUnitsEnum moistureUnits) const
{
    return crownInputs_.getMoistureFoliar(moistureUnits);
}
