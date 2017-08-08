//------------------------------------------------------------------------------
/*! \file ContainSim.h
    \author Copyright (C) 2006 by Collin D. Bevins.
    \license This is released under the GNU Public License 2.
    \brief An implementation of Freid and Fried (\ref friedfried1995)
    wildfire containment model.
  
    This is a fire containment algorithm based upon the paper
        Fried, Jeremy S. and Fried, Burton D.  199?.
            Simulating wildfire containment with realistic tactics.
    and heavily adapted from the FCAT Fire Containment Algorithm Test Pascal
    program by Jeremy S. Fried (1991).
  
    Primary assumptions are that the fire is growing under uniform fuel,
    terrain, and weather conditions, resulting in a steadily growing
    elliptical fire (except where contained).
  
    Containment "tactics" may be either head or tail attack.
    Containment production rates apply to a single flank.
    One flank is simulated and the other is considered to be a mirror image.
  
    All times (elapsed, resource arrival, containment time, etc)
    are from when the fire was first \b REPORTED.
  
    Simulation proceeds according to some fixed distance step (to achieve
    the min-max step range) and ends when
    - the fire is contained by 1 or more of the resources,
    - the fire overruns the resources (and there is no retry), or
    - all the resources are exhausted.
  
    Four 4 classes of objects are used
  
    1   The ContainResource Class describes a single fire containment resource
        unit that can be dispatched to the fire, such as an engine crew,
        line crew, bulldozer, helicopter, airtanker, type I crew, etc.
        Attributes include arrival time at the fire, line production rate,
        and production duration.
        There is also a Flank attribute (if the application wishes to use two-
        flank simulation, which hasn't really been tried yet), which can also
        be used to "turn off" a resource by setting it to NeitherFlank.
  
    2   The ContainForce Class is the collection of all ContainResources
        available for use on the fire.
  
    3   The Contain Class includes just the core simulation methods needed
        to make a single simulation pass for a single flank ala Fried.
        It includes the initial fire conditions, attack tactics, simulation
        step size, and attack point at the current time step. The fire is
        assumed to grow in an elliptical shape, e.g., under uniform terrain,
        fuels, and weather throughout the attack.
  
    4   The ContainSim Class has all the attributes to control multiple
        simulation passes (to achieve a desired number of perimeter points or
        to retry an attack after an initial failure), and accumulate perimeter
        points at each simulation step.  It also has two Contain objects,
        one each for the left and right flanks.  Currently only the left flank
        object is used and the right flank is presumed to be a mirror image
        of the left flank.
 */

#ifndef _CONTAINSIM_H_INCLUDED_
#define _CONTAINSIM_H_INCLUDED_

// Custom include files
#include "Contain.h"
#include "ContainForce.h"
#include "ContainResource.h"

namespace Sem
{

//------------------------------------------------------------------------------
/*! \class ContainSim Contain.h
    \brief Fire containment simulation object.
  
    Contains all the information to make a complete simulation run,
    and when completed, display the fire perimeter in Cartesian coordinates.
 */

class ContainSim
{
// Public methods
public:
    // Custom constructor
    ContainSim(
        double reportSize,
        double reportRate,
        double *diurnalROS,
        int fireStartMinutesStartTime,      
        double lwRatio=1.,
        ContainForce *force=0,
        ContainTactic::ContainTacticEnum tactic=ContainTactic::HeadAttack,
        double attackDist=0.,
        bool retry=true,
        int minSteps=250,
        int maxSteps=1000,
        int maxFireSize=1000, 
        int maxFireTime=1080) ;
    // Virtual destructor
    ~ContainSim( void ) ;

    // Access to input properties
    double attackDistance( void ) const ;
    double attackPointX( void ) const ;
    double attackPointY( void ) const ;
    double attackTime( void ) const ;
    double distanceStep( void ) const ;
    double fireBackAtAttack( void ) const ;
    double fireBackAtReport( void ) const ;
    double fireHeadAtAttack( void ) const ;
    double fireHeadAtReport( void ) const ;
    double fireLwRatioAtReport( void ) const ;
    double fireReportTime( void ) const ;
    double fireSizeAtReport( void ) const ;
    double fireSpreadRateAtBack( void ) const ;
    double fireSpreadRateAtReport( void ) const ;
    ContainForce* force( void ) const ;
    int maximumSimulationSteps( void ) const ;
    int minimumSimulationSteps( void ) const ;
    ContainStatus::ContainStatusEnum status( void ) const ;
    ContainTactic::ContainTacticEnum tactic( void ) const ;

    // Access to output properties
    double finalFireCost( void ) const ;
    double finalFireLine( void ) const ;
    double finalFirePerimeter( void ) const ;
    double finalFireSize( void ) const ;
    double finalFireSweep( void ) const ;
    double finalFireTime( void ) const ;
    int    finalResourcesUsed( void ) const ;

    // Access to simulation coordinate array
    double* fireHeadX( void ) const ;
    double* firePerimeterX( void ) const ;
    double* firePerimeterY( void ) const ;
    int     firePoints( void ) const ;

    // Run the simulation!
    void run( void );
    static void checkmem( const char* fileName, int lineNumber, void* ptr,
        const char* type, int size ) ;

	// Calculate the area of the uncontained portion of the ellipse
	double UncontainedArea( double head, double lwRatio, double x, double y, ContainTactic::ContainTacticEnum tactic  );	 // By DT 1/2013

private:
    void finalStats( void ) ;

// Protected data
protected:
    double   m_finalCost;   //!< Final total cost of all resources used
    double   m_finalLine;   //!< Final fire line at containment or escape (ch)
    double   m_finalPerim;  //!< Final line plus fire perimeter at containment or escape (ch)
    double   m_finalSize;   //!< Final fire size at containment or escape (ac)
    double   m_finalSweep;  //!< Final containment area at containment or escape (ac)
    double   m_finalTime;   //!< Containment or escape time since report (min)   
    double   m_xMax;        //!< Maximum X coordinate of constructed line (ch)
    double   m_xMin;        //!< Minimum X coordinate of constructed line (ch)
    double   m_yMax;        //!< Maximum Y coordinate of constructed line (ch)
    double  *m_u;           //!< Array of attack point angles at each distance step
    double  *m_h;           //!< Array of free burning head positions (radians)
    double  *m_x;           //!< Array of perimeter x coordinates (ch)
    double  *m_y;           //!< Array of perimeter y coordinates (ch)
    double  *m_a;           //!< Array of flank area segments (ch2)
    double  *m_p;           //!< Array of flank perimeter segments (ch)
    Contain *m_left;        //!< Left flank Contain object.
    Contain *m_right;       //!< Right flank Contain object.
    ContainForce *m_force;  //!< Containment forces for both flanks
    int      m_minSteps;    //!< Minimum number of simulation distance steps
    int      m_maxSteps;    //!< Maximum number of simulation distance steps
    int      m_size;        //!< Size of the arrays (m_maxSteps or 2*m_maxSteps)
    int      m_pass;        //!< Pass number
    int      m_used;        //!< Number of containment resources deployed
    bool     m_retry;       //!< Retry with later attack time if forces overrun
    int   m_maxFireSize;	//!< Maximum size a fire can burn before it escapes (acres)
    int   m_maxFireTime;     //!< Maximum time a fire can burn before it escapes (minutes)
};

}   // End of namespace Sem

//Error codes
#define INVALID_RESOURCE_TIME_ERROR  "Error: a resource can't have a negative production start time" 


#endif

//------------------------------------------------------------------------------
//  End of Contain.h
//------------------------------------------------------------------------------

