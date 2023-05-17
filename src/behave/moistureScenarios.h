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
    int getMoistureScenarioIndexByName(const std::string name);
    bool getIsMoistureScenarioDefinedByName(const std::string name);
    std::string getMoistureScenarioDecriptionByName(const std::string name);
    double getMoistureScenarioOneHourByName(const std::string name);
    double getMoistureScenarioTenHourByName(const std::string name);
    double getMoistureScenarioHundredHourByName(const std::string name);
    double getMoistureScenarioLiveHerbaceousByName(const std::string name);
    double getMoistureScenarioLiveWoodyByName(const std::string name);
  
    // Getters by vector index
    bool getIsMoistureScenarioDefinedByIndex(const int index);
    std::string getMoistureScenarioNameByIndex(const int index);
    std::string getMoistureScenarioDecriptionByIndex(const int index);
    double getMoistureScenarioOneHourByIndex(const int index);
    double getMoistureScenarioTenHourByIndex(const int index);
    double getMoistureScenarioHundredHourByIndex(const int index);
    double getMoistureScenarioLiveHerbaceousByIndex(const int index);
    double getMoistureScenarioLiveWoodyByIndex(const int index);
   
private:
    void memberwiseCopyAssignment(const MoistureScenarios& rhs);
    void setMoistureScenarioRecord(const std::string name, const std::string description,
        const double moistureOneHour, const double moistureTenHour, const double moistureHundredHour,
        const double moistureLiveHerbaceous, double moistureLiveWoody);
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
