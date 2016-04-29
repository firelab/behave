#include "crown.h"

#include "fuelModels.h"
#include "surfaceFireSpread.h"

Crown::Crown(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs)
    : crownFireSpread_(fuelModels, crownInputs_)
{
    fuelModels_ = &fuelModels;
    surfaceInputs_ = &surfaceInputs;
    crownInputs_ = *surfaceInputs_;
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
    crownInputs_.setFuelModelNumber(10);    // set the fuel model used to fuel model 10
    crownInputs_.setSlope(0.0);             // slope is always assumed to be zero in crown ROS
    crownInputs_.setWindDirection(0.0);     // wind direction is assumed to be upslope in crown ROS
    double windAdjustmentFactor = 0.4;      // wind adjustment factor is assumed to be 0.4 for crown ROS
    double midflameWindSpeed = windAdjustmentFactor * windSpeedAtTwentyFeet;
    crownInputs_.setWindSpeed(midflameWindSpeed);

    // Step 2: Determine fire behavior.
    double ros = crownFireSpread_.calculateForwardSpreadRate();
    double crownRos = 3.34 * ros; // Rothermel 1991

    return crownRos;
}
