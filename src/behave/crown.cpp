#include "crown.h"

#include "fuelModels.h"

Crown::Crown(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs, const CrownInputs& crownInputs, double surfaceHeatPerUnitArea)
    : crownFireSpread_(fuelModels, crownDeepCopyOfSurfaceInputs_)
{
    crownInputs_ = &crownInputs;  // points to the same location as crownInputs
    fuelModels_ = &fuelModels; // points to the same location as fuelModels
    surfaceInputs_ = &surfaceInputs; // points to the same location as surfaceInputs
    crownDeepCopyOfSurfaceInputs_ = *surfaceInputs_; // copy the actual data surfaceInputs is pointing to

    crownCopyOfSurfaceHeatPerUnitArea_ = surfaceHeatPerUnitArea;
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
    crownRateOfSpread_ = 3.34 * rateOfSpread; // Rothermel 1991
    return crownRateOfSpread_;
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
/*! \brief Calculates the canopy portion of the crown fire heat per unit area
*  given the crown fire fuel load and low heat of combustion.
*
*/
void Crown::calculateCanopyHeatPerUnitArea()
{
    canopyHeatPerUnitArea_ = crownFuelLoad_ * LOW_HEAT_OF_COMBUSTION;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fuel load
*  given the canopy bulk density and canopy height.
*
*/
void Crown::calculateCrownFuelLoad()
{
    double canopyBulkDensity = crownInputs_->getCanopyBulkDensity();
    double canopyBaseHeight = crownInputs_->getCanopyBaseHeight();
    double canopyHeight = surfaceInputs_->getCanopyHeight();
    crownFuelLoad_ = canopyBulkDensity * (canopyHeight - canopyBaseHeight);
}
