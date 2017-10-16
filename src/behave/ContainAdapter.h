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
#include "ContainForceAdapter.h"

#include "behaveUnits.h"
#include "fireSize.h"

#include <string>

//------------------------------------------------------------------------------
/*! \enum ContainTactic
\brief Identifies the possible fire containment tactic.
are assigned.
*/

namespace ContainAdapterEnums
{
    struct ContainTactic
    {
        enum ContainTacticEnum
        {
            HeadAttack = 0,     //!< Containment forces attack fire head
            RearAttack = 1      //!< Containment forces attack fire rear
        };
    };

    //------------------------------------------------------------------------------
    /*! \enum ContainStatus
    \brief Identifies the current fire containment status.
    */
    struct ContainStatus
    {
        enum ContainStatusEnum
        {
            Unreported = 0,     //!< Fire started but not yet reported (init() not called)
            Reported = 1,     //!< Fire reported but not yet attacked (init() called)
            Attacked = 2,     //!< Fire attacked but not yet resolved
            Contained = 3,     //!< Fire contained by attacking forces
            Overrun = 4,     //!< Attacking forces are overrun
            Exhausted = 5,     //!< Fire escaped when all resources are exhausted
            Overflow = 6,     //!< Simulation max step overflow
            SizeLimitExceeded = 7,      //!< Simulation max fire size exceeded    
            TimeLimitExceeded = 8	    //!< Simulation max fire time exceeded 
        };
    };

    struct ContainFlank
    {
        enum ContainFlankEnum
        {
            LeftFlank = 0,   //!< Attack left (upper) flank only (full production)
            RightFlank = 1,   //!< Attack right (lower) flank only (full production)
            BothFlanks = 2,   //!< Attack both flanks (half of production per flank)
            NeitherFlank = 3    //!< Attack neither flank (inactive)
        };
    };
}

using std::string;
using namespace ContainAdapterEnums;

class ContainAdapter
{
public:
    ContainAdapter();
    ~ContainAdapter();

    void addResource(Sem::ContainResource& resource);
    // Construct ContainResource into ContainForce
    void addResource(
        double arrival,
        double duration,
        TimeUnits::TimeUnitsEnum timeUnit,
        double productionRate,
        SpeedUnits::SpeedUnitsEnum productionRateUnits,
        std::string description = "",
        double baseCost = 0.0,
        double hourCost = 0.0);
    int removeResourceAt(int index);
    int removeResourceWithThisDesc(string desc);
    int removeAllResourcesWithThisDesc(string desc);
    void removeAllResources();

    void setReportSize(double reportSize, AreaUnits::AreaUnitsEnum areaUnits);
    void setReportRate(double reportRate, SpeedUnits::SpeedUnitsEnum speedUnits);
    void setFireStartTime(int fireStartTime);
    void setLwRatio(double lwRatio);
    void setTactic(ContainAdapterEnums::ContainTactic::ContainTacticEnum tactic);
    void setAttackDistance(double attackDistance, LengthUnits::LengthUnitsEnum lengthUnits);
    void setRetry(bool retry);
    void setMinSteps(int minSteps);
    void setMaxSteps(int maxSteps);
    void setMaxFireSize(int maxFireSize);
    void setMaxFireTime(int maxFireTime);

    void doContainRun();

    double getFinalCost() const;
    double getFinalFireLineLength(LengthUnits::LengthUnitsEnum lengthUnits) const;
    double getPerimiterAtInitialAttack(LengthUnits::LengthUnitsEnum lengthUnits) const;
    double getPerimeterAtContainment(LengthUnits::LengthUnitsEnum lengthUnits) const;
    double getFireSizeAtInitialAttack(AreaUnits::AreaUnitsEnum areaUnits) const;
    double getFinalFireSize(AreaUnits::AreaUnitsEnum areaUnits) const;
    double getFinalContainmentArea(AreaUnits::AreaUnitsEnum areaUnits) const;
    double getFinalTimeSinceReport(TimeUnits::TimeUnitsEnum timeUnits) const;
    ContainStatus::ContainStatusEnum getContainmentStatus() const;

private:
    FireSize size_; 

    Sem::Contain::ContainTactic convertAdapterTacticToSemTactic(ContainAdapterEnums::ContainTactic::ContainTacticEnum tactic);
    ContainAdapterEnums::ContainStatus::ContainStatusEnum convertSemStatusToAdapterStatus(Sem::Contain::ContainStatus status);
    Sem::ContainFlank converAdapterFlankToSemFlank(ContainAdapterEnums::ContainFlank::ContainFlankEnum flank);

    // Contain Inputs
    double reportSize_;
    double reportRate_;
    double diurnalROS_[24];
    int fireStartTime_;
    double lwRatio_;
    ContainForceAdapter force_;
    Sem::Contain::ContainTactic tactic_;
    double attackDistance_;
    bool retry_;
    int minSteps_;
    int maxSteps_;
    int maxFireSize_;
    int maxFireTime_;

    // Contain Outputs
    double finalCost_; // Final total cost of all resources used
    double finalFireLineLength_;  // Final fire line at containment or escape
    double perimeterAtInitialAttack_; // firee perimeter at time of initial attack
    double perimeterAtContainment_; // Final line plus fire perimeter at containment or escape
    double fireSizeAtIntitialAttack_; // Fire size (area) at time of initial attack
    double finalFireSize_; // Final fire size (area) at containment or escape
    double finalContainmentArea_; // Final containment area at containment or escape
    double finalTime_; // Containment or escape time since report
    ContainAdapterEnums::ContainStatus::ContainStatusEnum containmentStatus_;
};

#endif //CONTAINADAPTER_H