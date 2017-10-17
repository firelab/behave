#include "containAdapter.h"
#define _USE_MATH_DEFINES
#include <math.h>

ContainAdapter::ContainAdapter()
{
    lwRatio_ = 1.0,
    tactic_ = Sem::Contain::ContainTactic::HeadAttack,
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

void ContainAdapter::addResource(Sem::ContainResource& resource)
{
    force_.addResource(resource);
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
    double arrivalInMinutes = TimeUnits::toBaseUnits(arrival, timeUnits);

    Sem::ContainResource resource(arrivalInMinutes, productionRateInChainsPerHour, durationInMinutes, myflank, (char* const)description.c_str(), baseCost, hourCost);
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
    force_.resourceVector.clear();
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

void ContainAdapter::setFireStartTime(int fireStartTime)
{
    fireStartTime_ = fireStartTime;
}

void ContainAdapter::setLwRatio(double lwRatio)
{
    lwRatio_ = lwRatio;
}

void ContainAdapter::setTactic(ContainAdapterEnums::ContainTactic::ContainTacticEnum tactic)
{
    tactic_ = convertAdapterTacticToSemTactic(tactic);
}

void ContainAdapter::setAttackDistance(double attackDistance, LengthUnits::LengthUnitsEnum lengthUnits)
{
    double attackDistanceInFeet = LengthUnits::toBaseUnits(attackDistance, lengthUnits); // convert attack distance to base units
    attackDistance_ = LengthUnits::fromBaseUnits(attackDistanceInFeet, LengthUnits::Chains); // Contain expects chains
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
    if (reportRate_ < 0.00001)
    {
        reportRate_ = 0.00001; // Contain algorithm can not deal with zero ROS
    }

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

        Sem::ContainForce oldForce;
        Sem::ContainForce* oldForcePointer = &oldForce;
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

        Sem::ContainSim containSim(reportSize_, reportRate_, diurnalROS_, fireStartTime_, lwRatio_,
            oldForcePointer, tactic_, attackDistance_, retry_, minSteps_, maxSteps_, maxFireSize_,
            maxFireTime_);

        // Do Contain simulation
        containSim.run();

        // Get results from Contain simulation
        finalCost_ = containSim.finalFireCost();
        finalFireLineLength_ = LengthUnits::toBaseUnits(containSim.finalFireLine(), LengthUnits::Chains);
        perimeterAtContainment_ = LengthUnits::toBaseUnits(containSim.finalFirePerimeter(), LengthUnits::Chains);
        finalFireSize_ = AreaUnits::toBaseUnits(containSim.finalFireSize(), AreaUnits::Acres);
        finalContainmentArea_ = AreaUnits::toBaseUnits(containSim.finalFireSweep(), AreaUnits::Acres);
        finalTime_ = TimeUnits::toBaseUnits(containSim.finalFireTime(), TimeUnits::Minutes);
        containmentStatus_ = convertSemStatusToAdapterStatus(containSim.status());
        containmentStatus_ = static_cast<ContainStatus::ContainStatusEnum>(containSim.status());

        // Calculate effective windspeed needed for Size module
        // Find the effective windspeed
        double effectiveWindspeed = 4.0 * (lwRatio_ - 1.0);
        size_.calculateFireBasicDimensions(effectiveWindspeed, SpeedUnits::MilesPerHour, reportRate_, SpeedUnits::ChainsPerHour);
        // Find the time elapsed to created the fire at time of report 
        LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Feet;
        double elapsedTime = 1.0;
        double ellipticalA = size_.getEllipticalA(lengthUnits, elapsedTime, TimeUnits::Minutes); // get base elliptical dimensions
        double ellipticalB = size_.getEllipticalB(lengthUnits, elapsedTime, TimeUnits::Minutes); // get base elliptical dimensions

        // Equation for area of ellipse used in Size Module (calculateFireArea() in fireSize.cpp) 
        // A = pi*a*b*s^2
        double reportSizeInSquareFeet = AreaUnits::toBaseUnits(reportSize_, AreaUnits::Acres);
        double intialElapsedTime = 0.0; // time for the fire to get to the reported size
        double totalElapsedTime = 0.0;
        perimeterAtInitialAttack_ = 0.0;
        fireSizeAtIntitialAttack_ = 0.0;
        double denominator = M_PI * ellipticalA * ellipticalB; // pi*a*b

        // Get the time that the first resource begins to attack the fire
        double firstArrivalTime = force_.firstArrival(Sem::ContainFlank::LeftFlank);
        if (firstArrivalTime < 0)
        {
            firstArrivalTime = 0.0; // make sure the time isn't negative for some weird reason
        }

        // Solve for seconds elapsed for reported fire size to reach its size at time of report assuming constant rate of growth

        if (denominator > 1.0e-07)
        {
            intialElapsedTime = sqrt(reportSizeInSquareFeet / denominator); // s = sqrt(A/(pi*a*b)) 
            totalElapsedTime = intialElapsedTime + firstArrivalTime;
            // Use total time elapsed to solve for perimeter and area of fire at time of initial attack
            LengthUnits::LengthUnitsEnum lengthUnits = LengthUnits::Feet;
            perimeterAtInitialAttack_ = size_.getFirePerimeter(lengthUnits, totalElapsedTime, TimeUnits::Minutes);
            AreaUnits::AreaUnitsEnum areaUnits = AreaUnits::SquareFeet;
            fireSizeAtIntitialAttack_ = size_.getFireArea(areaUnits, totalElapsedTime, TimeUnits::Minutes);
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

double ContainAdapter::getFireSizeAtInitialAttack(AreaUnits::AreaUnitsEnum areaUnits) const
{
    return AreaUnits::fromBaseUnits(fireSizeAtIntitialAttack_, areaUnits);
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

ContainAdapterEnums::ContainStatus::ContainStatusEnum ContainAdapter::getContainmentStatus() const
{
    return containmentStatus_;
}

Sem::Contain::ContainTactic ContainAdapter::convertAdapterTacticToSemTactic(ContainAdapterEnums::ContainTactic::ContainTacticEnum tactic)
{
    return (Sem::Contain::ContainTactic)tactic;
}

ContainAdapterEnums::ContainStatus::ContainStatusEnum ContainAdapter::convertSemStatusToAdapterStatus(Sem::Contain::ContainStatus status)
{
    return (ContainAdapterEnums::ContainStatus::ContainStatusEnum)status;
}

Sem::ContainFlank ContainAdapter::converAdapterFlankToSemFlank(ContainAdapterEnums::ContainFlank::ContainFlankEnum flank)
{
    return (Sem::ContainFlank)flank;
}
