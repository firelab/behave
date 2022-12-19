/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating surface fire reaction intensity
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

#ifndef SURFACEFIREREACTIONINTENSITY_H
#define SURFACEFIREREACTIONINTENSITY_H

#include "surfaceInputs.h"

class SurfaceFuelbedIntermediates;

class SurfaceFireReactionIntensity
{
public:
    SurfaceFireReactionIntensity();
    SurfaceFireReactionIntensity(const SurfaceFireReactionIntensity& rhs);
    SurfaceFireReactionIntensity& operator=(const SurfaceFireReactionIntensity& rhs);
    SurfaceFireReactionIntensity(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates);

    double calculateReactionIntensity();
    void calculateEtaM();
    void calculateEtaS();
    double getReactionIntensity(HeatSourceAndReactionIntensityUnits::HeatSourceAndReactionIntensityUnitsEnum reactiontionIntensityUnits) const;

    double getetaMDead() const;
    double getetaMLive() const;
    double getWeightedMoistureDead() const;
    double getWeightedMoistureLive() const;
    double getMoistureExtinctionDead() const;
    double getMoistureExtinctionLive() const;

private:
    void memberwiseCopyAssignment(const SurfaceFireReactionIntensity& rhs);

    double etaM_[FuelConstants::MAX_LIFE_STATES];                            // Moisture damping coefficient for  i-th categort (dead/live)
    double etaS_[FuelConstants::MAX_LIFE_STATES];                            // Mineral(silica) damping coefficient for i - th categort(dead / live)
    double reactionIntensityForLifeState_[FuelConstants::MAX_LIFE_STATES];   // Reaction intensity for i-th category (dead/live)
    double reactionIntensity_;                                                              // Reaction Intensity, Rothermel 1972, equation 27 (Btu/ft2/min)

    double MoE_Dead_;
    double MoE_Live_;
    double weightedMoistureDead_;
    double weightedMoistureLive_;

    const SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;

};

#endif // SURFACEFIREREACTIONINTENSITY_H
