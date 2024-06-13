#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <algorithm>
#include <functional>

#include "mortality_inputs.h" 
#include "mortality.h"
#include "species_master_table.h"
#include "mortality_equation_table.h"

Mortality::Mortality(SpeciesMasterTable& speciesMasterTable)
    : boleCharTable_
{
    { 100,  2.3014,    -0.3267, 1.1137  },
    { 101, -0.8727,    -0.1814, 4.1947  },
    { 102,  2.7899,    -0.5511, 1.2888  },
    { 103,  1.9438,    -0.4602, 1.6352  },
    { 104, -1.8137,    -0.0603, 0.8666  },
    { 105, -1.6262,    -0.0339, 0.6901  },
    { 106,  0.3714,    -0.1005, 1.5577  },
    { 107, -1.4416,    -0.1469, 1.3159  },
    { 108,  0.1122,    -0.1287, 1.2612  },
    { 109,  1.6779,    -1.0299, 10.2855 },
    {  -1,        0,         0,       0 }
}
{
    speciesMasterTable_ = &speciesMasterTable;
    speciesMasterTable_->initializeMasterTable();
    initializeOutputs();
}

Mortality::Mortality(const Mortality& rhs)
{
    memberwiseCopyAssignment(rhs);
}

Mortality& Mortality::operator=(const Mortality& rhs)
{
    if(this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void Mortality::memberwiseCopyAssignment(const Mortality& rhs)
{
    mortalityInputs_ = rhs.mortalityInputs_;

    speciesMasterTable_ = rhs.speciesMasterTable_;
    equationRequiredFieldTable_ = rhs.equationRequiredFieldTable_;
    boleCharTable_ = rhs.boleCharTable_;
    canopyCoefficientTable_ = rhs.canopyCoefficientTable_;
}

Mortality::~Mortality()
{
   
}

//------------------------------------------------------------------------------
/*! \brief Calculates scorch height from fireline intensity, wind speed, and
 *  air temperature.
 *
 *  \param firelineIntensity Fireline (Byram's) intensity (btu/ft/s).
 *  \param windSpeed         Wind speed at midlame height (upslope)
 *  \param airTemperature    Air temperature (degrees F).
 *
 *  \return Scorch height
 */

double Mortality::calculateScorchHeight(double firelineIntensity, FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits,
    double midFlameWindSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits, double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits,
    LengthUnits::LengthUnitsEnum scorchHeightUnits)
{
    firelineIntensity = FirelineIntensityUnits::toBaseUnits(firelineIntensity, firelineIntensityUnits);

    double midFlameWindSpeedInBaseUnits = SpeedUnits::toBaseUnits(midFlameWindSpeed, windSpeedUnits);
    double midFlameWindSpeedInMilesPerHour = SpeedUnits::fromBaseUnits(midFlameWindSpeedInBaseUnits, SpeedUnits::MilesPerHour);

    airTemperature = TemperatureUnits::toBaseUnits(airTemperature, temperatureUnits);
    double scorchHeight = ((firelineIntensity < 1.0e-07)
        ? (0.0)
        : ((63. / (140. - airTemperature))
            * pow(firelineIntensity, 1.166667)
            / sqrt(firelineIntensity + (midFlameWindSpeedInMilesPerHour * midFlameWindSpeedInMilesPerHour * midFlameWindSpeedInMilesPerHour))
            ));
    return LengthUnits::fromBaseUnits(scorchHeight, scorchHeightUnits);
}

void Mortality::setRegion(RegionCode region)
{
    mortalityInputs_.setRegion(region);
}

void Mortality::setSpeciesCode(std::string speciesCode)
{
    mortalityInputs_.setSpeciesCode(speciesCode);
    EquationType equationType = mortalityInputs_.getEquationType();

    if(speciesCode != "" && equationType != EquationType::not_set)
    {
        updateInputsForSpeciesCodeAndEquationType(speciesCode, equationType);
    }
}

void Mortality::setEquationType(EquationType equationType)
{
    mortalityInputs_.setEquationType(equationType);
    string speciesCode = mortalityInputs_.getSpeciesCode();
  
    if(speciesCode != "" && equationType != EquationType::not_set)
    {
        updateInputsForSpeciesCodeAndEquationType(speciesCode, equationType);
    }
}

void Mortality::setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch flameLengthOrScorchHeightSwitch)
{
    mortalityInputs_.setFlameLengthOrScorchHeightSwitch(flameLengthOrScorchHeightSwitch);
}

void Mortality::setFlameLengthOrScorchHeightValue(double flameLengthOrScorchHeightValue, LengthUnits::LengthUnitsEnum flameLengthOrScorchHeightUnits)
{
    mortalityInputs_.setFlameLengthOrScorchHeightValue(flameLengthOrScorchHeightValue, flameLengthOrScorchHeightUnits);
}

void Mortality::setFlameLength(double flameLength, LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    mortalityInputs_.setFlameLength(flameLength, flameLengthUnits);
}

void Mortality::setScorchHeight(double scorchHeight, LengthUnits::LengthUnitsEnum scorchHeightUnits)
{
    mortalityInputs_.setScorchHeight(scorchHeight, scorchHeightUnits);
}

void Mortality::setTreeDensityPerUnitArea(double numberOfTrees, AreaUnits::AreaUnitsEnum areaUnits)
{
    mortalityInputs_.setTreeDensityPerUnitArea(numberOfTrees, areaUnits);
}

void Mortality::setDBH(double dbh, LengthUnits::LengthUnitsEnum diameterUnits)
{
    mortalityInputs_.setDBH(dbh, diameterUnits);
}

void Mortality::setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum treeHeightUnits)
{
    mortalityInputs_.setTreeHeight(treeHeight, treeHeightUnits);
}

void Mortality::setCrownRatio(double crownRatio)
{
    mortalityInputs_.setCrownRatio(crownRatio);
}

void Mortality::setCrownDamage(double crownDamage)
{
    mortalityInputs_.setCrownDamage(crownDamage);
}

void Mortality::setCambiumKillRating(double cambiumKillRating)
{
    mortalityInputs_.setCambiumKillRating(cambiumKillRating);
}

void Mortality::setBeetleDamage(BeetleDamage beetleDamage)
{
    mortalityInputs_.setBeetleDamage(beetleDamage);
}

void Mortality::setBoleCharHeight(double boleCharHeight, LengthUnits::LengthUnitsEnum boleCharHeightUnits)
{
    mortalityInputs_.setBoleCharHeight(boleCharHeight, boleCharHeightUnits);
}

void Mortality::setFireSeverity(FireSeverity fireSeverity)
{
    mortalityInputs_.setFireSeverity(fireSeverity);
}

void Mortality::setFirelineIntensity(double firelineIntensity, FirelineIntensityUnits::FirelineIntensityUnitsEnum firelineIntensityUnits)
{
    mortalityInputs_.setFirelineIntensity(firelineIntensity, firelineIntensityUnits);
}

void Mortality::setMidFlameWindSpeed(double midFlameWindSpeed, SpeedUnits::SpeedUnitsEnum windSpeedUnits)
{
    mortalityInputs_.setMidFlameWindSpeed(midFlameWindSpeed, windSpeedUnits);
}
void Mortality::setAirTemperature(double airTemperature, TemperatureUnits::TemperatureUnitsEnum temperatureUnits)
{
    mortalityInputs_.setAirTemperature(airTemperature, temperatureUnits);
}

RegionCode Mortality::getRegion() const
{
    return mortalityInputs_.getRegion();
}

std::string Mortality::getSpeciesCode() const
{
    return mortalityInputs_.getSpeciesCode();
}

EquationType Mortality::getEquationType() const
{
    return mortalityInputs_.getEquationType();
}

FlameLengthOrScorchHeightSwitch Mortality::getFlameLengthOrScorchHeightSwitch() const
{
    return mortalityInputs_.getFlameLengthOrScorchHeightSwitch();
}

double Mortality::getFlameLengthOrScorchHeightValue(LengthUnits::LengthUnitsEnum flameLengthOrScorchHeightUnits) const
{
    return mortalityInputs_.getFlameLengthOrScorchHeightValue(flameLengthOrScorchHeightUnits);
}

double Mortality::getTreeDensityPerUnitArea(AreaUnits::AreaUnitsEnum areaUnits) const
{
    return mortalityInputs_.getTreeDensityPerUnitArea(areaUnits);
}

double Mortality::getDBH(LengthUnits::LengthUnitsEnum diameterUnits) const
{
    return mortalityInputs_.getDBH(diameterUnits);
}

double Mortality::getTreeHeight(LengthUnits::LengthUnitsEnum treeHeightUnits) const
{
    return mortalityInputs_.getTreeHeight(treeHeightUnits);
}

double Mortality::getCrownRatio() const
{
    return mortalityInputs_.getCrownRatio();
}

double Mortality::getCrownDamage() const
{
    return mortalityInputs_.getCrownDamage();
}

CrownDamageEquationCode Mortality::getCrownDamageEquationCode() const
{
    return mortalityInputs_.getCrownDamageEquationCode();
}

CrownDamageType Mortality::getCrownDamageType() const
{
    return mortalityInputs_.getCrownDamageType();
}

double Mortality::getCambiumKillRating() const
{
    return mortalityInputs_.getCambiumKillRating();
}

BeetleDamage Mortality::getBeetleDamage() const
{
    return mortalityInputs_.getBeetleDamage();
}

double Mortality::getBoleCharHeight(LengthUnits::LengthUnitsEnum boleCharHeightUnits) const
{
    return mortalityInputs_.getBoleCharHeight(boleCharHeightUnits);
}

int Mortality::getCrownScorchOrBoleCharEquationNumber() const
{
    return mortalityInputs_.getCrownScorchOrBoleCharEquationNumber();
}

FireSeverity Mortality::getFireSeverity() const
{
    return mortalityInputs_.getFireSeverity();
}

double Mortality::getBarkThickness(LengthUnits::LengthUnitsEnum barkThicknessUnits) const
{
    return mortalityInputs_.getBarkThickness(barkThicknessUnits);
}

/*******************************************************************************************************
* Name: calculateMortality
* Desc: Calculate Mortality,
*       Looks at the Equation Type and calls the proper calculation function.
*******************************************************************************************************/
double Mortality::calculateMortality(FractionUnits::FractionUnitsEnum probablityUnits)
{
    initializeOutputs();

    if (mortalityInputs_.getCrownScorchOrBoleCharEquationNumber() == -1 && mortalityInputs_.getCrownDamageEquationCode() == CrownDamageEquationCode::not_set)
    {
        //sprintf(cr_ErrMes, "No Equation Code for %s", mortalityInputs_.speciesCode_);
        //strcpy(cr_ErrMes, "calculateMortality() - Logic Error - Invalid Equation Type");
        probabilityOfMortality_ = -1.0;
    }

    if (mortalityInputs_.getEquationType() == EquationType::crown_scorch)
    {      
        // Crown Scorch Equations
        probabilityOfMortality_ = calculateMortalityCrownScorch();
        if (probabilityOfMortality_ < 0)
        {
            probabilityOfMortality_ = -1.0;
        }
    }

    if (mortalityInputs_.getEquationType() == EquationType::crown_damage)
    {     
        // Crown Damage Equations
        probabilityOfMortality_ = PostFireInjuryCalculation();
        if(probabilityOfMortality_ < 0)
        {
            probabilityOfMortality_ = -1.0;
        }
    }

    if (mortalityInputs_.getEquationType() == EquationType::bole_char)
    {     
        // Bole Char Equations
        probabilityOfMortality_ = BoleCharCalculate();
        if(probabilityOfMortality_ < 0)
        {
            probabilityOfMortality_ = -1.0;
        }
    }

    return FractionUnits::fromBaseUnits(probabilityOfMortality_, probablityUnits);
}

string Mortality::getSpeciesCodeAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].speciesCode;
}

string Mortality::getScientificNameAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].scientificName;
}

string Mortality::getCommonNameAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].commonName;
}

int Mortality::getMortalityEquationNumberAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].mortalityEquationNumber;
}

int Mortality::getBarkEquationNumberAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].barkEquationNumber;
}

int Mortality::getCrownCoefficientCodeAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].crownCoefficientCode;
}

EquationType Mortality::getEquationTypeAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].equationType;
}

CrownDamageEquationCode Mortality::getCrownDamageEquationCodeAtSpeciesTableIndex(int index) const
{
    return speciesMasterTable_->record_[index].crownDamageEquationCode;
}

bool Mortality::checkIsInRegionAtSpeciesTableIndex(int index, RegionCode region) const
{
    bool isInRegion = false;

    if((index >= 0) && (index < speciesMasterTable_->record_.size()))
    {
        switch(region)
        {
            case RegionCode::interior_west:
            {
                if(speciesMasterTable_->record_[index].regionInteriorWest == (int)RegionCode::interior_west)
                {
                    isInRegion = true;
                }
                break;
            }

            case RegionCode::pacific_west:
            {
                if(speciesMasterTable_->record_[index].regionPacificWest == (int)RegionCode::pacific_west)
                {
                    isInRegion = true;
                }
                break;
            }
            case RegionCode::north_east:
            {
                if(speciesMasterTable_->record_[index].regionNorthEast == (int)RegionCode::north_east)
                {
                    isInRegion = true;
                }
                break;
            }
            case RegionCode::south_east:
            {
                if(speciesMasterTable_->record_[index].regionSouthEast == (int)RegionCode::south_east)
                {
                    isInRegion = true;
                }
                break;
            }
            default: // error case
            {
                isInRegion = false;
                break;
            }
        }
    }

    return isInRegion;
}

string Mortality::getScientificNameFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getScientificNameAtSpeciesTableIndex(index);
}

string Mortality::getCommonNameFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getCommonNameAtSpeciesTableIndex(index);
}

int Mortality::getMortalityEquationNumberFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getMortalityEquationNumberAtSpeciesTableIndex(index);
}

int Mortality::getBarkEquationNumberFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getBarkEquationNumberAtSpeciesTableIndex(index);
}

int Mortality::getCrownCoefficientCodeFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getCrownCoefficientCodeAtSpeciesTableIndex(index);
}

EquationType Mortality::getEquationTypeFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getEquationTypeAtSpeciesTableIndex(index);
}

CrownDamageEquationCode Mortality::getCrownDamageEquationCodeFromSpeciesCode(string speciesCode) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return getCrownDamageEquationCodeAtSpeciesTableIndex(index);
}

bool Mortality::checkIsInRegionFromSpeciesCode(string speciesCode, RegionCode region) const
{
    const int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
    return checkIsInRegionAtSpeciesTableIndex(index, region);
}

double Mortality::getFlameLength(LengthUnits::LengthUnitsEnum flameLengthUnits)
{
    double  blackHillsFlameLength, flameLengthOrScorchHeightValue, flameLength;

    flameLength = mortalityInputs_.getFlameLength(flameLengthUnits);
    if (flameLength != -1) {
        return LengthUnits::fromBaseUnits(flameLength, flameLengthUnits);
    } else {
        // Assume we're given Scorch Height
        flameLengthOrScorchHeightValue = mortalityInputs_.getFlameLengthOrScorchHeightValue(LengthUnits::Feet);
        blackHillsFlameLength = Calc_Flame(flameLengthOrScorchHeightValue); // For Black Hills PiPo

        // But if it's Flame Length - convert
        if(mortalityInputs_.getFlameLengthOrScorchHeightSwitch() == FlameLengthOrScorchHeightSwitch::flame_length) // if flame length
            {
                // convert to scorch height
                flameLengthOrScorchHeightValue = Calc_Scorch(mortalityInputs_.getFlameLengthOrScorchHeightValue(LengthUnits::Feet));
                // save flame length for Black Hills PiPo
                blackHillsFlameLength = flameLengthOrScorchHeightValue;
            }
        return  LengthUnits::fromBaseUnits(blackHillsFlameLength, flameLengthUnits);
    }
}

double Mortality::getScorchHeight(LengthUnits::LengthUnitsEnum scorchHeightUnits)
{
    double scorchHeight;

    scorchHeight = mortalityInputs_.getScorchHeight(LengthUnits::Feet);

    if (scorchHeight != -1 ) {
        return LengthUnits::fromBaseUnits(scorchHeight, scorchHeightUnits);
    } else if (mortalityInputs_.getFlameLengthOrScorchHeightValue(LengthUnits::Feet) == -1) {
        scorchHeight = calculateScorchHeight(mortalityInputs_.getFirelineIntensity(FirelineIntensityUnits::BtusPerFootPerSecond),
                                             FirelineIntensityUnits::BtusPerFootPerSecond,
                                             mortalityInputs_.getMidFlameWindSpeed(SpeedUnits::FeetPerMinute),
                                             SpeedUnits::FeetPerMinute,
                                             mortalityInputs_.getAirTemperature(TemperatureUnits::Fahrenheit),
                                             TemperatureUnits::Fahrenheit,
                                             LengthUnits::Feet);
    } else if (mortalityInputs_.getFlameLengthOrScorchHeightSwitch() == FlameLengthOrScorchHeightSwitch::flame_length) {
        scorchHeight = Calc_Scorch(mortalityInputs_.getFlameLengthOrScorchHeightValue(LengthUnits::Feet));
    } else if (mortalityInputs_.getFlameLengthOrScorchHeightSwitch() == FlameLengthOrScorchHeightSwitch::scorch_height) {
        scorchHeight = mortalityInputs_.getFlameLengthOrScorchHeightValue(LengthUnits::Feet);
    } else {
        scorchHeight = -1.0;
    }

    return LengthUnits::fromBaseUnits(scorchHeight, scorchHeightUnits);
}

/*******************************************************************************************************
* Name: calculateMortalityCrownScorch
* Desc: Calculate the Probility of Mortality, Basal Area, Trees Killed
*        Canopy Cover. See the MortalityOutputs structure defines in fomt.h
*        This function is called with a Species, Density etc, see below.
*        Individual calcs are made for the species, the values are also
*         accumulated so that the caller can have accumlated totals and
*         averages, for Prob Mort, Tree Totals, Basal Area, Canopy Cover.
*
*        For example the FOFEM Bach programs uses this function. The Batch
*         program processes multiple Stands, when beginning a Stand it
*         initializes the MO struct to zero it out, then it begins calling
*         this function for every species in a particular Stand, values
*         continue to get accumulated and get averaged. When the Batch
*         program has a Stand change the Stand values are ready in the
*         MO struct. The MO can be initialize and the next Stand can be
*         started.
*
*        Running totals and averages are kept in the MO, thru the use of
*         values in MO and some global variables the values are kept
*         current so the caller doesn't have to do anything, just use
*         the totals and averages when they are ready.
*
* Note-1: We used to calculate these if they were missing but now we make
*         the user enter them, so when I cleaned up the code I just left
*         these old lines here, so I don't have to change the rest of code.
* Note-2: Equation 5 can get done in a varity of ways but that are all
*          considerated to be Eq 5 and report back to caller in MortalityOutputs as a 5
*          1. coastal plain with relative humidity
*          2. coastal plain no relative humidity
*          3. non-coastal plain uses previous versions equation 1, even
*              though we report back in MortalityOutputs we used eq 5
*
* Note-3: change 7-19-12 I add the compare for "L" because the batch file uses
*         "L" or "" - I could catch it when I read in the file and covert to
"         "Low" but I decided to just check for "L" here
*   In: cr_NameCode......Species                 - required
*       dbh.......Diameter Breast Height  - required
*       flameLengthOrScorchHeight........Flame Length or Scorch Height  - required
*       flameOrScorchSignifier.......ce_flame, ce_scorch see defines in fomt.h - required
*       treeHeight.......Height                  - required
*       CrnRat....Crown Ratio             - required
*       fireSeverity..."Low" or "" Fire Severity, - required, you must set
*                     if to "Low" or ""
* NOTE: Check for an Error by checking the cr_ErrMes, it will be "" if no
*        errors occured
*  Ret: percent of mortality
*       -1.0 if Error - see cr_ErrMes
*******************************************************************************************************/
double  Mortality::calculateMortalityCrownScorch()
{
    double f, barkThicknessPrime, DBHcm, treeHeight, LCR, HCR, CSL, P, Fl, CH;
    double  blackHillsFlameLength, CBH;
    double DBH, scorchHeight;
    int  mortalityEquationNumber;

    DBH = mortalityInputs_.getDBH(LengthUnits::Inches);
    blackHillsFlameLength = Fl = getFlameLength(LengthUnits::Feet);
    scorchHeight = getScorchHeight(LengthUnits::Feet);

    if(speciesMasterTable_->getSpeciesTableIndexFromSpeciesCodeAndEquationType(mortalityInputs_.getSpeciesCode(), mortalityInputs_.getEquationType()) == -1) // Check for Valid Species        
    {
        //sprintf(cr_ErrMes, "Invalid Species: %s", mortalityInputs_.speciesCode_);
        return -1.0;
    }

    // Calculate Bark Thickness
    mortalityInputs_.setBarkThickness(calculateBarkThickness(), LengthUnits::Inches);

    treeHeight = mortalityInputs_.getTreeHeight(LengthUnits::Feet); // Note-1                        
    HCR = treeHeight * (mortalityInputs_.getCrownRatio());
    treeCrownLengthScorched_ = scorchHeight - (treeHeight - HCR);

    if(treeCrownLengthScorched_ <= 0.0)
    {
        treeCrownLengthScorched_ = 0.0;
    }
    if(treeCrownLengthScorched_ > HCR)
    {
        treeCrownLengthScorched_ = HCR;
    }

    // Calc crown killed                                                         
    //  f_CK = % Crown Volume Scorched, f_CSL = % Crown Height/Length Scorched   
    //  % = 0 -> 100, scorched basically means killed                            
    if(HCR > 0.0)
    {
        treeCrownVolumeScorched_ = treeCrownLengthScorched_ * (2.0 * HCR - treeCrownLengthScorched_) / (HCR * HCR);
        CSL = 100.0 * (treeCrownLengthScorched_ / HCR);
    }
    else
    {
        treeCrownVolumeScorched_ = 0.0;
        CSL = 0.0;
        //strcpy(cr_ErrMes, "Mortality Calculaton is attempting to Divide by 0");
        return -1.0;
    }

    //...........................................................................

    mortalityEquationNumber = mortalityInputs_.getCrownScorchOrBoleCharEquationNumber(); // Get equation number               

    double crownVolumeScorchedPercent = treeCrownVolumeScorched_ * 100.0;
    switch(mortalityEquationNumber)
    {
        case 1:
        {
            if(DBH >= 1.0)
            {
                P = 1.0 / (1.0 + exp(-1.941 + (6.316 * (1.0 - exp(-mortalityInputs_.getBarkThickness(LengthUnits::Inches)))) - 0.000535 * (crownVolumeScorchedPercent * crownVolumeScorchedPercent)));
            }
            else if(CSL > 50.0)
            {
                P = 1.0;
            }
            else if(treeHeight < 3.0)
            {
                P = 1.0;
                // Fl = Calc_Flame(flameLengthOrScorchHeightValue);
            }
            else
            {
                mortalityInputs_.setBarkThickness(calculateBarkThickness(), LengthUnits::Inches);
                P = 1.0 / (1.0 + exp(-1.941 + (6.316 * (1.0 - exp(-mortalityInputs_.getBarkThickness(LengthUnits::Inches)))) - 0.000535 * (crownVolumeScorchedPercent * crownVolumeScorchedPercent)));
                P = P + (1.0 - P) * (1.0 - ((treeHeight - 3.0) / (((1.0 / DBH) * treeHeight) - 3.0)));
            }
            break;
        }
        case 3:
        {
            if(DBH > 1.0)
            {
                P = 1.0 / (1.0 + exp(-1.941 + (6.316 * (1.0 - exp(-mortalityInputs_.getBarkThickness(LengthUnits::Inches)))) - 0.000535 * (crownVolumeScorchedPercent * crownVolumeScorchedPercent)));
            }
            else if(CSL > 50.0)
            {
                P = 1.0;
            }
            else if(treeHeight < 3.0)
            {
                P = 1.0;
            }
            else
            {
                mortalityInputs_.setBarkThickness(calculateBarkThickness(), LengthUnits::Inches);
                P = 1.0 / (1.0 + exp(-1.941 + (6.316 * (1.0 - exp(-mortalityInputs_.getBarkThickness(LengthUnits::Inches)))) - 0.000535 * (crownVolumeScorchedPercent * crownVolumeScorchedPercent)));
                P = P + (1.0 - P) * (1.0 - ((treeHeight - 3.0) / (((1.0 / DBH) * treeHeight) - 3.0)));
            }
            if(P < 0.8)
            {
                P = 0.8;
            }
            break;
        }
        case 4:
        {
            // Fl = Calc_Flame(flameLengthOrScorchHeightValue);
            CH = Fl / 1.8;
            if(mortalityInputs_.getFireSeverity() == FireSeverity::low)
            {
                // Fire Severity - See Note-3 Above     
                P = 1.0 / (1.0 + exp((0.251 * DBH * 2.54) - (0.07 * CH * 2.54 * 12.0) - 4.407));
            }
            else
            {
                P = 1.0 / (1.0 + exp((0.0858 * DBH * 2.54) - (0.118 * CH * 2.54 * 12.0) - 2.157));
            }
            break;
        }
        //...................................................
        //    case 5: 
        //...................................................
        // Change - 8-20-2012 
        // New formula from Duncan Lutes  
        // we were having trouble with this, the original paper was  
        // saying that proportion of crown scorch was 0->1 but we found 
        // that we had to use a value at 1->10   
        case 5:
        {
            if(CSL <= 0.0)
            {
                P = 0.0;
                break;
            }
            DBHcm = LengthUnits::fromBaseUnits(DBH, LengthUnits::Centimeters);
            barkThicknessPrime = 0.435 + (0.031 * DBHcm);
            f = crownVolumeScorchedPercent / 10.0; // see comments just above 
            barkThicknessPrime = 0.169 + (5.136 * barkThicknessPrime) + (14.492 * Squaredouble(barkThicknessPrime)) - (0.348 * Squaredouble(f));
            P = 1.0 / (1.0 + exp(barkThicknessPrime));
            //sprintf(cr, "%20.18f ", P);

            // I don't know if these conditions could 
            // happen but I'm checking anyway  
            if(P > 1.0)
            {
                P = 1.0;
            }
            if(P < 0.0)
            {
                P = 0.0;
            }
            break;
        }
        // test-pfi 
        // New equations from Sharon - april/may 2008                                
        case 10:
        {
            P = Whitefir(CSL); // ABICON                            
            break;
        }
        case 11:
        {
            P = SubalpineFir(crownVolumeScorchedPercent); // ABILAS                            
            break;
        }
        case 12:
        {
            P = IncenseCedar(CSL); // LIBDEC                            
            break;
        }
        case 14:
        {
            P = WesternLarch(crownVolumeScorchedPercent, DBH); // LAROCC                            
            break;
        }
        case 15:
        {
            P = EngelmannSpruce(crownVolumeScorchedPercent); // PICENG                            
            break;
        }
        case 16:
        {
            P = RedFir(CSL); // ABIMAG                          
            break;
        }
        case 17:
        {
            P = WhitebarkPine(crownVolumeScorchedPercent, DBH); // PINALB                      
            break;
        }
        case 18:
        {
            P = SugarPine(CSL);
            break;
        }
        case 19:
        {
            P = PonderosaJeffreyPine(crownVolumeScorchedPercent);
            break;
        }
        case 20:
        {
            P = DouglasFir(crownVolumeScorchedPercent);
            break;
        }
        // Change 9-6-2016 - new Black Hills PiPo, see Duncan Lutes's .docx document saved in fofem project folder 
        case 21:
        {
            f = mortalityInputs_.getCrownRatio(); // crown ratio 
            CBH = mortalityInputs_.getTreeHeight(LengthUnits::Feet) - (mortalityInputs_.getTreeHeight(LengthUnits::Feet) * f);
            P = Eq21_BlkHilPiPo(mortalityInputs_.getTreeHeight(LengthUnits::Feet), CBH, mortalityInputs_.getDBH(LengthUnits::Feet), scorchHeight, blackHillsFlameLength);
            break;
        }
        default:
        {
            //sprintf(cr_ErrMes, "Equation Not implemented,  Equ Num: %d\n", i_MortEqu);
            P = 0;
            return -1.0;
            break;
        }
    }  // switch end 

    probabilityOfMortality_ = P;

    calculateMortalityTotals(); // Accumulate and calculate averages etc.

    return P;
}

/**********************************************************************************************************
* Name: Eq21_BlkHilPiPo
* Desc: Black Hills Ponderosa Pine PiPo
*
* (1) Seedlings (Height <1.37 m (4.5 ft)):
* P(m) = 1/1 + EXP(-(2.714 + (4.08 * flame length) + (-3.63 * height)))
*
* (2) Saplings (Height >1.37 m (4.5 ft) and DBH <10.2 cm (4 in.)):
* P(m) = 1/1 + EXP(-(-0.7661 + (2.7981 * flame length) + (-1.2487 * height)
*
* (3) Other trees (DBH >10.2 (4 in.)):
* P(m) = 1/1 + EXP(-(1.104 + (DBH * -0.156) + (0.013* pct live crown length scorched) + (0.001 * DBH * pct live crown length scorched)))
*
* pct live crown length scorched =
*  ((max height of crown length scorched – canopy base height)/(tree height – canopy base height)) *100
*
*  In: treeHeight.... Tree height - feet
*      crownBaseHeight.... crown base height - feet
*      dbh.... dia breast height - inch
*      scorch. scorch height - feet
*      flalen  flame length - feet
* Out: the calculated mortality
* Ret: the mortality 0 -> 1.0 - this value is check to see if it
*      exceeds the 0.5 point, if so it is consided dead, in which
*      case 1.0 is returned
**********************************************************************************************************/
double  Mortality::Eq21_BlkHilPiPo(double treeHeight, double crownBaseHeight, double dbh, double scorch, double flameLength)
{
    double f, g, treeHeightInMeters, DBHInCentimeters, flameLengthInMeters, crownBaseHeightInMeters, CLS, scorchHeightInMeters;

    treeHeightInMeters = LengthUnits::fromBaseUnits(treeHeight, LengthUnits::Meters);
    flameLengthInMeters = LengthUnits::fromBaseUnits(flameLength, LengthUnits::Meters);
    DBHInCentimeters = LengthUnits::fromBaseUnits(dbh, LengthUnits::Centimeters);
    scorchHeightInMeters = LengthUnits::fromBaseUnits(scorch, LengthUnits::Meters);
    crownBaseHeightInMeters = LengthUnits::fromBaseUnits(crownBaseHeight, LengthUnits::Meters);

    // 1-27-2017  Ticket-106, make sure scorch hit crown, to CLS doesn't get a negative value 
    // Calc the percent of the crown lenght that gets scorched
    if (scorchHeightInMeters > crownBaseHeightInMeters)
    {
        // make sure scorch hits the crown
        CLS = (scorchHeightInMeters - crownBaseHeightInMeters) / (treeHeightInMeters - crownBaseHeightInMeters); // (length of crown scorched) / ( length of crown)
        CLS = CLS * 100.0;
    }
    else
    {
        CLS = 0;
    }

    if (treeHeightInMeters < 1.37)
    {
        // Seedling < 4.5 feet, 1.37 meter
        g = exp(-(2.714 + (4.08 * flameLengthInMeters) + (-3.63 * treeHeightInMeters)));
        f = 1.0 / (1.0 + g);
    }
    else if (treeHeightInMeters >= 1.37 && DBHInCentimeters < 10.2)
    {
        // Sapling >= 4.5 feet (1.37 meter) and < 4 inch (10.2cm) DBH
        g = exp(-(-0.7661 + (2.7981 * flameLengthInMeters) + (-1.2487 * treeHeightInMeters)));
        f = 1.0 / (1.0 + g);
    }

    // Other trees (DBH >10.2 (4 in.))
    else
    {
        g = exp(-(1.104 + (DBHInCentimeters * -0.156) + (0.013 * CLS) + (0.001 * DBHInCentimeters * CLS)));
        f = 1.0 / (1.0 + g);
    }

    // 2-6-17 Ticket-106, Duncan Lutes said take this out, not sure why we had it in here 
    // if ( f >= 0.5 )
    //   f = 1.0; 

    return f;
}

/*******************************************************************************************************
* Name: EngelmannSpruce       PICENG
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::EngelmannSpruce(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(0.0845 + (cs * 0.0445))));
    return f;
}

/*******************************************************************************************************
* Name: WesternLarch    LAROCC
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::WesternLarch(double cs, double dbh)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-1.6594 +
        (cs * 0.0327) -
        (dbh * 0.1241))));

    return f;
}

/*******************************************************************************************************
* Name: IncenseCedar     LIBDEC
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGTH scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::IncenseCedar(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-4.2466 +
        (pow((double)cs, (double)3.0) * 0.000007172))));
    return f;
}

/*******************************************************************************************************
* Name: SugarPine
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGTH scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::SugarPine(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-2.0588 +
        (pow((double)cs, (double)2.0) * 0.000814))));

    return f;
}

/*******************************************************************************************************
* Name: PonderosaJeffreyPine
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::PonderosaJeffreyPine(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-2.7103 +
        (pow((double)cs, (double)3.0) * 0.000004093))));

    return f;
}

/*******************************************************************************************************
* Name: DouglasFir
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::DouglasFir(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-2.0346 +
        (cs * 0.0906) -
        (pow((double)cs, (double)2.0) * 0.0022) +
        (pow((double)cs, (double)3.0) * 0.000019))));

    return f;
}

/*******************************************************************************************************
* Name: WhitebarkPine  PINALB
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*       dbh....diam breast height
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::WhitebarkPine(double cs, double dbh)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-0.3268 +
        (cs * 0.1387) -
        (pow((double)cs, (double)2.0) * 0.0033) +
        (pow((double)cs, (double)3.0) * 0.000025) -
        (dbh * 0.0676))));
    return f;
}

/*******************************************************************************************************
* Name: RedFir    ABIMAG
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, May 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGHT scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::RedFir(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-2.3085 +
        (pow((double)cs, (double)3.0) * 0.000004059))));
    return f;
}

/*******************************************************************************************************
* Name: SubalpineFir     ABILAS
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*       DBH...dbh inch
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::SubalpineFir(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-1.6950 +
        (cs * 0.2071) -
        (pow((double)cs, (double)2.0) * 0.0047) +
        (pow((double)cs, (double)3.0) * 0.000035))));
    return f;
}

/*******************************************************************************************************
* Name: Whitefir   ABICON
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGHT scorched percent
*  Ret: mortality probability 0 -> 1.0
*******************************************************************************************************/
double Mortality::Whitefir(double cs)
{
    double f;
    f = 1.0 / (1.0 + exp(-(-3.5083 +
        (cs * 0.0956) -
        (pow((double)cs, (double)2.0) * 0.00184) +
        (pow((double)cs, (double)3.0) * 0.000017))));
    return f;
}

/***********************************************************************************
// Name: CC_Overlap
// Desc: Given a square foot amount of Canopy Cover, this will return
//        what percentage that is of and acre,
// NOTE: This will calculate the amount of overlap.
//       For example. if you send in a large amount like 43000 sq feet,
//        which is almost equal to the number squar feet in an acre,
//        you will get back a number around 67, because of the overlap.
//       You would have to send in a number up over 200,000 before you
//        would get back 99 for a percent.
//       Also for example, a number like 400 square feet sent in would get
//         you a return value of about 1 percent
// NOTE: this function will never return a value greater than 100
//   In: canopy cover in square feet.
//  Ret: percent as a whole number  0 > 100
*******************************************************************************************************/
double Mortality::CC_Overlap(double SqFtCov)
{
    double x, p;
    constexpr double ce_SqFtAcre = 43560.0;

    x = (SqFtCov / ce_SqFtAcre);
    p = 100.0 * (1.0 - exp(-x));
    return p;
}

/***********************************************************************************
// Name: Basal_Area
// Desc: Get the Basal Area of a number of trees
//  In: dbh.....diam breast height
//Cnt.....number of trees to times by
// Ret: total basal area - square inches
*******************************************************************************************************/
double Mortality::Basal_Area(double DBH, double Cnt)
{
    double f, r;
    r = DBH / 2; // radius 
    f = 3.14159 * (r * r);// area = pi r sqrd  
    f = f * Cnt; // Times how many trees
    return f;
}

/***********************************************************************************
// Name: Calc_Scorch
// Desc: Calculate the scorch height from the flame length
//Orig FORTRAN statement 'FLAME_TO_SCORCH = ((flame / 0.45)**2.174)**0.667'
//  In: flame....flame length
// Ret: scorch height
*******************************************************************************************************/
double Mortality::Calc_Scorch(double flame)
{
    double f, g, h;
    f = flame / 0.45;
    g = pow((double)f, (double)2.174);
    h = pow((double)g, (double)0.667);
    return h;
}

/***********************************************************************************
// Name: Calc_Flame
// Desc: Calculate the flame length from the scorch height
// Orig FORTRAN statement 'SCORCH_TO_FLAME = 0.45 * (flame**1.5)**0.46'
//  In: scorch height
// Ret: flame length
*******************************************************************************************************/
double Mortality::Calc_Flame(double scorchHeight)
{
    double f, g;
    g = pow((double)scorchHeight, (double)1.5);
    f = 0.45 * pow((double)g, (double)0.46);
    return f;
}

/*******************************************************************************************************
* Name: calculateMortalityTotals
* Desc: Do individual species and stand level calcs and totals.
*       This function calculations the stand level values everytime it
*        is called and plugs them into the MO structure so that they
*        are always available to the caller.
*       PLEASE see notes in calling function.
* Note-1: change 11-05--5
*        I caught this div by 0 bug when I was test batch input files,
*        the only time accumulate total killed 'TotKilled' will
*        be 0 is when nothing has been killed on the stand yet in the
*        course of processing each tree record, this div by 0 inparticular
*        will happen when the first tree record for the stand has nothing
*        killed (no mortality)

*   In: mortalityInputs_....Mortality Inputs
*       Prob..Probability of Mortality for the species
*  Out: a_MO....Mortalitity Output struct
*******************************************************************************************************/
void  Mortality::calculateMortalityTotals()
{
    double f;
    killedTrees_ = probabilityOfMortality_ * mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres); // get # trees killed
    killedTrees_ += 0.5;
    killedTrees_ = (int)killedTrees_;
    if (killedTrees_ > mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres))
    {
        killedTrees_ = mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres);
    }

    // Individual Basal Area................
    basalAreaPrefire_ = Basal_Area(mortalityInputs_.getDBH(LengthUnits::Inches), mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres)); // Total Basal Area
    basalAreaKillled_ = Basal_Area(mortalityInputs_.getDBH(LengthUnits::Inches), killedTrees_); // Killed basal area
    basalAreaPostfire_ = basalAreaPrefire_ - basalAreaKillled_;

    basalAreaPrefire_ = basalAreaPrefire_ / 144.0; // Get Squaredouble inches to Squaredouble Feet
    basalAreaKillled_ = basalAreaKillled_ / 144.0;
    basalAreaPostfire_ = basalAreaPostfire_ / 144.0;
    
    // Basal Area Totals ...................
    totalBasalAreaPrefire_ += basalAreaPrefire_; // Total Basal Areas
    totalBasalAreaKillled_ += basalAreaKillled_;
    totalBasalAreaPostfire_ += basalAreaPostfire_;

    // Canopy Cover .............................................................
    f = calculateCrownCover();// Crown Cover Area for 1 tree

    prefireCanopyCover_ = f * mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres); // Crown Cov area * number trees
    postfireCanopyCover_ = prefireCanopyCover_ - (f * killedTrees_);

    gloabalTotalCoveragePrefireLive_ += prefireCanopyCover_; // Need to accumulate these before doing  overlap 
    globalTotalCoverPostfireLive_ += postfireCanopyCover_;

    prefireCanopyCover_ = CC_Overlap(prefireCanopyCover_);
    postfireCanopyCover_ = CC_Overlap(postfireCanopyCover_);

    // Canopy Cover Totals..........................
    totalPrefireCanopyCover_ = CC_Overlap(gloabalTotalCoveragePrefireLive_);
    totalPostfireCanopyCover_ = CC_Overlap(globalTotalCoverPostfireLive_);
    totalPrefireCanopyCover_ = (int)(totalPrefireCanopyCover_ + 0.5);
    totalPostfireCanopyCover_ = (int)(totalPostfireCanopyCover_ + 0.5);

    /* Note: I do this here but not on the actual FOFEM6 Mortality Report, so    */
    /*  there can be a slight difference between them                            */
    /* The FOFEM6 report does some of its own calcs, totals, averages and        */
    /*  use the ones in the MO struct                                            */
    if (totalPrefireCanopyCover_ < 1.0) // Show at least 1 percent
    {
        totalPrefireCanopyCover_ = 1;
    }
    if (totalPostfireCanopyCover_ < 1.0)
    {
        totalPostfireCanopyCover_ = 1;
    }

    /*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
    /* Accumulate for Stand                                                      */
    totalPrefireTrees_ += mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres); // Total Prefire trees

    globalTotalProbabilityOfMortality_ += probabilityOfMortality_; // total probability of mortality
    globalDivisorForAvgTotalProbabilityOfMortality_++; // count for averaging
    averageMortality_ = (int)((globalTotalProbabilityOfMortality_ / globalDivisorForAvgTotalProbabilityOfMortality_) + 0.5);

    if (mortalityInputs_.getDBH(LengthUnits::Feet) >= 4.0) // Average Prob of Mort on trees over 4 inches
    {
        globalTotalMortMortalityGreaterThan4DBH_ += probabilityOfMortality_;
        globalDivisorForAvgTotalMortMortalityGreaterThan4DBH_++;
        averageMortalityGreaterThan4DBH_ = (int)((globalTotalMortMortalityGreaterThan4DBH_ / globalDivisorForAvgTotalMortMortalityGreaterThan4DBH_) + 0.5);
    }

    totalKilled_ += killedTrees_; // total up
    globalKillledTreesTimesDBH_ += mortalityInputs_.getDBH(LengthUnits::Inches) * killedTrees_;
    /* Change 11-5-05 - Fix to catch divide by 0 - See Note-1 above              */
    if (totalKilled_ != 0)
    {
        averageDBHKilled_ = globalKillledTreesTimesDBH_ / totalKilled_;
    }
}

/*******************************************************************************************************
* Name: initializeOutputs
* Desc: Zeros out all variables need to do Mortality running totals
*        and averages.
*       See Notes in heading of MT_Calc()
* NOTE: the global variable that get init too.
* In/Out: a_MO....Mortality Output struct
*******************************************************************************************************/
void  Mortality::initializeOutputs()
{
    totalPrefireTrees_ = 0;
    probabilityOfMortality_ = 0;
    killedTrees_ = 0;
    basalAreaPrefire_ = 0;
    basalAreaKillled_ = 0;
    basalAreaPostfire_ = 0;
    prefireCanopyCover_ = 0;
    postfireCanopyCover_ = 0;

    // Stand Level Totals, Averages
    averageMortality_ = 0;
    averageMortalityGreaterThan4DBH_ = 0;
    totalKilled_ = 0;
    averageDBHKilled_ = 0;
    totalBasalAreaPrefire_ = 0;
    totalBasalAreaKillled_ = 0;
    totalBasalAreaPostfire_ = 0;
    totalPrefireCanopyCover_ = 0;
    totalPostfireCanopyCover_ = 0;

    // Global Totals
    globalTotalProbabilityOfMortality_ = 0;
    globalDivisorForAvgTotalProbabilityOfMortality_ = 0;
    globalTotalMortMortalityGreaterThan4DBH_ = 0;
    globalDivisorForAvgTotalMortMortalityGreaterThan4DBH_ = 0;
    globalKillledTreesTimesDBH_ = 0;
    gloabalTotalCoveragePrefireLive_ = 0;
    globalTotalCoverPostfireLive_ = 0;
}

/*******************************************************************************************************
* Name: PostFireInjuryCalculation
* Desc: Perform Calc for a species
*
*   In: mortalityInputs...contains species, inputs, and Eq code to use
*  Out: mortalityInputs...mortality in Prop
*
*  Ret: 1 OK, 0 error see cr_ErrMes
*******************************************************************************************************/
double Mortality::PostFireInjuryCalculation()
{
    std::function<double(void)> func;

    //strcpy(cr_ErrMes, "");

    // Execute the appropriate functon
    switch(mortalityInputs_.getCrownDamageEquationCode())
    {
        case CrownDamageEquationCode::white_fir:
        {
            probabilityOfMortality_ = Eq_WhiteFir_WF();
            break;
        }
        case CrownDamageEquationCode::subalpine_fir:
        {
            probabilityOfMortality_ = Eq_SubalpineFir_SF();
            break;
        }
        case CrownDamageEquationCode::incense_cedar:
        {
            probabilityOfMortality_ = Eq_IncenseCedar_IC();
            break;
        }
        case CrownDamageEquationCode::western_larch:
        {
            probabilityOfMortality_ = Eq_WesternLarch_WL();
            break;
        }
        case CrownDamageEquationCode::whitebark_pine:
        {
            probabilityOfMortality_ = Eq_WhitebarkPine_WP();
            break;
        }
        case CrownDamageEquationCode::engelmann_spruce:
        {
            probabilityOfMortality_ = Eq_EngelmannSpruce_ES();
            break;
        }
        case CrownDamageEquationCode::sugar_pine:
        {
            probabilityOfMortality_ = Eq_SugarPine_SP();
            break;
        }
        case CrownDamageEquationCode::red_fir:
        {
            probabilityOfMortality_ = Eq_RedFir_RF();
            break;
        }
        case CrownDamageEquationCode::ponderosa_pine:
        {
            probabilityOfMortality_ = Eq_PonderosaPine_PP();
            break;
        }
        case CrownDamageEquationCode::ponderosa_kill:
        {
            probabilityOfMortality_ = Eq_PonderosaKill_PK();
            break;
        }
        case CrownDamageEquationCode::douglas_fir:
        {
            probabilityOfMortality_ = Eq_DouglasFir_DF();
            break;
        }
        default:
        {
            // an error has occurred
        }
    }

    // Will also get put into a_MO as a whole int value
    calculateMortalityTotals(); // Accum and Calc, averges etc.

    return probabilityOfMortality_;
}

/*******************************************************************************************************
* Name: isValidInput
* Desc: Valididate all inputs for species input data
*   In: mortalityInputs_....single species record/struct
*  Ret: true ok, else false
*******************************************************************************************************/
bool Mortality::isValidInput()
{
    /*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
    /* DBH and Beetle Kill might be required, so look in Required Field String */
    if(mortalityInputs_.isFieldRequiredVector_[(int)RequiredFieldNames::dbh] == true)
    {
        if (mortalityInputs_.getDBH(LengthUnits::Inches) < 0 || mortalityInputs_.getDBH(LengthUnits::Inches) > 500.0)
        {
            return false;
        }
    }

    if(mortalityInputs_.isFieldRequiredVector_[(int)RequiredFieldNames::beetle_damage] == true)
    {
        if (mortalityInputs_.getBeetleDamage() == BeetleDamage::not_set)
        {
            return false;
        }
    }

    if(mortalityInputs_.isFieldRequiredVector_[(int)RequiredFieldNames::crown_damage] == true)
    {
        if((mortalityInputs_.getCrownDamageType() < CrownDamageType::not_set) || (mortalityInputs_.getCrownDamage() < 0) || (mortalityInputs_.getCrownDamage() > 100.0))
        {
            return false;
        }
    }

    if(mortalityInputs_.isFieldRequiredVector_[(int)RequiredFieldNames::cambium_kill_rating] == true)
    {
        if(mortalityInputs_.getCambiumKillRating() < 0 || mortalityInputs_.getCambiumKillRating() > 4.0)
        {
            return false;
        }
    }

    // Tree density is always required
    if (mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres) < 1.0 || mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres) > 20000)
    {
        return false;
    }

    return true;
}

/*******************************************************************************************************
* Name: Eq_WhiteFir_WF            ABICON
* Desc:
*   In: mortalityInputs_->CrnDam....crown length scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*       mortalityInputs_->dbh....inches
*       mortalityInputs_->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: mortalityInputs_->Prop ...0 -> 1.0
*  Ret:
*******************************************************************************************************/
double Mortality::Eq_WhiteFir_WF()
{
    double f, cs, ckr, dbh, beetleCoefficient;

    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    dbh = mortalityInputs_.getDBH(LengthUnits::Inches);

    if (mortalityInputs_.getBeetleDamage() == BeetleDamage::yes)
    {
        beetleCoefficient = 1.0;
    }
    else
    {
        beetleCoefficient = -1.0;
    }

    f = 1.0 +
        exp(-(-3.5964 +
        (pow(cs, 3.0) * 0.00000628) +
            (ckr * 0.3019) +
            (dbh * 0.0483) +
            (beetleCoefficient * 0.5209)));
    f = 1.0 / f;
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_SubalpineFir_SF   ABILAS
* Desc:
*   In: mortalityInputs_->CrnDam....crown volume scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*  Out: mortalityInputs_->probability ...0 -> 1.0
*  Ret: 1 ok
*******************************************************************************************************/
double Mortality::Eq_SubalpineFir_SF()
{
    double f, cs, ckr;

    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    f = 1.0 / (1.0 + exp(-
        (-2.6036 +
        (pow(cs, 3.0) * 0.000004587) +
            (ckr * 1.3554))));
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_WesternLarch_WL    LAROCC
* Desc:
*   In: mortalityInputs_->CrnDam....crown volume scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*  Out: mortalityInputs_->probability ...0 -> 1.0
*******************************************************************************************************/
double Mortality::Eq_WesternLarch_WL()
{
    double f, cs, ckr;

    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    f = 1.0 / (1.0 + exp(-
        (-3.8458 +
        (pow(cs, 2.0) * 0.0004) +
            (ckr * 0.6266))));
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_IncenseCedar_IC    LIBDEC
* Desc:
*   In: mortalityInputs_->CrnDam....crown length scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*  Out: mortalityInputs_->probability ...0 -> 1.0
*  Ret: 1 ok
*******************************************************************************************************/
double Mortality::Eq_IncenseCedar_IC()
{
    double f, cs, ckr;

    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    f = 1.0 / (1.0 + exp(-
        (-5.6465 +
        (pow(cs, 3.0) * 0.000007274) +
            (ckr * 0.5428))));
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_WhitebarkPine_WP   PINALB
* Desc:
*   In: mortalityInputs_->CrnDam....crown volume scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*       mortalityInputs_->dbh
*  Out: mortalityInputs_->probability ...0 -> 1.0
*
*******************************************************************************************************/
double Mortality::Eq_WhitebarkPine_WP()
{
    double f, cs, dbh, ckr;
    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    dbh = mortalityInputs_.getDBH(LengthUnits::Inches);

    f = 1.0 / (1.0 + exp(-
        (-1.4059 +
        (pow(cs, 3.0) * 0.000004459) +
            ((ckr * ckr) * 0.2843) +
            (dbh * -0.1232))));
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_EngelmannSpruce_ES   PICENG
* Desc:
*   In: mortalityInputs_->CrnDam........crown volume scorched percent
*       mortalityInputs_->ckr........cambium kill ratio
*  Out: mortalityInputs_->probability ...0 -> 1.0
*******************************************************************************************************/
double Mortality::Eq_EngelmannSpruce_ES()
{
    double f, cs, ckr;
    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    f = 1.0 / (1.0 + exp(-
        (-2.9791 +
        (cs * 0.0405) +
        (ckr * 1.1596))));
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_RedFir_RF  ABIMAG
* Desc:
*   In: mortalityInputs_->CrnDam....crown length scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*  Out: mortalityInputs_->probability ...0 -> 1.0
*******************************************************************************************************/
double Mortality::Eq_RedFir_RF()
{
    double f, cs, ckr;
    cs = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    f = 1.0 / (1.0 + exp(-
        (-4.7515 +
        (pow(cs, 3.0) * 0.000005989) +
        (ckr * 1.0668))));
   
    return f;
}

/*******************************************************************************************************
* Name: Eq_SugarPine_SP   PINLAM
* Desc:
*   In: mortalityInputs_->CrnDam....crown length scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*       mortalityInputs_->dbh
*       mortalityInputs_->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: mortalityInputs_->probability ...0 -> 1.0
*
*******************************************************************************************************/
double Mortality::Eq_SugarPine_SP()
{
    double f, crownDamage, ckr, beetleCoefficient;
    crownDamage = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();

    if (mortalityInputs_.getBeetleDamage() == BeetleDamage::yes)
    {
        beetleCoefficient = 1.0;
    }
    else
    {
        beetleCoefficient = -1.0;
    }
    f = 1.0 / (1.0 + exp(-
        (-2.7598 +
        ((crownDamage * crownDamage) * 0.000642) +
            (pow(ckr, 3.0) * 0.0386) + (beetleCoefficient * 0.8485))));

    return f;
}

/*******************************************************************************************************
* Name: Eq_PonderosaPine_PP  PINPON
* Desc: There are 2 ponderosa equation, (see Eq_PonderosaKill_PK)
* NOTE: Currently Jeffrey Pine also uses this
*   In: mortalityInputs_->getCrownDamage()....crown volume scorched percent
*       mortalityInputs_->getCambiumKillRating()....cambium kill ratio 0-4
*       mortalityInputs_->getBeetleDamage() ...Beetle Kill  BeetleDamage::yes, BeetleDamage::no
*  Out: mortalityInputs_->probability ...0 -> 1.0
*
*******************************************************************************************************/
double Mortality::Eq_PonderosaPine_PP()
{
    double f, crownDamage, ckr, beetleCoefficient, F;

    crownDamage = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    if (mortalityInputs_.getBeetleDamage() == BeetleDamage::yes)
    {
        beetleCoefficient = 1.0;
    }
    else
    {
        beetleCoefficient = 0.0;
    }
    F = (-4.1914 + (pow(crownDamage, 2.0) * 0.000376)
        + (ckr * 0.5130)
        + (beetleCoefficient * 1.5873));
    f = 1.0 / (1.0 + exp(-(F)));

    return f;
}

/*******************************************************************************************************
* Name: Eq_PonderosaKill_PK  PINPONk
* Desc: There are 2 ponderosa equation, (see Eq_PonderosaPine_PP)
*       This equations uses Killed percent
* NOTE: Currently Jeffrey Pine also uses this
*   In: mortalityInputs_->getCrownDamage()....percent of crown killed
*       mortalityInputs_->getCambiumKillRating()....cambium kill rating 0-4
*       mortalityInputs_->getBeetleDamage() ...Beetle Kill  BeetleDamage::yes, BeetleDamage::no
*  Out: mortalityInputs_->probability ...0 -> 1.0
*
*******************************************************************************************************/
double Mortality::Eq_PonderosaKill_PK()
{
    double f, crownDamage, ckr, beetleCoefficient, F;

    crownDamage = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    if(mortalityInputs_.getBeetleDamage() == BeetleDamage::yes)
    {
        beetleCoefficient = 1.0;
    }
    else
    {
        beetleCoefficient = 0.0;
    }
    F = (-3.5729 + (pow(crownDamage, 2.0) * 0.000567)
        + (ckr * 0.4573)
        + (beetleCoefficient * 1.6075));
    f = 1.0 / (1.0 + exp(-(F)));

    return f;
}

/*******************************************************************************************************
* Name: Eq_DouglasFir_DF  PSEMEN
* Desc:
*   In: mortalityInputs_->CrnDam....crown volume scorched percent
*       mortalityInputs_->ckr....cambium kill ratio 0-4
*       mortalityInputs_->dbh
*       mortalityInputs_->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: mortalityInputs_->Prop ...0 -> 1.0
*
*******************************************************************************************************/
double Mortality::Eq_DouglasFir_DF()
{
    double f, crownDamage, dbh, ckr, beetleCoefficient, F;

    crownDamage = mortalityInputs_.getCrownDamage();
    ckr = mortalityInputs_.getCambiumKillRating();
    dbh = mortalityInputs_.getDBH(LengthUnits::Inches);

    if (mortalityInputs_.getBeetleDamage() == BeetleDamage::yes)
    {
        beetleCoefficient = 1.0;
    }
    else
    {
        beetleCoefficient = 0.0;
    }
    F = (-1.8912 + (crownDamage * 0.07) - (pow(crownDamage, 2.0) * 0.0019) +
        (pow(crownDamage, 3.0) * 0.000018) + (ckr * 0.5840)
        - (dbh * 0.0788) - (beetleCoefficient * 0.7959) + (dbh * beetleCoefficient * 0.1251)
        );

    f = 1.0 / (1.0 + exp(-(F)));

    return f;
}

/*******************************************************************************************************
* Name: BoleCharCalculate
* Desc: Bole Char - mortality probability
*       Southern US broadleaf species (Tara L. Keyser, others)
*       International Journal of Wildland Fire
*       https://doi.org/10.1071/WF17058
*
*  This is taken directly from Duncan Lutes's spread sheet, F, G, etc are the
*  spread sheet colums letters
*  coefficients
   B1      spreadsheet column - formula coefficent
   B2
   B3

  E = DBH  cm
  F = Bole Char Hgt  meter
  =1/(1+EXP((-1*(B1+(B2*E)+(B3*F)))))   from Tara L. Keyser, others
*
*******************************************************************************************************/
double Mortality::BoleCharCalculate()
{
    int i;
    double f;
    double dbh_centimeters;
    double bole_char_height_meters;
    double B1, B2, B3;
    f = 0;

    for (i = 0; i < boleCharTable_.size(); i++)
    {
        if(boleCharTable_[i].equationNumber == mortalityInputs_.getCrownScorchOrBoleCharEquationNumber())
        {
            break; // found it
        }
        else if(boleCharTable_[i].equationNumber == -1)
        {
            return -1; // Error - didn't find equation number in table
        }
    }
    
    B1 = boleCharTable_[i].B1; // spreadsheet column - formula coefficent
    B2 = boleCharTable_[i].B2;
    B3 = boleCharTable_[i].B3;

    dbh_centimeters = LengthUnits::fromBaseUnits(mortalityInputs_.getDBH(LengthUnits::Feet), LengthUnits::Centimeters);
    bole_char_height_meters = LengthUnits::fromBaseUnits(mortalityInputs_.getBoleCharHeight(LengthUnits::Feet), LengthUnits::Meters);
    f = 1.0 / (1.0 + exp(-1.0*(B1 + (B2*dbh_centimeters) + (B3*bole_char_height_meters))));
     // will also get put into a_MO as whole int value

    // test..................................
    double a, b;
    a = -1.0*(B1 + (B2*dbh_centimeters) + (B3*bole_char_height_meters));
    b = exp(a);

    // test.................................
    calculateMortalityTotals(); // Accumulate and calculate averages, etc.
    return f;
}

/***********************************************************************************
* Name: MIS_Killed
* Desc: Number of trees killed for a one MortalityInputs record
*  
* Ret: trees killed
*******************************************************************************************************/
double Mortality:: MIS_Killed()
{
    double mortalityProbability, treesKilled;
   
    /* first get the mortality percent */
    mortalityProbability = calculateMortality(FractionUnits::Fraction);
    treesKilled = mortalityProbability * mortalityInputs_.getTreeDensityPerUnitArea(AreaUnits::Acres);  /* mortality times density */
  // 5-14-18, Duncan Lutes don't round off  
  //  killed = int (killed + 0.5);   /* round to whole number */
    return treesKilled;
}

/***********************************************************************************
// Name: Squaredouble
// Desc: Square a number
//  In: f...number to square
// Ret: number
*******************************************************************************************************/
double Mortality::Squaredouble(double f)
{
    return f * f;
}

int Mortality::getSpeciesTableIndexFromSpeciesCode(string speciesCode) const
{
    return speciesMasterTable_->getSpeciesTableIndexFromSpeciesCode(speciesCode);
}

int Mortality::getNumberOfRecordsInSpeciesTable() const
{
    return (int)speciesMasterTable_->record_.size();
}

int Mortality::getSpeciesTableIndexFromSpeciesCodeAndEquationType(string speciesNameCode, EquationType equationType) const
{
    return speciesMasterTable_->getSpeciesTableIndexFromSpeciesCodeAndEquationType(speciesNameCode, equationType);
}

vector<bool> Mortality::getRequiredFieldVector()
{
    return equationRequiredFieldTable_.getRequiredFieldVector(mortalityInputs_.getEquationType(), mortalityInputs_.getCrownDamageEquationCode());
}

SpeciesMasterTableRecord Mortality::getSpeciesRecordAtIndex(int index) const
{
    return speciesMasterTable_->record_[index];
}

SpeciesMasterTableRecord Mortality::getSpeciesRecordBySpeciesCodeAndEquationType(string speciesCode, EquationType equationType) const
{
    int index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCodeAndEquationType(speciesCode, equationType);
    return speciesMasterTable_->record_[index];
}

std::vector<SpeciesMasterTableRecord> Mortality::getSpeciesRecordVectorForRegion(RegionCode region) const
{
    std::vector<SpeciesMasterTableRecord> speciesInSelectedRegion;
    bool isErroneousInput = false;
    for(int i = 0; i < speciesMasterTable_->record_.size(); i++)
    {
        switch(region)
        {
            case RegionCode::interior_west:
            {
                if(speciesMasterTable_->record_[i].regionInteriorWest == (int)RegionCode::interior_west)
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            case RegionCode::pacific_west:
            {
                if(speciesMasterTable_->record_[i].regionPacificWest == (int)RegionCode::pacific_west)
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            case RegionCode::north_east:
            {
                if(speciesMasterTable_->record_[i].regionNorthEast == (int)RegionCode::north_east)
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            case RegionCode::south_east:
            {
                if(speciesMasterTable_->record_[i].regionSouthEast == (int)RegionCode::south_east)
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            default: // error case
            {
                isErroneousInput = true;
                break;
            }
        }

        if(isErroneousInput)
        {
            break; // get out of for loop
        }
    }

    return speciesInSelectedRegion;
}

std::vector<SpeciesMasterTableRecord> Mortality::getSpeciesRecordVectorForRegionAndEquationType(RegionCode region, EquationType equationType) const
{
    std::vector<SpeciesMasterTableRecord> speciesInSelectedRegion;
    bool isErroneousInput = false;
   
    for(int i = 0; i < speciesMasterTable_->record_.size(); i++)
    {
        switch(region)
        {
            case RegionCode::interior_west:
            {
                if((speciesMasterTable_->record_[i].regionInteriorWest == (int)RegionCode::interior_west) &&
                    (speciesMasterTable_->record_[i].equationType == equationType))
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            case RegionCode::pacific_west:
            {
                if((speciesMasterTable_->record_[i].regionPacificWest == (int)RegionCode::pacific_west) &&
                    (speciesMasterTable_->record_[i].equationType == equationType))
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            case RegionCode::north_east:
            {
                if((speciesMasterTable_->record_[i].regionNorthEast == (int)RegionCode::north_east) &&
                    (speciesMasterTable_->record_[i].equationType == equationType))
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            case RegionCode::south_east:
            {
                if((speciesMasterTable_->record_[i].regionSouthEast == (int)RegionCode::south_east) &&
                    (speciesMasterTable_->record_[i].equationType == equationType))
                {
                    speciesInSelectedRegion.push_back(speciesMasterTable_->record_[i]);
                }
                break;
            }
            default: // error case
            {
                isErroneousInput = true;
                break;
            }
        }

        if(isErroneousInput)
        {
            break; // get out of for loop
        }
    }

    return speciesInSelectedRegion;
}

double Mortality::getProbabilityOfMortality(FractionUnits::FractionUnitsEnum probabilityUnits) const
{
    return FractionUnits::fromBaseUnits(probabilityOfMortality_, probabilityUnits);
}

double Mortality::getTotalPrefireTrees() const
{
    return totalPrefireTrees_;
}

double Mortality::getKilledTrees() const
{
    return killedTrees_;
}

double Mortality::getTreeCrownLengthScorched(FractionUnits::FractionUnitsEnum fractionUnits) const
{
  return FractionUnits::fromBaseUnits(treeCrownLengthScorched_, fractionUnits);
}

double Mortality::getTreeCrownVolumeScorched(FractionUnits::FractionUnitsEnum fractionUnits) const
{
  return FractionUnits::fromBaseUnits(treeCrownVolumeScorched_, fractionUnits);
}

double Mortality::getBasalAreaPrefire() const
{
    return basalAreaPrefire_;
}

double Mortality::getBasalAreaKillled() const
{
    return basalAreaKillled_;
}

double Mortality::getBasalAreaPostfire() const
{
    return basalAreaPostfire_;
}

double Mortality::prefireCanopyCover() const
{
    return prefireCanopyCover_;
}

double Mortality::postfireCanopyCover() const
{
    return postfireCanopyCover_;
}

/*************************************************************
* Name: updateInputsForSpeciesCodeAndEquationType
* Desc: Initialize mortalityInputs_
* Out:  true is ok, false if error
**************************************************************/
bool Mortality::updateInputsForSpeciesCodeAndEquationType(std::string speciesCode, EquationType equationType)
{
    int speciesIndex = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCodeAndEquationType(speciesCode, equationType);
    
    if(speciesIndex >= 0)
    {
        mortalityInputs_.setEquationType(speciesMasterTable_->record_[speciesIndex].equationType);
        mortalityInputs_.setCrownScorchOrBoleCharEquationNumber(speciesMasterTable_->record_[speciesIndex].mortalityEquationNumber);
        mortalityInputs_.setCrownDamageEquationCode(speciesMasterTable_->record_[speciesIndex].crownDamageEquationCode);

        mortalityInputs_.isFieldRequiredVector_ = equationRequiredFieldTable_.getRequiredFieldVector(mortalityInputs_.getEquationType(), mortalityInputs_.getCrownDamageEquationCode());
        mortalityInputs_.setCrownDamageType(equationRequiredFieldTable_.getCrownDamageType(mortalityInputs_.getEquationType(), mortalityInputs_.getCrownDamageEquationCode()));
        return true; // OK
    }

    return false; // Error
}

/****************************************************************************
* Name: calculateBarkThickness
* Desc: Calculate the Bark Thickness
*   In: speciesCode....Species
*       dbh.....DBH
* Note-1: Equation(s) that do not use a bark thickness equation
*         Ex: PIPA2
*  Ret: bark thinkness,
*      -1 if no equation
****************************************************************************/
double Mortality::calculateBarkThickness()
{
    int index, barkEquation;
    double f, barkThickness;
    index = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCodeAndEquationType(mortalityInputs_.getSpeciesCode(), mortalityInputs_.getEquationType());
    if(index < 0)
    {
        //strcpy(errMes, "Logic Error - Can't Find Species in function calculateBarkThickness");
        return -1;
    }

    barkEquation = speciesMasterTable_->record_[index].barkEquationNumber;

    switch(barkEquation)
    {
        case 1:
        {
            f = 0.019;
            break;
        }
        case 2:
        {
            f = 0.022;
            break;
        }
        case 3:
        {
            f = 0.024;
            break;
        }
        case 4:
        {
            f = 0.025;
            break;
        }
        case 5:
        {
            f = 0.026;
            break;
        }
        case 6:
        {
            f = 0.027;
            break;
        }
        case 7:
        {
            f = 0.028;
            break;
        }
        case 8:
        {
            f = 0.029;
            break;
        }
        case 9:
        {
            f = 0.03;
            break;
        }
        case 10:
        {
            f = 0.031;
            break;
        }
        case 11:
        {
            f = 0.032;
            break;
        }
        case 12:
        {
            f = 0.033;
            break;
        }
        case 13:
        {
            f = 0.034;
            break;
        }
        case 14:
        {
            f = 0.035;
            break;
        }
        case 15:
        {
            f = 0.036;
            break;
        }
        case 16:
        {
            f = 0.037;
            break;
        }
        case 17:
        {
            f = 0.038;
            break;
        }
        case 18:
        {
            f = 0.039;
            break;
        }
        case 19:
        {
            f = 0.04;
            break;
        }
        case 20:
        {
            f = 0.041;
            break;
        }
        case 21:
        {
            f = 0.042;
            break;
        }
        case 22:
        {
            f = 0.043;
            break;
        }
        case 23:
        {
            f = 0.044;
            break;
        }
        case 24:
        {
            f = 0.045;
            break;
        }
        case 25:
        {
            f = 0.046;
            break;
        }
        case 26:
        {
            f = 0.047;
            break;
        }
        case 27:
        {
            f = 0.048;
            break;
        }
        case 28:
        {
            f = 0.049;
            break;
        }
        case 29:
        {
            f = 0.05;
            break;
        }
        case 30:
        {
            f = 0.052;
            break;
        }
        case 31:
        {
            f = 0.055;
            break;
        }
        case 32:
        {
            f = 0.057;
            break;
        }
        case 33:
        {
            f = 0.059;
            break;
        }
        case 34:
        {
            f = 0.06;
            break;
        }
        case 35:
        {
            f = 0.062;
            break;
        }
        case 36:
        {
            f = 0.063;
            break;
        }
        case 37:
        {
            f = 0.068;
            break;
        }
        case 38:
        {
            f = 0.072;
            break;
        }
        case 39:
        {
            f = 0.081;
            break;
        }
        case 100: // See Note-1 above
        {
            f = 0;
            break;
        }
        default:
        {
            return -1;
            break;
        }
    }

    barkThickness = f * mortalityInputs_.getDBH(LengthUnits::Inches);
    return barkThickness;
}

/****************************************************************************
* Name: calculateCrownCover
* Desc: Calculate the Crown Cover, based on the Crown Cover using the
*        coefficients in the CCT (Crown Cover Table).
*       Species in the fospp.dat file are assigned 2 char Crown Cover Codes
*        which are used to find the coefficients in the CCT Table
*
*   In: speciesCode... Species codes
*       Dia.....Diamenter
*       treeHeight.....Tree Height
*  Ret: Crown Cover - in square feet
****************************************************************************/
double Mortality::calculateCrownCover()
{
    int speciesTableIndex;
    double f, r, a;

    if(mortalityInputs_.getTreeHeight(LengthUnits::Feet) <= 0)
    {
        return 0;
    }
    if(mortalityInputs_.getDBH(LengthUnits::Inches) <= 0)
    {
        return 0;
    }

    speciesTableIndex = speciesMasterTable_->getSpeciesTableIndexFromSpeciesCodeAndEquationType(mortalityInputs_.getSpeciesCode(), mortalityInputs_.getEquationType()); // Get index into Species table

    const int canopyCoefficientTableIndex = speciesMasterTable_->record_[speciesTableIndex].crownCoefficientCode;

    /* Get Diameter of Crown using Coefficients                                  */
    if(mortalityInputs_.getTreeHeight(LengthUnits::Feet) <= 4.5) // Small trees
    {
        //f = s_CCT.coefficientR_ * Dia;
        f = canopyCoefficientTable_.record_[canopyCoefficientTableIndex].coefficientR_ * mortalityInputs_.getDBH(LengthUnits::Inches);
    }
    else // Large Trees
    {
        //f = pow((double)Dia, (double)s_CCT.coefficientB_); // raise it to power
        f = pow((double)mortalityInputs_.getDBH(LengthUnits::Inches), (double)canopyCoefficientTable_.record_[canopyCoefficientTableIndex].coefficientB_); // raise it to power
        //f = f * s_CCT.coefficientA_;  // and multiply
        f = f * canopyCoefficientTable_.record_[canopyCoefficientTableIndex].coefficientA_;  // and multiply
    }

    /* Use Diameter of Crown to get Area..........                               */
    r = f / 2.0; // Get diameter to radius
    r = r * r; // pi r sqrd gets area of crown 
    a = 3.14159 * r;
    return a;
}
