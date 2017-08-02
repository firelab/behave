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

    double calculateFirebrandIgnitionProbability();
    double calculateLightningIgnitionProbability();

    void setFuelTemperature(double fuelTemperature);
    void setMoistureOneHour(double moistureOneHour, MoistureUnits::MoistureUnitsEnum desiredUnits);
    void setMoistureHundredHour(double moistureHundredHour, MoistureUnits::MoistureUnitsEnum desiredUnits);
    void setAirTemperature(double airTemperature);
    void setSunShade(double sunShade);
    void setIgnitionFuelBedType(IgnitionFuelBedType::IgnitionFuelBedTypeEnum fuelBedType_);
    void setDuffDepth(double duffDepth, LengthUnits::LengthUnitsEnum lengthUnits);
    void setLightningChargeType(LightningCharge::LightningChargeEnum lightningChargeType);

    double getFuelTemperature();
    double getOneHourMoisture();
    double getAirTemperature();
    double getSunShade();
    IgnitionFuelBedType::IgnitionFuelBedTypeEnum getIgnitionFuelBedType();
    double getDuffDepth();
    double getHundredHourMoisture();
    LightningCharge::LightningChargeEnum getLightningChargeType();

private:
    double calculateFuelTemperature();

    IgniteInputs igniteInputs_;
};

#endif  // IGNITE_H
