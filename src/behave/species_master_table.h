/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
 * Name: species_master_table.h
 * Desc:
 *}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

#ifndef SPECIES_MASTER_TABLE_H
#define SPECIES_MASTER_TABLE_H

#include "canopy_coefficient_table.h"
#include "mortality_equation_table.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

// Geographic Area Coordination Centers
enum class GACC {
    Alaska = 1,
    California = 2,
    EasternArea = 3,
    GreatBasin = 4,
    NorthernRockies = 5,
    Northwest = 6,
    RockeyMountain = 7,
    SouthernArea = 8,
    Southwest = 9,
    NotSet = -1,
};

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                          Species Master Table Record                      */
struct SpeciesMasterTableRecord
{
    string speciesCode = "";
    string scientificName = "";
    string commonName = "";
    int  mortalityEquationNumber = -1;
    int  barkEquationNumber = -1;
    int  crownCoefficientCode; /* canopy cover equation #, (FVS Species Index No. )      */
    int8_t Alaska = false;
    int8_t California = false;
    int8_t EasternArea = false;
    int8_t GreatBasin = false;
    int8_t NorthernRockies = false;
    int8_t Northwest = false;
    int8_t RockeyMountain = false;
    int8_t SouthernArea = false;
    int8_t Southwest = false;
    EquationType equationType = EquationType::not_set;
    CrownDamageEquationCode crownDamageEquationCode = CrownDamageEquationCode::not_set;
};

class SpeciesMasterTable
{
public:
    /*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
    SpeciesMasterTable();
    ~SpeciesMasterTable();

    void  initializeMasterTable();

    int getSpeciesTableIndexFromSpeciesCode(string speciesCode) const;
    int getSpeciesTableIndexFromSpeciesCodeAndEquationType(string speciesCode, EquationType equationType) const;

    vector<SpeciesMasterTableRecord> record_;

    CanopyCoefficientTable canopyCoefficientTable;

    void insertRecord (string speciesCode,
                       string scientificName,
                       string commonName,
                       int  mortalityEquation,
                       int  brkEqu,
                       int  crownCoefficientCode,
                       int8_t Alaska,
                       int8_t California,
                       int8_t EasternArea,
                       int8_t GreatBasin,
                       int8_t NorthernRockies,
                       int8_t Northwest,
                       int8_t RocketyMountain,
                       int8_t SouthernArea,
                       int8_t SouthWest,
                       EquationType equationType,
                       CrownDamageEquationCode crownDamageEquationCode);
};

#endif // SPECIES_MASTER_TABLE_H
