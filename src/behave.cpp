#include "behave.h"

#include <iostream>

Behave::Behave()
	: surfaceInputs_{}, surface_{ fuelModels_, surfaceInputs_ }
{
	// Default Constructor
}

Behave::Behave(SurfaceInputs &surfaceInputs)
	: surface_{ fuelModels_, surfaceInputs }
{
	// Constructor taking a SurfaceInputs object as a parameter
}

Behave::Behave(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
	double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
	double midflameWindSpeed, double windDirection, double slope, double slopeAspect)
	: surfaceInputs_(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour,
	moistureLiveHerbaceous, moistureLiveWoody, midflameWindSpeed, windDirection, slope, slopeAspect),
	surface_{ fuelModels_, surfaceInputs_ }
{
	// Constructor takes a list of doubles as parameters
}

Behave::~Behave()
{
	// Default Destructor
}

void Behave::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
	double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
	double midflameWindSpeed, double windDirection, double slope, double slopeAspect)
{
	surfaceInputs_.updateInput(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
		moistureLiveWoody, midflameWindSpeed, windDirection, slope, slopeAspect);
}

void Behave::useTwoFuelModels(int firstFuelModel, int secondFuelModel, TwoFuelModelsMethod method)
{

}

double Behave::calculateSurfaceFireForwardSpreadRate(double directionOfInterest)
{
	// Calculate Spread Rate
	return surface_.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
}

void Behave::setWindAndSpreadDirectionMode(WindAndSpreadDirectionEnumType mode)
{
	surfaceInputs_.setWindAndSpreadDirectionMode(mode);
}

void Behave::setSlopeInputMode(SlopeInputModeEnumType mode)
{
	surfaceInputs_.setSlopeInputMode(mode);
}

void Behave::setWindAndSpreadAnglesRelativeToNorth()
{
	surfaceInputs_.setWindAndSpreadDirectionMode(Behave::RELATIVE_TO_NORTH);
}

void Behave::setWindAndSpreadAnglesRelativeToUpslope()
{
	surfaceInputs_.setWindAndSpreadDirectionMode(Behave::RELATIVE_TO_UPSLOPE);
}

void Behave::setSlopeInputToPercent()
{
	surfaceInputs_.setSlopeInputMode(SLOPE_IN_PERCENT);
}

void Behave::setSlopeInputToDegrees()
{
	surfaceInputs_.setSlopeInputMode(SLOPE_IN_DEGREES);
}

bool Behave::isWindAndSpreadAngleRelativeToNorth() const
{
	bool isRelativeToNorth = surfaceInputs_.isWindAndSpreadAngleRelativeToNorth();
	return isRelativeToNorth;
}

bool Behave::isWindAndSpreadAngleRelativeToUpslope() const
{
	bool isRelativeToUpslope = surfaceInputs_.isWindAndSpreadAngleRelativeToUpslope();
	return isRelativeToUpslope;
}

bool Behave::isSlopeInDegrees() const
{
	bool isSlopeModeDegrees = surfaceInputs_.isSlopeInDegrees();
	return isSlopeModeDegrees;
}

bool Behave::isSlopeInPercent() const
{
	bool isSlopeInPercent = surfaceInputs_.isSlopeInPercent();
	return isSlopeInPercent;
}

double Behave::getDirectionOfMaxSpread() const
{
	return surface_.getDirectionOfMaxSpread();
}

double Behave::getFlameLength() const
{
	return surface_.getFlameLength();
}

double Behave::getFireLengthToWidthRatio() const
{
	return surface_.getFireLengthToWidthRatio();
}

double Behave::getFireEccentricity() const
{
	return surface_.getFireEccentricity();
}

bool Behave::isFuelModelDefined(int fuelModelNumber) const
{
	return fuelModels_.isFuelModelDefined(fuelModelNumber);
}
