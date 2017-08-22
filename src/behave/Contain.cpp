//------------------------------------------------------------------------------
/*! \file Contain.cpp
    \author Copyright (C) 2006 by Collin D. Bevins.
    \license This is released under the GNU Public License 2.
    \brief An implementation of the Fried and Fried containemnt model.
  
    This is a fire containment algorithm based upon the paper
        Fried, Jeremy S. and Fried, Burton D.  1995.
            <i>Simulating wildfire containment with realistic tactics.</i>
    and heavily adapted from the FCAT Fire Containment Algorithm Test Pascal
    program by Jeremy S. Fried (1991).
  
    Primary assumptions are that the fire is growing under uniform fuel,
    terrain, and weather conditions, resulting in a steadily growing
    elliptical fire (except where contained).
  
    Containment "tactics" may be either head or rear attack.
    Containment resources are evenly divided and applied to each flank.
  
    All times (elapsed, resource arrival, containment time, etc)
    are from when the fire was first \b reported.
 */


//==============================================================================
//==============================================================================
//
//   Modified by MAF, 10/6/2008
//
//        added functionality for hourly varying spread rates
//        functions added:
//             SetDiurnalSpreadRates(double *rates)
//             SetFireStartTimeMinutes(double starttime)
//             GetDiurnalSpreadRate(double minutesSinceReport)
//
//        To use, for each fire:
//             1. Initialize Contain Class array of 24 double hourly spread rates
//                  with call to SetDiurnalSpreadRates().
//             2. Initialize the actual start time (m_StartTime, in minutes
//                  with call to SetFireStartTimeMinutes();
//             3. Replace call to fire = spreadRate( minutesSinceReport );
//                  with fire = GetDiurnalSpreadRate( minutesSinceReport );
//
//==============================================================================
//==============================================================================


//==============================================================================
//==============================================================================
//
//   Modified by MAF, 11/19/2008
//
//        commented out overrun case for reversal of "angular rotation"
//        functions changed:
//             Sem::Contain::calcUh()
//
//==============================================================================
//==============================================================================


//==============================================================================
//==============================================================================
//
//   Modified by MAF, 6/8/2010
//
//        added time steps (m_currentTimeAtFireHead, m_timeIncrement) for use in determining ROS
//        Changed the following functions:
//             ::CalcU()
//             ::headPosition()
//             ::productionRate()
//             ::productionRatio()
//             ::reset()
//
//
//==============================================================================
//==============================================================================


//==============================================================================
//==============================================================================
//
//   Modified by MAF, 9/29/2010
//
//                
//        Changed the following functions:
//             ::reset() -  modified previous mod that determines reportHead and reportBack
//
//
//==============================================================================
//==============================================================================

// Local include files
#include "Contain.h"
#include "ContainForce.h"

// Standard include files
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
using namespace std;

/*! \def M_PI
    \brief A well rounded number
 */
#ifndef M_PI
#define M_PI 3.141592654
#endif

/* Silence warnings that aren't our fault */
#if defined(OMFFR) && defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC push
#endif /* defined(OMFFR) && defined(__GNUC__) */

#ifdef OMFFR
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC push
#endif /* OMFFR */

//for logging inside contain.cpp - ok to be static since it's used as a readonly var
static int     m_logLevel = 0;


//------------------------------------------------------------------------------
/*! \brief Contain constructor.
  
    \param[in] reportSize Fire size at time of report (ac)
    \param[in] reportRate Fire spread rate at time of report (ch/h).
    \param[in] diurnalROS a ROS array for each hour of the day - 24 values
    \param[in] fireStartMinutesStartTime fire start time since midnight in minutes
    \param[in] lwRatio    Fire length-to-width ratio.
    \param[in] distStep   Simulation fire head distance step size (ch).
    \param[in] flank      Apply ContainResources assigned to the Left or Right
                          flank.  ContainResources assigned to Both flanks have
                          half their production rate applied to this flank.
    \param[in] force      Pointer to the ContainForce applied to the fire.
    \param[in] attackTime Elapsed time since fire report that ContainForces
                          are first committed to the fire.  This may be after the
                          arrival of one or more resources.
    \param[in] tactic     HeadAttack or RearAttack.
    \param[in] attackDist Forces build fireline this far from the fire edge (ch).
 */

Sem::Contain::Contain():
    m_eps(1.),
    m_eps2(1.),
    m_a(1.),
    m_reportHead(0.),
    m_reportTime(0.),
    m_backRate(0.),
    m_reportBack(0.),
    m_attackHead(0.),
    m_attackBack(0.),
    m_exhausted(0.),
    m_time(0.),
    m_step(0),
    m_u(0.),
    m_u0(0.),
    m_h(0.),
    m_h0(0.),
    m_x(0.),
    m_y(0.)
{
    //reset();
}

Sem::Contain::Contain(
        double reportSize,
        double reportRate,
        double diurnalROS[24],
        int fireStartMinutesStartTime,              
        double lwRatio,
        double distStep,
        ContainFlank flank,
        ContainForce& force,
        double attackTime,
        ContainTactic tactic,
        double attackDist ) :
    m_reportSize(reportSize),
    m_reportRate(reportRate),
    m_lwRatio(lwRatio),
    m_attackDist(attackDist),
    m_attackTime(attackTime),
    m_distStep(0.01),
    m_flank(flank),
    m_tactic(tactic),
    m_force(&force),
    m_eps(1.),
    m_eps2(1.),
    m_a(1.),
    m_reportHead(0.),
    m_reportTime(0.),
    m_backRate(0.),
    m_reportBack(0.),
    m_attackHead(0.),
    m_attackBack(0.),
    m_exhausted(0.),
    m_time(0.),
    m_step(0),
    m_u(0.),
    m_u0(0.),
    m_h(0.),
    m_h0(0.),
    m_x(0.),
    m_y(0.),
    m_status(Unreported),
    m_startTime(fireStartMinutesStartTime)
{
    // Set all the input parameters.
    setReport( reportSize, reportRate, lwRatio, distStep );
    setAttack( flank, force, attackTime, tactic, attackDist );
    setFireStartTimeMinutes(fireStartMinutesStartTime);
    setDiurnalSpreadRates(diurnalROS);
    // Set all the intermediate parameters.
    reset();
    
    containLog( false, "===== start contain ========= starttimemin=%d \n",fireStartMinutesStartTime );
    
    return;
}

//------------------------------------------------------------------------------
/*! \brief Contain class destructor.
 */

Sem::Contain::~Contain( void )
{
}

void Sem::Contain::updateInputs(double reportSize, double reportRate, double * diurnalROS, int fireStartMinutesStartTime, double lwRatio, double distStep, ContainFlank flank, ContainForce& force, double attackTime, ContainTactic tactic, double attackDist)
{
    // Set all the input parameters.
    m_status = Unreported;
    setFireStartTimeMinutes(fireStartMinutesStartTime);
    setForce(force);
    setReport(reportSize, reportRate, lwRatio, distStep);
    setAttack(flank, force, attackTime, tactic, attackDist);

    setDiurnalSpreadRates(diurnalROS);

    reset();
}

void Sem::Contain::setForce(ContainForce& force)
{
    m_force = &force;
}

//------------------------------------------------------------------------------
/*! \brief Determines the next value of the angle from the fire origin to the
    point of active fireline construction.
  
    \retval Next value of the angle from the fire origin to the point of
                active fireline construction is stored in m_u.
    \retval Next value of free-burning head position is stored in m_h.
    \retval Current value of m_status may be reset to Overrun upon return!
 */

void Sem::Contain::calcU( void )
{
    // Store the current u and h as the old u and h.
    m_u0 = m_u;
    m_h0 = m_h;
    m_status = Attacked;
    // Calculate constants used in the 4th order Runga-Kutta approximation.
    double rk[4], deriv;
    double OldDistStep=m_distStep;

    /*
    m_timeIncrement=0.0;   // MAF 6/2010
    m_rkpr[0] = ( m_step ) ? m_rkpr[2] : productionRatio( m_attackHead );
    m_timeIncrement/=2.0;  // MAF 6/2010
    m_rkpr[1] = productionRatio( m_h0 + ( 0.5 * m_distStep ) );
    m_rkpr[2] = productionRatio( m_h0 + m_distStep );
    */

   //--------------------------------------------------------------------------
   //     Changed MAF 6/2010, force timestep to recognize 1-minute resource arrivals
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
     do
     {    
         m_timeIncrement = 0.0;  // MAF  6/2010
         m_rkpr[0] = (m_step) ? m_rkpr[2] : productionRatio(m_attackHead);
         m_timeIncrement /= 2.0;  // MAF   6/2010
         m_rkpr[1] = productionRatio(m_h0 + (0.5 * m_distStep));
         m_rkpr[2] = productionRatio(m_h0 + m_distStep);
	     if(m_timeIncrement>1.0)        // mins, m_timeIncrement calc'd & set in productionRatio()
	     {	m_distStep/=2.0;

               continue;
          }
          //if(fabs(m_rkpr[0]-m_rkpr[2])>1e-6)
          //	m_distStep/=2.0;
     }while(m_timeIncrement>1.0);//fabs(m_rkpr[0]-m_rkpr[2])>1e-6);

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------



    // First constant
    if ( ! calcUh( m_rkpr[0], m_h0, m_u0, &deriv ) )
    {
          m_distStep=OldDistStep;

        return;
    }
    rk[0] = m_distStep * deriv;
    // Second constant
    if ( ! calcUh( m_rkpr[1], ( m_h0 + 0.5 * m_distStep ),
            ( m_u0 + 0.5 * rk[0] ), &deriv ) )
    {
          m_distStep=OldDistStep;

        return;
    }
    rk[1] = m_distStep * deriv;
    // Third constant
    if ( ! calcUh( m_rkpr[1], ( m_h0 + 0.5 * m_distStep ),
            ( m_u0 + 0.5 * rk[1] ), &deriv ) )
    {
          m_distStep=OldDistStep;

        return;
    }
    rk[2] = m_distStep * deriv;
    // Fourth constant
    if ( ! calcUh( m_rkpr[2], ( m_h0 + m_distStep ),
            ( m_u0 + rk[2] ), &deriv ) )
    {
          m_distStep=OldDistStep;

        return;
    }
    rk[3] = m_distStep * deriv;

    // Calculate 4th order Runga-Kutta approximation of u for next step.
    m_u = m_u0 + ( rk[0] + rk[3] + 2. * ( rk[1] + rk[2] ) ) / 6.;

    //--------------------------------------------------------------------------
    //  Modified by MAF 6/2010, to account for forcing timestep to be less than 1 min
    //--------------------------------------------------------------------------
    // Calculate next free-burning fire head position (ch from origin)
    //m_h = m_attackHead + ( m_step + 1 ) * m_distStep;
    if(m_step==0)
          m_h=m_attackHead;
    m_h+=m_distStep;

    m_distStep=OldDistStep;
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    return;
}

//------------------------------------------------------------------------------
/*! \brief Determines du/dh for a particular u, h, and p,
    and returns the value in d.
  
    If there is a negative term under the radical,
    which is indicative of the ContainResources being overrun by the fire,
    m_status is set to Overrun and the function returns FALSE.
    Similarly if a sign change occurs in du/dh.
  
    \param[in] p Fireline production rate (ch/h).
    \param[in] h Current distance of free-burning fire head from the origin (ch).
    \param[in] u Current angle from the fire origin to the point of active
                 fireline construction.
    \param[in] d Address where the du/dh derivative is returned.
  
    \retval TRUE if ContainResources are not overrun and d has valid result.
    \retval FALSE if ContainResources are overrun, and m_status is
            set to Overrun.
 */

bool Sem::Contain::calcUh( double p, double h, double u, double *d )
{
    // lastUh is used to check sign change between previous and current step
    static double lastUh = 0.0;
    double cosU = cos(u);
    double sinU = sin(u);
    *d = 0;

    // If the expression under the radical sign is negative,
    // must change course to avoid a complex (number) result
    double x = 1. - m_eps * cosU;
    double uh_radical = ( p * p * x / ( 1. + m_eps * cosU ) ) - m_a * m_a;

    // The gcc and VC6 compilers yield different results for uh_radical
    // as ros approaches the fireline production rate;
    // uh_radical approaches zero faster under VC6 than under gcc.
    // Enable the following containLog() calls to demonstrate.
    containLog( (m_logLevel>1),
        "\nStep %04d: p=%15.13f, h=%15.13f, u=%15.13f, sinU=%15.12f, cosU=%15.13f\n",
        m_step+1, p, h, u, sinU, cosU );
    containLog( (m_logLevel>1),
        "           x=%15.13f, m_eps=%15.13f, m_a=%15.13f, uh_radical=%15.13f\n",
        x, m_eps, m_a, uh_radical );

    if ( uh_radical <= 1.0e-10 )
    {
        uh_radical=0.0;				// MAF  6/2010
        
        //m_status = Overrun;	// MAF  6/2010
        //return( false );		// MAF  6/2010
    }
    // dh is the change in fire perimeter distance at point of attack

    double dh = x * h;
    if ( m_attackDist > 0.001 )
    {
        dh = x * ( h + ( 1. - m_eps ) *
           ( m_attackDist * sqrt( 1. - m_eps2 )
             / exp( 1.5 * log( 1. - ( m_eps2 * cosU * cosU ) ) ) ) );
    }
    // du is the change in angle of attack point from fire origin
    double du;
    if ( m_tactic == RearAttack )
    {
        du = m_eps * sinU - ( 1. + m_eps ) * sqrt( uh_radical );
    }
    else
    {
        du = m_eps * sinU + ( 1. + m_eps ) * sqrt( uh_radical );
    }
    double uh = du / dh;
    containLog( (m_logLevel>1),
        "           sqrt(uh_radical)=%15.13f, du=%12.10f, dh=%12.10f, uh(du/dh)=%12.10f\n",
        sqrt(uh_radical), du, dh, uh );

   /* Commented out as instruction from Mark Finney
    // If "angular rotation" has reversed. firefighters may be overrun
    // and cannot even build line making NO rotational progress
    if ( ( m_tactic == RearAttack && lastUh < 0. && uh >= 0. )
       | ( m_tactic == HeadAttack && lastUh > 0. && uh <= 0. ) )
    {
        if ( m_step )
        {
            m_status = Overrun;
            return( false );
        }
    }*/

    // overrun -- line production less than the minimum spread rate
    //if(p<=m_backRate)			// MAF 6/2010
    //{	m_status=Overrun;		// MAF 6/2010
    //      return false;		// MAF 6/2010
    //}

    // Store uh in lastUh and returned value
    lastUh = uh;
    *d = uh;
    return( true );
}

//------------------------------------------------------------------------------
/*! \brief Determines the x- and y- coordinates ( m_x and m_y)
    for the current angle (m_u) and free-burning head position (m_h).
 */

void Sem::Contain::calcCoordinates( void )
{
    // Determine the x and y coordinate.
    m_y = sin( m_u ) * m_h * m_a;
    m_x = ( cos( m_u ) + m_eps ) * m_h / ( 1. + m_eps );
    if ( m_attackDist > 0.001 )
    {
        double psiVal = containPsi( m_u, m_eps2 );
        m_y += m_attackDist * sin( psiVal );
        m_x += m_attackDist * cos( psiVal );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief Logs the message to stdout.
 */

void Sem::Contain::containLog( bool dolog, char *fmt, ... ) const
{
    static FILE *fptr = 0;
    if ( dolog )
    {
        // Open log file on first call.
        if ( ! fptr )
        {
            fptr = fopen( "/Cprogramming/Helitack/contain.log", "w" );
        }
        
        // Write formatted output
        va_list ap;
        va_start( ap, fmt );
        if (fptr) {
          vfprintf( fptr, fmt, ap );
          va_end( ap );
          fflush(fptr);
        } else {
           printf(fmt,ap);
           va_end( ap );
           fflush(stdout);	
        }
      
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief Used only in the case of parallel attack, this function supplies
    values of Psi when translating from u,h to x,y.
 */

double Sem::Contain::containPsi( double u, double eps2 )
{
    // As u gets close to pi/2, accuracy of the arctan becomes problematic,
    // so we check for this and fudge.
    double ro = ( u - ( M_PI / 2. ) );
    if ( fabs( ro ) < 0.00001 )
    {
        if ( ro > 0. )
        {
            u = ( M_PI / 2. ) + 0.00001;
        }
        else
        {
            u = ( M_PI / 2. ) - 0.00001;
        }
    }
    // Since Psi is never negative in our simulation,
    // we need to map into the appropriate quadrant.
    double psiVal = atan( ( sin( u ) / cos( u ) ) / sqrt(1. - eps2) );
    if ( psiVal < 0. )
    {
        psiVal += M_PI;
    }
    return( psiVal );
}

//------------------------------------------------------------------------------
/*! \brief API access to the time when all the ContainResources are exhausted.
  
    \return Time when all the ContainResources are exhausted
            (minutes since report).
 */

double Sem::Contain::exhaustedTime( void ) const
{
    return( m_exhausted );
}

//------------------------------------------------------------------------------
/*! \brief Access to the parallel attack distance from the fire perimeter.

    \return Parallel attack distance from the fire perimeter (ch).
 */

double Sem::Contain::attackDistance( void ) const
{
    return( m_attackDist );
}

//------------------------------------------------------------------------------
/*! \brief Access to the current containment attack point x-coordinate.

    \return Current containment attack point x-coordinate.
 */

double Sem::Contain::attackPointX( void ) const
{
    return( m_x );
}

//------------------------------------------------------------------------------
/*! \brief Access to the current containment attack point y-coordinate.

    \return Current containment attack point y-coordinate.
 */

double Sem::Contain::attackPointY( void ) const
{
    return( m_y );
}

//------------------------------------------------------------------------------
/*! \brief Access to the initial attack time
    (minutes since the fire report time).

    \return Initial attach time (minutes since the fire report time).
 */

double Sem::Contain::attackTime( void ) const
{
    return( m_attackTime );
}

//------------------------------------------------------------------------------
/*! \brief Access to the simulation fire head distance step
    estimated to achieve the target [minSteps..maxSteps] simulation steps.

    \return Simulation fire head distance step (ch).
 */

double Sem::Contain::distanceStep( void ) const
{
    return( m_distStep );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire back position at initial attack.

    \return Fire back spread distance at time of initial attack (ch).
 */

double Sem::Contain::fireBackAtAttack( void ) const
{
    return( m_attackBack );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire back position at report time.

    \return Fire back spread distance at time of report (ch).
 */

double Sem::Contain::fireBackAtReport( void ) const
{
    return( m_reportBack );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire head position at initial attack.

    \return Fire head spread distance at time of initial attack (ch).
 */

double Sem::Contain::fireHeadAtAttack( void ) const
{
    return( m_attackHead );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire head position at report time.

    \return Fire head spread distance at time of report (ch).
 */

double Sem::Contain::fireHeadAtReport( void ) const
{
    return( m_reportHead );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire length-to-width ratio the time of report.

    \return Fire length-to-width ratio at the time of report (ch/ch)
 */

double Sem::Contain::fireLwRatioAtReport( void ) const
{
    return( m_lwRatio );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire elapsed time from ignition to report.

    \return Elapsed time from fire ignition to fire report (min).
 */

double Sem::Contain::fireReportTime( void ) const
{
    return( m_reportTime );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire size at the time of report.

    \return Fire size at the time of report (ac).
 */

double Sem::Contain::fireSizeAtReport( void ) const
{
    return( m_reportSize );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire backing spread rate at the time of report.

    \return Fire backing spread rate at the time of report (ch/hr).
 */

double Sem::Contain::fireSpreadRateAtBack( void ) const
{
    return( m_reportBack );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire spread rate at the time of report.

    \return Fire spread rate at the time of report (ch/hr).
 */

double Sem::Contain::fireSpreadRateAtReport( void ) const
{
    return( m_reportRate );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainForce assigned to the fire.

    \return Pointer to the ContainForce assigned to the fire.
 */

Sem::ContainForce* Sem::Contain::force( void ) const
{
    return( m_force );
}

//------------------------------------------------------------------------------
/*! \brief Determines the fire head position at the specified time.
  
    \param[in] minutesSinceReport The fire head position is determined for this
    many minutes since the fire was reported.
  
    \note This function must be modified to support variable ROS fires.
  
    \return Head position at the specified time (chains from fire origin).
 */

double Sem::Contain::headPosition( double minutesSinceReport ) const
{
//    return( m_reportHead + m_reportRate * minutesSinceReport / 60. );

//----------------------------------------------------
//   Added MAF 5/27/2010
//        determines head fire dist by iterating by hour
//----------------------------------------------------
     long i, NumHours;
     double AddHeadDist, minutesIncrement, minutesCumulative, minutesRemainder;
//	double AddBackDist;

	 minutesCumulative=0.0;
	 AddHeadDist=m_reportHead;

	 // DT 4/17/12 Need to deal with the partial hour that might exist in the first hour of the fire since discovery
	 double timeRemainingHr = 60 - ( m_startTime - 60 * static_cast< int >(m_startTime / 60.0) );
	 if ( minutesSinceReport < timeRemainingHr )
		timeRemainingHr = minutesSinceReport;
	 AddHeadDist+=getDiurnalSpreadRate(0)*timeRemainingHr/60.0;	
	 minutesSinceReport = minutesSinceReport - timeRemainingHr;
	 minutesCumulative = timeRemainingHr;
	 // end added DT 4/17/12

     NumHours=(long) (minutesSinceReport/60.0)+1;
     minutesRemainder=minutesSinceReport-((double) (NumHours-1)*60.0);

     for(i=0; i<NumHours; i++)
     {    if(i<NumHours-1)
               minutesIncrement=60.0;
          else
               minutesIncrement=minutesRemainder;
          
          AddHeadDist+=getDiurnalSpreadRate(minutesCumulative)*minutesIncrement/60.0;
		  minutesCumulative+=minutesIncrement;
     }

     return AddHeadDist;
}

//------------------------------------------------------------------------------
/*! \brief Determines the aggregate fireline production rate of the entire
    containment force on the specified flank when the free burning fire head
    would have reached the specified position.
    THIS IS HALF THE TOTAL PRODUCTION RATE FOR BOTH FLANKS,
    AND HALF THE PRODUCTION RATE ENTERED FOR EACH RESOURCE.
  
    \param[in] fireHeadPosition Position of the free-burning fire head (ch).
  
    \return Aggregate containment force holdable fireline production rate
    (ch/h).
 */

double Sem::Contain::productionRate( double fireHeadPosition ) const
{
    //----------------------------------------------
    //    MAF 6/2010
    //         determines minutes since report based on accumulated time
    //----------------------------------------------
    double minutesSinceReport;// = timeSinceReport( fireHeadPosition );

    minutesSinceReport=m_currentTimeAtFireHead+m_attackTime;
    //----------------------------------------------

    double prod = m_force->productionRate( minutesSinceReport, m_flank );
    return( prod );
}

//------------------------------------------------------------------------------
/*! \brief Determines the ratio of the aggregate fireline production rate of
    the entire containment force on the specified flank when the free burning
    fire head would have reached the specified position,
    to the fire head spread rate at that specified time.
  
    THIS IS HALF THE TOTAL PRODUCTION RATE FOR BOTH FLANKS,
    AND HALF THE PRODUCTION RATE ENTERED FOR EACH RESOURCE.
  
    \param[in] fireHeadPosition Position of the free-burning fire head (ch).
  
    \return Ratio of the aggregate containment force holdable fireline
    production rate to the fire head spread rate.
 */

double Sem::Contain::productionRatio( double fireHeadPosition ) 
{
    //--------------------------------------------------------
    //    MAF 6/2010
    //         determines minutes using time steps instead - needed if ROS changes with time
    //--------------------------------------------------------
    double minutesSinceReport;// = timeSinceReport( fireHeadPosition );

    minutesSinceReport=m_currentTimeAtFireHead+m_attackTime+m_timeIncrement;
    //--------------------------------------------------------

    double prod = m_force->productionRate( minutesSinceReport, m_flank );
    //double originalfire = spreadRate( minutesSinceReport );
    
    double fire = getDiurnalSpreadRate( minutesSinceReport );
    if ( fire < 0.0001 )
    {
        fire = 0.0001;
    }

    //--------------------------------------------------------
    // MAF 6/2010
    //--------------------------------------------------------
    m_timeIncrement=m_distStep/fire*60.0;       // minutes = ch/ch/hr*60
    //--------------------------------------------------------
    //--------------------------------------------------------

    double ratio = prod / fire;
    containLog( (m_logLevel>1), " %lf %lf %lf \n", minutesSinceReport, prod, fire );
    return( ratio );

}



//===============================================================================
//===============================================================================
//
//   Added MAF 10/6/2008
//        attempt to include diurnal spread rates
//             added m_StartTime = actual minutes of day fire started
//             added DiurnalSpreadRates[24] -- contains hours spread rates
//
//===============================================================================
//===============================================================================
bool Sem::Contain::setDiurnalSpreadRates(double *rates)
{	
     memcpy(m_diurnalSpreadRate, rates, 24*sizeof(double));    
     return true;
}


double Sem::Contain::setFireStartTimeMinutes(int starttime)
{
	// removed 6/29/2010,  MAF
	//	changed interpretation of StartTime to reportTime, 
	//	m_startTime set to 0.0 and not used
     //m_startTime=starttime;   // m_StarTime added to private variable list

	// added 6/29/2010, MAF
		m_startTime=0;
		m_reportTime=starttime;

		return starttime;
}


double Sem::Contain::getDiurnalSpreadRate(double minutesSinceReport) const
{
     //long i, loc;
     long CurrentHour;
     double CurrentTime;

	 // Modified the following statement DT 4/17/12
	 // m_reportTime - the time the fire took to grow to the discovery size, is no longer used
	 //CurrentTime=minutesSinceReport+m_reportTime+m_startTime;
     CurrentTime=minutesSinceReport+m_startTime;
	 
     while(CurrentTime>=1440.0)
     {    CurrentTime-=1440.0;
     };
     CurrentHour=(long) (CurrentTime/60.0);
     if(CurrentHour>23)
          CurrentHour=23;

     return m_diurnalSpreadRate[CurrentHour];
}

//===============================================================================
//===============================================================================

//------------------------------------------------------------------------------
/*! \brief Initializes the Contain state from the current parameter values.

    Should be called after calling either setAttack() or setReport().
 */

void Sem::Contain::reset( void )
{
    //double CTime, fsize, CRate, L, W, atktime;
	
    m_currentTimeAtFireHead=0.0;
    m_timeIncrement=0.0;
    m_currentTime=m_attackTime;//0.0;


    // Eccentricity
    double r = 1. / m_lwRatio;
    m_eps2 = 1. - (r * r);
    m_eps = ( m_eps2 > 0.00001 ) ? sqrt( m_eps2 ) : 0.0;
    m_a = sqrt( (1. - m_eps) / (1. + m_eps) );

    // Fire head position at time of report (ch)
    double ch2 = 10. * m_reportSize;
    m_reportHead = (1. + m_eps ) * sqrt( ch2 / ( M_PI * sqrt(1. - m_eps2) ) );
	
    // Elapsed time from fire start to time of report (min)
    // Used only to draw ellipses using fire start as origin
    if ( m_reportRate > 0.0001 )
    {
        m_reportTime = 60. * m_reportHead / m_reportRate;
    }

    // Fire backing spread rate (ch/h)
    // (used only to draw ellipses when using fire start as axis origin)
    m_backRate = m_reportRate * (1. - m_eps) / (1. + m_eps);

    // Fire tail position at time of report (ch)
    // (used only to draw ellipses when using fire start as axis origin)
    m_reportBack = m_backRate * m_reportTime / 60.;


//--------------------------------------------------------------
//--------------------------------------------------------------
// added MAF 6/2010
// gets head distance and report time using timestep with check on discovery fire size
// will work as long as eccentricity of ellipse is constant with time
//	
//	removed/commented 9/2010
//--------------------------------------------------------------
//--------------------------------------------------------------
/*
	m_reportTime=0.0;
	m_reportHead=0.0;
	m_reportBack=0.0;
	double timeinc=0.5, headinc, backinc;
     do
	{    m_reportTime+=timeinc;	
		CRate=getDiurnalSpreadRate(0.0);
		headinc=CRate*timeinc/60.0;
		m_reportHead+=headinc;
		m_backRate=CRate*(1.0-m_eps)/(1.0+m_eps);
		backinc=m_backRate*timeinc/60.0;
		m_reportBack+=backinc;
		L=(m_reportHead+m_reportBack)/2.0;
		W=L*r;
		fsize=M_PI*(L*W)/10.0;  // convert LW square chains to acres
		if(fsize-m_reportSize>1e-6)	// overshoot, then redo with shorter timestep
		{	m_reportTime-=timeinc;
			m_reportHead-=headinc;
			m_reportBack-=backinc;
			timeinc/=2.0;			// half of timeinc
			fsize=0.0;
		}
	}while(fabs(fsize-m_reportSize)>1e-6);
*/
	
//--------------------------------------------------------------
//--------------------------------------------------------------
//  ADDED 9/27/2010		, MAF
//	Leaves m_reportTime alone, does not estimate from "m_StartTime"
//--------------------------------------------------------------
	// DT  4/17/12 removed the following line because it calculates the semi minor axis of the elipse not the the head distance at the fire's report time
	// The correct value of m_reportHead is calculated earlier in this function
	//m_reportHead=sqrt((r*m_reportSize*10.0)/M_PI);
	m_reportBack=m_reportHead*(1.0-m_eps)/(1.0+m_eps);

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

    // Fire head position at first attack
    m_attackHead = headPosition( m_attackTime );

    // Fire back position at first attack
    // (used only to draw ellipses when using fire start as axis origin)
    //m_attackBack = m_backRate * ( m_reportTime + m_attackTime ) /60.;

    //------------------------------------------------------------------------
    // added MAF  6/2010, will work as long as the eccentricity of ellipse is constant
    //------------------------------------------------------------------------
    m_attackBack=m_attackHead*(1.0-m_eps)/(1.0+m_eps);
    //------------------------------------------------------------------------

    // Initial angle to attack point depends on whether HeadAttack or RearAttack
    if ( m_tactic == RearAttack )
    {
        m_u = m_u0 = M_PI;
        m_x = -m_attackBack - m_attackDist;
    }
    else
    {
        m_u = m_u0 = 0.;
        m_x = m_attackHead + m_attackDist;
    }
    m_h = m_h0 = m_attackHead;
    m_y = 0.;

    // Initialization
    m_step = 0;
    m_time = 0.0;
    m_rkpr[0] = m_rkpr[1] = m_rkpr[2] = 0.;
    m_status = Reported;        // Also means that we're initialized

    // Log it
    containLog( (m_logLevel>1), "\n\nCONTAIN RESET-----------------------------\n\n" );
    containLog( (m_logLevel>1), "Eta   = %12.10f\n", m_distStep );
    containLog( (m_logLevel>1), "eps   = %12.10f\n", m_eps );
    containLog( (m_logLevel>1), "EpsSq = %12.10f\n", m_eps2 );
    containLog( (m_logLevel>1), "A     = %12.10f\n", m_a );
    containLog( (m_logLevel>1), "hr    = %12.10f\n", m_reportHead );
    containLog( (m_logLevel>1), "ho    = %12.10f\n", m_attackHead );
    return;
}

//------------------------------------------------------------------------------
/*! \brief Access to the number of ContainResources in the containment force.
 
    \return Number of ContainResources in the ContainForce.
 */

int Sem::Contain::resources( void ) const
{
    return( m_force->m_resourceVector.size() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the arrival time of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's arrival time (minutes since fire was
    reported).
 */

double Sem::Contain::resourceArrival( int index ) const
{
    return( m_force->resourceArrival( index ) );
}

//------------------------------------------------------------------------------
/*! \brief Access to the base cost of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's base cost.
 */

double Sem::Contain::resourceBaseCost( int index ) const
{
    return( m_force->resourceBaseCost( index ) );
}

//------------------------------------------------------------------------------
/*! \brief Access to the specified ContainmentResouce's description.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's description.
 */

std::string Sem::Contain::resourceDescription( int index ) const
{
    return m_force->resourceDescription( index );
}

//------------------------------------------------------------------------------
/*! \brief Access to the duration time of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's duration time (minutes).
 */

double Sem::Contain::resourceDuration( int index ) const
{
    return( m_force->resourceDuration( index ) );
}

//------------------------------------------------------------------------------
/*! \brief Access to the hourly cost of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's hourly cost.
 */

double Sem::Contain::resourceHourCost( int index ) const
{
    return( m_force->resourceHourCost( index ) );
}

//------------------------------------------------------------------------------
/*! \brief Access to the holdable fireline production rate of the
    specified ContainmentResouce ON BOTH FLANKS.  The rate for one flank
    is half this amount, since the resource is assumed to be split in two
    and working on both flanks simultaneously.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's holdable fireline production rate (ch/h).
 */

double Sem::Contain::resourceProduction( int index ) const
{
    return( m_force->resourceProduction( index ) );
}

//------------------------------------------------------------------------------
/*! \brief Sets the Contain attack parameters.
  
    \param[in] flank      Apply ContainResources assigned to the Left or Right
                          flank.  ContainResources assigned to Both flanks have
                          half their production rate applied to this flank.
    \param[in] force      Pointer to the ContainForce applied to the fire.
    \param[in] attackTime Elapsed time since fire report that ContainForces
                          are first committed to the fire.  This may be after
                          the arrival of one or more resources.
    \param[in] tactic     HeadAttack or RearAttack.
    \param[in] attackDist Forces build fireline this far from the fire edge (ch).

    Called only by the constructor.
 */

void Sem::Contain::setAttack( ContainFlank flank, ContainForce& force,
        double attackTime, ContainTactic tactic, double attackDist )
{
    m_flank      = flank;
    m_force      = &force;
    m_attackTime = attackTime;
    m_tactic     = tactic;
    m_attackDist = attackDist;
    m_exhausted  = m_force->exhausted( m_flank );
    return;
}

//------------------------------------------------------------------------------
/*! \brief Sets the Contain fire report time parameters.
  
    \param[in] reportSize Fire size at time of report (ac)
    \param[in] reportRate Fire spread rate at time of report (ch/h).
    \param[in] lwRatio    Fire length-to-width ratio
    \param[in] distStep   Simulation fire head distance step size (ch).

    Called only by the constructor.
 */

void Sem::Contain::setReport( double reportSize, double reportRate, double lwRatio,
        double distStep )
{
    m_reportSize = reportSize;
    m_reportRate = reportRate;
    m_distStep   = distStep;
    if ( ( m_lwRatio = lwRatio ) < 1.0 )
    {
        m_lwRatio = 1.0;
    }

    //--------------------------------------------------------
    //    MAF
    //--------------------------------------------------------
    for(long i=0; i<24; i++)
          //DiurnalSpreadRate[i]=reportRate;
          m_diurnalSpreadRate[i]=reportRate;
 //ssb 0616 not sure if this was the intent
    //--------------------------------------------------------

    return;
}

//------------------------------------------------------------------------------
/*! \brief Access to the simulation time step.
  
    \return Simulation time step.
 */

int Sem::Contain::simulationStep( void ) const
{
    return( m_step );
}

//------------------------------------------------------------------------------
/*! \brief Access to the simulation time.
  
    \return Simulation time (minutes since report).
 */

double Sem::Contain::simulationTime( void ) const
{
    return( m_time );
}

//------------------------------------------------------------------------------
/*! \brief Access to the containment simulation status.

    \return Attack tactic Sem::ContainStatus
        - Unreported = Fire started but not yet reported (init() not called)
        - Reported   = Fire reported but not yet attacked (init() called)
        - Attacked   = Fire attacked but not yet resolved
        - Contained  = Fire contained by attacking forces
        - Overrun    = Attacking forces are overrun
        - Exhausted  = Fire escaped when all resources are exhausted
        - Overflow   = Simulation max step overflow
 */

Sem::Contain::ContainStatus Sem::Contain::status( void ) const
{
    return( m_status );
}

//------------------------------------------------------------------------------
/*! \brief Determines the fire head spread rate at the specified time.
  
    \param[in] minutesSinceReport Minutes since the fire was reported.
            Currently UNUSED until support for variable ROS is added.
  
    Note: This function must be modified to support variable ROS fires.
  
    \return Fire head spread rate (ch/h).
 */

double Sem::Contain::spreadRate( double /* minutesSinceReport */ ) const
{
    return( m_reportRate );
}

//------------------------------------------------------------------------------
/*! \brief Performs one containment simulation step by incrementing the head
    position by the distance step \a m_distStep.

    \retval Current fire status.
 */

Sem::Contain::ContainStatus Sem::Contain::step( void )
{
    // Determine next angle and fire head position.
    calcU();

    // Increment step counter
    m_step++;

    // Determine the elapsed time since the fire was reported.
    m_time = timeSinceReport( m_h );

    // If forces were overrun, simply return false
    if ( m_status == Overrun )
    {
        return( m_status );
    }
    // If the forces contain the fire, interpolate the final u and h.
    if ( m_tactic == HeadAttack && m_u >= M_PI )
    {
        m_status = Contained;
        m_h = m_h0 - m_distStep * m_u0 / ( m_u0 + fabs( m_u ) );
        m_u = M_PI;
    }
    else if ( m_tactic == RearAttack && m_u <= 0.0 )
    {
        m_status = Contained;
        m_h = m_h0 + m_distStep * m_u0 / ( m_u0 + fabs( m_u ) );
        m_u = 0.;
    }
    // Determine the x and y coordinate.
    calcCoordinates();
    //---------------------------------------------
    // MAF 6/2010
    //---------------------------------------------
    m_currentTimeAtFireHead+=m_timeIncrement;
    m_currentTime=m_currentTimeAtFireHead+m_attackTime;
    //---------------------------------------------

    return( m_status );
}

//------------------------------------------------------------------------------
/*! \brief Access to the attack tactic.

    \return Attack tactic Sem::ContainTactic
        - HeadAttack = 0
        - RearAttack = 1
 */

Sem::Contain::ContainTactic Sem::Contain::tactic( void ) const
{
    return( m_tactic );
}

//------------------------------------------------------------------------------
/*! \brief Determines time since fire report at which the free-burning fire
    head position reaches the specified distance from the fire origin (min).
  
    \param[in] headPos Free-burning fire head position (chains from origin).
  
    Note: This function must be modified to support variable ROS fires.
  
    \return Time since fire report (min).
 */

double Sem::Contain::timeSinceReport( double headPos ) const
{
    if ( m_reportRate > 0.00001 )
    {
        return( 60. * ( headPos - m_reportHead ) / m_reportRate );
    }
    return( 0. );
}

char * Sem::Contain::printStatus(Sem::Contain::ContainStatus cs) {
	char *status;
	switch (cs) {
    case Sem::Contain::Unreported:
	    status =  "Unreported"; //!< Fire started but not yet reported (init() not called)
	    break;
    case Sem::Contain::Reported:
      status =  "reported"; //!< Fire reported but not yet attacked (init() called)
      break;
    case Sem::Contain::Attacked:
      status =  "Attacked"; //!< Fire attacked but not yet resolved
      break;
    case Sem::Contain::Contained:
      status = "Contained"; //!< Fire contained by attacking forces
      break;
    case Sem::Contain::Overrun:
      status = "Overrun"; //!< Attacking forces are overrun
      break;
    case Sem::Contain::Exhausted:
      status = "Exhausted";//!< Fire escaped when all resources are exhausted
      break;
    case Sem::Contain::Overflow:
      status= "Overflow"; //!< Simulation max step overflow
      break;
    case Sem::Contain::SizeLimitExceeded:
 	    status = "SizeLimitExceeded";  //!< Simulation max fire size exceeded   
 	    break; 	       	      
 	case Sem::Contain::TimeLimitExceeded:
 	    status = "TimeLimitExceeded";  //!< Simulation max fire time exceeded   
 	    break; 	    
    default:
 	    status = "unknown state";
 	    break;
	}
	return status;
}

#if defined(OMFFR) && defined(__GNUC__)
#pragma GCC pop
#endif


//------------------------------------------------------------------------------
//  End of Contain.cpp
//------------------------------------------------------------------------------

