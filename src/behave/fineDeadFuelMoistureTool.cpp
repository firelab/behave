#include "finedeadfuelmoisturetool.h"

FineDeadFuelMoistureTool::FineDeadFuelMoistureTool()
{
    referenceMoisture_ = -1;
    correctionMoisture_ = -1;
    fineDeadFuelMoisture_ = -1;

    const int numAspectValues = 4;
    aspects_.resize(numAspectValues);
    aspects_ =
    {
        "North",
        "East",
        "South",
        "West"
    };

    const int numDryBulbValues = 6;
    dryBulbTemperatures_.resize(numDryBulbValues);
    dryBulbTemperatures_ = 
    {
        "10 - 29 oF",
        "30 - 49 oF",
        "50 - 69 oF",
        "70 - 89 oF",
        "90 -109 oF",
        "> 109 oF"
    };

    const int numElevationValues = 3;
    elevations_.resize(numElevationValues);
    elevations_ =
    {
        "Below (1000 - 2000 ft)",
        "Level (within 1000 ft)",
        "Above (1000 - 2000 ft)"
    };

    const int numMonthValues = 3;
    months_.resize(numMonthValues);
    months_ =
    {
        "May June July",
        "Feb Mar Apr Aug Sep Oct",
        "Nov Dec Jan"
    };

    const int numRelativeHumidityValues = 21;
    relativeHumidities_.resize(numRelativeHumidityValues);
    relativeHumidities_ =
    {
        " 0 -  4 %",
        " 5 -  9 %",
        "10 - 14 %",
        "15 - 19 %",
        "20 - 24 %",
        "25 - 29 %",
        "30 - 34 %",
        "35 - 39 %",
        "40 - 44 %",
        "45 - 49 %",
        "50 - 54 %",
        "55 - 59 %",
        "60 - 64 %",
        "65 - 69 %",
        "70 - 74 %",
        "75 - 79 %",
        "80 - 84 %",
        "85 - 89 %",
        "90 - 94 %",
        "95 - 99 %",
        "100 %"
    };

    const int numShadingValues = 2;
    shadings_.resize(numShadingValues);
    shadings_ =
    {
        "Exposed (< 50% shading)",
        "Shaded (>=50% shading)"
    };

    const int numSlopeValues = 2;
    slopes_.resize(numSlopeValues);
    slopes_ =
    {
        "0 - 30%",
        "31+ %"
    };

    const int numTimeOfDayValues = 6;
    timesOfDay_.resize(numTimeOfDayValues);
    timesOfDay_ =
    {
        "08:00 - 09:59",
        "10:00 - 11:59",
        "12:00 - 13:59",
        "14:00 - 15:59",
        "16:00 - 17:59",
        "18:00 - Sunset"
    };

    referenceMostures_.resize(numDryBulbValues);
    for (int i = 0; i < numDryBulbValues; i++)
    {
        referenceMostures_.resize(numRelativeHumidityValues);
    }
    referenceMostures_ =
    {
        { 1, 2, 2, 3, 4, 5, 5, 6, 7, 8, 8, 8, 9, 9, 10, 11, 12, 12, 13, 13, 14 },
        { 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 7, 8, 9, 9, 10, 10, 11, 12, 13, 13, 13 },
        { 1, 2, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9,  9, 10, 11, 12, 12, 12, 13 },
        { 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 8, 8,  9, 10, 10, 11, 12, 12, 13 },
        { 1, 1, 2, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 8,  9, 10, 10, 11, 12, 12, 13 },
        { 1, 1, 2, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 8,  9, 10, 10, 11, 12, 12, 12 }
    };

    const int numCorrectionRows = 36;
    const int numCorrectionColumns = 18;
    correctionMoistures_.resize(numCorrectionRows);
    for (int i = 0; i < numCorrectionRows; i++)
    {
        correctionMoistures_.resize(numCorrectionColumns);
    }
    correctionMoistures_ =
    {
        // May-Jun-Jul Exposed
        { 2, 3, 4, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 3, 4 },
        { 3, 4, 4, 1, 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 3, 4, 4 },
        { 2, 2, 3, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 2, 3, 4, 4 },
        { 1, 2, 2, 0, 0, 1, 0, 0, 1, 1, 1, 2, 2, 3, 4, 4, 5, 6 },
        { 2, 3, 3, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 3, 3 },
        { 2, 3, 3, 1, 1, 2, 0, 1, 1, 0, 1, 1, 1, 1, 2, 2, 3, 3 },
        { 2, 3, 4, 1, 1, 2, 0, 0, 1, 0, 0, 1, 0, 1, 1, 2, 3, 3 },
        { 4, 5, 6, 2, 3, 4, 1, 1, 2, 0, 0, 1, 0, 0, 1, 1, 2, 2 },
        // May-Jun-Jul Shaded
        { 4, 5, 5, 3, 4, 5, 3, 3, 4, 3, 3, 4, 3, 4, 5, 4, 5, 5 },
        { 4, 4, 5, 3, 4, 5, 3, 3, 4, 3, 4, 4, 3, 4, 5, 4, 5, 6 },
        { 4, 4, 5, 3, 4, 5, 3, 3, 4, 3, 3, 4, 3, 4, 5, 4, 5, 5 },
        { 4, 5, 6, 3, 4, 5, 3, 3, 4, 3, 3, 4, 3, 4, 5, 4, 4, 5 },
        // Feb-Mar-Apr/Aug-Sep-Oct Exposed
        { 3, 4, 5, 1, 2, 3, 1, 1, 2, 1, 1, 2, 1, 2, 3, 3, 4, 5 },
        { 3, 4, 5, 3, 3, 4, 2, 3, 4, 2, 3, 4, 3, 3, 4, 3, 4, 5 },
        { 3, 4, 5, 1, 2, 3, 1, 1, 1, 1, 1, 2, 1, 2, 3, 3, 4, 5 },
        { 3, 3, 4, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 4, 5, 3, 4, 6 },
        { 3, 4, 5, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 4, 5 },
        { 3, 4, 5, 1, 2, 2, 0, 1, 1, 0, 1, 1, 1, 2, 2, 3, 4, 5 },
        { 3, 4, 5, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 4, 5 },
        { 4, 5, 6, 3, 4, 5, 1, 2, 3, 1, 1, 1, 1, 1, 1, 3, 3, 4 },
        // Feb-Mar-Apr/Aug-Sep-Oct Shaded
        { 4, 5, 6, 4, 5, 5, 3, 4, 5, 3, 4, 5, 4, 5, 5, 4, 5, 6 },
        { 4, 5, 6, 3, 4, 5, 3, 4, 5, 3, 4, 5, 4, 5, 6, 4, 5, 6 },
        { 4, 5, 6, 3, 4, 5, 3, 4, 5, 3, 4, 5, 3, 4, 5, 4, 5, 6 },
        { 4, 5, 6, 4, 5, 6, 3, 4, 5, 3, 4, 5, 3, 4, 5, 4, 5, 6 },
        // Nov-Dec-Jan Exposed
        { 4, 5, 6, 3, 4, 5, 2, 3, 4, 2, 3, 4, 3, 4, 5, 4, 5, 6 },
        { 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6 },
        { 4, 5, 6, 3, 4, 4, 2, 3, 3, 2, 3, 3, 3, 4, 5, 4, 5, 6 },
        { 4, 5, 6, 2, 3, 4, 2, 2, 3, 3, 4, 4, 4, 5, 6, 4, 5, 6 },
        { 4, 5, 6, 3, 4, 5, 2, 3, 3, 2, 2, 3, 3, 4, 4, 4, 5, 6 },
        { 4, 5, 6, 2, 3, 3, 1, 1, 2, 1, 1, 2, 2, 3, 3, 4, 5, 6 },
        { 4, 5, 6, 3, 4, 5, 2, 3, 3, 2, 3, 3, 3, 4, 4, 4, 5, 6 },
        { 4, 5, 6, 4, 5, 6, 3, 4, 4, 2, 2, 3, 2, 3, 4, 4, 5, 6 },
        // Nov-Dec-Jan Shaded
        { 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6 },
        { 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6 },
        { 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6 },
        { 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6, 4, 5, 6 }
    };
}

FineDeadFuelMoistureTool::~FineDeadFuelMoistureTool()
{

}

void FineDeadFuelMoistureTool::calculate(const FDFMToolAspectIndex::AspectIndexEnum aspectIndex,
    const FDFMToolDryBulbIndex::DryBulbIndexEnum dryBulbIndex,
    const FDFMToolElevationIndex::ElevationIndexEnum elevationIndex,
    const FDFMToolMonthIndex::MonthIndexEnum monthIndex,
    const FDFMToolRHIndex::RHIndexEnum relativeHumidityIndex,
    const FDFMToolShadingIndex::ShadingIndexEnum shadingIndex,
    const FDFMToolSlopeIndex::SlopeIndexEnum slopeIndex,
    const FDFMToolTimeOfDayIndex::TimeOfDayIndexEnum timeOfDayIndex)
{
    const int aspectIndexInt = static_cast<int>(aspectIndex);
    const int dryBulbIndexInt = static_cast<int>(dryBulbIndex);
    const int elevationIndexInt = static_cast<int>(elevationIndex);
    const int monthIndexInt = static_cast<int>(monthIndex);
    const int relativeHumidityIndexInt = static_cast<int>(relativeHumidityIndex);
    const int shadingIndexInt = static_cast<int>(shadingIndex);
    const int slopeIndexInt = static_cast<int>(slopeIndex);
    const int timeOfDayIndexInt = static_cast<int>(timeOfDayIndex);

    calculateByIndex(aspectIndexInt, dryBulbIndexInt,
        elevationIndexInt, monthIndexInt,
        relativeHumidityIndexInt, shadingIndexInt,
        slopeIndexInt, timeOfDayIndexInt);
}

void FineDeadFuelMoistureTool::calculateByIndex(const int aspectIndex, const int dryBulbIndex,
    const int elevationIndex, const int monthIndex, const int relativeHumidityIndex,
    const int shadingIndex, const int slopeIndex, const int timeOfDayIndex)
{
    referenceMoisture_ = -1;
    correctionMoisture_ = -1;
    fineDeadFuelMoisture_ = -1;

    // Check index bounds
    if (aspectIndex >= 0 && aspectIndex < aspects_.size() &&
        dryBulbIndex  >= 0 && dryBulbIndex < dryBulbTemperatures_.size() &&
        elevationIndex >= 0 && elevationIndex < elevations_.size() &&
        monthIndex >= 0 && months_.size() &&
        relativeHumidityIndex >= 0 && relativeHumidities_.size() &&
        shadingIndex >= 0 && shadings_.size() &&
        slopeIndex >= 0 && slopes_.size() &&
        timeOfDayIndex >= 0 && timesOfDay_.size())
    {
        // Determine reference fuel moisture
        referenceMoisture_ = referenceMostures_[dryBulbIndex][relativeHumidityIndex];

        // Determine fuel moisture correction
        // Correction table row
        int row = (shadingIndex == 0)
            ? (slopeIndex + 2 * aspectIndex)
            : (8 + aspectIndex);
        row += 12 * monthIndex;

        // Correction table column
        const int column = elevationIndex + 3 * timeOfDayIndex;
        correctionMoisture_ = correctionMoistures_[row][column];

        // Corrected fuel moisture
        fineDeadFuelMoisture_ = referenceMoisture_ + correctionMoisture_;
    }
}

int FineDeadFuelMoistureTool::getReferenceMoisture() const
{
    return referenceMoisture_;
}

int FineDeadFuelMoistureTool::getCorrectionMoisture() const
{
    return correctionMoisture_;
}

int FineDeadFuelMoistureTool::getFineDeadFuelMoisture() const
{
    return fineDeadFuelMoisture_;
}

int FineDeadFuelMoistureTool::getAspectIndexSize() const
{
    return aspects_.size();
}

int FineDeadFuelMoistureTool::getDryBulbTemperatureIndexSize() const
{
    return dryBulbTemperatures_.size();
}

int FineDeadFuelMoistureTool::getElevationIndexSize() const
{
    return elevations_.size();
}

int FineDeadFuelMoistureTool::getMonthIndexSize() const
{
    return months_.size();
}

int FineDeadFuelMoistureTool::getRelativeHumidityIndexSize() const
{
    return relativeHumidities_.size();
}

int FineDeadFuelMoistureTool::getSlopeIndexSize() const
{
    return slopes_.size();
}

int FineDeadFuelMoistureTool::getShadingIndexSize() const
{
    return shadings_.size();
}

int FineDeadFuelMoistureTool::getTimeOfDayIndexSize() const
{
    return timesOfDay_.size();
}

std::string FineDeadFuelMoistureTool::getAspectLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < aspects_.size())
    {
        resultString = aspects_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getDryBulbTemperatureLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < dryBulbTemperatures_.size())
    {
        resultString = dryBulbTemperatures_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getElevationLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < elevations_.size())
    {
        resultString = elevations_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getMonthLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < months_.size())
    {
        resultString = months_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getRelativeHumidityLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < relativeHumidities_.size())
    {
        resultString = relativeHumidities_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getSlopeLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < slopes_.size())
    {
        resultString = slopes_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getShadingLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < shadings_.size())
    {
        resultString = shadings_[index];
    }
    return resultString;
}

std::string FineDeadFuelMoistureTool::getTimeOfDayLabelAtIndex(const int index) const
{
    std::string resultString = "error";
    if (index < timesOfDay_.size())
    {
        resultString = timesOfDay_[index];
    }
    return resultString;
}
