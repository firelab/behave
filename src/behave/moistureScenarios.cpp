#include "moistureScenarios.h"

#include <algorithm>

MoistureScenarios::MoistureScenarios()
{
    populateMoistureScenarios();
}

MoistureScenarios::~MoistureScenarios()
{

}

MoistureScenarios& MoistureScenarios::operator=(const MoistureScenarios& rhs)
{
    if(this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

MoistureScenarios::MoistureScenarios(const MoistureScenarios& rhs)
{
    memberwiseCopyAssignment(rhs);
}

int MoistureScenarios::getNumberOfMoistureScenarios()
{
    return moistureScenarioVector_.size();
}

int MoistureScenarios::getMoistureScenarioIndexByName(const std::string name)
{
    int index = -1;
    std::string searchNameAllUppercase = name;
    std::transform(searchNameAllUppercase.begin(), searchNameAllUppercase.end(), searchNameAllUppercase.begin(), ::toupper);
    for(int i = 0; i < moistureScenarioVector_.size(); i++)
    {
        std::string currentNameAllUppercase = moistureScenarioVector_[i].name_;
        std::transform(currentNameAllUppercase.begin(), currentNameAllUppercase.end(), currentNameAllUppercase.begin(), ::toupper);
        if(currentNameAllUppercase == searchNameAllUppercase)
        {
            index = i;
            break;
        }
    }
    return index;
}

bool MoistureScenarios::getIsMoistureScenarioDefinedByName(const std::string name)
{
    bool isMoistureScenarioDefined = false;
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        isMoistureScenarioDefined = true;
    }
    return isMoistureScenarioDefined;
}

std::string MoistureScenarios::getMoistureScenarioDescriptionByName(const std::string name)
{
    std::string description = "Error: Scenario " + name + " is not defined";
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        description = moistureScenarioVector_[index].description_;
    }
    return description;
}

double MoistureScenarios::getMoistureScenarioOneHourByName(const std::string name)
{
    double moistureOneHour = -1.0;
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureOneHour = moistureScenarioVector_[index].moistureOneHour_;
    }
    return moistureOneHour;
}

double MoistureScenarios::getMoistureScenarioTenHourByName(const std::string name)
{
    double moistureTenHour = -1.0;
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureTenHour = moistureScenarioVector_[index].moistureTenHour_;
    }
    return moistureTenHour;
}

double MoistureScenarios::getMoistureScenarioHundredHourByName(const std::string name)
{
    double moistureHundredHour = -1.0;
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureHundredHour = moistureScenarioVector_[index].moistureHundredHour_;
    }
    return moistureHundredHour;
}

double MoistureScenarios::getMoistureScenarioLiveHerbaceousByName(const std::string name)
{
    double moistureLiveHerbaceous = -1.0;
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureLiveHerbaceous = moistureScenarioVector_[index].moistureLiveHerbaceous_;
    }
    return moistureLiveHerbaceous;
}

double MoistureScenarios::getMoistureScenarioLiveWoodyByName(const std::string name)
{
    double moistureLiveWoody = -1.0;
    int index = getMoistureScenarioIndexByName(name);
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureLiveWoody = moistureScenarioVector_[index].moistureLiveWoody_;
    }
    return moistureLiveWoody;
}

bool MoistureScenarios::getIsMoistureScenarioDefinedByIndex(const int index)
{
    bool isMoistureScenarioDefined = false;

    if((moistureScenarioVector_.size() > 0) && (index >= 0) && (index < moistureScenarioVector_.size()))
    {
        isMoistureScenarioDefined = true;
    }
    return isMoistureScenarioDefined;
}

std::string MoistureScenarios::getMoistureScenarioNameByIndex(const int index)
{
    std::string name = "Error: Scenario with vector index " + std::to_string(index) + " is not defined";
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        name = moistureScenarioVector_[index].name_;
    }
    return name;
}

std::string MoistureScenarios::getMoistureScenarioDescriptionByIndex(const int index)
{
    std::string description = "Error: Scenario with vector index " + std::to_string(index) + " is not defined";
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        description = moistureScenarioVector_[index].description_;
    }
    return description;
}

double MoistureScenarios::getMoistureScenarioOneHourByIndex(const int index)
{
    double moistureOneHour = -1.0;
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureOneHour = moistureScenarioVector_[index].moistureOneHour_;
    }
    return moistureOneHour;
}

double MoistureScenarios::getMoistureScenarioTenHourByIndex(const int index)
{
    double moistureTenHour = -1.0;
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureTenHour = moistureScenarioVector_[index].moistureTenHour_;
    }
    return moistureTenHour;
}

double MoistureScenarios::getMoistureScenarioHundredHourByIndex(const int index)
{
    double moistureHundredHour = -1.0;
    if((index >= 0) && (index < moistureScenarioVector_.size()))
    {
        moistureHundredHour = moistureScenarioVector_[index].moistureHundredHour_;
    }
    return moistureHundredHour;
}

double MoistureScenarios::getMoistureScenarioLiveHerbaceousByIndex(const int vectorIndex)
{
    double moistureLiveHerbaceous = -1.0;
    if((vectorIndex >= 0) && (vectorIndex < moistureScenarioVector_.size()))
    {
        moistureLiveHerbaceous = moistureScenarioVector_[vectorIndex].moistureLiveHerbaceous_;
    }
    return moistureLiveHerbaceous;
}

double MoistureScenarios::getMoistureScenarioLiveWoodyByIndex(const int vectorIndex)
{
    double moistureLiveWoody = -1.0;
    if((vectorIndex >= 0) && (vectorIndex < moistureScenarioVector_.size()))
    {
        moistureLiveWoody = moistureScenarioVector_[vectorIndex].moistureLiveWoody_;
    }
    return moistureLiveWoody;
}

void MoistureScenarios::memberwiseCopyAssignment(const MoistureScenarios& rhs)
{
    moistureScenarioVector_ = rhs.moistureScenarioVector_;
}

void MoistureScenarios::setMoistureScenarioRecord(const std::string name, const std::string description,
    const double moistureOneHour, const double moistureTenHour, const double moistureHundredHour,
    const double moistureLiveHerbaceous, double moistureLiveWoody)
{
    MoistureScenarioRecord record;
    record.name_ = name;
    record.description_ = description;
    record.moistureOneHour_ = moistureOneHour;
    record.moistureTenHour_ = moistureTenHour;
    record.moistureHundredHour_ = moistureHundredHour;
    record.moistureLiveHerbaceous_ = moistureLiveHerbaceous;
    record.moistureLiveWoody_ = moistureLiveWoody;
    moistureScenarioVector_.push_back(record);
}

void MoistureScenarios::populateMoistureScenarios()
{
    enum DeadIndex
    {
        ONE_HOUR = 0,
        TEN_HOUR = 1,
        HUNDRED_HOUR = 2
    };
    enum LiveIndex
    {
        HERBACEOUS = 0,
        WOODY = 1
    };

    std::vector<double> veryLowDead = {0.03, 0.04, 0.05};
    std::vector<double> lowDead = {0.06, 0.07, 0.08};
    std::vector<double> moderateDead = {0.09, 0.10, 0.11};
    std::vector<double> highDead = {0.12, 0.13, 0.14};

    std::vector<double> fullyCuredHerb = {0.30, 0.60};
    std::vector<double> twoThirdsCuredHerb = {0.60, 0.90};
    std::vector<double> oneThirdCuredHerb = {0.90, 1.20};
    std::vector<double> fullyGreenHerb = {1.20, 1.50};

    // Very low dead scenarios
    setMoistureScenarioRecord("D1L1", "D1L1 - Very low dead, fully cured herb (3,4,5,30,60)",
        veryLowDead[ONE_HOUR], veryLowDead[TEN_HOUR], veryLowDead[HUNDRED_HOUR],
        fullyCuredHerb[HERBACEOUS], fullyCuredHerb[WOODY]);
    setMoistureScenarioRecord("D1L2", "D1L2 - Very low dead, 2/3 cured herb (3,4,5,60,90)",
        veryLowDead[ONE_HOUR], veryLowDead[TEN_HOUR], veryLowDead[HUNDRED_HOUR],
        twoThirdsCuredHerb[HERBACEOUS], twoThirdsCuredHerb[WOODY]);
    setMoistureScenarioRecord("D1L3", "D1L3 - Very low dead, 1/3 cured herb (3,4,5,90,120)",
        veryLowDead[ONE_HOUR], veryLowDead[TEN_HOUR], veryLowDead[HUNDRED_HOUR],
        oneThirdCuredHerb[HERBACEOUS], oneThirdCuredHerb[WOODY]);
    setMoistureScenarioRecord("D1L4", "D1L4 - Very low dead, fully green herb (3,4,5,120,150)",
        veryLowDead[ONE_HOUR], veryLowDead[TEN_HOUR], veryLowDead[HUNDRED_HOUR],
        fullyGreenHerb[HERBACEOUS], fullyGreenHerb[WOODY]);

    // Low dead scenarios
    setMoistureScenarioRecord("D2L1", "D2L1 - Low dead, fully cured herb (6,7,8,30,60)",
        lowDead[ONE_HOUR], lowDead[TEN_HOUR], lowDead[HUNDRED_HOUR],
        fullyCuredHerb[HERBACEOUS], fullyCuredHerb[WOODY]);
    setMoistureScenarioRecord("D2L2", "D2L2 - Low dead, 2/3 cured herb (6,7,8,60,90)",
        lowDead[ONE_HOUR], lowDead[TEN_HOUR], lowDead[HUNDRED_HOUR],
        twoThirdsCuredHerb[HERBACEOUS], twoThirdsCuredHerb[WOODY]);
    setMoistureScenarioRecord("D2L3", "D2L3 - Low dead, 1/3 cured herb (6,7,8,90,120)",
        lowDead[ONE_HOUR], lowDead[TEN_HOUR], lowDead[HUNDRED_HOUR],
        oneThirdCuredHerb[HERBACEOUS], oneThirdCuredHerb[WOODY]);
    setMoistureScenarioRecord("D2L4", "D2L4 - Low dead, fully green herb (6,7,8,120,150)",
        lowDead[ONE_HOUR], lowDead[TEN_HOUR], lowDead[HUNDRED_HOUR],
        fullyGreenHerb[HERBACEOUS], fullyGreenHerb[WOODY]);

    // Moderate dead scenarios
    setMoistureScenarioRecord("D3L1", "D3L1 - Moderate dead, fully cured herb (9,10,11,30,60)",
        moderateDead[ONE_HOUR], moderateDead[TEN_HOUR], moderateDead[HUNDRED_HOUR],
        fullyCuredHerb[HERBACEOUS], fullyCuredHerb[WOODY]);
    setMoistureScenarioRecord("D3L2", "D3L2 - Moderate dead, 2/3 cured herb (9,10,11,60,90)",
        moderateDead[ONE_HOUR], moderateDead[TEN_HOUR], moderateDead[HUNDRED_HOUR],
        twoThirdsCuredHerb[HERBACEOUS], twoThirdsCuredHerb[WOODY]);
    setMoistureScenarioRecord("D3L3", "D3L3 - Moderate dead, 1/3 cured herb (9,10,11,90,120)",
        moderateDead[ONE_HOUR], moderateDead[TEN_HOUR], moderateDead[HUNDRED_HOUR],
        oneThirdCuredHerb[HERBACEOUS], oneThirdCuredHerb[WOODY]);
    setMoistureScenarioRecord("D3L4", "D3L4 - Moderate dead, fully green herb (9,10,11,120,150)",
        moderateDead[ONE_HOUR], moderateDead[TEN_HOUR], moderateDead[HUNDRED_HOUR],
        fullyGreenHerb[HERBACEOUS], fullyGreenHerb[WOODY]);

    // High dead scenarios
    setMoistureScenarioRecord("D4L1", "D3L1 - High dead, fully cured herb (12,13,14,30,60)",
        highDead[ONE_HOUR], highDead[TEN_HOUR], highDead[HUNDRED_HOUR],
        fullyCuredHerb[HERBACEOUS], fullyCuredHerb[WOODY]);
    setMoistureScenarioRecord("D4L2", "D3L2 - High dead, 2/3 cured herb (12,13,14,60,90)",
        highDead[ONE_HOUR], highDead[TEN_HOUR], highDead[HUNDRED_HOUR],
        twoThirdsCuredHerb[HERBACEOUS], twoThirdsCuredHerb[WOODY]);
    setMoistureScenarioRecord("D4L3", "D3L3 - High dead, 1/3 cured herb (12,13,14,90,120)",
        highDead[ONE_HOUR], highDead[TEN_HOUR], highDead[HUNDRED_HOUR],
        oneThirdCuredHerb[HERBACEOUS], oneThirdCuredHerb[WOODY]);
    setMoistureScenarioRecord("D4L4", "D3L4 - High dead, fully green herb (12,13,14,120,150)",
        highDead[ONE_HOUR], highDead[TEN_HOUR], highDead[HUNDRED_HOUR],
        fullyGreenHerb[HERBACEOUS], fullyGreenHerb[WOODY]);
}
