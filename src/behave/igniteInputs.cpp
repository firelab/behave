#include "igniteInputs.h"

IgniteInputs::IgniteInputs()
{
    fuelTemperature_ = 0.0;
    oneHourMoisture_ = 0.0;
    airTemperature_ = 0.0;
    sunShade_ = 0.0;
    fuelBedType_ = IgnitionFuelBedType::PONDEROSA_PINE_LITTER;
    duffDepth_ = 0.0;
    hundredHourMoisture_ = 0.0;
    lightningChargeType_ = LightningCharge::UNKNOWN;
}

IgniteInputs::~IgniteInputs()
{

}

void IgniteInputs::updateIgniteInputs(double fuelTemperature, double oneHourMoisture, double airTemperature, 
    double sunShade, IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType, double duffDepth, 
    double hundredHourMoisture, LightningCharge::LightningChargeEnum lightningChargeType)
{
    fuelTemperature_ = fuelTemperature;
    oneHourMoisture_ = oneHourMoisture;
    airTemperature_ = airTemperature;
    sunShade_ = sunShade;
    fuelBedType_ = fuelBedType;
    duffDepth_ = duffDepth;
    hundredHourMoisture_ = hundredHourMoisture;
    lightningChargeType_ = lightningChargeType;
}

void IgniteInputs::setFuelTemperature(double fuelTemperature)
{
    fuelTemperature_ = fuelTemperature;
}

void IgniteInputs::setOneHourMoisture(double oneHourMoisture)
{
    oneHourMoisture_ = oneHourMoisture;
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

void IgniteInputs::setDuffDepth(double duffDepth)
{
    duffDepth_ = duffDepth;
}

void IgniteInputs::setHundredHourMoisture(double hundredHourMoisture)
{
    hundredHourMoisture_ = hundredHourMoisture;
}

void IgniteInputs::setLightningChargeType(LightningCharge::LightningChargeEnum lightningChargeType)
{
    lightningChargeType_ = lightningChargeType;
}

double IgniteInputs::getFuelTemperature()
{
    return fuelTemperature_;
}

double IgniteInputs::getOneHourMoisture()
{
    return oneHourMoisture_;
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

double IgniteInputs::getDuffDepth()
{
    return duffDepth_;
}

double IgniteInputs::getHundredHourMoisture()
{
    return hundredHourMoisture_;
}

LightningCharge::LightningChargeEnum IgniteInputs::getLightningChargeType()
{
    return lightningChargeType_;
}
