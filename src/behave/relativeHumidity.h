#pragma once
#include "behaveUnits.h"

class RelativeHumidityTool {
public:

  // Constructors
  RelativeHumidityTool();
  RelativeHumidityTool(const RelativeHumidityTool& rhs);
  RelativeHumidityTool& operator=(const RelativeHumidityTool& rhs);

  // Destructors
  ~RelativeHumidityTool();

  // Calculate
  void calculate();

  // Getters
  double getDryBulbTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits);
  double getSiteElevation(LengthUnits::LengthUnitsEnum lengthUnits);
  double getWetBulbTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits);

  double getDewPointTemperature(TemperatureUnits::TemperatureUnitsEnum temperatureUnits);
  double getRelativeHumidity(FractionUnits::FractionUnitsEnum fractionUnits);
  double getWetBulbDepression(TemperatureUnits::TemperatureUnitsEnum temperatureUnits);

  // Setters
  void setDryBulbTemperature(double dryBulbTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits);
  void setSiteElevation(double siteElevation, LengthUnits::LengthUnitsEnum lengthUnits);
  void setWetBulbTemperature(double wetBulbTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits);


protected:
  void memberwiseCopyAssignment(const RelativeHumidityTool& rhs);

  // Input Variables
  double dryBulbTemperature_ = 0;
  double siteElevation_ = 0;
  double wetBulbTemperature_ = 0;

  // Output Variables
  double dewPointTemperature_ = 0;
  double relativeHumidity_ = 0;
  double wetBulbDepression_ = 0;
};


