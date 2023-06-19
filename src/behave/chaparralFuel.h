#ifndef CHAPARRALFUEL_H
#define CHAPARRALFUEL_H

#include "surfaceInputEnums.h"

/* ChaparralFuel
 * Implements the Rothermel & Philpot (1973) "Predicting Changes in
 * Chaparral Flammability" (Journal of Fotestry, Oct 1973) including
 * modifications made by Jack Cohen in the FIRECAST program.
 * Rothermel & Philpot are referred to as R&P in the remainder of this
 * comment block. 
 *
 * The purpose of the R&P paper was to assess seasonal and year-to-year
 * changes in chaparral flammability.  This was accomplished by providing
 * chaparral fuel descriptors to the Rothermel fire spread model.
 *
 * The Rothermel fire spread model requires as input the (1) fuel bed depth,
 * (2) fuel bed dead extinction moisture, and (3) fuel particle size, load,
 * density, heat content, moisture content, and silica content by life
 * category and size class.
 *
 * A major portion of the R&P paper is devoted to the derivation of the
 * chaparral fuel descriptors over time (with the remainder devoted to the
 * actual assessment of flammability over time).  Towards this end, R&P:
 * - applied a constant dead fuel extinction of 0.4 (Cohen uses 0.3);
 * - applied constant fuel particle surface area-to-volume ratio, density,
 * and silica content by life category and size class; and
 * - applied constant dead fuel particle heat of combustion of 8000 btu/lb.
 *
 * The remaining parameters are either age-dependent (fuel depth and load)
 * or season-dependent (live fuel moisture and live heat of combustion).
 *
 * First R&P developed a set of equations to apportion fuel load amongst the
 * life and size classes based upon the total fuel load and a dead fuel fraction.
 * They then provide a relationship between total fuel load and age,
 * and between dead fuel fraction and age.  Finally, they developed a relationship
 * between fuel bed depth and age.
 *
 * R&P wanted to include seasonal effects on flammability, so they also
 * developed a relationship between live fuel heat content and seasonal date,
 * and between live fuel moisture content and seasonal date.
 *
 * We can summarize the capabilities of the R&P chaparral fuel models with
 * respect to fire behavior modeling as follows:
 * - fuel bed depth can be (1) specified or (2) estimated from age;
 * - total fuel load can be (1) specified or (2) estimated from age;
 * - dead fuel fraction can be (1) specified or (2) estimated from age;
 * - live fuel heat content can be (1) specified or (2) estimated from seasonal date;
 * - live fuel moisture content can be (1) specified or (2) estimated from seasonal date;
 * - dead fuel moisture content must be specified;
 * - all other fuel model parameters are constant over time, including:
 * - dead fuel moisture of extinction
 * - fuel particle surface area-to-volume ratio by life category and size class;
 * - fuel particle density by life category and size class;
 * - fuel particle total and effective silica content by life category and size class;
 * - dead fuel particle heat content by size class;
 */

class ChaparralFuel
{
public:
    ChaparralFuel();
    virtual ~ChaparralFuel();

    // Accessors
    double getAge() const;
    double getDaysSinceMayFirst() const;
    double getDeadFuelFraction() const;
    double getDeadMoistureOfExtinction() const;
    double getLiveMoistureOfExtinction() const;
    double getDensity(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getFuelBedDepth() const;
    double getHeatOfCombustion(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getLoad(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getMoisture(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getSavr(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getEffectiveSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getTotalSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;
    double getTotalDeadFuelLoad() const;
    double getTotalFuelLoad() const;
    double getTotalLiveFuelLoad() const;
    ChaparralFuelType::ChaparralFuelTypeEnum getChaparralFuelType() const;

    // Mutators
    void setDepth(double depth);
    void setDeadFuelFraction(double deadFuelFraction);
    void setChaparralFuelType(ChaparralFuelType::ChaparralFuelTypeEnum fuelType);
    void setTotalFuelLoad(double totalFuelLoad);

    // Mutators that automatically update other parameters
    void setAge(double years);
    void setDate(int daysSinceMayFirst);
    void setDate(int month, int day);
   
    // Mutators that override previously estimated parameters
    void setMoisture(double moistureValue, FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass);
    void setLiveFuelHeatOfCombustion(double liveLeafHeatOfCombustion, double liveWoodHeatOfCombustion);
    void setLiveFuelMoisture(double liveLeafMoisture, double liveWoodMositure);
 
    // Updaters
    void updateFuelLoadFromDepthAndDeadFuelFraction();
    void updateFuelLoadFromDepthAndFuelType();
    void updateDeadFuelFractionFromAge();
    void updateFuelLoads();
    void updateLiveFuelHeatFromDate();
    void updateLiveFuelMoistureFromDate();
    void updateAgeFromDepth();
    void updateFuelBedDepthFromAge();
    void updateTotalFuelLoadFromAge();

protected:
    void initializeFuelArrays();
    bool isGoodSizeClassIndex(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const;

    double age_;                                        // Fuel age (years since last fire)
    double days_;                                       // Days since May 1
    double deadFuelFraction_;                           // Fraction of total fuel load that is dead
    double deadMoistureOfExtinction_;                   // Dead fuel moisture of extinction content
    double fuelBedDepth_;                               // Fuel bed epth (ft)
    double liveMoistureOfExtinction_;                   // Dead fuel moisture of extinction content
    double totalDeadLoad_;                              // Total dead fuel load
    double totalFuelLoad_;                              // Total fuel load
    double totalLiveLoad_;                              // Total live fuel load
    ChaparralFuelType::ChaparralFuelTypeEnum fuelType_; // Type of chaparral fuel, either chemise or mixed brush

    double density_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];                   // Fuel particle density (lb/ft3) by life category and size class
    double heatOfCombustion_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];          // Fuel heat of combustion (btu/lb) by life category and size class
    double load_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];                      // Fuel load (lb/ft2) by life category and size class
    double moisture_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];                  // Fuel moisture content (ratio) by life category and size class
    double savr_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];                      // Fuel surface area-to-volume ratios by life category and size class
    double effectiveSilicaContent_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];    // Fuel effective silica content (ratio) by life category and size class
    double totalSilicaContent_[FuelConstants::MaxLifeStates][ChaparralContants::NumFuelClasses];        // Fuel total silica content (ratio) by life category and size class
};
#endif    // CHAPARRALFUEL_H
