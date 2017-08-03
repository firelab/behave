/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Interface for Behave application based on the Facade OOP Design
*           Pattern used to tie together the modules and objects used by Behave
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

#include "behaveRun.h"

#include "fuelModelSet.h"

BehaveRun::BehaveRun(FuelModelSet& fuelModelSet)
    : surface(fuelModelSet),
    crown(fuelModelSet)
{
    fuelModelSet_ = &fuelModelSet;
}

BehaveRun::BehaveRun(const BehaveRun& rhs)
    : surface(*rhs.fuelModelSet_),
    crown(*rhs.fuelModelSet_)
{
    memberwiseCopyAssignment(rhs);
}

BehaveRun& BehaveRun::operator=(const BehaveRun& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void BehaveRun::memberwiseCopyAssignment(const BehaveRun& rhs)
{
    setFuelModelSet(*rhs.fuelModelSet_);
    surface = rhs.surface;
    crown = rhs.crown;
    spot = rhs.spot;
}

BehaveRun::~BehaveRun()
{

}

void BehaveRun::setFuelModelSet(FuelModelSet& fuelModelSet)
{
    // makes this behaveRun's fuelModelSet_ point to the FuelModelSet given to this method as a parameter
    fuelModelSet_ = &fuelModelSet;
    surface.setFuelModelSet(fuelModelSet);
    crown.setFuelModelSet(fuelModelSet);
}

bool BehaveRun::isFuelModelDefined(int fuelModelNumber) const
{
    return fuelModelSet_->isFuelModelDefined(fuelModelNumber);
}

double BehaveRun::getFuelLoadOneHour(int fuelModelNumber) const
{
    return fuelModelSet_->getFuelLoadOneHour(fuelModelNumber);
}

double BehaveRun::getFuelLoadTenHour(int fuelModelNumber) const
{
    return fuelModelSet_->getFuelLoadTenHour(fuelModelNumber);
}

double BehaveRun::getFuelLoadHundredHour(int fuelModelNumber) const
{
    return fuelModelSet_->getFuelLoadHundredHour(fuelModelNumber);
}

double BehaveRun::getFuelLoadLiveHerbaceous(int fuelModelNumber) const
{
    return fuelModelSet_->getFuelLoadLiveHerbaceous(fuelModelNumber);
}

double BehaveRun::getFuelLoadLiveWoody(int fuelModelNumber) const
{
    return fuelModelSet_->getFuelLoadLiveWoody(fuelModelNumber);
}
