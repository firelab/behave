#ifndef CANOPY_TRIANGLE_DISTRIBUTION_H
#define CANOPY_TRIANGLE_DISTRIBUTION_H

#include <math.h>
#include <cstring>
//#include <plstream.h>
#include "canopy.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Class that stores canopy density, etc. information for a triangle distribution.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class canopy_triangle_distribution : public canopy
{

public:
    canopy_triangle_distribution();
    canopy_triangle_distribution(double A1_, double Ax_, double Ab_, double zmax_, double zbot_);
    canopy_triangle_distribution(canopy_triangle_distribution &rhs);
    canopy_triangle_distribution &operator=(const canopy_triangle_distribution &rhs);
    ~canopy_triangle_distribution();

    //inputs
    double A1;      //density of top
    double Ax;      //density at max point
    double Ab;      //density of bottom (trunk space)
    double zmax;    //height to Ax
    double zbot;    //height to bottom of triangular part

    //      Diagram of above parameters
    //
    //      ----->|  \<----- A1
    //            |   \
    //            |    \
    //            |     \
    //            |      \
    //            |       \
    //            |        \
    //  ^   ----->|        /<----- Ax
    //  |         |       /
    //  |         |      /
    //  |         |     /
    // zmax       |    /
    //  |         |   /
    //  |         |  /
    //  |     ^   | |
    //  |     |   | |
    //  |     |   | |
    //  |   ----->| |<----- Ab
    //  |     |   | |
    //  |     |   | |
    //  |    zbot | |
    //  |     |   | |


protected:
    void compute_haz();
};

#endif // CANOPY_TRIANGLE_DISTRIBUTION_H
