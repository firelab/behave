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

struct FireType
{
    enum FireTypeEnum
    {
        Surface = 0,    // surface fire with no torching or crown fire spread.
        Torching = 1,   // surface fire with torching.
        ConditionalCrownFire = 2, // active crown fire possible if the fire transitions to the overstory        
        Crowning = 3    // active crown fire, fire is spreading through the canopy.
    };
};

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
    void setMoistureFoliar(double moistureFoliar, MoistureUnits::MoistureUnitsEnum moistureUnits);

    double getCanopyBaseHeight() const;
    double getCanopyBulkDensity() const;
    double getCanopyFlameLength() const;
    double getCanopyFirelineIntensity() const;
    DensityUnits::DensityUnitsEnum  getCanopyBulkDensityUnits() const;
    LengthUnits::LengthUnitsEnum getCanopyBaseHeightUnits() const;
    double getMoistureFoliar(MoistureUnits::MoistureUnitsEnum moistureUnits) const;

    void updateCrownInputs(double canopyBaseHeight, LengthUnits::LengthUnitsEnum heightUnits, 
        double canopyBulkDensity, DensityUnits::DensityUnitsEnum densityUnits, double moistureFoliar, 
        MoistureUnits::MoistureUnitsEnum moistureUnits);

private:
    double canopyBaseHeight_; //Canopy base height(ft)
    double canopyBulkDensity_; // Canopy bulk density(lb / ft3)
    double canopyUserProvidedFlameLength_; // Flame length(ft)
    double canopyUserProvidedFirelineIntensity_; // Fireline intensity(ft)
    double moistureFoliar_; // Tree foliar moisture content (lb water/lb foliage)

};

#endif // CROWNINPUTS_H
