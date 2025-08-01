#include "safeSeparationDistanceCalculator.h"

// Calculate

void SafeSeparationDistanceCalculator::calculate() {
    double vegetationHeight = LengthUnits::fromBaseUnits(_vegetationHeight, LengthUnits::Feet);
    
    std::tuple<SpeedClass::SpeedClassEnum, BurningCondition::BurningConditionEnum, SlopeClass::SlopeClassEnum> key = { _speedClass, _burningCondition, _slopeClass };
    double delta = _deltaLookup[key];
    
    // Perform calculation using delta
    double safeSeparationDistance = 8.0 * vegetationHeight * delta;
    double safetyZoneSite = M_PI * pow(safeSeparationDistance, 2.0);

    _safeSeparationDistance = LengthUnits::toBaseUnits(safeSeparationDistance, LengthUnits::Feet);
    _safetyZoneSite = AreaUnits::toBaseUnits(safetyZoneSite, AreaUnits::Acres);
}

// Getters
BurningCondition::BurningConditionEnum SafeSeparationDistanceCalculator::getBurningCondition() { 
  return _burningCondition; 
}

SlopeClass::SlopeClassEnum SafeSeparationDistanceCalculator::getSlopeClass() { 
  return _slopeClass; 
}

SpeedClass::SpeedClassEnum SafeSeparationDistanceCalculator::getSpeedClass() { 
  return _speedClass; 
}

double SafeSeparationDistanceCalculator::getVegetationHeight(LengthUnits::LengthUnitsEnum lengthUnits) { 
  return LengthUnits::fromBaseUnits(_vegetationHeight, lengthUnits); 
}

double SafeSeparationDistanceCalculator::getSafeSeparationDistance(LengthUnits::LengthUnitsEnum lengthUnits) { 
  return LengthUnits::fromBaseUnits(_safeSeparationDistance, lengthUnits); 
}

double SafeSeparationDistanceCalculator::getSafetyZoneSite(AreaUnits::AreaUnitsEnum areaUnits) { 
  return AreaUnits::fromBaseUnits(_safetyZoneSite, areaUnits); 
}

// Setters
void SafeSeparationDistanceCalculator::setBurningCondition(BurningCondition::BurningConditionEnum condition) { 
 _burningCondition = condition; 
}

void SafeSeparationDistanceCalculator::setSlopeClass(SlopeClass::SlopeClassEnum slope) { 
 _slopeClass = slope; 
}

void SafeSeparationDistanceCalculator::setSpeedClass(SpeedClass::SpeedClassEnum speed) { 
 _speedClass = speed; 
}

void SafeSeparationDistanceCalculator::setVegetationHeight(double vegetationHeight, LengthUnits::LengthUnitsEnum lengthUnits) { 
  _vegetationHeight = LengthUnits::toBaseUnits(vegetationHeight, lengthUnits); 
}
