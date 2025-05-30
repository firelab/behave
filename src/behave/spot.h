/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating spotting distance from a wind-driven surface
*			fire, torching trees, or a burning pile
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in the corresponding cpp file is, in part or in
*           whole, from BehavePlus5 source originally authored by Collin D.
*           Bevins and is used with or without modification.
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

#ifndef SPOT_H
#define SPOT_H

#include "spotInputs.h"
#include "CrownFirebrandProcessor.h"

class Spot
{
public:
    Spot();
    ~Spot();

    Spot(const Spot& rhs);
    Spot& operator=(const Spot& rhs);

    void initializeMembers();

    void calculateSpottingDistanceFromBurningPile();
    void calculateSpottingDistanceFromSurfaceFire();
    void calculateSpottingDistanceFromTorchingTrees();
    void calculateSpottingDistanceFromActiveCrown();

  // Spot Inputs Setters
    void setBurningPileFlameHeight(double buringPileflameHeight, LengthUnits::LengthUnitsEnum flameHeightUnits);
    void setDBH(double DBH, LengthUnits::LengthUnitsEnum DBHUnits);
    void setDownwindCoverHeight(double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits);
    void setDownwindCanopyMode(SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode);
    void setFlameLength(double flameLength, LengthUnits::LengthUnitsEnum flameLengthUnits);
    void setFirelineIntensity(double firelineIntensity, FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits);
    void setLocation(SpotFireLocation::SpotFireLocationEnum location);
    void setRidgeToValleyDistance(double ridgeToValleyDistance, LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits);
    void setRidgeToValleyElevation(double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits);
    void setTorchingTrees(int torchingTrees);
    void setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits);
    void setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies);
    void setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits);

  void updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
      LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
      double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode,
      double buringPileFlameHeight, LengthUnits::LengthUnitsEnum flameHeightUnits, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits);
  void updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
      LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
      double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode,
      double windSpeedAtTwentyFeet,
      SpeedUnits::SpeedUnitsEnum windSpeedUnits, double flameLength, LengthUnits::LengthUnitsEnum flameLengthUnits);
  void updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
      LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
      double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode,
      int torchingTrees, double DBH, LengthUnits::LengthUnitsEnum DBHUnits, double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits,
      SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits);

  // Spot Inputs Getters
    double getBurningPileFlameHeight(LengthUnits::LengthUnitsEnum flameHeightUnits) const;
    double getDBH(LengthUnits::LengthUnitsEnum DBHUnits) const;
    double getDownwindCoverHeight(LengthUnits::LengthUnitsEnum coverHeightUnits) const;
    SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum getDownwindCanopyMode() const;
    double getSurfaceFlameLength(LengthUnits::LengthUnitsEnum surfaceFlameLengthUnits) const;
    double getCrownFirelineIntensity(FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits) const;
    SpotFireLocation::SpotFireLocationEnum getLocation() const;
    double getRidgeToValleyDistance(LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits) const;
    double getRidgeToValleyElevation(LengthUnits::LengthUnitsEnum elevationUnits) const;
    int getTorchingTrees() const;
    double getTreeHeight(LengthUnits::LengthUnitsEnum  treeHeightUnits) const;
    SpotTreeSpecies::SpotTreeSpeciesEnum getTreeSpecies() const;
    double getWindSpeedAtTwentyFeet(SpeedUnits::SpeedUnitsEnum windSpeedUnits) const;

  // Spot Outputs Getters
    double getCoverHeightUsedForBurningPile(LengthUnits::LengthUnitsEnum coverHeightUnits) const;
    double getCoverHeightUsedForSurfaceFire(LengthUnits::LengthUnitsEnum coverHeightUnits) const;
    double getCoverHeightUsedForTorchingTrees(LengthUnits::LengthUnitsEnum coverHeightUnits) const;
    double getFlameHeightForTorchingTrees(LengthUnits::LengthUnitsEnum flameHeightUnits) const;
    double getFlameRatioForTorchingTrees() const;
    double getFlameDurationForTorchingTrees(TimeUnits::TimeUnitsEnum durationUnits) const;
    double getMaxFirebrandHeightFromBurningPile(LengthUnits::LengthUnitsEnum firebrandHeightUnits) const;
    double getMaxFirebrandHeightFromSurfaceFire(LengthUnits::LengthUnitsEnum firebrandHeightUnits) const;
    double getMaxFirebrandHeightFromTorchingTrees(LengthUnits::LengthUnitsEnum firebrandHeightUnits) const;
    double getMaxFlatTerrainSpottingDistanceFromBurningPile(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;
    double getMaxFlatTerrainSpottingDistanceFromSurfaceFire(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;
    double getMaxFlatTerrainSpottingDistanceFromTorchingTrees(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;
    double getMaxMountainousTerrainSpottingDistanceFromBurningPile(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;
    double getMaxMountainousTerrainSpottingDistanceFromSurfaceFire(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;
    double getMaxMountainousTerrainSpottingDistanceFromTorchingTrees(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;

    double getMaxMountainousTerrainSpottingDistanceFromActiveCrown(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const;

protected:
    void memberwiseCopyAssignment(const Spot& rhs);
    double calculateSpotCriticalCoverHeight(double firebrandHeight, double coverHeight);
    double calculateDownwindCanopyCoverHeight(LengthUnits::LengthUnitsEnum coverHeightUnits) const;
    double calculateTreeHeight(LengthUnits::LengthUnitsEnum coverHeightUnits) const;
    double spotDistanceFlatTerrain(double firebrandHeight, double coverHeight, double windSpeedAtTwentyFeet);
    double spotDistanceMountainTerrain(double flatDistance, SpotFireLocation::SpotFireLocationEnum location,
    double ridgeToValleyDistance, double ridgeToValleyElevation);

    SpotInputs spotInputs_;

    double speciesFlameHeightParameters_[SpotInputs::SpotArrayConstants::NUM_SPECIES][SpotInputs::SpotArrayConstants::NUM_COLS];
    double speciesFlameDurationParameters_[SpotInputs::SpotArrayConstants::NUM_SPECIES][SpotInputs::SpotArrayConstants::NUM_COLS];
    double firebrandHeightFactors_[SpotInputs::SpotArrayConstants::NUM_FIREBRAND_ROWS][SpotInputs::SpotArrayConstants::NUM_COLS];

  // Outputs
    double coverHeightUsedForSurfaceFire_;      // Actual tree / vegetation ht used for surface fire(ft)
    double coverHeightUsedForBurningPile_;      // Actual tree / vegetation ht used for burning pile(ft)
    double coverHeightUsedForTorchingTrees_;    // Actual tree / vegetation ht used for burning pile(ft)
    double flameHeightForTorchingTrees_;        // Steady state flame height for torching trees(ft).
    double flameRatio_;                         // Ratio of tree height to steady flame height(ft / ft).
    double firebrandDrift_;                     // Maximum firebrand drift from surface fire(mi).
    double flameDuration_;                      // Flame duration(dimensionless)
    double firebrandHeightFromBurningPile_;     // Initial maximum firebrand height for burning pile(ft).
    double firebrandHeightFromSurfaceFire_;     // Initial maximum firebrand height for surface fire(ft).
    double firebrandHeightFromTorchingTrees_;   // Initial maximum firebrand height for torching trees(ft).
    double flatDistanceFromBurningPile_;        // Maximum spotting distance over flat terrain for burning pile(mi).
    double flatDistanceFromSurfaceFire_;        // Maximum spotting distance over flat terrain for surface fire(mi).
    double flatDistanceFromTorchingTrees_;      // Maximum spotting distance over flat terrain for torching trees(mi).
    double flatDistanceFromActiveCrown_;      // Maximum spotting distance over flat terrain for torching trees(mi).

    double mountainDistanceFromBurningPile_;    // Maximum spotting distance over mountain terrain for burning pile(mi).
    double mountainDistanceFromSurfaceFire_;    // Maximum spotting distance over mountain terrain surface fire(mi).
    double mountainDistanceFromTorchingTrees_;  // Maximum spotting distance over mountain terrain torching trees(mi).
    double mountainDistanceFromActiveCrown_;    // Maximum spotting distance over mountain terrain active crown(mi).
};

#endif // SPOT_H
