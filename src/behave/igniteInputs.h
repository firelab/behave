/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for handling the inputs required for Behave's Ignite module
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

#ifndef IGNITEINPUTS_H
#define IGNITEINPUTS_H

#include "behaveUnits.h"

struct IgnitionFuelBedType
{
    enum IgnitionFuelBedTypeEnum
    {
        PONDEROSA_PINE_LITTER = 0, //Ponderosa Pine Litter
        PUNKY_WOOD_ROTTEN_CHUNKY = 1, // Punky wood, rotten, chunky
        PUNKY_WOOD_POWDER_DEEP = 2, // Punky wood powder, deep (4.8 cm)
        PUNK_WOOD_POWDER_SHALLOW = 3, // Punk wood powder, shallow (2.4 cm)
        LODGEPOLE_PINE_DUFF = 4, // Lodgepole pine duff
        DOUGLAS_FIR_DUFF = 5, //  Douglas - fir duff
        HIGH_ALTITUDE_MIXED = 6, // High altitude mixed (mainly Engelmann spruce)
        PEAT_MOSS = 7  // Peat moss (commercial)
    };
};

struct LightningCharge
{
    enum LightningChargeEnum
    {
        NEGATIVE = 0,
        POSITIVE = 1,
        UNKNOWN = 2
    };
};

class IgniteInputs
{
public:
    IgniteInputs();
    ~IgniteInputs();

private:
 
};

#endif // IGNITEINPUTS_H
