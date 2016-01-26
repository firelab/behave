#include "surfaceInputs.h"

SurfaceInputs::SurfaceInputs()
{
	initializeMembers();
}

SurfaceInputs::SurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
	double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
	double midflameWindSpeed, double windDirFromUpslope, double slope, double slopeAspect)
{
	updateInput(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
		moistureLiveWoody, midflameWindSpeed, windDirFromUpslope, slope, slopeAspect);
}

void SurfaceInputs::initializeMembers()
{
	fuelModelNumber_ = 0;
	moistureOneHour_ = 0.0;
	moistureTenHour_ = 0.0;
	moistureHundredHour_ = 0.0;
	moistureLiveHerbaceous_ = 0.0;
	moistureLiveWoody_ = 0.0;
	slope_ = 0.0;
	slopeAspect_ = 0.0;
	midflameWindSpeed_ = 0.0;
	windDirection_ = 0.0;

	slopeInputMode_ = SLOPE_IN_PERCENT;
	windAndSpreadAngleMode_ = FROM_UPSLOPE;

	for (int i = 0; i < MAX_SIZES; i++)
	{
		moistureDead_[i] = 0.0;
		moistureLive_[i] = 0.0;
	}
}

void SurfaceInputs::updateInput(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
	double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
	double midflameWindSpeed, double windDirection, double slope, double slopeAspect)
{
	for (int i = 0; i < MAX_SIZES; i++)
	{
		moistureDead_[i] = 0.0;
		moistureLive_[i] = 0.0;
	}

	setSlope(slope);
	slopeAspect_ = slopeAspect;

	//	To Fix Original BehavePlus's reporting 360 as direction of max spread,
	//		just add an equaul sign after the less than or greater than sign 
	//		in the next 2 conditional statements 
	if (windDirection < 0.0)
	{
		windDirection += 360.0;
	}
	while (windDirection > 360.0)
	{
		windDirection -= 360.0;
	}

	if (windAndSpreadAngleMode_ == FROM_NORTH)
	{
		windDirection = convertWindToUpslope(windDirection);
	}

	fuelModelNumber_ = fuelModelNumber;
	moistureOneHour_ = moistureOneHour;
	moistureTenHour_ = moistureTenHour;
	moistureHundredHour_ = moistureHundredHour;
	moistureLiveHerbaceous_ = moistureLiveHerbaceous;
	moistureLiveWoody_ = moistureLiveWoody;
	midflameWindSpeed_ = midflameWindSpeed;
	windDirection_ = windDirection;
	
	setMoistureDead();
	setMoistureLive();
}

SurfaceInputs::WindAndSpreadDirectionEnumType SurfaceInputs::getWindAndSpreadAngleMode() const
{
	return windAndSpreadAngleMode_;
}

void SurfaceInputs::setWindAndSpreadDirectionMode(int mode)
{
	if (mode == FROM_UPSLOPE)
	{
		windAndSpreadAngleMode_ = FROM_UPSLOPE;
	}
	if (mode == FROM_NORTH)
	{
		windAndSpreadAngleMode_ = FROM_NORTH;
	}
}

double SurfaceInputs::convertWindToUpslope(double windDirectionFromNorth)
{
	// Important information: 
	// when wind is given relativet to upslope, it is given as the direction the wind pushes the fire, not the direction from which is blowing - WMC 01/2016
	//double windDirectionFromUpslope = windDirectionFromNorth - slopeAspect_  + 180; // wind direction is now in degrees clockwise relative to blowing in the upslope direction
	double windDirectionFromUpslope = windDirectionFromNorth - slopeAspect_; // wind direction is now in degrees clockwise relative to blowing in the upslope direction
	return windDirectionFromUpslope; 
}

void SurfaceInputs::setFuelModelNumber(int fuelModelNumber)
{
	fuelModelNumber_ = fuelModelNumber;
}

void SurfaceInputs::setMoistureDead()
{
	moistureDead_[0] = moistureOneHour_;
	moistureDead_[1] = moistureTenHour_;
	moistureDead_[2] = moistureHundredHour_;
	moistureDead_[3] = moistureOneHour_;
}

void SurfaceInputs::setMoistureLive()
{
	moistureLive_[0] = moistureLiveHerbaceous_;
	moistureLive_[1] = moistureLiveWoody_;
}

void SurfaceInputs::setMoistureDead(double moistureOneHour, double moistureTenHour, double moistureHundredHour)
{
	moistureDead_[0] = moistureOneHour;
	moistureDead_[1] = moistureTenHour;
	moistureDead_[2] = moistureHundredHour;
	moistureDead_[3] = moistureOneHour;
}

void SurfaceInputs::setMoistureLive(double moistureLiveHerbaceous, double moistureLiveWoody)
{
	moistureLive_[0] = moistureLiveHerbaceous;
	moistureLive_[1] = moistureLiveWoody;
}

void SurfaceInputs::setMoistureOneHour(double moistureOneHour)
{
	moistureDead_[0] = moistureOneHour;
	moistureDead_[3] = moistureOneHour;
}

void SurfaceInputs::setMoistureTenHour(double moistureTenHour)
{ 
	moistureDead_[1] = moistureTenHour;
}

void SurfaceInputs::setMoistureHundredHour(double moistureHundredHour)
{ 
	moistureDead_[2] = moistureHundredHour;
}

void SurfaceInputs::setMoistureLiveHerbaceous(double moistureLiveHerbaceous)
{
	moistureLive_[0] = moistureLiveHerbaceous_;
}

void SurfaceInputs::setMoistureLiveWoody(double moistureLiveWoody)
{
	moistureLive_[1] = moistureLiveWoody_;
}

void SurfaceInputs::setSlope(double slope)
{
	if (getSlopeInputMode() == SLOPE_IN_PERCENT)
	{
		const double PI = 3.141592653589793238463;
		slope = (180 / PI) * atan(slope / 100); // slope is now in degees
	}
	slope_ = slope; 
}

void SurfaceInputs::setSlopeAspect(double slopeAspect)
{
	slopeAspect_ = slopeAspect;
}

void SurfaceInputs::setSlopeInputMode(int mode)
{
	if (mode == SLOPE_IN_PERCENT)
	{
		slopeInputMode_ = SLOPE_IN_PERCENT;
	}
	if (mode == SLOPE_IN_DEGREES)
	{
		slopeInputMode_ = SLOPE_IN_DEGREES;
	}
}

void SurfaceInputs::setMidflameWindSpeed(double midflameWindSpeed)
{ 
	midflameWindSpeed_ = midflameWindSpeed; 
}

double SurfaceInputs::getMoistureDeadAtIndex(int index) const
{ 
	return moistureDead_[index];
}
double SurfaceInputs::getMoistureLiveAtIndex(int index) const
{ 
	return moistureLive_[index];
}

int SurfaceInputs::getFuelModelNumber() const
{ 
	return fuelModelNumber_; 
}

double SurfaceInputs::getSlope() const
{ 
	return slope_; 
}

double SurfaceInputs::getSlopeAspect() const
{
	return slopeAspect_;
}

SurfaceInputs::SlopeInputModeEnumType SurfaceInputs::getSlopeInputMode() const
{
	return slopeInputMode_;
}

double SurfaceInputs::getWindDirection() const
{ 
	return windDirection_; 
}

double SurfaceInputs::getMidflameWindSpeed() const 
{
	return midflameWindSpeed_; 
}

double SurfaceInputs::getMoistureOneHour() const
{
	return moistureOneHour_;
}

double SurfaceInputs::getMoistureTenHour() const
{
	return moistureTenHour_;
}

double SurfaceInputs::getMoistureHundredHour() const
{
	return moistureHundredHour_;
}

double SurfaceInputs::getMoistureLiveHerbaceous() const
{
	return moistureLiveHerbaceous_;
}

double SurfaceInputs::getMoistureLiveWoody() const
{
	return moistureLiveWoody_;
}
