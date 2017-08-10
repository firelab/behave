/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class to handle resource vector required for Contain simulations.
*           Allows for original Contain source files to be used without
*           modification with a C++ interface to be used in Behave-like 
*           applications
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

#ifndef CONTAINFORCEADAPTER_H
#define CONTAINFORCEADAPTER_H

#include <vector>
#include "ContainResource.h"

class ContainForceAdapter
{
public:


    ContainForceAdapter();
    ~ContainForceAdapter();

    int size() const;

    void addResource(Sem::ContainResource& resource);
    void ContainForceAdapter::addResource(double arrival, double production, double duration,
        Sem::ContainFlank flank, std::string desc, double baseCost, double hourCost);
    void removeAllResources();
    int removeResourceAt(int index);
    int removeResourceWithThisDesc(std::string desc);
    int removeAllResourcesWithThisDesc(std::string desc);

    double getResourceArrivalAtIndex(int index) const;
    double getResourceBaseCostAtIndex(int index) const;
    std::string getResourceDescriptionAtIndex(int index) const;
    double getResourceDurationAtIndex(int index) const;
    Sem::ContainFlank getResourceFlankAtIndex(int index) const;
    double GetResourceHourCostAtIndex(int index) const;
    double GetResourceProductionAtIndex(int index) const;

private:
    std::vector<Sem::ContainResource> resourceVector;
};

#endif // CONTAINADAPTER_H
