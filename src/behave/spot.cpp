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
    // Set up speciesFlameHeightParameters_
    const double tempSpeciesFlameHeightParameters[NUM_SPECIES][NUM_COLS] =
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
    memcpy(speciesFlameHeightParameters_, tempSpeciesFlameHeightParameters, NUM_SPECIES * sizeof(speciesFlameHeightParameters_[0]));

    // Set up speciesFlameDurationParameters_
    const double tempSpeciesFlameDurationParameters[NUM_SPECIES][NUM_COLS] =
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
    memcpy(speciesFlameDurationParameters_, tempSpeciesFlameDurationParameters, NUM_SPECIES * sizeof(speciesFlameDurationParameters_[0]));

    // Set up firebrandHeightFactors_
    const double tempFirebrandHeightFactors[NUM_FIREBRAND_ROWS][NUM_COLS] =
    {
        { 4.24, 0.332 },
        { 3.64, 0.391 },
        { 2.78, 0.418 },
        { 4.70, 0.000 }
    };
    memcpy(firebrandHeightFactors_, tempFirebrandHeightFactors, NUM_FIREBRAND_ROWS * sizeof(firebrandHeightFactors_[0]));
}

Spot::~Spot()
{

}

//-------------------------------------------------------------------------------------
/*! \brief Calculates cover height used in spotting distance calculations.
 *
 *  \param firebrandHeight      Maximum firebrand height.
 *  \param coverHeight          Tree/vegetation cover height(ft).
 *
 */

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

//---------------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance adjusted for mountain terrain.
 *
 *  \param flatDistance             Maximum spotting distance over flat terrain (mi).
 *  \param location                 Location of the burning pile:
 *                                      0 == midslope, windward
 *                                      1 == valley bottom
 *                                      2 == midslope, leeward
 *                                      3 == ridge top
 *  \param ridgeToValleyDistance    Horizontal distance from ridge top to valley bottom
 *                                  (mi).
 *  \param ridgeToValleyElevation   Vertical distance from ridge top to valley bottom
 *                                  (ft).
 *  \return mountainDistance       Maximum spotting distance from the torching trees
 *                                  (mi).
 */

double Spot::spotDistanceMountainTerrain(
    double flatDistance,
    int    location,
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

//---------------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance over flat terrain.
 *
 *  \param firebrandHeight_         Maximum firebrand height (ft).
 *  \param coverHeight_             Tree/vegetation cover height (ft).
 *  \param windSpeedAtTwentyFeet_   Wind speed at 20 ft (mi/h).
 *
 *  \return flatDistance      Maximum spotting distance over flat terrain.
 */

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

//---------------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance from a surface fire.
 *
 *  \param location                 Location of the burning pile:
 *                                      0 == midslope, windward
 *                                      1 == valley bottom
 *                                      2 == midslope, leeward
 *                                      3 == ridge top
 *  \param ridgeToValleyDistance    Horizontal distance from ridge top to valley bottom
 *                                  (mi).
 *  \param ridgeToValleyElevation   Vertical distance from ridge top to valley bottom
 *                                  (ft).
 *  \param coverHeight              Tree/vegetation cover height (ft).
 *  \param windSpeedAtTwentyFeet    Wind speed at 20 ft (mi/h).
 *  \param flameLength              Surface fire flame length (ft).
 *  \param[out] heightUsed_         Actual tree/vegetation ht used (ft).
 *  \param[out] firebrandHeight_    Initial maximum firebrand height (ft).
 *  \param[out] firebrandDrift_     Maximum firebrand drift (mi).
 *  \param[out] flatDistance_       Maximum spotting distance over flat terrain (mi).
 *
 *  \return Maximum sptting distance from the surface fire (mi).
 */

double Spot::calculateSpottingDistanceFromSurfaceFire(
    int    location,
    double ridgeToValleyDistance,
    double ridgeToValleyElevation,
    double coverHeight,
    double windSpeedAtTwentyFeet,
    double flameLength)
{
    // Initialize return values
    firebrandHeightFromSurfaceFire_ = 0.0;
    flatDistanceFromSurfaceFire_ = 0.0;
    flatDistanceFromSurfaceFire_ = 0.0;
    firebrandDrift_ = 0.0;

    // Determine maximum firebrand height
    if ((windSpeedAtTwentyFeet) > 1e-7 && (flameLength > 1e-7))
    {
        // f is a function relating thermal energy to windspeed.
        double f = 322. * pow((0.474 * windSpeedAtTwentyFeet), -1.01);

        // Byram's fireline intensity is derived back from flame length.
        double byrams = pow((flameLength / .45), (1. / 0.46));

        // Initial firebrand height (ft).
        firebrandHeightFromSurfaceFire_ = ((f * byrams) < 1e-7)
            ? (0.0)
            : (1.055 * sqrt(f * byrams));

        // Cover height used in calculation of localflatDistance.
        coverHeightUsedForSurfaceFire_ = calculateSpotCriticalCoverHeight(firebrandHeightFromSurfaceFire_, coverHeight);

        if (coverHeightUsedForSurfaceFire_ > 1e-7)
        {
            firebrandDrift_ = 0.000278 * windSpeedAtTwentyFeet * pow(firebrandHeightFromSurfaceFire_, 0.643);
            flatDistanceFromSurfaceFire_ = spotDistanceFlatTerrain(firebrandHeightFromSurfaceFire_, coverHeightUsedForSurfaceFire_, windSpeedAtTwentyFeet) + firebrandDrift_;
            mountainDistanceFromSurfaceFire_ = spotDistanceMountainTerrain(flatDistanceFromSurfaceFire_,
                location, ridgeToValleyDistance, ridgeToValleyElevation);
        }
    }

    return mountainDistanceFromSurfaceFire_;
}

//---------------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance from a burning pile.
 *
 *  \param location                 Location of the burning pile:
 *                                      0 == midslope, windward
 *                                      1 == valley bottom
 *                                      2 == midslope, leeward
 *                                      3 == ridge top
 *  \param ridgeToValleyDistance    Horizontal distance from ridge top to valley bottom
 *                                  (mi).
 *  \param ridgeToValleyElevation   Vertical distance from ridge top to valley bottom
 *                                  (ft).
 *  \param coverHeight              Tree/vegetation cover height (ft).
 *  \param windSpeedAtTwentyFeet    Wind speed at 20 ft (mi/h).
 *  \param flameHeight              Burning pile's flame height (ft).
 *  \param[out] heightUsed_         Actual tree/vegetation ht used (ft).
 *  \param[out] firebrandHeight_    Initial maximum firebrand height (ft).
 *  \param[out] flatDistance_       Maximum spotting distance over flat terrain (mi).
 *
 *  \return Maximum spotting distance from the burning pile (mi).
 */

double Spot::calculateSpottingDistanceFromBurningPile(
    int    location,
    double ridgeToValleyDistance,
    double ridgeToValleyElevation,
    double coverHeight,
    double windSpeedAtTwentyFeet,
    double flameHeight)
{
    // Initialize return values
    firebrandHeightFromBurningPile_ = 0.0;
    flatDistanceFromBurningPile_ = 0.0;
    mountainDistanceFromBurningPile_ = 0.0;

    // Determine maximum firebrand height
    if ((windSpeedAtTwentyFeet > 1e-7) && (flameHeight > 1e-7))
    {
        // Determine maximum firebrand height
        firebrandHeightFromBurningPile_ = 12.2 * flameHeight;

        // Cover height used in calculation of flatDist.
        coverHeightUsedForBurningPile_ = calculateSpotCriticalCoverHeight(firebrandHeightFromBurningPile_, coverHeight);
        if (coverHeightUsedForBurningPile_ > 1e-7)
        {
            // Flat terrain spotting distance.
            flatDistanceFromBurningPile_ = 0.000718 * windSpeedAtTwentyFeet * sqrt(coverHeightUsedForBurningPile_)
                * (0.362 + sqrt(firebrandHeightFromBurningPile_ / coverHeightUsedForBurningPile_) / 2.0
                    * log(firebrandHeightFromBurningPile_ / coverHeightUsedForBurningPile_));
            // Adjust for mountainous terrain.
            mountainDistanceFromBurningPile_ = spotDistanceMountainTerrain(flatDistanceFromBurningPile_,
                location, ridgeToValleyDistance, ridgeToValleyElevation);
        } // if ht > 1e-7
    } // if windSpeedAt20Ft > 1e-7 && z > 1e-7

    return mountainDistanceFromBurningPile_;
}

//---------------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance from a group of torching trees.
 *
 *  \param location				Location of the burning pile:
 *									0 == midslope, windward
 *									1 == valley bottom
 *									2 == midslope, leeward
 *									3 == ridge top
 *  \param ridgeToValleyDist	    Horizontal distance from ridge top to valley bottom
 *								    (mi).
 *  \param ridgeToValleyElev	    Vertical distance from ridge top to valley bottom
 *								    (ft).
 *  \param coverHeight		        Tree/vegetation cover height (ft).
 *  \param windSpeedAtTwentyFeet    Wind speed at 20 ft (mi/h).
 *  \param torchingTrees		    Number of torching trees.
 *  \param treeDbh				    Tree dbh (in).
 *  \param treeHeight				Tree height (ft).
 *  \param treeSpecies			    Tree species code.
 *  \param[out] heightUsed_		    Actual tree/vegetation ht used (ft).
 *  \param[out] flameHeight			Steady state flame ht (ft).
 *  \param[out] flameRatio		    Ratio of tree height to steady flame height (ft/ft).
 *  \param[out] flameDuration_		Flame duration (min).
 *  \param[out] firebrandHeight_    Initial maximum firebrand height (ft).
 *  \param[out] flatDistance_	    Maximum spotting distance over flat terrain (mi).
 *
 *  \return Maximum spotting distance from the torching trees (mi).
 */

double Spot::calculateSpottingDistanceFromTorchingTrees(
    int    location,
    double ridgeToValleyDistance,
    double ridgeToValleyElevation,
    double coverHeight,
    double windSpeedAtTwentyFeet,
    double torchingTrees,
    double treeDBH,
    double treeHeight,
    int    treeSpecies)
{
    // Initialize return variables
    flameRatio_ = 0.0;
    flameHeightForTorchingTrees_ = 0.0;
    flameDuration_ = 0.0;
    firebrandHeightFromTorchingTrees_ = 0.0;
    flatDistanceFromTorchingTrees_ = 0.0;
    mountainDistanceFromTorchingTrees_ = 0.0;

    // Determine maximum firebrand height
    if (windSpeedAtTwentyFeet > 1e-7 && treeDBH > 1e-7 && torchingTrees >= 1.0)
    {
        // Catch species errors.
        if (treeSpecies < 0 || treeSpecies >= 14)
        {
            return(mountainDistanceFromTorchingTrees_);
        }
        // Steady flame height (ft).
        flameHeightForTorchingTrees_ = speciesFlameHeightParameters_[treeSpecies][0]
            * pow(treeDBH, speciesFlameHeightParameters_[treeSpecies][1])
            * pow(torchingTrees, 0.4);

        flameRatio_ = treeHeight / flameHeightForTorchingTrees_;
        // Steady flame duration.
        flameDuration_ = speciesFlameDurationParameters_[treeSpecies][0]
            * pow(treeDBH, speciesFlameDurationParameters_[treeSpecies][1])
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
        coverHeightUsedForTorchingTrees_ = calculateSpotCriticalCoverHeight(firebrandHeightFromTorchingTrees_, coverHeight);
        if (coverHeightUsedForTorchingTrees_ > 1e-7)
        {
            flatDistanceFromTorchingTrees_ = spotDistanceFlatTerrain(firebrandHeightFromTorchingTrees_, coverHeightUsedForTorchingTrees_, windSpeedAtTwentyFeet);
            mountainDistanceFromTorchingTrees_ = spotDistanceMountainTerrain(flatDistanceFromTorchingTrees_, location, ridgeToValleyDistance,
                ridgeToValleyElevation);
        }
    }

    return mountainDistanceFromTorchingTrees_;
}
