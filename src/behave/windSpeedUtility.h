#ifndef WINDSPEED_HEADER
#define WINDSPEED_HEADER

class WindSpeedUtility
{
public:
    WindSpeedUtility();
    double windSpeedAtMidflame(double windSpeedAtTwentyFeet, double windAdjustmentFactor);
    double windSpeedAtMidflame(double windSpeedAtTenMeters);

private:
    double windSpeedAtMidflame_;
    double windSpeedAtTenMeters_;
    double windSpeedAtTwentyFeet_;
};

#endif // WINDSPEED_HEADER
