#include "crown.h"

#include <iostream> // Test

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

double Crown::CrownFireSpreadRate()
{
	//--------------------------------------------------------------------------
	// Step 1: Create the crown fuel model (fire behavior fuel model 10)
	//--------------------------------------------------------------------------
    crownInputs_.setFuelModelNumber(10);
    double windAtTwentyFeet = surfaceInputs_->getWindSpeed();
    crownInputs_.setSlope(0.0);
    crownInputs_.setWindDirection(0.0);
    double windAdjustmentFactor = 0.4; // wind adjustment factor is always set to 0.4
    double midflameWindSpeed = 0.4 * windAtTwentyFeet;
    crownInputs_.setWindSpeed(midflameWindSpeed);

	////--------------------------------------------------------------------------
	//// Step 2: Determine fire behavior.
	////--------------------------------------------------------------------------
    double ros = 0;
    ros = crownFireSpread_.calculateForwardSpreadRate();

	//// Rothermel 1991
	double crownRos = 3.34 * ros;

	return(crownRos);
}
