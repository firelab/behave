//------------------------------------------------------------------------------
/*! \file randfuel.cpp
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
#include "randfuel.h"

// Standard include files
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------

RandFuel::RandFuel( void )
{
    init();
    return;
}

//------------------------------------------------------------------------------

RandFuel::~RandFuel( void )
{
    freeBlockArrays();
    if ( m_maxRosArray )
    {
        delete[] m_maxRosArray;
    }
    m_maxRosArray = 0;
    freeFuels();
    return;
}

//------------------------------------------------------------------------------

bool RandFuel::allocFuels( long p_fuels )
{
    freeFuels();
    if ( ! ( m_fuelTypeArray = new FuelType[ p_fuels ] ) )
    {
        return( false );
    }
    m_fuels = p_fuels;
    return( true );
}

//------------------------------------------------------------------------------

bool RandFuel::allocRandThreads( void )
{
    closeRandThreads();
    if ( ! ( m_randThread = new RandThread[m_threads] ) )
    {
        return( false );
    }
    return( true );
}


//------------------------------------------------------------------------------
/*! \brief
 *
 *  Performs the following steps:
 *  -#  Calculates factorial combinations of all fuel types,
 *  -#  puts probabilities into *comb and spread rates into *ros
 *  -#  fills **ca with factorial combinations of probabilities (from comb)
 *      for all rows and columns in test block ( p_nX x p_nY )
 *  -#  Fills **ra with factorial combinations of spread rates (from ros)
 *      for all rows and columns in test block ( p_nX x p_nY )
 */

bool RandFuel::calcCombinations( long p_nX, long p_nY, long *p_nT,
        double ***p_ca, double ***p_ra)
{
    double *comb;                          // array of probability distribution
    double *ros;                           // array of spread rate distribution

    unsigned long types = (unsigned long) pow( (double) m_fuels, (int) p_nX ) * p_nX;
    if ( ! ( comb = new double[types] ) )
    {
        return( false );
    }
    memset( comb, 0x0, types * sizeof( double ) );

    if ( ! ( ros = new double[types] ) )
    {
        delete[] comb;
        return( false );
    }
    memset( ros,  0x0, types * sizeof( double ) );

    // calculate the combinations that form the breadth of the fuel patch
    long terms = 1;
    long i, j, k, m, n, p, q;
    for ( i=0; i<p_nX; i++ )
    {
        m=0;
        for ( j=0; j<m_fuels; j++ )
        {
            for ( k=0; k<terms; k++ )
            {
                comb[m+i] = m_fuelTypeArray[j].m_fract;
                ros[m+i]  = m_fuelTypeArray[j].m_relRos;
                m += p_nX;
            }
        }
        if ( i < (p_nX - 1) )
        {
            m = 1;
            do
            {
                memcpy( &comb[ m * m_fuels * p_nX * terms ], &comb[0],
                    m_fuels * terms * p_nX * sizeof( double ) );
                memcpy( &ros[  m * m_fuels * p_nX * terms ], &ros[0],
                    m_fuels * terms * p_nX * sizeof( double ) );
                m += 1;
            } while( m < m_fuels );
        }
        terms *= m_fuels;
    }

    long cols = (long) pow( (double) m_fuels, (int) p_nX );
    if ( p_nT )
    {
        *p_nT = (long) pow( (double) cols, (int) p_nY );
    }
    if ( ! ( *p_ca     = new double*[ *p_nT ] ) )
    {
        delete[] comb;
        delete[] ros;
        return( false );
    }

    if ( ! ( *p_ra = new double*[ *p_nT ] ) )
    {
        delete[] comb;
        delete[] ros;
        delete[] *p_ca;
        return( false );
    }

    // calculate block array probabilities and spread rates
    for ( i=0; i<*p_nT; i++ )
    {
        (*p_ca)[i] = new double[ p_nX * p_nY ];
        (*p_ra)[i] = new double[ p_nX * p_nY ];
    }

    terms = 1;
    for ( i=0; i<p_nY; i++ )
    {
        m = 0;
        for ( j=0; j<cols; j++ )
        {
            for ( k=0; k<terms; k++ )
            {
                memcpy( &((*p_ca)[m][ i * p_nX ]),  &comb[ j * p_nX ],
                    p_nX * sizeof( double ) );
                memcpy( &((*p_ra)[m++][ i * p_nX]), &ros[ j * p_nX ],
                    p_nX * sizeof( double ) );
            }
        }
        n = cols * m;
        if ( i < ( p_nY - 1 ) )
        {
            q = m;
            do
            {
                for ( p=0; p<q; p++ )
                {
                    memcpy( &((*p_ca)[m][0]), &((*p_ca)[p][0]),
                        p_nY * p_nX * sizeof( double ) );
                    memcpy( &((*p_ra)[m++][0]), &((*p_ra)[p][0]),
                        p_nY * p_nX * sizeof( double ) );
                }
            } while( m < n );
        }
        terms *= cols;
    }

    delete[] comb;
    delete[] ros;
    return( true );
}

//-----------------------------------------------------------------------------------
/*! \brief Calculates the spread rates for a sample block with
 *  count=p_laterals extensions (columns) on either side.
 *
 *  \param p_cols        Number of columns of the sample block.
 *  \param p_rows        Number of rows of the sample block.
 *  \param p_laterals    Number of fuel arrangements in the block to calculate.
 *  \param p_combArray   2d array of probabilities for the block.
 *  \param p_rosArray    2d array of spread rates for the block.
 *  \param p_latRosArray Array of spread rates in the new lee-side row.
 *  \param p_maxRosExtArray The return array with the maximum spread rates
 *                      for count=NumLatCombs arrangements.
 */

void RandFuel::calcExtendedSpreadRates2( long p_cols, long p_rows,
        long p_latCombs, double **p_combArray, double **p_rosArray,
        double *p_latRosArray, double *p_maxRosExtArray, long p_laterals )
{
    double ipart;
    double interval = ( (double) p_latCombs ) / ( (double) m_threads );
    double fract = modf( interval, &ipart );
    long   range = (long) interval;
    if ( fract > 0.0 )
    {
       range++;
    }
    long begin = 0;
    long end   = 0;
    for ( int i=0; i<m_threads; i++ )
    {
        end = begin + range;
        if ( begin >= p_latCombs )
        {
            continue;
        }
        if ( end > p_latCombs )
        {
            end = p_latCombs;
        }
        m_randThread[i].setThreadData(p_cols, p_rows, p_latCombs,
            m_lbRatio, p_combArray, p_rosArray, p_maxRosExtArray, begin, end, p_laterals,
            ( p_cols - p_laterals), p_latRosArray, m_lessIgns ) ;
        begin = end;
    }
    m_randThread[0].calcSpreadPaths2();
    return;
}

//------------------------------------------------------------------------------
/*! \brief
 *
 *  -#  Allocates threads and m_maxRosArray array to store max spread rates
 *      from all blocks
 *  -#  Divides the Number of Combinations (m_combs) into parts for each thread.
 *  -#  Runs each thread and wait until they are all finished (hRandSyncEvent)
 *  -#  Calculates Expected Spread Rates by Prob[i] X MaxSpread[i]
 *
 */

void RandFuel::calcSpreadRates( void )
{
    if ( m_maxRosArray )
    {
        delete[] m_maxRosArray;
    }
    m_maxRosArray = new double[m_combs];
    memset( m_maxRosArray, 0x0, m_combs * sizeof( double ) );

    double interval = ( (double) m_combs ) / ( (double) m_threads );
    double ipart;
    double fract    = modf( interval, &ipart );
    long   range    = (long) interval;
    if ( fract > 0.0 )
    {
        range++;
    }
    long begin = 0;
    long end   = 0;
    for ( int i=0; i<m_threads; i++)
    {
        end = begin + range;
        if ( begin >= m_combs )
        {
            continue;
        }
        if ( end > m_combs )
        {
            end = m_combs;
        }
        m_randThread[i].setThreadData(m_samples, m_depths, m_combs, m_lbRatio,
            m_combArray, m_rosArray, m_maxRosArray, begin, end, 0, m_samples,
            0, m_lessIgns );
        begin = end;
    }
    m_randThread[0].calcSpreadPaths2();
    return;
}

//------------------------------------------------------------------------------

void RandFuel::closeRandThreads( void )
{
    if ( m_randThread )
    {
        delete[] m_randThread;
        m_randThread = 0;
    }
    return;
}
//------------------------------------------------------------------------------
/*! \brief Computes Expected Spread Rate from all factorial combinations
 *  of fuels and their probabilities.
 *
 *  Also adds ROS from lateral extensions=Extend.
 */

double RandFuel::computeSpread2( long p_samples, long p_depths,
        double p_lbRatio, long p_threads, double *p_maxRos,
        double *p_harmonicRos, long p_exts, long p_lessIgns )
{
    long i, j, k, m, fuelCombs;
    double maxRos   = 0.0;
    double minRos   = 1e12;
    double harmonic = 0.0;
    double average  = 0.0;

    if ( p_samples < 1 || p_samples > 50 )
    {
        return( 0.0 );
    }

    m_samples  = p_samples;
    m_depths   = p_depths;
    m_threads  = p_threads;
    m_lessIgns = p_lessIgns;
    m_lbRatio  = p_lbRatio;

    for ( i=0; i<m_fuels; i++ )
    {
        if ( maxRos < m_fuelTypeArray[i].m_absRos )
        {
            maxRos = m_fuelTypeArray[i].m_absRos;
        }
        if ( minRos > m_fuelTypeArray[i].m_absRos )
        {
            minRos = m_fuelTypeArray[i].m_absRos;
        }
    }
    for ( i=0; i<m_fuels; i++ )
    {
        m_fuelTypeArray[i].m_relRos = m_fuelTypeArray[i].m_absRos / maxRos;
    }

    // base combinations for sample block
    calcCombinations( m_samples, m_depths, &m_combs, &m_combArray, &m_rosArray );
    if ( ! allocRandThreads() )
    {
        return( -1.0 );
    }

    calcSpreadRates(); // ri for sample block
    double **latComb, **latRos;

    double prob, cuumProb;

    if ( p_exts > 0 )
    {
        calcCombinations( 2, m_depths, &m_exts, &m_combExtArray, &m_rosExtArray ); // extensions only
        calcCombinations( 1, 2, &fuelCombs, &latComb, &latRos );
        Extension *ext = new Extension[ p_exts ];

        // allocate the number of extensions
        for ( j=0; j<p_exts; j++ )
        {
            ext[j].allocExtension( m_exts, ( m_samples + ( j * 2 ) + 2 ),
                m_depths, m_fuels );
            ext[j].m_rf = this;
        }
        // assign a pointer to the next one
        for ( j=p_exts-2; j>=0; j-- )
        {
            ext[j].m_nextExt = &ext[j+1];
        }

        // for all original combinations of the sample block
fprintf( stderr, "%ld extensions: ", m_combs );
        for ( j=0; j<m_combs; j++ )
        {
            cuumProb = 0.0;
            // don't need to do this if spread rate is already 1.0
            if ( m_maxRosArray[j] < 1.0 )
            {
                spliceExtensions2( m_combArray[j], m_rosArray[j],
                    &(ext[0].m_combArray), &(ext[0].m_rosArray),
                    m_samples);

                for ( k=0; k<fuelCombs; k++ )
                {
                    ext[0].run( 1, latRos[k], latComb[k], m_maxRosArray[j] );
                    for ( m=0; m<m_exts; m++ )
                    {
                        if ( ext[0].m_maxRosArray[m] > m_maxRosArray[j] )
                        {
                            prob      = ext[0].calcProb( m, true );
                            cuumProb += prob;
                            harmonic += prob / ext[0].m_maxRosArray[m];
                            average  += prob * ext[0].m_maxRosArray[m];
                        }
                    }
                }
                // accumulate harmonc ros from ext[0] only
                harmonic += ext[0].m_harmonicRos;
                // accumulate average ros from ext[0] only
                average  += ext[0].m_averageRos;
                // accumulate harmonic ros from all extensions
                // (ext[0] contains all >ext[0]
                harmonic += ext[0].m_extHarmonicRos;
                // accumulate average ros from all extensions
                // (ext[0] contains all >ext[0]
                average  += ext[0].m_extAverageRos;
            }
            prob = 1.0;
            for ( i=0; i<m_samples*m_depths; i++ )
            {
                prob *= ( (double) m_combArray[j][i] );
            }
            for ( i=0; i<p_exts; i++ )
            {
                cuumProb += ext[i].m_extCuumProb;
                ext[i].m_extCuumProb = 0.0;
            }
            // subtract probabilities of faster spread rates
            prob     -= cuumProb;
            // harmonic mean from sample block with faster spread rates than extensions
            harmonic += prob / m_maxRosArray[j];
            // average from sample block with faster spread rates than extensions
            average  += prob * m_maxRosArray[j];

fprintf( stderr, "." );
        }
fprintf( stderr, "\n" );
        delete[] ext;
        if ( latComb )
        {
            for ( i=0; i<fuelCombs; i++ )
            {
                delete[] latComb[i];
            }
            delete[] latComb;
            latComb = 0;
        }
        if ( latRos )
        {
            for ( i=0; i<fuelCombs; i++ )
            {
                delete[] latRos[i];
            }
            delete[] latRos;
            latRos = 0;
        }
    }
    else
    {
        for ( i=0; i<m_combs; i++ )
        {
            prob = 1.0;
            for ( j=0; j<m_depths; j++ )
            {
                for ( k=0; k<m_samples; k++ )
                {
                    prob *= m_combArray[i][j*m_samples+k];
                }
            }
            average += m_maxRosArray[i] * prob;
            if ( m_maxRosArray[i] > 0.0 )
            {
                harmonic += prob / m_maxRosArray[i];
            }
        }
    }

    if ( p_maxRos )
    {
        *p_maxRos = maxRos;
    }
    if ( p_harmonicRos )
    {
        *p_harmonicRos = 0.;
        if ( harmonic > 0.0 )
        {
            *p_harmonicRos = ( 1.0 / harmonic );
        }
    }
    closeRandThreads();
    freeBlockArrays();
    return( average );
}

//------------------------------------------------------------------------------

void RandFuel::freeFuels( void )
{
    if ( m_fuelTypeArray )
    {
        delete[] m_fuelTypeArray;
    }
    m_fuelTypeArray = 0;
    m_fuels = 0;
    return;
}

//------------------------------------------------------------------------------

void RandFuel::freeBlockArrays( void )
{
     long i;
     if ( m_combArray )
     {
        for ( i=0; i<m_combs; i++ )
        {
            delete[] m_combArray[i];
        }
        delete[] m_combArray;
        m_combArray = 0;
     }
     if ( m_rosArray )
     {
        for ( i=0; i<m_combs; i++ )
        {
            delete[] m_rosArray[i];
        }
        delete[] m_rosArray;
        m_rosArray = 0;
     }
     if ( m_combExtArray )
     {
        for ( i=0; i<m_exts; i++ )
        {
            delete[] m_combExtArray[i];
        }
        delete[] m_combExtArray;
        m_combExtArray = 0;
     }
     if ( m_rosExtArray )
     {
        for ( i=0; i<m_exts; i++ )
        {
            delete[] m_rosExtArray[i];
        }
        delete[] m_rosExtArray;
        m_rosExtArray = 0;
     }
     if ( m_maxRosExtArray )
     {
        delete[] m_maxRosExtArray;
        m_maxRosExtArray = 0;
    }
    m_combs = 0;
    m_exts  = 0;
    return;
}

//------------------------------------------------------------------------------

void RandFuel::init( void )
{
    m_samples        = 0;
    m_depths         = 0;
    m_fuels          = 0;
    m_combs          = 0;
    m_exts           = 0;
    m_threads        = 0;
    m_lessIgns       = 0;
    m_lbRatio        = 0.0;
    m_cellSize       = 0.0;
    m_combArray      = 0;
    m_rosArray       = 0;
    m_combExtArray   = 0;
    m_rosExtArray    = 0;
    m_maxRosArray    = 0;
    m_maxRosExtArray = 0;
    m_fuelTypeArray  = 0;
    m_randThread     = 0;
    return;
}

//------------------------------------------------------------------------------
/*! \brief Recomputes spread using the existing spread rate array m_maxRosArray after
 *  the user has run ComputeSpread().
 *
 *  RecomputeSpread() assumes the user has loaded only new fuel data,
 *  changing only the proportions of fuels on the landscape.
 *  All that is needed then is to recompute the probabilities of the fuel
 *  arrangement for weighting in the harmonic mean calculation.
 */

double RandFuel::recomputeSpread( double *p_harmonicRos )
{
    if ( ! m_maxRosArray )
    {
        *p_harmonicRos = -1.0;
        return( -1.0 );
    }

    double expectedRos = 0.0;
    double harmonicRos = 0.0;
    double totalProb   = 0.0;
    calcCombinations( m_samples, m_depths, &m_combs, &m_combArray, &m_rosArray );
    for ( int i=0; i<m_combs; i++ )
    {
        double prob = 1.0;
        for ( int j=0; j<m_depths; j++ )
        {
            for ( int k=0; k<m_samples; k++ )
            {
                prob *= ( (double) ( m_combArray[i][j*m_samples+k] ) );
            }
        }
        expectedRos += ( (double) m_maxRosArray[i] ) * prob;
        if ( m_maxRosArray[i] > 0.0 )
        {
            totalProb   += prob;
            harmonicRos += prob / ( (double) m_maxRosArray[i] );
        }
     }

     // convert harmonicRos to double for return
     *p_harmonicRos = (double) totalProb / harmonicRos;
     freeBlockArrays();
     return( expectedRos );
}

//------------------------------------------------------------------------------

void RandFuel::setCellDimensions( double p_cellSize )
{
    m_cellSize = p_cellSize;
}

//------------------------------------------------------------------------------

void RandFuel::setFuelData( long p_type, double p_ros, double p_fract )
{
    if ( p_type <= m_fuels )
    {
        m_fuelTypeArray[p_type].m_absRos = p_ros;
        m_fuelTypeArray[p_type].m_relRos = -1.0;
        m_fuelTypeArray[p_type].m_fract  = p_fract;
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief Splices *p_ca into m_combExtArray and *p_ra into m_rosExtArray
 *  and puts the results into **p_cs and **p_rs.
 *
 *  \param p_ca Array of combinations (probabilities of the fuels occurring)
 *  \param p_ra Array of spread rates for each fuel type
 *  \param p_cs 2x array of all combinations of the fuels
 *              in m_combExtArray and p_ca
 *  \param p_rs 2x array of all spread rates of the fuels
 *              in m_combExtArray and p_ra
 */

void RandFuel::spliceExtensions2( double *p_ca, double *p_ra, double ***p_cs,
        double ***p_rs, long p_oldCols )
{
    long newCols;
    unsigned loc;
    newCols = p_oldCols + 2;          // adding 2 columns to p_oldCols
    for ( long m=0; m<m_exts; m++ )
    {
        for ( long j=0; j<m_depths; j++ )
        {
            memcpy( &((*p_cs)[m][j*newCols]), &m_combExtArray[m][j*2],
                sizeof(double));
            ((*p_rs)[m][j*newCols]) = m_rosExtArray[m][j*2];
            loc = j * newCols + 1;
            memcpy( &((*p_cs)[m][loc]), &p_ca[j*p_oldCols],
                p_oldCols*sizeof(double));
            memcpy( &((*p_rs)[m][loc]), &p_ra[j*p_oldCols],
                p_oldCols*sizeof(double));
            loc = j * newCols + p_oldCols + 1;
            memcpy( &((*p_cs)[m][loc]), &m_combExtArray[m][j*2+1],
                sizeof(double));
            ((*p_rs)[m][j*newCols+p_oldCols+1]) = m_rosExtArray[m][j*2+1];
        }
    }
    return;
}

//------------------------------------------------------------------------------
// End of randfuel.cpp
//------------------------------------------------------------------------------

