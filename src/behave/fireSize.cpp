#include "fireSize.h"
#define _USE_MATH_DEFINES
#include <cmath>

FireSize::FireSize()
{

}

FireSize::~FireSize()
{

}

void FireSize::calculateFireBasicDimensions(double effectiveWindSpeed, double forwardSpreadRate)
{
    effectiveWindSpeed_ = effectiveWindSpeed;
    forwardSpreadRate_ = forwardSpreadRate; // spread rate is feet per minute

    calculateFireLengthToWidthRatio();
    calculateSurfaceFireEccentricity();
    calculateBackingSpreadRate();

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

double FireSize::getBackingSpreadRate() const
{
    return backingSpreadRate_;
}

double FireSize::getEllipticalA(double elapsedTime) const
{
    return ellipticalA_ * elapsedTime;
}

double FireSize::getEllipticalB(double elapsedTime) const
{
    return ellipticalB_ * elapsedTime;
}

double FireSize::getEllipticalC(double elapsedTime) const
{
    return ellipticalC_ * elapsedTime;
}

double FireSize::getFireLength(double elapsedTime) const
{
    return ellipticalB_ * elapsedTime * 2.0;
}

double FireSize::getMaxFireWidth(double elapsedTime) const
{
    return ellipticalA_ * elapsedTime * 2.0;
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

    // Internally A, B, and C are in terms of ft travelled in one minute
    ellipticalB_ = (forwardSpreadRate_ + backingSpreadRate_) / 2.0;
    if (fireLengthToWidthRatio_ > 1e-07)
    {
        ellipticalA_ = ellipticalB_ / fireLengthToWidthRatio_;
    }
    ellipticalC_ = ellipticalB_ - backingSpreadRate_;
}

void FireSize::calculateBackingSpreadRate()
{
    backingSpreadRate_ = forwardSpreadRate_ * (1.0 - eccentricity_) / (1.0 + eccentricity_);
}

double FireSize::calculateFirePerimeter(double elapsedTime) const
{
    double perimeter = 0;
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
    return perimeter;
}

double FireSize::calculateFireArea(double elapsedTime) const
{
    double area = 0;
    double myEllipticalA = ellipticalA_ * elapsedTime;
    double myEllipticalB = ellipticalB_ * elapsedTime;
    
    return area = M_PI * myEllipticalA * myEllipticalB;
}
