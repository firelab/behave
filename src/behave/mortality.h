//{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
// Name: mortality.h
// Desc: Main interface for Morality CodeBlock
//
//*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}

#ifndef MORTALITY_H
#define MORTALITY_H

#include <string>
#include <vector>

#include "canopy_coefficient_table.h"
#include "mortality_inputs.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
class Mortality
{
public:
    Mortality() = delete; // There is no default constructor
    explicit Mortality(SpeciesMasterTable& speciesMasterTable);
    Mortality(const Mortality& rhs);
    Mortality& operator=(const Mortality& rhs);
    ~Mortality();

    // Mortality Inputs setters (for indiviual species)
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
    void setFireSeverity(FireSeverity fireSeverity);

    bool updateInputsForSpeciesCodeAndEquationType(std::string speciesCode, EquationType equationType);

    // Mortality Inputs getters (for indiviual species)
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
    CrownDamageEquationCode getCrownDamageEquationCode() const;
    CrownDamageType getCrownDamageType() const;
    double getCambiumKillRating() const;
    BeetleDamage getBeetleDamage() const;
    double getBoleCharHeight(LengthUnits::LengthUnitsEnum boleCharHeightUnits) const;
    int getCrownScorchOrBoleCharEquationNumber() const;
    FireSeverity getFireSeverity() const;
    double getBarkThickness(LengthUnits::LengthUnitsEnum barkThicknessUnits) const;

    double calculateMortality(ProbabilityUnits::ProbabilityUnitsEnum probablityUnits);

    // Species Master Table Interface
    string getSpeciesCodeAtSpeciesTableIndex(int index) const;
    string getScientificNameAtSpeciesTableIndex(int index) const;
    string getCommonNameAtSpeciesTableIndex(int index) const;
    int  getMortalityEquationNumberAtSpeciesTableIndex(int index) const;
    int  getBarkEquationNumberAtSpeciesTableIndex(int index) const;
    int  getCrownCoefficientCodeAtSpeciesTableIndex(int index) const; /* canopy cover equation #, (FVS Species Index No. )      */
    EquationType getEquationTypeAtSpeciesTableIndex(int index) const;
    CrownDamageEquationCode getCrownDamageEquationCodeAtSpeciesTableIndex(int index) const;
    bool  checkIsInRegionAtSpeciesTableIndex(int index, RegionCode region) const;

    string getScientificNameFromSpeciesCode(string speciesCode) const;
    string getCommonNameFromSpeciesCode(string speciesCode) const;
    int  getMortalityEquationNumberFromSpeciesCode(string speciesCode) const;
    int  getBarkEquationNumberFromSpeciesCode(string speciesCode) const;
    int  getCrownCoefficientCodeFromSpeciesCode(string speciesCode) const; /* canopy cover equation #, (FVS Species Index No. )      */
    EquationType getEquationTypeFromSpeciesCode(string speciesCode) const;
    CrownDamageEquationCode getCrownDamageEquationCodeFromSpeciesCode(string speciesCode) const;
    bool  checkIsInRegionFromSpeciesCode(string speciesCode, RegionCode region) const;

    int getNumberOfRecordsInSpeciesTable() const;
    int getSpeciesTableIndexFromSpeciesCode(string speciesNameCode) const;
    int getSpeciesTableIndexFromSpeciesCodeAndEquationType(string speciesNameCode, EquationType equationType) const;
    vector<bool> getRequiredFieldVector();

    SpeciesMasterTableRecord getSpeciesRecordAtIndex(int index) const;
    SpeciesMasterTableRecord getSpeciesRecordBySpeciesCodeAndEquationType(string speciesCode, EquationType equationType) const;

    std::vector<SpeciesMasterTableRecord> getSpeciesRecordVectorForRegion(RegionCode region) const;
    std::vector<SpeciesMasterTableRecord> getSpeciesRecordVectorForRegionAndEquationType(RegionCode region, EquationType equationType) const;

    // Mortality outputs getters
    double getProbabilityOfMortality(ProbabilityUnits::ProbabilityUnitsEnum probabilityUnits) const;   // Individual Species Probility of Mortality
    double getTotalPrefireTrees() const;        // Total Prefire Trees               
    double getKilledTrees() const;

    double getBasalAreaPrefire() const;         // Prefire Basal Area                
    double getBasalAreaKillled() const;         // Basal Area of Killed trees        
    double getBasalAreaPostfire() const;        // Post fire Basal Area              

    double prefireCanopyCover() const;          // Prefire Canopy Cover              
    double postfireCanopyCover() const;         // Postfire Canopy Cover             

private:
    void memberwiseCopyAssignment(const Mortality& rhs);
    void initializeOutputs();

    double calculateMortalityCrownScorch();

    void calculateMortalityTotals();
    double Squaredouble(double f);
    double calculateBarkThickness();
    double calculateCrownCover();
    double Eq21_BlkHilPiPo(double treeHeight, double crownBaseHeight, double dbh, double scorch, double flalen);

    double PostFireInjuryCalculation();
    double BoleCharCalculate();

    double MIS_Killed();

    double CC_Overlap(double f_SqFtCov);

    double Basal_Area(double  f_DBH, double  f_Cnt);
    double Calc_Scorch(double f_flame);
    double Calc_Flame(double f_Scorch);

    double Eq_WhiteFir_WF();
    double Eq_SubalpineFir_SF();
    double Eq_IncenseCedar_IC();
    double Eq_WesternLarch_WL();
    double Eq_WhitebarkPine_WP();
    double Eq_EngelmannSpruce_ES();
    double Eq_SugarPine_SP();
    double Eq_RedFir_RF();
    double Eq_PonderosaPine_PP();
    double Eq_PonderosaKill_PK();
    double Eq_DouglasFir_DF();

    bool isValidInput();

    double Whitefir(double cs);
    double RedFir(double cs);
    double SubalpineFir(double cs);
    double IncenseCedar(double cs);
    double WesternLarch(double cs, double f_dbh);
    double EngelmannSpruce(double cs);
    double WhitebarkPine(double cs, double dbh);
    double SugarPine(double cs);
    double PonderosaJeffreyPine(double cs);
    double DouglasFir(double cs);

    MortalityInputs mortalityInputs_;
    //MortalityOutputs mortalityOutputs_;

    SpeciesMasterTable* speciesMasterTable_;
    EquationRequiredFieldTable equationRequiredFieldTable_;
    std::vector <BoleCharCoefficientTableRecord> boleCharTable_;
    CanopyCoefficientTable canopyCoefficientTable_;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Low and High Limits on Flame Length and Scorch Height                     
    static constexpr const double ce_flame_length_lower_limit = 1.0;
    static constexpr const double ce_flame_length_upper_limit = 20.0;

    static constexpr const double ce_scorch_lower_limit = 1.0;
    static constexpr const double ce_scorch_upper_limit = 245.0;

    // Outputs
    // These values are calculated for a single Species                          
    double probabilityOfMortality_;      // Individual Species Probility of Mortality
    double totalPrefireTrees_;           // Total Prefire Trees               
    double killedTrees_;

    double basalAreaPrefire_;            // Prefire Basal Area                
    double basalAreaKillled_;            // Basal Area of Killed trees        
    double basalAreaPostfire_;           // Post fire Basal Area              

    double prefireCanopyCover_;          // Prefire Canopy Cover              
    double postfireCanopyCover_;         // Postfire Canopy Cover             

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // These values are accumulated for the Stand                                
    double averageMortality_;                // Averged all Mortality             
    double averageMortalityGreaterThan4DBH_; // Averged all Mortality > 4 DBH     
    double totalKilled_;                     // Total Killed Trees                
    double averageDBHKilled_;                // Avg DBH of Killed Trees           

    double totalBasalAreaPrefire_;       // Basal Area, Prefire               
    double totalBasalAreaKillled_;       // Basal Area of Killed trees        
    double totalBasalAreaPostfire_;      // Basal Area Post fire,(Pre - Killed)

    double totalPrefireCanopyCover_;     // Prefire Canopy Cover              
    double totalPostfireCanopyCover_;    // Postfire Canopy Cover             

    // These are used in Mortality to keep running totals and averages
    double globalTotalProbabilityOfMortality_;
    double globalDivisorForAvgTotalProbabilityOfMortality_;
    double globalTotalMortMortalityGreaterThan4DBH_;
    double globalDivisorForAvgTotalMortMortalityGreaterThan4DBH_;
    double globalKillledTreesTimesDBH_;
    double gloabalTotalCoveragePrefireLive_;
    double globalTotalCoverPostfireLive_;
};

#endif //MORTALITY_H
