#ifndef CROWNENUMS_H
#define CROWNENUMS_H

struct FireType
{
    enum FireTypeEnum
    {
        SURFACE = 0,    // surface fire with no torching or crown fire spread.
        TORCHING = 1,   // surface fire with torching.
        CONDITIONAL_CROWN_FIRE = 2, // active crown fire possible if the fire transitions to the overstory        
        CROWNING = 3    // active crown fire, fire is spreading through the canopy.
    };
};

#endif // CROWNENUMS_H
