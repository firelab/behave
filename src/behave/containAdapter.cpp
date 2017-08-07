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
}

ContainAdapter::~ContainAdapter()
{

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
    reportRate_ = 20;
    reportSize_ = 20;
    lwRatio_ = 1.0;
    fireStartTime_ = 0;

    force_.addResource(0, 60);
    Sem::ContainForce* forcePointer = &force_;
   
    for (int i = 0; i < 24; i++)
    {
        diurnalROS_[i] = reportRate_;
    }

    Sem::ContainSim containSim(reportSize_, reportRate_, diurnalROS_, fireStartTime_, lwRatio_,
        forcePointer, tactic_, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
        maxFireTime_);

    containSim.run();
}
