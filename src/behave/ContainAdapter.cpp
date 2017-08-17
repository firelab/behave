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
}

ContainAdapter::~ContainAdapter()
{

}

void ContainAdapter::addResource(double arrival, double production, double duration, 
    ContainFlank::ContainFlankEnum flank, std::string description, double baseCost, double hourCost)
{
    Sem::ContainFlank myflank = static_cast<Sem::ContainFlank>(flank);

    Sem::ContainResource resource(arrival, production, duration, myflank, description, baseCost, hourCost);
    force_.addResource(resource);
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
    if (force_.resources() > 0 && reportSize_ != 0)
    {
        if (reportRate_ < 0.00001)
        {
            reportRate_ = 0.00001; // Contain algorithm can not deal with ROS = 0
        }
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

        Sem::Contain::ContainTactic tactic = static_cast<Sem::Contain::ContainTactic>(tactic_);
      
        containSim_.updateInputs(reportSize_, reportRate_, diurnalROS_, fireStartTime_, force_, lwRatio_,
            tactic, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
            maxFireTime_);

        containSim_.run();

        finalCost_ = containSim_.finalFireCost();
        finalFireLineLength_ = containSim_.finalFireLine();
        perimeterAtContainment_ = containSim_.finalFirePerimeter();
        finalFireSize_ = containSim_.finalFireSize();
        finalContainmentArea_ = containSim_.finalFireSweep();
        finalTime_ = containSim_.finalFireTime();
        containmentStatus_ = static_cast<ContainStatus::ContainStatusEnum>(containSim_.status());
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