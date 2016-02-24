#ifndef SURFACEENUMS_H
#define SURFACEENUMS_H

struct WindAndSpreadAngleMode
{
	enum WindAndSpreadAngleModeEnum
	{
		RELATIVE_TO_UPSLOPE = 0,	// Wind and spread angles I/O are clockwise relative to upslope
		RELATIVE_TO_NORTH = 1		// Wind direction angles I/O are clockwise relative to compass north
	};
};

struct SlopeInputMode
{
	enum SlopeInputModeEnum
	{
		SLOPE_IN_PERCENT = 0,	// Slope is input as a percent
		SLOPE_IN_DEGREES = 1	// Slope is input as degrees
	};
};

struct TwoFuelModelsMethod
{
	enum TwoFuelModelsMethodEnum
	{
		NO_METHOD = 0,			// Don't use TwoFuel Model Method
		ARITHMETIC = 1,			// Use arithmetic mean
		HARMONIC = 2,			// Use harmoic mean
		TWO_DIMENSIONAL = 3	// Use Finney's two dimensional method
	};
};

struct WindHeightInputMode
{
	enum WindHeightInputModeEnum
	{
		DIRECT_MIDFLAME = 0,	// User enters midflame wind speed directy
		TWENTY_FOOT = 1,		// User enters the 20 foot wind speed
		TEN_METER = 2,			// User enters the 10 meter wind speed
	};
};

#endif //SURFACEENUMS_H