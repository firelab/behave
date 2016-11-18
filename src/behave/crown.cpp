/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling crown fire behavior
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
*******************************************************************************
*
* THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
* MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
* IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
* OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
* PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
* LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
* PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
* RELIABILITY, OR ANY OTHER CHARACTERISTIC.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
******************************************************************************/

#include "crown.h"

#include <cmath>
#include "fuelModelSet.h"
#include "surfaceEnums.h"
#include "windSpeedUtility.h"

Crown::Crown(const FuelModelSet& fuelModelSet, const Surface& surface)
    : crownDeepCopyOfSurface_(fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet; // point to the same location as BehaveRun'sfuelModels
    surface_ = &surface; // point to the same location as BehaveRun's surface
    crownDeepCopyOfSurface_ = *surface_; // copy the actual data surfaceInputs is pointing to

    initializeMemmbers();
}

Crown::~Crown()
{

}

Crown::Crown(const Crown& rhs)
    : crownDeepCopyOfSurface_(*rhs.fuelModelSet_)
{
    memberwiseCopyAssignment(rhs);
}

Crown& Crown::operator=(const Crown& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void Crown::memberwiseCopyAssignment(const Crown& rhs)
{
    fuelModelSet_ = rhs.fuelModelSet_;
    surface_ = rhs.surface_;
    crownDeepCopyOfSurface_ = rhs.crownDeepCopyOfSurface_;
    crownInputs_ = rhs.crownInputs_;

    fireType_ = rhs.fireType_;
    crownCopyOfSurfaceHeatPerUnitArea_ = rhs.crownCopyOfSurfaceHeatPerUnitArea_;
    crownCopyOfSurfaceFirelineIntensity_ = rhs.crownCopyOfSurfaceFirelineIntensity_;
    crownFuelLoad_ = rhs.crownFuelLoad_;
    canopyHeatPerUnitArea_ = rhs.canopyHeatPerUnitArea_;
    crownFireHeatPerUnitArea_ = rhs.crownFireHeatPerUnitArea_;
    crownFirelineIntensity_ = rhs.crownFirelineIntensity_;
    crownFlameLength_ = rhs.crownFlameLength_;
    crownFireSpreadRate_ = rhs.crownFireSpreadRate_;
    crownCriticalSurfaceFireIntensity_ = rhs.crownCriticalSurfaceFireIntensity_;
    crownCriticalFireSpreadRate_ = rhs.crownCriticalFireSpreadRate_;
    crownCriticalSurfaceFlameLength_ = rhs.crownCriticalSurfaceFlameLength_;
    crownPowerOfFire_ = rhs.crownPowerOfFire_;
    crownPowerOfWind_ = rhs.crownPowerOfWind_;
    crownFirePowerRatio_ = rhs.crownFirePowerRatio_;
    crownFireActiveRatio_ = rhs.crownFireActiveRatio_;
    crownFireTransitionRatio_ = rhs.crownFireTransitionRatio_;
    windSpeedAtTwentyFeet_ = rhs.windSpeedAtTwentyFeet_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire spread rate.
*
*  This uses Rothermel's 1991 crown fire correlation.
*
*  \return Crown fire average spread rate (ft/min).
*
*/
void Crown::doCrownRun()
{
    // Step 0: Update Crown's copy of Surface
    updateDeepCopyOfSurface();

    // Step 1: Create the crown fuel model (fire behavior fuel model 10)
    crownDeepCopyOfSurface_.setFuelModelNumber(10);    // set the fuel model used to fuel model 10
    crownDeepCopyOfSurface_.setSlope(0.0);             // slope is always assumed to be zero in crown ROS
    crownDeepCopyOfSurface_.setWindDirection(0.0);     // wind direction is assumed to be upslope in crown ROS

    double windAdjustmentFactor = 0.4; // wind adjustment factor is assumed to be 0.4 for crown ROS
    crownDeepCopyOfSurface_.setUserProvidedWindAdjustmentFactor(windAdjustmentFactor);

    // Step 2: Determine fire behavior
    crownDeepCopyOfSurface_.doSurfaceRunInDirectionOfMaxSpread(); // Crown fire is always in direction of max spread
    crownFireSpreadRate_ = 3.34 * crownDeepCopyOfSurface_.getSpreadRate(); // Rothermel 1991

    //  Step 3:  Get values from Surface needed for further calculations 
    crownCopyOfSurfaceHeatPerUnitArea_ = surface_->getHeatPerUnitArea();
    crownCopyOfSurfaceFirelineIntensity_ = surface_->getFirelineIntensity();

    //  Step 4: Calculate remaining crown fire characteristics
    calculateCrownFuelLoad();
    calculateCanopyHeatPerUnitArea();
    calculateCrownFireHeatPerUnitArea();
    calculateCrownFirelineIntensity();
    calculateCrownFlameLength();

    calculateCrownCriticalFireSpreadRate();
    calculateCrownFireActiveRatio();

    calculateCrownCriticalSurfaceFireIntensity();
    calculateCrownCriticalSurfaceFlameLength();
    calculateCrownFireTransitionRatio();

    calculateCrownPowerOfFire();
    calcuateCrownPowerOfWind();
    calcualteCrownFirePowerRatio();

    calculateFireType();
}

void Crown::updateDeepCopyOfSurface()
{
    crownDeepCopyOfSurface_ = *surface_; // copy the actual data surface is pointing to
}

double Crown::getCrownFireSpreadRate() const
{
    return crownFireSpreadRate_;
}

double Crown::getCrownFirelineIntensity() const
{
    return crownFirelineIntensity_;
}

double Crown::getCrownFlameLength() const
{
    return crownFlameLength_;
}

FireType::FireTypeEnum Crown::getFireType() const
{
    return fireType_;
}

DensityUnits::DensityUnitsEnum Crown::getCanopyBulkDensityUnits() const
{
    return crownInputs_.getCanopyBulkDensityUnits();
}

LengthUnits::LengthUnitsEnum Crown::getCanopyBaseHeightUnits() const
{
    return crownInputs_.getCanopyBaseHeightUnits();
}

void Crown::initializeMemmbers()
{
    fireType_ = FireType::SURFACE;
    crownCopyOfSurfaceHeatPerUnitArea_ = 0.0;
    crownCopyOfSurfaceFirelineIntensity_ = 0.0;
    crownFuelLoad_ = 0.0;
    canopyHeatPerUnitArea_ = 0.0;
    crownFireHeatPerUnitArea_ = 0.0;
    crownFirelineIntensity_ = 0.0;
    crownFlameLength_ = 0.0;
    crownFireSpreadRate_ = 0.0;
    crownCriticalSurfaceFireIntensity_ = 0.0;
    crownCriticalFireSpreadRate_ = 0.0;
    crownCriticalSurfaceFlameLength_ = 0.0;
    crownPowerOfFire_ = 0.0;
    crownPowerOfWind_ = 0.0;
    crownFirePowerRatio_ = 0.0;
    crownFireActiveRatio_ = 0.0;
    crownFireTransitionRatio_ = 0.0;
    windSpeedAtTwentyFeet_ = 0.0;;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the canopy portion of the crown fire heat per unit area
*  given the crown fire fuel load and low heat of combustion.
*
*/
void Crown::calculateCanopyHeatPerUnitArea()
{
    const double LOW_HEAT_OF_COMBUSTION = 8000.0; // Low heat of combustion (hard coded to 8000 Btu/lbs)
    canopyHeatPerUnitArea_ = crownFuelLoad_ * LOW_HEAT_OF_COMBUSTION;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the total crown fire heat per unit area
*  by summing surface HPUA and canopy HPUA.
*
*/
void Crown::calculateCrownFireHeatPerUnitArea()
{
    crownFireHeatPerUnitArea_ = crownCopyOfSurfaceHeatPerUnitArea_ + canopyHeatPerUnitArea_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fuel load
*  given the canopy bulk density and canopy height.
*
*/
void Crown::calculateCrownFuelLoad()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity();
    double canopyBaseHeight = crownInputs_.getCanopyBaseHeight();
    double canopyHeight = surface_->getCanopyHeight();
    crownFuelLoad_ = canopyBulkDensity * (canopyHeight - canopyBaseHeight);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire transition ratio.
*
*  given the surface fireline intensity (Btu/ft/s) and the
*  critical crown fire fireline intensity (Btu/ft/s).
*
*/
void Crown::calculateCrownFireTransitionRatio()
{
   crownFireTransitionRatio_ = ((crownCriticalSurfaceFireIntensity_ < 1.0e-7)
        ? (0.00)
        : (crownCopyOfSurfaceFirelineIntensity_ / crownCriticalSurfaceFireIntensity_));
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire fireline intensity
*  given the surface fire and crown fire heats per unit area
*  and the crown fire spread rate.
*
*
*/
void Crown::calculateCrownFirelineIntensity()
{
    crownFirelineIntensity_ = (crownFireSpreadRate_ / 60.0) * crownFireHeatPerUnitArea_;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical surface fire intensity for a surface fire
*  to transition to a crown fire given the foliar moisture and crown base height
*
*/
void Crown::calculateCrownCriticalSurfaceFireIntensity()
{
    const double KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND = 0.288672;

    double moistureFoliar = crownInputs_.getMoistureFoliar();

    // Convert foliar moisture content to percent and constrain lower limit
    moistureFoliar = MoistureUnits::fromBaseUnits(moistureFoliar, MoistureUnits::PERCENT);
    moistureFoliar = (moistureFoliar < 30.0) ? 30.0 : moistureFoliar;

    double crownBaseHeight = crownInputs_.getCanopyBaseHeight();
    // Convert crown base height to meters and constrain lower limit
    crownBaseHeight = LengthUnits::fromBaseUnits(crownBaseHeight, LengthUnits::METERS);
    crownBaseHeight = (crownBaseHeight < 0.1) ? 0.1 : crownBaseHeight;

    // Critical surface fireline intensity (kW/m)
    // Need to changed value in calculation below from 450 to 460 at some point
    crownCriticalSurfaceFireIntensity_ = pow((0.010 * crownBaseHeight * (450.0 + 25.9 * moistureFoliar)), 1.5);

    // Return as Btu/ft/s
    crownCriticalSurfaceFireIntensity_ *= KILOWATTS_PER_METER_TO_BTUS_PER_FOOT_PER_SECOND;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical surface fire flame length for a surface fire
*  to transition to a crown fire given the critical fireline intensity.
*
*/
void Crown::calculateCrownCriticalSurfaceFlameLength()
{
    crownCriticalSurfaceFlameLength_ = crownDeepCopyOfSurface_.calculateFlameLength(crownCriticalSurfaceFireIntensity_);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire flame length
*  given the crown fireline intensity.
*
*/
void Crown::calculateCrownFlameLength()
{
    crownFlameLength_ = 0.2 * pow(crownFirelineIntensity_, (2.0 / 3.0));
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire 'power of the fire'
*  given the crown fireline intensity.
*
*/
void Crown::calculateCrownPowerOfFire()
{
    crownPowerOfFire_ = crownFirelineIntensity_ / 129.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire 'power of the wind'
*  given the 20-ft wind speed and crown fire spread rate.
*
*/
void Crown::calcuateCrownPowerOfWind()
{
    const double SECONDS_PER_MINUTE = 60.0;
    
    windSpeedAtTwentyFeet_ = calculateWindSpeedAtTwentyFeet();

    //double windSpeedInFeetPerMinute = windSpeedAtTwentyFeet_ * MILES_PER_HOUR_TO_FEET_PER_MINUTE;

    double WindspeedMinusCrownROS = 0.0; 

    // Eq. 7, Rothermel 1991
    WindspeedMinusCrownROS = (windSpeedAtTwentyFeet_ - crownFireSpreadRate_) / SECONDS_PER_MINUTE;
    WindspeedMinusCrownROS = (WindspeedMinusCrownROS < 1e-07) ? 0.0 : WindspeedMinusCrownROS;
    crownPowerOfWind_ = 0.00106 * (WindspeedMinusCrownROS * WindspeedMinusCrownROS * WindspeedMinusCrownROS);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire 'power-of-fire to power-of-wind' ratio
*
*  \return Ratio of the crown fire 'power-of-the-fire' to 'power-of-the-wind).
*/
void Crown::calcualteCrownFirePowerRatio()
{
    crownFirePowerRatio_ = (crownPowerOfWind_ > 1e-07) ? (crownPowerOfFire_ / crownPowerOfWind_) : 0.0;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the critical crown fire spread rate to achieve active
*  crowning given the canopy crown bulk density
*
*/
void Crown::calculateCrownCriticalFireSpreadRate()
{
    double canopyBulkDensity = crownInputs_.getCanopyBulkDensity();
   
    // Convert canopy bulk density to Kg/m3
    double convertedCanopyBulkDensity = DensityUnits::fromBaseUnits(canopyBulkDensity, DensityUnits::KILOGRAMS_PER_CUBIC_METER);
    crownCriticalFireSpreadRate_ = (convertedCanopyBulkDensity < 1e-07) ? 0.00 : (3.0 / convertedCanopyBulkDensity);

    // Convert spread rate from m/min to ft/min
    crownCriticalFireSpreadRate_ = VelocityUnits::toBaseUnits(crownCriticalFireSpreadRate_, VelocityUnits::METERS_PER_MINUTE);
}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire active ratio given the crown fire spread 
*  rate and the critical crown fire spread rate.
*
*/
void Crown::calculateCrownFireActiveRatio()
{
    crownFireActiveRatio_ = (crownCriticalFireSpreadRate_ < 1e-07)
        ? (0.00)
        : (crownFireSpreadRate_ / crownCriticalFireSpreadRate_);
}

double Crown::calculateWindSpeedAtTwentyFeet()
{
    windSpeedAtTwentyFeet_ = -1; // If negative 1 is returned, there is an error
    WindHeightInputMode::WindHeightInputModeEnum windHeightInputMode;
    windHeightInputMode = surface_->getWindHeightInputMode();

    if (windHeightInputMode == WindHeightInputMode::TWENTY_FOOT)
    {
        windSpeedAtTwentyFeet_ = surface_->getWindSpeed();
    }
    else if (windHeightInputMode == WindHeightInputMode::TEN_METER)
    {
        WindSpeedUtility windSpeedUtility;
        double windSpeedAtTenMeters = surface_->getWindSpeed();
        windSpeedAtTwentyFeet_ = windSpeedUtility.windSpeedAtTwentyFeetFromTenMeter(windSpeedAtTenMeters);
    }
    return windSpeedAtTwentyFeet_;
}

//----------------------------------------------------------------------------------
/*! \brief Calculates the fire type; surface, passive, or active.
*
*  \param transitionRatio  Crown fire transition ratio.
*  \param activeRatio      Crown fire active ratio.
*
*  \retval SURFACE                  == Surface: surface fire with no torching or
*                                       crown fire spread.
*  \retval TORCHING                 == Torching: surface fire with torching.
*  \retval CONDITIONAL_CROWN_FIRE   == Conditional crown fire: conditions indicate 
*                                       the fire will not transition from the 
*                                       surface to the crown. But if it does, an 
*                                       active crown fire may result.
*  \retval CROWNING                 == Crowing: active crown fire, fire is spreading
*                                       through the canopy.
*/

void Crown::calculateFireType()
{
    fireType_ = FireType::SURFACE;
    // If the fire CAN NOT transition to the crown ...
    if (crownFireTransitionRatio_ < 1.0)
    {
        if (crownFireActiveRatio_ < 1.0)
        {
            fireType_ = FireType::SURFACE; // Surface fire
        }
        else // crownFireActiveRatio_ >= 1.0 
        {
            fireType_ = FireType::CONDITIONAL_CROWN_FIRE; // Conditional crown fire
        }
    }
    // If the fire CAN transition to the crown ...
    else // crownFireTransitionRatio_ >= 1.0 )
    {
        if (crownFireActiveRatio_ < 1.0)
        {
            fireType_ = FireType::TORCHING; // Torching
        }
        else // crownFireActiveRatio_ >= 1.0
        {
            fireType_ = FireType::CROWNING; // Crowning
        }
    }
}

void Crown::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    crownInputs_.updateCrownInputs(canopyBaseHeight, canopyBulkDensity, foliarMoisture);
}

void Crown::setCanopyBaseHeight(double canopyBaseHeight)
{
    crownInputs_.setCanopyBaseHeight(canopyBaseHeight);
}

void Crown::setCanopyBulkDensity(double canopyBulkDensity)
{
    crownInputs_.setCanopyBulkDensity(canopyBulkDensity);
}

void Crown::setMoistureFoliar(double moistureFoliar)
{
    crownInputs_.setMoistureFoliar(moistureFoliar);
}

void Crown::setMoistureUnits(MoistureUnits::MoistureUnitsEnum moistureUnits)
{
    crownInputs_.setMoistureUnits(moistureUnits);
}

void Crown::setCanopyBulkDensityUnits(DensityUnits::DensityUnitsEnum densityUnits)
{
    crownInputs_.setCanopyBulkDensityUnits(densityUnits);
}

void Crown::setCanopyBaseHeightUnits(LengthUnits::LengthUnitsEnum canopyBaseHeightUnits)
{
    crownInputs_.setCanopyBaseHeightUnits(canopyBaseHeightUnits);
}

double Crown::getCanopyBaseHeight() const
{
    return crownInputs_.getCanopyBaseHeight();
}

double Crown::getCanopyBulkDensity() const
{
    return crownInputs_.getCanopyBulkDensity();
}

double Crown::getMoistureFoliar() const
{
    return crownInputs_.getMoistureFoliar();
}
