#include "containAdapter.h"


ContainAdapter::ContainAdapter()
{
    lwRatio_ = 1.0,
    tactic_ = Sem::Contain::HeadAttack,
    attackDistance_ = 0.0,
    retry_ = true,
    minSteps_ = 250,
    maxSteps_ = 1000,
    maxFireSize_ = 1000,
    maxFireTime_ = 1080;

    doContainRun();
}

ContainAdapter::~ContainAdapter()
{

}

void ContainAdapter::addResource(ContainResource& resource)
{
    force_.addResource(resource);
}

void ContainAdapter::addResource(double arrival, double production, double duration, 
    ContainFlank flank, std::string desc, double baseCost, double hourCost)
{
    force_.addResource(arrival, production, duration, flank, desc, baseCost, hourCost);
}

int ContainAdapter::removeResourceAt(int index)
{
    return force_.removeResourceAt(index);
}

int ContainAdapter::removeResourceWithThisDesc(std::string desc)
{
    return force_.removeResourceWithThisDesc(desc);
}

int ContainAdapter::removeAllResourcesWithThisDesc(std::string desc)
{
    int rc;
    int success = 1; // 1 means didn't find it
    while ((rc = force_.removeResourceWithThisDesc(desc)) == 0)
    {
        success = 0; // found at least one
    }
    return success;
}

void ContainAdapter::removeAllResources()
{
    force_.clearResourceVector();
}

void ContainAdapter::setReportSize(double reportSize)
{
    reportSize_ = reportSize;
}

void ContainAdapter::setReportRate(double reportRate)
{
    reportRate_ = reportRate;
}

void ContainAdapter::setFireStartTime(int fireStartTime)
{
    fireStartTime_ = fireStartTime;
}

void ContainAdapter::setLwRatio(double lwRatio)
{
    lwRatio_ = lwRatio;
}

void ContainAdapter::setTactic(Sem::Contain::ContainTactic tactic)
{
    tactic_ = tactic;
}

void ContainAdapter::setAttackDistance(double attackDistance)
{
    attackDistance_ = attackDistance;
}

void ContainAdapter::setRetry(bool retry)
{
    retry_ = retry;
}

void ContainAdapter::setMinSteps(int minSteps)
{
    minSteps_ = minSteps;
}

void ContainAdapter::setMaxSteps(int maxSteps)
{
    maxSteps_ = maxSteps;
}

void ContainAdapter::setMaxFireSize(int maxFireSize)
{
    maxFireSize_ = maxFireSize;
}

void ContainAdapter::setMaxFireTime(int maxFireTime)
{
    maxFireTime_ = maxFireTime;
}

void ContainAdapter::doContainRun()
{
    if (force_.resources() > 0)
    {
        for (int i = 0; i < 24; i++)
        {
            diurnalROS_[i] = reportRate_;
        }
        ContainForce* forcePointer = &force_;
        ContainSim containSim(reportSize_, reportRate_, diurnalROS_, fireStartTime_, lwRatio_,
            forcePointer, tactic_, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
            maxFireTime_);

        containSim.run();
    }
}
