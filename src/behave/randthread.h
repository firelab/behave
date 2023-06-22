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
/*! \file randthread.h
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

#ifndef RANDTHREAD_H
#define RANDTHREAD_H

#define REFRACT_LATERAL 0
#define REFRACT_FORWARD 1

//------------------------------------------------------------------------------

double pow2(double input);

//------------------------------------------------------------------------------
/*! \typedef PathStruct
 *  \brief Linked list structure for pathtimes allocated by each RandThread
 */

typedef struct
{
    long    m_loc;            // location in combarray and rosarray
    long    m_ignitionPt;     // 0=center, -1=left, 1=right
    double  m_pathTime;       // time for fire to reach this cell
    double  m_relCellSize;    // cumulative relative cell size for this point
} PathStruct;

//------------------------------------------------------------------------------
/*! \class RandThread randfuel.h
 */

class RandThread
{
    // Public methods
public:
    RandThread();
    ~RandThread();
    void    calcSpreadPaths2(void);
    void    setThreadData(long p_samples, long p_depths, long p_combs,
        double p_lbRatio, double **p_combArray, double **p_rosArray,
        double *p_maxRosArray, long p_start, long p_end,
        long p_firstSample, long p_lastSample,
        double *p_m_latRosArray, long p_lessIgns);

    // Private methods:
protected:
    void    addNewPath(long *NumPath2, double p_time, long p_loc,
        long p_ignitionPt, double p_relCellSize);
    void    calcEllipticalDimensions(void);
    double  calcFlankingTime(long p_numLayers, double p_separation,
        double p_overlap, double *p_latDist, double *p_ros,
        long p_refractDir);
    double  calcLateralRos(double p_forwardRos);
    void    calcStartDelay(long p_laterals, long p_leftRight);
    double  fastFlankTime(long XStart, long YStart, double Xmid,
        long XEnd, long YEnd, long NumX, double **ros);
    double  spreadTime(long XStart, long YStart, double Xmid,
        long XEnd, long YEnd, long NumX,
        double **ros, long FastFlank);

    //Private data
protected:
    double      m_lbRatio;      //!< elliptical fire length-to-breadth ratio
    double      m_a;            //!< elliptical fire spread rate dimensions
    double      m_b;            //!< elliptical fire spread rate dimensions
    double      m_c;            //!< elliptical fire spread rate dimensions
    double      m_cellSize;     //!< size of raster cell
    long        m_samples;      //!< number of columns in the block
    long        m_depths;       //!< number of rows in the block
    long        m_combs;        //!< number of combinations in block
    long        m_lessIgns;     //!< number of ignition points fewer than m_samples
    long        m_start;        //!< start and end for thread
    long        m_end;          //!< start and end for thread
    long        m_firstSample;  //!< specify ignition pts along the x axis
    long        m_lastSample;   //!< specify ignition pts along the x axis
    double    **m_combArray;    //!< probability array for all blocks, from RandFuel
    double    **m_rosArray;     //!< spread rate array for all blocks, from RandFuel
    double     *m_maxRosArray;  //!< max ROS for all blocks, passed in from RandFuel
    double      m_latRos;       //!< lateral spread rate from ig pt
    PathStruct *m_firstPath;    //!< pointer to array of PathStructs
    PathStruct *m_curPath;      //!< pointer to array of PathStructs
    PathStruct *m_newPath;      //!< pointer to array of PathStructs
    double     *m_startDelay[2];//!< pointer to delay data for extra row
    double     *m_latRosArray;  //!< pointer to delay data for extra row
};

#endif // RANDTHREAD_H

//------------------------------------------------------------------------------
//  End of randthread.h
//------------------------------------------------------------------------------
