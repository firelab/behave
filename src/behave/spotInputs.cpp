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

#include "spotInputs.h"

SpotInputs::SpotInputs()
{
	initializeMembers();
}

void SpotInputs::setBurningPileFlameHeight(double buringPileFlameHeight, LengthUnits::LengthUnitsEnum flameHeightUnits)
{
	buringPileFlameHeight_ = LengthUnits::toBaseUnits(buringPileFlameHeight, flameHeightUnits);
}

void SpotInputs::setDBH(double DBH, LengthUnits::LengthUnitsEnum DBHUnits)
{
	DBH_ = LengthUnits::toBaseUnits(DBH, DBHUnits);
}

void SpotInputs::setDownwindCoverHeight(double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits)
{
	downwindCoverHeight_ = LengthUnits::toBaseUnits(downwindCoverHeight, coverHeightUnits);
}

void SpotInputs::setSurfaceFlameLength(double surfaceFlameLength, LengthUnits::LengthUnitsEnum flameLengthUnits)
{
	surfaceFlameLength_ = LengthUnits::toBaseUnits(surfaceFlameLength, flameLengthUnits);
}

void SpotInputs::setLocation(SpotFireLocation::SpotFireLocationEnum location)
{
	location_ = location;
}

void SpotInputs::setRidgeToValleyDistance(double ridgeToValleyDistance, LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits)
{
	ridgeToValleyDistance_ = LengthUnits::toBaseUnits(ridgeToValleyDistance, ridgeToValleyDistanceUnits);
}

void SpotInputs::setRidgeToValleyElevation(double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits)
{
	ridgeToValleyElevation_ = LengthUnits::toBaseUnits(ridgeToValleyElevation, elevationUnits);
}

void SpotInputs::setTorchingTrees(double torchingTrees)
{
	torchingTrees_ = torchingTrees;
}

void SpotInputs::setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits)
{
	treeHeight_ = LengthUnits::toBaseUnits(treeHeight, treeHeightUnits);
}

void SpotInputs::setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies)
{
	treeSpecies_ = treeSpecies;
}

void SpotInputs::setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
	windSpeedAtTwentyFeet_ = SpeedUnits::toBaseUnits(windSpeedAtTwentyFeet, windSpeedUnits);
}

void SpotInputs::updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
    double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, double buringPileFlameHeight,
    LengthUnits::LengthUnitsEnum flameHeightUnits, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
	setLocation(location);
	setRidgeToValleyDistance(ridgeToValleyDistance, ridgeToValleyDistanceUnits);
	setRidgeToValleyElevation(ridgeToValleyElevation, elevationUnits);
	setDownwindCoverHeight(downwindCoverHeight, coverHeightUnits);
	setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet, windSpeedUnits);
	setBurningPileFlameHeight(buringPileFlameHeight, flameHeightUnits);
}

void SpotInputs::updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
    double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, double windSpeedAtTwentyFeet,
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, double surfaceFlameLength, LengthUnits::LengthUnitsEnum flameLengthUnits)
{
	setLocation(location);
    setRidgeToValleyDistance(ridgeToValleyDistance, ridgeToValleyDistanceUnits);
    setRidgeToValleyElevation(ridgeToValleyElevation, elevationUnits);
    setDownwindCoverHeight(downwindCoverHeight, coverHeightUnits);
    setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet, windSpeedUnits);
	setSurfaceFlameLength(surfaceFlameLength, flameLengthUnits);
}

void SpotInputs::updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
    double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, double torchingTrees, double DBH,
    LengthUnits::LengthUnitsEnum DBHUnits, double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits, 
    SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
	setLocation(location);
    setRidgeToValleyDistance(ridgeToValleyDistance, ridgeToValleyDistanceUnits);
    setRidgeToValleyElevation(ridgeToValleyElevation, elevationUnits);
    setDownwindCoverHeight(downwindCoverHeight, coverHeightUnits);
	setTorchingTrees(torchingTrees);
	setDBH(DBH, DBHUnits);
	setTreeHeight(treeHeight, treeHeightUnits);
	setTreeSpecies(treeSpecies);
	setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet, windSpeedUnits);
}

double SpotInputs::getBurningPileFlameHeight(LengthUnits::LengthUnitsEnum flameHeightUnits)
{
	return LengthUnits::fromBaseUnits(buringPileFlameHeight_, flameHeightUnits);
}

double SpotInputs::getDBH(LengthUnits::LengthUnitsEnum DBHUnits)
{
	return LengthUnits::fromBaseUnits(DBH_, DBHUnits);
}

double SpotInputs::getDownwindCoverHeight(LengthUnits::LengthUnitsEnum coverHeightUnits)
{
	return LengthUnits::fromBaseUnits(downwindCoverHeight_, coverHeightUnits);
}

double SpotInputs::getSurfaceFlameLength(LengthUnits::LengthUnitsEnum surfaceFlameLengthUnits)
{
	return LengthUnits::fromBaseUnits(surfaceFlameLength_, surfaceFlameLengthUnits);
}

SpotFireLocation::SpotFireLocationEnum SpotInputs::getLocation()
{
	return location_;
}

double SpotInputs::getRidgeToValleyDistance(LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits)
{
	return LengthUnits::fromBaseUnits(ridgeToValleyDistance_, ridgeToValleyDistanceUnits);
}

double SpotInputs::getRidgeToValleyElevation(LengthUnits::LengthUnitsEnum elevationUnits)
{
	return LengthUnits::fromBaseUnits(ridgeToValleyElevation_, elevationUnits);
}

double SpotInputs::getTorchingTrees()
{
	return torchingTrees_;
}

double SpotInputs::getTreeHeight(LengthUnits::LengthUnitsEnum  treeHeightUnits)
{
	return LengthUnits::fromBaseUnits(treeHeight_, treeHeightUnits);
}

SpotTreeSpecies::SpotTreeSpeciesEnum SpotInputs::getTreeSpecies()
{
	return treeSpecies_;
}

double SpotInputs::getWindSpeedAtTwentyFeet(SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
	return SpeedUnits::fromBaseUnits(windSpeedAtTwentyFeet_, windSpeedUnits);
}

void SpotInputs::initializeMembers()
{
	downwindCoverHeight_ = 0.0;
	location_ = SpotFireLocation::MIDSLOPE_WINDWARD;
	ridgeToValleyDistance_ = 0.0;
	ridgeToValleyElevation_ = 0.0;
	windSpeedAtTwentyFeet_ = 0.0;
	buringPileFlameHeight_ = 0.0;
    surfaceFlameLength_ = 0.0;
	torchingTrees_ = 0.0;
	DBH_ = 0.0;
	treeHeight_ = 0.0;
}
