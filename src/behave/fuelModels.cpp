/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling values associated with fuel models used in the
*           Rothermel model
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

#include "fuelModels.h"

#include "surfaceInputs.h"

FuelModels::FuelModels()
{
    FuelModelVector_.resize(FuelConstants::MaxFuelModels);
    initializeAllFuelModelRecords();
    populateFuelModels();
}

FuelModels::FuelModels(const FuelModels& rhs)
{
    memberwiseCopyAssignment(rhs);
}

FuelModels& FuelModels::operator=(const FuelModels& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void FuelModels::memberwiseCopyAssignment(const FuelModels& rhs)
{
    FuelModelVector_.resize(rhs.FuelModelVector_.size());
    for (unsigned int i = 0; i < rhs.FuelModelVector_.size(); i++)
    {
        FuelModelVector_[i].fuelModelNumber_ = rhs.FuelModelVector_[i].fuelModelNumber_;
        FuelModelVector_[i].code_ = rhs.FuelModelVector_[i].code_;
        FuelModelVector_[i].name_ = rhs.FuelModelVector_[i].name_;
        FuelModelVector_[i].fuelbedDepth_ = rhs.FuelModelVector_[i].fuelbedDepth_;
        FuelModelVector_[i].moistureOfExtinctionDead_ = rhs.FuelModelVector_[i].moistureOfExtinctionDead_;
        FuelModelVector_[i].heatOfCombustionDead_ = rhs.FuelModelVector_[i].heatOfCombustionDead_;
        FuelModelVector_[i].heatOfCombustionLive_ = rhs.FuelModelVector_[i].heatOfCombustionLive_;
        FuelModelVector_[i].fuelLoadOneHour_ = rhs.FuelModelVector_[i].fuelLoadOneHour_;
        FuelModelVector_[i].fuelLoadTenHour_ = rhs.FuelModelVector_[i].fuelLoadTenHour_;
        FuelModelVector_[i].fuelLoadHundredHour_ = rhs.FuelModelVector_[i].fuelLoadHundredHour_;
        FuelModelVector_[i].fuelLoadLiveHerbaceous_ = rhs.FuelModelVector_[i].fuelLoadLiveHerbaceous_;
        FuelModelVector_[i].fuelLoadLiveWoody_ = rhs.FuelModelVector_[i].fuelLoadLiveWoody_;
        FuelModelVector_[i].savrOneHour_ = rhs.FuelModelVector_[i].savrOneHour_;
        FuelModelVector_[i].savrLiveHerbaceous_ = rhs.FuelModelVector_[i].savrLiveHerbaceous_;
        FuelModelVector_[i].savrLiveWoody_ = rhs.FuelModelVector_[i].savrLiveWoody_;
        FuelModelVector_[i].isReserved_ = rhs.FuelModelVector_[i].isReserved_;
        FuelModelVector_[i].isDefined_ = rhs.FuelModelVector_[i].isDefined_;
    }
}

FuelModels::~FuelModels()
{

}

void FuelModels::initializeSingleFuelModelRecord(int fuelModelNumber)
{
    FuelModelVector_[fuelModelNumber].fuelModelNumber_ = 0;
    FuelModelVector_[fuelModelNumber].code_ = "NO_CODE";
    FuelModelVector_[fuelModelNumber].name_ = "NO_NAME";
    FuelModelVector_[fuelModelNumber].fuelbedDepth_ = 0;
    FuelModelVector_[fuelModelNumber].moistureOfExtinctionDead_ = 0;
    FuelModelVector_[fuelModelNumber].heatOfCombustionDead_ = 0;
    FuelModelVector_[fuelModelNumber].heatOfCombustionLive_ = 0;
    FuelModelVector_[fuelModelNumber].fuelLoadOneHour_ = 0;
    FuelModelVector_[fuelModelNumber].fuelLoadTenHour_ = 0;
    FuelModelVector_[fuelModelNumber].fuelLoadHundredHour_ = 0;
    FuelModelVector_[fuelModelNumber].fuelLoadLiveHerbaceous_ = 0;
    FuelModelVector_[fuelModelNumber].fuelLoadLiveWoody_ = 0;
    FuelModelVector_[fuelModelNumber].savrOneHour_ = 0;
    FuelModelVector_[fuelModelNumber].savrLiveHerbaceous_ = 0;
    FuelModelVector_[fuelModelNumber].savrLiveWoody_ = 0;
    FuelModelVector_[fuelModelNumber].isReserved_ = false;
    FuelModelVector_[fuelModelNumber].isDefined_ = false;
}

void FuelModels::initializeAllFuelModelRecords()
{
    for (int i = 0; i < FuelConstants::MaxFuelModels; i++)
    {
        initializeSingleFuelModelRecord(i);
    }
}

void FuelModels::setFuelModelRecord(int fuelModelNumber, std::string code, std::string name,
    double fuelBedDepth, double moistureOfExtinctionDead, double heatOfCombustionDead, double heatOfCombustionLive,
    double fuelLoadOneHour, double fuelLoadTenHour, double fuelLoadHundredHour, double fuelLoadliveHerbaceous,
    double fuelLoadliveWoody, double savrOneHour, double savrLiveHerbaceous, double savrLiveWoody,
    bool isDynamic, bool isReserved)
{
    FuelModelVector_[fuelModelNumber].fuelModelNumber_ = fuelModelNumber;
    FuelModelVector_[fuelModelNumber].code_ = code;
    FuelModelVector_[fuelModelNumber].name_ = name;
    FuelModelVector_[fuelModelNumber].fuelbedDepth_ = fuelBedDepth;
    FuelModelVector_[fuelModelNumber].moistureOfExtinctionDead_ = moistureOfExtinctionDead;
    FuelModelVector_[fuelModelNumber].heatOfCombustionDead_ = heatOfCombustionDead;
    FuelModelVector_[fuelModelNumber].heatOfCombustionLive_ = heatOfCombustionLive;
    FuelModelVector_[fuelModelNumber].fuelLoadOneHour_ = fuelLoadOneHour;
    FuelModelVector_[fuelModelNumber].fuelLoadTenHour_ = fuelLoadTenHour;
    FuelModelVector_[fuelModelNumber].fuelLoadHundredHour_ = fuelLoadHundredHour;
    FuelModelVector_[fuelModelNumber].fuelLoadLiveHerbaceous_ = fuelLoadliveHerbaceous;
    FuelModelVector_[fuelModelNumber].fuelLoadLiveWoody_ = fuelLoadliveWoody;
    FuelModelVector_[fuelModelNumber].savrOneHour_ = savrOneHour;
    FuelModelVector_[fuelModelNumber].savrLiveHerbaceous_ = savrLiveHerbaceous;
    FuelModelVector_[fuelModelNumber].savrLiveWoody_ = savrLiveWoody;
    FuelModelVector_[fuelModelNumber].isDynamic_ = isDynamic;
    FuelModelVector_[fuelModelNumber].isReserved_ = isReserved;
    FuelModelVector_[fuelModelNumber].isDefined_ = true;
}

// PopulateFuelModels() fills FuelModelArray[] with the standard fuel model parameters
// as well as earmarking which models are available for use as custom models
void FuelModels::populateFuelModels()
{
    // See Standard Fire Behavior Fuel Models: A Comprehensive Set for Use with Rothermel’s
    // Surface Fire Spread Model by Joe H.Scott and Robert E.Burgan, 2005
    // https://www.fs.fed.us/rm/pubs/rmrs_gtr153.pdf

    // Index 0 is not used
    setFuelModelRecord(0, "NO_CODE", "NO_NAME", 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, false, false);
    FuelModelVector_[0].isDefined_ = false;
    /*
    fuelModelNumber, code, name
    fuelBedDepth, moistureOfExtinctionDeadFuel, heatOfCombustionDeadFuel, heatOfCombustionLiveFuel,
    fuelLoad1Hour, fuelLoad10Hour, fuelLoad100Hour, fuelLoadLiveHerb, fuelLoadLiveWood,
    savr1HourFuel, savrLiveHerb, savrLiveWood,
    isDynamic, isReserved
    - WMC 10/2015
    */

    // Code FMx: Original 13 Fuel Models
    setFuelModelRecord(1, "FM1", "Short grass [1]",
        1.0, 0.12, 8000, 8000,
        0.034, 0, 0, 0, 0,
        3500, 1500, 1500,
        false, true);
    setFuelModelRecord(2, "FM2", "Timber grass and understory [2]",
        1.0, 0.15, 8000, 8000,
        0.092, 0.046, 0.023, 0.023, 0,
        3000, 1500, 1500,
        false, true);
    setFuelModelRecord(3, "FM3", "Tall grass [3]",
        2.5, 0.25, 8000, 8000,
        0.138, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(4, "FM4", "Chaparral [4]",
        6.0, 0.2, 8000, 8000,
        0.230, 0.184, 0.092, 0, 0.230,
        2000, 1500, 1500,
        false, true);
    setFuelModelRecord(5, "FM5", "Brush [5]",
        2.0, 0.20, 8000, 8000,
        0.046, 0.023, 0, 0, 0.092,
        2000, 1500, 1500,
        false, true);
    setFuelModelRecord(6, "FM6", "Dormant brush, hardwood slash [6]",
        2.5, 0.25, 8000, 8000,
        0.069, 0.115, 0.092, 0, 0,
        1750, 1500, 1500,
        false, true);
    setFuelModelRecord(7, "FM7", "Southern rough [7]",
        2.5, 0.40, 8000, 8000,
        0.052, 0.086, 0.069, 0, 0.017,
        1750, 1500, 1500,
        false, true);
    setFuelModelRecord(8, "FM8", "Short needle litter [8]",
        0.2, 0.3, 8000, 8000,
        0.069, 0.046, 0.115, 0, 0,
        2000, 1500, 1500,
        false, true);
    setFuelModelRecord(9, "FM9", "Long needle or hardwood litter [9]",
        0.2, 0.25, 8000, 8000,
        0.134, 0.019, 0.007, 0, 0,
        2500, 1500, 1500,
        false, true);
    setFuelModelRecord(10, "FM10", "Timber litter & understory [10]",
        1.0, 0.25, 8000, 8000,
        0.138, 0.092, 0.230, 0, 0.092,
        2000, 1500, 1500,
        false, true);
    setFuelModelRecord(11, "FM11", "Light logging slash [11]",
        1.0, 0.15, 8000, 8000,
        0.069, 0.207, 0.253, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(12, "FM12", "Medium logging slash [12]",
        2.3, 0.20, 8000, 8000,
        0.184, 0.644, 0.759, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(13, "FM13", "Heavy logging slash [13]",
        3.0, 0.25, 8000, 8000,
        0.322, 1.058, 1.288, 0, 0,
        1500, 1500, 1500,
        false, true);

    // 14-89 Available for custom models

    // Code NBx: Non-burnable
    // 90 Available for custom NB model  
    setFuelModelRecord(91, "NB1", "Urban, developed [91]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(92, "NB2", "Snow, ice [92]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(93, "NB3", "Agricultural [93]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);

    // Indices 94-95 Reserved for future standard non-burnable models
    for (int i = 94; i <= 95; i++)
    {
        markAsReservedModel(i);
    }

    setFuelModelRecord(94, "NB4", "Future standard non-burnable [94]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(95, "NB5", "Future standard non-burnable [95]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);

    // Indices 96-97 Available for custom NB model

    setFuelModelRecord(98, "NB8", "Open water [98]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);
    setFuelModelRecord(99, "NB9", "Bare ground [99]",
        1.0, 0.10, 8000, 8000,
        0, 0, 0, 0, 0,
        1500, 1500, 1500,
        false, true);

    // Code GRx: Grass
    // Index 100 Available for custom GR model
    double f = 2000.0 / 43560.0;
    setFuelModelRecord(101, "GR1", "Short, sparse, dry climate grass (D)",
        0.4, 0.15, 8000, 8000,
        0.10*f, 0, 0, 0.30*f, 0,
        2200, 2000, 1500,
        true, true);
    setFuelModelRecord(102, "GR2", "Low load, dry climate grass (D)",
        1.0, 0.15, 8000, 8000,
        0.10*f, 0, 0, 1.0*f, 0,
        2000, 1800, 1500,
        true, true);
    setFuelModelRecord(103, "GR3",
        "Low load, very coarse, humid climate grass (D)",
        2.0, 0.30, 8000, 8000,
        0.10*f, 0.40*f, 0, 1.50*f, 0,
        1500, 1300, 1500,
        true, true);
    setFuelModelRecord(104, "GR4", "Moderate load, dry climate grass (D)",
        2.0, 0.15, 8000, 8000,
        0.25*f, 0, 0, 1.9*f, 0,
        2000, 1800, 1500,
        true, true);
    setFuelModelRecord(105, "GR5", "Low load, humid climate grass (D)",
        1.5, 0.40, 8000, 8000,
        0.40*f, 0.0, 0.0, 2.50*f, 0.0,
        1800, 1600, 1500,
        true, true);
    setFuelModelRecord(106, "GR6",
        "Moderate load, humid climate grass (D)",
        1.5, 0.40, 9000, 9000,
        0.10*f, 0, 0, 3.4*f, 0,
        2200, 2000, 1500,
        true, true);
    setFuelModelRecord(107, "GR7",
        "High load, dry climate grass (D)",
        3.0, 0.15, 8000, 8000,
        1.0*f, 0, 0, 5.4*f, 0,
        2000, 1800, 1500,
        true, true);
    setFuelModelRecord(108, "GR8",
        "High load, very coarse, humid climate grass (D)",
        4.0, 0.30, 8000, 8000,
        0.5*f, 1.0*f, 0, 7.3*f, 0,
        1500, 1300, 1500,
        true, true);
    setFuelModelRecord(109, "GR9",
        "Very high load, humid climate grass (D)",
        5.0, 0.40, 8000, 8000,
        1.0*f, 1.0*f, 0, 9.0*f, 0,
        1800, 1600, 1500,
        true, true);

    setFuelModelRecord(110, "V-Hb",
        "Short Gass, < 0.5 m (Dynamic)",
        0.35, 24, 19000, 19000,
        0.3*f, 0, 0, 1.2*f, 0,
        6000, 6000, 6000,
        true, true);
    setFuelModelRecord(111, "V-Ha",
        "Tall Grass, > 0.5 m (Dynamic)",
        0.6, 24, 19000, 19000,
        0.5*f, 0.1*f, 0, 2.5*f, 0.3*f,
        4000, 6000, 4000,
        true, true);

    // 112 reserved for future standard grass models
    markAsReservedModel(112);

    // 113-119 are available for custom grass models

    // Code GSx: Grass and shrub
    // 120 available for custom grass and shrub model
    setFuelModelRecord(121, "GS1",
        "Low load, dry climate grass-shrub (D)",
        0.9, 0.15, 8000, 8000,
        0.2*f, 0, 0, 0.5*f, 0.65*f,
        2000, 1800, 1800,
        true, true);
    setFuelModelRecord(122, "GS2",
        "Moderate load, dry climate grass-shrub (D)",
        1.5, 0.15, 8000, 8000,
        0.5*f, 0.5*f, 0, 0.6*f, 1.0*f,
        2000, 1800, 1800,
        true, true);
    setFuelModelRecord(123, "GS3",
        "Moderate load, humid climate grass-shrub (D)",
        1.8, 0.40, 8000, 8000,
        0.3*f, 0.25*f, 0, 1.45*f, 1.25*f,
        1800, 1600, 1600,
        true, true);
    setFuelModelRecord(124, "GS4",
        "High load, humid climate grass-shrub (D)",
        2.1, 0.40, 8000, 8000,
        1.9*f, 0.3*f, 0.1*f, 3.4*f, 7.1*f,
        1800, 1600, 1600,
        true, true);
    // 125-130 reserved for future standard grass and shrub models
    for (int i = 125; i <= 130; i++)
    {
        markAsReservedModel(i);
    }
    // 131-139 available for custom grass and shrub models

    // Shrub
    // 140 available for custom shrub model
    setFuelModelRecord(141, "SH1",
        "Low load, dry climate shrub (D)",
        1.0, 0.15, 8000, 8000,
        0.25*f, 0.25*f, 0, 0.15*f, 1.3*f,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(142, "SH2",
        "Moderate load, dry climate shrub (S)",
        1.0, 0.15, 8000, 8000,
        1.35*f, 2.4*f, 0.75*f, 0, 3.85*f,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(143, "SH3",
        "Moderate load, humid climate shrub (S)",
        2.4, 0.40, 8000., 8000.,
        0.45*f, 3.0*f, 0, 0, 6.2*f,
        1600, 1800, 1400,
        true, true);
    setFuelModelRecord(144, "SH4",
        "Low load, humid climate timber-shrub (S)",
        3.0, 0.30, 8000, 8000,
        0.85*f, 1.15*f, 0.2*f, 0, 2.55*f,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(145, "SH5",
        "High load, dry climate shrub (S)",
        6.0, 0.15, 8000, 8000,
        3.6*f, 2.1*f, 0, 0, 2.9*f,
        750, 1800, 1600,
        true, true);
    setFuelModelRecord(146, "SH6",
        "Low load, humid climate shrub (S)",
        2.0, 0.30, 8000, 8000,
        2.9*f, 1.45*f, 0, 0, 1.4*f,
        750, 1800, 1600,
        true, true);
    setFuelModelRecord(147, "SH7",
        "Very high load, dry climate shrub (S)",
        6.0, 0.15, 8000, 8000,
        3.5*f, 5.3*f, 2.2*f, 0, 3.4*f,
        750, 1800, 1600,
        true, true);
    setFuelModelRecord(148, "SH8",
        "High load, humid climate shrub (S)",
        3.0, 0.40, 8000, 8000,
        2.05*f, 3.4*f, 0.85*f, 0, 4.35*f,
        750, 1800, 1600,
        true, true);
    setFuelModelRecord(149, "SH9",
        "Very high load, humid climate shrub (D)",
        4.4, 0.40, 8000, 8000,
        4.5*f, 2.45*f, 0, 1.55*f, 7.0*f,
        750, 1800, 1500,
        true, true);

    // Standard Shrub Models
    setFuelModelRecord(150, "SCAL17",
        "Chamise with Moderate Load Grass, 4 feet (Static)",
        4, 20, 8000, 8000,
        1.3*f, 1.0*f, 1.0*f, 2.0*f, 2.0*f,
        640, 2200, 640,
        false, true);
    setFuelModelRecord(151, "SCAL15",
        "Chamise with Low Load Grass, 3 feet (Static)",
        3, 13, 10000, 10000,
        2.0*f, 3.0*f, 1.0*f, 0.5*f, 2.0*f,
        640, 2200, 640,
        false, true);
    setFuelModelRecord(152, "SCAL16",
        "North Slope Ceanothus with Moderate Load Grass (Static)",
        6, 15, 8000, 8000,
        2.2*f, 4.8*f, 1.8*f, 3.0*f, 2.8*f,
        500, 1500, 500,
        false, true);
    setFuelModelRecord(153, "SCAL14",
        "Manzanita/Scrub Oak with Low Load Grass (Static)",
        3, 15, 9211, 9211,
        3.0*f, 4.5*f, 1.1*f, 1.4*f, 5.0*f,
        350, 1500, 250,
        false, true);
    setFuelModelRecord(154, "SCAL18",
        "Coastal Sage/Buckwheat Scrub with Low Load Grass (Static)",
        3, 25, 9200, 9200,
        5.5*f, 0.8*f, 0.1*f, 0.75*f, 2.5*f,
        640, 1500, 640,
        false, true);

    setFuelModelRecord(155, "V-MH",
        "Short Green Shrub < 1 m With Grass, Discontinuous (< 1 m) often discontinuous and with grass (Dynamic)",
        0.55, 25, 19500, 19500,
        1.0*f, 1.0*f, 0, 1.5*f, 5.5*f,
        4500, 8500, 4000,
        true, true);
    setFuelModelRecord(156, "V-MMb",
        "Short Shrub < 1 m, Low Dead Fraction and/or Thick Foliage (Static)",
        0.9, 20, 20500, 20500,
        4.0*f, 0.5*f, 0, 0, 7.0*f,
        3000, 3000, 3000,
        false, true);
    setFuelModelRecord(157, "V-MAb",
        "Short Shrub < 1 m, High Dead Fraction and/or Thin Fuel (Static)",
        0.5, 35, 21000, 21000,
        6.0*f, 0.5*f, 0, 0, 7.5*f,
        4500, 4500, 4500,
        false, true);
    setFuelModelRecord(158, "V-MMa",
        "Tall Shrub > 1 m, Low Dead Fraction and/or Thick Foliage (Static)",
        1.7, 24, 20500, 20500,
        6.0*f, 4.0*f, 0, 0, 13.0*f,
        2500, 3000, 3000,
        false, true);
    setFuelModelRecord(159, "V-MAa",
        "Tall Shrub > 1 m, High Dead Fraction and/or Thin Fuel (Static)",
        1.05, 35, 21000, 21000,
        9.5*f, 2.5*f, 0, 0, 14.5*f,
        3500, 4000, 4000,
        false, true);

    // Timber and understory
    // 160 available for custom timber and understory model
    setFuelModelRecord(161, "TU1",
        "Light load, dry climate timber-grass-shrub (D)",
        0.6, 0.20, 8000, 8000,
        0.2*f, 0.9*f, 1.5*f, 0.2*f, 0.9*f,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(162, "TU2",
        "Moderate load, humid climate timber-shrub (S)",
        1.0, 0.30, 8000, 8000,
        0.95*f, 1.8*f, 1.25*f, 0, 0.2*f,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(163, "TU3",
        "Moderate load, humid climate timber-grass-shrub (D)",
        1.3, 0.30, 8000, 8000,
        1.1*f, 0.15*f, 0.25*f, 0.65*f, 1.1*f,
        1800, 1600, 1400,
        true, true);
    setFuelModelRecord(164, "TU4",
        "Dwarf conifer understory (S)",
        0.5, 0.12, 8000, 8000,
        4.5*f, 0, 0, 0, 2.0*f,
        2300, 1800, 2000,
        true, true);
    setFuelModelRecord(165, "TU5",
        "Very high load, dry climate timber-shrub (S)",
        1.0, 0.25, 8000, 8000,
        4.0*f, 4.0*f, 3.0*f, 0, 3.0*f,
        1500, 1800, 750,
        true, true);


    setFuelModelRecord(166, "M-EUCd",
        "Discontinuous Litter Eucalyptus Plantation, With or Without Shrub Understory (Static)",
        0.4, 26, 21000, 20500,
        1.37*f, 2.89*f, 1.59*f, 0, 1.84*f,
        4500, 4200, 5000,
        false, true);
    setFuelModelRecord(167, "M-H",
        "Deciduous or Conifer Litter, Shrub and Herb Understory",
        0.1, 30, 20500, 20500,
        2.71*f, 1.0*f, 0, 0.66*f, 0.1*f,
        5500, 8000, 4500,
        true, true);
    setFuelModelRecord(168, "M-F", "Deciduous or Conifer Litter, Shrub and Fern Understory (Dynamic)",
        0.3, 35, 19500, 19500,
        4.5*f, 1.5*f, 0.5*f, 2.35*f, 0.48*f,
        6000, 8000, 4500,
        true, true);
    setFuelModelRecord(169, "M-CAD",
        "Deciduous Litter, Shrub Understory (Static)",
        0.63, 30, 20000, 20000,
        4.54*f, 1.87*f, 0.61*f, 0, 9.08*f,
        6000, 4921, 5000,
        false, true);
    setFuelModelRecord(170, "M-ESC",
        "Sclerophyll Broadleaf Litter, Shrub Understory (Static)",
        0.5, 27, 20500, 20500,
        5.65*f, 1.5*f, 0.48*f, 0, 7.89*f,
        5000, 4921, 5500,
        false, true);
    setFuelModelRecord(171, "M-PIN",
        "Medium-Long Needle Pine Litter, Shrub Understory (Static)",
        0.5, 40, 20500, 21500,
        7.21*f, 3.0*f, 0, 0, 6.89*f,
        5500, 5500, 6000,
        false, true);
    setFuelModelRecord(172, "M-EUC",
        "Eucalyptus Litter, Shrub Understory (Static)",
        0.64, 32, 21000, 21000,
        8.37*f, 3.81*f, 0, 0, 4.51*f,
        4700, 4200, 5000,
        false, true);

    // 173-179 available for custom timber and understory models

    // Timber and litter
    // 180 available for custom timber and litter models
    setFuelModelRecord(181, "TL1",
        "Low load, compact conifer litter (S)",
        0.2, 0.30, 8000, 8000,
        1.0*f, 2.2*f, 3.6*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(182, "TL2",
        "Low load broadleaf litter (S)",
        0.2, 0.25, 8000, 8000,
        1.4*f, 2.3*f, 2.2*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(183, "TL3",
        "Moderate load conifer litter (S)",
        0.3, 0.20, 8000, 8000,
        0.5*f, 2.2*f, 2.8*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(184, "TL4",
        "Small downed logs (S)",
        0.4, 0.25, 8000, 8000,
        0.5*f, 1.5*f, 4.2*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(185, "TL5",
        "High load conifer litter (S)",
        0.6, 0.25, 8000, 8000,
        1.15*f, 2.5*f, 4.4*f, 0, 0,
        2000, 1800, 160,
        true, true);
    setFuelModelRecord(186, "TL6",
        "High load broadleaf litter (S)",
        0.3, 0.25, 8000, 8000,
        2.4*f, 1.2*f, 1.2*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(187, "TL7",
        "Large downed logs (S)",
        0.4, 0.25, 8000, 8000,
        0.3*f, 1.4*f, 8.1*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(188, "TL8",
        "Long-needle litter (S)",
        0.3, 0.35, 8000, 8000,
        5.8*f, 1.4*f, 1.1*f, 0, 0,
        1800, 1800, 1600,
        true, true);
    setFuelModelRecord(189, "TL9",
        "Very high load broadleaf litter (S)",
        0.6, 0.35, 8000, 8000,
        6.65*f, 3.30*f, 4.15*f, 0, 0,
        1800, 1800, 1600,
        true, true);

    setFuelModelRecord(190, "F-RAC",
        "Very Compact Litter, Short Needle Conifers (Static)",
        0.05, 28, 20500, 20500,
        3.75*f, 2.0*f, 1.0*f, 0, 1.18*f,
        6500, 4921, 4500,
        false, true);
    setFuelModelRecord(191, "F-FOL",
        "Compact Litter, Deciduous or Evergreen Foliage (Static)",
        0.15, 25, 20500, 20500,
        2.67*f, 1.27*f, 0.69*f, 0, 1.16*f,
        4500, 5500, 5000,
        false, true);
    setFuelModelRecord(192, "F-PIN",
        "Litter from Medium-Long Needle Pine Trees (Static)",
        0.1, 45, 20500, 21500,
        6.5*f, 1.5*f, 0, 0, 0,
        5500, 5500, 5500,
        false, true);
    setFuelModelRecord(193, "F-EUC",
        "Pure Eucalyptus Litter, No Understory (Static)",
        0.32, 26, 21000, 20500,
        4.63*f, 2.96*f, 1.27*f, 0, 1.12*f,
        4200, 4200, 5000,
        false, true);

    // 194-199 available for custom timber and litter models

    // Slash and blowdown
    // 200 available for custom slash and blowdown model
    setFuelModelRecord(201, "SB1",
        "Low load activity fuel (S)",
        1.0, 0.25, 8000, 8000,
        1.5*f, 3.0*f, 11.0*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(202, "SB2",
        "Moderate load activity or low load blowdown (S)",
        1.0, 0.25, 8000, 8000,
        4.5*f, 4.25*f, 4.0*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(203, "SB3",
        "High load activity fuel or moderate load blowdown (S)",
        1.2, 0.25, 8000, 8000,
        5.5*f, 2.75*f, 3.0*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    setFuelModelRecord(204, "SB4",
        "High load blowdown (S)",
        2.7, 0.25, 8000, 8000,
        5.25*f, 3.5*f, 5.25*f, 0, 0,
        2000, 1800, 1600,
        true, true);
    // 205-210 reserved for future slash and blowdown models
    for (int i = 205; i <= 210; i++)
    {
        markAsReservedModel(i);
    }
    // 211-219 available for custom  slash and blowdown models

    // 220 - 256 Available for custom models
}

// SetCustomFuelModel() is used by client code to define custom fuel types
// The function can fail in the case of trying to set a record whose isReserve field is set to 1.
// The return value is TRUE if successful, and FALSE in case of failure
bool FuelModels::setCustomFuelModel(int fuelModelNumber, std::string code, std::string name,
    double fuelBedDepth, LengthUnits::LengthUnitsEnum lengthUnits, double moistureOfExtinctionDead,
    FractionUnits::FractionUnitsEnum moistureUnits, double heatOfCombustionDead, double heatOfCombustionLive,
    HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits,
    double fuelLoadOneHour, double fuelLoadTenHour, double fuelLoadHundredHour, double fuelLoadLiveHerbaceous,
    double fuelLoadLiveWoody, LoadingUnits::LoadingUnitsEnum loadingUnits, double savrOneHour, double savrLiveHerbaceous,
    double savrLiveWoody, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits, bool isDynamic)
{
    bool successStatus = false;
    
    fuelBedDepth = LengthUnits::toBaseUnits(fuelBedDepth, lengthUnits);

    moistureOfExtinctionDead = FractionUnits::toBaseUnits(moistureOfExtinctionDead, moistureUnits);

    if (loadingUnits != LoadingUnits::PoundsPerSquareFoot)
    {
        fuelLoadOneHour = LoadingUnits::toBaseUnits(fuelLoadOneHour, loadingUnits);
        fuelLoadTenHour = LoadingUnits::toBaseUnits(fuelLoadTenHour, loadingUnits);
        fuelLoadHundredHour = LoadingUnits::toBaseUnits(fuelLoadHundredHour, loadingUnits);
        fuelLoadLiveHerbaceous = LoadingUnits::toBaseUnits(fuelLoadLiveHerbaceous, loadingUnits);
        fuelLoadLiveWoody = LoadingUnits::toBaseUnits(fuelLoadLiveWoody, loadingUnits);
    }

    code = code.substr(0, 3);

    if (savrUnits != SurfaceAreaToVolumeUnits::SquareFeetOverCubicFeet)
    {
        savrOneHour = SurfaceAreaToVolumeUnits::toBaseUnits(savrOneHour, savrUnits);
        savrLiveHerbaceous = SurfaceAreaToVolumeUnits::toBaseUnits(savrLiveHerbaceous, savrUnits);
        savrLiveWoody = SurfaceAreaToVolumeUnits::toBaseUnits(savrLiveWoody, savrUnits);
    }

    if (FuelModelVector_[fuelModelNumber].isReserved_ == false)
    {
        setFuelModelRecord(fuelModelNumber, code, name,
            fuelBedDepth, moistureOfExtinctionDead, heatOfCombustionDead, heatOfCombustionLive,
            fuelLoadOneHour, fuelLoadTenHour, fuelLoadHundredHour, fuelLoadLiveHerbaceous,
            fuelLoadLiveWoody, savrOneHour, savrLiveHerbaceous, savrLiveWoody, isDynamic,
            false);
        successStatus = true;
    }
    return successStatus;
}

bool FuelModels::clearCustomFuelModel(int fuelModelNumber)
{
    bool successStatus = false;

    if (FuelModelVector_[fuelModelNumber].isReserved_)
    {
        successStatus = false;
    }
    else
    {
        initializeSingleFuelModelRecord(fuelModelNumber);
        successStatus = true;
    }
    return successStatus;
}

void FuelModels::markAsReservedModel(int fuelModelNumber)
{
    FuelModelVector_[fuelModelNumber].isReserved_ = true;
}

double FuelModels::getFuelbedDepth(int fuelModelNumber, LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return LengthUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].fuelbedDepth_, lengthUnits);
}

std::string FuelModels::getFuelCode(int fuelModelNumber) const
{
    return FuelModelVector_[fuelModelNumber].code_;
}

std::string FuelModels::getFuelName(int fuelModelNumber) const
{
    return FuelModelVector_[fuelModelNumber].name_;
}

double FuelModels::getMoistureOfExtinctionDead(int fuelModelNumber, FractionUnits::FractionUnitsEnum moistureUnits) const
{
    return FractionUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].moistureOfExtinctionDead_, moistureUnits);
}

double FuelModels::getHeatOfCombustionDead(int fuelModelNumber, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return HeatOfCombustionUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].heatOfCombustionDead_, heatOfCombustionUnits);
}

double FuelModels::getHeatOfCombustionLive(int fuelModelNumber, HeatOfCombustionUnits::HeatOfCombustionUnitsEnum heatOfCombustionUnits) const
{
    return HeatOfCombustionUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].heatOfCombustionLive_, heatOfCombustionUnits);
}

double FuelModels::getFuelLoadOneHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].fuelLoadOneHour_, loadingUnits);
}

double FuelModels::getFuelLoadTenHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].fuelLoadTenHour_, loadingUnits);
}

double FuelModels::getFuelLoadHundredHour(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].fuelLoadHundredHour_, loadingUnits);
}

double FuelModels::getFuelLoadLiveHerbaceous(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].fuelLoadLiveHerbaceous_, loadingUnits);
}

double FuelModels::getFuelLoadLiveWoody(int fuelModelNumber, LoadingUnits::LoadingUnitsEnum loadingUnits) const
{
    return LoadingUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].fuelLoadLiveWoody_, loadingUnits);
}

double FuelModels::getSavrOneHour(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].savrOneHour_, savrUnits);
}

double FuelModels::getSavrLiveHerbaceous(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].savrLiveHerbaceous_, savrUnits);
}

double FuelModels::getSavrLiveWoody(int fuelModelNumber, SurfaceAreaToVolumeUnits::SurfaceAreaToVolumeUnitsEnum savrUnits) const
{
    return SurfaceAreaToVolumeUnits::fromBaseUnits(FuelModelVector_[fuelModelNumber].savrLiveWoody_, savrUnits);
}

bool FuelModels::getIsDynamic(int fuelModelNumber) const
{
    if(fuelModelNumber <= 0 || fuelModelNumber > 256)
    {
        return false;
    }
    else
    {
        return FuelModelVector_[fuelModelNumber].isDynamic_;
    }
}

bool FuelModels::isFuelModelDefined(int fuelModelNumber) const
{
    if (fuelModelNumber <= 0 || fuelModelNumber > 256)
    {
        return false;
    }
    else
    {
        return FuelModelVector_[fuelModelNumber].isDefined_;
    }
}

bool FuelModels::isFuelModelReserved(int fuelModelNumber) const
{
    if(fuelModelNumber <= 0 || fuelModelNumber > 256)
    {
        return false;
    }
    else
    {
        return  FuelModelVector_[fuelModelNumber].isReserved_;
    }
}

bool FuelModels::isAllFuelLoadZero(int fuelModelNumber) const
{
    // if  all loads are zero, skip calculations
    bool isZeroLoad = !(getFuelLoadOneHour(fuelModelNumber, LoadingUnits::PoundsPerSquareFoot)
        || getFuelLoadTenHour(fuelModelNumber, LoadingUnits::PoundsPerSquareFoot)
        || getFuelLoadHundredHour(fuelModelNumber, LoadingUnits::PoundsPerSquareFoot)
        || getFuelLoadLiveHerbaceous(fuelModelNumber, LoadingUnits::PoundsPerSquareFoot)
        || getFuelLoadLiveWoody(fuelModelNumber, LoadingUnits::PoundsPerSquareFoot));

    return isZeroLoad;
}
