/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating surface fire reaction intensity
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

    // Set up torchArrayB_
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

//------------------------------------------------------------------------------
/*! \brief Calculates cover height used in spotting distance calculations.
 *
 *  \param firebrandHeight   Maximum firebrand height.
 *  \param coverHeight       Tree/vegetation cover height(ft).
 *
 *  \return Cover ht used in calculation of flat terrain spotting distance.
 */

double Spot::spotCriticalCoverHeight(double firebrandHeight, double coverHeight)
{
    // Minimum value of coverHeight used to calculate flatDistance
    // using log variation with ht.
    double criticalHeight = (firebrandHeight < 1e-7)
        ? ( 0.0 )
        : (2.2 * pow(firebrandHeight, 0.337) - 4.0);
    
    // Cover height used in calculation of flatDistance.
    coverHeightUsed_ = (coverHeight > criticalHeight)
        ? (coverHeight)
        : ( criticalHeight );
    return coverHeightUsed_;
}

//-------------------------------------------------------------------------------------
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
 *  \return mountainDistance_		Maximum spotting distance from the torching trees 
 *							        (mi).
 */

double Spot::spotDistanceMountainTerrain(
	double flatDistance,
	int    location,
	double ridgeToValleyDistance,
	double ridgeToValleyElevation )
{
    mountainDistance_ = flatDistance;
    if ( ridgeToValleyElevation > 1e-7 && ridgeToValleyDistance > 1e-7 )
    {
        double a1 = flatDistance / ridgeToValleyDistance;
        double b1 = ridgeToValleyElevation / ( 10.0 * M_PI ) / 1000.0;
        double x = a1;
        for ( int i = 0; i < 6; i++ )
        {
            x = a1 - b1 * ( cos( M_PI * x - location * M_PI / 2.0 )
              - cos( location * M_PI / 2.0 ) );
        }
        mountainDistance_ = x * ridgeToValleyDistance;
    }
    return mountainDistance_;
}

//-------------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance over flat terrain.
 *
 *  \param firebrandHeight          Maximum firebrand height (ft).
 *  \param coverHeightt             Tree/vegetation cover height (ft).
 *  \param windSpeedAtTwentyFeet    Wind speed at 20 ft (mi/h).
 *
 *  \return flatDistance_			Maximum spotting distance over flat terrain.
 */

double Spot::spotDistanceFlatTerrain(
	double firebrandHeight,
	double coverHeight,
	double windSpeedAtTwentyFeet )
{
    // Flat terrain spotting distance.
    flatDistance_ = 0.0;
    if ( coverHeight > 1e-7 )
    {
        flatDistance_ = 0.000718 * windSpeedAtTwentyFeet * sqrt(coverHeight)
                 * (0.362 + sqrt( firebrandHeight / coverHeight ) / 2.0
                 * log( firebrandHeight / coverHeight ) );
    }
    return flatDistance_;
}

//-------------------------------------------------------------------------------------
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

double Spot::spotDistanceFromSurfaceFire(
	int    location,
	double ridgeToValleyDistance,
	double ridgeToValleyElevation,
	double coverHeight,
	double windSpeedAtTwentyFeet,
	double flameLength)
{
    // Initialize return variables
    double firebrandHeight      = 0.0;
    double height               = 0.0;
    double localflatDistance    = 0.0;
    double mountainDistance     = 0.0;
    double drift                = 0.0;

    // Determine maximum firebrand height
    if ((windSpeedAtTwentyFeet) > 1e-7 && (flameLength > 1e-7))
    {
        // f is function relating thermal energy to windspeed.
        double f = 322. * pow((0.474 * windSpeedAtTwentyFeet), -1.01);

        // Byram's fireline intensity is derived back from flame length.
        double byrams = pow( ( flameLength / .45 ), ( 1. / 0.46 ) );

        // Initial firebrand height (ft).
        firebrandHeight = ((f * byrams) < 1e-7)
             ? ( 0.0 )
             : ( 1.055 * sqrt( f * byrams ) );

        // Cover height used in calculation of localflatDistance.
        if ((height = spotCriticalCoverHeight(firebrandHeight, coverHeight)) > 1e-7)
        {
            drift = 0.000278 * windSpeedAtTwentyFeet * pow(firebrandHeight, 0.643);
            localflatDistance = spotDistanceFlatTerrain(firebrandHeight, height, windSpeedAtTwentyFeet) + drift;
            mountainDistance = spotDistanceMountainTerrain(localflatDistance,
                            location, ridgeToValleyDistance, ridgeToValleyElevation );
        }
    }

    // Store return values and return max spot distance over mountainous terrain
    coverHeightUsed_ = height;
    firebrandHeight_ = firebrandHeight;
    firebrandDrift_ = drift;
    flatDistance_ = localflatDistance;
 
    return mountainDistance;
}

//------------------------------------------------------------------------------
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
 *  \param[out] flatDistance_		Maximum spotting distance over flat terrain (mi).
 *
 *  \return Maximum spotting distance from the burning pile (mi).
 */

double Spot::spotDistanceFromBurningPile(
	int    location,
	double ridgeToValleyDistance,
	double ridgeToValleyElevation,
	double coverHeight,
	double windSpeedAtTwentyFeet,
	double flameHeight)
{
    // Initialize return values
    double firebrandHeight      = 0.0;
    double height               = 0.0;
    double localflatDistance    = 0.0;
    double mountainDistance     = 0.0;

    // Determine maximum firebrand height
    if ((windSpeedAtTwentyFeet > 1e-7) && (flameHeight > 1e-7))
    {
        // Determine maximum firebrand height
        firebrandHeight = 12.2 * flameHeight;

        // Cover ht used in calculation of flatDist.
        if ((height = spotCriticalCoverHeight(firebrandHeight, coverHeight)) > 1e-7)
        {
            // Flat terrain spotting distance.
            localflatDistance = 0.000718 * windSpeedAtTwentyFeet * sqrt(height)
                * (0.362 + sqrt(firebrandHeight / height) / 2.0
                * log(firebrandHeight / height));
            // Adjust for mountainous terrain.
            mountainDistance = spotDistanceMountainTerrain(localflatDistance,
                location, ridgeToValleyDistance, ridgeToValleyElevation);
        } // if ht > 1e-7
    } // if windSpeedAt20Ft > 1e-7 && z > 1e-7

    // Store return values and return max spot distance over mountainous terrain
    coverHeightUsed_ = height;
    firebrandHeight_ = firebrandHeight;
    flatDistance_ = localflatDistance;
   
    return mountainDistance;
}

//------------------------------------------------------------------------------
/*! \brief Calculates maximum spotting distance from a group of torching trees.
 *
 *  \param location				Location of the burning pile:
 *									0 == midslope, windward
 *									1 == valley bottom
 *									2 == midslope, leeward
 *									3 == ridge top
 *  \param ridgeToValleyDist	Horizontal distance from ridge top to valley bottom
 *								(mi).
 *  \param ridgeToValleyElev	Vertical distance from ridge top to valley bottom
 *								(ft).
 *  \param coverHt				Tree/vegetation cover height (ft).
 *  \param windSpeedAt20Ft		Wind speed at 20 ft (mi/h).
 *  \param torchingTrees		Number of torching trees.
 *  \param treeDbh				Tree dbh (in).
 *  \param treeHt				Tree height (ft).
 *  \param treeSpecies			Tree species code.
 *  \param[out] heightUsed_		Actual tree/vegetation ht used (ft).
 *  \param[out] flameHt			Steady state flame ht (ft).
 *  \param[out] flameRatio		Ratio of tree height to steady flame height (ft/ft).
 *  \param[out] flameDur		Flame duration (min).
 *  \param[out] firebrandHt		Initial maximum firebrand height (ft).
 *  \param[out] flatDistance_	Maximum spotting distance over flat terrain (mi).
 *
 *  \return Maximum spotting distance from the torching trees (mi).
 */

double Spot::spotDistanceFromTorchingTrees(
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
    // Initialize potential return variables
    double ratio                = 0.0;
    double steadyFlameHeight    = 0.0;
    double duration             = 0.0;
    double firebrandHeight      = 0.0;
    double height               = 0.0;
    double localFlatDistance    = 0.0;
    double mountainDistance     = 0.0;

    // Determine maximum firebrand height
    if (windSpeedAtTwentyFeet > 1e-7 && treeDBH > 1e-7 && torchingTrees >= 1.0 )
    {
        // Catch species errors.
        if ( treeSpecies < 0 || treeSpecies >= 14 )
        {
            return(mountainDistance);
        }
        // Steady flame height (ft).
        steadyFlameHeight = speciesFlameHeightParameters_[treeSpecies][0]
            * pow(treeDBH, speciesFlameHeightParameters_[treeSpecies][1])
            * pow(torchingTrees, 0.4);

        ratio = treeHeight / steadyFlameHeight;
        // Steady flame duration.
        duration = speciesFlameDurationParameters_[treeSpecies][0]
            * pow(treeDBH, speciesFlameDurationParameters_[treeSpecies][1])
            * pow(torchingTrees, -0.2);

        int j;
        if (ratio >= 1.0)
        {
            j = 0;
        }
        else if (ratio >= 0.5)
        {
            j = 1;
        }
        else if (duration < 3.5)
        {
            j = 2;
        }
        else
        {
            j = 3;
        }

        // Initial firebrand height (ft).
        firebrandHeight = firebrandHeightFactors_[j][0] * pow(duration, firebrandHeightFactors_[j][1]) * steadyFlameHeight + treeHeight / 2.0;

        // Cover ht used in calculation of flatDist.
        if ((height = spotCriticalCoverHeight(firebrandHeight, coverHeight)) > 1e-7)
        {
            localFlatDistance = spotDistanceFlatTerrain(firebrandHeight, height, windSpeedAtTwentyFeet);
            mountainDistance = spotDistanceMountainTerrain(localFlatDistance, location, ridgeToValleyDistance, 
                ridgeToValleyElevation );
        }
    }

    // Store return values and return max spot distance over mountainous terrain
    coverHeightUsed_ = height;
    flameHeight_ = steadyFlameHeight;
    flameRatio_ = ratio;
    flameDuration_ = duration;
    firebrandHeight_ = firebrandHeight;
    flatDistance_ = localFlatDistance;
    
    return mountainDistance;
}
