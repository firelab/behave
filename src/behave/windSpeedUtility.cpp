/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for converting among midflame, 20 foot, and 10 meter winds
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some portions of code in this file are, in part or in whole, from
*           BehavePlus5 source originally authored by Collin D. Bevins and is
*           used with or without modification.
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

#include "windSpeedUtility.h"

WindSpeedUtility::WindSpeedUtility()
{
    windSpeedAtTenMeters_ = 0;
    windSpeedAtTwentyFeet_ = 0;
    windSpeedAtMidflame_ = 0;
}

//------------------------------------------------------------------------------
/*! \brief WindSpeedAtMidflame
*
*  Dependent Variables (Outputs)
*      windSpeedAtMidflame_ (mi/h)
*
*  Independent Variables (Inputs)
*      windSpeedAtTwentyFeet (mi/h)
*      windAdjustmentFactor (fraction)
*/

double WindSpeedUtility::windSpeedAtMidflame(double windSpeedAtTwentyFeet, double windAdjustmentFactor)
{
    // Calculate results
    windSpeedAtMidflame_ = windSpeedAtTwentyFeet * windAdjustmentFactor;
    return windSpeedAtMidflame_;
}

//------------------------------------------------------------------------------
/*! \brief WindSpeedAt20Ft
*
*  Dependent Variables (Outputs)
*      windSpeedAt20Ft_ (mi/h)
*
*  Independent Variables (Inputs)
*      windSpeedAtTenMeters (mi/h)
*/

double WindSpeedUtility::windSpeedAtTwentyFeetFromTenMeter(double windSpeedAtTenMeters)
{
    // Calculate results
    double windSpeedAt20Ft_ = windSpeedAtTenMeters / 1.15;
    return windSpeedAt20Ft_;
}
