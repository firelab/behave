/********************************************************
*
*
*********************************************************/
#pragma once

#include <functional>
#include <vector>

using std::vector;

enum class CrownDamageType
{
    not_set = -1,
    crown_length,
    crown_volume,
    crown_kill
};

enum class RequiredFieldNames
{
    region, // always required
    flame_length_or_scorch_height_switch, // always required
    flame_length_or_scorch_height_value, // always required
    equation_type, // always required
    dbh,
    tree_height,
    crown_ratio,
    crown_damage,
    cambium_kill_rating,
    beetle_damage,
    bole_char_height,
    bark_thickness,
    fire_severity,  // only if using mortality equation number 4
    num_inputs
};

enum class CrownDamageEquationCode
{
    not_set = -1,
    /* Equation Codes */
    white_fir,          //"WF"
    subalpine_fir,      //"SF"
    incense_cedar,      //"IC"
    western_larch,      //"WL"
    whitebark_pine,     //"WP"
    engelmann_spruce,   //"ES"
    sugar_pine,         //"SP"
    red_fir,            //"RF"
    ponderosa_pine,     //"PP"
    ponderosa_kill,     //"PK"
    douglas_fir,        //"DF"
};

enum class EquationType
{
    not_set = -1,
    /* Equation Type Mortality */
    crown_scorch, //"CRNSCH"   /* original Pre Fire */ 
    bole_char,    //"BOLCHR"   /* Bole Char */ 
    crown_damage,  //"CRCABE"   /* original Post Fire */   
};

/*****************************************************************************************
******************************************************************************************/
/* Bole Char Equation Coefficient table */
struct BoleCharCoefficientTableRecord
{
    int equationNumber; // Equation Number
    double B1; // Coefficents
    double B2; // Coefficents
    double B3; // Coefficents
};

class EquationRequiredFieldTableRecord
{
public:
    EquationRequiredFieldTableRecord();
    EquationRequiredFieldTableRecord(const EquationRequiredFieldTableRecord& rhs);
    EquationRequiredFieldTableRecord& operator=(const EquationRequiredFieldTableRecord& rhs);
    ~EquationRequiredFieldTableRecord();

    EquationType equationType_;
    CrownDamageType crownDamageType_;
    CrownDamageEquationCode crownDamageEquationCode_;
    vector<bool> isFieldRequiredVector_; // stores a bool for each of the equation dependent required inputs
private:
    void memberwiseCopyAssignment(const EquationRequiredFieldTableRecord& rhs);
};

class EquationRequiredFieldTable
{
public:
    EquationRequiredFieldTable();
    ~EquationRequiredFieldTable();

    vector<bool> getRequiredFieldVector(EquationType equationType, CrownDamageEquationCode crownDamageEquationCode);
    CrownDamageType getCrownDamageType(EquationType equationType, CrownDamageEquationCode crownDamageEquationCode) const;

private:
    vector<EquationRequiredFieldTableRecord> record_;
    void insertRecord(EquationType equationType, CrownDamageType crownDamageType,
        CrownDamageEquationCode crownDamageEquationCode, vector<bool>& isFieldRequired);
    void resetIsFieldRequiredVector(vector<bool>& isFieldRequired);
};
