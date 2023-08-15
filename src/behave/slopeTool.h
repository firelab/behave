#include <vector>
#include "behaveUnits.h"

struct RepresentativeFraction
{
    enum RepresentativeFractionEnum
    {
        NINTEEN_HUNDRED_EIGHTY,
        THREE_THOUSAND_NINEHUNDRED_SIXTY,
        SEVEN_THOUSAND_NINEHUNDRED_TWENTY,
        TEN_THOUSAND,
        FIFTEEN_THOUSAND_EIGHT_HUNDRED_FORTY,
        TWENTY_ONE_THOUSAND_ONE_HUNDRED_TWENTY,
        TWENTY_FOUR_THOUSAND,
        THRITY_ONE_THOUSAND_SIX_HUNDRED_EIGHTY,
        FIFTY_THOUSAND,
        SIXTY_TWO_THOUSAND_FIVE_HUNDRED,
        SIXTY_THREE_THOUSAND_THREE_HUNDRED_SIXTY,
        ONE_HUNDRED_THOUSAND,
        ONE_HUNDRED_TWENTY_SIX_THOUSAND_SEVEN_HUNDRED_TWENTY,
        TWO_HUNDRED_FIFTY_THOUSAND,
        TWO_HUNDRED_FIFTY_THREE_THOUSAND_FOUR_HUNDRED_FORTY,
        FIVE_HUNDRED_SIX_THOUSAND_EIGHT_HUNDRED_EIGHTY,
        ONE_MILLION,
        ONE_MILLION_THIRTEEN_THOUSAND_SEVEN_HUNDRED_SIXTY
    }; 
};

struct HorizontalDistanceIndex
{
    enum HorizontalDistanceIndexEnum
    {
        UPSLOPE_ZERO_DEGREES,
        FIFTEEN_DEGREES_FROM_UPSLOPE,
        THIRTY_DEGREES_FROM_UPSLOPE,
        FORTY_FIVE_DEGREES_FROM_UPSLOPE,
        SIXTY_DEGREES_FROM_UPSLOPE,
        SEVENTY_FIVE_DEGREES_FROM_UPSLOPE,
        CROSS_SLOPE_NINETY_DEGREES
    };
};

class SlopeTool
{
public:
    SlopeTool();
    ~SlopeTool();
    SlopeTool(const SlopeTool& rhs);
    SlopeTool& operator=(const SlopeTool& rhs);

    void calculateHorizontalDistance(const double distanceInInchesOrCentimeters, const double maxSlopeSteepness,
        const SlopeUnits::SlopeUnitsEnum slopeUnits);
    void calculateSlopeFromMapMeasurements(const int mapRepresentativeFraction, const double mapDistance,
        LengthUnits::LengthUnitsEnum distanceUnits,  const double contourInterval, const int numberOfContours);

    // calculateHorizontalDistance() getters
    int getNumberOfHorizontalDistances() const;
    double getMaxSlopeInDegrees() const;
    double getMaxSlopeInPercent() const;
    double getHorizontalDistance(HorizontalDistanceIndex::HorizontalDistanceIndexEnum horizontalDistanceIndex) const;
    double getHorizontalDistanceAtIndex(const int index) const;

    // calculateSlopeFromMapMeasurements() getters
    double getSlopeInDegrees() const;
    double getSlopeInPercent() const;
    double getSlopeHorizontalDistance() const;
    double getSlopeElevationChange() const;

    // representative fraction table getters
    int getNumberOfRepresentativeFractions() const;
    int getRepresentativeFractionAtIndex(const int index) const;
    double getInchesPerMileAtIndex(const int index) const;
    double getMilesPerInchAtIndex(const int index) const;
    double getCentimetersPerKilometerAtIndex(const int index) const;
    double getKilometersPerCentimeterAtIndex(const int index) const;
    int getRepresentativeFractionAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const;
    double getInchesPerMileAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const;
    double getMilesPerInchAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const;
    double getCentimetersPerKilometerAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const;
    double getKilometersPerCentimeterAtRepresentativeFraction(const RepresentativeFraction::RepresentativeFractionEnum representativeFraction) const;

protected:
    void memberwiseCopyAssignment(const SlopeTool& rhs);

    double maxSlopeInDegrees_; // calcuated by calculateHorizontalDistance()
    double maxSlopeInPercent_; // calcuated by calculateHorizontalDistance()
    std::vector<double> horizontalDistances_; // calcuated by calculateHorizontalDistance()

    double slopeInDegrees_; // calcuated by calculateSlopeFromMapMeasurements()
    double slopeInPercent_; // calcuated by calculateSlopeFromMapMeasurements()
    double slopeHorizontalDistance_; // calcuated by calculateSlopeFromMapMeasurements()
    double slopeElevationChange_; // calcuated by calculateSlopeFromMapMeasurements()
   

    std::vector<int> representativeFractions_; // representative fractions for representative fraction table
    std::vector<double> inchesPerMile_; // inches per mile for representative fraction table
    std::vector<double> milesPerInch_; // miles per inch for representative fraction table
    std::vector<double> centimetersPerKilometer_; // centimeters per kilometer for representative fraction table
    std::vector<double> kilometersPerCentimeter_; // kilometers per centimeter for representative fraction table
};
