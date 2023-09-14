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

#ifndef CROWNINPUTS_H
#define CROWNINPUTS_H

#include "behaveUnits.h"

class CrownInputs
{
public:
    CrownInputs();
    ~CrownInputs();

    void initializeMembers();

    void setCanopyBaseHeight(double canopyBaseHeight, LengthUnits::LengthUnitsEnum canopyBaseHeightUnits);
    void setCanopyBulkDensity(double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits);
    void setCanopyFlameLength(double canopyUserProvidedFlameLength);
    void setCanopyFirelineIntensity(double canopyUserProvidedFirelineIntensity);
    void setMoistureFoliar(double moistureFoliar, FractionUnits::FractionUnitsEnum moistureUnits);

    double getCanopyBaseHeight(LengthUnits::LengthUnitsEnum heightUnits) const;
    double getCanopyBulkDensity(DensityUnits::DensityUnitsEnum densityUnits) const;
    double getCanopyFlameLength() const;
    double getCanopyFirelineIntensity() const;
 
    double getMoistureFoliar(FractionUnits::FractionUnitsEnum moistureUnits) const;

    void updateCrownInputs(double canopyBaseHeight, LengthUnits::LengthUnitsEnum heightUnits, 
        double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits, double moistureFoliar, 
        FractionUnits::FractionUnitsEnum moistureUnits);

protected:
    double canopyBaseHeight_; //Canopy base height(ft)
    double canopyBulkDensity_; // Canopy bulk density(lb / ft3)
    double canopyUserProvidedFlameLength_; // Flame length(ft)
    double canopyUserProvidedFirelineIntensity_; // Fireline intensity(ft)
    double moistureFoliar_; // Tree foliar moisture content (lb water/lb foliage)

};

#endif // CROWNINPUTS_H
