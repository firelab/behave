#define _USE_MATH_DEFINES
#include "slopeTool.h"
#include <cmath>
#include <string>

SlopeTool::SlopeTool()
{
    const int numHorizontalDistanceValues = 7;
    horizontalDistances_.resize(numHorizontalDistanceValues);
   
    maxSlopeInDegrees_ = -1.0;
    maxSlopeInPercent_ = -1.0;
  
    slopeFromMapMeasurements_ = -1.0;
    slopeHorizontalDistance_ = -1.0;
    slopeElevationChange_ = -1.0;

    representativeFractions_.push_back(1980);
    representativeFractions_.push_back(3960);
    representativeFractions_.push_back(7920);
    representativeFractions_.push_back(10000);
    representativeFractions_.push_back(15840);
    representativeFractions_.push_back(21120);
    representativeFractions_.push_back(24000);
    representativeFractions_.push_back(31680);
    representativeFractions_.push_back(50000);
    representativeFractions_.push_back(62500);
    representativeFractions_.push_back(63360);
    representativeFractions_.push_back(100000);
    representativeFractions_.push_back(126720);
    representativeFractions_.push_back(250000);
    representativeFractions_.push_back(253440);
    representativeFractions_.push_back(506880);
    representativeFractions_.push_back(1000000);
    representativeFractions_.push_back(1013760);

    inchesPerMile_.push_back(32.0);
    inchesPerMile_.push_back(16.0);
    inchesPerMile_.push_back(8.0);
    inchesPerMile_.push_back(6.336);
    inchesPerMile_.push_back(4.0);
    inchesPerMile_.push_back(3.0);
    inchesPerMile_.push_back(2.64);
    inchesPerMile_.push_back(2.0);
    inchesPerMile_.push_back(1.2672);
    inchesPerMile_.push_back(1.0138);
    inchesPerMile_.push_back(1.0);
    inchesPerMile_.push_back(0.6336);
    inchesPerMile_.push_back(0.5);
    inchesPerMile_.push_back(0.2534);
    inchesPerMile_.push_back(0.25);
    inchesPerMile_.push_back(0.125);
    inchesPerMile_.push_back(0.0634);
    inchesPerMile_.push_back(0.0625);

    milesPerInch_.push_back(0.03125);
    milesPerInch_.push_back(0.0625);
    milesPerInch_.push_back(0.125);
    milesPerInch_.push_back(0.15783);
    milesPerInch_.push_back(0.25);
    milesPerInch_.push_back(0.33330);
    milesPerInch_.push_back(0.37879);
    milesPerInch_.push_back(0.5);
    milesPerInch_.push_back(0.78914);
    milesPerInch_.push_back(0.98643);
    milesPerInch_.push_back(1.0);
    milesPerInch_.push_back(1.57828);
    milesPerInch_.push_back(2.0);
    milesPerInch_.push_back(3.94571);
    milesPerInch_.push_back(4.0);
    milesPerInch_.push_back(8.0);
    milesPerInch_.push_back(15.78283);
    milesPerInch_.push_back(16.0);

    centimetersPerKilometer_.push_back(50.5051);
    centimetersPerKilometer_.push_back(25.2525);
    centimetersPerKilometer_.push_back(12.6263);
    centimetersPerKilometer_.push_back(10.0);
    centimetersPerKilometer_.push_back(6.3131);
    centimetersPerKilometer_.push_back(4.7348);
    centimetersPerKilometer_.push_back(4.1667);
    centimetersPerKilometer_.push_back(3.1566);
    centimetersPerKilometer_.push_back(2.0);
    centimetersPerKilometer_.push_back(1.6);
    centimetersPerKilometer_.push_back(1.5783);
    centimetersPerKilometer_.push_back(1.0);
    centimetersPerKilometer_.push_back(0.7891);
    centimetersPerKilometer_.push_back(0.4);
    centimetersPerKilometer_.push_back(0.3946);
    centimetersPerKilometer_.push_back(0.1973);
    centimetersPerKilometer_.push_back(0.1);
    centimetersPerKilometer_.push_back(0.0986);

    kilometersPerCentimeter_.push_back(0.0198);
    kilometersPerCentimeter_.push_back(0.0396);
    kilometersPerCentimeter_.push_back(0.0792);
    kilometersPerCentimeter_.push_back(0.1);
    kilometersPerCentimeter_.push_back(0.1584);
    kilometersPerCentimeter_.push_back(0.2112);
    kilometersPerCentimeter_.push_back(0.24);
    kilometersPerCentimeter_.push_back(0.3168);
    kilometersPerCentimeter_.push_back(0.5);
    kilometersPerCentimeter_.push_back(0.625);
    kilometersPerCentimeter_.push_back(0.6336);
    kilometersPerCentimeter_.push_back(1.0);
    kilometersPerCentimeter_.push_back(1.2672);
    kilometersPerCentimeter_.push_back(2.50);
    kilometersPerCentimeter_.push_back(2.5344);
    kilometersPerCentimeter_.push_back(5.0688);
    kilometersPerCentimeter_.push_back(10.0);
    kilometersPerCentimeter_.push_back(10.1376);
}

SlopeTool::~SlopeTool()
{

}

SlopeTool::SlopeTool(const SlopeTool& rhs)
{
    memberwiseCopyAssignment(rhs);
}

SlopeTool& SlopeTool::operator=(const SlopeTool& rhs)
{
    if (this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void SlopeTool::calculateHorizontalDistance(const double calulatedMapDistance, const LengthUnits::LengthUnitsEnum distanceUnits,
    const double maxSlopeSteepness, const SlopeUnits::SlopeUnitsEnum slopeUnits)
{
    maxSlopeInDegrees_ = SlopeUnits::toBaseUnits(maxSlopeSteepness, slopeUnits);
    maxSlopeInPercent_ = SlopeUnits::fromBaseUnits(maxSlopeInDegrees_, SlopeUnits::Percent);

    const double groundDistanceInFeet = LengthUnits::toBaseUnits(calulatedMapDistance, distanceUnits);
    const double groundDistanceInInches = LengthUnits::fromBaseUnits(calulatedMapDistance, LengthUnits::Inches);

    for (int i = 0; i < horizontalDistances_.size(); i++)
    {
        double direction = 15.0 * (double)i;
        double a = groundDistanceInInches * cos(direction * M_PI / 180.0);
        double b = groundDistanceInInches * sin(direction * M_PI / 180.0);
        double c = a * cos(maxSlopeInDegrees_ * M_PI / 180.0);
        double d = sqrt(c * c + b * b);
        horizontalDistances_[i] = LengthUnits::toBaseUnits(d, LengthUnits::Inches);
    }
}

void SlopeTool::calculateSlopeFromMapMeasurements(const int mapRepresentativeFraction, const double mapDistance,
    const LengthUnits::LengthUnitsEnum distanceUnits, const double contourInterval, const double numberOfContours,
    const LengthUnits::LengthUnitsEnum contourUnits)
{
    const double distanceFeet = LengthUnits::toBaseUnits(mapDistance, distanceUnits);
    const double distanceInches = LengthUnits::fromBaseUnits(distanceFeet, LengthUnits::Inches);

    // Calculate results
    slopeElevationChange_ = LengthUnits::toBaseUnits(contourInterval * numberOfContours, contourUnits);
    slopeHorizontalDistance_ = LengthUnits::toBaseUnits(mapRepresentativeFraction * distanceInches, LengthUnits::Inches);
    const double slopeInPercent = (slopeHorizontalDistance_ < 0.01) ? 0.0 : slopeElevationChange_ / slopeHorizontalDistance_;
    slopeFromMapMeasurements_ = (slopeHorizontalDistance_ < 0.01) ? 0.0 : atan(slopeInPercent) * 180.0 / M_PI;
}

int SlopeTool::getNumberOfHorizontalDistances() const
{
    return horizontalDistances_.size();
}

double SlopeTool::getHorizontalDistanceMaxSlope(const SlopeUnits::SlopeUnitsEnum slopeUnits) const
{
    return SlopeUnits::fromBaseUnits(maxSlopeInDegrees_, slopeUnits);
}

double SlopeTool::getHorizontalDistance(const HorizontalDistanceIndex::HorizontalDistanceIndexEnum horizontalDistanceIndex, const LengthUnits::LengthUnitsEnum mapDistanceUnits) const
{
    return getHorizontalDistanceAtIndex(static_cast<int>(horizontalDistanceIndex), mapDistanceUnits);
}

double SlopeTool::getHorizontalDistanceAtIndex(const int index, const LengthUnits::LengthUnitsEnum mapDistanceUnits) const
{
    double distance = -1.0;
    if (index >= 0 && index < horizontalDistances_.size())
    {
        distance = LengthUnits::fromBaseUnits(horizontalDistances_[index], mapDistanceUnits);
    }
    return distance;
}

double SlopeTool::getSlopeFromMapMeasurements(SlopeUnits::SlopeUnitsEnum slopeUnits) const
{
    return SlopeUnits::fromBaseUnits(slopeFromMapMeasurements_, slopeUnits);
}

double SlopeTool::getSlopeHorizontalDistanceFromMapMeasurements(LengthUnits::LengthUnitsEnum distanceUnits) const
{
    return LengthUnits::fromBaseUnits(slopeHorizontalDistance_, distanceUnits);
}

double SlopeTool::getSlopeElevationChangeFromMapMeasurements(LengthUnits::LengthUnitsEnum elevationUnits) const
{
    return LengthUnits::fromBaseUnits(slopeElevationChange_, elevationUnits);
}

int SlopeTool::getNumberOfRepresentativeFractions() const
{
    return representativeFractions_.size();
}

int SlopeTool::getRepresentativeFractionAtIndex(const int index) const
{
    int representativeFraction = -1;
    if (index >= 0 && index < representativeFractions_.size())
    {
        representativeFraction = representativeFractions_[index];
    }
    return representativeFraction;
}

double SlopeTool::getInchesPerMileAtIndex(const int index) const
{
    double inchesPerMile = -1.0;
    if (index >= 0 && index < inchesPerMile_.size())
    {
        inchesPerMile = inchesPerMile_[index];
    }
    return inchesPerMile;
}

double SlopeTool::getMilesPerInchAtIndex(const int index) const
{
    double milesPerInch = -1.0;
    if (index >= 0 && index < milesPerInch_.size())
    {
        milesPerInch = milesPerInch_[index];
    }
    return milesPerInch;
}

double SlopeTool::getCentimetersPerKilometerAtIndex(const int index) const
{
    double centimetersPerKilometer = -1.0;
    if (index >= 0 && index < centimetersPerKilometer_.size())
    {
        centimetersPerKilometer = centimetersPerKilometer_[index];
    }
    return centimetersPerKilometer;
}

double SlopeTool::getKilometersPerCentimeterAtIndex(const int index) const
{
    double kilometersPerCentimeter = -1.0;
    if (index >= 0 && index < kilometersPerCentimeter_.size())
    {
        kilometersPerCentimeter = kilometersPerCentimeter_[index];
    }
    return kilometersPerCentimeter;
}

int SlopeTool::getRepresentativeFractionAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const
{
    return getRepresentativeFractionAtIndex(static_cast<int>(representativeFraction));
}

double SlopeTool::getInchesPerMileAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const
{
    return getInchesPerMileAtIndex(static_cast<int>(representativeFraction));
}

double SlopeTool::getMilesPerInchAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const
{
    return getMilesPerInchAtIndex(static_cast<int>(representativeFraction));
}

double SlopeTool::getCentimetersPerKilometerAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const
{
    return getCentimetersPerKilometerAtIndex(static_cast<int>(representativeFraction));
}

double SlopeTool::getKilometersPerCentimeterAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const
{
    return getKilometersPerCentimeterAtIndex(static_cast<int>(representativeFraction));
}

void SlopeTool::memberwiseCopyAssignment(const SlopeTool& rhs)
{
    maxSlopeInDegrees_ = rhs.maxSlopeInDegrees_;
    maxSlopeInPercent_ = rhs.maxSlopeInPercent_;
    horizontalDistances_ = rhs.horizontalDistances_;

    slopeFromMapMeasurements_ = rhs.slopeFromMapMeasurements_;
    slopeHorizontalDistance_ = rhs.slopeHorizontalDistance_;
    slopeElevationChange_ = rhs.slopeElevationChange_;

    representativeFractions_ = rhs.representativeFractions_;
    inchesPerMile_ = rhs.inchesPerMile_;
    milesPerInch_ = rhs.milesPerInch_;
    centimetersPerKilometer_ = rhs.centimetersPerKilometer_;
    kilometersPerCentimeter_ = rhs.kilometersPerCentimeter_;
}
