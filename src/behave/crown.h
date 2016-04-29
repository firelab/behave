#ifndef CROWN_HEADER
#define CROWN_HEADER

#include "surfaceInputs.h"
#include "surfaceFireSpread.h"

class FuelModels;

class Crown
{
public:
    Crown(const FuelModels& fuelModels, const SurfaceInputs& surfaceInputs);
	~Crown();

    double calculateCrownFireSpreadRate(double windSpeedAtTwentyFeet);

private:
    const FuelModels* fuelModels_;
    const SurfaceInputs* surfaceInputs_;
    SurfaceInputs crownInputs_;
    SurfaceFireSpread crownFireSpread_; // stores Crown's surface fire data to allow parallel runs in Surface
};

#endif // CROWN_HEADER
