#include "surfaceFirelineIntensity.h"

//------------------------------------------------------------------------------
/*! \brief FBL_SurfaceFireFirelineIntensity
	// in: 
	// parameter 1: double type, spread rate in ft/min
	// parameter 2: double tpye, reaction intensity in Btu/min/ft2
	// parameter 3: double type, residence time in min

	// out:
	// double type, surface fireline intensity in Btu/ft/s
*/
double SurfaceFireFirelineIntensity::FBL_SurfaceFireFirelineIntensity( double spreadRate,
		double reactionIntensity, double residenceTime )
{
	

    return( spreadRate * reactionIntensity * residenceTime / 60. );
}

//------------------------------------------------------------------------------
/*! \brief FireLineIntAtHead
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireLineIntAtHead (Btu/ft/s)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 *      vSurfaceFireResidenceTime (min)
 *      vSurfaceFireSpreadAtHead (ft/min)
 */
double SurfaceFireFirelineIntensity::FireLineIntensityAtHead(double vSurfaceFireResidenceTime, 
		double vSurfaceFireSpreadAtHead, double vSurfaceFireReactionIntensity)
{
    // Access current input values
    double ros = vSurfaceFireSpreadAtHead;
    double rxi = vSurfaceFireReactionIntensity;
	double tau = vSurfaceFireResidenceTime;
    // Calculate results
    double fli = FBL_SurfaceFireFirelineIntensity( ros, rxi, tau ) ;
    // Store results
    //vSurfaceFireLineIntAtHead->update( fli );
    // Log results
	/*
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireLineIntAtHead() 3 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireResidenceTime %g %s\n", Margin,
            vSurfaceFireResidenceTime->m_nativeValue,
            vSurfaceFireResidenceTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLineIntAtHead %g %s\n", Margin,
            vSurfaceFireLineIntAtHead->m_nativeValue,
            vSurfaceFireLineIntAtHead->m_nativeUnits.latin1() );
    }
	*/
    return fli;
}

void SurfaceFireFirelineIntensity::FireFlameLengAtVector( void )
{
	// This function may be redundant as surfaceFireFlameLength class can 
	// calculate flame length
	/*
	// Access current input values
    double firelineIntensity = vSurfaceFireLineIntAtVector->m_nativeValue;
    // Calculate results
    double fl = FBL_SurfaceFireFlameLength( firelineIntensity );
    // Store results
    vSurfaceFireFlameLengAtVector->update( fl );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireFlameLengAtVector() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLineIntAtVector %g %s\n", Margin,
            vSurfaceFireLineIntAtVector->m_nativeValue,
            vSurfaceFireLineIntAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
    }
	*/
	return;
}