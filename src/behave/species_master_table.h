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
class SpeciesMasterTableRecord
{
public:
    SpeciesMasterTableRecord();
    SpeciesMasterTableRecord(const SpeciesMasterTableRecord& rhs);
    SpeciesMasterTableRecord& operator=(const SpeciesMasterTableRecord& rhs);
    ~SpeciesMasterTableRecord();

    string speciesCode_;
    string scientificName_;
    string commonName_;
    int  mortalityEquationNumber_;
    int  barkEquationNumber_;
    int  crownCoefficientCode_ ; /* canopy cover equation #, (FVS Species Index No. )      */
    int8_t  regionInteriorWest_; // Region 1
    int8_t  regionPacificWest_; // Region 2
    int8_t  regionNorthEast_; // Region 3
    int8_t  regionSouthEast_;  // Region 4
    EquationType equationType_;
    CrownDamageEquationCode crownDamageEquationCode_;
private:
    void memberwiseCopyAssignment(const SpeciesMasterTableRecord& rhs);
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
