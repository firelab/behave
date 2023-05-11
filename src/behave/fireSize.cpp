#include "fireSize.h"
#define _USE_MATH_DEFINES
#include <cmath>

FireSize::FireSize()
{

}

FireSize::~FireSize()
{

}

void FireSize::calculateFireBasicDimensions(double effectiveWindSpeed, SpeedUnits::SpeedUnitsEnum windSpeedRateUnits, double forwardSpreadRate, SpeedUnits::SpeedUnitsEnum spreadRateUnits)
{
    forwardSpreadRate_ = SpeedUnits::toBaseUnits(forwardSpreadRate, spreadRateUnits); // spread rate is now feet per minute
    if (windSpeedRateUnits != SpeedUnits::MilesPerHour)
    {
        effectiveWindSpeed_ = SpeedUnits::toBaseUnits(effectiveWindSpeed, windSpeedRateUnits); // wind speed is now feet per minute
        effectiveWindSpeed_ = SpeedUnits::fromBaseUnits(effectiveWindSpeed_, SpeedUnits::MilesPerHour); // wind speed is now miles per hour
    }
    else
    {
        effectiveWindSpeed_ = effectiveWindSpeed;
    }

    calculateFireLengthToWidthRatio();
    calculateSurfaceFireEccentricity();
    calculateBackingSpreadRate();
    calculateFlankingSpreadRate();
    calculateEllipticalDimensions();
}

double FireSize::getFireLengthToWidthRatio() const
{
    return fireLengthToWidthRatio_;
}

double FireSize::getEccentricity() const
{
    return eccentricity_;
}

double FireSize::getBackingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(backingSpreadRate_, spreadRateUnits);
}

double FireSize::getFlankingSpreadRate(SpeedUnits::SpeedUnitsEnum spreadRateUnits) const
{
    return SpeedUnits::fromBaseUnits(flankingSpreadRate_, spreadRateUnits);
}

double FireSize::getEllipticalA(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    return LengthUnits::fromBaseUnits((ellipticalA_ * elapsedTime), lengthUnits);
}

double FireSize::getEllipticalB(LengthUnits::LengthUnitsEnum lengthUnits ,double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    return  LengthUnits::fromBaseUnits((ellipticalB_ * elapsedTime), lengthUnits);
}

double FireSize::getEllipticalC(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    return LengthUnits::fromBaseUnits((ellipticalC_ * elapsedTime), lengthUnits);
}

double FireSize::getFireLength(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    return LengthUnits::fromBaseUnits((ellipticalB_ * elapsedTime * 2.0), lengthUnits);;
}

double FireSize::getMaxFireWidth(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    return LengthUnits::fromBaseUnits((ellipticalA_ * elapsedTime * 2.0), lengthUnits);;
}

void FireSize::calculateFireLengthToWidthRatio()
{
    if (effectiveWindSpeed_ > 1.0e-07)
    {
        fireLengthToWidthRatio_ = .936*exp(.1147*effectiveWindSpeed_)+.461*exp(-.0692*effectiveWindSpeed_)-.397;

        // maximum eccentricity
        if (fireLengthToWidthRatio_>8.0)
            fireLengthToWidthRatio_=8.0;
    }
    else
    {
        fireLengthToWidthRatio_ = 1.0;
    }
}

void FireSize::calculateSurfaceFireEccentricity()
{
    eccentricity_ = 0.0;
    double x = (fireLengthToWidthRatio_ * fireLengthToWidthRatio_) - 1.0;
    if (x > 0.0)
    {
        eccentricity_ = sqrt(x) / fireLengthToWidthRatio_;
    }
}

void FireSize::calculateEllipticalDimensions()
{
    ellipticalA_ = 0.0;
    ellipticalB_ = 0.0;
    ellipticalC_ = 0.0;
    headingToBackingRatio_ = 0.0;          // Alexander 1985 heading/backing ratio

    double part = 0.0; // Intermediate variable used to calculate Alexandar 1985 ratio
    // Internally A, B, and C are in terms of ft travelled in one minute
    ellipticalB_ = (forwardSpreadRate_ + backingSpreadRate_) / 2.0;
    if (fireLengthToWidthRatio_ > 1e-07)
    {
        part = sqrt(pow(fireLengthToWidthRatio_, 2)-1);
        headingToBackingRatio_ =(fireLengthToWidthRatio_+part)/(fireLengthToWidthRatio_-part);

        ellipticalA_ = ellipticalB_ / fireLengthToWidthRatio_;
    }
    ellipticalC_ = ellipticalB_ - backingSpreadRate_;
}

void FireSize::calculateBackingSpreadRate()
{
    backingSpreadRate_ = forwardSpreadRate_ * (1.0 - eccentricity_) / (1.0 + eccentricity_);
}

void FireSize::calculateFlankingSpreadRate()
{
    const double fireLength = backingSpreadRate_ + forwardSpreadRate_;
    const double width = fireLength / fireLengthToWidthRatio_;
    flankingSpreadRate_ = width * 0.5;
}

double FireSize::getFirePerimeter(LengthUnits::LengthUnitsEnum lengthUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    double perimeter = 0;
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    double myEllipticalA = ellipticalA_ * elapsedTime;
    double myEllipticalB = ellipticalB_ * elapsedTime;
    if((myEllipticalA + myEllipticalB) > 1.0e-07)
    {
        double aMinusB = (myEllipticalA - myEllipticalB);
        double aMinusBSquared = aMinusB * aMinusB;
        double aPlusB = (myEllipticalA + myEllipticalB);
        double aPlusBSquared = aPlusB * aPlusB;
        double h = aMinusBSquared / aPlusBSquared;
        perimeter = M_PI * aPlusB * (1 + (h / 4.0) + ((h*h) / 64.0));
    } 
    return LengthUnits::fromBaseUnits(perimeter, lengthUnits);
}

double FireSize::getHeadingToBackingRatio() const
{
    return headingToBackingRatio_;
}

double FireSize::getFireArea(AreaUnits::AreaUnitsEnum areaUnits, double elapsedTime, TimeUnits::TimeUnitsEnum timeUnits) const
{
    elapsedTime = TimeUnits::toBaseUnits(elapsedTime, timeUnits);
    return AreaUnits::fromBaseUnits(M_PI * ellipticalA_ * ellipticalB_ * elapsedTime * elapsedTime, areaUnits);
}
