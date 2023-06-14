/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating spotting distance from a wind-driven surface
*			fire, torching trees, or a burning pile
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this corresponding cpp file is, in part or in
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

#include "spot.h"
#define _USE_MATH_DEFINES
#include <cstring>
#include <cmath>

Spot::Spot()
{
    initializeMembers();
}

Spot::~Spot()
{

}

Spot::Spot(const Spot& rhs)
{
    memberwiseCopyAssignment(rhs);
}

Spot& Spot::operator=(const Spot& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void Spot::memberwiseCopyAssignment(const Spot& rhs)
{
    memcpy(speciesFlameHeightParameters_, rhs.speciesFlameHeightParameters_, SpotInputs::SpotArrayConstants::NUM_SPECIES * sizeof(speciesFlameHeightParameters_[0]));
    memcpy(speciesFlameDurationParameters_, rhs.speciesFlameDurationParameters_, SpotInputs::SpotArrayConstants::NUM_SPECIES * sizeof(speciesFlameDurationParameters_[0]));
    memcpy(firebrandHeightFactors_, rhs.firebrandHeightFactors_, SpotInputs::SpotArrayConstants::NUM_FIREBRAND_ROWS * sizeof(firebrandHeightFactors_[0]));

    coverHeightUsedForSurfaceFire_ = rhs.coverHeightUsedForSurfaceFire_;
    coverHeightUsedForBurningPile_ = rhs.coverHeightUsedForBurningPile_;
    coverHeightUsedForTorchingTrees_ = rhs.coverHeightUsedForTorchingTrees_;
    flameHeightForTorchingTrees_ = rhs.flameHeightForTorchingTrees_;
    flameRatio_ = rhs.flameRatio_;
    firebrandDrift_ = rhs.firebrandDrift_;
    flameDuration_ = rhs.flameDuration_;
    firebrandHeightFromBurningPile_ = rhs.firebrandHeightFromBurningPile_;
    firebrandHeightFromSurfaceFire_ = rhs.firebrandHeightFromSurfaceFire_;
    firebrandHeightFromTorchingTrees_ = rhs.firebrandHeightFromTorchingTrees_;
    flatDistanceFromBurningPile_ = rhs.flatDistanceFromBurningPile_;
    flatDistanceFromSurfaceFire_ = rhs.flatDistanceFromSurfaceFire_;
    flatDistanceFromTorchingTrees_ = rhs.mountainDistanceFromBurningPile_;
    mountainDistanceFromBurningPile_ = rhs.mountainDistanceFromBurningPile_;
    mountainDistanceFromSurfaceFire_ = rhs.mountainDistanceFromBurningPile_;
    mountainDistanceFromTorchingTrees_ = rhs.mountainDistanceFromBurningPile_;
}

void Spot::initializeMembers()
{
    // Set up speciesFlameHeightParameters_
    const double tempSpeciesFlameHeightParameters[SpotInputs::SpotArrayConstants::NUM_SPECIES][SpotInputs::SpotArrayConstants::NUM_COLS] =
    {
      { 15.7, 0.451 },  //  0 Engelmann spruce
      { 15.7, 0.451 },  //  1 Douglas-fir
      { 15.7, 0.451 },  //  2 subalpine fir
      { 15.7, 0.451 },  //  3 western hemlock
      { 12.9, 0.453 },  //  4 ponderosa pine
      { 12.9, 0.453 },  //  5 lodgepole pine
      { 12.9, 0.453 },  //  6 western white pine
      { 16.5, 0.515 },  //  7 grand fir
      { 16.5, 0.515 },  //  8 balsam fir
      { 2.71, 1.000 },  //  9 slash pine
      { 2.71, 1.000 },  // 10 longleaf pine
      { 2.71, 1.000 },  // 11 pond pine
      { 2.71, 1.000 },  // 12 shortleaf pine
      { 2.71, 1.000 }   // 13 loblolly pine
      //{12.9, .453 },  // 14 western larch (guessed)
      //{15.7, .515 }   // 15 western red cedar (guessed)
    };
    memcpy(speciesFlameHeightParameters_, tempSpeciesFlameHeightParameters, SpotInputs::SpotArrayConstants::NUM_SPECIES * sizeof(speciesFlameHeightParameters_[0]));

    // Set up speciesFlameDurationParameters_
    const double tempSpeciesFlameDurationParameters[SpotInputs::SpotArrayConstants::NUM_SPECIES][SpotInputs::SpotArrayConstants::NUM_COLS] =
    {
      { 12.6, -0.256 },  //  0 Engelmann spruce
      { 10.7, -0.278 },  //  1 Douglas-fir
      { 10.7, -0.278 },  //  2 subalpine fir
      { 6.30, -0.249 },  //  3 western hemlock
      { 12.6, -0.256 },  //  4 ponderosa pine
      { 12.6, -0.256 },  //  5 lodgepole pine
      { 10.7, -0.278 },  //  6 western white pine
      { 10.7, -0.278 },  //  7 grand fir
      { 10.7, -0.278 },  //  8 balsam fir
      { 11.9, -0.389 },  //  9 slash pine
      { 11.9, -0.389 },  // 10 longleaf pine
      { 7.91, -0.344 },  // 11 pond pine
      { 7.91, -0.344 },  // 12 shortleaf pine
      { 13.5, -0.544 }   // 13 loblolly pine
      //{ 6.3, -.249},   // 14 western larch (guessed)
      //{ 12.6, -.256}   // 15 western red cedar (guessed)
    };
    memcpy(speciesFlameDurationParameters_, tempSpeciesFlameDurationParameters, SpotInputs::SpotArrayConstants::NUM_SPECIES * sizeof(speciesFlameDurationParameters_[0]));

    // Set up firebrandHeightFactors_
    const double tempFirebrandHeightFactors[SpotInputs::SpotArrayConstants::NUM_FIREBRAND_ROWS][SpotInputs::SpotArrayConstants::NUM_COLS] =
    {
      { 4.24, 0.332 },
      { 3.64, 0.391 },
      { 2.78, 0.418 },
      { 4.70, 0.000 }
    };
    memcpy(firebrandHeightFactors_, tempFirebrandHeightFactors, SpotInputs::SpotArrayConstants::NUM_FIREBRAND_ROWS * sizeof(firebrandHeightFactors_[0]));

    coverHeightUsedForSurfaceFire_ = 0.0;
    coverHeightUsedForBurningPile_ = 0.0;
    coverHeightUsedForTorchingTrees_ = 0.0;
    flameHeightForTorchingTrees_ = 0.0;
    flameRatio_ = 0.0;
    firebrandDrift_ = 0.0;
    flameDuration_ = 0.0;
    firebrandHeightFromBurningPile_ = 0.0;
    firebrandHeightFromSurfaceFire_ = 0.0;
    firebrandHeightFromTorchingTrees_ = 0.0;
    flatDistanceFromBurningPile_ = 0.0;
    flatDistanceFromSurfaceFire_ = 0.0;
    flatDistanceFromTorchingTrees_ = 0.0;
    mountainDistanceFromBurningPile_ = 0.0;
    mountainDistanceFromSurfaceFire_ = 0.0;
    mountainDistanceFromTorchingTrees_ = 0.0;
}

double Spot::calculateSpotCriticalCoverHeight(double firebrandHeight, double coverHeight)
{
    // Minimum value of coverHeight used to calculate flatDistance
    // using log variation with ht.
    double criticalHeight = (firebrandHeight < 1e-7)
        ? (0.0)
        : (2.2 * pow(firebrandHeight, 0.337) - 4.0);

    // Cover height used in calculation of flatDistance.
    double coverHeightUsed = (coverHeight > criticalHeight)
        ? (coverHeight)
        : (criticalHeight);

    return coverHeightUsed;
}

double Spot::calculateDownwindCanopyCoverHeight() const
{
    SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode = getDownwindCanopyMode();
    if (downwindCanopyMode == SpotDownWindCanopyMode::OPEN)
    {
        return spotInputs_.getDownwindCoverHeight(LengthUnits::Feet) * 0.5;
    }
    else
    {
        return spotInputs_.getDownwindCoverHeight(LengthUnits::Feet);
    }
}

double Spot::spotDistanceMountainTerrain(
    double flatDistance,
    SpotFireLocation::SpotFireLocationEnum location,
    double ridgeToValleyDistance,
    double ridgeToValleyElevation)
{
    double mountainDistance = flatDistance;
    if (ridgeToValleyElevation > 1e-7 && ridgeToValleyDistance > 1e-7)
    {
        double a1 = flatDistance / ridgeToValleyDistance;
        double b1 = ridgeToValleyElevation / (10.0 * M_PI) / 1000.0;
        double x = a1;
        for (int i = 0; i < 6; i++)
        {
            x = a1 - b1 * (cos(M_PI * x - location * M_PI / 2.0)
                - cos(location * M_PI / 2.0));
        }
        mountainDistance = x * ridgeToValleyDistance;
    }
    return mountainDistance;
}

double Spot::spotDistanceFlatTerrain(
    double firebrandHeight,
    double coverHeight,
    double windSpeedAtTwentyFeet)
{
    // Flat terrain spotting distance.
    double flatDistance = 0.0;
    if (coverHeight > 1e-7)
    {
        flatDistance = 0.000718 * windSpeedAtTwentyFeet * sqrt(coverHeight)
            * (0.362 + sqrt(firebrandHeight / coverHeight) / 2.0
                * log(firebrandHeight / coverHeight));
    }
    return flatDistance;
}

void Spot::calculateSpottingDistanceFromBurningPile()
{
    // Get needed inputs
    SpotFireLocation::SpotFireLocationEnum location = spotInputs_.getLocation();
    double ridgeToValleyDistance = spotInputs_.getRidgeToValleyDistance(LengthUnits::Miles);
    double ridgeToValleyElevation = spotInputs_.getRidgeToValleyElevation(LengthUnits::Feet);
    double downwindCoverHeight = calculateDownwindCanopyCoverHeight();
    double windSpeedAtTwentyFeet = spotInputs_.getWindSpeedAtTwentyFeet(SpeedUnits::MilesPerHour);
    double burningPileflameHeight = spotInputs_.getBurningPileFlameHeight(LengthUnits::Feet);

    // Initialize return values
    firebrandHeightFromBurningPile_ = 0.0;
    flatDistanceFromBurningPile_ = 0.0;
    mountainDistanceFromBurningPile_ = 0.0;

    // Determine maximum firebrand height
    if ((windSpeedAtTwentyFeet > 1e-7) && (burningPileflameHeight > 1e-7))
    {
        // Determine maximum firebrand height
        firebrandHeightFromBurningPile_ = 12.2 * burningPileflameHeight;

        // Cover height used in calculation of flatDist.
        coverHeightUsedForBurningPile_ = calculateSpotCriticalCoverHeight(firebrandHeightFromBurningPile_, downwindCoverHeight);
        if (coverHeightUsedForBurningPile_ > 1e-7)
        {
            // Flat terrain spotting distance.
            flatDistanceFromBurningPile_ = 0.000718 * windSpeedAtTwentyFeet * sqrt(coverHeightUsedForBurningPile_)
                * (0.362 + sqrt(firebrandHeightFromBurningPile_ / coverHeightUsedForBurningPile_) / 2.0
                    * log(firebrandHeightFromBurningPile_ / coverHeightUsedForBurningPile_));
            // Adjust for mountainous terrain.
            mountainDistanceFromBurningPile_ = spotDistanceMountainTerrain(flatDistanceFromBurningPile_,
                location, ridgeToValleyDistance, ridgeToValleyElevation);
            // Convert distances from miles to feet (base distance unit)
            flatDistanceFromBurningPile_ = LengthUnits::toBaseUnits(flatDistanceFromBurningPile_, LengthUnits::Miles);
            mountainDistanceFromBurningPile_ = LengthUnits::toBaseUnits(mountainDistanceFromBurningPile_, LengthUnits::Miles);
        }
    }
}

void Spot::calculateSpottingDistanceFromSurfaceFire()
{
    // Get needed inputs
    SpotFireLocation::SpotFireLocationEnum location = spotInputs_.getLocation();
    double ridgeToValleyDistance = spotInputs_.getRidgeToValleyDistance(LengthUnits::Miles);
    double ridgeToValleyElevation = spotInputs_.getRidgeToValleyElevation(LengthUnits::Feet);
    double downwindCoverHeight = calculateDownwindCanopyCoverHeight();
    double windSpeedAtTwentyFeet = spotInputs_.getWindSpeedAtTwentyFeet(SpeedUnits::MilesPerHour);
    double flameLength = spotInputs_.getSurfaceFlameLength(LengthUnits::Feet);

    // Initialize return values
    firebrandHeightFromSurfaceFire_ = 0.0;
    flatDistanceFromSurfaceFire_ = 0.0;
    firebrandDrift_ = 0.0;

    // Determine maximum firebrand height
    if ((windSpeedAtTwentyFeet) > 1e-7 && (flameLength > 1e-7))
    {
        // f is a function relating thermal energy to windspeed.
        double f = 322. * pow((0.474 * windSpeedAtTwentyFeet), -1.01);

        // Byram's fireline intensity is derived back from flame length.
        double byrams = pow((flameLength / 0.45), (1. / 0.46));

        // Initial firebrand height (ft).
        firebrandHeightFromSurfaceFire_ = ((f * byrams) < 1e-7)
            ? (0.0)
            : (1.055 * sqrt(f * byrams));

        // Cover height used in calculation of localflatDistance.
        coverHeightUsedForSurfaceFire_ = calculateSpotCriticalCoverHeight(firebrandHeightFromSurfaceFire_, downwindCoverHeight);

        if (coverHeightUsedForSurfaceFire_ > 1e-7)
        {
            firebrandDrift_ = 0.000278 * windSpeedAtTwentyFeet * pow(firebrandHeightFromSurfaceFire_, 0.643);
            flatDistanceFromSurfaceFire_ = spotDistanceFlatTerrain(firebrandHeightFromSurfaceFire_, coverHeightUsedForSurfaceFire_, windSpeedAtTwentyFeet) + firebrandDrift_;
            mountainDistanceFromSurfaceFire_ = spotDistanceMountainTerrain(flatDistanceFromSurfaceFire_,
                location, ridgeToValleyDistance, ridgeToValleyElevation);
            // Convert distances from miles to feet (base distance unit)
            flatDistanceFromSurfaceFire_ = LengthUnits::toBaseUnits(flatDistanceFromSurfaceFire_, LengthUnits::Miles);
            mountainDistanceFromSurfaceFire_ = LengthUnits::toBaseUnits(mountainDistanceFromSurfaceFire_, LengthUnits::Miles);
        }
    }
}

void Spot::calculateSpottingDistanceFromTorchingTrees()
{
    // Get needed inputs
    SpotFireLocation::SpotFireLocationEnum location = spotInputs_.getLocation();
    double ridgeToValleyDistance = spotInputs_.getRidgeToValleyDistance(LengthUnits::Miles);
    double ridgeToValleyElevation = spotInputs_.getRidgeToValleyElevation(LengthUnits::Feet);
    double downwindCoverHeight = calculateDownwindCanopyCoverHeight();
    double windSpeedAtTwentyFeet = spotInputs_.getWindSpeedAtTwentyFeet(SpeedUnits::MilesPerHour);
    double torchingTrees = spotInputs_.getTorchingTrees();
    double DBH = spotInputs_.getDBH(LengthUnits::Inches);
    double treeHeight = spotInputs_.getTreeHeight(LengthUnits::Feet);
    SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies = spotInputs_.getTreeSpecies();

    // Initialize return variables
    flameRatio_ = 0.0;
    flameHeightForTorchingTrees_ = 0.0;
    flameDuration_ = 0.0;
    firebrandHeightFromTorchingTrees_ = 0.0;
    flatDistanceFromTorchingTrees_ = 0.0;
    mountainDistanceFromTorchingTrees_ = 0.0;

    // Determine maximum firebrand height
    if (windSpeedAtTwentyFeet > 1e-7 && DBH > 1e-7 && torchingTrees >= 1.0)
    {
        // Catch species errors.
        if (!(treeSpecies < 0 || treeSpecies >= 14))
        {
            // Steady flame height (ft).
            flameHeightForTorchingTrees_ = speciesFlameHeightParameters_[treeSpecies][0]
                * pow(DBH, speciesFlameHeightParameters_[treeSpecies][1])
                * pow(torchingTrees, 0.4);

            flameRatio_ = treeHeight / flameHeightForTorchingTrees_;
            // Steady flame duration.
            flameDuration_ = speciesFlameDurationParameters_[treeSpecies][0]
                * pow(DBH, speciesFlameDurationParameters_[treeSpecies][1])
                * pow(torchingTrees, -0.2);

            int i;
            if (flameRatio_ >= 1.0)
            {
                i = 0;
            }
            else if (flameRatio_ >= 0.5)
            {
                i = 1;
            }
            else if (flameDuration_ < 3.5)
            {
                i = 2;
            }
            else
            {
                i = 3;
            }

            // Initial firebrand height (ft).
            firebrandHeightFromTorchingTrees_ = firebrandHeightFactors_[i][0] * pow(flameDuration_, firebrandHeightFactors_[i][1]) * flameHeightForTorchingTrees_ + treeHeight / 2.0;

            // Cover ht used in calculation of flatDist.
            coverHeightUsedForTorchingTrees_ = calculateSpotCriticalCoverHeight(firebrandHeightFromTorchingTrees_, downwindCoverHeight);
            if (coverHeightUsedForTorchingTrees_ > 1e-7)
            {
                flatDistanceFromTorchingTrees_ = spotDistanceFlatTerrain(firebrandHeightFromTorchingTrees_, coverHeightUsedForTorchingTrees_, windSpeedAtTwentyFeet);
                mountainDistanceFromTorchingTrees_ = spotDistanceMountainTerrain(flatDistanceFromTorchingTrees_, location, ridgeToValleyDistance,
                    ridgeToValleyElevation);
                // Convert distances from miles to feet (base distance unit)
                flatDistanceFromTorchingTrees_ = LengthUnits::toBaseUnits(flatDistanceFromTorchingTrees_, LengthUnits::Miles);
                mountainDistanceFromTorchingTrees_ = LengthUnits::toBaseUnits(mountainDistanceFromTorchingTrees_, LengthUnits::Miles);
            }
        }
    }
}

void Spot::setBurningPileFlameHeight(double buringPileFlameHeight, LengthUnits::LengthUnitsEnum flameHeightUnits)
{
    spotInputs_.setBurningPileFlameHeight(buringPileFlameHeight, flameHeightUnits);
}

void Spot::setDBH(double DBH, LengthUnits::LengthUnitsEnum DBHUnits)
{
    spotInputs_.setDBH(DBH, DBHUnits);
}

void Spot::setDownwindCoverHeight(double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits)
{
    spotInputs_.setDownwindCoverHeight(downwindCoverHeight, coverHeightUnits);
}

void Spot::setDownwindCanopyMode(SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode)
{
    spotInputs_.setDownwindCanopyMode(downwindCanopyMode);
}

void Spot::setFlameLength(double flameLength, LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    spotInputs_.setSurfaceFlameLength(flameLength, flameLengthUnits);
}

void Spot::setLocation(SpotFireLocation::SpotFireLocationEnum location)
{
    spotInputs_.setLocation(location);
}

void Spot::setRidgeToValleyDistance(double ridgeToValleyDistance, LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits)
{
    spotInputs_.setRidgeToValleyDistance(ridgeToValleyDistance, ridgeToValleyDistanceUnits);
}

void Spot::setRidgeToValleyElevation(double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits)
{
    spotInputs_.setRidgeToValleyElevation(ridgeToValleyElevation, elevationUnits);
}

void Spot::setTorchingTrees(int torchingTrees)
{
    spotInputs_.setTorchingTrees(torchingTrees);
}

void Spot::setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits)
{
    spotInputs_.setTreeHeight(treeHeight, treeHeightUnits);
}

void Spot::setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies)
{
    spotInputs_.setTreeSpecies(treeSpecies);
}

void Spot::setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
    spotInputs_.setWindSpeedAtTwentyFeet(windSpeedAtTwentyFeet, windSpeedUnits);
}

void Spot::updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
    double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode,
    double buringPileFlameHeight, LengthUnits::LengthUnitsEnum flameHeightUnits, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
    spotInputs_.updateSpotInputsForBurningPile(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits, ridgeToValleyElevation, elevationUnits,
        downwindCoverHeight, coverHeightUnits, downwindCanopyMode, buringPileFlameHeight, flameHeightUnits, windSpeedAtTwentyFeet, windSpeedUnits);
}

void Spot::updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
    double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode, double windSpeedAtTwentyFeet,
    SpeedUnits::SpeedUnitsEnum windSpeedUnits, double surfaceFlameLength, LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    spotInputs_.updateSpotInputsForSurfaceFire(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits, ridgeToValleyElevation,elevationUnits,
        downwindCoverHeight, coverHeightUnits, downwindCanopyMode, windSpeedAtTwentyFeet, windSpeedUnits, surfaceFlameLength, flameLengthUnits);
}

void Spot::updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits, double ridgeToValleyElevation, LengthUnits::LengthUnitsEnum elevationUnits,
    double downwindCoverHeight, LengthUnits::LengthUnitsEnum coverHeightUnits, SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum downwindCanopyMode, int torchingTrees, double DBH,
    LengthUnits::LengthUnitsEnum DBHUnits, double treeHeight, LengthUnits::LengthUnitsEnum  treeHeightUnits,
    SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
    spotInputs_.updateSpotInputsForTorchingTrees(location, ridgeToValleyDistance, ridgeToValleyDistanceUnits, ridgeToValleyElevation,
        elevationUnits, downwindCoverHeight, coverHeightUnits, downwindCanopyMode, torchingTrees, DBH, DBHUnits, treeHeight, treeHeightUnits, treeSpecies,
        windSpeedAtTwentyFeet, windSpeedUnits);
}

double Spot::getBurningPileFlameHeight(LengthUnits::LengthUnitsEnum flameHeightUnits) const
{
    return spotInputs_.getBurningPileFlameHeight(flameHeightUnits);
}

double Spot::getDBH(LengthUnits::LengthUnitsEnum DBHUnits) const
{
    return spotInputs_.getDBH(DBHUnits);
}

double Spot::getDownwindCoverHeight(LengthUnits::LengthUnitsEnum coverHeightUnits) const
{
    return calculateDownwindCanopyCoverHeight();
}

SpotDownWindCanopyMode::SpotDownWindCanopyModeEnum Spot::getDownwindCanopyMode() const
{
    return spotInputs_.getDownWindCanopyMode();
}

double Spot::getSurfaceFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits) const
{
    return spotInputs_.getSurfaceFlameLength(flameLengthUnits);
}

SpotFireLocation::SpotFireLocationEnum Spot::getLocation() const
{
    return spotInputs_.getLocation();
}

double Spot::getRidgeToValleyDistance(LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits) const
{
    return spotInputs_.getRidgeToValleyDistance(ridgeToValleyDistanceUnits);
}

double Spot::getRidgeToValleyElevation(LengthUnits::LengthUnitsEnum elevationUnits) const
{
    return spotInputs_.getRidgeToValleyElevation(elevationUnits);
}

int Spot::getTorchingTrees() const
{
    return spotInputs_.getTorchingTrees();
}

double Spot::getTreeHeight(LengthUnits::LengthUnitsEnum  treeHeightUnits) const
{
    return spotInputs_.getTreeHeight(treeHeightUnits);
}

SpotTreeSpecies::SpotTreeSpeciesEnum Spot::getTreeSpecies() const
{
    return spotInputs_.getTreeSpecies();
}

double Spot::getWindSpeedAtTwentyFeet(SpeedUnits::SpeedUnitsEnum windSpeedUnits) const
{
    return spotInputs_.getWindSpeedAtTwentyFeet(windSpeedUnits);
}

double Spot::getCoverHeightUsedForBurningPile(LengthUnits::LengthUnitsEnum coverHeightUnits) const
{
    return LengthUnits::fromBaseUnits(coverHeightUsedForBurningPile_, coverHeightUnits);
}

double Spot::getCoverHeightUsedForSurfaceFire(LengthUnits::LengthUnitsEnum coverHeightUnits) const
{
    return  LengthUnits::fromBaseUnits(coverHeightUsedForSurfaceFire_, coverHeightUnits);
}

double Spot::getCoverHeightUsedForTorchingTrees(LengthUnits::LengthUnitsEnum coverHeightUnits) const
{
    return  LengthUnits::fromBaseUnits(coverHeightUsedForTorchingTrees_, coverHeightUnits);
}

double Spot::getFlameHeightForTorchingTrees(LengthUnits::LengthUnitsEnum flameHeightUnits) const
{
    return LengthUnits::fromBaseUnits(flameHeightForTorchingTrees_, flameHeightUnits);
}

double Spot::getFlameRatioForTorchingTrees() const
{
    return flameRatio_;
}

double Spot::getFlameDurationForTorchingTrees(TimeUnits::TimeUnitsEnum durationUnits) const
{
    return TimeUnits::fromBaseUnits(flameDuration_, durationUnits);
}

double Spot::getMaxFirebrandHeightFromBurningPile(LengthUnits::LengthUnitsEnum firebrandHeightUnits) const
{
    return LengthUnits::fromBaseUnits(firebrandHeightFromBurningPile_, firebrandHeightUnits);
}

double Spot::getMaxFirebrandHeightFromSurfaceFire(LengthUnits::LengthUnitsEnum firebrandHeightUnits) const
{
    return LengthUnits::fromBaseUnits(firebrandHeightFromSurfaceFire_, firebrandHeightUnits);
}

double Spot::getMaxFirebrandHeightFromTorchingTrees(LengthUnits::LengthUnitsEnum firebrandHeightUnits) const
{
    return LengthUnits::fromBaseUnits(firebrandHeightFromTorchingTrees_, firebrandHeightUnits);
}

double Spot::getMaxFlatTerrainSpottingDistanceFromBurningPile(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const
{
    return LengthUnits::fromBaseUnits(flatDistanceFromBurningPile_, spottingDistanceUnits);
}

double Spot::getMaxFlatTerrainSpottingDistanceFromSurfaceFire(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const
{
    return LengthUnits::fromBaseUnits(flatDistanceFromSurfaceFire_, spottingDistanceUnits);
}

double Spot::getMaxFlatTerrainSpottingDistanceFromTorchingTrees(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const
{
    return LengthUnits::fromBaseUnits(flatDistanceFromTorchingTrees_, spottingDistanceUnits);
}

double Spot::getMaxMountainousTerrainSpottingDistanceFromBurningPile(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const
{
    return LengthUnits::fromBaseUnits(mountainDistanceFromBurningPile_, spottingDistanceUnits);
}

double Spot::getMaxMountainousTerrainSpottingDistanceFromSurfaceFire(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const
{
    return LengthUnits::fromBaseUnits(mountainDistanceFromSurfaceFire_, spottingDistanceUnits);
}

double Spot::getMaxMountainousTerrainSpottingDistanceFromTorchingTrees(LengthUnits::LengthUnitsEnum spottingDistanceUnits) const
{
    return LengthUnits::fromBaseUnits(mountainDistanceFromTorchingTrees_, spottingDistanceUnits);
}
