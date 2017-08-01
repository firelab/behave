#include "igniteInputs.h"

IgniteInputs::IgniteInputs()
{
    fuelTemperature_ = 0.0;
    moistureOneHour_ = 0.0;
    moistureHundredHour_ = 0.0;
    airTemperature_ = 0.0;
    sunShade_ = 0.0;
    fuelBedType_ = IgnitionFuelBedType::PONDEROSA_PINE_LITTER;
    duffDepth_ = 0.0;
  
    lightningChargeType_ = LightningCharge::UNKNOWN;
}

IgniteInputs::~IgniteInputs()
{

}

void IgniteInputs::updateIgniteInputs(double fuelTemperature, double moistureOneHour, double moistureHundredHour,
    double airTemperature, double sunShade, IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType, double duffDepth, 
   LightningCharge::LightningChargeEnum lightningChargeType)
{
    fuelTemperature_ = fuelTemperature;
    moistureOneHour_ = moistureOneHour;
    moistureHundredHour_ = moistureHundredHour;
    airTemperature_ = airTemperature;
    sunShade_ = sunShade;
    fuelBedType_ = fuelBedType;
    duffDepth_ = duffDepth;
    lightningChargeType_ = lightningChargeType;
}

void IgniteInputs::setFuelTemperature(double fuelTemperature)
{
    fuelTemperature_ = fuelTemperature;
}

void IgniteInputs::setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureOneHour_ = MoistureUnits::toBaseUnits(moistureOneHour, moistureUnits);
}

void IgniteInputs::setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    moistureHundredHour_ = MoistureUnits::toBaseUnits(moistureHundredHour, moistureUnits);
}

void IgniteInputs::setAirTemperature(double airTemperature)
{
    airTemperature_ = airTemperature;
}

void IgniteInputs::setSunShade(double sunShade)
{
    sunShade_ = sunShade;
}

void IgniteInputs::setIgnitionFuelBedType(IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType)
{
    fuelBedType_ = fuelBedType;
}

void IgniteInputs::setDuffDepth(double duffDepth, LengthUnits::LengthUnitsEnum lengthUnits)
{
    duffDepth_ = LengthUnits::toBaseUnits(duffDepth, lengthUnits);
}

void IgniteInputs::setLightningChargeType(LightningCharge::LightningChargeEnum lightningChargeType)
{
    lightningChargeType_ = lightningChargeType;
}

double IgniteInputs::getFuelTemperature()
{
    return fuelTemperature_;
}

double IgniteInputs::getMoistureOneHour(MoistureUnits::MoistureUnitsEnum desiredUnits)
{
    return MoistureUnits::fromBaseUnits(moistureOneHour_, desiredUnits);
}

double IgniteInputs::getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum desiredUnits)
{
    return MoistureUnits::fromBaseUnits(moistureHundredHour_, desiredUnits);
}

double IgniteInputs::getAirTemperature()
{
    return airTemperature_;
}

double IgniteInputs::getSunShade()
{
    return sunShade_;
}

IgnitionFuelBedType::IgnitionFuelBedTypeEnum IgniteInputs::getIgnitionFuelBedType()
{
    return fuelBedType_;
}

double IgniteInputs::getDuffDepth(LengthUnits::LengthUnitsEnum desiredUnits)
{
    return LengthUnits::fromBaseUnits(duffDepth_,desiredUnits);
}

LightningCharge::LightningChargeEnum IgniteInputs::getLightningChargeType()
{
    return lightningChargeType_;
}
