#ifndef MEASURED_DISTRIBUTION_H
#define MEASURED_DISTRIBUTION_H

#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <cstring>
#include <string>
//#include <plstream.h>
#include <stdio.h>
#include "canopy.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Class that stores canopy density, etc. information for a measured distribution.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class measured_distribution : public canopy
{

public:
    measured_distribution();
    measured_distribution(std::string filename);
    measured_distribution(measured_distribution &rhs);
    measured_distribution &operator=(const measured_distribution &rhs);
    ~measured_distribution();

protected:
    void compute_haz();

    double* fodist;
    int n_fodist;
    double* zh;
    int n_zh;
};

#endif // MEASURED_DISTRIBUTION_H
