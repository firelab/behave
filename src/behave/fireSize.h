/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating various properties related to fire size
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

#ifndef FIRESIZE_H
#define FIRESIZE_H

#include "behaveUnits.h"

class FireSize
{
public:
    FireSize();
    ~FireSize();
    void calculateFireBasicDimensions(double effectiveWindSpeed, SpeedUnits::SpeedUnitsEnum windSpeedRateUnits, double forwardSpreadRate, SpeedUnits::SpeedUnitsEnum spreadRateUnits);

    double getFireLengthToWidthRatio() const;
    double getEccentricity() const;
    double getBackingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const;
    double getEllipticalA(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getEllipticalB(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getEllipticalC(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getFireLength(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getMaxFireWidth(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;

    double getFirePerimeter(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;
    double getFireArea(AreaUnits::AreaUnitsEnum areaUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const;

private:
    void calculateFireLengthToWidthRatio();
    void calculateSurfaceFireEccentricity();
    void calculateEllipticalDimensions();
    void calculateBackingSpreadRate();

    // Inputs
    double effectiveWindSpeed_; // internally stored in mph
    double forwardSpreadRate_; // internally stored in ft/min
    double elapsedTime_; // internally stored in minutes

    // Outputs
    double ellipticalA_; // semi-minor axis of fire ellipse
    double ellipticalB_; // semi-major axis of fire ellipse
    double ellipticalC_; // distance from center of ellipse to a focus
    double eccentricity_; // measure of deviance from perfect circle, ranges from [0,1)
    double forwardSpreadDistance_;
    double backingSpreadRate_;
    double backingSpreadDistance_;
    double fireLengthToWidthRatio_;

    double part_;              // Intermediate variable used to calculate Alexandar 1985 ratio
    double hb_ratio_;          // Alexander 1985 heading/backing ratio
};

#endif // FIRESIZE_H
