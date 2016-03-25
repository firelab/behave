
#include "canopy_normal_distribution.h"

canopy_normal_distribution::canopy_normal_distribution() : canopy()
{
    distributionType = normal_distribution;
    heightMaxFoliageDist = 0.5;
    standardDevFoliageDist = 0.25;
}

canopy_normal_distribution::canopy_normal_distribution(double heightMaxFoliageDist_, double standardDevFoliageDist_)
{
    distributionType = normal_distribution;
    heightMaxFoliageDist = heightMaxFoliageDist_;
    standardDevFoliageDist = standardDevFoliageDist_;
}

canopy_normal_distribution::canopy_normal_distribution(canopy_normal_distribution &rhs) : canopy(rhs)
{
    distributionType = rhs.distributionType;
    heightMaxFoliageDist = rhs.heightMaxFoliageDist;
    standardDevFoliageDist = rhs.standardDevFoliageDist;
}

canopy_normal_distribution &canopy_normal_distribution::operator=(const canopy_normal_distribution &rhs)
{
    if(&rhs != this)
    {
        canopy::operator=(rhs);
        heightMaxFoliageDist = rhs.heightMaxFoliageDist;
        standardDevFoliageDist = rhs.standardDevFoliageDist;
    }
    return *this;
}

canopy_normal_distribution::~canopy_normal_distribution()
{

}

void canopy_normal_distribution::compute_haz()
{
    double norm;
    //double hazn;
    double integHazn = 0.0;

    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //double intermediate = cellsize*2.0 / 3.0;
    double intermediate = 2.0 / 3.0;
    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    for(int i=0; i<numNodes; i++)   //integrate using extended simpson's rule
    {
        norm = (i*cellsize - heightMaxFoliageDist) / standardDevFoliageDist;
        haz[i] = exp(-norm * norm); //temporarily store this here
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


