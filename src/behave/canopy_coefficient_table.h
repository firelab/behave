/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: canopy_coefficient_table.h
* Desc: Canopy/Crown Cover Coefficient Table
* "Table 8. Coefficients for tree crown widths based on data from
*    R6 Permanent Plot Grid Inventory"
*  The above is the name on top of a document ER gave me.
*
*  How it's calculated
*   In SpeciesMasterTableRecord. a species Index No. is assigned to each
*    species.
*   The Index is then used basically as an equation number to go find
*     the proper coefficents.
*   > For a 6 char FOFEM species code we go thru the species master table
*   (stored in RAM) get Index No.
*   > find the match in the CCT coefficent table below,
*   > use the proper coefficients based on tree height, see
*     code and paper documentation for formulas (pretty simple formulas)
*   > the formula give a diameter of the crown
*   > area (square feet) is then calculation and accumulated for the stand
*   > another formula is used (see code) to converter total square feet
*     to a percent of cover for an acre, the fomula adjusts for overlap
*     It is done per acre because that is what user input of tree density
*      are entered as.
*   
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Crown Coefficients                                 */
/* The Crown Code is the FVS 2 char species code, which is what the Table 8  */
/*  document used.                                                           */

#ifndef CANOPY_COEFFICIENT_TABLE_H
#define CANOPY_COEFFICIENT_TABLE_H

#include <vector>

using std::vector;

struct CanopyCoefficientTableRecord
{
    int   indexNumber_;                                 /* Spe FVS Index No.            */
    char crownCode_[10];                             /* Crown Code,                  */
    double coefficientA_;                                  /* Large tree coefficients      */
    double coefficientB_;
    double coefficientR_;                                  /* Small tree coefficients      */
};

class CanopyCoefficientTable
{
public:
    CanopyCoefficientTable();
    vector<CanopyCoefficientTableRecord> record_;
};

#endif // CANOPY_COEFFICIENT_TABLE_H
