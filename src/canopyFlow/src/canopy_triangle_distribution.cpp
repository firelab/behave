

#include "canopy_triangle_distribution.h"

canopy_triangle_distribution::canopy_triangle_distribution() : canopy()
{
    distributionType = triangle;
    A1 = 0.08;      //density of top
    Ax = 3;      //density at max point
    Ab = 0.18;      //density of bottom (trunk space)
    zmax = 0.5;    //height to Ax (0 < zmax < 1)
    zbot = 0.1;    //height to bottom of triangular part (0 < zbot < 1; zbot < zmax)
}

canopy_triangle_distribution::canopy_triangle_distribution(double A1_, double Ax_, double Ab_, double zmax_, double zbot_)
{
    distributionType = triangle;
    A1 = A1_;
    Ax = Ax_;
    Ab = Ab_;
    zmax = zmax_;
    zbot = zbot_;
}

canopy_triangle_distribution::canopy_triangle_distribution(canopy_triangle_distribution &rhs) : canopy(rhs)
{
    distributionType = rhs.distributionType;
    A1 = rhs.A1;
    Ax = rhs.Ax;
    Ab = rhs.Ab;
    zmax = rhs.zmax;
    zbot = rhs.zbot;

}

canopy_triangle_distribution &canopy_triangle_distribution::operator=(const canopy_triangle_distribution &rhs)
{
    if(&rhs != this)
    {
        canopy::operator=(rhs);
        A1 = rhs.A1;
        Ax = rhs.Ax;
        Ab = rhs.Ab;
        zmax = rhs.zmax;
        zbot = rhs.zbot;
    }
    return *this;
}

canopy_triangle_distribution::~canopy_triangle_distribution()
{

}

void canopy_triangle_distribution::compute_haz()
{
    double integHazn = 0.0;
    double zheight;

    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //double intermediate = cellsize*2.0 / 3.0;
    double intermediate = 2.0 / 3.0;
    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    for(int i=0; i<numNodes; i++)   //integrate using extended simpson's rule
    {
        zheight = i*cellsize;
        if(zheight <= zbot)
            haz[i] = Ab;    //temporarily store this here
        else if(zheight <= zmax)
            haz[i] = Ab + (zheight - zbot) * (Ax - Ab)/(zmax - zbot);   //temporarily store this here
        else
            haz[i] = Ax - (zheight - zmax) * (Ax - A1)/((numNodes-1)*cellsize - zmax);  //temporarily store this here

        if(i%2 == 0)    //if even numbers
            integHazn += haz[i];
        else            //if odd numbers
            integHazn += 2.0 * haz[i];
    }

    integHazn = integHazn - 0.5 * (haz[0] + haz[numNodes-1]);
    integHazn *= intermediate;

    for(int i=0; i<numNodes; i++)
        haz[i] = leafAreaIndex * haz[i] /integHazn;
}
