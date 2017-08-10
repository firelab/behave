#include "containAdapter.h"

ContainAdapter::ContainAdapter()
{
    lwRatio_ = 1.0,
    tactic_ = ContainTactic::HeadAttack,
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

void ContainAdapter::addResource(double arrival, double production, double duration, 
    ContainFlank::ContainFlankEnum flank, std::string desc, double baseCost, double hourCost)
{
    Sem::ContainFlank myflank = static_cast<Sem::ContainFlank>(flank);
    force_.addResource(arrival, production, duration, myflank, desc, baseCost, hourCost);
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
    force_.removeAllResources();
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

void ContainAdapter::setTactic(ContainTactic::ContainTacticEnum tactic)
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
    if (force_.size() > 0 && reportSize_ != 0)
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

        Sem::ContainForce oldForce;
        Sem::ContainForce* oldForcePointer = &oldForce;
        for (int i = 0; i < force_.size(); i++)
        {
            resourceArrival = force_.getResourceArrivalAtIndex(i);
            resourceBaseCost = force_.getResourceBaseCostAtIndex(i);
            resourceDescription = force_.getResourceDescriptionAtIndex(i);
            resourceDuration = force_.getResourceDurationAtIndex(i);
            resourceFlank = force_.getResourceFlankAtIndex(i);
            resourceHourCost = force_.GetResourceHourCostAtIndex(i);
            resourceProduction = force_.GetResourceProductionAtIndex(i);

            char* const desc = (char* const)resourceDescription.c_str();
            oldForcePointer->addResource(resourceArrival, resourceProduction, resourceDuration, resourceFlank,
                desc, resourceBaseCost, resourceHourCost);
        }

        Sem::Contain::ContainTactic tactic = static_cast<Sem::Contain::ContainTactic>(tactic_);
        Sem::ContainSim containSim(reportSize_, reportRate_, diurnalROS_, fireStartTime_, lwRatio_,
            oldForcePointer, tactic, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
            maxFireTime_);

        containSim.run();

         finalCost_ = containSim.finalFireCost();
         finalFireLineLength_ = containSim.finalFireLine();
         perimeterAtContainment_ = containSim.finalFirePerimeter();
         finalFireSize_ = containSim.finalFireSize();
         finalContainmentArea_ = containSim.finalFireSweep();
         finalTime_ = containSim.finalFireTime();
         containmentStatus_ = static_cast<ContainStatus::ContainStatusEnum>(containSim.status());
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

ContainStatus::ContainStatusEnum ContainAdapter::getContainmentStatus() const
{
    return containmentStatus_;
}