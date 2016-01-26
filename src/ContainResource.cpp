//------------------------------------------------------------------------------
/*! \file ContainResource.cpp
    \author Copyright (C) 2006 by Collin D. Bevins.
    \license This is released under the GNU Public License 2.
    \brief A single fire containment resource unit that can be dispatched
    to a fire.  Examples include an engine crew, line crew, bulldozer,
    helicopter, airtanker, etc.
 */

#include <stdio.h>

// Local include files
#include "Contain.h"

/* Silence warnings that aren't our fault */
#if defined(OMFFR) && defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC push
#endif /* defined(OMFFR) && defined(__GNUC__) */

#ifdef OMFFR
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC push
#endif /* OMFFR */


//------------------------------------------------------------------------------
/*! \brief ContainResource default constructor.
 */

Sem::ContainResource::ContainResource( void ) :
    m_arrival(0.0),
    m_duration(0.0),
    m_production(0.0),
    m_baseCost(0.0),
    m_hourCost(0.0),
    m_flank(Sem::LeftFlank),
    m_desc("")
{
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainResource constructor.
  
    Describes a single fire containment resource unit that can be dispatched
    to a fire, such as an engine crew, line crew, bulldozer, helicopter,
    airtanker, etc.
  
    \param[in] arrival    Fireline production begins at this elapsed time since
                          the fire was \b reported (min).
    \param[in] production Sustained rate of holdable fireline production (ch/h).
                          THIS IS THE TOTAL RATE FOR BOTH FLANKS.
                          THE PRODUCTION RATE WILL BE SPLIT IN HALF AND APPLIED
                          TO ONE FLANK FOR SIMULATION.
    \param[in] duration   Amount of time during which the fireline production
                          rate is maintained (min).
    \param[in] flank      One of LeftFlank, RightFlank, BothFlanks, or NeitherFlank.
    \param[in] desc       Resource description or identification (informational
                          only; not used by the program).
    \param[in] baseCost   Base cost of deploying the resource to the fire.
    \param[in] hourCost   Hourly cost of the resource while at the fire.
 */

Sem::ContainResource::ContainResource(
        double arrival,
        double production,
        double duration,
        ContainFlank flank,
        char * const desc,
        double baseCost,
        double hourCost ) :
    m_arrival(arrival),
    m_duration(duration),
    m_production(production),
    m_baseCost(baseCost),
    m_hourCost(hourCost),
    m_flank(flank),
    m_desc(desc)
{
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainResource class destructor.
 */

Sem::ContainResource::~ContainResource( void )
{
    return;
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource arrival time at the fire.

    \return Resource arrival time at the fire (minutes since fire ignition).
 */

double Sem::ContainResource::arrival( void ) const
{
    return( m_arrival );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource unit base operating cost.

    \return Resource base operating cost.
 */

double Sem::ContainResource::baseCost( void ) const
{
    return( m_baseCost );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource description.

    \return Resource description.
 */

char * Sem::ContainResource::description( void ) const
{
    return( m_desc );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource production duration period.

    \return Resource production duration period (minutes).
 */

double Sem::ContainResource::duration( void ) const
{
    return( m_duration );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource flank assignment.

    \return Resource ContainFlank flank assignment
        - LeftFlank
        - RightFlank
        - BothFlanks
        - NeitherFlank
 */

Sem::ContainFlank Sem::ContainResource::flank( void ) const
{
    return( m_flank );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource unit hourly operating cost.

    \return Resource hourly operating cost.
 */

double Sem::ContainResource::hourCost( void ) const
{
    return( m_hourCost );
}

//------------------------------------------------------------------------------
/*! \brief Access to the ContainResource containment line production rate.

    \return Resource containment line production rate (ft/min).
 */

double Sem::ContainResource::production( void ) const
{
    return( m_production );
}

void Sem::ContainResource::print(char buf[], int buflen) const {
	char localbuf[100];
	int i=0;
	if (!buf || buflen<1) return;
	memset(buf,0,buflen);
	i=sprintf_s(localbuf,"arrival=%5.2f production=%5.2f duration=%6.1f ", m_arrival, m_production, m_duration);
	i=(i<(buflen-1)? i : (buflen-1));
	memcpy(buf,localbuf,i);
	buf[i]='\0';  
}

/* Silence warnings that aren't our fault */
#if defined(OMFFR) && defined(__GNUC__)
#pragma GCC pop
#endif /* OMFFR */


//------------------------------------------------------------------------------
//  End of ContainResource.cpp
//------------------------------------------------------------------------------

