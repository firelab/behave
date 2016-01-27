// Interface for BehavePlus application based on the Facade OOP Design Pattern
// It is used to tie together the modules and objects used by BehavePlus - WMC 10/2015

#ifndef BEHAVEPLUS_HEADER
#define BEHAVEPLUS_HEADER

#include "fuelModels.h"
#include "surface.h"

class BehavePlus
{
	enum WindAndSpreadDirectionEnumType {
		RELATIVE_TO_UPSLOPE = 0,	// Wind and spread direction are clockwise relative to upslope
		RELATIVE_TO_NORTH = 1		// Wind direction direction are clockwise relative to compass north
	};

	enum SlopeInputModeEnumType {
		SLOPE_IN_PERCENT = 0,	// Slope is input as a percent
		SLOPE_IN_DEGREES = 1	// Slope is input as degrees
	};

public:
	BehavePlus();
	explicit BehavePlus(SurfaceInputs &surfaceInputs);
	BehavePlus(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
		double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
		double midflameWindSpeed, double windDirection, double slope, double slopeAspect = 0);
	~BehavePlus();
	
	// SURFACE Module 
	void updateSurfaceInputs(int fuelModelNumber, double moistureOneHour, double moistureTenHour,
		double moistureHundredHour, double moistureLiveHerbaceous, double moistureLiveWoody,
		double midflameWindSpeed, double windDirection, double slope, double slopeAspect = 0);
	double calculateSurfaceFireForwardSpreadRate(double directionOfInterest = -1.0);
	double getDirectionOfMaxSpread() const;
	double getFlameLength() const;
	double getFireLengthToWidthRatio() const;
	double getFireEccentricity() const;
	bool isFuelModelDefined(int fuelModelNumber) const;
	bool isWindAndSpreadAngleRelativeToNorth() const;
	bool isWindAndSpreadAngleRelativeToUpslope() const;
	bool isSlopeInDegrees() const;
	bool isSlopeInPercent() const;
	
	void setWindAndSpreadAnglesRelativeToNorth();
	void setWindAndSpreadAnglesRelativeToUpslope();
	void setSlopeInputToPercent();
	void setSlopeInputToDegrees();

private:
	void setSlopeInputMode(SlopeInputModeEnumType mode);
	void setWindAndSpreadDirectionMode(WindAndSpreadDirectionEnumType mode);

	FuelModels fuelModels_;										// Object containing data for fuel models
	Surface surface_;											// SURFACE Moduel object
	SurfaceInputs surfaceInputs_;								// Object that manages user input to SURFACE Module
};

#endif //BEHAVEPLUS_HEADER
