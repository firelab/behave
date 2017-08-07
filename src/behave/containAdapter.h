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

class ContainAdapter
{
public:
    ContainAdapter();
    ~ContainAdapter();

    void setReportSize(double reportSize);
    void setReportRate(double reportRate);
    void setFireStartTime(int fireStartTime);
    void setLwRatio(double lwRatio);
    void setTactic(Sem::Contain::ContainTactic tactic);
    void setAttackDistance(double attackDistance);
    void setRetry(bool retry);
    void setMinSteps(int minSteps);
    void setMaxSteps(int maxSteps);
    void setMaxFireSize(int maxFireSize);
    void setMaxFireTime(int maxFireTime);

    void doContainRun();

private:
    double reportSize_;
    double reportRate_;
    double diurnalROS_[24];
    int fireStartTime_;
    double lwRatio_;
    Sem::ContainForce force_;
    Sem::Contain::ContainTactic tactic_;
    double attackDistance_;
    bool retry_;
    int minSteps_;
    int maxSteps_;
    int maxFireSize_;
    int maxFireTime_;
};

#endif //CONTAINADAPTER_H
