/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Part of Mark Finney's EXRATE package for determining expected
*           and harmonic mean spread rate in randomly arranged fuels
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in this file is, in part or in whole, from
*           BehavePlus5 and EXRATE source originally authored by Collin D. 
*           Bevins and Mark Finney respectively, and is used with or without 
*           modification.
*
*******************************************************************************
*
* THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
* MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
* IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
* OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
* PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
* LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
* PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
* RELIABILITY, OR ANY OTHER CHARACTERISTIC.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
******************************************************************************/
//-----------------------------------------------------------------------------
/*! \file newext.h
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

#ifndef NEWEXT_H
#define NEWEXT_H

// Custom include files
#include "randfuel.h"

class RandFuel;

class Extension
{
// Public methods
public:
    // Constructors/destructors
    Extension( void );
    ~Extension( void );
    // These are called by RandFuel::computeSpread2()
    bool   allocExtension( long p_blocks, long p_cols, long p_rows, long p_fuels ) ;
    double calcProb( long p_block, bool p_subtFaster ) ;
    void   run( long p_lats, double *p_latRos, double *p_latComb,
                double p_maxRos ) ;

// Private methods
protected:
    void   freeExtension( void ) ;
    double getFasterProbs( long p_block ) ;
    void   init( void );

// Public data
public:
    double     m_harmonicRos;   //!< cumulative harmonic mean of faster spread rates p/r
    double     m_averageRos;    //!< cumulative average of faster spread rates p/r
    double     m_extHarmonicRos;//!< cumulative harmonic mean of faster spread rates p/r
    double     m_extAverageRos; //!< cumulative average of faster spread rates p/r
    double     m_extCuumProb;   //!< cumulative prob of faster spread rates in extensions
    Extension *m_nextExt;       //!< link to next Extension
    RandFuel  *m_rf;            //!< pointer to calculations of RandFuel
    double    *m_maxRosArray;   //!< 1x array with resulting max spread rates for each block
    double   **m_combArray;     //!< 2x array with probabilities in it
    double   **m_rosArray;      //!< 2x array with spread rates in it

// Private data
protected:
    double     m_prob;          //!< prob of faster spread rates
    double     m_latProb;       //!< Probability of lateral fuel combination
    long       m_blocks;        //!< number of allocations (blocks)
    long       m_cols;          //!< number of columns in block
    long       m_rows;          //!< number of rows in block
    long       m_cells;         //!< number of cells in block
    long       m_fuels;         //!< number of fuel types
    long       m_leeFuels;      //!< number of lee fuel combinations
    char      *m_blockPtr;      //!< Pointer to single dynamic memory block
    double    *m_cuumProb;      //!< cumulative prob of faster spread rates
    double   **m_latRosArray;   //!< lee side spread rates and probabilities
    double   **m_latCombArray;  //!< lee side spread rates and probabilities
};

#endif //  NEWEXT_H

//------------------------------------------------------------------------------
//  End of newext.h
//------------------------------------------------------------------------------
