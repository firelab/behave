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
        //Reg = GACC Region Code
        //    AICC - Alaska
        //    ONCC & OSCC - Northern and Southern California
        //    EACC - Eastern Area
        //    GBCC - Great Basin
        //    NRCC - Northern Rockies
        //    NWCC - North West
        //    RMCC - Rocky Mountain
        //    SACC - Southern Area
        //    SWCC - South West
        //Equatio_Type
        //CRCABE = Crown Damage / Cambium / Beetle Equations(CRCABE)
    //Species,Scientific_Name,Common_Name,Mort,Brk,Crn,IW,PW,NE,SE,Equation_Type,CRCABE
    std::vector<SpeciesMasterTableRecord> tmp_records = {
        // "{Species, Scientific_Name, Common_Name, MORT, BKR, CRN, AICC, ONCC & OSCC, EACC, GBCC, NRCC, NWCC, RMCC, SACC, SWCC, Equation_Type, CRCABE},
        {"ABAM", "Abies amabilis", "Pacific silver fir", 1, 26, 1, 1, -1, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABBA", "Abies balsamea", "balsam fir", 1, 10, 2, -1, -1, 3, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABCO", "Abies concolor", "white fir", 10, 27, 2, -1, 2, 3, 4, -1, 6, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABGR", "Abies grandis", "grand fir", 11, 25, 3, -1, 2, -1, 4, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABLA", "Abies lasiocarpa", "corkbark fir", 11, 20, 4, 1, 2, -1, 4, 5, 6, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABLA", "Abies lasiocarpa", "subalpine fir", 11, 20, 4, 1, 2, -1, 4, 5, 6, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABMA", "Abies magnifica", "California red fir", 16, 18, 5, -1, 2, -1, 4, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ABPR", "Abies procera", "noble fir", 1, 24, 7, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACBA3", "Acer barbatum", "Florida maple", 1, 8, 21, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACMA3", "Acer macrophyllum", "bigleaf maple", 1, 3, 21, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACNE2", "Acer negundo", "boxelder", 1, 13, 21, -1, 2, 3, 4, 5, 6, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACNI5", "Acer nigrum", "black maple", 1, 14, 21, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACPE", "Acer pensylvanicum", "striped maple", 1, 24, 21, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACRU", "Acer rubrum", "red maple", 100, 7, 21, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"ACSA2", "Acer saccharinum", "silver maple", 1, 10, 21, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACSA3", "Acer saccharum", "sugar maple", 1, 12, 21, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ACSP2", "Acer spicatum", "mountain maple", 1, 19, 21, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"AEFL", "Aesculus flava", "yellow buckeye", 1, 29, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"AEGL", "Aesculus glabra", "Ohio buckeye", 1, 15, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"AIAL", "Ailanthus altissima", "ailanthus", 1, 29, 39, -1, 2, 3, -1, -1, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ALRH2", "Alnus rhombifolia", "white alder", 1, 1, 23, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ALRU2", "Alnus rubra", "red alder", 1, 1, 22, 1, 2, 3, -1, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"AMAR3", "Amelanchier arborea", "common serviceberry", 1, 29, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ARME", "Arbutus menziesii", "Pacific madrone", 1, 34, 39, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"BEAL2", "Betula alleghaniensis", "yellow birch", 1, 10, 24, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"BELE", "Betula lenta", "sweet birch", 1, 9, 24, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"BENI", "Betula nigra", "river birch", 1, 8, 24, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"BEOC2", "Betula occidentalis", "water birch", 1, 1, 24, -1, 2, -1, 4, 5, 6, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"BEPA", "Betula papyrifera", "paper birch", 1, 1, 24, 1, -1, 3, -1, 5, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"BETSPP", "Betula species", "birch species", 1, 1, 24, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CADE27", "Calocedrus decurrens", "incense-cedar", 12, 34, 18, -1, 2, -1, 4, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CACA18", "Carpinus caroliniana", "American hornbeam, musclewood", 1, 9, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CAAL27", "Carya alba", "mockernut hickory", 1, 22, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CAAQ", "Carya aquatica", "water hickory", 1, 19, 0, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CACOL3", "Carya cordiformis", "bitternut hickory", 1, 16, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CAGL8", "Carya glabra", "pignut hickory", 1, 16, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CAIL2", "Carya illinoinensis", "pecan", 1, 15, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CALA21", "Carya laciniosa", "shellbark hickory", 1, 22, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CAOV2", "Carya ovata", "shagbark hickory", 1, 19, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CARSPP", "Carya species", "hickory species", 1, 23, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CATE9", "Carya texana", "black hickory", 1, 19, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CADE12", "Castanea dentata", "American chestnut", 1, 19, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CELA", "Celtis laevigata", "sugarberry", 1, 15, 39, -1, -1, 3, -1, -1, 6, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CELA", "Celtis laevigata", "netleaf hackberry", 1, 15, 39, -1, -1, 3, -1, -1, 6, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CEOC", "Celtis occidentalis", "hackberry", 1, 14, 24, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CECA4", "Cercis canadensis", "eastern redbud", 1, 14, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CHLA", "Chamaecyparis lawsoniana", "Port-Orford-cedar", 1, 39, 9, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CHNO", "Chamaecyparis nootkatensis", "Alaska yellow-cedar", 1, 2, 9, 1, -1, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CHTH2", "Chamaecyparis thyoides", "Atlantic white-cedar", 1, 4, 9, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CHCHC4", "Chrysolepis chrysophylla", "giant chinkapin, golden chinkapin", 1, 24, 25, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"COFL2", "Cornus florida", "flowering dogwood", 101, 20, 34, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"CONU4", "Cornus nuttallii", "Pacific dogwood", 1, 35, 34, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"CRASPP", "Crataegus species", "hawthorn species", 1, 35, 35, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"DIVI5", "Diospyros virginiana", "common persimmon", 1, 20, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FAGR", "Fagus grandifolia", "American beech", 1, 4, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FRAM2", "Fraxinus americana", "white ash", 1, 21, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FRNI", "Fraxinus nigra", "black ash", 1, 14, 39, -1, -1, 3, -1, 5, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FRPE", "Fraxinus pennsylvanica", "green ash", 1, 18, 39, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FRPR", "Fraxinus profunda", "pumpkin ash", 1, 16, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FRQU", "Fraxinus quadrangulata", "blue ash", 1, 9, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"FRASPP", "Fraxinus species", "ash species", 1, 21, 39, -1, 2, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"GLTR", "Gleditsia triacanthos", "honeylocust", 1, 17, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"GOLA", "Gordonia lasianthus", "loblolly-bay", 1, 17, 39, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"GYDI", "Gymnocladus dioicus", "Kentucky coffeetree", 1, 10, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"HALSPP", "Halesia species", "silverbell species", 1, 17, 39, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ILOP", "Ilex opaca", "American holly", 1, 21, 39, -1, 2, 3, -1, -1, 6, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"JUCI", "Juglans cinerea", "butternut", 1, 20, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"JUNI", "Juglans nigra", "black walnut", 1, 20, 39, -1, -1, 3, -1, -1, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"JUOC", "Juniperus occidentalis", "western juniper", 1, 24, 29, -1, 2, -1, 4, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"JUVI", "Juniperus virginiana", "southern redcedar", 1, 19, 18, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"JUVI", "Juniperus virginiana", "eastern redcedar", 1, 19, 18, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"LALA", "Larix laricina", "tamarack", 1, 10, 14, 1, -1, 3, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"LALY", "Larix lyallii", "subalpine larch", 1, 29, 3, -1, -1, -1, -1, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"LAOC", "Larix occidentalis", "western larch", 14, 36, 14, -1, 2, -1, 4, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"LIST2", "Liquidambar styraciflua", "sweetgum", 1, 34, 39, -1, 2, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"LITU", "Liriodendron tulipifera", "yellow-poplar", 1, 29, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"LIDE3", "Lithocarpus densiflorus", "tanoak", 1, 30, 39, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MAPO", "Maclura pomifera", "Osage-orange", 1, 16, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MAAC", "Magnolia acuminata", "cucumbertree", 1, 15, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MAGR4", "Magnolia grandiflora", "southern magnolia", 1, 12, 39, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MAMA2", "Magnolia macrophylla", "bigleaf magnolia", 1, 12, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MAGSPP", "Magnolia species", "magnolia species", 1, 18, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MAVI2", "Magnolia virginiana", "sweetbay", 1, 19, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MALSPP", "Malus species", "apple species", 1, 22, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MOAL", "Morus alba", "white mulberry", 1, 17, 39, -1, 2, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MORU2", "Morus rubra", "red mulberry", 1, 17, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MORSPP", "Morus species", "mulberry species", 1, 12, 39, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"NYAQ2", "Nyssa aquatica", "water tupelo", 1, 32, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"NYBI", "Nyssa biflora", "swamp tupelo", 1, 32, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"NYOG", "Nyssa ogeche", "Ogeechee tupelo", 1, 32, 39, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"NYSY", "Nyssa sylvatica", "blackgum", 102, 32, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"OSVI", "Ostrya virginiana", "eastern hophornbeam", 1, 16, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"OXAR", "Oxydendrum arboreum", "sourwood", 103, 15, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"PATO2", "Paulownia tomentosa", "paulownia, empress-tree", 1, 29, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PEBO", "Persea borbonia", "redbay", 1, 17, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIAB", "Picea abies", "Norway spruce", 3, 8, 1, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIEN", "Picea engelmannii", "Engelmann spruce", 15, 15, 1, -1, 2, -1, 4, 5, 6, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIGL", "Picea glauca", "white spruce", 3, 4, 1, 1, -1, 3, -1, 5, -1, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIMA", "Picea mariana", "black spruce", 3, 11, 1, 1, -1, 3, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIPU", "Picea pungens", "blue spruce", 3, 10, 1, -1, -1, 3, 4, 5, -1, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIRU", "Picea rubens", "red spruce", 3, 13, 1, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PISI", "Picea sitchensis", "Sitka spruce", 3, 6, 1, 1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIAL", "Pinus albicaulis", "whitebark pine", 17, 9, 31, -1, 2, -1, 4, 5, 6, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIAT", "Pinus attenuata", "knobcone pine", 1, 9, 32, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIBA2", "Pinus banksiana", "jack pine", 1, 19, 11, -1, -1, 3, -1, -1, -1, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PICL", "Pinus clausa", "sand pine", 1, 14, 11, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PICO", "Pinus contorta", "lodgepole pine", 17, 7, 11, 1, 2, -1, 4, 5, 6, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIEC2", "Pinus echinata", "shortleaf pine", 1, 16, 15, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIEL", "Pinus elliottii", "slash pine", 1, 31, 15, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIFL2", "Pinus flexilis", "limber pine", 1, 9, 31, -1, 2, -1, 4, 5, -1, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIGL2", "Pinus glabra", "spruce pine", 1, 14, 11, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIJE", "Pinus jeffreyi", "Jeffrey pine", 19, 37, 12, -1, 2, -1, 4, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PILA", "Pinus lambertiana", "sugar pine", 18, 38, 13, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIMO3", "Pinus monticola", "western white pine", 1, 14, 14, -1, 2, -1, 4, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIPA2", "Pinus palustris", "longleaf pine", 5, 100, 15, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIPO", "Pinus ponderosa", "ponderosa pine", 19, 36, 15, -1, 2, 3, 4, 5, 6, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIPU5", "Pinus pungens", "Table Mountain pine", 1, 19, 11, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIRE", "Pinus resinosa", "red pine", 1, 22, 11, -1, -1, 3, -1, -1, -1, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIRI", "Pinus rigida", "pitch pine", 1, 24, 11, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PISA2", "Pinus sabiniana", "gray or California foothill pine", 1, 12, 15, -1, 2, -1, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PISE", "Pinus serotina", "pond pine", 1, 35, 11, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIST", "Pinus strobus", "eastern white pine", 1, 24, 14, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PISY", "Pinus sylvestris", "Scotch pine", 1, 9, 11, -1, -1, 3, -1, 5, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PITA", "Pinus taeda", "loblolly pine", 1, 30, 15, -1, 2, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PIVI2", "Pinus virginiana", "Virginia pine", 1, 12, 11, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PLOC", "Platanus occidentalis", "American sycamore", 1, 12, 39, -1, 2, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"POBA2", "Populus balsamifera", "balsam poplar", 1, 19, 27, 1, 2, 3, 4, 5, 6, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"POBA2", "Populus balsamifera", "black cottonwood", 1, 19, 27, 1, 2, 3, 4, 5, 6, 7, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PODE", "Populus deltoides", "eastern cottonwood", 1, 19, 0, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PODE", "Populus deltoides", "plains cottonwood", 1, 19, 0, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"POGR4", "Populus grandidentata", "bigtooth aspen", 1, 18, 26, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"POHE4", "Populus heterophylla", "swamp cottonwood", 1, 29, 27, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"POTR12", "Populus tremuloides", "quaking aspen", 4, 23, 26, 1, 2, 3, 4, 5, 6, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PRAM", "Prunus americana", "American plum", 1, 19, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PREM", "Prunus emarginata", "bitter cherry", 1, 35, 36, -1, 2, -1, -1, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PRPE2", "Prunus pensylvanica", "pin cherry", 1, 24, 36, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PRSE2", "Prunus serotina", "black cherry", 1, 9, 36, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"MALPRU", "Prunus species", "cherry and plum species", 1, 17, 39, -1, -1, 3, -1, -1, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PRVI", "Prunus virginiana", "chokecherry", 1, 19, 36, -1, 2, 3, 4, 5, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"PSME", "Pseudotsuga menziesii", "Douglas-fir", 20, 36, 16, -1, 2, 3, 4, 5, 6, 7, -1, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUAG", "Quercus agrifolia", "California live oak", 1, 29, 28, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUAL", "Quercus alba", "white oak", 104, 19, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"QUBI", "Quercus bicolor", "swamp white oak", 1, 24, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUCH2", "Quercus chrysolepis", "canyon live oak", 1, 3, 28, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUCO2", "Quercus coccinea", "scarlet oak", 105, 19, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"QUDU", "Quercus douglasii", "blue oak", 1, 12, 28, -1, 2, -1, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUEL", "Quercus ellipsoidalis", "northern pin oak", 1, 17, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUFA", "Quercus falcata", "southern red oak", 1, 23, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUGA4", "Quercus garryana", "Oregon white oak", 1, 8, 28, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUIM", "Quercus imbricaria", "shingle oak", 1, 20, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUIN", "Quercus incana", "bluejack oak", 1, 17, 28, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUKE", "Quercus kelloggii", "California black oak", 1, 9, 28, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QULA2", "Quercus laevis", "turkey oak", 1, 16, 28, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QULA3", "Quercus laurifolia", "laurel oak", 1, 15, 28, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QULO", "Quercus lobata", "California white oak", 1, 22, 28, -1, 2, -1, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QULY", "Quercus lyrata", "overcup oak", 1, 18, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUMA2", "Quercus macrocarpa", "bur oak", 1, 21, 28, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUMA3", "Quercus marilandica", "blackjack oak", 106, 16, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"QUMI", "Quercus michauxii", "swamp chestnut oak", 1, 25, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUEMON", "Quercus montana", "chestnut oak", 107, 28, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"QUMU", "Quercus muehlenbergii", "chinkapin oak", 1, 21, 28, -1, -1, 3, -1, -1, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUNI", "Quercus nigra", "water oak", 1, 15, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUPA2", "Quercus palustris", "pin oak", 1, 20, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUPH", "Quercus phellos", "willow oak", 1, 20, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QURU", "Quercus rubra", "northern red oak", 1, 21, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUSH", "Quercus shumardii", "Shumard oak", 1, 16, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUESPP", "Quercus species", "oak species", 1, 24, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUST", "Quercus stellata", "post oak", 1, 23, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUTE", "Quercus texana", "Nuttall oak", 1, 9, 28, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUVE", "Quercus velutina", "black oak", 108, 24, 28, -1, -1, 3, -1, -1, -1, 7, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"QUVI", "Quercus virginiana", "live oak", 1, 22, 28, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"QUWI2", "Quercus wislizeni", "interior live oak", 1, 13, 28, -1, 2, -1, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ROPS", "Robinia pseudoacacia", "black locust", 1, 28, 39, -1, -1, 3, -1, -1, 6, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"SABE2", "Salix bebbiana", "Bebb willow", 1, 19, 37, -1, -1, 3, -1, -1, -1, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"SANI", "Salix nigra", "black willow", 1, 19, 37, -1, 2, 3, -1, 5, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"SALSPP", "Salix species", "willow species", 1, 20, 37, -1, 2, 3, -1, 5, 6, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"SAAL5", "Sassafras albidum", "sassafras", 109, 14, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::bole_char, CrownDamageEquationCode::not_set},
        {"SESE", "Sequoia sempervirens", "redwood", 1, 39, 0, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"SOAM3", "Sorbus americana", "American mountain-ash", 1, 19, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TAAS", "Taxodium ascendens", "pondcypress", 1, 21, 39, -1, -1, -1, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TADI2", "Taxodium distichum", "baldcypress", 1, 4, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TABR2", "Taxus brevifolia", "Pacific yew", 1, 4, 33, 1, 2, -1, -1, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"THOC2", "Thuja occidentalis", "northern white-cedar", 1, 4, 18, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"THPL", "Thuja plicata", "western redcedar", 1, 14, 18, 1, 2, -1, -1, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TIAM", "Tilia americana", "American basswood", 1, 17, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TIAM", "Tilia americana", "white basswood", 1, 17, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TIAM", "Tilia americana", "Carolina basswood", 1, 17, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TSCA", "Tsuga canadensis", "eastern hemlock", 1, 18, 19, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TSHE", "Tsuga heterophylla", "western hemlock", 1, 19, 19, 1, 2, -1, -1, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"TSME", "Tsuga mertensiana", "mountain hemlock", 1, 19, 2, 1, 2, -1, 4, 5, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ULAL", "Ulmus alata", "winged elm", 1, 10, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ULAM", "Ulmus americana", "American elm", 1, 10, 39, -1, -1, 3, -1, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ULPU", "Ulmus pumila", "Siberian elm", 1, 17, 39, -1, -1, 3, 4, 5, -1, 7, 8, 9, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ULRU", "Ulmus rubra", "slippery elm", 1, 11, 39, -1, -1, 3, -1, 5, -1, 7, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ULMSPP", "Ulmus species", "elm species", 1, 18, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"ULTH", "Ulmus thomasii", "rock elm", 1, 12, 39, -1, -1, 3, -1, -1, -1, -1, 8, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set},
        {"UMCA", "Umbellularia californica", "California laurel", 1, 5, 39, -1, 2, -1, -1, -1, 6, -1, -1, -1, EquationType::crown_scorch, CrownDamageEquationCode::not_set}};

    record_ = tmp_records;
}

int SpeciesMasterTable::getSpeciesTableIndexFromSpeciesCode(string speciesCode) const
{
    int i;
    string speciesCodeTemp;
    speciesCodeTemp = speciesCode;
    std::transform(speciesCodeTemp.begin(), speciesCodeTemp.end(), speciesCodeTemp.begin(), ::toupper); // got to upper case

    for(i = 0; i < record_.size(); i++)
    {
        if(record_[i].speciesCode == "")
        {
            break;
        }
        if(record_[i].speciesCode == speciesCodeTemp)
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
        if(record_[i].speciesCode =="")
        {
            break;
        }
        if((record_[i].speciesCode == speciesCodeTemp) && (record_[i].equationType == equationType))
        {
            return i;
        }
    }

    return -1;
}

void SpeciesMasterTable::insertRecord (string speciesCode, string scientificName, string commonName,
                                       int  mortalityEquation, int  brkEqu, int  crownCoefficientCode,
                                       int8_t gacc1, int8_t  gacc2, int8_t  gacc3, int8_t gacc4,
                                       int8_t gacc5, int8_t  gacc6, int8_t  gacc7, int8_t gacc8,
                                       int8_t gacc9,
                                       EquationType equationType, CrownDamageEquationCode crownDamageEquationCode)
{
    SpeciesMasterTableRecord recordTemp;
    recordTemp.speciesCode = speciesCode;
    recordTemp.scientificName = scientificName;
    recordTemp.commonName = commonName;

    recordTemp.mortalityEquationNumber = mortalityEquation;
    recordTemp.barkEquationNumber = brkEqu;
    recordTemp.crownCoefficientCode = crownCoefficientCode;
    recordTemp.gaccAlaska = gacc1;
    recordTemp.gaccCalifornia = gacc2;
    recordTemp.gaccEasternArea = gacc3;
    recordTemp.gaccGreatBasin = gacc4;
    recordTemp.gaccNorthernRockies = gacc5;
    recordTemp.gaccNorthwest = gacc6;
    recordTemp.gaccRocketyMountain = gacc7;
    recordTemp.gaccSouthernArea = gacc8;
    recordTemp.gaccSouthWest = gacc9;
    recordTemp.equationType = equationType;
    recordTemp.crownDamageEquationCode = crownDamageEquationCode;
    record_.push_back(recordTemp);
}
