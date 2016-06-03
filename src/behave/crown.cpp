#include "crown.h"

#include <cmath>
#include "fuelModelSet.h"
#include "surfaceEnums.h"
#include "windSpeedUtility.h"

Crown::Crown(const FuelModelSet& fuelModelSet, const Surface& surface)
    : crownDeepCopyOfSurface_(fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet; // point to the same location as BehaveRun'sfuelModels
    surface_ = &surface; // point to the same location as BehaveRun's surface

    crownDeepCopyOfSurface_ = *surface_; // copy the actual data surfaceInputs is pointing to
}

Crown::~Crown()
{

}

Crown::Crown(const Crown &rhs)
    : crownDeepCopyOfSurface_(*rhs.fuelModelSet_)
{
    fuelModelSet_ = rhs.fuelModelSet_;
    surface_ = rhs.surface_;
    crownDeepCopyOfSurface_ = rhs.crownDeepCopyOfSurface_;
    crownInputs_ = rhs.crownInputs_;
}

Crown& Crown::operator= (const Crown& rhs)
{
    if (this != &rhs)
    {
        fuelModelSet_ = rhs.fuelModelSet_;
        surface_ = rhs.surface_;
        crownDeepCopyOfSurface_ = rhs.crownDeepCopyOfSurface_;
        crownInputs_ = rhs.crownInputs_;
    }
    return *this;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire spread rate.
*
*  This uses Rothermel's 1991 crown fire correlation.
*
*  \return Crown fire average spread rate (ft/min).
*
*/
double Crown::calculateCrownFireSpreadRate()
{
    // Step 1: Create the crown fuel model (fire behavior fuel model 10)
    crownDeepCopyOfSurface_.setFuelModelNumber(10);    // set the fuel model used to fuel model 10
    crownDeepCopyOfSurface_.setSlope(0.0);             // slope is always assumed to be zero in crown ROS
    crownDeepCopyOfSurface_.setWindDirection(0.0);     // wind direction is assumed to be upslope in crown ROS
    double windAdjustmentFactor = 0.4;      // wind adjustment factor is assumed to be 0.4 for crown ROS

    windSpeedAtTwentyFeet_ = calculateWindSpeedAtTwentyFeet();
    double midflameWindSpeed = windAdjustmentFactor * windSpeedAtTwentyFeet_;
    crownDeepCopyOfSurface_.setWindSpeed(midflameWindSpeed);

    // Step 2: Determine fire behavior.
    crownFireSpreadRate_ = 3.34 * crownDeepCopyOfSurface_.calculateSurfaceFireForwardSpreadRate();  // Rothermel 1991

    //  Step 3:  Get values from Surface needed for further calculations 
    crownCopyOfSurfaceHeatPerUnitArea_ = surface_->getHeatPerUnitArea();
    crownCopyOfSurfaceFirelineIntensity_ = surface_->getFirelineIntensity();

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
    calcuateCrownPowerOfWind();
    calcualteCrownFirePowerRatio();

    return crownFireSpreadRate_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the canopy portion of the crown fire heat per unit area
*  given the crown fire fuel load and low heat of combustion.
*
*/
void Crown::calculateCanopyHeatPerUnitArea()
{
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lbs)
    canopyHeatPerUnitArea_ = crownFuelLoad_ * LOW_HEAT_OF_COMBUSTION;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the total crown fire heat per unit area
*  by summing surface HPUA and canopy HPUA.
*
*/
void Crown::calculateCrownFireHeatPerUnitArea()
{
    crownFireHeatPerUnitArea_ = crownCopyOfSurfaceHeatPerUnitArea_ + canopyHeatPerUnitArea_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fuel load
*  given the canopy bulk density and canopy height.
*
*/
void Crown::calculateCrownFuelLoad()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity();
    double canopyBaseHeight = crownInputs_.getCanopyBaseHeight();
    double canopyHeight = surface_->getCanopyHeight();
    crownFuelLoad_ = canopyBulkDensity * (canopyHeight - canopyBaseHeight);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire transition ratio.
*
*  given the surface fireline intensity (Btu/ft/s) and the
*  critical crown fire fireline intensity (Btu/ft/s).
*
*/
void Crown::calculateCrownFireTransitionRatio()
{
   crownFireTransitionRatio_ = ((crownCriticalSurfaceFireIntensity_ < 1.0e-7)
        ? (0.00)
        : (crownCopyOfSurfaceFirelineIntensity_ / crownCriticalSurfaceFireIntensity_));
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fireline intensity
*  given the surface fire and crown fire heats per unit area
*  and the crown fire spread rate.
*
*
*/
void Crown::calculateCrownFirelineIntensity()
{
    crownFirelineIntensity_ = (crownFireSpreadRate_ / 60.0) * crownFireHeatPerUnitArea_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical surface fire intensity for a surface fire
*  to transition to a crown fire given the foliar moisture and crown base height
*
*/
void Crown::calculateCrownCriticalSurfaceFireIntensity()
{
    const double KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND = 0.288672;
    const double FEET_TO_METERS = 0.3048;

    double foliarMoisture = crownInputs_.getFoliarMoisture();

    // Convert foliar moisture content to percent and constrain lower limit
    // double foliarMoisture *= 100.0;
    foliarMoisture = (foliarMoisture < 30.0) ? 30.0 : foliarMoisture;

    double crownBaseHeight = crownInputs_.getCanopyBaseHeight();
    // Convert crown base heigt to meters and constrain lower limit
    crownBaseHeight *= FEET_TO_METERS;
    crownBaseHeight = (crownBaseHeight < 0.1) ? 0.1 : crownBaseHeight;
    // Critical surface fireline intensity (kW/m)
    // Need to changed value in calculation below from 450 to 460 at some point
    crownCriticalSurfaceFireIntensity_ = pow((0.010 * crownBaseHeight * (450.0 + 25.9 * foliarMoisture)), 1.5);

    // Return as Btu/ft/s
    crownCriticalSurfaceFireIntensity_ *= KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical surface fire flame length for a surface fire
*  to transition to a crown fire given the critical fireline intensity.
*
*/
void Crown::calculateCrownCriticalSurfaceFlameLength()
{
    crownCriticalSurfaceFlameLength_ = crownDeepCopyOfSurface_.calculateFlameLength(crownCriticalSurfaceFireIntensity_);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire flame length
*  given the crown fireline intensity.
*
*/
void Crown::calculateCrownFlameLength()
{
    crownFlameLength_ = 0.2 * pow(crownFirelineIntensity_, (2.0 / 3.0));
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire 'power of the fire'
*  given the crown fireline intensity.
*
*/
void Crown::calculateCrownPowerOfFire()
{
    crownPowerOfFire_ = crownFirelineIntensity_ / 129.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire 'power of the wind'
*  given the 20-ft wind speed and crown fire spread rate.
*
*/
void Crown::calcuateCrownPowerOfWind()
{
    const double MILES_PER_HOUR_TO_FEET_PER_MINUTE = 5280.0 / 60.0;
    const double SECONDS_PER_MINUTE = 60.0;
    
    windSpeedAtTwentyFeet_ = calculateWindSpeedAtTwentyFeet();

    double windSpeedInFeetPerMinute = windSpeedAtTwentyFeet_ * MILES_PER_HOUR_TO_FEET_PER_MINUTE;

    double WindspeedMinusCrownROS = 0.0; // Eq. 7, Rothermel 1991

    WindspeedMinusCrownROS = (windSpeedInFeetPerMinute - crownFireSpreadRate_) / SECONDS_PER_MINUTE;
    WindspeedMinusCrownROS = (WindspeedMinusCrownROS < 1e-07) ? 0.0 : WindspeedMinusCrownROS;
    crownPowerOfWind_ = 0.00106 * (WindspeedMinusCrownROS * WindspeedMinusCrownROS * WindspeedMinusCrownROS);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire 'power-of-fire to power-of-wind' ratio
*
*  \return Ratio of the crown fire 'power-of-the-fire' to 'power-of-the-wind).
*/
void Crown::calcualteCrownFirePowerRatio()
{
    crownFirePowerRatio_ = (crownPowerOfWind_ > 1e-07) ? (crownPowerOfFire_ / crownPowerOfWind_) : 0.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical crown fire spread rate to achieve active
*  crowning given the canopy crown bulk density
*
*/
void Crown::calculateCrownCriticalFireSpreadRate()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity();
    const double LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER = 16.0185;
    // Convert to Kg/m3
    double convertedBulkDensity = LBS_PER_CUBIC_FOOT_TO_KG_PER_CUBIC_METER * canopyBulkDensity;
    crownCriticalFireSpreadRate_ = (convertedBulkDensity < 1e-07) ? 0.00 : (3.0 / convertedBulkDensity);
    const double METERS_PER_MIN_TO_FEET_PER_MIN = 3.28084;
    // Convert to ft/min
    crownCriticalFireSpreadRate_ *= METERS_PER_MIN_TO_FEET_PER_MIN;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire active ratio given the crown fire spread 
*  rate and the critical crown fire spread rate.
*
*/
void Crown::calculateCrownFireActiveRatio()
{
    crownFireActiveRatio_ = (crownCriticalFireSpreadRate_ < 1e-07)
        ? (0.00)
        : (crownFireSpreadRate_ / crownCriticalFireSpreadRate_);
}

double Crown::calculateWindSpeedAtTwentyFeet()
{
    windSpeedAtTwentyFeet_ = -1; // If negative 1 is returned, there is an error
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode;
    windHeightInputMode = surface_->getWindHeightInputMode();

    if (windHeightInputMode == WindHeightInputMode::TWENTY_FOOT)
    {
        windSpeedAtTwentyFeet_ = surface_->getWindSpeed();
    }
    else if (windHeightInputMode == WindHeightInputMode::TEN_METER)
    {
        WindSpeedUtility windSpeedUtility;
        double windSpeedAtTenMeters = surface_->getWindSpeed();
        windSpeedAtTwentyFeet_ = windSpeedUtility.windSpeedAtTwentyFeetFromTenMeter(windSpeedAtTenMeters);
    }
    return windSpeedAtTwentyFeet_;
}

void Crown::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    //const SurfaceInputs *const surfaceInputsPtr = &(surface_->getSurfaceInputs()); // copy the actual data surfaceInputs is pointing to
    crownDeepCopyOfSurface_ = *surface_; // copy the actual data surface_ is pointing to
    crownInputs_.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
}
