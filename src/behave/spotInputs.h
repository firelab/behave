/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for storing inputs used by the Spot class
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

#ifndef SPOTINPUTS_H
#define SPOTINPUTS_H

#include "behaveUnits.h"

struct SpotTreeSpecies
{
    enum SpotTreeSpeciesEnum
    {
        ENGELMANN_SPRUCE = 0,    //  0 Engelmann spruce (Picea engelmannii)
        DOUGLAS_FIR = 1,    //  1 Douglas-fir (Pseudotsuga menziessii)
        SUBALPINE_FIR = 2,    //  2 Subalpine fir (Abies lasiocarpa)
        WESTERN_HEMLOCK = 3,    //  3 Western hemlock (Tsuga heterophylla)
        PONDEROSA_PINE = 4,    //  4 Ponderosa pine (Pinus ponderosa)
        LODGEPOLE_PINE = 5,    //  5 Lodgepole pine (Pinus contora)
        WESTERN_WHITE_PINE = 6,    //  6 Western white pine (Pinus monticola)
        GRAND_FIR = 7,    //  7 Grand fir (Abies grandis)
        BALSAM_FIR = 8,    //  8 Balsam fir (Abies balsamea)
        SLASH_PINE = 9,    //  9 Slash pine (Pinus elliottii)
        LONGLEAF_PINE = 10,   // 10 Longleaf pine (Pinus palustrus)
        POND_PINE = 11,   // 11 Pond pine (Pinus serotina)
        SHORTLEAF_PINE = 12,   // 12 Shortleaf pine (Pinus echinata)
        LOBLOLLY_PINE = 13    // 13 Loblolly pine (Pinus taeda)
    };
};

struct SpotFireLocation
{
    enum SpotFireLocationEnum
    {
        MIDSLOPE_WINDWARD = 0,    // midslope, windward
        VALLEY_BOTTOM = 1,    // valley bottom
        MIDSLOPE_LEEWARD = 2,    // midslope, leeward
        RIDGE_TOP = 3,    // ridge top
    };
};

class SpotInputs
{
public:
  SpotInputs();

  void setBurningPileFlameHeight(double buringPileFlameHeight, LengthUnits::LengthUnitsEnum flameHeightUnits);
  void setDBH(double DBH, LengthUnits::LengthUnitsEnum DBHUnits);
  void setDownwindCoverHeight(double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits);
  void setDownwindOpenCanopy(int downwindOpenCanopy);
  void setSurfaceFlameLength(double surfaceFlameLength, LengthUnits::LengthUnitsEnum flameLengthUnits);
  void setLocation(SpotFireLocation::SpotFireLocationEnum location);
  void setRidgeToValleyDistance(double ridgeToValleyDistance, LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits);
  void setRidgeToValleyElevation(double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits);
  void setTorchingTrees(int torchingTrees);
  void setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits);
  void setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies);
  void setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits);

  void updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
        LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
        double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, double buringPileFlameHeight,
        LengthUnits::LengthUnitsEnum flameHeightUnits, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits);
  void updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
        LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
        double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, double windSpeedAtTwentyFeet,
        SpeedUnits::SpeedUnitsEnum windSpeedUnits, double surfaceFlameLength, LengthUnits::LengthUnitsEnum flameLengthUnits);
  void updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
        LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
        double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, int torchingTrees, double DBH,
        LengthUnits::LengthUnitsEnum DBHUnits, double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits,
        SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits);

  double getBurningPileFlameHeight(LengthUnits::LengthUnitsEnum flameHeightUnits);
  double getDBH(LengthUnits::LengthUnitsEnum DBHUnits);
  double getDownwindCoverHeight(LengthUnits::LengthUnitsEnum coverHeightUnits);
  int getDownwindOpenCanopy();
  double getSurfaceFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits);
  SpotFireLocation::SpotFireLocationEnum getLocation();
  double getRidgeToValleyDistance(LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits);
  double getRidgeToValleyElevation(LengthUnits::LengthUnitsEnum elevationUnits);
  int getTorchingTrees();
  double getTreeHeight(LengthUnits::LengthUnitsEnum  treeHeightUnits);
  SpotTreeSpecies::SpotTreeSpeciesEnum getTreeSpecies();
  double getWindSpeedAtTwentyFeet(SpeedUnits::SpeedUnitsEnum windSpeedUnits);

    struct SpotArrayConstants
    {
        enum SpotArrayConstantsEnum
        {
            NUM_COLS = 2,
            NUM_FIREBRAND_ROWS = 4,
            NUM_SPECIES = 14
        };
    };

private:
  void initializeMembers();

  double DBH_;
  double downwindCoverHeight_;
  double downwindOpenCanopy_;
  SpotFireLocation::SpotFireLocationEnum location_;
  double ridgeToValleyDistance_;
  double ridgeToValleyElevation_;
  double windSpeedAtTwentyFeet_;
  double buringPileFlameHeight_;
  double surfaceFlameLength_;
  int torchingTrees_;
  double treeHeight_;
  SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies_;
};

#endif // SPOTINPUTS_H
