#pragma once

#include <string>
#include <vector>

struct FDFMToolAspectIndex
{
    enum AspectIndexEnum
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };
};

struct FDFMToolDryBulbIndex
{
    enum DryBulbIndexEnum
    {
        TEN_TO_TWENTY_NINE_DEGREES_F,
        THRITY_TO_FOURTY_NINE_DEGREES_F,
        FIFTY_TO_SIXTY_NINE_DEGREES_F,
        SEVENTY_TO_EIGHTY_NINE_DEGREES_F,
        NINETY_TO_ONE_HUNDRED_NINE_DEGREES_F,
        GREATER_THAN_ONE_HUNDRED_NINE_DEGREES_F
    };
};

struct FDFMToolElevationIndex
{
    enum ElevationIndexEnum
    {
        BELOW_1000_TO_2000_FT,
        LEVEL_WITHIN_1000_FT,
        ABOVE_1000_TO_2000_FT
    };
};

struct FDFMToolMonthIndex
{
    enum MonthIndexEnum
    {
        MAY_JUNE_JULY,
        FEB_MAR_APR_AUG_SEP_OCT,
        NOV_DEC_JAN
    };
};

struct FDFMToolRHIndex
{
    enum RHIndexEnum
    {
        ZERO_TO_FOUR_PERCENT,
        FIVE_TO_NINE_PERCENT,
        TEN_TO_FOURTEEN_PERCENT,
        FIFTEEN_TO_NINETEEN_PERCENT,
        TWENTY_TO_TWENTY_FOUR_PERCENT,
        TWENTY_FIVE_TO_TWENTY_NINE_PERCENT,
        THIRTY_TO_THIRTY_FOUR_PERCENT,
        THIRTY_FIVE_TO_THIRTY_NINE_PERCENT,
        FORTY_TO_FORTY_FOUR_PERCENT,
        FORTY_FIVE_TO_FORTY_NINE_PERCENT,
        FIFTY_TO_FIFTY_FOUR_PERCENT,
        FIFTY_FIVE_TO_FIFTY_NINE_PERCENT,
        SIXTY_TO_SIXTY_FOUR_PERCENT,
        SIXTY_FIVE_TO_SIXTY_NINE_PERCENT,
        SEVENTY_TO_SEVENTY_FOUR_PERCENT,
        SEVENTY_FIVE_TO_SEVENTY_NINE_PERCENT,
        EIGHTY_TO_EIGHTY_FOUR_PERCENT,
        EIGHTY_FIVE_TO_EIGHTY_NINE_PERCENT,
        NINETY_TO_NINETY_FOUR_PERCENT,
        NINETY_FIVE_TO_NINETY_NINE_PERCENT,
        ONE_HUNDRED_PERCENT
    };
};

struct FDFMToolShadingIndex
{
    enum ShadingIndexEnum
    {
        EXPOSED,
        SHADED
    };
};

struct FDFMToolSlopeIndex
{
    enum SlopeIndexEnum
    {
        ZERO_TO_THIRTY_PERCENT,
        GREATER_THAN_OR_EQUAL_TO_THIRTY_ONE_PERCENT
    };
};

struct FDFMToolTimeOfDayIndex
{
    enum TimeOfDayIndexEnum
    {
        EIGHT_HUNDRED_HOURS_TO_NINE_HUNDRED_FIFTY_NINE,
        TEN_HUNDRED_HOURS_TO_ELEVEN__HUNDRED_FIFTY_NINE,
        TWELVE_HUNDRED_HOURS_TO_THIRTEEN_HUNDRED_FIFTY_NINE,
        FOURTEEN_HUNDRED_HOURS_TO_FIFTEEN_HUNDRED_FIFTY_NINE,
        SIXTEEN_HUNDRED_HOURS_TO_SIXTEEN_HUNDRED_FIFTY_NINE,
        EIGHTTEEN_HUNDRED_HOURS_TO_SUNSET,
    };
};

class FineDeadFuelMoistureTool
{
public:
    FineDeadFuelMoistureTool();
    ~FineDeadFuelMoistureTool();
    FineDeadFuelMoistureTool(const FineDeadFuelMoistureTool& rhs);
    FineDeadFuelMoistureTool& operator=(const FineDeadFuelMoistureTool& rhs);

    void calculate(const FDFMToolAspectIndex::AspectIndexEnum aspectIndex,
        const FDFMToolDryBulbIndex::DryBulbIndexEnum dryBulbIndex,
        const FDFMToolElevationIndex::ElevationIndexEnum elevationIndex,
        const FDFMToolMonthIndex::MonthIndexEnum monthIndex,
        const FDFMToolRHIndex::RHIndexEnum relativeHumidityIndex,
        const FDFMToolShadingIndex::ShadingIndexEnum shadingIndex,
        const FDFMToolSlopeIndex::SlopeIndexEnum slopeIndex,
        const FDFMToolTimeOfDayIndex::TimeOfDayIndexEnum timeOfDayIndex);

    void calculateByIndex(const int aspectIndex, const int dryBulbIndex,
        const int elevationIndex, const int monthIndex,
        const int relativeHumidityIndex, const int shadingIndex,
        const int slopeIndex, const int timeOfDayIndex);

    // Getters for the maximum valid values for various indices, applies to enums and string vectors 
    int getAspectIndexSize() const;
    int getDryBulbTemperatureIndexSize() const;
    int getElevationIndexSize() const;
    int getMonthIndexSize() const;
    int getRelativeHumidityIndexSize() const;
    int getSlopeIndexSize() const;
    int getShadingIndexSize() const;
    int getTimeOfDayIndexSize() const;

    // String vector getters, can be used to populate combo boxes/drop-downs in a gui or to get descriptions of indices
    std::string getAspectLabelAtIndex(const int index) const;
    std::string getDryBulbTemperatureLabelAtIndex(const int index) const;
    std::string getElevationLabelAtIndex(const int index) const;
    std::string getMonthLabelAtIndex(const int index) const;
    std::string getRelativeHumidityLabelAtIndex(const int index) const;
    std::string getSlopeLabelAtIndex(const int index) const;
    std::string getShadingLabelAtIndex(const int index) const;
    std::string getTimeOfDayLabelAtIndex(const int index) const;

    // Output getters (requires prior calculation)
    int getReferenceMoisture() const;
    int getCorrectionMoisture() const;
    int getFineDeadFuelMoisture() const;

protected:
    void memberwiseCopyAssignment(const FineDeadFuelMoistureTool& rhs);

    std::vector<std::string> aspects_; // aspect index labels
    std::vector<std::string> dryBulbTemperatures_; // labels for indices of ranges of dry bulb temperatures
    std::vector<std::string> elevations_; // labels for indices of ranges of elevations
    std::vector<std::string> months_; // labels for indices of ranges of months
    std::vector<std::string> relativeHumidities_; // labels for indices of ranges of humidities
    std::vector<std::string> slopes_; // labels for indices of ranges of slopes
    std::vector<std::string> shadings_; // labels for indices of shading
    std::vector<std::string> timesOfDay_; // labels for indices of ranges of time of day

    std::vector<std::vector<int>> referenceMostures_;
    std::vector<std::vector<int>> correctionMoistures_;
    
    // Outputs: results of calcuate() public member function
    int referenceMoisture_; // is a function of the dry bulb temperature and relative humidity
    int correctionMoisture_; // is a function of month, time of day, elevation difference, slope steepness, aspect, and shading
    int fineDeadFuelMoisture_; // is the sum of a reference fuel moisture and a moisture correction.
};
