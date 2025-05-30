/*!	\file SemFireCrownFirebrandProcessor.cpp
	\brief CrownFirebrandProcessor class definition
*/

#include <math.h>
#include "CrownFirebrandProcessor.h"

//------------------------------------------------------------------------------
/*!	\brief Custom constructor
	\param canopyHt Mean canopy height at the top of the canopy of an active crown fire (m).
	\param fireInt Active crown fireline intensity (kW/m).
	\param windSpeed Mean wind speed at the top of the canopy of an active crown fire (km/h).
	\param windHt Height at which the wind speed measurement was taken (m).
	\param emberDiam Diameter of a viable firebrand ember when alighting at the fuel surface (mm).
	\param deltaStep Plume layer slice size (default = 0.2 m)
*/
CrownFirebrandProcessor::CrownFirebrandProcessor(
		double canopyHt,
		double fireInt,
		double windSpeed,
		double windHt,
		double emberDiam,
		double deltaStep ) :
	m_isDirty( true ),
	// Inputs
	m_canopyHt( canopyHt ),		// Mean canopy height of active crown fire (m)
	m_deltaStep( deltaStep ),	// Plume layer step size (m)
	m_emberDiam( emberDiam ),	// Diameter of a viable firebrand ember when it alights at the fuel surface (mm)
	m_fireInt( fireInt ),		// Mean crown fire intensity (kW/m)
	m_windHt( windHt ),			// Height at which the wind measurement was taken (m)
	m_windSpeed( windSpeed ),	// Mean windspeed at the top of the canopy (km/h)
	// Primary output properties
	m_canopyWind( 0. ),		// Mean wind speed at the top of the canopy (m/s).
	m_flameHt( 0. ),		// Mean flame height above the canopy top (m)
	m_loftHt( 0. ),			// Maximum firebrand loft height (m)
	m_spotDist( 0. ),		// Maximum firebrand spotting distance (m)
	// Intermediate output properties
	m_ang( 0. ),			// Angle of the plume centerline aove horizontal
	m_bf( 0. ),				// Plume width normal to the plume centerline
	m_driftX( 0. ),			// Normalized downwind drift distance (m / m_canopyHt)
	m_eta( 1. ),			// Safety factor for the ember diameter on impact ( m_eta > 1. )
	m_fm( 0. ),				// Mass flux in plume flow per unit length of fire edge
	m_hf( 0. ),				// Normalized flame height above crown top (i.e., mflameHt / m_canopyHt)
	m_layer( 0 ),			// Plume layer (base 0) into which the ember is lofted
	m_loftX( 0. ),			// Normalized coordinate firebrand loft height (m / m_canopyHt)
	m_loftZ( 0. ),			// Normalized coordinate firebrand loft height (m / m_canopyHt)
	m_qfac( 0. ),			// Subroutine drop(), constant for a given m_uc
	m_qreq( 0. ),			// Required q (normalized dynamic pressure of vertical flow component) for the m_emberDiam
	m_rhof( 0. ),			// Mean mass density of the plume flow
	m_uc( 0. ),				// Normalized wind speed at the crown top (i.e., m_canopyWind / m_wn );
	m_uf( 0. ),				// Horizontal component of Favre avereage plume flow velocity
	m_vf( 0. ),				// Favre avereage plume flow velocity
	m_wf( 0. ),				// Vertical component of Favre avereage plume flow velocity
	m_wn( 0. )				// Factor used to adjust wind speed to canopy (i.e., log( g * m_canopyHt ) )
{
}

//------------------------------------------------------------------------------
void CrownFirebrandProcessor::process()
{
	reset();
	// Fireline intensity must be > 1000 kW/m, and there must be canopy and wind
	if ( m_canopyHt > 0.1 && m_windSpeed > 0.1 && m_fireInt >= 1000. )
	{
		processCanopyWindSpeed();
		processFlameHeight();
		processFlameBoundaryConditions();
		processFirebrandLoft();
		processFirebrandDrift();
	}
}

//------------------------------------------------------------------------------
/*!	\brief Adjusts the input wind speed measured at the input anemometer height
	to the wind speed (m/s) at the top of the canopy.

	This is from Frank Albini's 'dist.for' FORTRAN source listing:
	zonh = anem / htop
	fact = 1. + alog( 1. + 2.94 * fact )
	utop = uan / ( 3.6 * fact )
*/
void CrownFirebrandProcessor::processCanopyWindSpeed()
{
	double factor = 1.;
	if ( m_canopyHt > 0.1 )
	{
		factor = 1. + log( 1. + 2.94 * ( m_windHt / m_canopyHt ) );
	}
	// Apply factor and convert to m/s
	m_canopyWind = m_windSpeed / ( 3.6 * factor );
}

//------------------------------------------------------------------------------
/*!	\brief Determines the drift distance of the lofted firebrand.

	This is from Frank Albini's 'drift.for' FORTRAN source, SUBROUTINE DRIFT()
*/
void CrownFirebrandProcessor::processFirebrandDrift()
{
	double f0 = 1. + 2.94 * m_loftZ;
	double f1 = sqrt( m_eta / ( m_eta + m_loftZ ) );
	double f2 = sqrt( m_eta / ( m_eta - 0.34 ) );
	double f3 = f2 / f1;
	double f2log = log( ( f2 + 1. ) / ( f2 - 1. ) );
	double f3log = log( ( f3 + 1. ) / ( f3 - 1. ) );
	double f = 1. + log( f0 ) - f1 + ( f3log - f2log ) / f3;
	m_driftX = 10.9 * f * m_uc * sqrt( m_loftZ + m_eta );
	m_spotDist = m_canopyHt * ( m_loftX + m_driftX );
}

//------------------------------------------------------------------------------
/*!	\brief Determines the height and drift of the lofted firebrand.

	This is from Frank Albini's 'prof.for' FORTRAN source SUBROUTINE PROF()
	and file 'drop.for' SUBROUTINE DROP().
*/
void CrownFirebrandProcessor::processFirebrandLoft()
{
	// eta = safety factor for the ember diameter on impact ( m_eta > 1. )
	double dlosmm = 0.064 * m_canopyHt;
	double dtopmm = m_emberDiam + dlosmm;
	m_eta = dtopmm / dlosmm;

	// SUBROUTINE PROF() setup (these are all undocumented variables)
	double zc2 = m_hf;
	double xc2 = m_hf / tan( m_ang );
	double fmf = m_fm;	// need this??
	double fmuf = m_fm * m_uf;
	double fmadd = 0.2735 * m_fm;
	double hfarg = 1. + 2.94 * m_hf;
	double fmuadd = 0.3765 * ( m_hf + hfarg * log( hfarg) * log( hfarg ) );
	double dmwfac = 2. * fmf / ( 3. * m_uc * m_uc );
	double tratf = 2. * fmf / 3.;

	// These get updated for each plume layer
	double sing = sin( m_ang );
	double cosg = cos( m_ang );
	double delx = 0.5 * m_bf * sing;
	double delz = 0.5 * m_bf * cosg;
	double x = xc2;
	double z = m_hf;
	double v = m_vf;
	double w = m_wf;
	double fmw = m_fm * m_wf;

	// xb, zb = normalized coordinates of the plume base streamline
	// ... initiazed for the lower boundary layer (Albini's index==1)
	double xbLower = delx;
	double zbLower = 0.;
	// .. initialized for the top of the first plume layer (Alini's index==2)
	double xbUpper = xc2 + delx;
	double zbUpper = zc2 - delz;

	// q = normalized dynamic pressure of vertical flow component
	double qLower = 0.5 * m_rhof * m_wf * m_wf ;
	double qUpper = qLower;

	// Find highest plume layer that supports m_qreq
	m_layer = 1;
	while( true )
	{
		// If the upper boundary of this plume layer cannot support the ember
		m_qreq = m_qfac * ( zbUpper + m_eta );
		if ( qUpper < m_qreq )
		{
			m_loftZ = zbLower;
			m_loftX = xbLower;
			m_loftHt = m_canopyHt * m_loftZ;
			return;
		}
		// Otherwise, try the next level up
		m_layer++;
		double dx = m_deltaStep * cosg;
		double dz = m_deltaStep * sing;
		x += dx;
		z += dz;
		double zarg = 1. + 2.94 * z;
		double fm = 0.34 * zarg * log( zarg ) + fmadd;
		double fmu = z + ( 0.34 * zarg * log( zarg ) * log( zarg ) ) + fmuadd;
		double u = fmu / fm;
		fmw += ( dmwfac / v ) * dz;
		w = fmw / fm;
		v = sqrt( u * u + w * w );
		double trat = 1. + tratf / fm;
		double b = fm * trat / v;
		sing = w / v;
		cosg = u / v;
		delx = 0.5 * b * sing;
		delz = 0.5 * b * cosg;
		qLower  = qUpper;
		qUpper  = 0.5 * w * w / trat;
		xbLower = xbUpper;
		xbUpper = x + delx;
		zbLower = zbUpper;
		zbUpper = z - delz;
		// Make sure we don't get over 10km high!
		if ( ( m_layer * m_deltaStep ) > 10000. )
		{
			return;
		}
	}
}

//------------------------------------------------------------------------------
/*!	\brief Calculates the average flame height above the canopy top (m)

	This is from Frank Albini's 'tip.for' FORTRAN source, function tip().
*/
void CrownFirebrandProcessor::processFlameBoundaryConditions()
{
	// m_hf = normalized flame height above crown top
	m_hf = m_flameHt / m_canopyHt;

	// m_uc = normalized wind speed at the crown top
	m_wn = sqrt( 9.82 * m_canopyHt );
	m_uc = m_canopyWind / m_wn;

	// m_qfac is from subroutine drop()
	m_qfac = 0.00838 / ( m_uc * m_uc );

	double rfc = 1. + 2.94 * m_hf;
	double logRfc = log( rfc );

	// m_fm = mass flux in plume flow per unit length of fire edge
	m_fm = 0.468 * rfc * logRfc;

	// m_uf = horizontal component of Favre average plume flow velocity
	double fmuf = 1.3765 * ( m_hf + rfc * logRfc * logRfc );
	m_uf = fmuf / m_fm;

	// m_ang = angle of centerline of plume above horizontal
	double ctn2f = rfc - 1. + rfc * logRfc * logRfc;
	double tang = 1.40 * m_hf / ( m_uc * sqrt( ctn2f ) );
	m_ang = atan( tang );

	// m_wf = vertical component of Favre average plume flow velocity
	m_wf = tang * m_uf;

	// vf = Favre average plume flow velocity
	m_vf = sqrt( m_uf * m_uf + m_wf * m_wf );

	// m_rhof = mean mass density of plume flow
	m_rhof = 0.6;

	// m_bf = plume width normal to centerline
	m_bf = m_fm / ( m_rhof * m_vf );
}

//------------------------------------------------------------------------------
/*!	\brief Calculates the average flame height above the canopy top (m)

	This is from Frank Albini's 'dist.for' FORTRAN source, function height().
*/
void CrownFirebrandProcessor::processFlameHeight()
{
	double denom = m_canopyHt * m_canopyWind;
	if ( denom > 0. )
	{
		double con = 7.791e-03 * m_fireInt / denom;
		double ylo = 1.;
		double yhi = exp( con );
		while( true )
		{
			double y = 0.5 * ( ylo + yhi );
			double test = y * log( y );
			if ( fabs( test - con ) <= 1.0e-06 )
			{
				m_flameHt = m_canopyHt * ( y - 1. ) / 2.94;
				return;
			}
			yhi = ( test >= con ) ? y : yhi;
			ylo = ( test < con ) ? y : ylo;
		}
	}
}

//------------------------------------------------------------------------------
/*!	\brief Private method that resets all the output variables.
*/
void CrownFirebrandProcessor::reset()
{
	m_canopyWind = 0.;
	m_flameHt    = 0.;
	m_loftHt     = 0.;
	m_spotDist   = 0.;

	m_ang    = 0.;
	m_bf     = 0.;
	m_driftX = 0.;
	m_eta    = 1.;
	m_fm     = 0.;
	m_hf     = 0.;
	m_layer  = 0;
	m_loftX  = 0.;
	m_loftZ  = 0.;
	m_qfac   = 0.;
	m_qreq   = 0.;
	m_rhof   = 0.;
	m_uc     = 0.;
	m_uf     = 0.;
	m_vf     = 0.;
	m_wf     = 0.;
	m_wn     = 0.;
}

//------------------------------------------------------------------------------
/*!	\brief Resets all the input properties.
	\param canopyHt Mean canopy height at the top of the canopy of an active crown fire (m).
	\param fireInt Active crown fireline intensity (kW/m).
	\param windSpeed Mean wind speed at the top of the canopy of an active crown fire (km/h).
	\param windHt Height at which the wind speed measurement was taken (m).
	\param emberDiam Diameter of a viable firebrand ember when alighting at the fuel surface (mm)
	\param deltaStep Plume layer slice size (default = 0.2 m)
*/
void CrownFirebrandProcessor::set(
		double canopyHt,
		double fireInt,
		double windSpeed,
		double windHt,
		double emberDiam,
		double deltaStep )
{
	setCanopyHeight( canopyHt );
	setDeltaStep( deltaStep );
	setEmberDiameter( emberDiam );
	setFirelineIntensity( fireInt );
	setWindSpeedHeight( windHt );
	setWindSpeed( windSpeed );
}

//------------------------------------------------------------------------------
/*!	\brief Sets the mean canopy height at the top of the canopy of an active crown fire.
	\param canopyHt Mean canopy height at the top of the canopy of an active crown fire (m).
*/
void CrownFirebrandProcessor::setCanopyHeight( double canopyHt )
{
	if ( canopyHt != m_canopyHt )
	{
		m_canopyHt = canopyHt;
		m_isDirty = true;
	}
}

//------------------------------------------------------------------------------
/*!	\brief Sets the plume layer slice size (m).
	\param deltaStep The plume layer slice size (default = 0.2 m)
*/
void CrownFirebrandProcessor::setDeltaStep( double deltaStep )
{
	if ( deltaStep != m_deltaStep )
	{
		m_deltaStep = deltaStep;
		m_isDirty = true;
	}
}

//------------------------------------------------------------------------------
/*!	\brief Sets the diameter of a viable firebrand ember when it alights at the fuel surface.
	\param emberDiam Diameter of a viable firebrand embers when it alights at
	the fuel surface (mm).
*/
void CrownFirebrandProcessor::setEmberDiameter( double emberDiam )
{
	if ( emberDiam != m_emberDiam )
	{
		m_emberDiam = emberDiam;
		m_isDirty = true;
	}
}
//------------------------------------------------------------------------------
/*!	\brief Sets the active crown fireline intensity (kW/m).
	\param fireInt Active crown fireline intensity (kW/m).
*/
void CrownFirebrandProcessor::setFirelineIntensity( double fireInt )
{
	if ( fireInt != m_fireInt )
	{
		m_fireInt = fireInt;
		m_isDirty = true;
	}
}

//------------------------------------------------------------------------------
/*!	\brief Sets the mean wind speed.
	\param windSpeed Mean wind speed (km/h).
*/
void CrownFirebrandProcessor::setWindSpeed( double windSpeed )
{
	if ( windSpeed != m_windSpeed )
	{
		m_windSpeed = windSpeed;
		m_isDirty = true;
	}
}

//------------------------------------------------------------------------------
/*!	\brief Sets the height at which the mean wind speed is measured.
	\param windSpeedHt Height at which the mean wind speed is measured (m).
*/
void CrownFirebrandProcessor::setWindSpeedHeight( double windSpeedHt )
{
	if ( windSpeedHt != m_windHt )
	{
		m_windHt = windSpeedHt;
		m_isDirty = true;
	}
}

//------------------------------------------------------------------------------
/*!	\brief Private method called by all the accessor methods to ensure the
	processor calculations are up to date.
*/
void CrownFirebrandProcessor::update()
{
	if ( m_isDirty )
	{
		process();
		m_isDirty = false;
	}
}
