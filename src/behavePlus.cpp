#include "behavePlus.h"

#include <iostream>

BehavePlus::BehavePlus()
	: surfaceInputs_{}, surface_{ fuelModels_, surfaceInputs_ }
{
	// Default Constructor
}

BehavePlus::BehavePlus(SurfaceInputs &surfaceInputs)
	: surface_{ fuelModels_, surfaceInputs }
{
	// Constructor taking a SurfaceInputs object as a parameter
}

BehavePlus::BehavePlus(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
	double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
	double midflameWindSpeed, double windDirection, double slope, double slopeAspect)
	: surfaceInputs_(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour,
	moistureLiveHerbaceous, moistureLiveWoody, midflameWindSpeed, windDirection, slope, slopeAspect),
	surface_{ fuelModels_, surfaceInputs_ }
{
	// Constructor takes a list of doubles as parameters
}

BehavePlus::~BehavePlus()
{
	// Default Destructor
}

void BehavePlus::updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
	double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
	double midflameWindSpeed, double windDirection, double slope, double slopeAspect)
{
	surfaceInputs_.updateInput(fuelModelNumber, moistureOneHour, moistureTenHour, moistureHundredHour, moistureLiveHerbaceous,
		moistureLiveWoody, midflameWindSpeed, windDirection, slope, slopeAspect);
}

double BehavePlus::calculateSurfaceFireForwardSpreadRate(double directionOfInterest)
{
	// Calculate Spread Rate
	return surface_.calculateSurfaceFireForwardSpreadRate(directionOfInterest);
}

BehavePlus::WindAndSpreadDirectionEnumType BehavePlus::getWindAndSpreadAngleMode() const
{
	int windAndSpreadMode = surfaceInputs_.getWindAndSpreadAngleMode();
	return (WindAndSpreadDirectionEnumType)windAndSpreadMode;
}

BehavePlus::SlopeInputModeEnumType BehavePlus::getSlopeInputMode() const
{
	int slopeMode = surfaceInputs_.getSlopeInputMode();
	return (SlopeInputModeEnumType)slopeMode;
}

void BehavePlus::setWindAndSpreadDirectionMode(WindAndSpreadDirectionEnumType mode)
{
	surfaceInputs_.setWindAndSpreadDirectionMode(mode);
}

void BehavePlus::setSlopeInputMode(SlopeInputModeEnumType mode)
{
	surfaceInputs_.setSlopeInputMode(mode);
}

void BehavePlus::setWindAndSpreadAnglesRelativeToNorth()
{
	surfaceInputs_.setWindAndSpreadDirectionMode(BehavePlus::RELATIVE_TO_NORTH);
}

void BehavePlus::setWindAndSpreadAnglesRelativeToUpslope()
{
	surfaceInputs_.setWindAndSpreadDirectionMode(BehavePlus::RELATIVE_TO_UPSLOPE);
}

void BehavePlus::setSlopeInputToPercent()
{
	surfaceInputs_.setSlopeInputMode(SLOPE_IN_PERCENT);
}

void BehavePlus::setSlopeInputToDegrees()
{
	surfaceInputs_.setSlopeInputMode(SLOPE_IN_DEGREES);
}

bool BehavePlus::isWindAndSpreadAngleRelativeToNorth() const
{
	bool isRelativeToNorth = (getWindAndSpreadAngleMode() == RELATIVE_TO_NORTH);
	return isRelativeToNorth;
}

bool BehavePlus::isWindAndSpreadAngleRelativeToUpslope() const
{
	bool isRelativeToUpslope = (getWindAndSpreadAngleMode() == RELATIVE_TO_UPSLOPE);
	return isRelativeToUpslope;
}

bool BehavePlus::isSlopeInDegrees() const
{
	bool isSlopeModeDegrees = (getSlopeInputMode() == SLOPE_IN_DEGREES);
	return isSlopeModeDegrees;
}

bool BehavePlus::isSlopeInPercent() const
{
	bool isSlopeInPercent = (getSlopeInputMode() == SLOPE_IN_PERCENT);
	return isSlopeInPercent;
}

double BehavePlus::getDirectionOfMaxSpread() const
{
	return surface_.getDirectionOfMaxSpread();
}

double BehavePlus::getFlameLength() const
{
	return surface_.getFlameLength();
}

double BehavePlus::getFireLengthToWidthRatio() const
{
	return surface_.getFireLengthToWidthRatio();
}

double BehavePlus::getFireEccentricity() const
{
	return surface_.getFireEccentricity();
}

bool BehavePlus::isFuelModelDefined(int fuelModelNumber) const
{
	return fuelModels_.isFuelModelDefined(fuelModelNumber);
}
