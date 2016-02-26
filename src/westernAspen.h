#ifndef WESTERNASPEN_HEADER
#define WESTERNASPEN_HEADER

#include <cmath>

class WesternApsen
{
public:
    WesternApsen();
    ~WesternApsen();

    // The following getter methods are used to populate FuelModel data fields 
    double getAspenFuelBedDepth(int typeIndex);
    double getAspenFuelMoistureOfExtinctionDead();
    double getAspenLoadDeadOneHour(int typeIndex, double curing);
    double getAspenLoadDeadTenHour(int typeIndex);
    double getAspenLoadLiveHerbaceous(int typeIndex, double curing);
    double getAspenLoadLiveWoody(int typeIndex, double curing);
    double getAspenSavrDeadOneHour(int typeIndex, double curing);
    double getAspenSavrDeadTenHour();
    double getAspenSavrLiveHerbaceous();
    double getAspenSavrLiveWoody(int typeIndex, double curing);

    // Mortality must be calculated AFTER spread rate
    double calculateAspenMortality(int severity, double flameLength, double dbh);

private:
    double aspenInterpolate(double curing, double *valueArray);
    // Member variables
    double mortality_;
};

#endif //WESTERNASPEN_HEADER
