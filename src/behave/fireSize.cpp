#include "fireSize.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include "behaveUnits.h"

FireSize::FireSize()
{

}

FireSize::~FireSize()
{

}

void FireSize::calculateFireDimensions(double effectiveWindSpeed, double forwardSpreadRate, double elapsedTime)
{
    effectiveWindSpeed_ = effectiveWindSpeed;
    forwardSpreadRate_ = forwardSpreadRate; // spread rate is feet per minute
    elapsedTime_ = TimeUnits::fromBaseUnits(elapsedTime, TimeUnits::Minutes); // convert time units to minutes

    calculateFireLengthToWidthRatio();
    calculateSurfaceFireEccentricity();
    calculateBackingSpreadRate();
    forwardSpreadDistance_ = forwardSpreadRate_ * elapsedTime_;
    backingSpreadDistance_ = backingSpreadRate_ * elapsedTime_;
    calculateEllipticalDimensions();
    calculateFirePerimeter();
    calculateFireArea();
}

double FireSize::getFireLengthToWidthRatio() const
{
    return fireLengthToWidthRatio_;
}

double FireSize::getEccentricity() const
{
    return eccentricity_;
}

double FireSize::getBackingSpreadRate() const
{
    return backingSpreadRate_;
}

double FireSize::getEllipticalA() const
{
    return ellipticalA_;
}

double FireSize::getEllipticalB() const
{
    return ellipticalB_;
}

double FireSize::getEllipticalC() const
{
    return ellipticalC_;
}

double FireSize::getFirePerimeter() const
{
    return perimeter_;
}

void FireSize::calculateFireLengthToWidthRatio()
{
    if (effectiveWindSpeed_ > 1.0e-07)
    {
        fireLengthToWidthRatio_ = 1.0 + (0.25 * effectiveWindSpeed_);
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

    // Internally A, B, and C are in terms of ft
    ellipticalB_ = (forwardSpreadDistance_ + backingSpreadDistance_) / 2;
    if (fireLengthToWidthRatio_ > 1e-07)
    {
        ellipticalA_ = ellipticalB_ / fireLengthToWidthRatio_;
    }
    ellipticalC_ = ellipticalB_ - backingSpreadDistance_;
}

void FireSize::calculateBackingSpreadRate()
{
    backingSpreadRate_ = forwardSpreadRate_ * (1.0 - eccentricity_) / (1.0 + eccentricity_);
}

void FireSize::calculateFirePerimeter()
{
    perimeter_ = 0;
    if((ellipticalA_ + ellipticalB_) > 1.0e-07)
    {
        double aMinusB = (ellipticalA_ - ellipticalB_);
        double aMinusBSquared = aMinusB * aMinusB;
        double aPlusB = (ellipticalA_ + ellipticalB_);
        double aPlusBSquared = aPlusB * aPlusB;
        double h = aMinusBSquared / aPlusBSquared;
        double pi = M_PI;
        perimeter_ = pi * aPlusB * (1 + (h/4.0) + ((h * h)/64.0));
    } 
}

void FireSize::calculateFireArea()
{
    double pi = M_PI;
    area_ = pi * ellipticalA_ * ellipticalB_;
}
