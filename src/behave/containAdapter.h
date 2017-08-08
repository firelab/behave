/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Interface for Contain to be used in Behave-like applications
*           used to tie together the classes used in a Contain simulation
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

#ifndef CONTAINADAPTER_H
#define CONTAINADAPTER_H

#include "Contain.h"
#include "ContainSim.h"
#include <string>

using namespace Sem;

class ContainAdapter
{
public:
    ContainAdapter();
    ~ContainAdapter();

    void addResource(ContainResource& resource);
    // Construct ContainResource into ContainForce
    void addResource(
        double arrival,
        double production,
        double duration = 480.,
        ContainFlank::ContainFlankEnum flank = Sem::ContainFlank::LeftFlank,
        std::string desc = "",
        double baseCost = 0.0,
        double hourCost = 0.0);
    int removeResourceAt(int index);
    int removeResourceWithThisDesc(std::string desc);
    int removeAllResourcesWithThisDesc(std::string desc);
    void removeAllResources();

    void setReportSize(double reportSize);
    void setReportRate(double reportRate);
    void setFireStartTime(int fireStartTime);
    void setLwRatio(double lwRatio);
    void setTactic(ContainTactic::ContainTacticEnum tactic);
    void setAttackDistance(double attackDistance);
    void setRetry(bool retry);
    void setMinSteps(int minSteps);
    void setMaxSteps(int maxSteps);
    void setMaxFireSize(int maxFireSize);
    void setMaxFireTime(int maxFireTime);

    void doContainRun();

    double getFinalCost() const;   // Final total cost of all resources used
    double getFinalFireLineLength() const;   // Final fire line at containment or escape (ch)
    double getPerimeterAtContainment() const;  // Final line plus fire perimeter at containment or escape (ch)
    double getFinalFireSize() const;   // Final fire size at containment or escape (ac)
    double getFinalContainmentArea() const;  // Final containment area at containment or escape (ac)
    double getFinalTimeSinceReport() const;   // Containment or escape time since report (min)   
    ContainStatus::ContainStatusEnum getContainmentStatus() const; // Status of fire at end of contain simultation

private:
    // Contain Inputs
    double reportSize_;
    double reportRate_;
    double diurnalROS_[24];
    int fireStartTime_;
    double lwRatio_;
    ContainForce force_;
    ContainTactic::ContainTacticEnum tactic_;
    double attackDistance_;
    bool retry_;
    int minSteps_;
    int maxSteps_;
    int maxFireSize_;
    int maxFireTime_;

    // Contain Outputs
    double finalCost_;
    double finalFireLineLength_;
    double perimeterAtContainment_;
    double finalFireSize_;
    double finalContainmentArea_;
    double finalTime_;
    ContainStatus::ContainStatusEnum containmentStatus_;
};

#endif //CONTAINADAPTER_H
