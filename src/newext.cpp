//------------------------------------------------------------------------------
/*! \file newext.cpp
 *  \version BehavePlus3
 *  \author Copyright (C) 2002-2004 by Collin D. Bevins.  All rights reserved.
 *
 *  \brief Part of Mark Finney's EXRATE package for determining expected
 *  and harmonic mean spread rate in randomly arranged fuels.
 *
 *  Original code by Mark Finney.  CDB has renamed all functions and
 *  variables according to BehavePlus3 coding style.  The following files
 *  make up the entire code package:
 *  - newext.cpp
 *  - newext.h
 *  - randfuel.cpp
 *  - randfuel.h
 *  - randthread.cpp
 *  - randthread.h
 */

// Custom include files
#include "newext.h"

// Standard include files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------
/*! \brief Extension default constructor.
*/

Extension::Extension( void )
{
    init();
    return;
}

//------------------------------------------------------------------------------
/*! \brief Extension destructor.
*/

Extension::~Extension( void )
{
    freeExtension();
    return;
}

//------------------------------------------------------------------------------

bool Extension::allocExtension( long p_blocks, long p_cols, long p_rows,
        long p_fuels )
{
    freeExtension();

    m_blocks   = p_blocks;
    m_cols     = p_cols;
    m_rows     = p_rows;
    m_fuels    = p_fuels;
    m_cells    = m_cols * m_rows;
    m_leeFuels = m_fuels * m_fuels;

#ifndef USE_OLD_METHOD
    // Allocate one large block of doubles for the following:
    //  Variable        Doubles                 Pointers
    //  m_combArray     m_blocks * m_cells      m_blocks
    //  m_rosArray      m_blocks * m_cells      m_blocks
    //  m_cuumProb      m_blocks                0
    //  m_maxRosArray   m_blocks                0
    int doubles  = 2 * m_blocks + 2 * ( m_blocks * m_cells );
    int pointers = 2 * m_blocks;
    int blockSize = pointers * sizeof( double * ) + doubles * sizeof( double );
    m_blockPtr = new char[ blockSize ];
    if ( ! m_blockPtr )
    {
        return( false );
    }
    memset( m_blockPtr, 0x0, blockSize * sizeof( char ) );

    int i, len;
    // Start of m_combArray of pointers to doubles
    char *cPtr   = m_blockPtr;
    m_combArray  = (double **) cPtr;
    len          = m_blocks * sizeof( double * );
    cPtr        += len;         // Move to end of pointer array
    // Set pointers to start of each cell block of doubles
    for ( i=0; i<m_blocks; i++ )
    {
        m_combArray[i] = (double *) cPtr;
        cPtr += ( m_cells * sizeof( double ) );
    }

    // Start of m_rosArray of pointers to doubles
    m_rosArray = (double **) cPtr;
    len        = m_blocks * sizeof( double * );
    cPtr      += len;           // Move to end of pointer array
    // Set pointers to start of each cell block of doubles
    for ( i=0; i<m_blocks; i++ )
    {
        m_rosArray[i] = (double *) cPtr;
        cPtr += ( m_cells * sizeof( double ) );
    }

    // Start of m_maxRosArray data array of doubles
    m_maxRosArray = (double *) cPtr;
    len           = m_blocks * sizeof( double );
    cPtr         += len;
    // Start of m_cuumProb data array of doubles
    m_cuumProb    = (double *) cPtr;

    return( true );
#else
    if ( ! ( m_combArray = new double*[m_blocks] ) )
    {
        return( false );
    }

    if ( ! ( m_rosArray = new double*[m_blocks] ) )
    {
        freeExtension();
        return( false );
    }

    for ( long i=0; i<m_blocks; i++ )
    {
        if ( ! ( m_combArray[i] = new double[ m_cells ] ) )
        {
            freeExtension();
            return( false );
        }
        if ( ! ( m_rosArray[i]  = new double[ m_cells ] ) )
        {
            freeExtension();
            return( false );
        }
    }

    if ( ! ( m_cuumProb = new double[ m_blocks ] ) )
    {
        freeExtension();
        return( false );
    }
    memset( m_cuumProb, 0x0, m_blocks * sizeof( double ) );

    if ( ! ( m_maxRosArray = new double[ m_blocks ] ) )
    {
        freeExtension();
        return( false );
    }
    memset( m_maxRosArray, 0x0, m_blocks * sizeof( double ) );

    return( true );
#endif
}

//------------------------------------------------------------------------------
/*! \brief  Calculates the probability of the fuel arrangement occurring
 *  and subtracts the probability of faster combinations from occurring
 *  in outer lateral extensions (m_cuumProb[])
 *
 *  \param BlockNum Index to the sample block to calculate the probability
 *  returns the probability
 */

double Extension::calcProb( long p_block, bool p_subtFaster )
{
    double prob = 1.0;

    for ( long i=0; i<m_cells; i++ )
    {
        prob *= ( (double) m_combArray[p_block][i] );
    }
    prob *= m_latProb;
    if ( p_subtFaster )
    {
        prob -= getFasterProbs(p_block);
    }
    return( prob );
}

//------------------------------------------------------------------------------
/*! \brief Frees all dynamically allocated memory and reset all data to
 *  initial values.
 */

void Extension::freeExtension( void )
{
#ifndef USE_OLD_METHOD
    if ( m_blockPtr )
    {
        delete[] m_blockPtr;
    }
#else
    for ( int i=0; i<m_blocks; i++ )
    {
        if ( m_combArray )
        {
            if ( m_combArray[i] )
            {
                delete[] m_combArray[i];
                m_combArray[i] = 0;
            }
        }
        if ( m_rosArray )
        {
            if ( m_rosArray[i] )
            {
                delete[] m_rosArray[i];
                m_rosArray[i] = 0;
            }
        }
    }
    if ( m_combArray )
    {
        delete[] m_combArray;
        m_combArray = 0;
    }
    if ( m_rosArray )
    {
        delete m_rosArray;
        m_rosArray = 0;
    }
    if ( m_cuumProb )
    {
        delete[] m_cuumProb;
        m_cuumProb = 0;
    }
    if ( m_maxRosArray )
    {
        delete[] m_maxRosArray;
        m_maxRosArray = 0;
    }
#endif
    init();
    return;
}

//------------------------------------------------------------------------------

double Extension::getFasterProbs( long p_block )
{
    double prob = m_cuumProb[p_block];
    Extension *xt = m_nextExt;
    while ( xt != NULL )
    {
        for ( int i=0; i<xt->m_blocks; i++ )
        {
            prob += xt->m_cuumProb[i];
        }
        xt = xt->m_nextExt;
    }
    return( prob );
}

//------------------------------------------------------------------------------

void Extension::init( void )
{
    // Public data
    m_harmonicRos    = 0.0;
    m_averageRos     = 0.0;
    m_extHarmonicRos = 0.0;
    m_extAverageRos  = 0.0;
    m_extCuumProb    = 0.;
    m_nextExt        = 0;
    m_rf             = 0;
    m_maxRosArray    = 0;
    m_combArray      = 0;
    m_rosArray       = 0;

    // Private data
    m_prob           = 0.0;
    m_latProb        = 0.0;
    m_blocks         = 0;
    m_cols           = 0;
    m_rows           = 0;
    m_cells          = 0;
    m_fuels          = 0;
    m_leeFuels       = 0;
    m_cuumProb       = 0;
    m_blockPtr       = 0;
    m_latRosArray    = 0;
    m_latCombArray   = 0;
    return;
}

//------------------------------------------------------------------------------

void Extension::run( long p_lats, double *p_latRos, double *p_latComb,
        double p_maxRos )
{
    // Initialize
    m_harmonicRos    = 0.0;
    m_averageRos     = 0.0;
    m_extHarmonicRos = 0.0;
    m_extAverageRos  = 0.0;
    double maxRos    = p_maxRos;

    //
    m_rf->calcExtendedSpreadRates2( m_cols, m_rows, m_blocks, m_combArray,
        m_rosArray, p_latRos, m_maxRosArray, p_lats );
    long oldLats = 2 * p_lats;
    m_latProb = 1.0;
    long i;
    for ( i=0; i<oldLats; i++ )
    {
        m_latProb *= p_latComb[i];
    }
    memset( m_cuumProb, 0x0, m_blocks * sizeof( double ) );     //*m_leeFuels

    // Return if this is the last extension
    if ( ! m_nextExt )
    {
        return;
    }

    long newLats = 2 * ( p_lats + 1 );
    double *latros2  = new double[newLats];
    double *latcomb2 = new double[newLats];
    long fuelCombs;
    long j, k;

    m_rf->calcCombinations( 1, 2, &fuelCombs, &m_latCombArray,
        &m_latRosArray);
    for ( i=0; i<m_blocks; i++ )
    {
        m_rf->spliceExtensions2( m_combArray[i], m_rosArray[i],
            &(m_nextExt->m_combArray), &(m_nextExt->m_rosArray), m_cols );
        for ( j=0; j<fuelCombs; j++ )
        {
            latros2[0] = m_latRosArray[j][0];
            memcpy( &latros2[1], p_latRos, oldLats * sizeof( double ) );
            latros2[p_lats*2+1] = m_latRosArray[j][1];
            latcomb2[0] = m_latCombArray[j][0];
            memcpy( &latcomb2[1], p_latComb, oldLats * sizeof( double ) );
            latcomb2[p_lats*2+1] = m_latCombArray[j][1];

            m_nextExt->run( p_lats+1, latros2, latcomb2, m_maxRosArray[i] );
            for ( k=0; k<m_blocks; k++ )
            {
                if ( m_nextExt->m_maxRosArray[k] > maxRos
                 &&  m_nextExt->m_maxRosArray[k] > m_maxRosArray[i] )
                {
                    m_prob = m_nextExt->calcProb(k, true);
                    m_cuumProb[i] += m_prob;
                    m_harmonicRos += m_prob / m_nextExt->m_maxRosArray[k];
                    m_averageRos  += m_prob * m_nextExt->m_maxRosArray[k];
                }
            }
            // accumulate harmonic ros from next extension
            m_extHarmonicRos += m_nextExt->m_harmonicRos;
            // accumulate average ros from next extension
            m_extAverageRos  += m_nextExt->m_averageRos;
        }
        // Accumulate cumulative probabilities from next extension
        m_extCuumProb += m_cuumProb[i];
    }
    if ( m_latCombArray )
    {
        for ( i=0; i<fuelCombs; i++ )
        {
            delete[] m_latCombArray[i];
        }
        delete[] m_latCombArray;
        m_latCombArray = 0;
    }
    if ( m_latRosArray )
    {
        for ( i=0; i<fuelCombs; i++)
        {
            delete[] m_latRosArray[i];
        }
        delete[] m_latRosArray;
        m_latRosArray = 0;
    }
    if ( latros2 )
    {
        delete[] latros2;
    }
    if ( latcomb2 )
    {
        delete[] latcomb2;
    }
    return;
}

//------------------------------------------------------------------------------
//  End of newext.cpp
//------------------------------------------------------------------------------

