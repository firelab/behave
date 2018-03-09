/******************************************************************************
 *
 * $Id$
 *
 * Project:  Spotting distance
 * Purpose:  Compute spot fire distance from a burning pile with Behave
 * Author:   Natalie Wagenbrenner <nwagenbrenner@gmail.com> 
 *
 ******************************************************************************
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
 *****************************************************************************/
#include "behaveRun.h"
#include <iostream>
using namespace std;

#define EQUAL(a,b) (strcmp(a,b)==0)

void Usage()
{
    printf("Usage:\n");
    printf("compute_spot_distance_pile --location location --ridge_to_valley_distance distance\n");
    printf("      --ridge_to_valley_elevation elevation --downwind_cover_height height\n");
    printf("      --20ft_speed speed --flame_length length [--verbose]\n");
    printf("\n");
    printf("Returns:\n");
    printf("Spotting distance [m]\n");
    printf("\n");
    printf("Notes:\n");
    printf("All units are metric.\n");
    printf("    distance: m\n");
    printf("    height: m\n");
    printf("    speed: m/s\n");
    printf("    dbh: cm\n");
    printf("\n");
    printf("location options:\n");
    printf("    MIDSLOPE_WINDWARD\n");
    printf("    VALLEY_BOTTOM\n");
    printf("    MIDSLOPE_LEEWARD\n");
    printf("    RIDGE_TOP\n");
    printf("\n");
    printf("Example:\n");
    printf("compute_spot_distance_pile --location RIDGE_TOP --ridge_to_valley_distance 1000 ");
    printf("--ridge_to_valley_elevation 2000 --downwind_cover_height 15 --20ft_wind_speed 10 ");
    printf("--flame_length 5\n");
    printf("\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    std::string spotLocation = "!set";
    double ridgeToValleyDistance = -1.0;
    double ridgeToValleyElevation = -1.0;
    double downwindCoverHeight = -1.0;
    double windSpeedAtTwentyFeet = -1.0;
    double flameLength = -1.0;
    bool verbose = false;

    SpotFireLocation::SpotFireLocationEnum location;

    if(argc < 12){
        Usage();
    }
    
    int i = 1;
    while(i < argc)
    {
        if(EQUAL(argv[i], "--location"))
        {
            spotLocation = argv[++i];
            if(EQUAL(spotLocation.c_str(), "RIDGE_TOP"))
            {
                location = SpotFireLocation::RIDGE_TOP;
            }
            else if(EQUAL(spotLocation.c_str(), "MIDSLOPE_WINDWARD"))
            {
                location = SpotFireLocation::MIDSLOPE_WINDWARD;
            }
            else if(EQUAL(spotLocation.c_str(), "VALLEY_BOTTOM"))
            {
                location = SpotFireLocation::VALLEY_BOTTOM;
            }
            else if(EQUAL(spotLocation.c_str(), "MIDSLOPE_LEEWARD"))
            {
                location = SpotFireLocation::MIDSLOPE_LEEWARD;
            }
            else
            {
                cout<<"location option "<<argv[i]<<" not valid."<<endl;
                exit(-1);
            }
        }
        else if(EQUAL(argv[i], "--ridge_to_valley_distance"))
        {
            ridgeToValleyDistance = atof(argv[++i]);
        }
        else if(EQUAL(argv[i], "--ridge_to_valley_elevation"))
        {
            ridgeToValleyElevation = atof(argv[++i]);
        }
        else if(EQUAL(argv[i], "--downwind_cover_height"))
        {
           downwindCoverHeight  = atof(argv[++i]);
        }
        else if(EQUAL(argv[i], "--20ft_wind_speed"))
        {
            windSpeedAtTwentyFeet = atof(argv[++i]);
        }
        else if(EQUAL(argv[i], "--flame_length"))
        {
            flameLength = atof(argv[++i]);
        }
        else if(EQUAL(argv[i], "--verbose"))
        {
            verbose = true;
        }
        else
        {
            Usage();
        }
        i++;
    }

    if(verbose){
        cout<<"\nlocation                  = "<<spotLocation<<endl;
        cout<<"ridge_to_valley           = "<<ridgeToValleyDistance<<" m"<<endl;
        cout<<"ridge_to_valley_elevation = "<<ridgeToValleyElevation<<" m"<<endl;
        cout<<"downwind_cover_height     = "<<downwindCoverHeight<<" m"<<endl;
        cout<<"20ft_wind_speed           = "<<windSpeedAtTwentyFeet<<" m/s"<<endl;
        cout<<"flame_length              = "<<flameLength<<" m"<<endl;
    }

    Spot spot;

    double mountainSpottingDistance = 0.0;
    double flatSpottingDistance = 0.0;

    //set units
    LengthUnits::LengthUnitsEnum ridgeToValleyDistanceUnits = LengthUnits::Meters;
    LengthUnits::LengthUnitsEnum elevationUnits = LengthUnits::Meters;
    LengthUnits::LengthUnitsEnum coverHeightUnits = LengthUnits::Meters;
    SpeedUnits::SpeedUnitsEnum windSpeedUnits = SpeedUnits::MetersPerSecond;
    LengthUnits::LengthUnitsEnum flameLengthUnits = LengthUnits::Meters;
    LengthUnits::LengthUnitsEnum spottingDistanceUnits = LengthUnits::Meters;

    //spotting distance from a burning pile
    spot.updateSpotInputsForBurningPile(location, ridgeToValleyDistance,
            ridgeToValleyDistanceUnits, ridgeToValleyElevation, elevationUnits,
            downwindCoverHeight, coverHeightUnits, flameLength, flameLengthUnits,
            windSpeedAtTwentyFeet, windSpeedUnits);

    spot.calculateSpottingDistanceFromBurningPile();

    mountainSpottingDistance = spot.getMaxMountainousTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits);
    flatSpottingDistance = spot.getMaxFlatTerrainSpottingDistanceFromBurningPile(spottingDistanceUnits);

    cout<<"\nmountainSpottingDistance = "<<mountainSpottingDistance<<" m"<<endl;
    cout<<"flatSpottingDistance = "<<flatSpottingDistance<<" m\n"<<endl;

    return 0;
}
