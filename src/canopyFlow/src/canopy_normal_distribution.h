#ifndef CANOPY_NORM_H
#define CANOPY_NORM_H

#include <math.h>
#include <cstring>
//#include <plstream.h>
#include "canopy.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Class that stores canopy density, etc. information for a normal distribution.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class canopy_normal_distribution : public canopy
{

public:
    canopy_normal_distribution();
    canopy_normal_distribution(double heightMaxFoliageDist_, double standardDevFoliageDist_);
    canopy_normal_distribution(canopy_normal_distribution &rhs);
    canopy_normal_distribution &operator=(const canopy_normal_distribution &rhs);
    ~canopy_normal_distribution();

    //inputs
    double heightMaxFoliageDist;    //height of maximum foliage distribution for the normal distribution (m)
    double standardDevFoliageDist;  //standard deviation of foliage distribution for the normal distribution (m)

protected:
    void compute_haz();
};

#endif // CANOPY_NORM_H
