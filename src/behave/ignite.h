/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating the probability of ignition from a firebrand
*           and from lightning
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in the corresponding cpp file is, in part or in
*           whole, from BehavePlus5 source originally authored by Collin D.
*           Bevins and is used with or without modification.
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

#ifndef IGNITE_H
#define IGNITE_H

#include "igniteInputs.h"

class Ignite
{
public:
    Ignite();
    ~Ignite();

    void initializeMembers();

    double calculateFirebrandIgnitionProbability(ProbabilityUnits::ProbabilityUnitsEnum desiredUnits);
    double calculateLightningIgnitionProbability(ProbabilityUnits::ProbabilityUnitsEnum desiredUnits);

    void setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits);
    void setAirTemperature(double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnites);
    void setSunShade(double sunShade, CoverUnits::CoverUnitsEnum sunShadeUnits);
    void setDuffDepth(double duffDepth, LengthUnits::LengthUnitsEnum lengthUnits);
    void setIgnitionFuelBedType(IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType_);
    void setLightningChargeType(LightningCharge::LightningChargeEnum lightningChargeType);

    void updateIgniteInputs(double moistureOneHour, double moistureHundredHour, MoistureUnits::MoistureUnitsEnum moistureUnits,
        double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits, double sunShade, CoverUnits::CoverUnitsEnum sunShadeUnits,
        IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType, double duffDepth, LengthUnits::LengthUnitsEnum duffDepthUnits,
        LightningCharge::LightningChargeEnum lightningChargeType);

    double getAirTemperature(TemperatureUnits::TemperatureUnitsEnum desiredUnits);
    double getFuelTemperature(TemperatureUnits::TemperatureUnitsEnum desiredUnits);
    double getMoistureOneHour(MoistureUnits::MoistureUnitsEnum desiredUnits);
    double getMoistureHundredHour(MoistureUnits::MoistureUnitsEnum desiredUnits);
    double getSunShade(CoverUnits::CoverUnitsEnum desiredUnits);
    double getDuffDepth(LengthUnits::LengthUnitsEnum desiredUnits);
    IgnitionFuelBedType::IgnitionFuelBedTypeEnum getFuelBedType();
    LightningCharge::LightningChargeEnum getLightningChargeType();

    // isFuelDepthNeeded() can be used in applications to determine whether user input of fuel bed depth is 
    // neccessary based on the current value of fuel bed used by the ignite module 
    bool isFuelDepthNeeded();

private:
    double calculateFuelTemperature();

    IgniteInputs igniteInputs_;

    double fuelTemperature_;
};

#endif  // IGNITE_H
