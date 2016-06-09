/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling crown fire behavior based on the Facade OOP 
*           Design Pattern and using the Rothermel spread model
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

#ifndef CROWN_HEADER
#define CROWN_HEADER

#include "crownInputs.h"
#include "crownEnums.h"
#include "surface.h"

class FuelModelSet;

class Crown
{
public:
    Crown() = delete; // No default constructor
    Crown(const FuelModelSet& fuelModelSet, const Surface& surface);
    ~Crown();

    Crown(const Crown &rhs);
    Crown& operator= (const Crown& rhs);

    void doCrownRun();
    void updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture);

    double getCrownFireSpreadRate() const;

private:
    const FuelModelSet* fuelModelSet_;  // pointer to BehaveRun's FuelModelSet object
    CrownInputs crownInputs_;    // pointer to BehaveRun's CrownInputs object
    
    // SURFACE module components
    const Surface* surface_;                            // pointer to the BehaveRun's Surface object
    Surface crownDeepCopyOfSurface_;                    // deep copy of Surface's surface inputs to allow parallel runs in Surface

    // Private methods
    void initializeMemmbers();
    void calculateCanopyHeatPerUnitArea();
    void calculateCrownFireHeatPerUnitArea();
    void calculateCrownFuelLoad();
    void calculateCrownFirelineIntensity();
    void calculateCrownFlameLength();
    void calculateCrownPowerOfFire();
    void calcuateCrownPowerOfWind();
    void calculateCrownCriticalFireSpreadRate();
    void calculateCrownCriticalSurfaceFireIntensity();
    void calculateCrownCriticalSurfaceFlameLength();
    void calculateCrownFireTransitionRatio();
    void calcualteCrownFirePowerRatio();
    void calculateCrownFireActiveRatio();
    void calculateFireType();
    double calculateWindSpeedAtTwentyFeet();

    // Member variables
    FireType::FireTypeEnum fireType_;
    double crownCopyOfSurfaceHeatPerUnitArea_;
    double crownCopyOfSurfaceFirelineIntensity_;
    double crownFuelLoad_;                          // return Crown fire fuel load (lb / ft2)
    double canopyHeatPerUnitArea_;
    double crownFireHeatPerUnitArea_;
    double crownFirelineIntensity_;                 // Crown fire fireline intensity (Btu / ft / s)
    double crownFlameLength_;                       // Crown fire flame length (ft)
    double crownFireSpreadRate_;
    double crownCriticalSurfaceFireIntensity_;      // Critical surface fire intensity (Btu / ft / s)
    double crownCriticalFireSpreadRate_;            // Critical crown fire spread rate (ft / min)
    double crownCriticalSurfaceFlameLength_;        // Critical surface fire flame length (ft)
    double crownPowerOfFire_;                       // Crown fire 'power of the fire' ( ft*lb / s / ft2)
    double crownPowerOfWind_;                       // Crown fire 'power of the wind' ( ft*lb / s / ft2)
    double crownFirePowerRatio_;
    double crownFireActiveRatio_;                   // Crown fire active ratio
    double crownFireTransitionRatio_;
    double windSpeedAtTwentyFeet_;
};

#endif // CROWN_HEADER
