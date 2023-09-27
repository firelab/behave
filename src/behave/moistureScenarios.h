/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for storing data and methods for Behave's moisture scenarios
* Author:   William Chatham <wchatham@fs.fed.us>
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

#ifndef MOISTURE_SCENARIOS_H
#define MOISTURE_SCENARIOS_H

#include <string>
#include <vector>

#include "behaveUnits.h"

class MoistureScenarios
{
public:
    MoistureScenarios();
    ~MoistureScenarios();
    MoistureScenarios& operator=(const MoistureScenarios& rhs);
    MoistureScenarios(const MoistureScenarios& rhs);
    
    // Getters by scenario name

    // Returns the number of mositure scenarios that are currently defined
    int getNumberOfMoistureScenarios();
    // If a moisture scenerio having "name" is found, returns that scenario's vector index 
    // Otherwise returns -1
    int getMoistureScenarioIndexByName(std::string name);
    bool getIsMoistureScenarioDefinedByName(std::string name);
    std::string getMoistureScenarioDescriptionByName(std::string name);
    double getMoistureScenarioOneHourByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioTenHourByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioHundredHourByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioLiveHerbaceousByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioLiveWoodyByName(std::string name, FractionUnits::FractionUnitsEnum moistureUnits);
  
    // Getters by vector index
    bool getIsMoistureScenarioDefinedByIndex(int index);
    std::string getMoistureScenarioNameByIndex(int index);
    std::string getMoistureScenarioDescriptionByIndex(int index);
    double getMoistureScenarioOneHourByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioTenHourByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioHundredHourByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioLiveHerbaceousByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits);
    double getMoistureScenarioLiveWoodyByIndex(int index, FractionUnits::FractionUnitsEnum moistureUnits);
   
protected:
    void memberwiseCopyAssignment(const MoistureScenarios& rhs);
    void setMoistureScenarioRecord(std::string name, std::string description,
        double moistureOneHour, double moistureTenHour, double moistureHundredHour,
        double moistureLiveHerbaceous, double moistureLiveWoody);
    void populateMoistureScenarios();

    struct MoistureScenarioRecord
    {
        std::string name_;                  // Moisture scenario name code, usually letter,number,letter,number (e.g., "D1L1")
        std::string description_;           // Textual description of scenario (e.g., D1L1 - Very low dead, fully cured herb)
        double moistureOneHour_;            // Dead 1 hour moisture (fraction)
        double moistureTenHour_;            // Dead 10 hour moisture (fraction)
        double moistureHundredHour_;        // Dead 100 hour fmoisture (fraction)
        double moistureLiveHerbaceous_;     // Live herb moisture (fraction)
        double moistureLiveWoody_;          // Live wood moisture (fraction)
    };

    std::vector<MoistureScenarioRecord> moistureScenarioVector_;
};

#endif //MOISTURE_SCENARIOS_H
