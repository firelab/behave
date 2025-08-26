#include "safeSeparationDistanceCalculator.h"

SafeSeparationDistanceCalculator::~SafeSeparationDistanceCalculator()
{
  burningCondition_ = BurningCondition::Low;
  slopeClass_ = SlopeClass::Flat;
  speedClass_ = SpeedClass::Light;
  vegetationHeight_ = 0.0;
  safeSeparationDistance_ = 0.0;
  safetyZoneSite_ = 0.0;
}

SafeSeparationDistanceCalculator::SafeSeparationDistanceCalculator(const SafeSeparationDistanceCalculator& rhs)
{
    memberwiseCopyAssignment(rhs);
}

SafeSeparationDistanceCalculator& SafeSeparationDistanceCalculator::operator=(const SafeSeparationDistanceCalculator& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

// Calculate

void SafeSeparationDistanceCalculator::calculate() {
    double vegetationHeight = LengthUnits::fromBaseUnits(vegetationHeight_, LengthUnits::Feet);

    std::tuple<SpeedClass::SpeedClassEnum, BurningCondition::BurningConditionEnum, SlopeClass::SlopeClassEnum> key = { speedClass_, burningCondition_, slopeClass_ };
    double delta = SafeSeparationDistanceCalculator::deltaLookup[key];

    // Perform calculation using delta
    double safeSeparationDistance = 8.0 * vegetationHeight * delta;
    double safetyZoneSite = M_PI * pow(safeSeparationDistance, 2.0);

    safeSeparationDistance_ = LengthUnits::toBaseUnits(safeSeparationDistance, LengthUnits::Feet);
    safetyZoneSite_ = AreaUnits::toBaseUnits(safetyZoneSite, AreaUnits::Acres);
}

// Getters
BurningCondition::BurningConditionEnum SafeSeparationDistanceCalculator::getBurningCondition() {
  return burningCondition_;
}

SlopeClass::SlopeClassEnum SafeSeparationDistanceCalculator::getSlopeClass() {
  return slopeClass_;
}

SpeedClass::SpeedClassEnum SafeSeparationDistanceCalculator::getSpeedClass() {
  return speedClass_;
}

SafetyCondition::SafetyConditionEnum SafeSeparationDistanceCalculator::getSafetyCondition() {
  if (slopeClass_ == SlopeClass:Flat) {
    if (speedClass_ == SpeedClass::High) {
      return SafetyCondition::Moderate;
    } else {
      return SafetyCondition::Low;
    }
  } else if (slopeClass_ == SlopeClass:Moderate) {
    if (speedClass_ == SpeedClass::Light) {
      return SafetyCondition::Low;
    } else if (speedClass_ == SpeedClass::Moderate) {
      return SafetyCondition::Moderate;
    } else {
      return SafetyCondition::Extreme;
    }
  } else /* Steep Slope */ {
    if (speedClass_ == SpeedClass::Light) {
      if (burningCondition_ == BurningCondition::Extreme) {
        return SafetyCondition::Moderate;
      } else {
        return SafetyCondition::Low;
      }
    } else if (speedClass == SpeedClass::Moderate) {
      return SafetyCondition::Moderate;
    } else {
      return SafetyCondition::Extreme;
    }
  }
}

double SafeSeparationDistanceCalculator::getVegetationHeight(LengthUnits::LengthUnitsEnum lengthUnits) {
  return LengthUnits::fromBaseUnits(vegetationHeight_, lengthUnits);
}

double SafeSeparationDistanceCalculator::getSafeSeparationDistance(LengthUnits::LengthUnitsEnum lengthUnits) {
  return LengthUnits::fromBaseUnits(safeSeparationDistance_, lengthUnits);
}

double SafeSeparationDistanceCalculator::getSafetyZoneSite(AreaUnits::AreaUnitsEnum areaUnits) {
  return AreaUnits::fromBaseUnits(safetyZoneSite_, areaUnits);
}

// Setters
void SafeSeparationDistanceCalculator::setBurningCondition(BurningCondition::BurningConditionEnum condition) {
 burningCondition_ = condition;
}

void SafeSeparationDistanceCalculator::setSlopeClass(SlopeClass::SlopeClassEnum slope) {
 slopeClass_ = slope;
}

void SafeSeparationDistanceCalculator::setSpeedClass(SpeedClass::SpeedClassEnum speed) {
 speedClass_ = speed;
}

void SafeSeparationDistanceCalculator::setVegetationHeight(double vegetationHeight, LengthUnits::LengthUnitsEnum lengthUnits) {
  vegetationHeight_ = LengthUnits::toBaseUnits(vegetationHeight, lengthUnits);
}

void SafeSeparationDistanceCalculator::memberwiseCopyAssignment(const SafeSeparationDistanceCalculator& rhs)
{
  burningCondition_ = rhs.burningCondition_;
  slopeClass_ = rhs.slopeClass_;
  speedClass_ = rhs.speedClass_;
  vegetationHeight_ = rhs.vegetationHeight_;
  safeSeparationDistance_ = rhs.safeSeparationDistance_;
  safetyZoneSite_ = rhs.safetyZoneSite_;
}
