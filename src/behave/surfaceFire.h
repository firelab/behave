/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating values associated with surface fires used
*           in Rothermel Model
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in the corresponding cpp file is, in part or in
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

#ifndef SURFACEFIRE_H
#define SURFACEFIRE_H

#include "fireSize.h"
#include "surfaceFireReactionIntensity.h"
#include "surfaceFuelbedIntermediates.h"

class SurfaceFire
{
    friend class SurfaceTwoFuelModels; // to keep setters for outputs out of public interface
public:
    SurfaceFire();
    SurfaceFire(const SurfaceFire& rhs);
    SurfaceFire& operator=(const SurfaceFire& rhs);
    SurfaceFire(const FuelModelSet& fuelModelSet, const SurfaceInputs& surfaceInputs, FireSize& size);
    double calculateNoWindNoSlopeSpreadRate(double reactionIntensity, double propagatingFlux, double heatSink);
    double calculateForwardSpreadRate(int fuelModelNumber, bool hasDirectionOfInterest = false, 
        double directionOfInterest = -1.0);
    double calculateSpreadRateAtVector(double directionOfInterest);
    double calculateFlameLength(double firelineIntensity);
   
    void initializeMembers();
    void skipCalculationForZeroLoad();

    // Public getters
    double getFuelbedDepth() const;
    double getSpreadRate() const;
    double getSpreadRateInDirectionOfInterest() const;
    double getDirectionOfMaxSpread() const;
    double getEffectiveWindSpeed() const;
    double getFirelineIntensity() const;
    double getFlameLength() const;
    double getMaxFlameLength() const;
    double getFireLengthToWidthRatio() const;
    double getFireEccentricity() const;
    double getHeatPerUnitArea() const;
    double getResidenceTime() const;
    double getWindSpeedLimit() const;
    double getReactionIntensity() const;
    double getMidflameWindSpeed() const;

    double getWindAdjustmentFactor() const;
    bool getIsWindLimitExceeded() const;
 
    void calculateMidflameWindSpeed();

protected:
    // Protected setters accessible to friend classes
    void setDirectionOfMaxSpread(double directionOFMaxSpread);
    void setEffectiveWindSpeed(double effectiveWindSpeed);
    void setFirelineIntensity(double firelineIntensity);
    void setFlameLength(double flameLength);
    void setFireLengthToWidthRatio(double lengthToWidthRatio);
    void setResidenceTime(double residenceTime);
    void setWindSpeedLimit(double windSpeedLimit);
    void setReactionIntensity(double reactionIntensity);
    void setHeatPerUnitArea(double heatPerUnitArea);
    void setIsWindLimitExceeded(bool isWindLimitExceeded);
    void setWindAdjustmentFactor(double windAdjustmentFactor);
    void setMidflameWindSpeed(double midflameWindSpeed);

private:
    void memberwiseCopyAssignment(const SurfaceFire& rhs);
    void calculateHeatPerUnitArea();
    void calculateWindAdjustmentFactor();
    void calculateWindFactor();
    void calculateSlopeFactor();

    void calculateResidenceTime();
    void calculateFireFirelineIntensity(double forwardSpreadRate);
    void calculateFlameLength();
    void calculateWindSpeedLimit();
    void calculateDirectionOfMaxSpread();
 
    void calculateEffectiveWindSpeed();
    void applyWindSpeedLimit();
    double convertDirectionOfSpreadToRelativeToNorth(double directionOfMaxSpreadFromUpslope) const;

    // Pointers and references to other objects
    const FuelModelSet* fuelModelSet_;
    const SurfaceInputs* surfaceInputs_;
    FireSize* size_;
    SurfaceFuelbedIntermediates surfaceFuelbedIntermediates_;
    SurfaceFireReactionIntensity surfaceFireReactionIntensity_;
  
    // Member variables
    bool isWindLimitExceeded_;
    double directionOfInterest_;
    double effectiveWindSpeed_;
    double windSpeedLimit_;
    double phiS_;											// Slope factor, Rothermel 1972, equation 51
    double phiW_;											// Wind factor, Rothermel 1972, equation 47
    double windB_;											// Rothermel 1972, Equation 49
    double windC_;											// Rothermel 1972, Equation 48
    double windE_;											// Rothermel 1972, Equation 50
    double directionOfMaxSpread_;							// Direction of max fire spread in degrees clockwise from upslope
    double noWindNoSlopeSpreadRate_;						// No-wind-no-slope fire spread rate, Rothermel 1972, equation 52
    double forwardSpreadRate_;								// Maximum rate of fire spread rate, Rothermel 1972, equation 52
    double spreadRateInDirectionOfInterest_;				// spreadRateInDirectionOfInterest
    double heatPerUnitArea_;                                // Heat per unit area (Btu/ft^2)
    double fireLengthToWidthRatio_;
    double residenceTime_;
    double reactionIntensity_;
    double firelineIntensity_;
    double maxFlameLength_;                                 // Flame length computed from spread rate in max direction, used in SAFETY
    double flameLength_;
    double backingSpreadRate_;
   
    // Elliptical dimensions
    double eccentricity_;   // deviation from a perfect circle being zero in the case of a perfect circle, unitless real in interval [0,1) 
    double ellipticalA_;    // semiminor axis, commonly refered to as 'b' in mathematics and 'a' in fire literature (ch)
    double ellipticalB_;    // semimajor axis, commonly refered to as 'a' in mathematics and 'b' in fire literature (ch)
    double ellipticalC_;    // distance from center of ellipse to one of its foci, commonly referered to a 'c' in mathematics (ch)

    double midflameWindSpeed_; 
    double windAdjustmentFactor_;
    WindAdjustmentFactorShelterMethod::WindAdjustmentFactorShelterMethodEnum windAdjustmentFactorShelterMethod_;
    WindAdjustmentFactorCalculationMethod::WindAdjustmentFactorCalculationMethodEnum windAdjustmentFactorCalculationMethod_;
    double canopyCrownFraction_;

    double aspenMortality_;
};

#endif // SURFACEFIRE_H
