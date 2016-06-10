/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the Palmetto-Gallbery special case fuel model
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

#ifndef PALMETTOGALLBERRY_HEADER
#define PALMETTOGALLBERRY_HEADER

class PalmettoGallberry
{
public:
    PalmettoGallberry();
    PalmettoGallberry(const PalmettoGallberry &rhs);
    PalmettoGallberry& operator= (const PalmettoGallberry& rhs);

    void initializeMembers();

    double calculatePalmettoGallberyDeadOneHourLoad(double ageOfRough, double heightOfUnderstory);
    double calculatePalmettoGallberyDeadTenHourLoad(double ageOfRough, double palmettoCoverage);
    double calculatePalmettoGallberyDeadFoliageLoad(double ageOfRough, double palmettoCoverage);
    double calculatePalmettoGallberyFuelBedDepth(double heightOfUnderstory);
    double calculatePalmettoGallberyLitterLoad(double ageOfRough, double overstoryBasalArea);
    double calculatePalmettoGallberyLiveOneHourLoad(double ageOfRough, double heightOfUnderstory);
    double calculatePalmettoGallberyLiveTenHourLoad(double ageOfRough, double heightOfUnderstory);
    double calculatePalmettoGallberyLiveFoliageLoad(double ageOfRough, double palmettoCoverage, double heightOfUnderstory);

    double getMoistureOfExtinctionDead() const;
    double getHeatOfCombustionDead() const;
    double getHeatOfCombustionLive() const;
    double getPalmettoGallberyDeadOneHourLoad() const;
    double getPalmettoGallberyDeadTenHourLoad() const;
    double getPalmettoGallberyDeadFoliageLoad() const;
    double getPalmettoGallberyFuelBedDepth() const;
    double getPalmettoGallberyLitterLoad() const;
    double getPalmettoGallberyLiveOneHourLoad() const;
    double getPalmettoGallberyLiveTenHourLoad() const;
    double getPalmettoGallberyLiveFoliageLoad() const;

private:
    double moistureOfExtinctionDead_;
    double heatOfCombustionDead_;
    double heatOfCombustionLive_;
    double palmettoGallberyDeadOneHourLoad_;
    double palmettoGallberyDeadTenHourLoad_;
    double palmettoGallberyDeadFoliageLoad_;
    double palmettoGallberyFuelBedDepth_;
    double palmettoGallberyLitterLoad_;
    double palmettoGallberyLiveOneHourLoad_;
    double palmettoGallberyLiveTenHourLoad_;
    double palmettoGallberyLiveFoliageLoad_;
};

#endif //PALMETTOGALLBERRY_HEADER
