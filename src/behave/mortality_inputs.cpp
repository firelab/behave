/*****************************************************************
* Name: fof_mis.cpp
* Desc: Mortality Input Structure,
*
*****************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "mortality.h"
#include "mortality_inputs.h" 

MortalityInputs::MortalityInputs()
{
    speciesCode_ = "";
    equationType_ = EquationType::not_set;
    densityPerAcre_ = -1.0;
    dbh_ = -1.0;
    treeHeight_ = -1.0;
    crownRatio_ = -1.0;
    flameLengthOrScorchHeightSwitch_ = FlameLengthOrScorchHeightSwitch::flame_length; // determines whether the value for flameLengthOrScorchHeightValue_ is flame length or scorch height
    flameLengthOrScorchHeightValue_ = -1.0;
    fireSeverity_ = FireSeverity::not_set;
    crownDamage_ = -1.0;
    cambiumKillRating_ = -1.0;
    beetleDamage_ = BeetleDamage::not_set;
    boleCharHeight_ = -1.0;
   
    isFieldRequiredVector_.resize((int)RequiredFieldNames::num_inputs);
    std::fill(isFieldRequiredVector_.begin(), isFieldRequiredVector_.end(), false);
    isFieldRequiredVector_[(int)RequiredFieldNames::region] = true; // always required
    isFieldRequiredVector_[(int)RequiredFieldNames::equation_type] = true; // always required
    isFieldRequiredVector_[(int)RequiredFieldNames::flame_length_or_scorch_height_switch] = true; // always required
    isFieldRequiredVector_[(int)RequiredFieldNames::flame_length_or_scorch_height_value] = true; // always required
}

MortalityInputs::MortalityInputs(const MortalityInputs& rhs)
{
    memberwiseCopyAssignment(rhs);
}

MortalityInputs& MortalityInputs::operator=(const MortalityInputs& rhs)
{
    if(this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void MortalityInputs::memberwiseCopyAssignment(const MortalityInputs& rhs)
{
    speciesCode_ = rhs.speciesCode_;
    equationType_ = rhs.equationType_;
    densityPerAcre_ = rhs.densityPerAcre_;
    dbh_ = rhs.dbh_;
    treeHeight_ = rhs.treeHeight_;
    crownRatio_ = rhs.crownRatio_;
    flameLengthOrScorchHeightSwitch_ = rhs.flameLengthOrScorchHeightSwitch_;
    flameLengthOrScorchHeightValue_ = rhs.flameLengthOrScorchHeightValue_;
    fireSeverity_ = rhs.fireSeverity_;
    crownDamage_ = rhs.crownDamage_;
    cambiumKillRating_ = rhs.cambiumKillRating_;
    beetleDamage_ = rhs.beetleDamage_;
    boleCharHeight_ = rhs.boleCharHeight_;
    isFieldRequiredVector_ = rhs.isFieldRequiredVector_; 
}

MortalityInputs::~MortalityInputs()
{

}

void MortalityInputs::setRegion(RegionCode region)
{
    region_ = region;
}

void MortalityInputs::setSpeciesCode(std::string speciesCode)
{
    speciesCode_ = speciesCode;
}

void MortalityInputs::setEquationType(EquationType equationType)
{
    equationType_ = equationType;
}

void MortalityInputs::setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch flameLengthOrScorchHeightSwitch)
{
    flameLengthOrScorchHeightSwitch_ = flameLengthOrScorchHeightSwitch;
}

void MortalityInputs::setFlameLengthOrScorchHeightValue(double flameLengthOrScorchHeightValue, LengthUnits::LengthUnitsEnum flameLengthOrScorchHeightUnits)
{
    flameLengthOrScorchHeightValue_ = LengthUnits::toBaseUnits(flameLengthOrScorchHeightValue, flameLengthOrScorchHeightUnits);
}

void MortalityInputs::setTreeDensityPerUnitArea(double numberOfTrees, AreaUnits::AreaUnitsEnum areaUnits)
{
    densityPerAcre_ = AreaUnits::toBaseUnits(numberOfTrees, areaUnits);
}

void MortalityInputs::setDBH(double dbh, LengthUnits::LengthUnitsEnum diameterUnits)
{
    dbh_ = LengthUnits::toBaseUnits(dbh, diameterUnits);
}

void MortalityInputs::setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum treeHeightUnits)
{
    treeHeight_ = LengthUnits::toBaseUnits(treeHeight, treeHeightUnits);
}

void MortalityInputs::setCrownRatio(double crownRatio)
{
    crownRatio_ = crownRatio;
}

void MortalityInputs::setCrownDamage(double crownDamage)
{
    crownDamage_ = crownDamage;
}

void MortalityInputs::setCambiumKillRating(double cambiumKillRating)
{
    cambiumKillRating_ = cambiumKillRating;
}

void MortalityInputs::setBeetleDamage(BeetleDamage beetleDamage)
{
    beetleDamage_ = beetleDamage;
}

void MortalityInputs::setBoleCharHeight(double boleCharHeight, LengthUnits::LengthUnitsEnum boleCharHeightUnits)
{
    boleCharHeight_ = LengthUnits::toBaseUnits(boleCharHeight, boleCharHeightUnits);;
}

void MortalityInputs::setCrownScorchOrBoleCharEquationNumber(int crownScorchOrBoleCharEquationNumber)
{
    crownScorchOrBoleCharEquationNumber_ = crownScorchOrBoleCharEquationNumber;
}

void MortalityInputs::setCrownDamageEquationCode(CrownDamageEquationCode crownDamageEquationCode)
{
    crownDamageEquationCode_ = crownDamageEquationCode;
}

void MortalityInputs::setCrownDamageType(CrownDamageType crownDamageType)
{
    crownDamageType_ = crownDamageType;
}

void MortalityInputs::setFireSeverity(FireSeverity fireSeverity)
{
    fireSeverity_ = fireSeverity;
}

void MortalityInputs::setBarkThickness(double barkThickness, LengthUnits::LengthUnitsEnum barkThicknessUnits)
{
    barkThickness_ = LengthUnits::toBaseUnits(barkThickness, barkThicknessUnits);;
}

RegionCode MortalityInputs::getRegion() const
{
    return region_;
}

std::string MortalityInputs::getSpeciesCode() const
{
    return speciesCode_;
}

EquationType MortalityInputs::getEquationType() const
{
    return equationType_;
}

FlameLengthOrScorchHeightSwitch MortalityInputs::getFlameLengthOrScorchHeightSwitch() const
{
    return flameLengthOrScorchHeightSwitch_;
}

double MortalityInputs::getFlameLengthOrScorchHeightValue(LengthUnits::LengthUnitsEnum flameLengthOrScorchHeightUnits) const
{
    return LengthUnits::fromBaseUnits(flameLengthOrScorchHeightValue_, flameLengthOrScorchHeightUnits);
}

double MortalityInputs::getTreeDensityPerUnitArea(AreaUnits::AreaUnitsEnum areaUnits) const
{
    return AreaUnits::fromBaseUnits(densityPerAcre_, areaUnits);
}

double MortalityInputs::getDBH(LengthUnits::LengthUnitsEnum diameterUnits) const
{
    return LengthUnits::fromBaseUnits(dbh_, diameterUnits);
}

double MortalityInputs::getTreeHeight(LengthUnits::LengthUnitsEnum treeHeightUnits) const
{
    return LengthUnits::fromBaseUnits(treeHeight_, treeHeightUnits);
}

double MortalityInputs::getCrownRatio() const
{
    return crownRatio_;
}

double MortalityInputs::getCrownDamage() const
{
    return crownDamage_;
}

double MortalityInputs::getCambiumKillRating() const
{
    return cambiumKillRating_;
}

BeetleDamage MortalityInputs::getBeetleDamage() const
{
    return beetleDamage_;
}

double MortalityInputs::getBoleCharHeight(LengthUnits::LengthUnitsEnum boleCharHeightUnits) const
{
    return LengthUnits::fromBaseUnits(boleCharHeight_, boleCharHeightUnits);
}

int MortalityInputs::getCrownScorchOrBoleCharEquationNumber() const
{
    return crownScorchOrBoleCharEquationNumber_;
}

CrownDamageEquationCode MortalityInputs::getCrownDamageEquationCode() const
{
    return crownDamageEquationCode_;
}

CrownDamageType MortalityInputs::getCrownDamageType() const
{
    return crownDamageType_;
}

FireSeverity MortalityInputs::getFireSeverity() const
{
    return fireSeverity_;
}

double MortalityInputs::getBarkThickness(LengthUnits::LengthUnitsEnum barkThicknessUnits) const
{
    return LengthUnits::fromBaseUnits(barkThickness_, barkThicknessUnits);
}
