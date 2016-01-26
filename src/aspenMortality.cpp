#include "aspenMortality.h"
#include <cmath>

 AspenMortality::AspenMortality() // Default Constructor
 {

 }
 AspenMortality::~AspenMortality() // Default Destructor
 {

 }

//------------------------------------------------------------------------------
/*! \brief Calculates the aspen mortality rate.
 *
 *	Calculates the aspen mortality rate using the Brown and Debyle (1987) model
 *	Source: Brown, James K.; Debyle, Norbert V. 1987. Fire damage, mortality, and 
 *		suckering in aspen. Canadian Journal of Forest Research, 17: 1100-1109.
 *
 *  \param severity Fire severity level: 0 = low severity, 1= moderate+ severity
 *  \param flameLength: Flame length of the fire at the tree (ft).
 *  \param diameterBreastHeight: Aspen diameter at breast height (in).
 *
 *  \return Aspen mortality rate (fraction).
 */

double AspenMortality::CalculateAspenMortality( int severity, double flameLength, double diameterBreastHeight )
{
    double mortality = 1.;
    double ch = flameLength / 1.8;
    if ( severity == 0 )
    {
        mortality = 1. / ( 1. + exp( -4.407 + 0.638 * diameterBreastHeight - 2.134 * ch ) );
    }
    else if ( severity == 1 )
    {
        mortality = 1. / ( 1. + exp( -2.157 + 0.218 * diameterBreastHeight - 3.600 * ch ) );
    }
    mortality = ( mortality < 0.0 ) ? 0.0 : mortality;
    mortality = ( mortality > 1.0 ) ? 1.0 : mortality;
    return( mortality );
}
