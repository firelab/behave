#ifndef WINDSPEED_HEADER
#define WINDSPEED_HEADER

class WindSpeedUtility
{
public:
    WindSpeedUtility();
    double WindSpeedAtMidflame(double windSpeedAtTwentyFeet, double windAdjustmentFactor);
    double WindSpeedAtTwentyFeetFromTenMeter(double windSpeedAtTenMeters);

private:
    double windSpeedAtMidflame_;
    double windSpeedAtTenMeters_;
    double windSpeedAtTwentyFeet_;
};

#endif // WINDSPEED_HEADER
