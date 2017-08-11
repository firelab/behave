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

class FireSize
{
public:
    FireSize();
    ~FireSize();
    void calculateFireDimensions(double effectiveWindSpeed, double forwardSpreadRate, double elapsedTime);

    double getFireLengthToWidthRatio() const;
    double getEccentricity() const;
    double getBackingSpreadRate() const;
    double getEllipticalA() const;
    double getEllipticalB() const;
    double getEllipticalC() const;
    double getFirePerimeter() const;

private:
    void calculateFireLengthToWidthRatio();
    void calculateSurfaceFireEccentricity();
    void calculateEllipticalDimensions();
    void calculateBackingSpreadRate();
    void calculateFirePerimeter();

    // Inputs
    double effectiveWindSpeed_;
    double forwardSpreadRate_;
    double elapsedTime_;

    // Outputs
    double ellipticalA_;
    double ellipticalB_;
    double ellipticalC_;
    double eccentricity_;
    double forwardSpreadDistance_;
    double backingSpreadRate_;
    double backingSpreadDistance_;
    double fireLengthToWidthRatio_;
    double area_;
    double perimeter_;
    double fireLength_;
    double maximumFireWidth_;
};

#endif // FIRESIZE_H
