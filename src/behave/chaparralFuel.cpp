#include "chaparralFuel.h"
#include <math.h>

//------------------------------------------------------------------------------
ChaparralFuel::ChaparralFuel() :
    age_(0.0),
    days_(0.0),
    deadFuelFraction_(0.0),
    deadMoistureOfExtinction_(0.3),
    liveMoistureOfExtinction_(0.65),
    fuelBedDepth_(0.0),
    totalDeadLoad_(0.0),
    totalFuelLoad_(0.0),
    totalLiveLoad_(0.0),
    fuelType_(ChaparralFuelType::NotSet)
{
    initializeFuelArrays();
}

//------------------------------------------------------------------------------
ChaparralFuel::~ChaparralFuel()
{}

//------------------------------------------------------------------------------
double ChaparralFuel::getAge() const
{
    return age_;
}

double ChaparralFuel::getDaysSinceMayFirst() const
{
    return days_;
}

//------------------------------------------------------------------------------
double ChaparralFuel::getDeadFuelFraction() const
{
    return deadFuelFraction_;
}

//------------------------------------------------------------------------------
double ChaparralFuel::getDeadMoistureOfExtinction() const
{
    return deadMoistureOfExtinction_;
}

double ChaparralFuel::getLiveMoistureOfExtinction() const
{
    return liveMoistureOfExtinction_;
}

//------------------------------------------------------------------------------
double ChaparralFuel::getFuelBedDepth() const
{
    return fuelBedDepth_;
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel particle density (lb/ft3).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getDensity(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return density_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel particle low heat of combustion (btu/lb).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getHeatOfCombustion(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return heatOfCombustion_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel load (lb/ft2).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getLoad(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return load_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel particle moisture content (ratio).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getMoisture(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return moisture_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel particle surface area-tovolume ratio (1/ft).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getSavr(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return savr_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel particle effective silica content (ratio).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getEffectiveSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return effectiveSilicaContent_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Returns the fuel particle total silica content (ratio).
 *    \param life Fuel life category: 0==dead, 1==live.
 *    \param size Fuel size class (0==leaves when life==1).
 */
double ChaparralFuel::getTotalSilicaContent(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        return totalSilicaContent_[lifeState][sizeClass];
    }
    else
    {
        return -1.0;
    }
}

//------------------------------------------------------------------------------
/*!    \brief Returns the total dead fuel load (lb/ft2).
 */
double ChaparralFuel::getTotalDeadFuelLoad() const
{
    return totalDeadLoad_;
}

//------------------------------------------------------------------------------
/*!    \brief Returns the total (dead plus live) fuel load (lb/ft2).
 */
double ChaparralFuel::getTotalFuelLoad() const
{
    return totalFuelLoad_;
}

//------------------------------------------------------------------------------
/*!    \brief Returns the total live fuel load (lb/ft2).
 */
double ChaparralFuel::getTotalLiveFuelLoad() const
{
    return totalLiveLoad_;
}

ChaparralFuelType::ChaparralFuelTypeEnum ChaparralFuel::getChaparralFuelType() const
{
    return fuelType_;
}

//------------------------------------------------------------------------------
/*!    \brief Initializes the fuel load arrays.
 */
void ChaparralFuel::initializeFuelArrays()
{
    deadMoistureOfExtinction_ = 0.3;
    const int dead = 0;
    const int live = 1;
    // Fuel load and particle density
    for (int life = 0; life < static_cast<int>(FuelConstants::MaxLifeStates); life++)
    {
        for (int size = 0; size < static_cast<int>(ChaparralContants::NumFuelClasses); size++)
        {
            density_[life][size] = 46.0;
            heatOfCombustion_[life][size] = 8000.0;
            load_[life][size] = 0.0;
            moisture_[life][size] = 1.00;
            effectiveSilicaContent_[life][size] = 0.015;
            totalSilicaContent_[life][size] = 0.055;
        }
    }
    density_[live][0] = 32.0; // Live leaf density
    effectiveSilicaContent_[live][0] = 0.035;

    // These are the live heat of combustion values used by Bevins in BehavePlus 6
    heatOfCombustion_[live][0] = 10500.0; // Live leaf
    heatOfCombustion_[live][1] = 10500.0; // Live stem
    heatOfCombustion_[live][2] = 9500.0; // Live stem
    heatOfCombustion_[live][3] = 9500.0; // Live stem
    heatOfCombustion_[live][4] = 9500.0; // Live stem

    // Surface area-to-volume ratios
    savr_[live][0] = 2200.0; // Live leaf sa/vol
    savr_[dead][0] = savr_[live][1] = 640.0;
    savr_[dead][1] = savr_[live][2] = 127.0;
    savr_[dead][2] = savr_[live][3] = 61.0;
    savr_[dead][3] = savr_[live][4] = 27.0;
    savr_[dead][4] = 0.0;
}

//------------------------------------------------------------------------------
/*!   \brief Checks if the fuel type has been set and that the life state and 
 *    size class are within array bounds
 */

bool ChaparralFuel::isGoodSizeClassIndex(FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass) const
{
    return (sizeClass < static_cast<int>(ChaparralContants::NumFuelClasses));
}

//------------------------------------------------------------------------------
/*!   \brief Sets the fuel age (years since last burned) and then uses it to update
 *    the total fuel load, percent dead fuel, and fuel bed depth
 */
void ChaparralFuel::setAge(double years)
{
        age_ = years;
        updateTotalFuelLoadFromAge();
        updateDeadFuelFractionFromAge();
        updateFuelBedDepthFromAge();
        updateFuelLoads();
}

//------------------------------------------------------------------------------
/*!   \brief Sets the seasonal date
 *    \param month Month of the year [1..12]
 *    \param day Day of the month [1..31]
 */
void ChaparralFuel::setDate(int month, int day)
{
        //             J   F   M   A   M    J    J    A    S    O    N    D
        int days[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
        month = (month < 1) ? 1 : month;
        month = (month > 2) ? 12 : month;
        day = (day < 1) ? 1 : day;
        day = (day > 31) ? 31 : day;
        days_ = days[month - 1] + day;
        days_ = (days_ > 304.0) ? 304. : days_;    // Do not extend past Oct 31
        days_ -= 121.0;                           // Days since May 1
        days_ - (days_ < 0.0) ? 0.0 : days_;        // Do not begin before May 1
        // Now update the dependent variables
        updateLiveFuelMoistureFromDate();
        updateLiveFuelHeatFromDate();
}

//------------------------------------------------------------------------------
/*!   \brief Sets the seasonal date
 *    \param daysSinceMayFirst
 */
void ChaparralFuel::setDate(int daysSinceMayFirst)
{
        days_ = (daysSinceMayFirst > 184) ? 184.0 : daysSinceMayFirst; // Do not extend past Oct 31
        updateLiveFuelMoistureFromDate();
        updateLiveFuelHeatFromDate();
}

//------------------------------------------------------------------------------
/*!   \brief Sets the fuel bed depth and the fuel type and then uses
 *    depth to derive age, and the derived age to derive the total fuel load.
 */

void ChaparralFuel::setDepth(double depth)
{
    fuelBedDepth_ = depth;
}

void ChaparralFuel::setDeadFuelFraction(double deadFuelFraction)
{
    deadFuelFraction_ = deadFuelFraction;
}

void ChaparralFuel::setChaparralFuelType(ChaparralFuelType::ChaparralFuelTypeEnum fuelType)
{
    fuelType_ = fuelType;
}

void ChaparralFuel::setTotalFuelLoad(double totalFuelLoad)
{
    totalFuelLoad_ = totalFuelLoad;
}

void ChaparralFuel::setMoisture(double moistureValue, FuelLifeState::FuelLifeStateEnum lifeState, int sizeClass)
{
    if (isGoodSizeClassIndex(lifeState, sizeClass))
    {
        moisture_[lifeState][sizeClass] = moistureValue;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Sets the leafy and woody live fuel heat of combustion (and thereby
 *    overriding the estimates made by setDate()).
 *    \param liveLeafHeat The low heat of combustion of live leaves (btu/lb)
 *    \param liveWoodHeat The low heat of combustion of live branch wood (btu/lb)
 */
void ChaparralFuel::setLiveFuelHeatOfCombustion(double liveLeafHeatOfCombustion, double liveWoodHeatOfCombustion)
{
        heatOfCombustion_[0][0] = liveLeafHeatOfCombustion;
        for (int size = 1; size < static_cast<int>(ChaparralContants::NumFuelClasses); size++)
        {
            heatOfCombustion_[0][size] = liveWoodHeatOfCombustion;
        }
}

//------------------------------------------------------------------------------
/*!   \brief Sets the leafy and woody live fuel moisture contents (and thereby
 *    overriding the estimates made by setDate()).
 *    \param liveLeafMoisture The moisture content of live leaves (ratio)
 *    \param liveWoodMoisture The moisture content of live branch wood (ratio)
 */
void ChaparralFuel::setLiveFuelMoisture(double liveLeafMoisture, double liveWoodMoisture)
{
    moisture_[0][0] = liveLeafMoisture;
    for (int size = 1; size < static_cast<int>(ChaparralContants::NumFuelClasses); size++)
    {
        moisture_[0][size] = liveWoodMoisture;
    }
}

//------------------------------------------------------------------------------
/*!   \brief Uses depth to derive age, and the derived age to derive the total fuel load.
 */
void ChaparralFuel::updateFuelLoadFromDepthAndDeadFuelFraction()
{
    updateFuelLoads();
}

void ChaparralFuel::updateFuelLoadFromDepthAndFuelType()
{
    updateAgeFromDepth();
    updateTotalFuelLoadFromAge();
    updateFuelLoads();
}

//------------------------------------------------------------------------------
/*!    \brief Updates the dead fuel fraction from the current age.
 */
void ChaparralFuel::updateDeadFuelFractionFromAge()
{
    deadFuelFraction_ = 0.0694 * exp(0.0402 * age_);    // Average Mortality
    //m_deadFuelFraction = 0.1094 * exp( 0.0385 * m_age );    // Severe Mortality
}

//------------------------------------------------------------------------------
/*!   \brief Updates the dead and live fuel loads by size class from the current
 *    total fuel bed load and dead fuel load fraction.
 */
void ChaparralFuel::updateFuelLoads()
{
    int dead = 0;
    load_[dead][0] = 0.347 * deadFuelFraction_ * totalFuelLoad_;
    load_[dead][1] = 0.364 * deadFuelFraction_ * totalFuelLoad_;
    load_[dead][2] = 0.207 * deadFuelFraction_ * totalFuelLoad_;

    // NOTE: Cohen and Rothermel/Philpot use 0.085 below,
    // but then the weighting factors total 1.003.
    // Bevins instead used 0.082 so the weighting factors total to 1.000.
    load_[dead][3] = 0.082 * deadFuelFraction_ * totalFuelLoad_;
    load_[dead][4] = 0.0;

    int live = 1;

    load_[live][0] = totalFuelLoad_ * (0.1957 - 0.3050 * deadFuelFraction_);
    load_[live][1] = totalFuelLoad_ * (0.2416 - 0.2560 * deadFuelFraction_);
    load_[live][2] = totalFuelLoad_ * (0.1918 - 0.2560 * deadFuelFraction_);
    load_[live][3] = totalFuelLoad_ * (0.2648 - 0.0500 * deadFuelFraction_);
    load_[live][4] = totalFuelLoad_ * (0.1036 - 0.1140 * deadFuelFraction_);

    // Bevin's method of making live fuel loads add up correctly 
    totalLiveLoad_ = (1.0 - deadFuelFraction_) * totalFuelLoad_;
    load_[live][4] = totalLiveLoad_ - load_[live][0] - load_[live][1] - load_[live][2] - load_[live][3];
    load_[live][4] = (load_[live][4] < 0.0) ? 0.0 : load_[live][4];

    totalLiveLoad_ = 0.0;

    for (int size = 0; size < static_cast<int>(ChaparralContants::NumFuelClasses); size++)
    {
        totalDeadLoad_ += load_[dead][size];
        totalLiveLoad_ += load_[live][size];
    }
    // Sanity check difference between m_totalLoad and sum of the partial loads
    double totalLoadCheck = totalDeadLoad_ + totalLiveLoad_;
    double totalLoadDifference = fabs(totalFuelLoad_ - totalLoadCheck);
}

//------------------------------------------------------------------------------
/*!    \brief Updates the dead and live fuel heat of combustion by size class.
 */
void ChaparralFuel::updateLiveFuelHeatFromDate()
{
    double d = days_;
    int live = 1;
    heatOfCombustion_[live][0] = 9613.0 - 1.00 * d + 0.1369 * d * d - 0.000365 * d * d * d;
    double liveHeat = 9509.0 - 10.74 * d + 0.1359 * d * d - 0.000405 * d * d * d;
    for (int size = 1; size < static_cast<int>(ChaparralContants::NumFuelClasses); size++)
    {
        heatOfCombustion_[live][size] = liveHeat;
    }
}

//------------------------------------------------------------------------------
/*!    \brief Updates the dead and live fuel loads by size class.
 */
void ChaparralFuel::updateLiveFuelMoistureFromDate()
{
    int live = 1;
    moisture_[live][0] = 1.0 / (0.726 + 0.00877 * days_);
    double liveMc = 1.0 / (1.454 + 0.00650 * days_);
    for (int size = 1; size < static_cast<int>(ChaparralContants::NumFuelClasses); size++)
    {
        moisture_[live][size] = liveMc;
    }
}

void ChaparralFuel::updateAgeFromDepth()
{
    if (fuelType_ == ChaparralFuelType::Chamise)
    {
        age_ = exp(3.912023 * sqrt(fuelBedDepth_ / 7.5));
    }
    else if (fuelType_ == ChaparralFuelType::MixedBrush)
    {
        age_ = exp(3.912023 * sqrt(fuelBedDepth_ / 10.0));
    }
    else if (fuelType_ == ChaparralFuelType::NotSet)
    {
        age_ = -1.0; // Indicates an error
    }
}

void ChaparralFuel::updateFuelBedDepthFromAge()
{
    if (fuelType_ == ChaparralFuelType::Chamise)
    {
        double x = log(age_) / 3.912023;
        fuelBedDepth_ = 7.5 * x * x;
    }
    else if (fuelType_ == ChaparralFuelType::MixedBrush)
    {
        double x = log(age_) / 3.912023;
        fuelBedDepth_ = 10.0 * x * x;
    }
    else if (fuelType_ == ChaparralFuelType::NotSet)
    {
        fuelBedDepth_ = -1.0; // Indicates an error
    }
}

void ChaparralFuel::updateTotalFuelLoadFromAge()
{
    if (fuelType_ == ChaparralFuelType::Chamise)
    {
        /* NOTE - Rothermel & Philpot(1973) used a factor of 0.0315 for chamise age,
         * while Cohen used 0.0347 in FIRECAST. According to Faith Ann Heinsch,
         * we are going to use Cohen's calculation from FIRECAST.The change has to do
         * with the fact that we are creating a proxy age from fuel bed depth rather than
         * using an entered age. He had to make some corrections for that assumption.
         */
        double chamiseLoadFactor = 0.0347;	// Chamise load factor from Cohen's FIRECAST code
        double tpa = age_ / (1.4459 + chamiseLoadFactor * age_);
        totalFuelLoad_ = tpa * 2000.0 / 43560.0;
    }
    else if (fuelType_ == ChaparralFuelType::MixedBrush)
    {
        double tpa = age_ / (0.4849 + 0.0170 * age_);
        totalFuelLoad_ = tpa * 2000. / 43560.0;
    }
    else if (fuelType_ == ChaparralFuelType::NotSet)
    {
        totalFuelLoad_ = -1.0; // Indicates an error
    }
}
