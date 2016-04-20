#ifndef WINDSPEED_HEADER
#define WINDSPEED_HEADER

class WindSpeed
{
	public:
		WindSpeed();
		double WindSpeedAtMidflame(double windSpeedAt20Ft, double windAdjFactor);
        double WindSpeedAtTwentyFtFromTenMeter(double windSpeedAt10M);
	
	private:
		double windSpeedAt20Ft;
		double windSpeedAtMidflame;
		double windSpeedAt10M;
};

#endif // WINDSPEED_HEADER