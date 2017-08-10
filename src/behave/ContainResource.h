//------------------------------------------------------------------------------
/*! \file ContainResource.h
    \author Copyright (C) 2006 by Collin D. Bevins.
    \license This is released under the GNU Public License 2.
    \brief A single fire containment resource unit that can be dispatched
    to a fire.  Examples include an engine crew, line crew, bulldozer,
    helicopter, airtanker, etc.
 */

#ifndef _CONTAINRESOURCE_H_INCLUDED_
#define _CONTAINRESOURCE_H_INCLUDED_

// Custom files
//#include "ContainForce.h"

namespace Sem
{

// Forward class references
class ContainForce;

//------------------------------------------------------------------------------
/*! \enum ContainFlank
    \brief Identifies the fire flank to which ContainResource objects
    are assigned.
 */
enum ContainFlank
{
    LeftFlank    = 0,   //!< Attack left (upper) flank only (full production)
    RightFlank   = 1,   //!< Attack right (lower) flank only (full production)
    BothFlanks   = 2,   //!< Attack both flanks (half of production per flank)
    NeitherFlank = 3    //!< Attack neither flank (inactive)
};

//------------------------------------------------------------------------------
/*! \class ContainResource ContainResource.h
    \brief A single fire containment resource unit that can be dispatched
    to a fire.  Examples include an engine crew, line crew, bulldozer,
    helicopter, airtanker, etc.
  
    If the resource is assigned to either the LeftFlank or RightFlank,
    its fireline production rate is applied in full to that flank.
    If the resource is assigned to BothFlanks, then half of its fireline
    production rate is applied to each flank.
    If the resource is assigned to NeitherFlank, it is not used.
 */

class ContainResource
{

// Class version
    static const int containResourceVersion = 1;    //!< Class version

// Public interface
public:
    // Default constructor
    ContainResource( void ) ;
    // Custom constructors
    ContainResource(
        double arrival,
        double production,
        double duration=480.,
        Sem::ContainFlank flank=Sem::LeftFlank,
        char * const desc="",
        double baseCost=0.00,
        double hourCost=0.00 );
    // Virtual destructor
    virtual ~ContainResource( void ) ;

    // Access methods
    double arrival( void ) const ;
    double baseCost( void ) const ;
    char * description( void ) const ;
    double duration( void ) const ;
    Sem::ContainFlank flank( void ) const ;
    double hourCost( void ) const ;
    double production( void ) const ;
    void   print(char buf[], int buflen) const;

// Protected properties
protected:
    double  m_arrival;          //!< Time of arrival since fire report (min)
    double  m_duration;         //!< Production rate duration (min)
    double  m_production;       //!< Total fireline production rate on both flanks (ch/h)
    double  m_baseCost;         //!< Base resource cost
    double  m_hourCost;         //!< Hourly resource cost
    Sem::ContainFlank m_flank;  //!< Both, Left, or Right flank attack
    char * m_desc;             //!< Resource description

    friend class ContainForce;
};

}   // End of namespace Sem

#endif

//------------------------------------------------------------------------------
//  End of ContainResource.h
//------------------------------------------------------------------------------

