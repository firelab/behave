#ifndef WINDSPEED_HEADER
#define WINDSPEED_HEADER

class WindSpeed
{
public:
    WindSpeed();
    double WindSpeedAtMidflame(double windSpeedAtTwentyFeet, double windAdjustmentFactor);
    double WindSpeedAtTwentyFeetFromTenMeter(double windSpeedAtTenMeters);

private:
    double windSpeedAtMidflame_;
    double windSpeedAtTenMeters_;
    double windSpeedAtTwentyFeet_;
};

#endif // WINDSPEED_HEADER
