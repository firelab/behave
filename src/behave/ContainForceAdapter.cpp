#include "ContainForceAdapter.h"

ContainForceAdapter::ContainForceAdapter()
    :resourceVector(0)
{
}

ContainForceAdapter::~ContainForceAdapter()
{
}

void ContainForceAdapter::addResource(Sem::ContainResource& resource)
{
    resourceVector.push_back(resource);
}

void ContainForceAdapter::addResource(double arrival, double production, double duration,
    Sem::ContainFlank flank, std::string desc, double baseCost, double hourCost)
{
    Sem::ContainResource resource(arrival, production, duration, flank, (char* const)desc.c_str(), baseCost, hourCost);
    addResource(resource);
}

double ContainForceAdapter::firstArrival(Sem::ContainFlank flank) const
{
    double at = 99999999.0;
    for (int i = 0; i < resourceVector.size(); i++)
    {
        if ((resourceVector[i].flank() == flank || resourceVector[i].flank() == Sem::BothFlanks)
            && resourceVector[i].arrival() < at)
        {
            at = resourceVector[i].arrival();
        }
    }
    return(at);
}

int ContainForceAdapter::removeResourceAt(int index)
{
    int success = 1; // 1 means didn't find it
    if (index < resourceVector.size())
    {
        resourceVector.erase(resourceVector.begin() + index);
        return 0; // success
    }
    return success;
}

int ContainForceAdapter::removeResourceWithThisDesc(std::string desc)
{
    std::string descriptionString;
    for (int i = 0; i < resourceVector.size(); i++)
    {
        descriptionString = resourceVector[i].description();
        if (descriptionString == desc)
        {
            removeResourceAt(i);
            return 0; // success
        }
    }
    // didn't find it
    return 1; // error
}

int ContainForceAdapter::removeAllResourcesWithThisDesc(std::string desc)
{
    int rc;
    int success = 1; // 1 means didn't find it
    while ((rc = removeResourceWithThisDesc(desc)) == 0)
    {
        success = 0; // found at least one
    }
    return success;
}
