#include "safety.h"

#define _USE_MATH_DEFINES
#include <cmath>

void Safety::setFlameHeight(double flameHeight)
{
    flameHeight_ = flameHeight;
}

void Safety::setNumberOfPersonnel(double numberOfPersonnel)
{
    numberOfPersonnel_ = numberOfPersonnel;
}

void Safety::setAreaPerPerson(double areaPerPerson)
{
    areaPerPerson_ = areaPerPerson;
}

void Safety::setNumberOfEquipment(double numberOfEquipment)
{
    numberOfEquipment_ = numberOfEquipment;
}

void Safety::setAreaPerEquipment(double areaPerEquipment)
{
    areaPerEquipment_ = areaPerEquipment;
}

void Safety::updateSafetyInputs(double flameHeight, double numberOfPersonnel, double areaPerPerson, 
    double numberOfEquipment, double areaPerEquipment)
{
    setFlameHeight(flameHeight);
    setNumberOfPersonnel(numberOfPersonnel);
    setAreaPerPerson(areaPerPerson);
    setNumberOfEquipment(numberOfEquipment);
    setAreaPerEquipment(areaPerEquipment);
}

void Safety::calculateSafetyZoneRadius()
{
    // Space needed by firefighters and equipment in core of safety zone
    double coreRadius = (areaPerPerson_ * numberOfPersonnel_ +
        numberOfEquipment_ * areaPerEquipment_) / M_PI;
    if (coreRadius > 1.0e-07)
    {
        coreRadius = sqrt(coreRadius);
    }
    // Add 4 times the flame ht to the protected safety zone core
    safetyZoneRadius_ = separationDistance_ + coreRadius;
}

void Safety::calculateSafetyZoneSeparationDistance()
{
    separationDistance_ = 4.0 * flameHeight_;
}
