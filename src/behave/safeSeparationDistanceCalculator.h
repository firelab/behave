#pragma once
#include <math.h>
#include <unordered_map>
#include <tuple>
#include "behaveUnits.h"

struct BurningCondition {
  enum BurningConditionEnum {
    Low,
    Moderate,
    Extreme
  };
};

struct SlopeClass {
  enum SlopeClassEnum {
    Flat,
    Moderate,
    Steep
  };
};

struct SpeedClass {
  enum SpeedClassEnum {
    Light,
    Moderate,
    High
  };
};

struct SafetyCondition {
  enum SafetyConditionEnum {
    Low,
    Moderate,
    Extreme
  };
};

struct TupleHash {
    template <typename T1, typename T2, typename T3>
    std::size_t operator()(const std::tuple<T1, T2, T3>& t) const {
        auto hash1 = std::hash<T1>{}(std::get<0>(t));
        auto hash2 = std::hash<T2>{}(std::get<1>(t));
        auto hash3 = std::hash<T3>{}(std::get<2>(t));
        return hash1 ^ (hash2 << 1) ^ (hash3 << 2); // Combine the hash values
    }
};

class SafeSeparationDistanceCalculator {
public:
  SafeSeparationDistanceCalculator();
  ~SafeSeparationDistanceCalculator();
  SafeSeparationDistanceCalculator(const SafeSeparationDistanceCalculator& rhs);
  SafeSeparationDistanceCalculator& operator=(const SafeSeparationDistanceCalculator& rhs);

  // calculate
  void calculate();

  // Getters
  BurningCondition::BurningConditionEnum getBurningCondition();
  SlopeClass::SlopeClassEnum getSlopeClass();
  SpeedClass::SpeedClassEnum getSpeedClass();
  SafetyCondition::SafetyConditionEnum getSafetyCondition();
  double getVegetationHeight(LengthUnits::LengthUnitsEnum lengthUnits);
  double getSafeSeparationDistance(LengthUnits::LengthUnitsEnum lengthUnits);
  double getSafetyZoneSize(AreaUnits::AreaUnitsEnum areaUnits);

  // Setters
  void setBurningCondition(BurningCondition::BurningConditionEnum condition);
  void setSlopeClass(SlopeClass::SlopeClassEnum slope);
  void setSpeedClass(SpeedClass::SpeedClassEnum speed);
  void setVegetationHeight(double height, LengthUnits::LengthUnitsEnum lengthUnits);

protected:
  void memberwiseCopyAssignment(const SafeSeparationDistanceCalculator& rhs);
  static double getValue(SpeedClass::SpeedClassEnum speed, BurningCondition::BurningConditionEnum burning, SlopeClass::SlopeClassEnum slope) {
    auto key = std::make_tuple(speed, burning, slope);
    auto it = deltaLookup.find(key);
    if (it != deltaLookup.end()) {
      return it->second;
    }
    return 0.0;
  }

  BurningCondition::BurningConditionEnum burningCondition_;
  SlopeClass::SlopeClassEnum slopeClass_;
  SpeedClass::SpeedClassEnum speedClass_;
  double vegetationHeight_;
  double safeSeparationDistance_;
  double safetyZoneSize_;

  static const std::unordered_map<std::tuple<SpeedClass::SpeedClassEnum, BurningCondition::BurningConditionEnum, SlopeClass::SlopeClassEnum>, double, TupleHash> deltaLookup;
};
