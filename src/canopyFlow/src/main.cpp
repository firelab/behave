

#include <iostream>
#include <stdio.h>
#include "canopyFlow.h"

int main() {

    canopyFlow wind;

    //-------Normal Distribution-----------------------
    //double heightMaxFoliageDist = 0.5;
    //double standardDevFoliageDist = 0.25;
    //wind.C = new canopy_normal_distribution(heightMaxFoliageDist, standardDevFoliageDist);
    //wind.C->leafAreaIndex = 1.0;
    //wind.C->canopyHeight = 10.0;                        //canopy height (m)
    //wind.C->dragCoefAth = 0.2;

    //-------Triangle Distribution---------------------
    double A1 = 0.32;       //density of top
    double Ax = 1.0;          //density at max point
    double Ab = 0.02;       //density of bottom (trunk space)
    double zmax = 0.36;      //height to Ax (0 < zmax < 1)
    double zbot = 0.12;      //height to bottom of triangular part (0 < zbot < 1; zbot < zmax)
    wind.C = new canopy_triangle_distribution(A1, Ax, Ab, zmax, zbot);
    wind.C->leafAreaIndex = 3.28;
    wind.C->canopyHeight = 4.0;                        //canopy height (m)
    wind.C->dragCoefAth = 0.2;

    //-------Massman Distribution----------------------
    //double A1 = 1.10;
    //double A2 = 2.0;
    //double A3 = 1.0;
    //double zmax = 0.7;
    //wind.C = new massman_distribution(A1, A2, A3, zmax);
    //wind.C->leafAreaIndex = 1.0;
    //wind.C->canopyHeight = 10.0;                        //canopy height (m)
    //wind.C->dragCoefAth = 0.2;

//-------Measured Distribution---------------------
//    wind.C = new measured_distribution("C:\\Users\\wchatham\\Source\\Repos\\Behave\\src\\CanopyFlow\\data\\Aspen_canopy_distribution.txt");
//    wind.readData("C:\\Users\\wchatham\\Source\\Repos\\Behave\\src\\CanopyFlow\\data\\Aspen_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Corn_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Corn_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Hardwood_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Hardwood_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Jack_Pine_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Jack_Pine_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Loblolly_Pine_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Loblolly_Pine_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Rice_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Rice_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Scots_Pine_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Scots_Pine_Wind.txt");

//    wind.C = new measured_distribution("/mnt/ScratchDrive/src/canopy-flow/data/Spruce_canopy_distribution.txt");
//    wind.readData("/mnt/ScratchDrive/src/canopy-flow/data/Spruce_Wind.txt");

    //Set inputs
    wind.C->z0g = 0.025;                      //ground roughness length (m)
    wind.C->numNodes = 10001;                 //number of cells to use for numerical integration

    wind.C->initialize();
    wind.computeWind();

    std::cout << wind.get_windAdjustmentFactorShelteredMidFlame(6.096, 1.0) << std::endl;
    //std::cout << wind.get_windAdjustmentFactorShelteredIntegral(6.096, 2.0) << std::endl;
    //std::cout << wind.get_windAdjustmentFactorUnshelteredIntegral(6.096, 22.19) << std::endl;

    double inputHeight = wind.C->canopyHeight + 6.096;
    double midFlameHeight = 7.0;
    double inputSpeed = 10.0;
    //double lowLAI = 0.001;
    //double highLAI = 10.0;
    int profileType = 1;    //  0 => sheltered;  1 => unsheltered;

//    wind.plotDimensionalWind(inputSpeed, inputHeight);
//    wind.plotWAFvsCdLAI(inputHeight, midFlameHeight, lowLAI, highLAI, profileType);
//    wind.plotz0ohvsCdLAI(inputHeight, lowLAI, highLAI);
//    wind.plotdohvsCdLAI(inputHeight, lowLAI, highLAI);
//    wind.plotz0ohvsone_doh(inputHeight, lowLAI, highLAI);
//    wind.plotz0ohvsdoh(inputHeight, lowLAI, highLAI);

    std::cout << "Done!" << std::endl;
    return 0;
}
