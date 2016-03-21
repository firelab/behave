
#ifndef CANOPYFLOW_H_
#define CANOPYFLOW_H_

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cmath>
#include <stdexcept>
//#include "boost/math/special_functions.hpp"
//#include <plstream.h>
#include "canopy.h"
#include "canopy_normal_distribution.h"
#include "canopy_triangle_distribution.h"
#include "massman_distribution.h"
#include "measured_distribution.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Class that computes wind flow in a canopy.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class canopyFlow
{

public:
    canopyFlow();
    canopyFlow(canopyFlow &rhs);
    canopyFlow &operator=(const canopyFlow &rhs);
    ~canopyFlow();

    void readData(std::string filename);
    void plot();
    void plotWind(double inputSpeed, double inputHeight);
    void plotDimensionalWind(double inputSpeed, double inputHeight);
    void plotWAFvsCdLAI(double inputHeight, double midFlameHeight, double lowLAI, double highLAI, int profileType);
    void plotz0ohvsCdLAI(double inputHeight, double lowLAI, double highLAI);
    void plotdohvsCdLAI(double inputHeight, double lowLAI, double highLAI);
    void plotz0ohvsone_doh(double inputHeight, double lowLAI, double highLAI);
    void plotz0ohvsdoh(double inputHeight, double lowLAI, double highLAI);

    void make_canopy(canopy::eCanopyType t);
    void make_canopy(canopy* X);    //takes a pointer to a base class (but object is actually a derived) and makes a clone of the object (same type and data)
    canopy* C;

    void computeWind(); //Precomputes some necessary wind stuff, run before calling get_windspeed
    double get_windAdjustmentFactorShelteredMidFlame(double inputHeight, double midFlameHeight);
    double get_windAdjustmentFactorShelteredIntegral(double inputHeight, double flameHeight);
    double get_windAdjustmentFactorUnshelteredIntegral(double inputHeight, double flameHeight);
    double get_windspeed(double inputSpeed, double inputHeight, double desiredHeight);    //Computes windspeed for given input speed, input height, and desiredHeight (speed and height units must be same as inputs)
    double get_uhuH(double inputSpeed, double inputHeight);

    double doh;     //non-dimensional canopy displacement height
    double z0oh;    //non-dimensional canopy roughness length
    double Iz0;     //intermediate variable in canopy height wind calculation
    double one_doh; // 1 - doh
    std::string dataFile;

protected:
    double usuh;
    double* uzc;
    double* uzcs;

    //static const double K = 0.4;    //von Karman constant
    //static const double c1 = 0.38;
    //static const double c3 = 15.0;
    //static const double STRSC = 0.5;    //adjustment factor to get the stress profile closer to observations of canopy Reynolds stresses
    //static const double rough = 1.07;   //correction factor for roughness sublayer

    double K;    //von Karman constant
    double c1;
    double c3;
    double STRSC;    //adjustment factor to get the stress profile closer to observations of canopy Reynolds stresses
    double rough;   //correction factor for roughness sublayer
    double logRough;    //precompute log of roughness to speed computations
    bool measuredDataExists;
    int n_measured;     //size of measured arrays
    double* measuredHeight;
    double* measuredSpeed;
    double* measuredShear;
};

#endif /* CANOPYFLOW_H_ */
