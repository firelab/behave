#define _USE_MATH_DEFINES
#include <cmath>

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

    finalCost_ = 0.0;
    finalFireLineLength_ = 0.0;
    perimeterAtContainment_ = 0.0;
    finalFireSize_ = 0.0;
    finalContainmentArea_ = 0.0;
    perimeterAtInitialAttack_ = 0.0;
    finalTime_ = 0.0;
}

ContainAdapter::~ContainAdapter()
{

}

void ContainAdapter::addResource(double arrival, double duration, TimeUnits::TimeUnitsEnum timeUnits, double productionRate, SpeedUnits::SpeedUnitsEnum productionRateUnits,
    std::string description, double baseCost, double hourCost)
{
    Sem::ContainFlank myflank = Sem::ContainFlank::LeftFlank; // Currently only attacking left flanks is used, production is halved, left flank is mirrored
    
    double productionRateInChainsPerHour = productionRate;
    if (!(productionRateUnits == SpeedUnits::ChainsPerHour))
    {
        double productionRateInFeetPerMinute = SpeedUnits::toBaseUnits(productionRate, productionRateUnits);
        productionRateInChainsPerHour = SpeedUnits::fromBaseUnits(productionRateInFeetPerMinute, SpeedUnits::ChainsPerHour); // Contain expects chains per hour
    }
   
    // Contain expects minutes
    double durationInMinutes = TimeUnits::toBaseUnits(duration, timeUnits); 
    double arrivalInMinutes = TimeUnits::toBaseUnits(duration, timeUnits);
    
    Sem::ContainResource resource(arrival, productionRateInChainsPerHour, durationInMinutes, myflank, description, baseCost, hourCost);
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

void ContainAdapter::setReportSize(double reportSize, AreaUnits::AreaUnitsEnum areaUnits)
{
    double reportSizeInSquareFeet = AreaUnits::toBaseUnits(reportSize, areaUnits); // convert report size to base units
    reportSize_ = AreaUnits::fromBaseUnits(reportSizeInSquareFeet, AreaUnits::Acres); // Contain expects acres
}

void ContainAdapter::setReportRate(double reportRate, SpeedUnits::SpeedUnitsEnum speedUnits)
{
    double reportRateInFeetPerMinute = SpeedUnits::toBaseUnits(reportRate, speedUnits); // convert report rate to base units
    reportRate_ = SpeedUnits::fromBaseUnits(reportRateInFeetPerMinute, SpeedUnits::ChainsPerHour); // Contain expects chains per hour
}

void ContainAdapter::setLwRatio(double lwRatio)
{
    lwRatio_ = lwRatio;
}

void ContainAdapter::setTactic(ContainTactic::ContainTacticEnum tactic)
{
    tactic_ = tactic;
}

void ContainAdapter::setAttackDistance(double attackDistance, LengthUnits::LengthUnitsEnum lengthUnits)
{
    double attackDistanceInFeet = LengthUnits::toBaseUnits(attackDistance, lengthUnits); // convert attack distance to base units
    attackDistance_ = LengthUnits::fromBaseUnits(attackDistanceInFeet, LengthUnits::Chains); // Contain expects chains
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

        Sem::Contain::ContainTactic tactic = static_cast<Sem::Contain::ContainTactic>(tactic_);
      
        int fireStartTime = 0;
        containSim_.updateInputs(reportSize_, reportRate_, diurnalROS_, fireStartTime, force_, lwRatio_,
            tactic, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
            maxFireTime_);

        containSim_.run();

        finalCost_ = containSim_.finalFireCost();
        finalFireLineLength_ = LengthUnits::toBaseUnits(containSim_.finalFireLine(), LengthUnits::Chains);
        perimeterAtContainment_ = LengthUnits::toBaseUnits(containSim_.finalFirePerimeter(), LengthUnits::Chains);
        finalFireSize_ = AreaUnits::toBaseUnits(containSim_.finalFireSize(), AreaUnits::Acres);
        finalContainmentArea_ = AreaUnits::toBaseUnits(containSim_.finalFireSweep(), AreaUnits::Acres);
        finalTime_ = TimeUnits::toBaseUnits(containSim_.finalFireTime(), TimeUnits::Minutes);
        containmentStatus_ = static_cast<ContainStatus::ContainStatusEnum>(containSim_.status());

        // Calculate effective windspeed needed for Size module
        // Find the effective windspeed
        double effectiveWindspeed = 4.0 * (lwRatio_ - 1.0);
        double reportRateInFeetPerMinute = SpeedUnits::toBaseUnits(reportRate_, SpeedUnits::ChainsPerHour);
        size_.calculateFireBasicDimensions(effectiveWindspeed, reportRateInFeetPerMinute);
        // Find the time elapsed to created the fire at time of report 
        double ellipticalA = size_.getEllipticalA(1);
        double ellipticalB = size_.getEllipticalB(1);

        double intialElapsedTime = 0;
        double totalElapsedTime = 0;
        double denominator = M_PI * ellipticalA * ellipticalB;

        double reportSizeInSquareFeet = AreaUnits::toBaseUnits(reportSize_, AreaUnits::Acres);

        double minArriveTime = TimeUnits::toBaseUnits(force_.firstArrival(Sem::ContainFlank::LeftFlank), TimeUnits::Hours);
        if (minArriveTime < 0)
        {
            minArriveTime = 0;
        }

        if (denominator > 1e-07)
        {
            intialElapsedTime =  sqrt(reportSizeInSquareFeet / denominator);
            totalElapsedTime = intialElapsedTime + minArriveTime;
            perimeterAtInitialAttack_ = size_.calculateFirePerimeter(totalElapsedTime);
        }
        
    }
}

double ContainAdapter::getFinalCost() const
{
    return finalCost_;
}

double ContainAdapter::getFinalFireLineLength(LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return LengthUnits::fromBaseUnits(finalFireLineLength_, lengthUnits);
}

double ContainAdapter::getPerimiterAtInitialAttack(LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return LengthUnits::fromBaseUnits(perimeterAtInitialAttack_, lengthUnits);
}

double ContainAdapter::getPerimeterAtContainment(LengthUnits::LengthUnitsEnum lengthUnits) const
{
    return LengthUnits::fromBaseUnits(perimeterAtContainment_, lengthUnits);
}

double ContainAdapter::getFinalFireSize(AreaUnits::AreaUnitsEnum areaUnits) const
{
    return AreaUnits::fromBaseUnits(finalFireSize_, areaUnits);
}

double ContainAdapter::getFinalContainmentArea(AreaUnits::AreaUnitsEnum areaUnits) const
{
    return AreaUnits::fromBaseUnits(finalContainmentArea_, areaUnits);
}

double ContainAdapter::getFinalTimeSinceReport(TimeUnits::TimeUnitsEnum timeUnits) const
{
    return TimeUnits::fromBaseUnits(finalTime_, timeUnits);
}

ContainStatus::ContainStatusEnum ContainAdapter::getContainmentStatus() const
{
    return containmentStatus_;
}