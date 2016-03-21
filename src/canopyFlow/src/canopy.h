#ifndef CANOPY_H
#define CANOPY_H

#include <math.h>
#include <cstring>
//#include <plstream.h>
#include <stdexcept>

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Pure virtual base class that stores canopy density, etc. information.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class canopy
{

public:
    canopy();
    canopy(canopy &rhs);
    canopy &operator=(const canopy &rhs);
    virtual ~canopy();

    void initialize();
    void plot();

    enum eCanopyType{
        normal_distribution,
        triangle,
        Massman,
        measured
    };
    eCanopyType distributionType;

    //inputs
    double canopyHeight;            //canopy height (m), if no canopy set to 1
    double leafAreaIndex;
    double dragCoefAth;
    double z0g;                     //ground roughness length (m)
    int numNodes;                   //number of cells to use for numerical integration

    //calculated
    double cellsize;                //cellsize for integration, this is computed, NOT INPUT
    double* haz;                    //nondimensional leaf area density
    double  zetah;                  //this is hacpn at the top node
    double* zetaz;                  //normalized mapped vertical coordinate
    double  z0gh;                   //this is z0g/h
    double* hacpz;                  //nondimensional drag area density

protected:
    void initialize_memory();
    virtual void compute_haz() = 0;
    double get_dragCoef(double zOverh);
    double get_shelterFactor(double hazn);
    void compute_foliage_drag_area_index();

    double totalDragAreaIndex;
    double* cumulativeLeafDragArea; //cumulative leaf drag area (m^2/m^2)
};

#endif // CANOPY_H
