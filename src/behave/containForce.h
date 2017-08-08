//------------------------------------------------------------------------------
/*! \file ContainForce.h
    \author Copyright (C) 2006 by Collin D. Bevins.
    \license This is released under the GNU Public License 2.
    \brief Collection of all ContainResources dispatched to the fire.
 */

#ifndef _CONTAINFORCE_H_INCLUDED_
#define _CONTAINFORCE_H_INCLUDED_

// Custom include files
#include "Contain.h"
#include "ContainResource.h"
#include <string>
#include <vector>

namespace Sem
{

// Forward class references
class Contain;

//------------------------------------------------------------------------------
/*! \class ContainForce ContainForce.h
    \brief Collection of all ContainResources dispatched to the fire.
 */

class ContainForce
{
// Class version
    static const int containForceVersion = 1;   //!< Class version

// Public methods
public:
    // Custom constructors
    ContainForce( int maxResources=250 ) ;
    // Virtual destructor
    virtual ~ContainForce( void ) ;
    // Add ContainResource into ContainForce
    int addResource( ContainResource& resource ) ;
    // Construct ContainResource into ContainForce
    int addResource(
        double arrival,
        double production,
        double duration=480.,
        ContainFlank::ContainFlankEnum flank=ContainFlank::LeftFlank,
        std::string desc="",
        double baseCost=0.0,
        double hourCost=0.0 );
    int removeResourceAt(int index);
    int removeResourceWithThisDesc(std::string desc);
    void clearResourceVector();

    // Force-level access methods
    double exhausted(ContainFlank::ContainFlankEnum  flank ) const ;
    double firstArrival(ContainFlank::ContainFlankEnum  flank ) const ;
    double nextArrival( double after, double until, ContainFlank::ContainFlankEnum  flank ) const ;
    double productionRate( double minutesSinceReport, ContainFlank::ContainFlankEnum  flank ) const ;
    
    //for debug
    void   logResources(bool debug,const Contain*) const ;

    // Public access to individual ContainResources
    int     resources( void ) const ;
    double  resourceArrival( int index ) const ;
    double  resourceBaseCost( int index ) const ;
    double  resourceCost( int index, double finalTime ) const ;
    std::string resourceDescription( int index ) const ;
    double  resourceDuration( int index ) const ;
    ContainFlank::ContainFlankEnum  resourceFlank( int index ) const ;
    double  resourceHourCost( int index ) const ;
    double  resourceProduction( int index ) const ;

// Protected data
protected:
    //ContainResource **m_cr;     //!< Array of pointers to ContainResources
    std::vector<ContainResource> m_resourceVector;

    int     m_maxSize;         

friend class Contain;
};

}   // End of namespace Sem

#endif

//------------------------------------------------------------------------------
//  End of ContainForce.h
//------------------------------------------------------------------------------

