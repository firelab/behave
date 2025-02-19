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

enum class RegionCode
    {
        interior_west = 1,
        pacific_west = 2,
        north_east = 3,
        south_east = 4
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
    int8_t gaccAlaska = false;
    int8_t gaccCalifornia = false;
    int8_t gaccEasternArea = false;
    int8_t gaccGreatBasin = false;
    int8_t gaccNorthernRockies = false;
    int8_t gaccNorthwest = false;
    int8_t gaccRocketyMountain = false;
    int8_t gaccSouthernArea = false;
    int8_t gaccSouthWest = false;
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
                       int8_t gaccAlaska,
                       int8_t gaccCalifornia,
                       int8_t gaccEasternArea,
                       int8_t gaccGreatBasin,
                       int8_t gaccNorthernRockies,
                       int8_t gaccNorthwest,
                       int8_t gaccRocketyMountain,
                       int8_t gaccSouthernArea,
                       int8_t gaccSouthWest,
                       EquationType equationType,
                       CrownDamageEquationCode crownDamageEquationCode);
};

#endif // SPECIES_MASTER_TABLE_H
