#include "containAdapter.h"


ContainAdapter::ContainAdapter()
{
    lwRatio_ = 1.0,
    tactic_ = Contain::HeadAttack,
    attackDistance_ = 0.0,
    retry_ = true,
    minSteps_ = 250,
    maxSteps_ = 1000,
    maxFireSize_ = 1000,
    maxFireTime_ = 1080;
    reportSize_ = 0;
    reportRate_ = 0;
    fireStartTime_ = 0;

    finalCost_ = 0.0;
    finalFireLineLength_ = 0.0;
    perimeterAtContainment_ = 0.0;
    finalFireSize_ = 0.0;
    finalContainmentArea_ = 0.0;
    finalTime_ = 0.0;

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
    return force_.removeAllResourcesWithThisDesc(desc);
}

void ContainAdapter::removeAllResources()
{
    force_.resourceVector.clear();
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

void ContainAdapter::setTactic(Contain::ContainTactic tactic)
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
    if (force_.resourceVector.size() > 0 && reportSize_ != 0)
    {
        for (int i = 0; i < 24; i++)
        {
            diurnalROS_[i] = reportRate_;
        }

        double  resourceArrival;
        double  resourceBaseCost;
        double  resourceCost;
        std::string resourceDescription;
        double  resourceDuration;
        Sem::ContainFlank resourceFlank;
        double  resourceHourCost;
        double  resourceProduction;

        ContainForce oldForce;
        ContainForce* oldForcePointer = &oldForce;
        for (int i = 0; i < force_.resourceVector.size(); i++)
        {
            resourceArrival = force_.resourceVector[i].arrival();
            resourceBaseCost = force_.resourceVector[i].baseCost();
            resourceDescription = force_.resourceVector[i].description();
            resourceDuration = force_.resourceVector[i].duration();
            resourceFlank = force_.resourceVector[i].flank();
            resourceHourCost = force_.resourceVector[i].hourCost();
            resourceProduction = force_.resourceVector[i].production();

            char* const desc = (char* const)resourceDescription.c_str();
            oldForcePointer->addResource(resourceArrival, resourceProduction, resourceDuration, resourceFlank,
                desc, resourceBaseCost, resourceHourCost);
        }

        ContainSim containSim(reportSize_, reportRate_, diurnalROS_, fireStartTime_, lwRatio_,
            oldForcePointer, tactic_, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
            maxFireTime_);

        containSim.run();

         finalCost_ = containSim.finalFireCost();
         finalFireLineLength_ = containSim.finalFireLine();
         perimeterAtContainment_ = containSim.finalFirePerimeter();
         finalFireSize_ = containSim.finalFireSize();
         finalContainmentArea_ = containSim.finalFireSweep();
         finalTime_ = containSim.finalFireTime();
         containmentStatus_ = containSim.status();
    }
}

double ContainAdapter::getFinalCost() const
{
    return finalCost_;
}

double ContainAdapter::getFinalFireLineLength() const
{
    return finalFireLineLength_;
}

double ContainAdapter::getPerimeterAtContainment() const
{
    return perimeterAtContainment_;
}

double ContainAdapter::getFinalFireSize() const
{
    return finalFireSize_;
}

double ContainAdapter::getFinalContainmentArea() const
{
    return finalContainmentArea_;
}

double ContainAdapter::getFinalTimeSinceReport() const
{
    return finalTime_;
}

Contain::ContainStatus ContainAdapter::getContainmentStatus() const
{
    return containmentStatus_;
}
