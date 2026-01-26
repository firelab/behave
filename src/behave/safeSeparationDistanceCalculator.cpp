#include "safeSeparationDistanceCalculator.h"
#define _USE_MATH_DEFINES
#include <math.h>

const std::unordered_map<std::tuple<SpeedClass::SpeedClassEnum, BurningCondition::BurningConditionEnum, SlopeClass::SlopeClassEnum>, double, TupleHash> SafeSeparationDistanceCalculator::deltaLookup = {
  {{SpeedClass::Light, BurningCondition::Low, SlopeClass::Flat}, 1.0},
  {{SpeedClass::Light, BurningCondition::Low, SlopeClass::Moderate}, 1.0},
  {{SpeedClass::Light, BurningCondition::Low, SlopeClass::Steep}, 2.0},

  // Moderate Burning Conditions
  {{SpeedClass::Light, BurningCondition::Moderate, SlopeClass::Flat}, 1.0},
  {{SpeedClass::Light, BurningCondition::Moderate, SlopeClass::Moderate}, 1.0},
  {{SpeedClass::Light, BurningCondition::Moderate, SlopeClass::Steep}, 2.0},

  // Extreme Burning Conditions
  {{SpeedClass::Light, BurningCondition::Extreme, SlopeClass::Flat}, 1.0},
  {{SpeedClass::Light, BurningCondition::Extreme, SlopeClass::Moderate}, 2.0},
  {{SpeedClass::Light, BurningCondition::Extreme, SlopeClass::Steep}, 3.0},

  /// Moderate Wind Speed
  // Light Burning Conditions
  {{SpeedClass::Moderate, BurningCondition::Low, SlopeClass::Flat}, 1.5},
  {{SpeedClass::Moderate, BurningCondition::Low, SlopeClass::Moderate}, 3.0},
  {{SpeedClass::Moderate, BurningCondition::Low, SlopeClass::Steep}, 4.0},

  // Moderate Burning Conditions
  {{SpeedClass::Moderate, BurningCondition::Moderate, SlopeClass::Flat}, 2.0},
  {{SpeedClass::Moderate, BurningCondition::Moderate, SlopeClass::Moderate}, 4.0},
  {{SpeedClass::Moderate, BurningCondition::Moderate, SlopeClass::Steep}, 5.0},

  // Extreme Burning Conditions
  {{SpeedClass::Moderate, BurningCondition::Extreme, SlopeClass::Flat}, 2.5},
  {{SpeedClass::Moderate, BurningCondition::Extreme, SlopeClass::Moderate}, 5.0},
  {{SpeedClass::Moderate, BurningCondition::Extreme, SlopeClass::Steep}, 5.0},

  /// High Wind Speed
  {{SpeedClass::High, BurningCondition::Low, SlopeClass::Flat}, 3.0},
  {{SpeedClass::High, BurningCondition::Low, SlopeClass::Moderate}, 4.0},
  {{SpeedClass::High, BurningCondition::Low, SlopeClass::Steep}, 6.0},

  // Moderate Burning Conditions
  {{SpeedClass::High, BurningCondition::Moderate, SlopeClass::Flat}, 3.0},
  {{SpeedClass::High, BurningCondition::Moderate, SlopeClass::Moderate}, 5.0},
  {{SpeedClass::High, BurningCondition::Moderate, SlopeClass::Steep}, 7.0},

  // Extreme Burning Conditions
  {{SpeedClass::High, BurningCondition::Extreme, SlopeClass::Flat}, 4.0},
  {{SpeedClass::High, BurningCondition::Extreme, SlopeClass::Moderate}, 5.0},
  {{SpeedClass::High, BurningCondition::Extreme, SlopeClass::Steep}, 10.0},
};

SafeSeparationDistanceCalculator::SafeSeparationDistanceCalculator()
{
  burningCondition_ = BurningCondition::Low;
  slopeClass_ = SlopeClass::Flat;
  speedClass_ = SpeedClass::Light;
  vegetationHeight_ = 0.0;
  safeSeparationDistance_ = 0.0;
  safetyZoneSize_ = 0.0;
}

SafeSeparationDistanceCalculator::~SafeSeparationDistanceCalculator() {
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

    double delta = SafeSeparationDistanceCalculator::getValue(speedClass_, burningCondition_, slopeClass_);

    // Perform calculation using delta
    double safeSeparationDistance = 8.0 * vegetationHeight * delta;
    double safetyZoneSize = M_PI * pow(safeSeparationDistance, 2.0);

    safeSeparationDistance_ = LengthUnits::toBaseUnits(safeSeparationDistance, LengthUnits::Feet);
    safetyZoneSize_ = AreaUnits::toBaseUnits(safetyZoneSize, AreaUnits::SquareFeet);
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
  if (slopeClass_ == SlopeClass::Flat) {
    if (speedClass_ == SpeedClass::High) {
      return SafetyCondition::Moderate;
    } else {
      return SafetyCondition::Low;
    }
  } else if (slopeClass_ == SlopeClass::Moderate) {
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
    } else if (speedClass_ == SpeedClass::Moderate) {
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

double SafeSeparationDistanceCalculator::getSafetyZoneSize(AreaUnits::AreaUnitsEnum areaUnits) {
  return AreaUnits::fromBaseUnits(safetyZoneSize_, areaUnits);
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
  safetyZoneSize_ = rhs.safetyZoneSize_;
}
