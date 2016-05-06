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
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lbs)
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

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire transition ratio.
*
*  \param surfaceFireInt   Surface fireline intensity (Btu/ft/s).
*  \param criticalFireInt  Critical crown fire fireline intensity (Btu/ft/s).
*
*  \return Transition ratio.
*/
double Crown::calculateCrownFireTransitionRatio(double surfaceFireIntensity,
    double criticalFireIntensity)
{
    return((criticalFireIntensity < 1.0e-7)
        ? (0.00)
        : (surfaceFireIntensity / criticalFireIntensity));
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
    return((crownFireSpreadRate_ / 60.) * (crownFireHeatPerUnitArea_));
}
