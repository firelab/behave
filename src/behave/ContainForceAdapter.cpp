#include "containForceAdapter.h"

ContainForceAdapter::ContainForceAdapter()
    :resourceVector(0)
{
}

ContainForceAdapter::~ContainForceAdapter()
{
}

int ContainForceAdapter::size() const
{
    return resourceVector.size();
}

void ContainForceAdapter::addResource(Sem::ContainResource& resource)
{
    resourceVector.push_back(resource);
}

void ContainForceAdapter::addResource(double arrival, double production, double duration,
    Sem::ContainFlank flank, std::string desc, double baseCost, double hourCost)
{
    char* const descCharStarConst = (char* const)desc.c_str();
    Sem::ContainResource resource(arrival, production, duration, flank, descCharStarConst, baseCost, hourCost);
    addResource(resource);
}

void ContainForceAdapter::removeAllResources()
{
    resourceVector.clear();
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

double ContainForceAdapter::getResourceArrivalAtIndex(int index) const
{
    return resourceVector[index].arrival();
}

double ContainForceAdapter::getResourceBaseCostAtIndex(int index) const
{
    return resourceVector[index].baseCost();
}

std::string ContainForceAdapter::getResourceDescriptionAtIndex(int index) const
{
    return resourceVector[index].description();
}

double ContainForceAdapter::getResourceDurationAtIndex(int index) const
{
    return resourceVector[index].duration();
}

Sem::ContainFlank ContainForceAdapter::getResourceFlankAtIndex(int index) const
{
    return resourceVector[index].flank();
}

double ContainForceAdapter::GetResourceHourCostAtIndex(int index) const
{
    return resourceVector[index].hourCost();
}

double ContainForceAdapter::GetResourceProductionAtIndex(int index) const
{
    return resourceVector[index].production();
}



