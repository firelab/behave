#include "surfaceFireWidth.h"

//------------------------------------------------------------------------------
	/*! \brief Calculates fire maximum width from its length and length-to-width
	 *  ratio.
	 *
	 *  \param fireLength       Fire length (ft).
	 *  \param lengthWidthRatio Fire length-to-width ratio (ft/ft).
	 *
	 *  \return Maximum fire width (ft).
*/
double SurfaceFireWidth::FBL_SurfaceFireWidth(double fireLength, double lengthWidthRatio)
{
    return( ( lengthWidthRatio < SMIDGEN )
          ? ( 0.0 )
          : ( fireLength / lengthWidthRatio ) );
}