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


struct SpotTreeSpecies
{
    enum SpotTreeSpeciesEnum
    {
        ENGELMANN_SPRUCE = 0,    //  0 Engelmann spruce (Picea engelmannii)
        DOUGLAS_FIRE = 1,    //  1 Douglas-fir (Pseudotsuga menziessii)
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

	void setBurningPileFlameHeight(double buringPileflameHeight);
	void setDBH(double DBH);
	void setDownwindCoverHeight(double downwindCoverHeight);
	void setFlameLength(double flameLength);
	void setLocation(SpotFireLocation::SpotFireLocationEnum location);
	void setRidgeToValleyDistance(double ridgeToValleyDistance);
	void setRidgeToValleyElevation(double ridgeToValleyElevation);
	void setTorchingTrees(double torchingTrees);
	void setTreeHeight(double treeHeight);
	void setTreeSpecies(SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies);
	void setWindSpeedAtTwentyFeet(double windSpeedAtTwentyFeet);

	void updateSpotInputsForBurningPile(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double buringPileFlameHeight, 
		double windSpeedAtTwentyFeet);
	void updateSpotInputsForSurfaceFire(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double windSpeedAtTwentyFeet,
		double flameLength);
	void updateSpotInputsForTorchingTrees(SpotFireLocation::SpotFireLocationEnum location, double ridgeToValleyDistance,
		double ridgeToValleyElevation, double downwindCoverHeight, double torchingTrees, double DBH, double treeHeight,
		SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies, double windSpeedAtTwentyFeet);

	double getBurningPileFlameHeight();
	double getDBH();
	double getDownwindCoverHeight();
	double getFlameLength();
	SpotFireLocation::SpotFireLocationEnum getLocation();
	double getRidgeToValleyDistance();
	double getRidgeToValleyElevation();
	double getTorchingTrees();
	double getTreeHeight();
	SpotTreeSpecies::SpotTreeSpeciesEnum getTreeSpecies();
	double getWindSpeedAtTwentyFeet();

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
	SpotFireLocation::SpotFireLocationEnum location_;
	double ridgeToValleyDistance_;
	double ridgeToValleyElevation_;
	double windSpeedAtTwentyFeet_;
	double buringPileFlameHeight_;
	double flameLength_;
	double torchingTrees_;
	double treeHeight_;
	SpotTreeSpecies::SpotTreeSpeciesEnum treeSpecies_;
};

#endif // SPOTINPUTS_H
