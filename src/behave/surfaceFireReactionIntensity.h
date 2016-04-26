#ifndef SURFACEFIREREACTIONINTENSITY_HEADER
#define SURFACEFIREREACTIONINTENSITY_HEADER

#include "surfaceEnums.h"

class SurfaceFuelbedIntermediates;

class SurfaceFireReactionIntensity
{
public:
    SurfaceFireReactionIntensity();
    SurfaceFireReactionIntensity(const SurfaceFireReactionIntensity &rhs);
    SurfaceFireReactionIntensity& operator= (const SurfaceFireReactionIntensity& rhs);
    SurfaceFireReactionIntensity(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates);

    double calculateReactionIntensity();
    void calculateEtaM();
    void calculateEtaS();
    double getReactionIntensity() const;

private:
    double etaM_[FuelConstants::MAX_LIFE_STATES];                           //< Moisture damping coefficient for  i-th categort (dead/live)
    double etaS_[FuelConstants::MAX_LIFE_STATES];                           //< Mineral(silica) damping coefficient for i - th categort(dead / live)
    double reactionIntensityForLifeState_[FuelConstants::MAX_LIFE_STATES];  //< Reaction intensity for i-th category (dead/live)
    double reactionIntensity_;                                              //< Reaction Intensity, Rothermel 1972, equation 27

    const SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;

};

#endif //SURFACEFIREREACTIONINTENSITY_HEADER
