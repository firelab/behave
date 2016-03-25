#ifndef MASSMAN_DISTRIBUTION_H
#define MASSMAN_DISTRIBUTION_H

#include <math.h>
#include <cstring>
//#include <plstream.h>
#include "canopy.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Class that stores canopy density, etc. information for a Massman distribution.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class massman_distribution : public canopy
{

public:
    massman_distribution();
    massman_distribution(double A1_, double A2_, double A3_, double zmax_);
    massman_distribution(massman_distribution &rhs);
    massman_distribution &operator=(const massman_distribution &rhs);
    ~massman_distribution();

    //inputs
    double A1;
    double A2;
    double A3;
    double zmax;

protected:
    void compute_haz();
};

#endif // MASSMAN_DISTRIBUTION_H
