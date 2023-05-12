/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
// Name: mortality_inputs.h
// Desc: New version of Mortality for CodeBlocks
//
//{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
//-------------------------------------------------

#ifndef MORTALITY_INPUTS_H
#define MORTALITY_INPUTS_H

#include "behaveUnits.h"
#include "mortality_equation_table.h"
#include "species_master_table.h"

#include <vector>

enum class BeetleDamage
{
    not_set = -1,
    no,
    yes,
};

enum class FireSeverity
{
    not_set = -1,
    empty,
    low
};

enum class FlameLengthOrScorchHeightSwitch
{
    flame_length,
    scorch_height,
};

//  Mortality Input Structure  
class MortalityInputs
{
public:
    MortalityInputs();
    MortalityInputs(const MortalityInputs& rhs);
    MortalityInputs& operator=(const MortalityInputs& rhs);
    ~MortalityInputs();

    void setRegion(RegionCode region);
    void setSpeciesCode(std::string speciesCode);
    void setEquationType(EquationType equationType);
    void setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch flameLengthOrScorchHeightSwitch);
    void setFlameLengthOrScorchHeightValue(double flameLengthOrScorchHeightValue, LengthUnits::LengthUnitsEnum flameLengthOrScorchHeightUnits);
    void setTreeDensityPerUnitArea(double numberOfTrees, AreaUnits::AreaUnitsEnum areaUnits);
    void setDBH(double dbh, LengthUnits::LengthUnitsEnum diameterUnits);
    void setTreeHeight(double treeHeight, LengthUnits::LengthUnitsEnum treeHeightUnits);
    void setCrownRatio(double crownRatio);
    void setCrownDamage(double crownDamage);
    void setCambiumKillRating(double cambiumKillRating);
    void setBeetleDamage(BeetleDamage beetleDamage);
    void setBoleCharHeight(double boleCharHeight, LengthUnits::LengthUnitsEnum boleCharHeightUnits);
    void setCrownScorchOrBoleCharEquationNumber(int crownScorchOrBoleCharEquationNumber);
    void setCrownDamageEquationCode(CrownDamageEquationCode crownDamageEquationCode);
    void setCrownDamageType(CrownDamageType crownDamageType);
    void setFireSeverity(FireSeverity fireSeverity);
    void setBarkThickness(double barkThickness, LengthUnits::LengthUnitsEnum barkThicknessUnits); // Not input by user, but calculated if needed for further calcuation

    RegionCode getRegion() const;
    std::string getSpeciesCode() const;
    EquationType getEquationType() const;
    FlameLengthOrScorchHeightSwitch getFlameLengthOrScorchHeightSwitch() const;
    double getFlameLengthOrScorchHeightValue(LengthUnits::LengthUnitsEnum flameLengthOrScorchHeightUnits) const;
    double getTreeDensityPerUnitArea(AreaUnits::AreaUnitsEnum areaUnits) const;
    double getDBH(LengthUnits::LengthUnitsEnum diameterUnits) const;
    double getTreeHeight(LengthUnits::LengthUnitsEnum treeHeightUnits) const;
    double getCrownRatio() const;
    double getCrownDamage() const;
    double getCambiumKillRating() const;
    BeetleDamage getBeetleDamage() const;
    double getBoleCharHeight(LengthUnits::LengthUnitsEnum boleCharHeightUnits) const;
    int getCrownScorchOrBoleCharEquationNumber() const;
    CrownDamageEquationCode getCrownDamageEquationCode() const;
    CrownDamageType getCrownDamageType() const;
    FireSeverity getFireSeverity() const;
    double getBarkThickness(LengthUnits::LengthUnitsEnum barkThicknessUnits) const;

    vector<bool> isFieldRequiredVector_; // stores a bool for each of the equation dependent required inputs

private:
    void memberwiseCopyAssignment(const MortalityInputs& rhs);

    // Inputs..................
    RegionCode region_;
    string speciesCode_; // 4 to 7 character species name code, together with equation type identitifies a unique record in the species table
    EquationType equationType_; // crown scorch, bole char, or crown damage equation, together with species name code identitifies a unique record in the species table
    FlameLengthOrScorchHeightSwitch flameLengthOrScorchHeightSwitch_; // determines whether flameLengthOrScorchHeightValue_ is a flame length or scorch height
    double flameLengthOrScorchHeightValue_; // the value for flame length or scorch height
    double densityPerAcre_; // number of trees per acre in the stand
    double dbh_;  // diameter at breast height
    double treeHeight_;
    double crownRatio_;
    double crownDamage_;
    double cambiumKillRating_;
    BeetleDamage beetleDamage_;
    double boleCharHeight_;
    // All crown scorch (previously refered to as pre-fire) mort
   // equations codes are numeric and all have the same input
   // requirements, so a single entry crown_scorch is entered in the table
   // likewise with bole_char for Bole Char equations which are numberd
   // 100 and up,
    int crownScorchOrBoleCharEquationNumber_; // Numeric identifier for crown scorch/bole char equation used
    // The Crown Damage (previously refered as post-fire) have alpha codes
    // mapped to an enum and have various input requirements, so they are listed in the table individually
    // See the Crown Damage equation functions for details on thier individual inputs
    CrownDamageEquationCode crownDamageEquationCode_; // Identifier for crown damage equation
    CrownDamageType crownDamageType_; // Whether crown equation uses crown length, crown volume or kill
    FireSeverity fireSeverity_; // Fire Severity: low, empty or not set

    double barkThickness_;             // Not input by user, but calculated if needed for further calcuation
};

#endif //MORTALITY_INPUTS_H
