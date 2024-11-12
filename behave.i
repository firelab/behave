#define SWIG_PYTHON_INTERPRETER_NO_DEBUG

%module behave %{
#define SWIG_FILE_WITH_INIT
#include "behaveUnits.h"
%}

enum AreaUnitsEnum {
  SquareFeet,
  Acres,
  Hectares,
  SquareMeters,
  SquareMiles,
  SquareKilometers
};

// enum BasalAreaUnits_BasalAreaUnitsEnum {
//   "BasalAreaUnits::SquareFeetPerAcre",
//   "BasalAreaUnits::SquareMetersPerHectare"
// };
// 
// enum FractionUnits_FractionUnitsEnum {
//   "FractionUnits::Fraction",
//   "FractionUnits::Percent"
// };
// 
// enum LengthUnits_LengthUnitsEnum {
//   "LengthUnits::Feet",
//   "LengthUnits::Inches",
//   "LengthUnits::Millimeters",
//   "LengthUnits::Centimeters",
//   "LengthUnits::Meters",
//   "LengthUnits::Chains",
//   "LengthUnits::Miles",
//   "LengthUnits::Kilometers"
// };
// 
// enum LoadingUnits_LoadingUnitsEnum {
//   "LoadingUnits::PoundsPerSquareFoot",
//   "LoadingUnits::TonsPerAcre",
//   "LoadingUnits::TonnesPerHectare",
//   "LoadingUnits::KilogramsPerSquareMeter"
// };
// 
// enum SurfaceAreaToVolumeUnits_SurfaceAreaToVolumeUnitsEnum {
//   "SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet",
//   "SurfaceAreaToVolumeUnits::SquareMetersOverCubicMeters",
//   "SurfaceAreaToVolumeUnits::SquareInchesOverCubicInches",
//   "SurfaceAreaToVolumeUnits::SquareCentimetersOverCubicCentimeters"
// };
// 
// enum SpeedUnits_SpeedUnitsEnum {
//   "SpeedUnits::FeetPerMinute",
//   "SpeedUnits::ChainsPerHour",
//   "SpeedUnits::MetersPerSecond",
//   "SpeedUnits::MetersPerMinute",
//   "SpeedUnits::MilesPerHour",
//   "SpeedUnits::KilometersPerHour"
// };
// 
// enum PressureUnits_PressureUnitsEnum {
//   "PressureUnits::Pascal",
//   "PressureUnits::HectoPascal",
//   "PressureUnits::KiloPascal",
//   "PressureUnits::MegaPascal",
//   "PressureUnits::GigaPascal",
//   "PressureUnits::Bar",
//   "PressureUnits::Atmosphere",
//   "PressureUnits::TechnicalAtmosphere",
//   "PressureUnits::PoundPerSquareInch"
// };
// 
// enum SlopeUnits_SlopeUnitsEnum {
//   "SlopeUnits::Degrees",
//   "SlopeUnits::Percent"
// };
// 
// enum DensityUnits_DensityUnitsEnum {
//   "DensityUnits::PoundsPerCubicFoot",
//   "DensityUnits::KilogramsPerCubicMeter"
// };
// 
// enum HeatOfCombustionUnits_HeatOfCombustionUnitsEnum {
//   "HeatOfCombustionUnits::BtusPerPound",
//   "HeatOfCombustionUnits::KilojoulesPerKilogram"
// };
// 
// enum HeatSinkUnits_HeatSinkUnitsEnum {
//   "HeatSinkUnits::BtusPerCubicFoot",
//   "HeatSinkUnits::KilojoulesPerCubicMeter"
// };
// 
// enum HeatPerUnitAreaUnits_HeatPerUnitAreaUnitsEnum {
//   "HeatPerUnitAreaUnits::BtusPerSquareFoot",
//   "HeatPerUnitAreaUnits::KilojoulesPerSquareMeter",
//   "HeatPerUnitAreaUnits::KilowattSecondsPerSquareMeter"
// };
// 
// enum HeatSourceAndReactionIntensityUnits_HeatSourceAndReactionIntensityUnitsEnum {
//   "HeatSourceAndReactionIntensityUnits::BtusPerSquareFootPerMinute",
//   "HeatSourceAndReactionIntensityUnits::BtusPerSquareFootPerSecond",
//   "HeatSourceAndReactionIntensityUnits::KilojoulesPerSquareMeterPerSecond",
//   "HeatSourceAndReactionIntensityUnits::KilojoulesPerSquareMeterPerMinute",
//   "HeatSourceAndReactionIntensityUnits::KilowattsPerSquareMeter"
// };
// 
// enum FirelineIntensityUnits_FirelineIntensityUnitsEnum {
//   "FirelineIntensityUnits::BtusPerFootPerSecond",
//   "FirelineIntensityUnits::BtusPerFootPerMinute",
//   "FirelineIntensityUnits::KilojoulesPerMeterPerSecond",
//   "FirelineIntensityUnits::KilojoulesPerMeterPerMinute",
//   "FirelineIntensityUnits::KilowattsPerMeter"
// };
// 
// enum TemperatureUnits_TemperatureUnitsEnum {
//   "TemperatureUnits::Fahrenheit",
//   "TemperatureUnits::Celsius",
//   "TemperatureUnits::Kelvin"
// };
// 
// enum TimeUnits_TimeUnitsEnum {
//   "TimeUnits::Minutes",
//   "TimeUnits::Seconds",
//   "TimeUnits::Hours"
// };

// %module behave_py %{
// #define SWIG_FILE_WITH_INIT
// #include "behaveUnits.h"
// #include "Contain.h"
// #include "ContainForce.h"
// #include "ContainForceAdapter.h"
// #include "ContainResource.h"
// #include "ContainSim.h"
// #include "ContainAdapter.h"

// #include "behaveRun.h"
// #include "canopy_coefficient_table.h"
// #include "chaparralFuel.h"
// #include "crown.h"
// #include "crownInputs.h"
// #include "fireSize.h"
// #include "fuelModels.h"
// #include "ignite.h"
// #include "igniteInputs.h"
// #include "mortality.h"
// #include "mortality_equation_table.h"
// #include "mortality_inputs.h"
// #include "newext.h"
// #include "palmettoGallberry.h"
// #include "randfuel.h"
// #include "randthread.h"
// #include "safety.h"
// #include "slopeTool.h"
// #include "species_master_table.h"
// #include "spot.h"
// #include "spotInputs.h"
// #include "surface.h"
// #include "surfaceFireReactionIntensity.h"
// #include "surfaceFuelbedIntermediates.h"
// #include "surfaceInputEnums.h"
// #include "surfaceInputs.h"
// #include "surfaceFire.h"
// #include "surfaceTwoFuelModels.h"
// #include "westernAspen.h"
// #include "windAdjustmentFactor.h"
// #include "windSpeedUtility.h"
// %}
