#include "treeMortality.h"
#include <cmath>

TreeMortality::TreeMortality()
{
	FofemSpeciesStruct FofemSpeciesArray[NUM_SPECIES] =
	{
		/* 000 */{ "ABIAMA", 1, 26, 2, "Abies amabilis", "Pacific silver fir" },
		/* 001 */{ "ABIBAL", 1, 10, 134, "Abies balsamea", "Balsam fir" },
		/* 002 */{ "ABICON", 1, 27, 12, "Abies concolor", "White fir" },
		/* 003 */{ "ABIGRA", 1, 25, 12, "Abies grandis", "Grand fir" },
		/* 004 */{ "ABILAS", 1, 20, 12, "Abies lasiocarpa", "Subalpine fir" },
		/* 005 */{ "ABIMAG", 1, 18, 12, "Abies magnifica", "Red fir" },
		/* 006 */{ "ABIPRO", 1, 24, 2, "Abies procera", "Noble fir" },
		/* 007 */{ "ABISPP", 1, 30, 34, "Abies species", "Firs" },
		/* 008 */{ "ACEBAR", 1, 8, 4, "Acer barbatum", "Florida maple" },
		/* 009 */{ "ACELEU", 1, 8, 4, "Acer leucoderme", "Chalk maple" },
		/* 010 */{ "ACEMAC", 1, 3, 2, "Acer macrophyllum", "Bigleaf maple" },
		/* 011 */{ "ACENEG", 1, 13, 34, "Acer negundo", "Boxelder" },
		/* 012 */{ "ACENIG", 1, 14, 34, "Acer nigrum", "Black maple" },
		/* 013 */{ "ACEPEN", 1, 24, 34, "Acer pensylvanicum", "Striped maple" },
		/* 014 */{ "ACERUB", 1, 7, 34, "Acer rubrum", "Red maple" },
		/* 015 */{ "ACESACI", 1, 10, 34, "Acer saccharinum", "Silver maple" },
		/* 016 */{ "ACESACU", 1, 12, 34, "Acer saccharum", "Sugar maple" },
		/* 017 */{ "ACESPI", 1, 19, 3, "Acer spicatum", "Mountain maple" },
		/* 018 */{ "ACESPP", 1, 8, 34, "Acer species", "Maples" },
		/* 019 */{ "AESGLA", 1, 15, 34, "Aesculus glabra", "Ohio buckeye" },
		/* 020 */{ "AESOCT", 1, 29, 34, "Aesculus octandra", "Yellow buckeye" },
		/* 021 */{ "AILALT", 1, 29, 34, "Ailanthus altissima", "Ailanthus" },
		/* 022 */{ "ALNRHO", 1, 35, 2, "Alnus rhombifolia", "White alder" },
		/* 023 */{ "ALNRUB", 1, 5, 2, "Alnus rubra", "Red alder" },
		/* 024 */{ "AMEARB", 1, 29, 34, "Amelanchier arborea", "Serviceberry" },
		/* 025 */{ "ARBMEN", 1, 34, 2, "Arbutus menziesii", "Pacific madrone" },
		/* 026 */{ "BETALL", 1, 10, 34, "Betula alleghaniensis", "Yellow birch" },
		/* 027 */{ "BETLEN", 1, 9, 4, "Betula lenta", "Sweet birch" },
		/* 028 */{ "BETNIG", 1, 8, 34, "Betula nigra", "River birch" },
		/* 029 */{ "BETOCC", 1, 29, 34, "Betula occidentalis", "Water birch" },
		/* 030 */{ "BETPAP", 1, 6, 234, "Betula papyrifera", "Paper birch" },
		/* 031 */{ "BETSPP", 1, 12, 234, "Betula species ", "Birches" },
		/* 032 */{ "CELOCC", 1, 14, 34, "Celtis occidentalis", "Hackberry" },
		/* 033 */{ "CARAQU", 1, 19, 34, "Carya aquatica", "Water hickory" },
		/* 034 */{ "CARCAR", 1, 9, 34, "Carpinus caroliniana", "American hornbeam" },
		/* 035 */{ "CARCOR", 1, 16, 34, "Carya cordiformis", "Bitternut hickory" },
		/* 036 */{ "CARGLA", 1, 16, 34, "Carya glabra", "Pignut hickory" },
		/* 037 */{ "CARILL", 1, 15, 34, "Carya illinoensis", "Pecan" },
		/* 038 */{ "CARLAC", 1, 22, 34, "Carya laciniosa", "Shellbark hickory" },
		/* 039 */{ "CAROVA", 1, 19, 34, "Carya ovata", "Shagbark hickory" },
		/* 040 */{ "CARSPP", 1, 23, 34, "Carya species", "Hickories" },
		/* 041 */{ "CARTEX", 1, 19, 4, "Carya texana", "Black hickory" },
		/* 042 */{ "CARTOM", 1, 22, 34, "Carya tomentosa", "Mockernut hickory" },
		/* 043 */{ "CASCHR", 1, 24, 2, "Castanopsis chrysophylla", "Golden chinkapin" },
		/* 044 */{ "CASDEN", 1, 19, 3, "Castanea dentata", "American chestnut" },
		/* 045 */{ "CATSPP", 1, 16, 4, "Catalpa species", "Catalpas" },
		/* 046 */{ "CELLAE", 1, 15, 34, "Celtis laevigata", "Sugarberry" },
		/* 047 */{ "CERCAN", 1, 14, 34, "Cercis canadensis", "Eastern redbud" },
		/* 048 */{ "CHALAW", 1, 39, 2, "Chamaecyparis lawsoniana", "Port Orford cedar" },
		/* 049 */{ "CHANOO", 1, 2, 2, "Chamaecyparis nootkatenis", "Alaska cedar" },
		/* 050 */{ "CHATHY", 1, 4, 34, "Chamaecyparis thyoides", "Atlantic white cedar" },
		/* 051 */{ "CORFLO", 1, 20, 34, "Cornus florida", "Flowering dogwood" },
		/* 052 */{ "CORNUT", 1, 35, 2, "Cornus nuttallii", "Pacific dogwood" },
		/* 053 */{ "CORSPP", 1, 10, 34, "Cornus species", "Dogwoods" },
		/* 054 */{ "CRADOU", 1, 17, 4, "Crataegus douglasii", "Black hawthorn" },
		/* 055 */{ "CRASPPW", 1, 35, 2, "Crataegus species (western)", "Hawthorns (western)" },
		/* 056 */{ "CRASPPE", 1, 17, 34, "Crataegus species (eastern)", "Hawthorns (eastern)" },
		/* 057 */{ "DIOVIR", 1, 20, 34, "Diospyros virginiana", "Persimmon" },
		/* 058 */{ "FAGGRA", 1, 4, 34, "Fagus grandifolia", "American beech" },
		/* 059 */{ "FRAAMA", 1, 21, 34, "Fraxinus americana", "White ash" },
		/* 060 */{ "FRANIG", 1, 14, 34, "Fraxinus nigra", "Black ash" },
		/* 061 */{ "FRAPEN", 1, 18, 34, "Fraxinus pennsylvanica", "Green ash" },
		/* 062 */{ "FRAPRO", 1, 16, 34, "Fraxinus profunda", "Pumpkin ash" },
		/* 063 */{ "FRAQUA", 1, 9, 34, "Fraxinus quadrangulata", "Blue ash" },
		/* 064 */{ "FRASPP", 1, 21, 34, "Fraxinus species", "Ashes" },
		/* 065 */{ "GLETRI", 1, 17, 34, "Gleditsia triacanthos", "Honeylocust" },
		/* 066 */{ "GORLAS", 1, 17, 4, "Gordonia lasianthus", "Loblolly bay" },
		/* 067 */{ "GYMDIO", 1, 10, 34, "Gymnocladus dioicus", "Kentucky coffeetree" },
		/* 068 */{ "HALSPP", 1, 17, 4, "Halesia species", "Silverbells" },
		/* 069 */{ "ILEOPA", 1, 21, 34, "Ilex opaca", "American holly" },
		/* 070 */{ "JUGCIN", 1, 20, 34, "Juglans cinerea", "Butternut" },
		/* 071 */{ "JUGNIG", 1, 20, 34, "Juglans nigra", "Black walnut" },
		/* 072 */{ "JUNOCC", 1, 4, 2, "Juniperus occidentalis", "Western juniper" },
		/* 073 */{ "JUNSPP", 1, 12, 34, "Juniperus species", "Junipers/Redcedars" },
		/* 074 */{ "JUNVIR", 1, 17, 34, "Juniperus virginiana", "Eastern redcedar" },
		/* 075 */{ "LARLAR", 1, 10, 34, "Larix laricina", "Tamarack" },
		/* 076 */{ "LARLYA", 1, 29, 2, "Larix lyallii", "Subalpine larch" },
		/* 077 */{ "LAROCC", 1, 36, 12, "Larix occidentalis", "Western larch" },
		/* 078 */{ "LIBDEC", 1, 34, 2, "Libocedrus decurrens", "Incense cedar" },
		/* 079 */{ "LIQSTY", 1, 15, 34, "Liquidambar styraciflua", "Sweetgum" },
		/* 080 */{ "LIRTUL", 1, 20, 34, "Liriodendron tulipifera", "Yellow poplar" },
		/* 081 */{ "LITDEN", 1, 30, 2, "Lithocarpus densiflorus", "Tanoak" },
		/* 082 */{ "MACPOM", 1, 16, 4, "Maclura pomifera", "Osage orange" },
		/* 083 */{ "MAGACU", 1, 15, 34, "Magnolia acuminata", "Cucumbertree" },
		/* 084 */{ "MAGGRA", 1, 12, 4, "Magnolia grandiflora", "Southern magnolia" },
		/* 085 */{ "MAGMAC", 1, 12, 4, "Magnolia macrophylla", "Bigleaf magnolia" },
		/* 086 */{ "MAGSPP", 1, 18, 34, "Magnolia species", "Magnolias" },
		/* 087 */{ "MAGVIR", 1, 19, 34, "Magnolia virginiana", "Sweetbay" },
		/* 088 */{ "MALPRU", 1, 17, 4, "Malus/Prunus species", "Apples/Cherries" },
		/* 089 */{ "MALSPP", 1, 22, 34, "Malus species", "Apples" },
		/* 090 */{ "MORALB", 1, 17, 4, "Morus alba", "White mulberry" },
		/* 091 */{ "MORRUB", 1, 17, 4, "Morus rubra", "Red mulberry" },
		/* 092 */{ "MORSPP", 1, 12, 34, "Morus species", "Mulberries" },
		/* 093 */{ "NYSAQU", 1, 9, 4, "Nyssa aquatica", "Water tupelo" },
		/* 094 */{ "NYSOGE", 1, 17, 4, "Nyssa ogache", "Ogeechee tupelo" },
		/* 095 */{ "NYSSPP", 1, 4, 34, "Nyssa species", "Tupelos" },
		/* 096 */{ "NYSSYL", 1, 18, 34, "Nyssa sylvatica", "Black tupelo" },
		/* 097 */{ "NYSSYLB", 1, 16, 4, "Nyssa sylvatica var. biflora", "Swamp tupelo" },
		/* 098 */{ "OSTVIR", 1, 16, 34, "Ostrya virginiana", "Eastern hophornbeam" },
		/* 099 */{ "OXYARB", 1, 15, 34, "Oxydendrum arboreum", "Sourwood" },
		/* 100 */{ "PAUTOM", 1, 29, 34, "Paulownia tomentosa", "Paulownia" },
		/* 101 */{ "PERBOR", 1, 17, 4, "Persea borbonia", "Redbay" },
		/* 102 */{ "PICABI", 3, 8, 34, "Picea abies", "Norway spruce" },
		/* 103 */{ "PICENG", 3, 15, 12, "Picea engelmannii", "Engelmann spruce" },
		/* 104 */{ "PICGLA", 3, 4, 123, "Picea glauca", "White spruce" },
		/* 105 */{ "PICMAR", 3, 11, 234, "Picea mariana", "Black spruce" },
		/* 106 */{ "PICPUN", 3, 10, 1, "Picea pungens", "Blue spruce" },
		/* 107 */{ "PICRUB", 3, 13, 34, "Picea rubens", "Red spruce" },
		/* 108 */{ "PICSIT", 3, 6, 2, "Picea sitchensis", "Sitka spruce" },
		/* 109 */{ "PICSPP", 3, 13, 34, "Picea species", "Spruces" },
		/* 110 */{ "PINALB", 1, 9, 12, "Pinus albicaulis", "Whitebark pine" },
		/* 111 */{ "PINATT", 1, 9, 2, "Pinus attenuata", "Knobcone pine" },
		/* 112 */{ "PINBAN", 1, 19, 3, "Pinus banksiana", "Jack pine" },
		/* 113 */{ "PINCLA", 1, 14, 4, "Pinus clausa", "Sand pine" },
		/* 114 */{ "PINCON", 1, 7, 12, "Pinus contorta", "Lodgepole pine" },
		/* 115 */{ "PINECH", 1, 16, 34, "Pinus echinata", "Shortleaf pine" },
		/* 116 */{ "PINELL", 1, 31, 4, "Pinus elliottii", "Slash pine" },
		/* 117 */{ "PINFLE", 1, 9, 1, "Pinus flexilis", "Limber pine" },
		/* 118 */{ "PINGLA", 1, 14, 4, "Pinus glabra", "Spruce pine" },
		/* 119 */{ "PINJEF", 1, 37, 12, "Pinus jeffreyi", "Jeffrey pine" },
		/* 120 */{ "PINLAM", 1, 38, 12, "Pinus lambertiana", "Sugar pine" },
		/* 121 */{ "PINMON", 1, 14, 12, "Pinus monticola", "Western white pine" },
		/* 122 */{ "PINPAL", 1, 28, 4, "Pinus palustrus", "Longleaf pine" },
		/* 123 */{ "PINPON", 1, 36, 12, "Pinus ponderosa", "Ponderosa pine" },
		/* 124 */{ "PINPUN", 1, 19, 34, "Pinus pungens", "Table mountain pine" },
		/* 125 */{ "PINRES", 1, 22, 34, "Pinus resinosa", "Red pine" },
		/* 126 */{ "PINRIG", 1, 24, 34, "Pinus rigida", "Pitch pine" },
		/* 127 */{ "PINSAB", 1, 12, 2, "Pinus sabiniana", "Digger pine" },
		/* 128 */{ "PINSER", 1, 35, 34, "Pinus serotina", "Pond pine" },
		/* 129 */{ "PINSPP", 1, 9, 34, "Pinus species", "Pines" },
		/* 130 */{ "PINSTR", 1, 24, 34, "Pinus strobus", "Eastern white pine" },
		/* 131 */{ "PINSYL", 1, 9, 34, "Pinus sylvestris", "Scotch pine" },
		/* 132 */{ "PINTAE", 1, 30, 34, "Pinus taeda", "Loblolly pine" },
		/* 133 */{ "PINVIR", 1, 12, 34, "Pinus virginiana", "Virginia pine" },
		/* 134 */{ "PLAOCC", 1, 12, 34, "Plantus occidentalis", "American sycamore" },
		/* 135 */{ "POPBAL", 1, 19, 34, "Populus balsamifera", "Balsam poplar" },
		/* 136 */{ "POPDEL", 1, 19, 34, "Populus deltoides", "Eastern cottonwood" },
		/* 137 */{ "POPGRA", 1, 18, 34, "Populus grandidentata", "Bigtooth aspen" },
		/* 138 */{ "POPHET", 1, 29, 34, "Populus heterophylla", "Swamp cottonwood" },
		/* 139 */{ "POPSPP", 1, 17, 34, "Populus species", "Poplars" },
		/* 140 */{ "POPTRI", 1, 23, 2, "Populus trichocarpa", "Black cottonwood" },
		/* 141 */{ "PRUAME", 1, 19, 3, "Prunus americana", "American plum" },
		/* 142 */{ "PRUEMA", 1, 35, 2, "Prunus emarginata", "Bitter cherry" },
		/* 143 */{ "PRUDEN", 1, 24, 34, "Prunus pensylvanica", "Pin cherry" },
		/* 144 */{ "PRUSER", 1, 9, 34, "Prunus serotina", "Black cherry" },
		/* 145 */{ "PRUSPP", 1, 29, 34, "Prunus species", "Cherries" },
		/* 146 */{ "PRUVIR", 1, 19, 3, "Prunus virginiana", "Chokecherry" },
		/* 147 */{ "PSEMEN", 1, 36, 12, "Pseudotsuga menziesii", "Douglas-fir" },
		/* 148 */{ "QUEAGR", 1, 29, 2, "Quercus agrifolia", "Coast live oak" },
		/* 149 */{ "QUEALB", 1, 19, 34, "Quercus alba", "White oak" },
		/* 150 */{ "QUEBIC", 1, 24, 34, "Quercus bicolor", "Swamp white oak" },
		/* 151 */{ "QUECHR", 1, 3, 2, "Quercus chrysolepis", "Canyon live oak" },
		/* 152 */{ "QUEOCC", 1, 19, 34, "Quercus coccinea", "Scarlet oak" },
		/* 153 */{ "QUEDOU", 1, 12, 2, "Quercus douglasii", "Blue oak" },
		/* 154 */{ "QUEELL", 1, 17, 34, "Quercus ellipsoidalis", "Northern pin oak" },
		/* 155 */{ "QUEENG", 1, 33, 2, "Quercus engelmannii", "Engelmann oak" },
		/* 156 */{ "QUEFAL", 1, 23, 34, "Quercus falcata", "Southern red oak" },
		/* 157 */{ "QUEGAR", 1, 8, 2, "Quercus garryana", "Oregon white oak" },
		/* 158 */{ "QUEIMB", 1, 20, 34, "Quercus imbricaria", "Shingle oak" },
		/* 159 */{ "QUEINC", 1, 17, 4, "Quercus incana", "Bluejack oak" },
		/* 160 */{ "QUEKEL", 1, 9, 2, "Quercus kellogii", "Califonia black oak" },
		/* 161 */{ "QUELAE", 1, 16, 4, "Quercus laevis", "Turkey oak" },
		/* 162 */{ "QUELAU", 1, 15, 4, "Quercus laurifolia", "Laurel oak" },
		/* 163 */{ "QUELOB", 1, 22, 2, "Quercus lobata", "California white oak" },
		/* 164 */{ "QUELYR", 1, 18, 34, "Quercus lyrata", "Overcup oak" },
		/* 165 */{ "QUEMAC", 1, 21, 34, "Quercus macrocarpa", "Bur oak" },
		/* 166 */{ "QUEMAR", 1, 16, 34, "Quercus marilandica", "Blackjack oak" },
		/* 167 */{ "QUEMIC", 1, 25, 34, "Quercus michauxii", "Swamp chestnut oak" },
		/* 168 */{ "QUEMUE", 1, 21, 34, "Quercus muehlenbergii", "Chinkapin oak" },
		/* 169 */{ "QUENIG", 1, 15, 34, "Quercus nigra", "Water oak" },
		/* 170 */{ "QUENUT", 1, 9, 4, "Quercus nuttallii", "Nuttall oak" },
		/* 171 */{ "QUEPAL", 1, 20, 34, "Quercus palustris", "Pin oak" },
		/* 172 */{ "QUEPHE", 1, 20, 34, "Quercus phellos", "Willow oak" },
		/* 173 */{ "QUEPRI", 1, 28, 34, "Quercus prinus", "Chestnut oak" },
		/* 174 */{ "QUERUB", 1, 21, 34, "Quercus rubra", "Northern red oak" },
		/* 175 */{ "QUESHU", 1, 16, 34, "Quercus shumardii", "Shumard oak" },
		/* 176 */{ "QUESPP", 1, 24, 34, "Quercus species", "Oaks" },
		/* 177 */{ "QUESTE", 1, 23, 34, "Quercus stellata", "Post oak" },
		/* 178 */{ "QUEVEL", 1, 24, 34, "Quercus velutina", "Black oak" },
		/* 179 */{ "QUEVIR", 1, 22, 4, "Quercus virginiana", "Live oak" },
		/* 180 */{ "QUEWIS", 1, 13, 2, "Quercus wislizenii", "Interior live oak" },
		/* 181 */{ "ROBPSE", 1, 28, 34, "Robinia pseudoacacia", "Black locust" },
		/* 182 */{ "SALDIA", 1, 19, 3, "Salix bebbiana", "Diamond willow" },
		/* 183 */{ "SALNIG", 1, 19, 34, "Salix nigra", "Black willow" },
		/* 184 */{ "SALSPP", 1, 20, 234, "Salix species", "Willows" },
		/* 185 */{ "SASALB", 1, 14, 34, "Sassafras albidum", "Sassafras" },
		/* 186 */{ "SEQGIG", 1, 39, 2, "Sequoiadendron giganteum", "Giant sequoia" },
		/* 187 */{ "SEQSEM", 1, 39, 2, "Sequoia sempervirens", "Redwood" },
		/* 188 */{ "SORAME", 1, 19, 3, "Sorbus americana", "American mountain ash" },
		/* 189 */{ "TAXBRE", 1, 4, 12, "Taxus brevifolia", "Pacific yew" },
		/* 190 */{ "TAXDIS", 1, 4, 34, "Taxodium distichum", "Baldcypress" },
		/* 191 */{ "TAXDISN", 1, 21, 4, "Taxodium distictum var. nutans", "Pondcypress" },
		/* 192 */{ "THUOCC", 1, 4, 34, "Thuja occidentalis", "Northern white cedar" },
		/* 193 */{ "THUPLI", 1, 14, 12, "Thuja plicata", "Western redcedar" },
		/* 194 */{ "THUSPP", 1, 12, 34, "Thuju species", "Arborvitae" },
		/* 195 */{ "TILAME", 1, 17, 34, "Tilia americana", "American basswood" },
		/* 196 */{ "TILHET", 1, 29, 34, "Tilia heterophylla", "White basswood" },
		/* 197 */{ "TSUCAN", 1, 18, 34, "Tsuga canadensis", "Eastern hemlock" },
		/* 198 */{ "TSUHET", 1, 19, 12, "Tsuga heterophylla", "Western hemlock" },
		/* 199 */{ "TSUMER", 1, 19, 12, "Tsuga mertensiana", "Mountain hemlock" },
		/* 200 */{ "ULMALA", 1, 10, 34, "Ulmus alata", "Winged elm" },
		/* 201 */{ "ULMAME", 1, 10, 34, "Ulmus americana", "American elm" },
		/* 202 */{ "ULMPUM", 1, 17, 34, "Ulmus pumila", "Siberian elm" },
		/* 203 */{ "ULMRUB", 1, 11, 34, "Ulmus rubra", "Slippery elm" },
		/* 204 */{ "ULMSPP", 1, 18, 34, "Ulmus species", "Elms" },
		/* 205 */{ "ULMTHO", 1, 12, 34, "Ulmus thomasii", "Rock elm" },
		/* 206 */{ "UMBCAL", 1, 5, 2, "Umbellularia californica", "California laurel" },
		/* 209 */{ NULL, 0, 0, 0, NULL, NULL }
	};

	double Fofem_Sbt[40] =
	{
		/* 00 */    0.000,      // Not used
		/* 01 */    0.019,      // Not used
		/* 02 */    0.022,
		/* 03 */    0.024,
		/* 04 */    0.025,
		/* 05 */    0.026,
		/* 06 */    0.027,
		/* 07 */    0.028,
		/* 08 */    0.029,
		/* 09 */    0.030,
		/* 10 */    0.031,
		/* 11 */    0.032,
		/* 12 */    0.033,
		/* 13 */    0.034,
		/* 14 */    0.035,
		/* 15 */    0.036,
		/* 16 */    0.037,
		/* 17 */    0.038,
		/* 18 */    0.039,
		/* 19 */    0.040,
		/* 20 */    0.041,
		/* 21 */    0.042,
		/* 22 */    0.043,
		/* 23 */    0.044,
		/* 24 */    0.045,
		/* 25 */    0.046,
		/* 26 */    0.047,
		/* 27 */    0.048,
		/* 28 */    0.049,
		/* 29 */    0.050,
		/* 30 */    0.052,
		/* 31 */    0.055,
		/* 32 */    0.057,      // Not used
		/* 33 */    0.059,
		/* 34 */    0.060,
		/* 35 */    0.062,
		/* 36 */    0.065,
		/* 37 */    0.068,
		/* 38 */    0.072,
		/* 39 */    0.081
	};
}

//------------------------------------------------------------------------------
/*! \brief Calculates scorch height from fireline intensity, wind speed, and
 *  air temperature.
 *
 *  \param firelineIntensity Fireline (Byram's) intensity (btu/ft/s).
 *  \param windSpeed         Wind speed at midlame height (m/h).
 *  \param airTemperature    Air temperature (oF).
 *
 *  \return Scorch height (ft).
 */
double TreeMortality::CalculateSurfaceFireScorchHeight(double firelineIntensity, double windSpeed,
    double airTemperature )
{
	
    return( ( firelineIntensity < SMIDGEN )
          ? ( 0.0 )
          : ( ( 63. / ( 140. - airTemperature ) )
            * pow( firelineIntensity, 1.166667 )
            / sqrt( firelineIntensity + ( windSpeed * windSpeed * windSpeed ) )
            ) );
}

//------------------------------------------------------------------------------
/*! \brief Calculates tree bark thickness using the FOFEM 5.0 equations.
 *
 *  \param speciesIndex Index into the FofemSpecies[] array.  This value is
 *  returned by a call to FBL_FofemSpeciesIndex().
 *  \param dbh Tree diameter at breast height (in).
 *
 *  \return Tree bark thickness (in).
 */
double TreeMortality::FBL_TreeBarkThicknessFofem( int speciesIndex, double dbh )
{
	

	// Fofem factors for determining Single Bark Thickness.
	// Each FOFEM species has a SBT equation index "barkEq" [1-39] into this array.
	return(Fofem_Sbt[FofemSpeciesArray[speciesIndex].barkEq] * dbh);
}

//------------------------------------------------------------------------------
/*! \brief Calculates tree crown volume scorched, tree length scorched,
 *  and tree crown length fraction scorched.
 *
 *  \param treeHt Tree height (ft).
 *  \param crownRatio Tree crown ratio (ft/ft).
 *  \param scorchHt Crown scorch height (ft).
 *  \param crownLengthScorched Address where the calculated crown length (ft)
 *  is stored.  If NULL or 0, no value is returned.
 *  \param crownLengthFractionScorched Address where the calculated crown
 *  length fraction (ft/ft) is stored.  If NULL or 0, no value is returned.
 *
 *  \return Fraction of the crown volume that is scorched (ft3/ft3).
 */
double TreeMortality::FBL_TreeCrownScorch(double treeHt, double crownRatio, double scorchHt, 
		double *crownLengthScorched, double *crownLengthFractionScorched)
{
    double scorchFrac, scorchVol;
    // Tree crown length (ft) and base height (ft)
    double crownLeng = treeHt * crownRatio;
    double baseHt    = treeHt - crownLeng;
    // Tree crown length scorched (ft)
    double scorchLeng = ( scorchHt <= baseHt )
                        ? ( 0.0 )
                        : ( scorchHt - baseHt );
    scorchLeng = ( scorchLeng > crownLeng )
                 ? ( crownLeng )
                 : ( scorchLeng );
    // Fraction of the crown length scorched (ft/ft)
    if ( crownLeng < SMIDGEN )
    {
        scorchFrac = ( scorchLeng > 0. )
                     ? ( 1.0 )
                     : ( 0.0 );
    }
    else
    {
        scorchFrac = scorchLeng / crownLeng;
    }
    // Fraction of the crown volume scorched (ft3/ft3)
    scorchVol = ( crownLeng < SMIDGEN )
                ? ( 0.0 )
                : ( scorchLeng * ( 2. * crownLeng - scorchLeng )
                    / ( crownLeng * crownLeng ) );
    // Store results in parameter addresses
    if ( crownLengthScorched )
    {
        *crownLengthScorched = scorchLeng;
    }
    if ( crownLengthFractionScorched )
    {
        *crownLengthFractionScorched = scorchFrac;
    }
    return( scorchVol );
}

//------------------------------------------------------------------------------
/*! \brief Calculates probability of tree mortality using the FOFEM 5.0
 *  equations for trees with dbh >= 1.
 *
 *  This is only a partial implementation of the FOFEM mortality algorithm.
 *  Specifically, it only implements those cases where the tree dbh >= 1".
 *  It also excludes the FOFEM special case of \e Populus \e tremuloides,
 *  which requires additional inputs (namely, flame height and fire severity).
 *
 *	Uses the Hood (2008) mortality equations
 *	Source: Hood, S.; Smith, S.; Cluck, D.; Reinhardt, E.; Ryan, K. 2008. Delayed 
 *	Tree Mortality following Fire in Western Conifers. JFSP Final Report 
 *	05-2-1-105, U.S. Department of Agriculture, Forest Service, Rocky Mountain 
 *	Research Station. 35 p.
 *
 *  \param speciesIndex Index into the FofemSpecies[] array.  This value is
 *  returned by a call to FBL_FofemSpeciesIndex().
 *  \param barkThickness Tree bark thickness (in).
 *  \param crownVolScorched Fraction of the crown volume that is scorched
 *  (ft3/ft3).
 *  \param scorchHt Scorch ht (ft)
 *	
 *  \return Tree mortality probability [0..1].
 */
double TreeMortality::FBL_TreeMortalityFofem(int speciesIndex, double barkThickness,
		double crownVolScorched, double scorchHt)
{
	// Pat requested that if scorch ht is zero, then mortality is zero
    if ( scorchHt < SMIDGEN )
    {
        return( 0.0 );
    }
    // FOFEM equation 1 for dbh > 1"
    double mortality = -1.941
              + 6.316 * ( 1.0 - exp( -barkThickness ) )
              - 5.35 * crownVolScorched * crownVolScorched;
    mortality = 1.0 / ( 1.0 + exp( mortality ) );
    // Adjustment for Picea (FOFEM equation 3).
	if (FofemSpeciesArray[speciesIndex].mortEq == 3 && mortality < 0.8)
    {
        mortality = 0.8;
    }
    // Confine results to range [0..1].
    if ( mortality > 1.0 )
    {
        mortality = 1.0;
    }
    if ( mortality < 0.0 )
    {
        mortality = 0.0;
    }
    return( mortality );
}

//------------------------------------------------------------------------------
/*! \brief Calculates probability of tree mortality using the Hood 2008 equations.
 *
 *  \param speciesIndex Index into the HoodSpecies[] array [0..9].
 *  \param dbh Tree diameter-at-breast-height (cm).
 *  \param crownLengScorched Fraction of the crown length that is scorched (ft2/ft2).
 *  \param crownVolScorched Fraction of the crown volume that is scorched
 *  (ft3/ft3).
 *
 *  \return Tree mortality probability [0..1].
*/
double TreeMortality::FBL_TreeMortalityHood(int speciesIndex, double dbh, 
	double crownLengScorched, double crownVolScorched )
{
	double mortality  = 0.00;
    // Convert to percents
    double cls = 100. * crownLengScorched;
    double cvs = 100. * crownVolScorched;
	bool zero = false;
    // White fir: crown LENGTH scorched only
    if ( speciesIndex == 0 )
    {
        mortality = -3.5083
           + ( 0.0956 * cls )
           - ( 0.00184 * cls * cls )
           + ( 0.000017 * cls * cls * cls );
		zero = ( cls < SMIDGEN );
    }
    // Subaline and grand fir: crown VOLUME scorched only
    else if ( speciesIndex == 1 )
    {
        mortality = -1.6950
           + ( 0.2071 * cvs )
           - ( 0.0047 * cvs * cvs )
           + ( 0.000035 * cvs * cvs * cvs );
  		zero = ( cvs < SMIDGEN );
	}
    // Red fir: crown LENGTH scorched only
    else if ( speciesIndex == 2 )
    {
        mortality = -2.3085 + 0.000004059 * cls * cls * cls;
 		zero = ( cls < SMIDGEN );
	}
    // Incense cedar: crown LENGTH scorched only
    else if ( speciesIndex == 3 )
    {
        mortality = -4.2466 + 0.000007172 * cls * cls * cls;
 		zero = ( cls < SMIDGEN );
	}
    // Western larch: crown VOLUME scorched and DBH
    else if ( speciesIndex == 4 )
    {
        mortality = -1.6594
           + ( 0.0327 * cvs )
           - ( 0.0489 * dbh );
 		zero = ( cvs < SMIDGEN );
	}
    // Whitebark and lodgepole pine: crown VOLUME scorched and DBH
    else if ( speciesIndex == 5 )
    {
        mortality = -0.3268
           + ( 0.1387 * cvs )
           - ( 0.0033 * cvs * cvs )
           + ( 0.000025 * cvs * cvs * cvs )
           - ( 0.0266 * dbh );
 		zero = ( cvs < SMIDGEN );
	}
    // Engelmann spruce: crown VOLUME scorch only
    else if ( speciesIndex == 6 )
    {
        mortality = 0.0845 + ( 0.0445 * cvs );
		zero = ( cvs < SMIDGEN );
    }
    // Sugar pine: crown LENGTH scorch only
    else if ( speciesIndex == 7 )
    {
        mortality = -2.0588 + ( 0.000815 * cls * cls );
		zero = ( cls < SMIDGEN );
    }
    // Ponderosa and Jeffrey pine: crown VOLUME scorch only
    else if ( speciesIndex == 8 )
    {
        mortality = -2.7103 + ( 0.000004093 * cvs * cvs * cvs );
  		zero = ( cvs < SMIDGEN );
	}
    // Douglas-fir: crown VOLUME scorched
    else if ( speciesIndex == 9 )
    {
        mortality = -2.0346
           + ( 0.0906 * cvs )
           - ( 0.0022 * cvs * cvs )
           + ( 0.000019 * cvs * cvs * cvs );
  		zero = ( cvs < SMIDGEN );
	}
    // UNKNOWN SPECIES INDEX
    else
    {
        //fprintf( stderr, "FBL_TreeMortalityHood(): speciesIndex %d is outside valid range 0..9.",
        //    speciesIndex );
        return( 0.0 );
    }
	if ( zero )
	{
		return 0.;
	}
    // All Hood's equations use this form so we do them all here:
    mortality = 1. / ( 1. + exp( -mortality ) );
    // Confine results to range [0..1].
	mortality = ( mortality > 1.0 ) ? 1. : mortality;
	mortality = ( mortality < 0.0 ) ? 0. : mortality;
    return( mortality );
}

//------------------------------------------------------------------------------
/*! \brief FireScorchHtFromFlameLengAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireScorchHtatVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vSurfaceFireFlameLengAtVector (ft)
 *      vWindSpeedAtMidflame (mi/h)
 *
 *  Intermediate Variables
 *      bi - Byram's fireline intensity (btu/ft/s)
*/
double TreeMortality::FireScorchHtFromFlameLengAtVector(double airTemp, double windSpeed, double flameLength, 
		double firelineIntensity)
{
	
    // Access current input values
    //double airTemp     = vWthrAirTemp->m_nativeValue;
    //double windSpeed   = vWindSpeedAtMidflame->m_nativeValue;
    //double flameLength = vSurfaceFireFlameLengAtVector->m_nativeValue;
    // Calculate results
    // double firelineIntensity = FBL_SurfaceFireFirelineIntensity( flameLength );
	double scorchHt = CalculateSurfaceFireScorchHeight(firelineIntensity,
		windSpeed, airTemp );
    // Store results
    //vSurfaceFireScorchHtAtVector->update( scorchHt );
    // Log results
	/*
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireScorchHtAtVector() 3 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
    }
	*/
    return scorchHt;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityRateFofemHoodAtVector
 *
 *  Combines the FOFEM and Hood models in a single entry point.
 *
 *  Dependent Variables (Outputs)
 *      vTreeMortalityRateAtVector (fraction)
 *	
 *  Independent Variables (Inputs)
 *      vTreeBarkThickness (in)
 *      vTreeCrownLengScorchedAtVector (fraction)
 *      vTreeCrownVolScorchedAtVector (fraction)
 *      vTreeSpecies (index)
 *      vSurfaceFireScorchHtAtVector
*/
double TreeMortality::TreeMortalityRateFofemHoodAtVector( int speciesIndex, double dbh, 
		double crownLengFractionScorched, double crownVolScorched, double barkThickness, 
		double surfaceFireScorchHt)
{
	// Access current input values
	/*
    int    spp = vTreeSpecies->activeItemDataIndex();			// index 
    double dbh = vTreeDbh->m_nativeValue;						// inches 
	double cls = vTreeCrownLengFractionScorchedAtVector->m_nativeValue;	// fraction
    double cvs = vTreeCrownVolScorchedAtVector->m_nativeValue;	// fraction
    double brk = vTreeBarkThickness->m_nativeValue;				// inches
    double sht = vSurfaceFireScorchHtAtVector->m_nativeValue;	// ft
	*/
	double mortalityRate  = 0.;

    // Determine whether to use FOFEM or Hood based on species index
    int hood = -1;
    if ( speciesIndex == 2 ) hood = 0;									// ABICON
    else if ( speciesIndex == 3 || speciesIndex == 4 ) hood = 1;		// ABIGRA & ABILAS
    else if ( speciesIndex == 5 ) hood =2;								// ABIMAG
    else if ( speciesIndex == 78 ) hood = 3;							// LIBDEC
    else if ( speciesIndex == 77) hood = 4;								// LAROCC
    else if ( speciesIndex == 110 || speciesIndex == 114 ) hood = 5;	// PINALB & PINCON
    else if ( speciesIndex == 103 ) hood = 6;							// PICENG
    else if ( speciesIndex == 120 ) hood = 7;							// PINLAM
    else if ( speciesIndex == 123 || speciesIndex == 119 ) hood = 8;	// PINPON & PINJEF
    else if ( speciesIndex == 147 ) hood = 9;							// PSEMEN

    // If using Hood ...
    if ( hood >= 0 )
    {
        mortalityRate  = FBL_TreeMortalityHood( hood, ( 2.54 * dbh ), crownLengFractionScorched, crownVolScorched );
    }
    // ... otherwise using FOFEM
    else
    {
        mortalityRate  = FBL_TreeMortalityFofem( speciesIndex, barkThickness, crownVolScorched, surfaceFireScorchHt );
    }
    // Store results
	/*
    vTreeMortalityRateAtVector->update( mortalityRate  );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityRateFofemHoodAtVector() 6 1\n", Margin );
        fprintf( m_log, "%s  i vTreeSpecies %d %s\n", Margin,
            vTreeSpecies->activeItemDataIndex(),
            vTreeSpecies->activeItemName().latin1() );
        fprintf( m_log, "%s  i vTreeDbh %g %s\n", Margin,
            vTreeDbh->m_nativeValue,
            vTreeDbh->m_nativeUnits.latin1() );
		fprintf( m_log, "%s  i vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownVolScorchedAtVector %g %s\n", Margin,
            vTreeCrownVolScorchedAtVector->m_nativeValue,
            vTreeCrownVolScorchedAtVector->m_nativeUnits.latin1() );
		fprintf( m_log, "%s  i vTreeCrownLengFractionScorchedAtVector %g %s\n", Margin,
            vTreeCrownLengFractionScorchedAtVector->m_nativeValue,
            vTreeCrownLengFractionScorchedAtVector->m_nativeUnits.latin1() );
 		fprintf( m_log, "%s  i vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityRateAtVector %g %s\n", Margin,
            vTreeMortalityRateAtVector->m_nativeValue,
            vTreeMortalityRateAtVector->m_nativeUnits.latin1() );
    }
	*/
    return mortalityRate ;
}

