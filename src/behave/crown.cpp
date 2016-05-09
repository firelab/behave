#include "crown.h"

#include "fuelModelSet.h"

Crown::Crown(const FuelModelSet& fuelModelSet, const CrownInputs& crownInputs, const SurfaceInputs& surfaceInputs, 
    double surfaceHeatPerUnitArea, double surfaceFirelineIntensity)
    : crownFireSpread_(fuelModelSet, crownDeepCopyOfSurfaceInputs_)
{
    crownInputs_ = &crownInputs;  // points to the same location as crownInputs
    fuelModelSet_ = &fuelModelSet; // points to the same location as fuelModels
    surfaceInputs_ = &surfaceInputs; // points to the same location as surfaceInputs
    crownDeepCopyOfSurfaceInputs_ = *surfaceInputs_; // copy the actual data surfaceInputs is pointing to

    crownCopyOfSurfaceHeatPerUnitArea_ = surfaceHeatPerUnitArea;
    crownCopyOfSurfaceFirelineIntensity_ = surfaceFirelineIntensity;
}

Crown::~Crown()
{

}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire spread rate.
*
*  This uses Rothermel's 1991 crown fire correlation.
*
*  \return Crown fire average spread rate (ft/min).
*
*/
double Crown::calculateCrownFireSpreadRate(double windSpeedAtTwentyFeet)
{
    // Step 1: Create the crown fuel model (fire behavior fuel model 10)
    crownDeepCopyOfSurfaceInputs_.setFuelModelNumber(10);    // set the fuel model used to fuel model 10
    crownDeepCopyOfSurfaceInputs_.setSlope(0.0);             // slope is always assumed to be zero in crown ROS
    crownDeepCopyOfSurfaceInputs_.setWindDirection(0.0);     // wind direction is assumed to be upslope in crown ROS
    double windAdjustmentFactor = 0.4;      // wind adjustment factor is assumed to be 0.4 for crown ROS
    double midflameWindSpeed = windAdjustmentFactor * windSpeedAtTwentyFeet;
    crownDeepCopyOfSurfaceInputs_.setWindSpeed(midflameWindSpeed);

    // Step 2: Determine fire behavior.
    double rateOfSpread = crownFireSpread_.calculateForwardSpreadRate();
    crownFireSpreadRate_ = 3.34 * rateOfSpread; // Rothermel 1991

    return crownFireSpreadRate_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the canopy portion of the crown fire heat per unit area
*  given the crown fire fuel load and low heat of combustion.
*
*/
double Crown::calculateCanopyHeatPerUnitArea()
{
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lbs)
    canopyHeatPerUnitArea_ = crownFuelLoad_ * LOW_HEAT_OF_COMBUSTION;

    return canopyHeatPerUnitArea_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the total crown fire heat per unit area
*  by summing surface HPUA and canopy HPUA.
*
*/
double Crown::calculateCrownFireHeatPerUnitArea()
{
    crownFireHeatPerUnitArea_ = crownCopyOfSurfaceHeatPerUnitArea_ + canopyHeatPerUnitArea_;

    return crownFireHeatPerUnitArea_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fuel load
*  given the canopy bulk density and canopy height.
*
*/
double Crown::calculateCrownFuelLoad()
{
    double canopyBulkDensity = crownInputs_->getCanopyBulkDensity();
    double canopyBaseHeight = crownInputs_->getCanopyBaseHeight();
    double canopyHeight = surfaceInputs_->getCanopyHeight();
    crownFuelLoad_ = canopyBulkDensity * (canopyHeight - canopyBaseHeight);

    return crownFuelLoad_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire transition ratio.
*
*  \param crownCopyOfSurfaceFirelineIntensity_  Surface fireline intensity (Btu/ft/s).
*  \param crownCriticalFireIntensity  Critical crown fire fireline intensity (Btu/ft/s).
*
*  \return Transition ratio.
*/
double Crown::calculateCrownFireTransitionRatio()
{
    double crownFireTransitionRatio = ((crownCriticalSurfaceFireIntensity_ < 1.0e-7)
        ? (0.00)
        : (crownCopyOfSurfaceFirelineIntensity_ / crownCriticalSurfaceFireIntensity_));

    return crownFireTransitionRatio;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fireline intensity
*  given the surface fire and crown fire heats per unit area
*  and the crown fire spread rate.
*
*  \param crownFireHpua Crown fire (surface + canopy) heat per unit area (Btu/ft2).
*  \param crownFireSpreadRate Crown fire rate of spread (ft/min).
*
*  \return Crown fire fireline intensity (Btu/ft/s).
*/

double Crown::calculateCrownFireFirelineIntensity()
{
    crownFireFirelineIntensity_ = (crownFireSpreadRate_ / 60.0) * crownFireHeatPerUnitArea_;
    return crownFireFirelineIntensity_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical surface fire intensity for a surface fire
*  to transition to a crown fire.
*
*  \param foliarMoisture   Tree foliar moisture content (lb water/lb foliage).
*  \param crownBaseHt      Tree crown base height (ft).
*
*  \return Critical surface fire intensity (Btu/ft/s).
*/
double Crown::calculateCrownFireCriticalSurfaceFireIntensity()
{
    const double KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND = 0.288672;
    const double FEET_TO_METERS = 0.3048;

    double foliarMoisture = crownInputs_->getFoliarMoisture();

    // Convert foliar moisture content to percent and constrain lower limit
    // double foliarMoisture *= 100.0;
    foliarMoisture = (foliarMoisture < 30.0) ? 30.0 : foliarMoisture;

    double crownBaseHeight = crownInputs_->getCanopyBaseHeight();
    // Convert crown base heigt to meters and constrain lower limit
    crownBaseHeight *= FEET_TO_METERS;
    crownBaseHeight = (crownBaseHeight < 0.1) ? 0.1 : crownBaseHeight;
    // Critical surface fireline intensity (kW/m)
    // Need to changed value in calculation below from 450 to 460 at some point
    crownCriticalSurfaceFireIntensity_ = pow((0.010 * crownBaseHeight * (450.0 + 25.9 * foliarMoisture)), 1.5);

    // Return as Btu/ft/s
    crownCriticalSurfaceFireIntensity_ *= KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND;

    return crownCriticalSurfaceFireIntensity_;
}

double Crown::calculateCrownFireCriticalSurfaceFlameLength()
{
    double criticalSurfaceFlameLength = crownFireSpread_.calculateFlameLength(crownCriticalSurfaceFireIntensity_);
    return criticalSurfaceFlameLength;
}

double Crown::calculateCrownFireFlameLength()
{
    crownFireFlameLength_ = 0.2 * pow(crownFireFirelineIntensity_, (2.0 / 3.0));
    //return(0.2 * pow(crownFireFirelineIntensity_, (2.0 / 3.0)));
    return crownFireFlameLength_;
}
