//------------------------------------------------------------------------------
/*! \file ContainForce.cpp
    \author Copyright (C) 2006 by Collin D. Bevins.
    \license This is released under the GNU Public License 2.
    \brief Collection of all ContainResources dispatched to the fire.
 */

// Local include files
#include "ContainSim.h"         // for checkmem()
#include "ContainForce.h"
#include <math.h>

/* Silence warnings that aren't our fault */
#if defined(OMFFR) && defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC push
#endif /* defined(OMFFR) && defined(__GNUC__) */

#ifdef OMFFR
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC push
#endif /* OMFFR */


//include "Logger.h"

//------------------------------------------------------------------------------
/*! \brief ContainForce constructor.
    
    \param[in] maxResources Maximum number of containment resources allowed.
 */

Sem::ContainForce::ContainForce( int maxResources ) :
    m_cr(0),
    m_size(maxResources),
    m_count(0)
{
    // Allocate ContainResource pointer array.
    m_cr = new ContainResource *[m_size];
    ContainSim::checkmem( __FILE__, __LINE__, m_cr, "ContainResource", m_size );
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainForce class destructor.
 */

Sem::ContainForce::~ContainForce( void )
{
    for ( int i=0; i<m_count; i++ )
    {
        delete m_cr[i];  m_cr[i] = 0;
    }
    delete[] m_cr;      m_cr = 0;
    return;
}

//------------------------------------------------------------------------------
/*! \brief Determines when all the containment resources will  be exhausted.
  
    \param[in] flank One of LeftFlank or RightFlank.
  
    \return Time when all scheduled resources will be exhausted
            (minutes since fire report).
 */

double Sem::ContainForce::exhausted( ContainFlank flank ) const
{
    double at = 0.;
    double done;
    for ( int i=0; i<m_count; i++ )
    {
        if ( m_cr[i]->m_flank == flank || m_cr[i]->m_flank == BothFlanks )
        {
            if ( ( done = m_cr[i]->m_arrival + m_cr[i]->m_duration ) > at )
            {
                at = done;
            }
        }
    }
    return( at );
}

//------------------------------------------------------------------------------
/*! \brief Determines first resource arrival time for the specified flank.
  
    \param flank One of LeftFlank or RightFlank.
  
    \return Time of first resource arrival on the specified flank
            (minutes since fire report).
 */

double Sem::ContainForce::firstArrival( ContainFlank flank ) const
{
    double at = 99999999.;
    for ( int i=0; i<m_count; i++ )
    {
        if ( ( m_cr[i]->m_flank == flank || m_cr[i]->m_flank == BothFlanks )
          && m_cr[i]->m_arrival < at )
        {
            at = m_cr[i]->m_arrival;
        }
    }
    return( at );
}

//------------------------------------------------------------------------------
/*! \brief Determines time of next productivity increase (usually the next
    resource arrival time) for the specified flank.  The search is restricted
    to the \b after and \b until time range.
  
    \param[in] after Find next resource arrival AFTER this time
                     (minutes since fire report).
    \param[in] until Find next resource arrival BEFORE this time
                     (minutes since fire report).
    \param[in] flank One of LeftFlank or RightFlank.
  
    \return Time of next resource arrival on the specified flank \a after
    the specified time (minutes since fire report).
 */

double Sem::ContainForce::nextArrival( double after, double until,
        ContainFlank flank ) const
{
    // Get the production rate at the requested time
    double prodRate = productionRate( after, flank );
    // Look for next production boost starting at the next minute
    int it = (int) after;
    after = (double) it + 1.;
    while ( after < until )
    {
        // Check production rate at the next minute
        if ( fabs(( productionRate( after, flank ) - prodRate )) > 0.001 )
        {
            return( after );
        }
		// Try the next minute
        after += 1.;
    }
    // No more productivity boosts after this time
    return( 0.0 );
}

//------------------------------------------------------------------------------
/*! \brief Adds a ContainResource to the ContainForce.
  
    \param[in] resource Pointer to the COntainResource to be added.

    \return Pointer to the new ContainResource object.
 */

Sem::ContainResource *Sem::ContainForce::addResource(
        Sem::ContainResource* resource )
{
	#define EXTRA_ALLOCATION 100
    // Check for vector space 
    if ( m_count >= m_size )
    {
        //need to reallocate memory - give some amount to grow
        int newsize=m_size+EXTRA_ALLOCATION;
        ContainResource ** cr_tmp = new ContainResource *[newsize];  
        if (cr_tmp) {
        	//copy to new place, and reassign 
        	memcpy(cr_tmp,m_cr,sizeof(ContainResource*)*m_size);
        	m_cr=cr_tmp;
        	m_size = newsize;
        } else {
           ContainSim::checkmem( __FILE__, __LINE__, cr_tmp, "ContainResource", newsize );          
           return NULL; //throw Exception("Out of Memory Exception");          
        }        	         
    }
    // Add the new record to the vector and return.
    m_cr[m_count++] = resource;
    return( resource );
}

//------------------------------------------------------------------------------
/*! \brief Adds a new ContainResource to the ContainForce.
  
    \param[in] arrival    Fireline production begins at this elapsed time since
                          the fire was \b reported (min).
    \param[in] production Sustained rate of holdable fireline production (ch/h).
    \param[in] duration   Amount of time during which the fireline production
                          rate is maintained (min).
    \param[in] flank      One of LeftFlank, RightFlank, BothFlanks, or NeitherFlank.
    \param[in] desc       Resource description or identification (informational
                          only; not used by the program).
    \param[in] baseCost   Resource base (or fixed) cost if deployed to the fire.
    \param[in] hourCost   Resource hourly cost once deployed to the fire.
  
    \return Pointer to the new ContainResource object.
 */

Sem::ContainResource *Sem::ContainForce::addResource(
        double arrival,
        double production,
        double duration,
        ContainFlank flank,
        char * const desc,
        double baseCost,
        double hourCost )
{
    // Create a new ContainResource record.
    ContainResource *resource = new ContainResource( arrival, production,
        duration, flank, desc, baseCost, hourCost );
    ContainSim::checkmem( __FILE__, __LINE__, resource, "ContainResource rec", 1 );
    return( addResource( resource ) );
}

//------------------------------------------------------------------------------
/*! \brief Determines the aggregate fireline production rate along one fire
    flank at the specified time by the entire available containment force.
    THIS IS HALF THE TOTAL PRODUCTION RATE FOR BOTH FLANKS, CALCULATED FROM
    HALF THE TOTAL PRODUCTION RATE FOR EACH AVAILABLE RESOURCE.
  
    \param[in] minSinceReport The fireline aggregate containment force
    production rate is determined for this many minutes since the fire was
    reported.
  
    \param[in] flank One of LeftFlank or RightFlank.
  
    \return Aggregate containment force fireline production rate (ch/h).
 */

double Sem::ContainForce::productionRate( double minSinceReport,
    Sem::ContainFlank flank ) const
{
    double fpm = 0.0;
    for ( int i=0; i<m_count; i++ )
    {
        if ( ( m_cr[i]->m_flank == flank || m_cr[i]->m_flank == BothFlanks )
          && ( m_cr[i]->m_arrival <= ( minSinceReport + 0.001 ) )
          && ( m_cr[i]->m_arrival + m_cr[i]->m_duration ) >= minSinceReport )
        {
            fpm += ( 0.50 * m_cr[i]->m_production );
        }
    }
    return( fpm );
}

//------------------------------------------------------------------------------
/*! \brief API access to the number of ContainResources in the
    containment force.
  
    \return Number of ContainResources in the ContainForce.
 */

int Sem::ContainForce::resources( void ) const
{
    return( m_count );
}

//------------------------------------------------------------------------------
/*! \brief Access to the arrival time of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's arrival time (minutes since fire was
    reported).
 */

double Sem::ContainForce::resourceArrival( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_arrival );
    }
    return( 0.0 );
}

//------------------------------------------------------------------------------
/*! \brief Access to the base cost of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's base cost (cost units).
 */

double Sem::ContainForce::resourceBaseCost( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_baseCost );
    }
    return( 0.0 );
}

//------------------------------------------------------------------------------
/*! \brief Access to the specified ContainmentResouce's cost.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.

    \param[in] finalTime  Final fire time.

    \return ContainResource's total cost on the fire from its scheduled
            arrival time until end of shift or end of fire.
 */

double Sem::ContainForce::resourceCost( int index, double finalTime ) const
{
    if ( index >= 0 && index < m_count )
    {
        // Was this resource deployed?
        if ( finalTime <= m_cr[index]->m_arrival )
        {
            return( 0.0 );
        }
        // Number of hours at the fire
        double minutes = finalTime - m_cr[index]->m_arrival;
        if ( minutes > m_cr[index]->m_duration )
        {
            minutes = m_cr[index]->m_duration;
        }
        return( m_cr[index]->m_baseCost
            + ( m_cr[index]->m_hourCost * minutes / 60. ) );
    }
    return( 0. );
}

//------------------------------------------------------------------------------
/*! \brief Access to the specified ContainmentResouce's description.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's description.
 */

char * Sem::ContainForce::resourceDescription( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_desc );
    }
    return( "" );
}

//------------------------------------------------------------------------------
/*! \brief Access to the duration time of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's duration time (minutes).
 */

double Sem::ContainForce::resourceDuration( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_duration );
    }
    return( 0.0 );
}

//------------------------------------------------------------------------------
/*! \brief Access to the flank (Left, Right, or Both) of the
    specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's flank: Active or Inactive.
 */

Sem::ContainFlank Sem::ContainForce::resourceFlank( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_flank );
    }
    return( NeitherFlank );
}

//------------------------------------------------------------------------------
/*! \brief Access to the hourly cost of the specified ContainmentResouce.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's hourly cost (cost units).
 */

double Sem::ContainForce::resourceHourCost( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_hourCost );
    }
    return( 0.0 );
}

//------------------------------------------------------------------------------
/*! \brief Access to the total holdable fireline production rate of the
    specified ContainmentResouce ON BOTH FLANKS.  The rate for one flank
    is half this amount, since the resource is assumed to be split in two
    and working on both flanks simultaneously.
  
    \param[in] index Index (base 0) of the ContainResource.  Indices are
    assigned in the order that the ContainResources are added to the
    ContainForce.
  
    \return ContainResource's holdable fireline production rate (ch/h).
 */

double Sem::ContainForce::resourceProduction( int index ) const
{
    if ( index >= 0 && index < m_count )
    {
        return( m_cr[index]->m_production );
    }
    return( 0.0 );
}

/*
 * Print all available resources into the log file
 * For debugging purposes
 */
void Sem::ContainForce::logResources(bool debug, const Contain* contain) const {
	char buf[70];
	if (!debug) return;
	for ( int i=0; i<m_count; i++ )
    {
       m_cr[i]->print(buf, 65);
       contain->containLog(true, "resource %d: %s\n",i+1,buf);
    }
}

#if defined(OMFFR) && defined(__GNUC__)
#pragma GCC pop
#endif

//------------------------------------------------------------------------------
//  End of ContainForce.cpp
//------------------------------------------------------------------------------

