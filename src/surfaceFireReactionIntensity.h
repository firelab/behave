#ifndef SURFACEFIREREACTIONINTENSITY_HEADER
#define SURFACEFIREREACTIONINTENSITY_HEADER

#include "surfaceFuelbedIntermediates.h"

// Moved to SurfaceFuelbedIntermediates for now
class SurfaceFireReactionIntensity
{
public:
    // Class constants
    static const int	MAX_LIFE_STATES = 2;	//< Life state, live or dead

    SurfaceFireReactionIntensity(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates);
    double calculateReactionIntensity();
    void calculateEtaM();
    void calculateEtaS();
    double getReactionIntensity() const;

private:
    const double SMIDGEN = 1.0e-07;							//< Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc.
    const int	DEAD = 0;									//< Dead life state index
    const int	LIVE = 1;									//< Live life state index

    double etaM_[MAX_LIFE_STATES];							//< Moisture damping coefficient for  i-th categort (dead/live)
    double etaS_[MAX_LIFE_STATES];							//< Mineral(silica) damping coefficient for i - th categort(dead / live)
    double reactionIntensityForLifeState_[MAX_LIFE_STATES];	//< Reaction intensity for i-th category (dead/live)
    double reactionIntensity_;								//< Reaction Intensity, Rothermel 1972, equation 27

    const SurfaceFuelbedIntermediates* surfaceFuelbedIntermediates_;

};

#endif //SURFACEFIREREACTIONINTENSITY_HEADER