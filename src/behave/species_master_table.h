/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: species_master_table.h
* Desc:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#pragma once

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
    int8_t  regionInteriorWest = false; // Region 1
    int8_t  regionPacificWest = false; // Region 2
    int8_t  regionNorthEast = false; // Region 3
    int8_t  regionSouthEast = false;  // Region 4
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

    void insertRecord(string speciesCode, string scientificName, string commonName,
        int  mortalityEquation, int  brkEqu, int  crownCoefficientCode,
        int8_t region1, int8_t  region2, int8_t  region3, int8_t region4, EquationType equationType,
        CrownDamageEquationCode crownDamageEquationCode);
};
