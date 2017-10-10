#include "safety.h"

#define _USE_MATH_DEFINES
#include <cmath>

void Safety::setFlameHeight(double flameHeight, LengthUnits::LengthUnitsEnum lengthUnits)
{

    flameHeight_ = LengthUnits::toBaseUnits(flameHeight, lengthUnits);
}

void Safety::setNumberOfPersonnel(double numberOfPersonnel)
{
    numberOfPersonnel_ = numberOfPersonnel;
}

void Safety::setAreaPerPerson(double areaPerPerson, AreaUnits::AreaUnitsEnum areaUnits)
{
    areaPerPerson_ = AreaUnits::toBaseUnits(areaPerPerson, areaUnits);
}

void Safety::setNumberOfEquipment(double numberOfEquipment)
{
    numberOfEquipment_ = numberOfEquipment;
}

void Safety::setAreaPerEquipment(double areaPerEquipment, AreaUnits::AreaUnitsEnum areaUnits)
{
    areaPerEquipment_ = AreaUnits::toBaseUnits(areaPerEquipment, areaUnits);
}

void Safety::updateSafetyInputs(double flameHeight, LengthUnits::LengthUnitsEnum lengthUnits, 
    int numberOfPersonnel, int numberOfEquipment, double areaPerPerson,
    double areaPerEquipment, AreaUnits::AreaUnitsEnum areaUnits)
{
    setFlameHeight(flameHeight, lengthUnits);
    setNumberOfPersonnel(numberOfPersonnel);
    setNumberOfEquipment(numberOfEquipment);
    setAreaPerPerson(areaPerPerson, areaUnits);
    setAreaPerEquipment(areaPerEquipment, areaUnits);
}

void Safety::calculateSafetyZone()
{
    calculateSafetyZoneSeparationDistance();
    // Space needed by firefighters and equipment in core of safety zone
    double coreRadius = (areaPerPerson_ * numberOfPersonnel_ +
        numberOfEquipment_ * areaPerEquipment_) / M_PI;
    if (coreRadius > 1.0e-07)
    {
        coreRadius = sqrt(coreRadius);
    }
    // Add 4 times the flame ht to the protected safety zone core
    safetyZoneRadius_ = separationDistance_ + coreRadius;
    safetyZoneArea_ = M_PI * safetyZoneRadius_ * safetyZoneRadius_;
}

double Safety::getSeparationDistance(LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return LengthUnits::fromBaseUnits(separationDistance_, lengthUnits);
}

double Safety::getSafetyZoneRadius(LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return LengthUnits::fromBaseUnits(safetyZoneRadius_, lengthUnits);
}

double Safety::getSafetyZoneArea(AreaUnits::AreaUnitsEnum areaUnits) const
{
    return AreaUnits::fromBaseUnits(safetyZoneArea_, areaUnits);
}

void Safety::initializeMembers()
{
    flameHeight_ = 0; 
    numberOfPersonnel_ = 0;
    areaPerPerson_ = 0;
    numberOfEquipment_ = 0;
    areaPerEquipment_ = 0;
    separationDistance_ = 0;
    safetyZoneRadius_ = 0;
    safetyZoneArea_ = 0;
}

void Safety::calculateSafetyZoneSeparationDistance()
{
    separationDistance_ = 4.0 * flameHeight_;
}
