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

#include "vaporPressureDeficitCalculator.h"

// Setters

void VaporPressureDeficitCalculator::setTemperature(
    double temperature, TemperatureUnits::TemperatureUnitsEnum units) {
  temperature_ = TemperatureUnits::toBaseUnits(temperature, units);
}

void VaporPressureDeficitCalculator::setRelativeHumidity(double relativeHumidity, FractionUnits::FractionUnitsEnum units) {
  relativeHumidity_ = FractionUnits::toBaseUnits(relativeHumidity, units);
}

// Run

void VaporPressureDeficitCalculator::runCalculation() {
  // Convert temperature to Celsius
  double temperatureCelsius =
      TemperatureUnits::fromBaseUnits(temperature_, TemperatureUnits::Celsius);

  // Convert temperature to Kelvin
  double temperatureKelvin = temperatureCelsius + 273.15;

  // Calculate saturation vapor pressure
  double saturationVaporPressure =
      6.1078 * std::exp((17.269 * temperatureCelsius) / temperatureKelvin);

  // Calculate actual vapor pressure
  double actualVaporPressure = relativeHumidity_ * saturationVaporPressure;

  // Calculate vapor pressure deficit
  vaporPressureDeficit_ = saturationVaporPressure - actualVaporPressure;

  // Store in base units (Pa -> hPa)
  vaporPressureDeficit_ = PressureUnits::toBaseUnits(vaporPressureDeficit_, PressureUnits::HectoPascal);
}

// Getters

double VaporPressureDeficitCalculator::getVaporPressureDeficit(PressureUnits::PressureUnitsEnum units) {
  return PressureUnits::fromBaseUnits(vaporPressureDeficit_, units);
}
