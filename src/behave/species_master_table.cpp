#include "species_master_table.h"

#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <string.h>

SpeciesMasterTable::SpeciesMasterTable()
{

}

SpeciesMasterTable::~SpeciesMasterTable()
{

}

/****************************************************************************
* Name: initializeMasterTable
* Desc: Initialize the empty table
****************************************************************************/
void SpeciesMasterTable::initializeMasterTable()
{
    /* ......................................................... Mort Brk Cnpy Cov............ */
    /* Species       Name                                        Eq   eq        eq   regions   */
    //FOFEM Mortality Species Equations
    //May 4 
    //Species = Species code
    //Scientific_Name
    //Common_Name
    //Mort = Mortality equation numeric identifiers
    //          Crown Scorch Equations(CRNSCH) : 1 -> 99
    //          Bole Char Equations(BOLCHR) : 100->110
    //Brk = Bark Equation
    //Crn = Crown Coefficient code used to calculate stand canopy cover
    //
    //Reg = Region
    //    IW - Interior West
    //    PW - Pacific West
    //    NE - North East
    //    SE - South East
    //CRCABE = Crown Damage / Cambium / Beetle Equations(CRCABE)
    // 
    //Species,Scientific_Name,Common_Name,Mort,Brk,Crn,IW,PW,NE,SE,Equation_Type,CRCABE
    insertRecord("ABAM", "Abies amabilis", "Pacific silver fir", 1, 26, 1, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABBA", "Abies balsamea", "Balsam fir", 1, 10, 2, 1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABBAB", "Abies balsamea", "Balsam fir", 1, 10, 2, 1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABBAP", "Abies balsamea", "Balsam fir", 1, 10, 2, 1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABLAL", "Abies balsamea", "Balsam fir", 1, 10, 2, 1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABCO", "Abies concolor", "White fir", 10, 27, 2, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABCO", "Abies concolor", "White fir", -1, 27, 2, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::white_fir);
    insertRecord("ABCOC", "Abies concolor", "White fir", 10, 27, 2, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABCOC", "Abies concolor", "White fir", -1, 27, 2, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::white_fir);
    insertRecord("ABLO", "Abies lowiana", "Sierra white fir", 10, 27, 2, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABLO", "Abies lowiana", "Sierra white fir", -1, 27, 2, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::white_fir);
    insertRecord("ABGR", "Abies grandis", "Grand fir", 11, 25, 3, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABGR", "Abies grandis", "Grand fir", -1, 25, 3, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABGRI2", "Abies grandis", "Grand fir", 11, 25, 3, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABGRI2", "Abies grandis", "Grand fir", -1, 25, 3, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABGRG", "Abies grandis", "Grand fir", 11, 25, 3, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABGRG", "Abies grandis", "Grand fir", -1, 25, 3, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABGRI", "Abies grandis", "Grand fir", 11, 25, 3, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABGRI", "Abies grandis", "Grand fir", -1, 25, 3, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABGRJ", "Abies grandis", "Grand fir", 11, 25, 3, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABGRJ", "Abies grandis", "Grand fir", -1, 25, 3, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABLA", "Abies lasiocarpa", "Subalpine fir", 11, 20, 4, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABLA", "Abies lasiocarpa", "Subalpine fir", -1, 20, 4, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABLAA", "Abies lasiocarpa", "Corkbark fir", 11, 20, 4, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABLAA", "Abies lasiocarpa", "Corkbark fir", -1, 20, 4, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABLAL", "Abies lasiocarpa", "Subalpine fir", 11, 20, 4, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABLAL", "Abies lasiocarpa", "Subalpine fir", -1, 20, 4, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::subalpine_fir);
    insertRecord("ABMA", "Abies magnifica", "Red Fir", 16, 18, 5, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABMA", "Abies magnifica", "Red Fir", -1, 18, 5, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::red_fir);
    insertRecord("ABMAM", "Abies magnifica", "California red fir", 16, 36, 5, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABMAM", "Abies magnifica", "California red fir", -1, 36, 5, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::red_fir);
    insertRecord("ABMAS", "Abies magnifica", "Shasta red fir", 16, 36, 5, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABMAS", "Abies magnifica", "Shasta red fir", -1, 36, 5, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::red_fir);
    insertRecord("ABMAC", "Abies magnifica", "Shasta red fir", 16, 36, 5, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABMAC", "Abies magnifica", "Shasta red fir", -1, 36, 5, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::red_fir);
    insertRecord("ABMAS2", "Abies magnifica", "Shasta red fir", 16, 36, 5, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABMAS2", "Abies magnifica", "Shasta red fir", -1, 36, 5, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::red_fir);
    insertRecord("ABNO", "Abies nobilis", "Noble Fir", 1, 24, 7, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABPR", "Abies procera", "Noble Fir", 1, 24, 7, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ABISPP", "Abies species", "Firs", 1, 30, 2, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACBA3", "Acer barbatum", "Southern sugar maple", 1, 8, 21, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACBAL", "Acer barbatum", "Southern sugar maple", 1, 8, 21, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACBAV", "Acer barbatum", "Southern sugar maple", 1, 8, 21, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACLE", "Acer leucoderme", "Chalk maple", 1, 8, 21, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACMA3", "Acer macrophyllum", "Bigleaf maple", 1, 3, 21, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNE2", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEA", "Acer negundo", "Arizona boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEC2", "Acer negundo", "California boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEC", "Acer negundo", "California boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNE12", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEI", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEN", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEV2", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNET", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEL", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNEV", "Acer negundo", "Boxelder", 1, 13, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNI5", "Acer nigrum", "Black maple", 1, 14, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACNIP", "Acer nigrum", "Black maple", 1, 14, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACPE", "Acer pensylvanicum", "Striped maple", 1, 24, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRU", "Acer rubrum", "Red maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRU", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACRUD", "Acer rubrum", "Drummond's maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRUD", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACRUD2", "Acer rubrum", "Drummond's maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRUD2", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACRUR", "Acer rubrum", "Red maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRUR", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACRUT2", "Acer rubrum", "Red maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRUT2", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACRUT", "Acer rubrum", "Red maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRUT", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACRUT3", "Acer rubrum", "Red maple", 1, 7, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACRUT3", "Acer rubrum", "Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("ACSA2", "Acer saccharinum", "Silver maple", 1, 10, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAL3", "Acer saccharinum", "Silver maple", 1, 10, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAW", "Acer saccharinum", "Silver maple", 1, 10, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSA3", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAS", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAG", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAR", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAS2", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAO", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSAS4", "Acer saccharum", "Sugar maple", 1, 12, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACSP2", "Acer spicatum", "Mountain maple", 1, 19, 21, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ACESPP", "Acer species", "Maples", 1, 8, 21, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGL", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLA", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLB", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLG", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLL", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLM", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLM2", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLP", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEGLS", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEFL", "Aesculus flava", "Yellow buckeye", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AEOC2", "Aesculus octandra", "Yellow buckeye", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AIAL", "Ailanthus altissima", "Ailanthus", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ALRH2", "Alnus rhombifolia", "White alder", 1, 1, 23, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ALRHB", "Alnus rhombifolia", "White alder", 1, 1, 23, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ALRU2", "Alnus rubra", "Red alder", 1, 1, 22, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ALOR", "Alnus rubra", "Red alder", 1, 1, 22, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AMAR3", "Amelanchier arborea", "Common serviceberry", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AMARA3", "Amelanchier arborea", "Alabama serviceberry", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AMARA4", "Amelanchier arborea", "Common serviceberry", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("AMARA5", "Amelanchier arborea", "Downy serviceberry", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ARME", "Arbutus menziesii", "Pacific madrone", 1, 34, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEAL2", "Betula alleghaniensis", "Yellow birch", 1, 10, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEALA", "Betula alleghaniensis", "Yellow birch", 1, 10, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEALF", "Betula alleghaniensis", "Yellow birch", 1, 10, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEALM", "Betula alleghaniensis", "Yellow birch", 1, 10, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BELE", "Betula lenta", "Sweet birch", 1, 9, 24, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BENI", "Betula nigra", "River Birch", 1, 8, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEOC2", "Betula occidentalis", "Water birch", 1, 1, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEOCI", "Betula occidentalis", "Water birch", 1, 1, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPA", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAC2", "Betula papyrifera", "Mountian paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAP", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAK", "Betula papyrifera", "Kenai birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAP", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAC", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAE", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAM2", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BEPAP2", "Betula papyrifera", "Paper birch", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("BETSPP", "Betula species", "Birches", 1, 1, 24, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CEOC", "Celtis occidentalis", "Common hackberry", 1, 14, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CEOCC", "Celtis occidentalis", "Common hackberry", 1, 14, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CEOCC3", "Celtis occidentalis", "Common hackberry", 1, 14, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CEOCC4", "Celtis occidentalis", "Common hackberry", 1, 14, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CEOCO2", "Celtis occidentalis", "Common hackberry", 1, 14, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CEOCP", "Celtis occidentalis", "Common hackberry", 1, 14, 24, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CANO9", "Callitropsis nootkatensis", "Alaska cedar", 1, 2, 9, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAAQ2", "Carya aquatica", "Water hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAAQA3", "Carya aquatica", "Water hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACA18", "Carpinus caroliniana", "American hornbeam", 1, 9, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACAC2", "Carpinus caroliniana", "American hornbeam", 1, 9, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACAV", "Carpinus caroliniana", "American hornbeam", 1, 9, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACAV2", "Carpinus caroliniana", "American hornbeam", 1, 9, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAAL27", "Carya alba", "Mockernut hickory", 1, 22, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACA38", "Carya carolinae - septentrionalis", "Shagbark hi", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACOL3", "Carya cordiformis", "Bitternut hickory", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACO15", "Carya cordiformis", "Bitternut hickory", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAGL8", "Carya glabra", "Pignut hickory", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAGLG2", "Carya glabra", "Pignut hickory", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAGLH", "Carya glabra", "Pignut hickory", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAGLM", "Carya glabra", "Pignut hickory", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAIL2", "Carya illinoinensis", "Pecan", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CALA21", "Carya laciniosa", "Shellbark hickory", 1, 22, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOV2", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOVC", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOVA", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOVF", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOVN", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOVP", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAOVA", "Carya ovata", "Shagbark hickory", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CARSPP", "Carya species", "Hickories", 1, 23, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CATE9", "Carya texana", "Black hickory", 1, 19, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CATEA", "Carya texana", "Black hickory", 1, 19, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CATEV", "Carya texana", "Black hickory", 1, 19, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CATO6", "Carya tomentosa", "Mockernut hickory", 1, 22, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CATOS", "Carya tomentosa", "Mockernut hickory", 1, 22, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACHM", "Castanopsis chrysophylla", "Giant chinkapin", 1, 24, 25, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CACH6", "Castanopsis chrysophylla", "Giant chinkapin", 1, 24, 25, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CADE12", "Castanea dentata", "American chestnut", 1, 19, 39, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CAAM29", "Castanea americana", "American chestnut", 1, 19, 39, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CATSPP", "Catalpa species", "Catalpas", 1, 16, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELA", "Celtis laevigata", "Sugarberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELAB", "Celtis laevigata", "Sugarberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELAL", "Celtis laevigata", "Sugarberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELAA", "Celtis laevigata", "Sugarberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELAB2", "Celtis laevigata", "Sugarberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELAR", "Celtis laevigata", "Netleaf hackberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CELAT8", "Celtis laevigata", "Texan sugarberry", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CECA4", "Cercis canadensis", "Eastern redbud", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CECAC", "Cercis canadensis", "Eastern redbud", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CECAM", "Cercis canadensis", "Mexican redbud", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CECAM2", "Cercis canadensis", "Mexican redbud", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CECAT", "Cercis canadensis", "Texas redbud", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CECAT2", "Cercis canadensis", "Texas redbud", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CHLA", "Chamaecyparis lawsoniana", "PortOrford - cedar", 1, 39, 9, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CHNO", "Chamaecyparis nootkatensis", "Alaska - cedar", 1, 2, 9, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CHTH2", "Chamaecyparis thyoides", "Atlantic white - cedar", 1, 4, 9, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CHTHH", "Chamaecyparis thyoides", "Atlantic white - cedar", 1, 4, 9, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CHCHC4", "Chrysolepis chrysophylla", "Giant chinkapin", 1, 24, 25, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CHCHM", "Chrysolepis chrysophylla", "Giant chinkapin", 1, 24, 25, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("COFL2", "Cornus florida", "Flowering dogwood", 1, 20, 34, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("COFL2", "Cornus florida", "Flowering dogwood", 101, 20, 34, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("CONU4", "Cornus nuttallii", "Pacific dogwood", 1, 35, 34, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CORSPP", "Cornus species", "Dogwoods", 1, 10, 34, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CRDO2", "Crataegus douglasii", "Black hawthorn", 1, 17, 35, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CRDOD", "Crataegus douglasii", "Black hawthorn", 1, 17, 35, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CRASPP", "Crataegus species", "Hawthorns", 1, 35, 35, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CUNO", "Cupressus nootkatensis", "Nootka Cypress", 1, 2, 9, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CUTH", "Cupressus thyoides", "Atlantic white - cedar", 1, 4, 9, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CUTHH", "Cupressus thyoides", "Atlantic white - cedar", 1, 4, 9, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("DIVI5", "Diospyros virginiana", "Persimmon", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("DIVIM2", "Diospyros virginiana", "Persimmon", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("DIVIP2", "Diospyros virginiana", "Persimmon", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("DIVIP3", "Diospyros virginiana", "Persimmon", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("DIVIV4", "Diospyros virginiana", "Persimmon", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FAGR", "Fagus grandifolia", "American beech", 1, 4, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FAGRH", "Fagus grandifolia", "American beech", 1, 4, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FAGRC", "Fagus grandifolia", "American beech", 1, 4, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRAM2", "Fraxinus americana", "White ash", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRAMB", "Fraxinus americana", "White ash", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRAMC", "Fraxinus americana", "White ash", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRAMC2", "Fraxinus americana", "White ash", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRAMJ", "Fraxinus americana", "White ash", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRAMM", "Fraxinus americana", "White ash", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRNI", "Fraxinus nigra", "Black ash", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRPE", "Fraxinus pennsylvanica", "Green ash", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRPEA", "Fraxinus pennsylvanica", "Green ash", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRPEI", "Fraxinus pennsylvanica", "Green ash", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRPEL", "Fraxinus pennsylvanica", "Green ash", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRPES", "Fraxinus pennsylvanica", "Green ash", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRPR", "Fraxinus profunda", "Pumpkin ash", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRQU", "Fraxinus quadrangulata", "Blue ash", 1, 9, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("FRASPP", "Fraxinus species", "Ashes", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("GLTR", "Gleditsia triacanthos", "Honeylocust", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("GLTRI", "Gleditsia triacanthos", "Honeylocust", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("GOLA", "Gordonia lasianthus", "Loblolly bay", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("GYDI", "Gymnocladus dioicus", "Kentucky coffeetree", 1, 10, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("HALSPP", "Halesia species", "Silverbells", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ILOP", "Ilex opaca", "American holly", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ILOPA", "Ilex opaca", "American holly", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ILOPO", "Ilex opaca", "American holly", 1, 21, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUCI", "Juglans cinerea", "Butternut", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUNI", "Juglans nigra", "Black walnut", 1, 20, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUOC", "Juniperus occidentalis", "Western juniper", 1, 24, 29, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUOCO", "Juniperus occidentalis", "Western juniper", 1, 24, 29, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUNSPP", "Juniperus species", "Junipers / Redcedars", 1, 19, 29, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVI", "Juniperus virginiana", "Eastern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVIS", "Juniperus virginiana", "Eastern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVIS3", "Juniperus virginiana", "Eastern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVIS", "Juniperus virginiana", "Southern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVIV", "Juniperus virginiana", "Southern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVIC", "Juniperus virginiana", "Eastern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("JUVIC2", "Juniperus virginiana", "Eastern redcedar", 1, 19, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LALA", "Larix laricina", "Tamarack", 1, 10, 14, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LALAA9", "Larix laricina", "Tamarack", 1, 10, 14, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LALAA5", "Larix laricina", "Tamarack", 1, 10, 14, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LALY", "Larix lyallii", "Subalpine Larch", 1, 29, 30, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LAOC", "Larix occidentalis", "Western Larch", 14, 36, 14, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LAOC", "Larix occidentalis", "Western Larch", -1, 36, 14, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::western_larch);
    insertRecord("CADE27", "Calocedrus decurrens", "Incense - cedar", 12, 34, 18, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("CADE27", "Calocedrus decurrens", "Incense - cedar", -1, 34, 18, -1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::incense_cedar);
    insertRecord("LIDE", "Libocedrus decurrens", "Incense - cedar", 12, 34, 18, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LIDE", "Libocedrus decurrens", "Incense - cedar", -1, 34, 18, -1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::incense_cedar);
    insertRecord("LIST2", "Liquidambar styraciflua", "Sweetgum", 1, 34, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LITU", "Liriodendron tulipifera", "Tuliptree", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LIDE3", "Lithocarpus densiflorus", "Tanoak", 1, 30, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LIDED2", "Lithocarpus densiflorus", "Tanoak", 1, 30, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("LIDEE", "Lithocarpus densiflorus", "Tanoak", 1, 30, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAPO", "Maclura pomifera", "Osage - orange", 1, 16, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAAC", "Magnolia acuminata", "Cucumber - tree", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAACC2", "Magnolia acuminata", "Cucumber - tree", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAACO", "Magnolia acuminata", "Cucumber - tree", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAACS", "Magnolia acuminata", "Cucumber - tree", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAGR4", "Magnolia grandiflora", "Southern magnolia", 1, 12, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAMA2", "Magnolia macrophylla", "Bigleaf magnolia", 1, 12, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAGSPP", "Magnolia species", "Magnolias", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAVI2", "Magnolia virginiana", "Sweetbay", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAVIT2", "Magnolia virginiana", "Sweetbay", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAVIA2", "Magnolia virginiana", "Sweetbay", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MAVIP", "Magnolia virginiana", "Sweetbay", 1, 19, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MALPRU", "Prunus species", "Apples / Cherries", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MALSPP", "Malus species", "Apples", 1, 22, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MOAL", "Morus alba", "White mulberry", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MOALM", "Morus alba", "White mulberry", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MOALT", "Morus alba", "White mulberry", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MORU2", "Morus rubra", "Red mulberry", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MORUR", "Morus rubra", "Red mulberry", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MORUT", "Morus rubra", "Red mulberry", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("MORSPP", "Morus species", "Mulberries", 1, 12, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYAQ2", "Nyssa aquatica", "Water tupelo", 1, 32, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYOG", "Nyssa ogeche", "Ogeechee tupelo", 1, 32, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYSSPP", "Nyssa species", "Tupelos", 1, 23, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYSY", "Nyssa sylvatica", "Blackgum", 1, 32, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYSY", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYSYB", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYSYC", "Nyssa sylvatica", "Blackgum", 1, 32, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYSYC", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYSYD", "Nyssa sylvatica", "Blackgum", 1, 32, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYSYD", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYSYT", "Nyssa sylvatica", "Blackgum", 1, 32, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYSYT", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYSYU", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYUR2", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("NYBI", "Nyssa biflora", "Swamp tupelo", 1, 32, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("NYBI", "Nyssa sylvatica", "Blackgum", 102, 32, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("OSVI", "Ostrya virginiana", "Hophornbeam", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("OSVIC", "Ostrya virginiana", "Chisos hophornbeam", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("OSVIV", "Ostrya virginiana", "Hophornbeam", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("OSVIL", "Ostrya virginiana", "Hophornbeam", 1, 16, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("OXAR", "Oxydendrum arboreum", "Sourwood", 1, 15, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("OXAR", "Oxydendrum arboreum", "Sourwood", 103, 15, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("PATO2", "Paulownia tomentosa", "Princesstree", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PEBO", "Persea borbonia", "Redbay", 1, 17, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIAB", "Picea abies", "Norway spruce", 3, 8, 10, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIEN", "Picea engelmannii", "Engelmann spruce", 15, 15, 10, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIEN", "Picea engelmannii", "Engelmann spruce", -1, 15, 10, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::engelmann_spruce);
    insertRecord("PIENE", "Picea engelmannii", "Engelmann spruce", 15, 15, 10, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIENE", "Picea engelmannii", "Engelmann spruce", -1, 15, 10, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::engelmann_spruce);
    insertRecord("PIENM2", "Picea engelmannii", "Engelmann spruce", 15, 15, 10, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIENM2", "Picea engelmannii", "Engelmann spruce", -1, 15, 10, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::engelmann_spruce);
    insertRecord("PIENM", "Picea engelmannii", "Engelmann spruce", 15, 15, 10, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIENM", "Picea engelmannii", "Engelmann spruce", -1, 15, 10, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::engelmann_spruce);
    insertRecord("PIGL", "Picea glauca", "White spruce", 3, 4, 10, 1, 2, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIMA", "Picea mariana", "Black spruce", 3, 11, 10, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIMAM4", "Picea mariana", "Black spruce", 3, 11, 10, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPU", "Picea pungens", "Blue spruce", 3, 10, 10, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPUG3", "Picea pungens", "Blue spruce", 3, 10, 10, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPUA", "Picea pungens", "Blue spruce", 3, 10, 10, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIRU", "Picea rubens", "Red spruce", 3, 13, 10, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PISI", "Picea sitchensis", "Sitka spruce", 3, 6, 10, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICSPP", "Picea species", "Spruces", 3, 13, 10, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIAL", "Pinus albicaulis", "Whitebark pine", 17, 9, 31, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIAL", "Pinus albicaulis", "Whitebark pine", -1, 9, 31, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PIAT", "Pinus attenuata", "Knobcone pine", 1, 9, 32, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIBA2", "Pinus banksiana", "Jack pine", 1, 19, 11, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICL", "Pinus clausa", "Sand pine", 1, 14, 11, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICLI", "Pinus clausa", "Sand pine", 1, 14, 11, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICLI2", "Pinus clausa", "Sand pine", 1, 14, 11, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICO", "Pinus contorta", "Lodgepole pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICO", "Pinus contorta", "Lodgepole pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOB", "Pinus contorta", "Bolander beach pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOB", "Pinus contorta", "Bolander beach pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOB2", "Pinus contorta", "Bolander beach pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOB2", "Pinus contorta", "Bolander beach pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOC", "Pinus contorta", "Beach pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOC", "Pinus contorta", "Beach pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOC2", "Pinus contorta", "Beach pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOC2", "Pinus contorta", "Beach pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOL", "Pinus contorta", "Lodgepole pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOL", "Pinus contorta", "Lodgepole pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOL2", "Pinus contorta", "Lodgepole pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOL2", "Pinus contorta", "Lodgepole pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOM", "Pinus contorta", "Sierra lodgepole pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOM", "Pinus contorta", "Sierra lodgepole pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PICOM4", "Pinus contorta", "Sierra lodgepole pine", 17, 7, 11, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PICOM4", "Pinus contorta", "Sierra lodgepole pine", -1, 7, 11, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::whitebark_pine);
    insertRecord("PIEC2", "Pinus echinata", "Shortleaf pine", 1, 16, 15, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIEL", "Pinus elliottii", "Slash pine", 1, 31, 15, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIELD", "Pinus elliottii", "Florida slash pine", 1, 31, 15, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIELA", "Pinus elliottii", "Florida slash pine", 1, 31, 15, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIELA2", "Pinus elliottii", "Florida slash pine", 1, 31, 15, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIELE2", "Pinus elliottii", "Slash pine", 1, 31, 15, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIFL2", "Pinus flexilis", "Limber pine", 1, 9, 31, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIFLA", "Pinus flexilis", "Limber pine", 1, 9, 31, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIFLA2", "Pinus flexilis", "Limber pine", 1, 9, 31, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIFLC", "Pinus flexilis", "Limber pine", 1, 9, 31, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIFLC2", "Pinus flexilis", "Limber pine", 1, 9, 31, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIGL2", "Pinus glabra", "Spruce pine", 1, 14, 11, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIJE", "Pinus jeffreyi", "Jeffrey pine", 19, 37, 12, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIJE", "Pinus jeffreyi", "Jeffrey pine", -1, 37, 12, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIJEK", "Pinus jeffreyi", "Jeffrey pine", -1, 37, 12, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PILA", "Pinus lambertiana", "Sugar pine", 18, 38, 13, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PILA", "Pinus lambertiana", "Sugar pine", -1, 38, 13, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::sugar_pine);
    insertRecord("PIMO3", "Pinus monticola", "Western white pine", 1, 14, 14, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPA2", "Pinus palustris", "Longleaf pine", 5, 100, 15, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPO", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPO", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPO_BH", "Pinus ponderosa", "Ponderosa Pine Black Hills", 21, 36, 15, 1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPO_BH", "Pinus ponderosa", "Ponderosa Pine Black Hills", -1, 36, 15, 1, -1, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOB2", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOB2", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOB", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOB", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOP2", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOP2", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOB3", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOB3", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOP", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOP", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOS", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOS", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOS2", "Pinus ponderosa", "Ponderosa pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOS2", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOW2", "Pinus ponderosa", "Washoe pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOW2", "Pinus ponderosa", "Washoe pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOW", "Pinus ponderosa", "Washoe pine", 19, 36, 15, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIPOW", "Pinus ponderosa", "Washoe pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_pine);
    insertRecord("PIPOK", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOBK", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOP2K", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOB3K", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOPK", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOSK", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOS2K", "Pinus ponderosa", "Ponderosa pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOW2K", "Pinus ponderosa", "Washoe pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPOWK", "Pinus ponderosa", "Washoe pine", -1, 36, 15, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::ponderosa_kill);
    insertRecord("PIPU5", "Pinus pungens", "Table mountain pine", 1, 19, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIRE", "Pinus resinosa", "Red pine", 1, 22, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIRI", "Pinus rigida", "Pitch pine", 1, 24, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PISA2", "Pinus sabiniana", "Gray pine", 1, 12, 15, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PISE", "Pinus serotina", "Pond pine", 1, 35, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PINSPP", "Pinus species", "Pines", 1, 9, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIST", "Pinus strobus", "Eastern white pine", 1, 24, 14, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PISY", "Pinus sylvestris", "Scots pine", 1, 9, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PITA", "Pinus taeda", "Loblolly pine", 1, 30, 15, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PIVI2", "Pinus virginiana", "Virginia pine", 1, 12, 11, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PLOC", "Platanus occidentalis", "American sycamore", 1, 12, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PLOCG", "Platanus occidentalis", "American sycamore", 1, 12, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBA2", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAB2", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAC2", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAF", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAL", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAM", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAS2", "Populus balsamifera", "Balsam poplar", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAT", "Populus balsamifera", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POBAC3", "Populus balsamifera", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODE3", "Populus deltoides", "Eastern cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODED", "Populus deltoides", "Eastern cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEA", "Populus deltoides", "Eastern cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEM2", "Populus deltoides", "Eastern cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEP", "Populus deltoides", "Eastern cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEV", "Populus deltoides", "Eastern cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEM", "Populus deltoides", "Plains cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEO", "Populus deltoides", "Plains cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEW", "Populus deltoides", "Rio Grande cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PODEW3", "Populus deltoides", "Rio Grande cottonwood", 1, 19, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POGR4", "Populus grandidentata", "Bigtooth aspen", 1, 18, 26, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POGRA2", "Populus grandidentata", "Bigtooth aspen", 1, 18, 26, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POGRM4", "Populus grandidentata", "Bigtooth aspen", 1, 18, 26, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POGRS", "Populus grandidentata", "Bigtooth aspen", 1, 18, 26, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POHE4", "Populus heterophylla", "Swamp cottonwood", 1, 29, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POPSPP", "Populus species", "Poplars", 1, 17, 27, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTR5", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRA", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTR5", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRC2", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTR12", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRM", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRR", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRV", "Populus tremuloides", "Quaking aspen", 4, 23, 26, 1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTR15", "Populus trichocarpa", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRH", "Populus trichocarpa", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRC", "Populus trichocarpa", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRH2", "Populus trichocarpa", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("POTRI", "Populus trichocarpa", "Black cottonwood", 1, 23, 27, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRAM", "Prunus americana", "American plum", 1, 19, 39, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PREM", "Prunus emarginata", "Bitter cherry", 1, 35, 36, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PREME", "Prunus emarginata", "Bitter cherry", 1, 35, 36, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PREMC", "Prunus emarginata", "Bitter cherry", 1, 35, 36, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PREMM", "Prunus emarginata", "Bitter cherry", 1, 35, 36, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRPE2", "Prunus pensylvanica", "Pin cherry", 1, 24, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRPEP", "Prunus pensylvanica", "Pin cherry", 1, 24, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRPES", "Prunus pensylvanica", "Pin cherry", 1, 24, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRPEC", "Prunus pensylvanica", "Pin cherry", 1, 24, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRSE2", "Prunus serotina", "Black cherry", 1, 9, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRSEE", "Prunus serotina", "Black cherry", 1, 9, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRSEE2", "Prunus serotina", "Black cherry", 1, 9, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRSER2", "Prunus serotina", "Black cherry", 1, 9, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRSES", "Prunus serotina", "Black cherry", 1, 9, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRSEV", "Prunus serotina", "Black cherry", 1, 9, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRUSPP", "Prunus species", "Cherries", 1, 29, 36, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRVI", "Prunus virginiana", "Chokecherry", 1, 19, 36, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRVID", "Prunus virginiana", "Western chokecherry", 1, 19, 36, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRVID2", "Prunus virginiana", "Chokecherry", 1, 19, 36, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRVIM", "Prunus virginiana", "Black chokecherry", 1, 19, 36, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRVIM2", "Prunus virginiana", "Chokecherry", 1, 19, 36, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PRVIV", "Prunus virginiana", "Chokecherry", 1, 19, 36, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PSME", "Pseudotsuga menziesii", "Douglas - fir", 20, 36, 16, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PSME", "Pseudotsuga menziesii", "Douglas - fir", -1, 36, 16, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::douglas_fir);
    insertRecord("PSMEG", "Pseudotsuga menziesii", "Rocky Mtn Douglas - fir", 20, 36, 16, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PSMEG", "Pseudotsuga menziesii", "Rocky Mtn Douglas - fir", -1, 36, 16, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::douglas_fir);
    insertRecord("PSMEF", "Pseudotsuga menziesii", "Douglas - fir", 20, 36, 16, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PSMEF", "Pseudotsuga menziesii", "Douglas - fir", -1, 36, 16, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::douglas_fir);
    insertRecord("PSMEM", "Pseudotsuga menziesii", "Douglas - fir", 20, 36, 16, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("PSMEM", "Pseudotsuga menziesii", "Douglas - fir", -1, 36, 16, 1, 2, -1, -1, EquationType::crown_damage, CrownDamageEquationCode::douglas_fir);
    insertRecord("QUAG", "Quercus agrifolia", "California live oak", 1, 29, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUAGA", "Quercus agrifolia", "California live oak", 1, 29, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUAGO", "Quercus agrifolia", "Coastal live oak", 1, 29, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUAL", "Quercus alba", "White oak", 1, 19, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUAL", "Quercus alba", "White oak", 104, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUALS", "Quercus alba", "White oak", 1, 19, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUALS", "Quercus alba", "White oak", 104, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUALS2", "Quercus alba", "White oak", 1, 19, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUALS2", "Quercus alba", "White oak", 104, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUAL3", "Quercus alba", "White oak", 104, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUBI", "Quercus bicolor", "Swamp white oak", 1, 24, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCH2", "Quercus chrysolepis", "Canyon live oak", 1, 3, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCHC", "Quercus chrysolepis", "Canyon live oak", 1, 3, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCHN", "Quercus chrysolepis", "Canyon live oak", 1, 3, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCO2", "Quercus coccinea", "Scarlet oak", 1, 19, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCO2", "Quercus coccinea", "Scarlet oak", 105, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUCOC", "Quercus coccinea", "Scarlet oak", 1, 19, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCOC", "Quercus coccinea", "Scarlet oak", 105, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUCOT", "Quercus coccinea", "Scarlet oak", 1, 19, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUCOT", "Quercus coccinea", "Scarlet oak", 105, 19, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUDU", "Quercus douglasii", "Blue oak", 1, 12, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUEL", "Quercus ellipsoidalis", "Northern pin oak", 1, 17, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUEN", "Quercus engelmannii", "Engelmann oak", 1, 33, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUFA", "Quercus falcata", "Southern red oak", 1, 23, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUFAT", "Quercus falcata", "Southern red oak", 1, 23, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUGA4", "Quercus garryana", "Oregon white oak", 1, 8, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUGAG2", "Quercus garryana", "Oregon white oak", 1, 8, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUGAS", "Quercus garryana", "Oregon white oak", 1, 8, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUIM", "Quercus imbricaria", "Shingle oak", 1, 20, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUIN", "Quercus incana", "Bluejack oak", 1, 17, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUKE", "Quercus kelloggii", "Califonia black oak", 1, 9, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULA2", "Quercus laevis", "Turkey oak", 1, 16, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULA3", "Quercus laurifolia", "Laurel oak", 1, 15, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULO", "Quercus lobata", "Valley oak", 1, 22, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULOA", "Quercus lobata", "Valley oak", 1, 22, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULOI", "Quercus lobata", "Valley oak", 1, 22, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULOT", "Quercus lobata", "Valley oak", 1, 22, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULOW", "Quercus lobata", "Valley oak", 1, 22, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QULY", "Quercus lyrata", "Overcup oak", 1, 18, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMA2", "Quercus macrocarpa", "Bur oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMAD", "Quercus macrocarpa", "Bur oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMAM", "Quercus macrocarpa", "Bur oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMAO", "Quercus macrocarpa", "Bur oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMA3", "Quercus marilandica", "Blackjack oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMA3", "Quercus marilandica", "Blackjack oak", 106, 16, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUMAA2", "Quercus marilandica", "Blackjack oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMAA2", "Quercus marilandica", "Blackjack oak", 106, 16, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUMAA", "Quercus marilandica", "Blackjack oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMAA", "Quercus marilandica", "Blackjack oak", 106, 16, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUMAM2", "Quercus marilandica", "Blackjack oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMAM2", "Quercus marilandica", "Blackjack oak", 106, 16, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUMI", "Quercus michauxii", "Swamp chestnut oak", 1, 25, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUPR4", "Quercus michauxii", "Swamp chestnut oak", 1, 25, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMU", "Quercus muehlenbergii", "Chinkapin oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMO4", "Quercus montana", "Chestnut oak", 107, 25, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUNI", "Quercus nigra", "Water oak", 1, 15, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUNIH", "Quercus nigra", "Water oak", 1, 15, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUNU", "Quercus nuttallii", "Nuttall oak", 1, 9, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUNUC", "Quercus nuttallii", "Nuttall oak", 1, 9, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUTE", "Quercus texana", "Texas red oak", 1, 9, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUPA2", "Quercus palustris", "Pin oak", 1, 20, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUPH", "Quercus phellos", "Willow oak", 1, 20, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUPR2", "Quercus prinus", "Chestnut oak", 1, 28, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUMO4", "Quercus prinus", "Chestnut oak", 1, 28, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QURU", "Quercus rubra", "Northern red oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QURUA", "Quercus rubra", "Northern red oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QURUB", "Quercus rubra", "Northern red oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QURUR", "Quercus rubra", "Northern red oak", 1, 21, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUSH", "Quercus shumardii", "Shumard oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUSHS", "Quercus shumardii", "Shneck oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUSHS2", "Quercus shumardii", "Shumard oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUSHS3", "Quercus shumardii", "Shumard oak", 1, 16, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUESPP", "Quercus species", "Oaks", 1, 24, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUST", "Quercus stellata", "Post oak", 1, 23, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUSTA2", "Quercus stellata", "Post oak", 1, 23, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUSTP2", "Quercus stellata", "Post oak", 1, 23, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUVE", "Quercus velutina", "Black oak", 1, 24, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUVE", "Quercus velutina", "Black oak", 108, 24, 28, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("QUVEM", "Quercus velutina", "Black oak", 1, 24, 28, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUVI", "Quercus virginiana", "Live oak", 1, 22, 28, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUWI2", "Quercus wislizeni", "Interior live oak", 1, 13, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUWIF", "Quercus wislizeni", "Interior live oak", 1, 13, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("QUWIW", "Quercus wislizeni", "Interior live oak", 1, 13, 28, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ROPS", "Robinia pseudoacacia", "Black locust", 1, 28, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("RURU5", "Acer rubrum", " Red maple", 100, 7, 21, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("SABE2", "Salix bebbiana", "Bebb willow", 1, 19, 37, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SABED", "Salix bebbiana", "Bebb willow", 1, 19, 37, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SABEL", "Salix bebbiana", "Bebb willow", 1, 19, 37, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SABEP", "Salix bebbiana", "Bebb willow", 1, 19, 37, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SABEP2", "Salix bebbiana", "Bebb willow", 1, 19, 37, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANI", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIA2", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIB", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIB2", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIF2", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIA2", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIL2", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIL3", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SANIM2", "Salix nigra", "Black willow", 1, 19, 37, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SALSPP", "Salix species", "Willows", 1, 20, 37, -1, 2, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SAAL5", "Sassafras albidum", "Sassafras", 1, 14, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SAAL5", "Sassafras albidum", "Sassafras", 109, 14, 39, -1, -1, -1, 4, EquationType::bole_char, CrownDamageEquationCode::not_set);
    insertRecord("SEGI2", "Sequoia gigantea", "Giant sequoia", 1, 39, 17, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SEGI3", "Sequoia gigantea", "Giant sequoia", 1, 39, 17, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SESE3", "Sequoia sempervirens", "Redwood", 1, 39, 17, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("SOAM3", "Sorbus americana", "American mountain - ash", 1, 19, 39, -1, -1, 3, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TABR2", "Taxus brevifolia", "Pacific yew", 1, 4, 33, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TADI2", "Taxodium distichum", "Bald cypress", 1, 4, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TAAS", "Taxodium ascendens", "Pond cypress", 1, 21, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TADII", "Taxodium ascendens", "Pond cypress", 1, 21, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TADIN", "Taxodium ascendens", "Pond cypress", 1, 21, 39, -1, -1, -1, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THOC2", "Thuja occidentalis", "arborvitae", 1, 4, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THOCF", "Thuja occidentalis", "arborvitae", 1, 4, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THOCN", "Thuja occidentalis", "arborvitae", 1, 4, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THOCP3", "Thuja occidentalis", "arborvitae", 1, 4, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THOCM3", "Thuja occidentalis", "arborvitae", 1, 4, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THPL", "Thuja plicata", "Western redcedar", 1, 14, 18, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("THUSPP", "Thuju species", "Arborvitae", 1, 12, 18, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TIAM", "Tilia americana", "American basswood", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TIAMA", "Tilia americana", "American basswood", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TIAMC", "Tilia americana", "Carolina basswood", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TIAMH", "Tilia americana", "American basswood", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TIAMN", "Tilia americana", "American basswood", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TIHE", "Tilia heterophylla", "White basswood", 1, 29, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TSCA", "Tsuga canadensis", "Eastern hemlock", 1, 18, 19, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TSHE", "Tsuga heterophylla", "Western hemlock", 1, 19, 19, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("TSME", "Tsuga mertensiana", "Mountain hemlock", 1, 19, 20, 1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULAL", "Ulmus alata", "Winged elm", 1, 10, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULAM", "Ulmus americana", "American elm", 1, 10, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULAMF2", "Ulmus americana", "American elm", 1, 10, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULPU", "Ulmus pumila", "Siberian elm", 1, 17, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULRU", "Ulmus rubra", "Slippery elm", 1, 11, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULMSPP", "Ulmus species", "Elms", 1, 18, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULTH", "Ulmus thomasii", "Rock elm", 1, 12, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("ULRA", "Ulmus racemosa", "Rock elm", 1, 12, 39, -1, -1, 3, 4, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("UMCA", "Umbellularia californica", "California - laurel", 1, 5, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("UMCAC", "Umbellularia californica", "California - laurel", 1, 5, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("UMCAF", "Umbellularia californica", "California - laurel", 1, 5, 39, -1, 2, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set);
    insertRecord("", "", "", 1, -1, -1, -1, -1, -1, -1, EquationType::not_set, CrownDamageEquationCode::not_set);
}

int SpeciesMasterTable::getSpeciesTableIndexFromSpeciesCode(string speciesCode) const
{
    int i;
    string speciesCodeTemp;
    speciesCodeTemp = speciesCode;
    std::transform(speciesCodeTemp.begin(), speciesCodeTemp.end(), speciesCodeTemp.begin(), ::toupper); // got to upper case

    for(i = 0; i < record_.size(); i++)
    {
        if(record_[i].speciesCode_ == "")
        {
            break;
        }
        if(record_[i].speciesCode_ == speciesCodeTemp)
        {
            return i;
        }
    }

    return -1;
}

/****************************************************************************
* Name: getSpeciesTableIndexFromSpeciesCode
* Desc: Look for Species in the SMT and return the table index
*   In: cr_NameCode.......Species to locate
*  Ret: index of species in table
*       -1 if not found
****************************************************************************/
int SpeciesMasterTable::getSpeciesTableIndexFromSpeciesCodeAndEquationType(string speciesCode, EquationType equationType) const
{
    int i;
    string speciesCodeTemp;
    speciesCodeTemp = speciesCode;
    std::transform(speciesCodeTemp.begin(), speciesCodeTemp.end(), speciesCodeTemp.begin(), ::toupper); // got to upper case

    for (i = 0; i < record_.size(); i++)
    {
        if(record_[i].speciesCode_ =="")
        {
            break;
        }
        if((record_[i].speciesCode_ == speciesCodeTemp) && (record_[i].equationType_ == equationType))
        {
            return i;
        }
    }

    return -1;
}

SpeciesMasterTableRecord::SpeciesMasterTableRecord()
{
    speciesCode_ = "";
    scientificName_ = "";
    commonName_ = "";
    mortalityEquationNumber_ = -1;
    barkEquationNumber_ = -1;
    regionInteriorWest_ = false;
    regionPacificWest_ = false;
    regionNorthEast_ = false;
    regionSouthEast_ = false;
    crownCoefficientCode_ = -1;      /* canopy cover equation #, (FVS Species Index No. )      */
    equationType_ = EquationType::not_set;
    crownDamageEquationCode_ = CrownDamageEquationCode::not_set;
}

SpeciesMasterTableRecord::SpeciesMasterTableRecord(const SpeciesMasterTableRecord& rhs)
{
    memberwiseCopyAssignment(rhs);
}

SpeciesMasterTableRecord& SpeciesMasterTableRecord::operator=(const SpeciesMasterTableRecord& rhs)
{
    if(this != &rhs)
    {
        memberwiseCopyAssignment(rhs);
    }
    return *this;
}

SpeciesMasterTableRecord::~SpeciesMasterTableRecord()
{
}

void SpeciesMasterTableRecord::memberwiseCopyAssignment(const SpeciesMasterTableRecord& rhs)
{
    speciesCode_ = rhs.speciesCode_;
    scientificName_ = rhs.scientificName_;
    commonName_ = rhs.commonName_;

    mortalityEquationNumber_ = rhs.mortalityEquationNumber_;
    barkEquationNumber_ = rhs.barkEquationNumber_;
    crownCoefficientCode_ = rhs.crownCoefficientCode_;
    regionInteriorWest_ = rhs.regionInteriorWest_;
    regionPacificWest_ = rhs.regionPacificWest_;
    regionNorthEast_ = rhs.regionNorthEast_;
    regionSouthEast_ = rhs.regionSouthEast_;
    equationType_ = rhs.equationType_;
    crownDamageEquationCode_ = rhs.crownDamageEquationCode_;
}

void SpeciesMasterTable::insertRecord(string speciesCode, string scientificName, string commonName,
    int  mortalityEquation, int  brkEqu, int  crownCoefficientCode,
    int8_t region1, int8_t  region2, int8_t  region3, int8_t region4, EquationType equationType,
    CrownDamageEquationCode crownDamageEquationCode)
{
    SpeciesMasterTableRecord recordTemp;
    recordTemp.speciesCode_ = speciesCode;
    recordTemp.scientificName_ = scientificName;
    recordTemp.commonName_ = commonName;

    recordTemp.mortalityEquationNumber_ = mortalityEquation;
    recordTemp.barkEquationNumber_ = brkEqu;
    recordTemp.crownCoefficientCode_ = crownCoefficientCode;
    recordTemp.regionInteriorWest_ = region1;
    recordTemp.regionPacificWest_ = region2;
    recordTemp.regionNorthEast_ = region3;
    recordTemp.regionSouthEast_ = region4;
    recordTemp.equationType_ = equationType;
    recordTemp.crownDamageEquationCode_ = crownDamageEquationCode;

    record_.push_back(recordTemp);
}
