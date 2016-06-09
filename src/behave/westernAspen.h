/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the Western Aspen special case fuel model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some portions of code in this file are, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
*
******************************************************************************
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
*****************************************************************************/

#ifndef WESTERNASPEN_HEADER
#define WESTERNASPEN_HEADER

class WesternAspen
{
public:
    WesternAspen();
    WesternAspen(const WesternAspen &rhs);
    WesternAspen& operator= (const WesternAspen& rhs);
    ~WesternAspen();

    void initializeMembers();
    double getAspenMortality() const;

    // The following getter methods are used to populate FuelModel data fields 
    double getAspenFuelBedDepth(int typeIndex);
    double getAspenHeatOfCombustionDead();
    double getAspenHeatOfCombustionLive();
    double getAspenMoistureOfExtinctionDead();
    double getAspenLoadDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenLoadDeadTenHour(int aspenFuelModelNumber);
    double getAspenLoadLiveHerbaceous(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenLoadLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenSavrDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel);
    double getAspenSavrDeadTenHour();
    double getAspenSavrLiveHerbaceous();
    double getAspenSavrLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel);
    
    // Mortality must be calculated AFTER spread rate
    double calculateAspenMortality(int severity, double flameLength, double DBH);

private:
    double aspenInterpolate(double curing, double *valueArray);

    // Member variables
    double DBH_;
    double mortality_;
};

#endif //WESTERNASPEN_HEADER
