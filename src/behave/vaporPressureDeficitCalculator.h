/******************************************************************************
 *
 * Project:  Code Blocks
 * Author:   Richard Sheperd <rsheperd@sig-gis.com>
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

#include "behaveUnits.h"
#include <cmath>

/** Calculator for Vapor Pressure Deficit */
class VaporPressureDeficitCalculator {

public:
  void runCalculation();
  void setTemperature(double temperature, TemperatureUnits::TemperatureUnitsEnum);
  void setRelativeHumidity(double relativeHumidity, FractionUnits::FractionUnitsEnum units);
  double getVaporPressureDeficit(PressureUnits::PressureUnitsEnum units);

private:
  double temperature_;          //<! Temparature is expected to be degrees Celsius
  double relativeHumidity_;     //<! Relative Humidity is expected to be a fraction
  double vaporPressureDeficit_; //<! Vapor Pressure Deficit is in kPa
};
