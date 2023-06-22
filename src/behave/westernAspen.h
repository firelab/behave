/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the Western Aspen special case fuel model
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

#ifndef WESTERNASPEN_H
#define WESTERNASPEN_H

class WesternAspen
{
public:
    WesternAspen();
    ~WesternAspen();

    void initializeMembers();
    double getAspenDBH() const;
    double getAspenMortality() const;

    // The following getter methods are used to populate FuelModel data fields 
    double getAspenFuelBedDepth(int typeIndex) const;
    double getAspenHeatOfCombustionDead() const;
    double getAspenHeatOfCombustionLive() const;
    double getAspenMoistureOfExtinctionDead() const;

    double getAspenLoadDeadOneHour() const;
    double getAspenLoadDeadTenHour() const;
    double getAspenLoadLiveHerbaceous() const;
    double getAspenLoadLiveWoody() const;

    double getAspenSavrDeadOneHour() const;
    double getAspenSavrDeadTenHour() const;
    double getAspenSavrLiveHerbaceous() const;
    double getAspenSavrLiveWoody() const;

    double calculateAspenLoadDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel);
    double calculateAspenLoadDeadTenHour(int aspenFuelModelNumber);
    double calculateAspenLoadLiveHerbaceous(int aspenFuelModelNumber, double aspenCuringLevel);
    double calculateAspenLoadLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel);
    double calculateAspenSavrDeadOneHour(int aspenFuelModelNumber, double aspenCuringLevel);
    double calculateAspenSavrDeadTenHour();
    double calculateAspenSavrLiveHerbaceous();
    double calculateAspenSavrLiveWoody(int aspenFuelModelNumber, double aspenCuringLevel);

    // Mortality must be calculated AFTER spread rate
    double calculateAspenMortality(int severity, double flameLength, double DBH);

protected:
    double aspenInterpolate(double curing, double* valueArray);
   
    // Member variables
    double DBH_;
    double mortality_;
    double aspenDeadOneHour_;
    double aspenDeadTenHour_;
    double aspenLiveHerbaceous_;
    double aspenLiveWoody_;
    double aspenSavrDeadOneHour_;
    double aspenSavrDeadTenHour_;
    double aspenSavrLiveHerbaceous_;
    double aspenSavrLiveWoody_;
};

#endif // WESTERNASPEN_H
