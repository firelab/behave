/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the inputs required for crown fire behavior
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

#include "crownInputs.h"

CrownInputs::CrownInputs()
{
    initializeMembers();
}

CrownInputs::~CrownInputs()
{

}

void CrownInputs::initializeMembers()
{
    canopyBaseHeight_ = 0;
    canopyBulkDensity_ = 0;
    canopyUserProvidedFlameLength_ = 0;
    canopyUserProvidedFirelineIntensity_ = 0;
    foliarMoisture_ = 0;
    canopyBaseHeightUnits_ = LengthUnits::FEET;
    canopyBulkDensityUnits_ = DensityUnits::POUNDS_PER_CUBIC_FOOT;
}

double CrownInputs::getCanopyBaseHeight() const
{
    return canopyBaseHeight_;
}

double CrownInputs::getCanopyBulkDensity() const
{
    return canopyBulkDensity_;
}

double CrownInputs::getCanopyFlameLength() const
{
    return canopyUserProvidedFlameLength_;
}

double CrownInputs::getCanopyFirelineIntensity() const
{
    return canopyUserProvidedFirelineIntensity_;
}

DensityUnits::DensityUnitsEnum CrownInputs::getCanopyBulkDensityUnits() const
{
    return canopyBulkDensityUnits_;
}

LengthUnits::LengthUnitsEnum CrownInputs::getCanopyBaseHeightUnits() const
{
    return canopyBaseHeightUnits_;
}

double CrownInputs::getFoliarMoisture() const
{
    return foliarMoisture_;
}

void CrownInputs::setCanopyBaseHeight(double canopyBaseHeight)
{
    canopyBaseHeight_ = LengthUnits::toBaseUnits(canopyBaseHeight, canopyBaseHeightUnits_);
}

void CrownInputs::setCanopyBulkDensity(double canopyBulkDensity)
{
    canopyBulkDensity_ = DensityUnits::toBaseUnits(canopyBulkDensity, canopyBulkDensityUnits_);
}

void CrownInputs::setCanopyFlameLength(double canopyUserProvidedFlameLength)
{
    canopyUserProvidedFlameLength_ = canopyUserProvidedFlameLength;
}

void CrownInputs::setCanopyFirelineIntensity(double canopyUserProvidedFirelineIntensity)
{
    canopyUserProvidedFirelineIntensity_ = canopyUserProvidedFirelineIntensity;
}

void CrownInputs::setCanopyBulkDensityUnits(DensityUnits::DensityUnitsEnum densityUnits)
{
    canopyBulkDensityUnits_ = densityUnits;
}

void CrownInputs::setCanopyBaseHeightUnits(LengthUnits::LengthUnitsEnum canopyBaseHeightUnits)
{
    canopyBaseHeightUnits_ = canopyBaseHeightUnits;
}

void CrownInputs::setFoliarMoisture(double foliarMoisture)
{
    foliarMoisture_ = foliarMoisture;
}

void CrownInputs::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    setCanopyBaseHeight(canopyBaseHeight);
    setCanopyBulkDensity(canopyBulkDensity);
    setFoliarMoisture(foliarMoisture);
}
