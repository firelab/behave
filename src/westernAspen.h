#ifndef WESTERNASPEN_HEADER
#define WESTERNASPEN_HEADER

#include <cmath>

class WesternAspen
{
public:
    WesternAspen();
    ~WesternAspen();

    void initialize();

    // The following getter methods are used to populate FuelModel data fields 
    double getAspenFuelBedDepth(int typeIndex);
    double getAspenHeatOfCombustionDead();
    double getAspenHeatOfCombustionLive();
    double getAspenMoistureOfExtinctionDead();
    double getAspenLoadDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenLoadDeadTenHour(int aspenFuelModelNumber);
    double getAspenLoadLiveHerbaceous(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenLoadLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenSavrDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenSavrDeadTenHour();
    double getAspenSavrLiveHerbaceous();
    double getAspenSavrLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel);

    // Mortality must be calculated AFTER spread rate
    double calculateAspenMortality(int severity, double flameLength, double dbh);

private:
    double aspenInterpolate(double curing, double *valueArray);
    // Member variables
    double mortality_;
};

#endif //WESTERNASPEN_HEADER
