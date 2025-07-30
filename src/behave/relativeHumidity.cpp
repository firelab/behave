#include <math.h>
#include "relativeHumidity.h"

// Constructors
RelativeHumidityTool::RelativeHumidityTool() {
  dryBulbTemperature_ = 0.;
  wetBulbTemperature_ = 0.;
  siteElevation_ = 0.;
  wetBulbDepression_ = 0.;
  dewPointTemperature_ = 0.;
  relativeHumidity_ = 0.;
}

RelativeHumidityTool::RelativeHumidityTool(const RelativeHumidityTool &rhs) {
  memberwiseCopyAssignment(rhs);
}

RelativeHumidityTool& RelativeHumidityTool::operator=(const RelativeHumidityTool& rhs) {
    if (this != &rhs) {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void RelativeHumidityTool::memberwiseCopyAssignment(const RelativeHumidityTool& rhs)
{
  dryBulbTemperature_ = rhs.dryBulbTemperature_;
  wetBulbTemperature_ = rhs.wetBulbTemperature_;
  siteElevation_ = rhs.siteElevation_;

  wetBulbDepression_ = rhs.wetBulbDepression_;
  dewPointTemperature_ = rhs.dewPointTemperature_;
  relativeHumidity_ = rhs.relativeHumidity_;
}

RelativeHumidityTool::~RelativeHumidityTool() {}

// Calculate
void RelativeHumidityTool::calculate() {
  double dryBulbTemp = TemperatureUnits::fromBaseUnits(dryBulbTemperature_, TemperatureUnits::Celsius);
  double wetBulbTemp = TemperatureUnits::fromBaseUnits(wetBulbTemperature_, TemperatureUnits::Celsius);
  double siteElevation = LengthUnits::fromBaseUnits(siteElevation_, LengthUnits::Meters);

  // Calculate Wet Bulb Depression
  double wetBulbDep = dryBulbTemp - wetBulbTemp;

  // Calculate Dew Point
  double dewPointTemp = dryBulbTemp;

  if ( wetBulbTemp < dryBulbTemp ) {

    double e2 = 6.1121 * exp( 17.502 * wetBulbTemp / (240.97 + wetBulbTemp) );

    if ( wetBulbTemp < 0. ) {
      e2 = 6.1115 * exp( 22.452 * wetBulbTemp / ( 272.55 + wetBulbTemp) );
    }

    double p = 1013. * exp( -0.0000375 * siteElevation);
    double d = 0.66 * ( 1. + 0.00115 * wetBulbTemp) * (dryBulbTemp - wetBulbTemp);
    double e3 = e2 - d * p / 1000.;

    if ( e3 < 0.001 ) {
      e3 = 0.001;
    }

    double t3 = -240.97 /  ( 1.- 17.502 / log(e3 / 6.1121) );
    dewPointTemp = t3;

    if ( dewPointTemp < -40. ) {
      dewPointTemp = -40.;
    }
  }

  // Calculate Relative Humidity
  double relativeHumidity = 0.;

  if ( dewPointTemp >= dryBulbTemp ) {
    relativeHumidity = 1.0;
  } else {
    relativeHumidity = exp( -7469. / ( dewPointTemp + 398.0 ) + 7469. / ( dryBulbTemp + 398.0 ) );
  }

  // Store results
  wetBulbDepression_ = TemperatureUnits::toBaseUnits(wetBulbDep, TemperatureUnits::Celsius);
  dewPointTemperature_ = TemperatureUnits::toBaseUnits(dewPointTemp, TemperatureUnits::Celsius);
  relativeHumidity_ = FractionUnits::toBaseUnits(relativeHumidity, FractionUnits::Fraction);
}

// Getters
double RelativeHumidityTool::getDryBulbTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits) {
  return TemperatureUnits::fromBaseUnits(dryBulbTemperature_, temperatureUnits);
}

double RelativeHumidityTool::getWetBulbTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits) {
  return TemperatureUnits::fromBaseUnits(wetBulbTemperature_, temperatureUnits);
}

double RelativeHumidityTool::getSiteElevation(LengthUnits::LengthUnitsEnum lengthUnits) {
  return LengthUnits::fromBaseUnits(siteElevation_, lengthUnits);
}

double RelativeHumidityTool::getWetBulbDepression(TemperatureUnits::TemperatureUnitsEnum temperatureUnits) {
  return TemperatureUnits::fromBaseUnits(wetBulbDepression_, temperatureUnits);
}

double RelativeHumidityTool::getDewPointTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits) {
  return TemperatureUnits::fromBaseUnits(dewPointTemperature_, temperatureUnits);
}

double RelativeHumidityTool::getRelativeHumidity(FractionUnits::FractionUnitsEnum fractionUnits) {
  return FractionUnits::fromBaseUnits(relativeHumidity_, fractionUnits);
}

// Setters
void RelativeHumidityTool::setDryBulbTemperature(double dryBulbTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits) {
    dryBulbTemperature_ = TemperatureUnits::toBaseUnits(dryBulbTemperature, temperatureUnits);
}

void RelativeHumidityTool::setWetBulbTemperature(double wetBulbTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits) {
    wetBulbTemperature_ = TemperatureUnits::toBaseUnits(wetBulbTemperature, temperatureUnits);
}

void RelativeHumidityTool::setSiteElevation(double siteElevation, LengthUnits::LengthUnitsEnum lengthUnits) {
    siteElevation_ = LengthUnits::toBaseUnits(siteElevation, lengthUnits);
}

