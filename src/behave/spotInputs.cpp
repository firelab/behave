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

SpotInputs::SpotInputs(const SpotInputs & rhs)
{
	DBH_ = rhs.DBH_;
	downwindCoverHeight_ = rhs.downwindCoverHeight_;
	location_ = rhs.location_;
	ridgeToValleyDistance_ = rhs.ridgeToValleyDistance_;
	ridgeToValleyElevation_ = rhs.ridgeToValleyElevation_;
	windSpeedAtTwentyFeet_ = rhs.windSpeedAtTwentyFeet_;
	buringPileFlameHeight_ = rhs.buringPileFlameHeight_;
	flameLength_ = rhs.flameLength_;
	torchingTrees_ = rhs.torchingTrees_;
	treeHeight_ = rhs.treeHeight_;
}

SpotInputs & SpotInputs::operator=(const SpotInputs & rhs)
{
	if (this != &rhs)
	{
        DBH_ = rhs.DBH_;
        downwindCoverHeight_ = rhs.downwindCoverHeight_;
        location_ = rhs.location_;
        ridgeToValleyDistance_ = rhs.ridgeToValleyDistance_;
        ridgeToValleyElevation_ = rhs.ridgeToValleyElevation_;
        windSpeedAtTwentyFeet_ = rhs.windSpeedAtTwentyFeet_;
        buringPileFlameHeight_ = rhs.buringPileFlameHeight_;
        flameLength_ = rhs.flameLength_;
        torchingTrees_ = rhs.torchingTrees_;
        treeHeight_ = rhs.treeHeight_;
	}
	return *this;
}

void SpotInputs::setBurningPileFlameHeight(double buringPileFlameHeight)
{
	buringPileFlameHeight_ = buringPileFlameHeight;
}

void SpotInputs::setDBH(double DBH)
{
	DBH_ = DBH;
}

void SpotInputs::setDownwindCoverHeight(double downwindCoverHeight)
{
	downwindCoverHeight_ = downwindCoverHeight;
}

void SpotInputs::setFlameLength(double flameLength)
{
	flameLength_ = flameLength;
}

void SpotInputs::setLocation(SpotFireLocation::SpotFireLocationEnum location)
{
	location_ = location;
}

void SpotInputs::setRidgeToValleyDistance(double ridgeToValleyDistance)
{
	ridgeToValleyDistance_ = ridgeToValleyDistance;
}

void SpotInputs::setRidgeToValleyElevation(double ridgeToValleyElevation)
{
	ridgeToValleyElevation_ = ridgeToValleyElevation;
}

void SpotInputs::setTorchingTrees(double torchingTrees)
{
	torchingTrees_ = torchingTrees;
}

void SpotInputs::setTreeHeight(double treeHeight)
{
	treeHeight_ = treeHeight;
}

void SpotInputs::setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies)
{
	treeSpecies_ = treeSpecies;
}

void SpotInputs::setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet)
{
	windSpeedAtTwentyFeet_ = windSpeedAtTwentyFeet;
}

void SpotInputs::updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance, double ridgeToValleyElevation, double downwindCoverHeight, double windSpeedAtTwentyFeet, double buringPileflameHeight)
{
	setLocation(location);
	setRidgeToValleyDistance(ridgeToValleyDistance);
	setRidgeToValleyElevation(ridgeToValleyElevation);
	setDownwindCoverHeight(downwindCoverHeight);
	setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet);
	setBurningPileFlameHeight(buringPileflameHeight);
}

void SpotInputs::updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance, double ridgeToValleyElevation, double downwindCoverHeight, double windSpeedAtTwentyFeet, double flameLength)
{
	setLocation(location);
	setRidgeToValleyDistance(ridgeToValleyDistance);
	setRidgeToValleyElevation(ridgeToValleyElevation);
	setDownwindCoverHeight(downwindCoverHeight);
	setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet);
	setFlameLength(flameLength);
}

void SpotInputs::updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance, double ridgeToValleyElevation, double downwindCoverHeight, double torchingTrees, double DBH, double treeHeight, SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet)
{
	setLocation(location);
	setRidgeToValleyDistance(ridgeToValleyDistance);
	setRidgeToValleyElevation(ridgeToValleyElevation);
	setDownwindCoverHeight(downwindCoverHeight);
	setTorchingTrees(torchingTrees);
	setDBH(DBH);
	setTreeHeight(treeHeight);
	setTreeSpecies(treeSpecies);
	setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet);
}

double SpotInputs::getBurningPileFlameHeight()
{
	return buringPileFlameHeight_;
}

double SpotInputs::getDBH()
{
	return DBH_;
}

double SpotInputs::getDownwindCoverHeight()
{
	return downwindCoverHeight_;
}

double SpotInputs::getFlameLength()
{
	return flameLength_;
}

SpotFireLocation::SpotFireLocationEnum SpotInputs::getLocation()
{
	return location_;
}

double SpotInputs::getRidgeToValleyDistance()
{
	return ridgeToValleyDistance_;
}

double SpotInputs::getRidgeToValleyElevation()
{
	return ridgeToValleyElevation_;
}

double SpotInputs::getTorchingTrees()
{
	return torchingTrees_;
}

double SpotInputs::getTreeHeight()
{
	return treeHeight_;
}

SpotTreeSpecies::SpotTreeSpeciesEnum SpotInputs::getTreeSpecies()
{
	return treeSpecies_;
}

double SpotInputs::getWindSpeedAtTwentyFeet()
{
	return windSpeedAtTwentyFeet_;
}

void SpotInputs::initializeMembers()
{
	downwindCoverHeight_ = 0.0;
	location_ = SpotFireLocation::MIDSLOPE_WINDWARD;
	ridgeToValleyDistance_ = 0.0;
	ridgeToValleyElevation_ = 0.0;
	windSpeedAtTwentyFeet_ = SpotSurfaceInputs::NOT_SET;
	buringPileFlameHeight_ = 0.0;
	flameLength_ = SpotSurfaceInputs::NOT_SET;
	torchingTrees_ = 0.0;
	DBH_ = 0.0;
	treeHeight_ = 0.0;
}
