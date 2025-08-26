#pragma once
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
  double getVegetationHeight(LengthUnits::LengthUnitsEnum lengthUnits);
  double getSafeSeparationDistance(LengthUnits::LengthUnitsEnum lengthUnits);
  double getSafetyZoneSite(AreaUnits::AreaUnitsEnum areaUnits);

  // Setters
  void setBurningCondition(BurningCondition::BurningConditionEnum condition);
  void setSlopeClass(SlopeClass::SlopeClassEnum slope);
  void setSpeedClass(SpeedClass::SpeedClassEnum speed);
  void setVegetationHeight(double height, LengthUnits::LengthUnitsEnum lengthUnits);

protected:
  void memberwiseCopyAssignment(const SafeSeparationDistanceCalculator& rhs);

  BurningCondition::BurningConditionEnum burningCondition_;
  SlopeClass::SlopeClassEnum slopeClass_;
  SpeedClass::SpeedClassEnum speedClass_;
  double vegetationHeight_;
  double safeSeparationDistance_;
  double safetyZoneSite_;

  static std::unordered_map<std::tuple<SpeedClass::SpeedClassEnum, BurningCondition::BurningConditionEnum, SlopeClass::SlopeClassEnum>, double> deltaLookup = {
      // Light Burning Conditions
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
};
