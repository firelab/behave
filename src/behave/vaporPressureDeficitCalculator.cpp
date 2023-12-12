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
  double temperatureKelvin = temperatureCelsius + 237.3;

  // Calculate saturated vapor pressure
  double saturatedVaporPressure = 6.11 * std::pow(10, ((7.5 * temperatureCelsius) / temperatureKelvin));

  // Store saturated vapor pressure
  saturatedVaporPressure_ = PressureUnits::toBaseUnits(saturatedVaporPressure, PressureUnits::HectoPascal);

  // Calculate actual vapor pressure
  double actualVaporPressure = relativeHumidity_ * saturatedVaporPressure;

  // Store actual vapor pressure
  actualVaporPressure_ = PressureUnits::toBaseUnits(actualVaporPressure, PressureUnits::HectoPascal);

  // Calculate vapor pressure deficit
  vaporPressureDeficit_ = saturatedVaporPressure - actualVaporPressure;

  // Store in base units (hPa -> Pa)
  vaporPressureDeficit_ = PressureUnits::toBaseUnits(vaporPressureDeficit_, PressureUnits::HectoPascal);
}

// Getters

double VaporPressureDeficitCalculator::getVaporPressureDeficit(PressureUnits::PressureUnitsEnum units) {
  return PressureUnits::fromBaseUnits(vaporPressureDeficit_, units);
}

double VaporPressureDeficitCalculator::getActualVaporPressure(PressureUnits::PressureUnitsEnum units) {
  return PressureUnits::fromBaseUnits(actualVaporPressure_, units);
}

double VaporPressureDeficitCalculator::getSaturatedVaporPressure(PressureUnits::PressureUnitsEnum units) {
  return PressureUnits::fromBaseUnits(saturatedVaporPressure_, units);
}
