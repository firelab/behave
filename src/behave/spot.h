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

#ifndef SPOT_HEADER
#define SPOT_HEADER

class Spot
{
public:
    Spot();
    ~Spot();

    double spotCriticalCoverHeight(double firebrandHeight, double coverHeightt);
    double spotDistanceMountainTerrain(double flatDistance, int location, double ridgeToValleyDistance, 
        double ridgeToValleyElevation);
    double spotDistanceFlatTerrain(double firebrandHeight, double coverHeightt, double windSpeedAtTwentyFeet);
    double spotDistanceFromSurfaceFire(int location, double ridgeToValleyDistance, double ridgeToValleyElevation,
        double coverHeight, double windSpeedAtTwentyFeet, double flameLength);
    double spotDistanceFromBurningPile(int location, double ridgeToValleyDistance, double ridgeToValleyElevation,
        double coverHeight, double windSpeedAtTwentyFeet, double flameHeight);
    double spotDistanceFromTorchingTrees(int location, double ridgeToValleyDistance, double ridgeToValleyElevation,
        double coverHeight, double windSpeedAtTwentyFeet, double torchingTrees, double treeDBH, double treeHeight,
        int treeSpecies);

    double getCoverHeightUsed();
    double getFlameHeight();
    double getFlameRatio();
    double getFlameDuration();
    double getMaxFirebrandHeight();
    double getMaxFlatTerrainSpottingDistance();
    double getMaxMountainTerrainSpottingDistance();   

private:
    static const int NUM_SPECIES = 14;
    static const int NUM_COLS = 2;
    static const int NUM_FIREBRAND_ROWS = 4;

    double speciesFlameHeightParameters_[NUM_SPECIES][NUM_COLS];
    double speciesFlameDurationParameters_[NUM_SPECIES][NUM_COLS];
    double firebrandHeightFactors_[NUM_FIREBRAND_ROWS][NUM_COLS];

    double coverHeightUsed_;    // Actual tree / vegetation ht used(ft).
    double flameHeight_;        // Steady state flame ht(ft).
    double flameRatio_;         // Ratio of tree height to steady flame height(ft / ft).
    double firebrandDrift_;     // Maximum firebrand drift(mi).
    double flameDuration_;      // Flame duration(dimensionless).
    double firebrandHeight_;    // Initial maximum firebrand height(ft).
    double flatDistance_;       // Maximum spotting distance over flat terrain(mi).
    double mountainDistance_;   // Maximum spotting distance over mountain terrain(mi).
};

#endif // SPOT_HEADER
