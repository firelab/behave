

#include "massman_distribution.h"

massman_distribution::massman_distribution() : canopy()
{
    distributionType = Massman;
    A1 = 1.10;
    A2 = 2.0;
    A3 = 1.0;
    zmax = 0.7;
}

massman_distribution::massman_distribution(double A1_, double A2_, double A3_, double zmax_)
{
    distributionType = Massman;
    A1 = A1_;
    A2 = A2_;
    A3 = A3_;
    zmax = zmax_;
}

massman_distribution::massman_distribution(massman_distribution &rhs) : canopy(rhs)
{
    distributionType = rhs.distributionType;
    A1 = rhs.A1;
    A2 = rhs.A2;
    A3 = rhs.A3;
    zmax = rhs.zmax;
}

massman_distribution &massman_distribution::operator=(const massman_distribution &rhs)
{
    if(&rhs != this)
    {
        canopy::operator=(rhs);
        A1 = rhs.A1;
        A2 = rhs.A2;
        A3 = rhs.A3;
        zmax = rhs.zmax;
    }
    return *this;
}

massman_distribution::~massman_distribution()
{

}

void massman_distribution::compute_haz()
{
    double integHazn = 0.0;
    double zheight;

    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //double intermediate = cellsize*2.0 / 3.0;
    double intermediate = 2.0 / 3.0;
    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    double A4 = A2 * (A3 + zmax) / (A1 - zmax);

    for(int i=0; i<numNodes; i++)   //integrate using extended simpson's rule
    {
        zheight = i*cellsize;
        haz[i] = pow((A1 - zheight), A2) * pow((A3 + zheight), A4);    //temporarily store this here

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


