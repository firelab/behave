//------------------------------------------------------------------------------
/*! \file ContainSim.cpp
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

// Local include files
#include <iostream>
#include "ContainSim.h"
//include "Logger.h"

// Standard include files

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
/*! \brief ContainSim custom constructor.
  
    ContainSim contains all the information to make a complete simulation run,
    and when completed, display the fire perimeter in Cartesian coordinates.
  
    The following inputs are usually provided by the user:
    \param[in] reportSize Fire size at time of report (ac)
    \param[in] reportRate Fire spread rate at time of report (ch/h).
    \param[in] diurnalROS a ROS array for each hour of the day - 24 values (ch/h)
    \param[in] fireStartMinutesStartTime fire start time since midnight in minutes
    \param[in] lwRatio    Fire length-to-width ratio.
    \param[in] force      Pointer to the ContainForce applied to the fire.
    \param[in] tactic     HeadAttack or RearAttack.
    \param[in] attackDist Forces build fireline this far from the fire edge (ch).
  
    The following inputs can usually be fixed parameters:
    \param[in] minSteps   Minimum number of simulation distance steps.
    \param[in] maxSteps   Maximum number of simulation distance steps.
    \param[in] retry      If TRUE, if forces are overrun, the simulation is re-run
                          starting with the next later attack time.
	\param[in] maxFireSize Max fire size (Acres). If fire reaches this size then the fire escapes.
	\param[in] maxFireTime Max fire time (Minutes). If fire burns for this long then the fire escapes.
                                        
                          
 */

Sem::ContainSim::ContainSim():
        m_finalCost(0.),
        m_finalPerim(0.),
        m_finalSize(0.),
        m_finalSweep(0.),
        m_finalTime(0.),
        m_xMax(0.),
        m_xMin(0.),
        m_yMax(0.),
        m_u(0),
        m_h(0),
        m_x(0),
        m_y(0),
        m_a(0),
        m_p(0),
        m_left(),
        m_right(),
        m_size(0),
        m_pass(0),
        m_used(0)
{
    m_minSteps = 250;
    m_maxSteps = 1000;
    m_retry = true;
    m_maxFireSize = 1000;
    m_maxFireTime = 1080;
}

Sem::ContainSim::ContainSim(
    double reportSize,
    double reportRate,
    double diurnalROS[24],
    int fireStartMinutesStartTime,
    ContainForce& force,
    double lwRatio,
    Sem::Contain::ContainTactic tactic,
    double attackDist,
    bool retry,
    int minSteps,
    int maxSteps,
    int maxFireSize,
    int maxFireTime) :
    m_finalCost(0.),
    m_finalPerim(0.),
    m_finalSize(0.),
    m_finalSweep(0.),
    m_finalTime(0.),
    m_xMax(0.),
    m_xMin(0.),
    m_yMax(0.),
    m_u(0),
    m_h(0),
    m_x(0),
    m_y(0),
    m_a(0),
    m_p(0),
    m_left(),
    m_right(),
    m_force(&force),
    m_minSteps(minSteps),
    m_maxSteps(maxSteps),
    m_size(0),
    m_pass(0),
    m_used(0),
    m_retry(retry),
    m_maxFireSize(maxFireSize),
    m_maxFireTime(maxFireTime)
{
	int logLevel = 0;
	
    // Estimate distance step size for the initial simulation.
    // May be adjusted in subsequent simulations to get the number of
    // simulation steps in the range [m_minSteps..m_maxSteps].
    if ( m_maxSteps < 10 )
    {
        m_maxSteps = 10;
    }
    //Diane Step's distance is approximately 1 minute regardless of the number of steps allowed
    double distStep = reportRate / 60.;
    //double distStep = force->exhausted( LeftFlank ) * ( reportRate / 60. )
    //                / (double) ( m_maxSteps - 2. );
                               
    // Try attacking at first resource arrival.
    // If the initial attack forces are overrun, subsequent simulations
    // delay the initial attack until the next arrival of forces.
    double attackTime = m_force->firstArrival( LeftFlank );

    // Create the left flank
	  m_left.updateInputs( reportSize, reportRate, 
        diurnalROS,fireStartMinutesStartTime,
        lwRatio, distStep,
        LeftFlank, force, attackTime, tactic, attackDist );


    if (logLevel > 0) {
       m_left.containLog( true, " reportSize=%f,  reportRate=%f, lwRatio=%f, tactic=%d ,attackDist=%f   \n",reportSize, reportRate, lwRatio,tactic, attackDist);
       m_left.containLog( true, "retry=%d minSteps=%d maxSteps=%d maxFireSize=%d maxFireTime=%d \n",    retry,  minSteps, maxSteps,   maxFireSize,  maxFireTime);
       m_left.containLog( true, "attackTime=%f m_force->numresource=%d \n" , attackTime, m_force->resources());
       force.logResources( true, m_left );
    }
    
    //Check for invalid attack time. The algorithm goes into an endless loop on negative values 
    if (attackTime<0) {
      m_left.containLog( true, "attackTime=%f m_force->numresource=%d \n" , attackTime, m_force->resources());    
      force.logResources( true, m_left );
      throw INVALID_RESOURCE_TIME_ERROR;
    }
    
    // Create the right flank
    m_right.updateInputs(reportSize, reportRate,
        diurnalROS, fireStartMinutesStartTime,
        lwRatio, distStep,
        LeftFlank, force, attackTime, tactic, attackDist);
    //attackTime = m_force->firstArrival( RightFlank );
    //m_right = new Contain( reportSize, reportRate, lwRatio,  distStep,
    //    RightFlank, force, attackTime, tactic, attackDist );

    // How big do the arrays need to be?
    //Carmi commented out - m_right is not being initialized anymore 
    //m_size = ( m_right ) ? 2 * m_maxSteps : m_maxSteps;
    //allocate an extra so we don't go out of bounds on the arrays
    m_size =  m_maxSteps+1; 

    // Array of attack point angles (radians) at each simulation step.
    m_u.resize(m_size);
    // Array of free-burning fire head positions (ch) at each simulation step.
    m_h.resize(m_size);
    // Array of attack point x coordinates (ch) at each simulation step.
    m_x.resize(m_size);
    // Array of attack point y coordinates (ch) at each simulation step.
    m_y.resize(m_size);
    // Array of area under the perimeter curve (ch2) burned at each sim step.
    m_a.resize(m_size);
    // Array of fireline perimeter (ch) constructed at each simulation step.
    m_p.resize(m_size);
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainSim destructor.
 */

Sem::ContainSim::~ContainSim( void )
{
    return;
}

void Sem::ContainSim::updateInputs(
    double reportSize, 
    double reportRate, 
    double diurnalROS[24],
    int fireStartMinutesStartTime,
    ContainForce & force,
    double lwRatio,
    Contain::ContainTactic tactic,
    double attackDist,
    bool retry,
    int minSteps,
    int maxSteps,
    int maxFireSize,
    int maxFireTime)
{
    int logLevel = 0;

    // Estimate distance step size for the initial simulation.
    // May be adjusted in subsequent simulations to get the number of
    // simulation steps in the range [m_minSteps..m_maxSteps].
    if (m_maxSteps < 10)
    {
        m_maxSteps = 10;
    }
    //Diane Step's distance is approximately 1 minute regardless of the number of steps allowed
    double distStep = reportRate / 60.;
    //double distStep = force->exhausted( LeftFlank ) * ( reportRate / 60. )
    //                / (double) ( m_maxSteps - 2. );

    // Try attacking at first resource arrival.
    // If the initial attack forces are overrun, subsequent simulations
    // delay the initial attack until the next arrival of forces.
    m_force = &force;
    double attackTime = m_force->firstArrival(LeftFlank);

    // Create the left flank
    m_left.updateInputs(reportSize, reportRate,
        diurnalROS, fireStartMinutesStartTime,
        lwRatio, distStep,
        LeftFlank, force, attackTime, tactic, attackDist);


    if (logLevel > 0) {
        m_left.containLog(true, " reportSize=%f,  reportRate=%f, lwRatio=%f, tactic=%d ,attackDist=%f   \n", reportSize, reportRate, lwRatio, tactic, attackDist);
        m_left.containLog(true, "retry=%d minSteps=%d maxSteps=%d maxFireSize=%d maxFireTime=%d \n", retry, minSteps, maxSteps, maxFireSize, maxFireTime);
        m_left.containLog(true, "attackTime=%f m_force->numresource=%d \n", attackTime, m_force->resources());
        force.logResources(true, m_left);
    }

    //Check for invalid attack time. The algorithm goes into an endless loop on negative values 
    if (attackTime<0) {
        m_left.containLog(true, "attackTime=%f m_force->numresource=%d \n", attackTime, m_force->resources());
        force.logResources(true, m_left);
        throw INVALID_RESOURCE_TIME_ERROR;
    }

    // Create the right flank
    //attackTime = m_force->firstArrival( RightFlank );
    //m_right = new Contain( reportSize, reportRate, lwRatio,  distStep,
    //    RightFlank, force, attackTime, tactic, attackDist );

    // How big do the arrays need to be?
    //Carmi commented out - m_right is not being initialized anymore 
    //m_size = ( m_right ) ? 2 * m_maxSteps : m_maxSteps;
    //allocate an extra so we don't go out of bounds on the arrays
    m_size = m_maxSteps + 1;

    // Array of attack point angles (radians) at each simulation step.
    //m_u = new double[m_size];
    //checkmem( __FILE__, __LINE__, m_u, "double m_u", m_size );
    m_u.resize(m_size);
    // Array of free-burning fire head positions (ch) at each simulation step.
    //m_h = new double[m_size];
    //checkmem( __FILE__, __LINE__, m_h, "double m_h", m_size );
    m_h.resize(m_size);
    // Array of attack point x coordinates (ch) at each simulation step.
    //m_x = new double[m_size];
    //checkmem( __FILE__, __LINE__, m_x, "double m_x", m_size );
    m_x.resize(m_size);
    // Array of attack point y coordinates (ch) at each simulation step.
    //m_y = new double[m_size];
    //checkmem( __FILE__, __LINE__, m_y, "double m_y", m_size );
    m_y.resize(m_size);
    // Array of area under the perimeter curve (ch2) burned at each sim step.
    //m_a = new double[m_size];
    //checkmem( __FILE__, __LINE__, m_a, "double m_a", m_size );
    m_a.resize(m_size);
    // Array of fireline perimeter (ch) constructed at each simulation step.
    //m_p = new double[m_size];
    //checkmem( __FILE__, __LINE__, m_p, "double m_p", m_size );
    m_p.resize(m_size);
    return;
}

//------------------------------------------------------------------------------
/*! \brief Access to the parallel attack distance from the fire perimeter.

    \return Parallel attack distance from the fire perimeter (ch).
 */

double Sem::ContainSim::attackDistance( void ) const
{
    return( m_left.attackDistance() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the current containment attack point x-coordinate.

    \return Current containment attack point x-coordinate.
 */

double Sem::ContainSim::attackPointX( void ) const
{
    return( m_left.attackPointX() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the current containment attack point y-coordinate.

    \return Current containment attack point y-coordinate.
 */

double Sem::ContainSim::attackPointY( void ) const
{
    return( m_left.attackPointY() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the initial attack time
    (minutes since the fire report time).

    \return Initial attach time (minutes since the fire report time).
 */

double Sem::ContainSim::attackTime( void ) const
{
    return( m_left.attackTime() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the simulation fire head distance step
    estimated to achieve the target [minSteps..maxSteps] simulation steps.

    \return Simulation fire head distance step (ch).
 */

double Sem::ContainSim::distanceStep( void ) const
{
    return( m_left.distanceStep() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final cost of all used ContainResources
    at the time the fire was contained or escaped containment.

    \return Final cost of all resources used
    at time of containment or escape.
 */

double Sem::ContainSim::finalFireCost( void ) const
{
    return( m_finalCost );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final containment line constructed
    at the time the fire was contained or escaped containment.

    \return Final containment line constructed
    at time of containment or escape (ac).
 */

double Sem::ContainSim::finalFireLine( void ) const
{
    return( m_finalLine );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final fire perimeter length
    at the time the fire was contained or escaped containment.

    \return Final fire perimeter length at time of containment or escape (ch).
 */

double Sem::ContainSim::finalFirePerimeter( void ) const
{
    return( m_finalPerim );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final fire size
    at the time the fire was contained or escaped containment.

    \return Final fire size at time of containment or escape (ac).
 */

double Sem::ContainSim::finalFireSize( void ) const
{
    return( m_finalSize );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final containment area
    at the time the fire was contained or escaped containment.

    \return Final containment area at time of containment or escape (ac).
 */

double Sem::ContainSim::finalFireSweep( void ) const
{
    return( m_finalSweep );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final elapsed time since report
    at the time the fire was contained or escaped containment.

    \return Final fire elapsed time since report
    at time of containment or escape (ch).
 */

double Sem::ContainSim::finalFireTime( void ) const
{
    return( m_finalTime );
}

//------------------------------------------------------------------------------
/*! \brief Access to the final number of resources used.

    \return Number of resources used at time of containment or escape (ch).
 */

int Sem::ContainSim::finalResourcesUsed( void ) const
{
    return( m_used );
}

//------------------------------------------------------------------------------
/*! \brief Prints final fire and containment statistics.

    Called at the end of run().
 */

void Sem::ContainSim::finalStats( void )
{
    // Final time
    m_finalTime = m_left.m_currentTime;//m_time; // MAF
    // So far we know the containment area and line constructed
    m_finalPerim = m_finalLine;
    m_finalSize = 0.;
    if ( m_left.m_status == Sem::Contain::Contained )
    {
        m_finalSize = m_finalSweep;
    } else {
        //Zero out all final size related items, those are not valid when we have an ESL
        m_finalSize=m_finalSweep;
        m_finalPerim=0;
       //m_finalLine=0;
        m_finalSweep=0;
    }
    // Resources used
    for ( int res=0; res<m_force->resources(); res++ )
    {
        if ( m_force->resourceArrival( res ) < m_finalTime )
        {
            m_used++;
            m_finalCost += m_force->resourceCost( res, m_finalTime ) ;
        }
    }
    m_left.containLog(false, "=== end simulation=== \n" );
    
    return;
}
 

//------------------------------------------------------------------------------
/*! \brief Access to the fire back position at initial attack.

    \return Fire back spread distance at time of initial attack (ch).
 */

double Sem::ContainSim::fireBackAtAttack( void ) const
{
    return( m_left.fireBackAtAttack() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire back position at report time.

    \return Fire back spread distance at time of report (ch).
 */

double Sem::ContainSim::fireBackAtReport( void ) const
{
    return( m_left.fireBackAtReport() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire head position at initial attack.

    \return Fire head spread distance at time of initial attack (ch).
 */

double Sem::ContainSim::fireHeadAtAttack( void ) const
{
    return( m_left.fireHeadAtAttack() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire head position at report time.

    \return Fire head spread distance at time of report (ch).
 */

double Sem::ContainSim::fireHeadAtReport( void ) const
{
    return( m_left.fireHeadAtReport() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the array of free-burning fire head positions
    during the simulation.

    \note Call firePoints() to determine the array size.

    \return Pointer to the array of free-burning fire head positions
    during the simulation (radians).
 */

std::vector<double> Sem::ContainSim::fireHeadX( void ) const
{
    return( m_h );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire length-to-width ratio the time of report.

    \return Fire length-to-width ratio at the time of report (ch/ch)
 */

double Sem::ContainSim::fireLwRatioAtReport( void ) const
{
    return( m_left.fireLwRatioAtReport() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the array of fire perimeter x-coordinates
    during the simulation.

    \note Call firePoints() to determine the array size.

    \return Pointer to the array of fire perimeter x-coordinates
    during the simulation.
 */

std::vector<double> Sem::ContainSim::firePerimeterX( void ) const
{
    return( m_x );
}

//------------------------------------------------------------------------------
/*! \brief Access to the array of fire perimeter x-coordinates
    during the simulation.

    \note Call firePoints() to determine the array size.

    \return Pointer to the array of fire perimeter x-coordinates
    during the simulation.
 */

std::vector<double> Sem::ContainSim::firePerimeterY( void ) const
{
    return( m_y );
}

//------------------------------------------------------------------------------
/*! \brief Access to the size of the fire perimeter and head array.

    \return Size of the fire perimeter and head arrays.
 */

int Sem::ContainSim::firePoints( void ) const
{
    return( m_size );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire elapsed time from ignition to report.

    \return Elapsed time from fire ignition to fire report (min).
 */

double Sem::ContainSim::fireReportTime( void ) const
{
    return( m_left.fireReportTime() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire backing spread rate at the time of report.

    \return Fire backing spread rate at the time of report (ch/hr).
 */

double Sem::ContainSim::fireSpreadRateAtBack( void ) const
{
    return( m_left.fireSpreadRateAtBack() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire size at the time of report.

    \return Fire size at the time of report (ac).
 */

double Sem::ContainSim::fireSizeAtReport( void ) const
{
    return( m_left.fireSizeAtReport() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the fire spread rate at the time of report.

    \return Fire spread rate at the time of report (ch/hr).
 */

double Sem::ContainSim::fireSpreadRateAtReport( void ) const
{
    return( m_left.fireSpreadRateAtReport() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainForce assigned to the fire.

    \return Pointer to the ContainForce assigned to the fire.
 */

Sem::ContainForce* Sem::ContainSim::force( void ) const
{
    return( m_left.force() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the maximum number of simulation steps.

    \return Maximum number of simulation steps.
 */

int Sem::ContainSim::maximumSimulationSteps( void ) const
{
    return( m_maxSteps );
}

//------------------------------------------------------------------------------
/*! \brief Access to the minimum number of simulation steps.

    \return Minimum number of simulation steps.
 */

int Sem::ContainSim::minimumSimulationSteps( void ) const
{
    return( m_minSteps );
}

//------------------------------------------------------------------------------
/*! \brief Runs the simulation to completion.
  
    The simulation is completed whenever:
    - the containment resources are overrun,
    - the fire is contained, or
    - all containment resources are exhausted.
 */

void Sem::ContainSim::run( void )
{
    // Status names
    const char *StatusName[] =
    {
        "Unreported",
        "Reported",
        "Attacked",
        "Contained",
        "Overrun",
        "Exhausted",
        "Sim Overflow",
        "Size Limit Exceeded",
        "Time Limit Exceeded"
    };
    
   	
 	
    const char *TacticName[] =
    {
        "Head",
        "Rear"
    };
    double at, elapsed, factor;

    // Log levels : 0=none, 1=major events, 2=stepwise
    int logLevel = 0;
    // Repeat simulation until [m_minSteps::m_maxSteps] steps achieved,
    // or if retry==TRUE, until sufficient resources are able to contain fire
    double area, dx, dy, suma, sumb, sumDT;
    double totalArea;
//    double maxArea = 500.0;
    bool rerun = true;
    bool MAXSTEPS_EXCEEDED=false;
    m_pass = 0;
    
    
    while ( rerun )
    {
        m_left.containLog( ( logLevel >= 1 ), "\nPass %d Begins:\n", m_pass );
        // Simulate until forces overrun, fire contained, or maxSteps reached
        int iLeft = 0;              // First index of left half values
        m_u[iLeft] = m_left.m_u;
        m_h[iLeft] = m_left.m_h;
        m_x[iLeft] = m_left.m_x;
        m_y[iLeft] = m_left.m_y;
        //int iRight = m_maxSteps;  // First index of right half values
        elapsed = m_left.m_attackTime;
        m_left.containLog( ( logLevel == 2 ),
            "%d: u=%12.10f,  h=%12.10f,  t=%f\n",
            iLeft, m_left.m_u, m_left.m_h, elapsed );

        // This is the main simulation loop!
        m_finalSweep = m_finalLine = m_finalPerim = 0.0;
        totalArea=0.0;
        suma = sumb = sumDT = 0.0;
        while ( m_left.m_status != Sem::Contain::Overrun
             && m_left.m_status != Sem::Contain::Contained
             && m_left.m_step    < m_maxSteps
             && totalArea <  m_maxFireSize
             && m_left.m_currentTime < m_maxFireTime		 		// MAF
             && m_left.m_currentTime < m_left.m_exhausted)		// MAF
        {
            // Store angle and head position in the proper array element
            m_left.step();

            // Store the new angle, head position, and coordinate values
            iLeft++;
                       
            m_u[iLeft] = m_left.m_u;
            m_h[iLeft] = m_left.m_h;
            m_x[iLeft] = m_left.m_x;
            m_y[iLeft] = m_left.m_y;
            elapsed = m_left.m_currentTime;//m_time; // MAF
//            m_left->containLog( (logLevel == 2 ),
//                "%d: u=%12.10f,  h=%12.10f,  t=%12.10f\n",
//                iLeft, m_u[iLeft], m_h[iLeft], elapsed );
            // Update the extent
            m_xMin = ( m_x[iLeft] < m_xMin ) ? m_x[iLeft] : m_xMin;
            m_xMax = ( m_x[iLeft] > m_xMax ) ? m_x[iLeft] : m_xMax;
            m_yMax = ( m_y[iLeft] > m_yMax ) ? m_y[iLeft] : m_yMax;

            // Line constructed and area swept during this simulation step
            dy = fabs( m_y[iLeft-1] - m_y[iLeft] );
            dx = fabs( m_x[iLeft-1] - m_x[iLeft] );
            m_p[iLeft-1] = sqrt( ( dy * dy ) + ( dx * dx ) );
            // Accumulate line constructed for BOTH flanks (ch)
            m_finalLine += 2.0 * m_p[iLeft-1];
            // Accumulate area of containment (apply trapazoidal rule)
            suma += ( m_y[iLeft-1] * m_x[iLeft] );
            sumb += ( m_x[iLeft-1] * m_y[iLeft] );
            area = ( suma > sumb )
                 ? ( 0.5 * ( suma - sumb ) )
                 : ( 0.5 * ( sumb - suma ) );
			
			// Calculate the area using the trapizoidal rule
			sumDT = 0;
			for ( int i = 1; i <= iLeft; i++ )	
				sumDT = (m_x[i] - m_x[i-1]) * (m_y[i] + m_y[i-1]) + sumDT;

			if ( sumDT < 0 )
				sumDT = -1.0 * sumDT;
				
			area = sumDT * .5;
			
			// Add in the area for the uncontained portion of the fire DT 1/2013
			double UCarea = UncontainedArea( m_h[iLeft], fireLwRatioAtReport(), m_x[iLeft], m_y[iLeft], tactic() );
			area = area + UCarea;
			
            // Accumulate area for BOTH flanks (ac)
            m_a[iLeft-1] = 0.2 * area;
            totalArea = m_a[iLeft-1];
            m_left.containLog( (logLevel == 2 ),
                "%d: u=%12.10f,  h=%12.10f,  x=%12.10f, y=%12.10f, t=%12.10f, UCA=%12.10f, CA=%12.1f, TA=%12.10f, TP=%12.10f\n",
                iLeft, m_u[iLeft], m_h[iLeft], m_x[iLeft], m_y[iLeft], elapsed, UCarea*0.2, (area-UCarea)*0.2, totalArea, m_finalLine );
        }
        // BEHAVEPLUS FIX: Adjust the last x-coordinate for contained head attacks
        if ( m_left.m_status == Sem::Contain::Contained
          && m_left.m_tactic == Sem::Contain::HeadAttack )
        {
            m_x[m_left.m_step] -= 2. * m_left.m_attackDist;
		}

        suma += ( m_y[m_left.m_step] * m_x[0] );
        sumb += ( m_x[m_left.m_step] * m_y[0] );
        m_finalSweep = ( suma > sumb )
                     ? ( 0.5 * ( suma - sumb ) )
                     : ( 0.5 * ( sumb - suma ) );
        m_finalSweep *= 0.20;

		// Calculate the area using the trapizoidal rule
		sumDT = 0;
		for ( int i = 1; i <= m_left.m_step; i++ )
			sumDT = (m_x[i] - m_x[i-1]) * (m_y[i] + m_y[i-1]) + sumDT;

		if ( sumDT < 0 )
			sumDT = -1.0 * sumDT;
				
		area = sumDT * .5;

		// Add in the area for the uncontained portion of the fire DT 1/2013
		double UCarea = UncontainedArea( m_h[m_left.m_step], fireLwRatioAtReport(), m_x[m_left.m_step], m_y[m_left.m_step], tactic() );
		area = area + UCarea;
			
        // Accumulate area for BOTH flanks (ac)
        m_finalSweep = 0.2 * area;

        // Cases 1-3: forces are overrun by fire...
        if ( m_left.m_status == Sem::Contain::Overrun )
        {
            // Case 1: No retry allowed, simulation is complete
            if ( ! m_retry )
            {
                rerun = false;
                m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d Result 1: Overrun\n"
                    "    - resources overrun at %3.1f minutes (%d steps)\n"
                    "    - re-run is FALSE\n"
                    "    - FIRE ESCAPES at %3.1f minutes\n",
                    m_pass, elapsed, m_left.m_step, elapsed );
            }
            // Case 2: Try initial attack after more forces have arrived
            else if ( ( at = m_force->nextArrival( m_left.m_attackTime,
                m_left.m_exhausted, LeftFlank ) ) > 0.01 )
            {
                m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d Result 2: Retry\n"
                    "    - resources overrun at %3.1f minutes (%d steps)\n"
                    "    - Pass %d will wait for IA until %3.1f minutes\n"
                    "    - when line building rate will be %3.2f ch/h\n"
                    "    - RE-RUN\n",
                    m_pass, elapsed, m_left.m_step, m_pass+1,
                    at, m_force->productionRate( at, LeftFlank ) );
                m_pass++;
                m_left.m_attackTime = at;
                m_left.reset();
                rerun = true;
            }
            // Case 3: All resources exhausted
            else
            {
                // No more forces available, so we're done
                rerun = false;
                m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d Result 3: Exhausted\n"
                    "    - resources exhausted at %3.1f minutes (%d steps)\n"
                    "    - FIRE ESCAPES at %3.1f minutes\n",
                    m_pass, elapsed, m_left.m_step, elapsed );
                m_left.m_status = Sem::Contain::Exhausted;
            }
        }
        
        // New Case 3: to set rerun to false when the outrun fires are 
        // removed  DT 7/8/10
        else if (m_left.m_currentTime >= m_left.m_exhausted)
        {
                // No more forces available, so we're done
                rerun = false;
                m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d Result 3: Exhausted\n"
                    "    - resources exhausted at %3.1f minutes (%d steps)\n"
                    "    - FIRE ESCAPES at %3.1f minutes\n",
                    m_pass, elapsed, m_left.m_step, elapsed );
                m_left.m_status = Sem::Contain::Exhausted;
        }
        
        // Case 4: maximum number of steps was exceeded
        // (should never happen as long as m_distStep is calculated from
        // m_exhausted, m_reportRate, ... )
        else if ( iLeft >= m_maxSteps )
        {
            // Make the distance step size bigger and rerun the simulation
            // MAF 9/29/2010, remove factor calc, just reverse previous factor and redo
		  //factor = (double) m_maxSteps / (double) m_minSteps;
		  factor = 2.0;	
            m_left.containLog( ( logLevel >= 1 ),
                "Pass %d Result 4: Less Precision\n"
                "    - fire uncontained at %f minutes\n"
                "    - %d steps exceeds maximum of %d steps\n"
                "    - increasing Eta from %f to %f chains for next Pass %d\n"
                "    - RE-RUN\n",
                m_pass, elapsed, m_left.m_step, m_maxSteps,
                m_left.m_distStep, (m_left.m_distStep*factor), m_pass+1 );
            m_left.m_distStep *= factor;
            m_pass++;
            
		  if(MAXSTEPS_EXCEEDED==false)
		  {	m_left.reset();
			rerun = true;
		  }
		  else
			rerun=false;
		  MAXSTEPS_EXCEEDED=true;
        }
        // Cases 5-6: fire is contained...
        else if ( m_left.m_status == Sem::Contain::Contained )
        {
            // Case 5: there were insufficient simulation steps...
            if (  iLeft < m_minSteps && MAXSTEPS_EXCEEDED==false) // MAF 9/29/2010 added MAXSTEPS_EXCEEDED check
            {
                // Make the distance step size smaller and rerun the simulation
                // Need to make sure that with the new smaller step we will not
                // exceed the MAX steps - otherwise we end up looping
                // Diane 08/10 decrease the step size at a slower rate  
                factor = 0.5 ; // (double) ( m_left->m_step + 1 ) / ((double) m_minSteps*1.25);
                m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d Result 5: More Precision\n"
                    "    - fire contained at %3.1f minutes\n"
                    "    - %d steps is less than minimum of %d steps\n"
                    "    - decreasing Eta from %f to %f chains for Pass %d\n"
                    "    - RE-RUN\n",
                    m_pass, elapsed, m_left.m_step, m_minSteps,
                    m_left.m_distStep, (m_left.m_distStep * factor), m_pass+1 );
                m_left.m_distStep *= factor;
                m_pass++;
                
                // Diane 08/10 decrease the step size at a slower rate 
                //if(m_pass<10)
                //{
                	m_left.reset();
                	rerun = true;
                //}
                //else
                //	rerun=false;
            }
            // Case 6: fire contained within the simulation step range
            else
            {
                m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d Result 6: Contained\n"
                    "    - FIRE CONTAINED at %3.1f minutes (%d steps)\n",
                    m_pass, elapsed, m_left.m_step );
                rerun = false;
            }
        }
        //Add check for maximum Area
        else if(totalArea >= m_maxFireSize){    	        
                at = m_force->nextArrival( m_left.m_attackTime,
                m_left.m_exhausted, LeftFlank );
        /*//////////////////////////////////////////////////////////////////////////
        	Removed DT 6/2010 Stop when fire exceeds maximum size
                if (at > .01){
                	rerun = true;
                	m_left->m_attackTime = at;
                	m_left->containLog( ( logLevel >= 1 ),
                    "Pass %d Result max area exceeded: Retry\n"
                    "    - Maximum Area of %d exceeded at %3.1f minutes (%d steps)\n"
                    "    - Pass %d will wait for IA until %3.1f minutes\n"
                    "    - when line building rate will be %3.2f ch/h\n"
                    "    - RE-RUN\n",
                    m_pass,m_maxFireSize, elapsed, m_left->m_step, m_pass+1,
                    at, m_force->productionRate( at, LeftFlank ) );
               	 	m_pass++;               
                	m_left->reset();               	
                }else{
         */
//                	cout << m_pass << " "  << totalArea << " " <<m_maxFireSize << " " << "\n";
 					m_left.containLog( ( logLevel >= 1 ),
                    "Pass %d total fire size of %3.2f acres exceeds max fire size of %d acres at time %3.1f minutes\n",                    
                    m_pass, totalArea, m_maxFireSize, elapsed);               	
                	rerun = false;
                	//Production rate is not longer increasing
                	m_left.m_status = Sem::Contain::SizeLimitExceeded;
 //               } 
       
        }
		// time limit exceeded
		//------------------------------------------------------------------
		//  MAF 6/2010
		//------------------------------------------------------------------
		else if(((m_left.m_currentTime) > (m_maxFireTime-1)))
			 {     m_left.m_currentTime=m_maxFireTime;
    	           m_left.m_status = Sem::Contain::TimeLimitExceeded;

				   rerun=false;
			 }
        // Case 7: anything else (should never get here!)...
        else
        {
            m_left.containLog( ( logLevel >= 1 ),
                "Pass %d Result 7:\n"
                "    - unknown condition at %3.1f minutes (%d steps)\n"
                "    - RE-RUN\n",
                m_pass, elapsed, m_left.m_step );
            rerun = true;
        }
    }
    // Special case for contained head tactic with non-zero offset
    if ( m_left.m_status == Sem::Contain::Contained
      && m_left.m_tactic == Sem::Contain::HeadAttack
      && m_left.m_attackDist > 0.01 )
    {
    }
    
    //special case for time limit
    //if the time is greater than the max time, then the fire escapes
    //all resources that arrive prior to the max time are considered used
    //always subtract 1 minute from the fire time, we don't get a correct state
    //otherwise because the simulation forces all resources to end work before the
    //fire time limit is reached
    //if ((m_left->m_time) > (m_maxFireTime-1)) {
    //	m_left->m_time=m_maxFireTime;
    //	m_left->m_status = Sem::Contain::TimeLimitExceeded;
    //}
    
    //------------------------------------------------------------------
    //  MAF 6/2010
    //------------------------------------------------------------------
    if ((m_left.m_currentTime) > (m_maxFireTime-1)) {
     	m_left.m_currentTime=m_maxFireTime;
     	m_left.m_status = Sem::Contain::TimeLimitExceeded;
     }
     
    // Simulation complete: display results
    finalStats();
    m_left.containLog( ( logLevel > 0 ),
        "\n    Pass %d Step Size  : %f ch\n", m_pass, m_left.m_distStep );
    m_left.containLog( ( logLevel > 0 ),
        "    Tactic            : %8s\n", TacticName[m_left.m_tactic] );
    m_left.containLog( ( logLevel > 0 ),
        "    Simulation Steps  : %8d\n", m_left.m_step+1 );
    m_left.containLog( ( logLevel > 0 ),
        "    Simulation Time   : %8.2f min\n", m_finalTime );
    m_left.containLog( ( logLevel > 0 ),
        "    Simulation Result : %s\n", StatusName[m_left.m_status] );
    m_left.containLog( ( logLevel > 0 ),
        "    Containment Line  : %8.4f ch\n", m_finalLine );
    m_left.containLog( ( logLevel > 0 ),
        "    Containment Size  : %8.4f ac\n", m_finalSize );
    m_left.containLog( ( logLevel > 0 ),
        "    Resources Used    : %8d\n", m_used );
    m_left.containLog( ( logLevel > 0 ),
        "    Resource Cost     : %8.0f\n\n", m_finalCost );
    return;
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

Sem::Contain::ContainStatus Sem::ContainSim::status( void ) const
{
    return( m_left.status() );
}

//------------------------------------------------------------------------------
/*! \brief Access to the attack tactic.

    \return Attack tactic Sem::ContainTactic
        - HeadAttack = 0
        - RearAttack = 1
 */

Sem::Contain::ContainTactic Sem::ContainSim::tactic( void ) const
{
    return( m_left.tactic() );
}

// Calculate the size of the uncontained portion of the fire ellipse  by DT 1/2013
double Sem::ContainSim::UncontainedArea( double head, double lwRatio, double x, double y, Sem::Contain::ContainTactic tactic )
{
        if( lwRatio < 1.0 )
            lwRatio = 1.00000001;
	// Calculate the semi-major and semi-minor axis lengths
	double ecc = sqrt( 1 - ( 1 / (lwRatio*lwRatio) ) );		// eccentricity

	double a = head / ( 1 + ecc );

	double b = a / lwRatio;

	// Calculate the angle from the center of the ellipse
	double xcenter = x - ( a * ecc );

	// If the x value is off the ellipse put it back on - the line built is greater than the need
	if ( xcenter < -a  )	{
		xcenter = -a;
		y=0;
	}
	if ( xcenter > a )	{
		xcenter = a;
		y=0;
	}
	
	// calculating the angle
	double r = sqrt( xcenter*xcenter + y*y);
	double theta = 0; 

	if ( xcenter >= 0 )
		theta = acos( xcenter / r );

	else	{	// xcenter < 0
		double thetaA = acos( -1 * xcenter / r );
		theta = 3.141592654 - thetaA;
	}
	
	// Calculate the ellipse sector for the values
	double sintheta = sin( 2 * theta );
	double costheta = cos( 2 * theta );
	
	double arctanof = (((b-a)*sintheta) / ((a+b)+(b-a)*costheta));
		
	double ellsector = (a*b/2) * (theta - atan(arctanof));
	
	double area = 0.0;

	// Add in the triangle to complete the area if scenter is less than 0 else subtract the triangle
	area = ellsector - ( xcenter * y / 2 );

	// If the tactic is head then need to use the complement area
	if ( tactic == Sem::Contain::HeadAttack )
		area = 3.141592654 * a * b / 2 - area;
	
	// If the area calculated is less than 0 set it to 0
	if ( area < 0 )
		area = 0;

	// Return area in chains sq will be converted to acres
	return area;
}

//------------------------------------------------------------------------------
//  End of ContainSim.cpp
//------------------------------------------------------------------------------

