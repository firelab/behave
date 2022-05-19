#include "mortality_equation_table.h"

EquationRequiredFieldTable::EquationRequiredFieldTable()
{
    //dbh,
    //tree_height,
    //crown_ratio,
    //crown_damage,
    //cambium_kill_rating,
    //beetle_damage,
    //bole_char_height,
    //bark_thickness,
    //num_inputs

    EquationType equationTypeTemp = EquationType::not_set;
    vector<bool> isFieldRequiredTemp((int)RequiredFieldNames::num_inputs);
    CrownDamageType crownDamageTypeTemp = CrownDamageType::not_set;
    CrownDamageEquationCode crownDamageEquationCodeTemp = CrownDamageEquationCode::not_set;

    // CrownDamageEquationCode::crown_scorch,       dbh, tree_height, crown_ratio
    // Crown Scorch record entry
    equationTypeTemp = EquationType::crown_scorch;
    crownDamageTypeTemp = CrownDamageType::not_set;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::not_set;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::tree_height] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_ratio] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);

    // Bole Char record entry
    // CrownDamageEquationCode::bole_char,          dbh, bole_char_height
    equationTypeTemp = EquationType::bole_char;
    crownDamageTypeTemp = CrownDamageType::not_set;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::not_set;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::bole_char_height] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);

    // White Fir record entry
    // CrownDamageEquationCode::white_fir,          dbh length ckr beetle_damage
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_length;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::white_fir;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::beetle_damage] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);

    // Subalpine Fir record entry
    // CrownDamageEquationCode::subalpine_fir,      dbh volume ckr
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_volume;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::subalpine_fir;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);

    // Incense Cedar record entry
     // CrownDamageEquationCode::incense_cedar,    length ckr
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_length;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::incense_cedar;
    std::fill(isFieldRequiredTemp.begin(), isFieldRequiredTemp.end(), false);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Western Larch record entry
    // CrownDamageEquationCode::western_larch,    volume ckr
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_volume;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::western_larch;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Whitebark Pine record entry
    // CrownDamageEquationCode::whitebark_pine,   dbh volume ckr
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_volume;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::whitebark_pine;
    std::fill(isFieldRequiredTemp.begin(), isFieldRequiredTemp.end(), false);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Engelmann Spruce record entry
    // CrownDamageEquationCode::engelmann_spruce, volume ckr
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_volume;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::engelmann_spruce;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Sugar Pine record entry
    // CrownDamageEquationCode::sugar_pine,       dbh length ckr beetle_damage
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_length;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::sugar_pine;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::beetle_damage] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Red Fir record entry
    // CrownDamageEquationCode::red_fir,          length  ckr
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_length;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::red_fir;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Ponderosa Pine record entry
    // CrownDamageEquationCode::ponderosa_pine,   volume ckr beetle_damage
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_volume;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::ponderosa_pine;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::beetle_damage] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
    
    // Ponderosa Kill record entry
    // CrownDamageEquationCode::ponderosa_kill,   kil ckr beetle_damage
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_kill;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::ponderosa_kill;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::beetle_damage] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);

    // Douglas Fir record entry
    // CrownDamageEquationCode::douglas_fir,      dbh volume ckr bark_thickness
    equationTypeTemp = EquationType::crown_damage;
    crownDamageTypeTemp = CrownDamageType::crown_volume;
    crownDamageEquationCodeTemp = CrownDamageEquationCode::douglas_fir;
    resetIsFieldRequiredVector(isFieldRequiredTemp);
    isFieldRequiredTemp[(int)RequiredFieldNames::crown_damage] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::dbh] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::cambium_kill_rating] = true;
    isFieldRequiredTemp[(int)RequiredFieldNames::bark_thickness] = true;
    insertRecord(equationTypeTemp, crownDamageTypeTemp, crownDamageEquationCodeTemp, isFieldRequiredTemp);
}

EquationRequiredFieldTable::~EquationRequiredFieldTable()
{

}

vector<bool> EquationRequiredFieldTable::getRequiredFieldVector(EquationType equationType, CrownDamageEquationCode crownDamageEquationCode)
{
    vector<bool> isFieldRequiredTemp((int)RequiredFieldNames::num_inputs);
    resetIsFieldRequiredVector(isFieldRequiredTemp);

    for(int i = 0; i < record_.size(); i++)
    {
        if((record_[i].equationType_ == equationType) && (record_[i].crownDamageEquationCode_ == crownDamageEquationCode))
        {
            isFieldRequiredTemp = record_[i].isFieldRequiredVector_;
            break; // found it, leave loop
        }
    }

    return isFieldRequiredTemp;
}

CrownDamageType EquationRequiredFieldTable::getCrownDamageType(EquationType equationType, CrownDamageEquationCode crownDamageEquationCode) const
{
    CrownDamageType crownDamageTypeTemp = CrownDamageType::not_set;

    if(equationType != EquationType::crown_damage)
    {
        for(int i = 0; i < record_.size(); i++)
        {
            if((record_[i].equationType_ == equationType) && (record_[i].crownDamageEquationCode_ == crownDamageEquationCode))
            {
                crownDamageTypeTemp = record_[i].crownDamageType_;
                break; // found it, leave loop
            }
        }
    }

    return crownDamageTypeTemp;
}

void EquationRequiredFieldTable::insertRecord(EquationType equationType, CrownDamageType crownDamageType,
    CrownDamageEquationCode crownDamageEquationCode, vector<bool>& isFieldRequired)
{
    EquationRequiredFieldTableRecord recordTemp;
    recordTemp.equationType_ = equationType;
    recordTemp.crownDamageType_ = crownDamageType;
    recordTemp.crownDamageEquationCode_ = crownDamageEquationCode;
    recordTemp.isFieldRequiredVector_ = isFieldRequired;
    record_.push_back(recordTemp);
}

void EquationRequiredFieldTable::resetIsFieldRequiredVector(vector<bool>& isFieldRequired)
{
    std::fill(isFieldRequired.begin(), isFieldRequired.end(), false);
    isFieldRequired[(int)RequiredFieldNames::region] = true; // always required
    isFieldRequired[(int)RequiredFieldNames::equation_type] = true; // always required
    isFieldRequired[(int)RequiredFieldNames::flame_length_or_scorch_height_switch] = true; // always required
    isFieldRequired[(int)RequiredFieldNames::flame_length_or_scorch_height_value] = true; // always required
}

EquationRequiredFieldTableRecord::EquationRequiredFieldTableRecord()
{
    equationType_ = EquationType::not_set;
    crownDamageEquationCode_ = CrownDamageEquationCode::not_set;
    isFieldRequiredVector_.resize((int)RequiredFieldNames::num_inputs);
    std::fill(isFieldRequiredVector_.begin(), isFieldRequiredVector_.end(), false);
    isFieldRequiredVector_[(int)RequiredFieldNames::region] = true; // always required
    isFieldRequiredVector_[(int)RequiredFieldNames::equation_type] = true; // always required
    isFieldRequiredVector_[(int)RequiredFieldNames::flame_length_or_scorch_height_switch] = true; // always required
    isFieldRequiredVector_[(int)RequiredFieldNames::flame_length_or_scorch_height_value] = true; // always required
}

EquationRequiredFieldTableRecord::~EquationRequiredFieldTableRecord()
{

}

EquationRequiredFieldTableRecord::EquationRequiredFieldTableRecord(const EquationRequiredFieldTableRecord& rhs)
{
    memberwiseCopyAssignment(rhs);
}

EquationRequiredFieldTableRecord& EquationRequiredFieldTableRecord::operator=(const EquationRequiredFieldTableRecord& rhs)
{
    if(this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

void EquationRequiredFieldTableRecord::memberwiseCopyAssignment(const EquationRequiredFieldTableRecord& rhs)
{
    equationType_ = rhs.equationType_;
    crownDamageEquationCode_ = rhs.crownDamageEquationCode_;
    crownDamageType_ = rhs.crownDamageType_;
    isFieldRequiredVector_.clear();
    isFieldRequiredVector_ = rhs.isFieldRequiredVector_;
}
