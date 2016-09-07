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

#include "spotEnums.h"
#include "spotInputs.h"

class Spot
{
public:
    Spot();
    ~Spot();
    
    Spot(const Spot &rhs);
    Spot& operator= (const Spot& rhs);

    void calculateSpottingDistanceFromBurningPile();
    void calculateSpottingDistanceFromSurfaceFire();
    void calculateSpottingDistanceFromTorchingTrees();

	// Spot Inputs Setters
	void setBurningPileFlameHeight(double buringPileflameHeight);
	void setDBH(double DBH);
    void setDownwindCoverHeight(double downwindCoverHeight);
    void setLocation(SpotFireLocation::SpotFireLocationEnum location);
    void setRidgeToValleyDistance(double ridgeToValleyDistance);
    void setRidgeToValleyElevation(double ridgeToValleyElevation);
    void setFlameLength(double flameLength);
    void setTorchingTrees(double torchingTrees);
    void setTreeHeight(double treeHeight);
	void setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies);
	void setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet);
	void updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double buringPileFlameHeight,
		double windSpeedAtTwentyFeet = SpotSurfaceInputs::NOT_SET);
	void updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double windSpeedAtTwentyFeet = SpotSurfaceInputs::NOT_SET,
		double flameLength = SpotSurfaceInputs::NOT_SET);
	void updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double torchingTrees, double DBH, double treeHeight,
		SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet = SpotSurfaceInputs::NOT_SET);

	// Spot Inputs Getters
	double getBurningPileFlameHeight();
	double getDBH();
	double getDownwindCoverHeight();
	SpotFireLocation::SpotFireLocationEnum getLocation();
	double getRidgeToValleyDistance();
	double getRidgeToValleyElevation();
	double getFlameLength();
	double getTorchingTrees();
	double getTreeHeight();
	SpotTreeSpecies::SpotTreeSpeciesEnum getTreeSpecies();
	double getWindSpeedAtTwentyFeet();

	// Spot Outputs Getters
    double getCoverHeightUsedForBurningPile();
    double getCoverHeightUsedForSurfaceFire();
    double getCoverHeightUsedForTorchingTrees();
    double getFlameHeightForTorchingTrees();
    double getFlameRatioForTorchingTrees();
    double getFlameDurationForTorchingTrees();
    double getMaxFirebrandHeightFromBurningPile();
    double getMaxFirebrandHeightFromSurfaceFire();
    double getMaxFirebrandHeightFromTorchingTrees();
    double getMaxFlatTerrainSpottingDistanceFromBurningPile();
    double getMaxFlatTerrainSpottingDistanceFromSurfaceFire();
    double getMaxFlatTerrainSpottingDistanceFromTorchingTrees();
    double getMaxSpottingDistanceFromBurningPile();
    double getMaxSpottingDistanceFromSurfaceFire();
    double getMaxSpottingDistanceFromTorchingTrees();

private:
	SpotInputs spotInputs_;

    void initializeMembers();
    double calculateSpotCriticalCoverHeight(double firebrandHeight, double coverHeight);
    double spotDistanceFlatTerrain(double firebrandHeight, double coverHeight, double windSpeedAtTwentyFeet);
    double spotDistanceMountainTerrain(double flatDistance, SpotFireLocation::SpotFireLocationEnum location, 
		double ridgeToValleyDistance, double ridgeToValleyElevation);

    double speciesFlameHeightParameters_[SpotArrayConstants::NUM_SPECIES][SpotArrayConstants::NUM_COLS];
    double speciesFlameDurationParameters_[SpotArrayConstants::NUM_SPECIES][SpotArrayConstants::NUM_COLS];
    double firebrandHeightFactors_[SpotArrayConstants::NUM_FIREBRAND_ROWS][SpotArrayConstants::NUM_COLS];

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
    double mountainDistanceFromBurningPile_;    // Maximum spotting distance over mountain terrain for burning pile(mi).
    double mountainDistanceFromSurfaceFire_;    // Maximum spotting distance over mountain terrain surface fire(mi).
    double mountainDistanceFromTorchingTrees_;  // Maximum spotting distance over mountain terrain torching trees(mi).
};

#endif // SPOT_H
