//------------------------------------------------------------------------------
/*! \file randthread.cpp
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
#include "randthread.h"

// Standard include files
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------

static const double PI = acos(-1.0);

//------------------------------------------------------------------------------

double pow2(double input)
{
    return(input * input);
}

//------------------------------------------------------------------------------

RandThread::RandThread(void)
{
    m_lbRatio = 0.0;
    m_a = 0.0;
    m_b = 0.0;
    m_c = 0.0;
    m_cellSize = 10.0;
    m_samples = 0;
    m_depths = 0;
    m_combs = 0;
    m_lessIgns = 0;
    m_start = 0;
    m_end = 0;
    m_firstSample = 0;
    m_lastSample = 0;
    m_combArray = 0;
    m_rosArray = 0;
    m_maxRosArray = 0;
    m_latRos = 0.0;
    m_firstPath = 0;
    m_curPath = 0;
    m_newPath = 0;
    m_startDelay[0] = 0;
    m_startDelay[1] = 0;
    m_latRosArray = 0;
    return;
}

//------------------------------------------------------------------------------

RandThread::~RandThread()
{
    return;
}

//------------------------------------------------------------------------------

void RandThread::addNewPath(long *NumPath2, double p_time, long p_loc,
    long p_ignitionPt, double p_relCellSize)
{
    if (p_loc < 0
        || p_loc > m_samples - 1)
    {
        return;
    }

    m_newPath[*NumPath2].m_pathTime = p_time;
    m_newPath[*NumPath2].m_loc = p_loc;
    m_newPath[*NumPath2].m_ignitionPt = p_ignitionPt;
    m_newPath[*NumPath2].m_relCellSize = p_relCellSize;
    *NumPath2 += 1;
    return;
}

//------------------------------------------------------------------------------
/*! \brief Calculates the elliptical dimensions given the m_lbRatio ratio input.
 *  m_a = Lateral spread rate
 *  b+c = forward spread rate.
 */

void RandThread::calcEllipticalDimensions(void)
{
    double hbRatio, hfRatio;
    hbRatio = (m_lbRatio + sqrt(pow2(m_lbRatio) - 1.0))
        / (m_lbRatio - sqrt(pow2(m_lbRatio) - 1.0));
    m_a = 0.5 * (1.0 + 1.0 / hbRatio) / m_lbRatio;
    m_b = (1.0 + 1.0 / hbRatio) / 2.0;
    m_c = m_b - 1.0 / hbRatio;
    hfRatio = (m_b + m_c) / m_a;
    calcLateralRos(1.0);
    return;
}

//------------------------------------------------------------------------------
/*! \brief Calculates total travel time for a point source ignition,
 *  given its spread at a constant angle B (Overlap/Separation).
 *
 *  \param *LatDist Contains count==NumLayers lateral distances of adjacent cells.
 *  \param *Ros     Contains count==NumLayers spread rates of adjacent cells.
 *  \param  RefractDir  Determines if the refraction is for the forward or
 *                      lateral direction.
 */

double RandThread::calcFlankingTime(long p_numLayers, double p_separation,
    double p_overlap, double *p_latDist, double *p_ros,
    long /* p_refractDir */)
{
    double beta = atan2(p_overlap, p_separation);
    double cosB = cos(beta);
    double sinB = sin(beta);
    double cosB2 = pow2(cosB);
    double sinB2 = pow2(sinB);

    // calculate theta, angle from center of ellipse
    double cosT = (m_a * cosB *
        sqrt(pow2(m_a) * cosB2 + (pow2(m_b) - pow2(m_c)) *sinB2)
        - m_b * m_c * sinB2)
        / (pow2(m_a) * cosB2 + pow2(m_b) * sinB2);

    double theta = acos(cosT);

    double ros;
    double travelTime = 0.0;
    for (int i = 0; i < p_numLayers; i++)
    {
        ros = m_a * sin(theta) * p_ros[i];
        travelTime += p_latDist[i] / ros;
    }
    return(travelTime);
}

//------------------------------------------------------------------------------

double RandThread::calcLateralRos(double p_forwardRos)
{
    double theta = acos(m_c / m_b);
    m_latRos = fabs(m_a * sin(theta)) * p_forwardRos;
    return(m_latRos);
}

//------------------------------------------------------------------------------
/*  \brief Calculates maximum spread rates and stores in
 *  RandFuel::m_maxRosArray for for each column in the test block
 *
 *  It does this by computing the spread rate from the 1st cell in the column
 *  through all other cells in the next row, either by flanking or by heading
 *  or by a combination of flanking and heading.
 *
 *  Stores only the maximum ROS (minimum path time) for each block in
 *  m_maxRosArray because this represents the path that fire would first
 *  emerge from the block.
 *
 *  This version 2 does a one time allocation of data needed,
 *  rather than linked list (it is probably slightly faster than version 1)
 */

void RandThread::calcSpreadPaths2(void)
{
    bool Lateral = false;
    long i, j, k, m, n, p;
    long NumPath1, NumPath2, ParentLoc, NumMax, StraightNum;
    unsigned long NumAlloc;
    double     Delay, *SampleTime, ParentRos, ParentTime;
    double     Separation, Overlap, OldOverlap, OldSeparation, StraightTime;
    double    DirectTime, *ExitTime, *LateralDistances, *SpreadRates;

    if (m_firstSample != 0)
    {
        Lateral = true;
    }

    ExitTime = new double[m_samples];
    memset(ExitTime, 0x0, m_samples*sizeof(double));
    if (m_firstSample > 0)
    {
        // store number of startdelays
        m_startDelay[0] = new double[m_firstSample];
        m_startDelay[1] = new double[m_firstSample];
        memset(m_startDelay[0], 0x0, m_firstSample*sizeof(double));
        memset(m_startDelay[1], 0x0, m_firstSample*sizeof(double));
    }
    SampleTime = new double[m_samples];
    NumAlloc = (unsigned long)pow((double)m_samples, (int)m_depths);
    m_firstPath = new PathStruct[NumAlloc];
    m_newPath = new PathStruct[NumAlloc];
    NumMax = m_samples;
    if (m_depths > m_samples)
    {
        NumMax = m_depths;
    }
    LateralDistances = new double[NumMax];
    SpreadRates = new double[NumMax + 1];
    calcEllipticalDimensions();
    if (m_firstSample > 0)
    {
        // calculate all start delays for lateral extensions, left
        calcStartDelay(m_firstSample, 0);
        // calculate all start delays for lateral extensions, right
        calcStartDelay(m_firstSample, 1);
    }
    for (i = m_start; i < m_end; i++)
    {
        m_maxRosArray[i] = 0.0;
        for (p = 0; p < m_samples; p++)   // make it very large
        {
            SampleTime[p] = 9e12;
        }
        for (k = m_lessIgns; k < m_samples - m_lessIgns; k++)
        {
            j = 0;
            m_curPath = m_firstPath;
            m_curPath->m_loc = k;
            Delay = 0.0;
            if (Lateral)
            {
                if (k < m_firstSample)
                {
                    // centered=0, -1=left, 1=right
                    m_curPath->m_ignitionPt = -1;
                    Delay = m_startDelay[0][m_firstSample - k - 1];
                    // no delay possible because spread rate ==0.0
                    if (Delay < 0.0)
                    {
                        continue;
                    }
                }
                else if (k >= m_lastSample)
                {
                    m_curPath->m_ignitionPt = 1;
                    Delay = m_startDelay[1][k - m_lastSample];
                    // no delay possible because spread rate ==0.0
                    if (Delay < 0.0)
                    {
                        continue;
                    }
                }
                else
                {
                    m_curPath->m_ignitionPt = 0;
                }
            }
            else
            {
                // centered=0, -1=left, 1=right
                m_curPath->m_ignitionPt = 0;
            }
            m_curPath->m_relCellSize = 0.0;
            m_curPath->m_pathTime = Delay;
            NumPath1 = 1;
            NumPath2 = 0;
            for (n = 0; n < NumPath1; n++)
            {
                ParentRos = m_rosArray[i][j * m_samples + m_curPath->m_loc];
                if (ParentRos > 0.0)
                {
                    Separation = m_cellSize;
                    Overlap = m_cellSize;
                    // if not ignition point centered
                    if (m_curPath->m_ignitionPt == 0)
                    {
                        Overlap /= 2.0;
                    }
                    Separation += m_curPath->m_relCellSize;
                    p = 0;
                    do
                    {
                        LateralDistances[p] = Overlap;
                        SpreadRates[p] = m_rosArray[i][p*m_samples + m_curPath->m_loc];
                        if (Separation > m_cellSize)
                        {
                            LateralDistances[p] = Overlap / (double)(j + 1);
                        }
                        p++;
                    } while (p <= j);
                    Delay = calcFlankingTime((long)(Separation / m_cellSize),
                        Separation, Overlap, LateralDistances, SpreadRates,
                        REFRACT_FORWARD);
                    ParentLoc = m_curPath->m_loc;
                    ParentTime = m_curPath->m_pathTime;
                    addNewPath(&NumPath2, (ParentTime + m_cellSize / ParentRos),
                        ParentLoc, m_curPath->m_ignitionPt, Separation);  // go straight ahead
                    if (j < m_depths - 1) // && Delay<NextTime)
                    {
                        StraightNum = (long)(m_curPath->m_relCellSize / m_cellSize);
                        StraightTime = 0.0;
                        for (p = 0; p < StraightNum; p++)
                        {
                            StraightTime += m_cellSize
                                / m_rosArray[i]
                                [(j - p - 1) * m_samples + m_curPath->m_loc];
                        }
                        Delay += (ParentTime - StraightTime);
                        Separation = m_cellSize;

                        switch (m_curPath->m_ignitionPt)
                        {
                        case -1:
                            addNewPath(&NumPath2, Delay, ParentLoc - 1, -1, 0.0);
                            break;
                        case 1:
                            addNewPath(&NumPath2, Delay, ParentLoc + 1, 1, 0.0);
                            break;
                        case 0:
                            addNewPath(&NumPath2, Delay, ParentLoc - 1, -1, 0.0);
                            addNewPath(&NumPath2, Delay, ParentLoc + 1, 1, 0.0);
                            break;
                        }

                        // go left
                        //LastDelay=ParentDelay;
                        OldOverlap = Overlap;
                        OldSeparation = Separation;
                        LateralDistances[0] = Overlap;
                        for (p = 1; p < m_samples; p++)
                        {
                            if (j * m_samples + ParentLoc - p < 0)
                            {
                                break;
                            }
                            SpreadRates[p] = m_rosArray[i]
                                [j * m_samples + ParentLoc - p];
                        }
                        for (p = 1; p < m_samples - 1; p++)
                        {
                            if (m_curPath->m_ignitionPt > 0
                                && m_curPath->m_relCellSize == 0)
                            {
                                break;
                            }
                            if (j * m_samples + ParentLoc - p < 0)
                            {
                                break;
                            }
                            LateralDistances[p] = m_cellSize;
                            //SpreadRates[p]=m_rosArray[i][j*m_samples+ParentLoc-p];
                            Overlap += m_cellSize;
                            Delay = calcFlankingTime(p + 1, Separation, Overlap,
                                LateralDistances, SpreadRates, REFRACT_LATERAL);
                            addNewPath(&NumPath2, (Delay + ParentTime),
                                ParentLoc - (p + 1), -1, 0.0);
                        }

                        // go right
                        Overlap = OldOverlap;
                        Separation = OldSeparation;
                        for (p = 1; p<m_samples; p++)
                        {
                            if (j*m_samples + ParentLoc + p >(long) NumAlloc - 1)
                            {
                                break;
                            }
                            SpreadRates[p] = m_rosArray[i]
                                [j*m_samples + ParentLoc + p];
                        }
                        for (p = 1; p < m_samples - 1; p++)
                        {
                            if (m_curPath->m_ignitionPt < 0
                                && m_curPath->m_relCellSize == 0)
                            {
                                break;
                            }
                            if (j*m_samples + ParentLoc + p >(long) NumAlloc - 1)
                            {
                                break;
                            }
                            //SpreadRates[p]=m_rosArray[i][j*m_samples+ParentLoc+p];
                            LateralDistances[p] = m_cellSize;
                            Overlap += m_cellSize;
                            Delay = calcFlankingTime(p + 1, Separation, Overlap,
                                LateralDistances, SpreadRates, REFRACT_LATERAL);
                            addNewPath(&NumPath2, (Delay + ParentTime),
                                ParentLoc + (p + 1), 1, 0.0);
                        }
                    }
                }
                else
                {
                    m_curPath->m_pathTime = 0.0;
                }

                // if this is the last one
                if (n == NumPath1 - 1)
                {
                    // if there are more paths to process
                    if (NumPath2 > 0)
                    {
                        NumPath1 = NumPath2;
                        NumPath2 = 0;
                        m_curPath = m_firstPath;
                        m_firstPath = m_newPath;
                        m_newPath = m_curPath;
                        m_curPath = m_firstPath;
                        n = -1;
                        j++;
                    }
                    else
                    {
                        NumPath1 = 0;
                    }
                }
                else
                {
                    m_curPath = &m_firstPath[n + 1];
                }
                if (j >= m_depths)
                {
                    break;
                }
            }
            DirectTime = 9e12;
            p = -1;

            for (j = 0; j<NumPath1; j++)
            {
                if (m_firstPath[j].m_pathTime >0.0
                    && m_firstPath[j].m_pathTime < SampleTime[k])
                {
                    SampleTime[k] = m_firstPath[j].m_pathTime;
                }
                if (m_firstPath[j].m_loc == k)
                {
                    if (m_firstPath[j].m_pathTime < DirectTime)
                    {
                        DirectTime = m_firstPath[j].m_pathTime;
                        p = j;
                    }
                }
            }
        }   // all Depths are done
        // find min time of all samples (max spread rate)
        m = 0;
        for (j = 0; j < m_samples; j++)
        {
            if (SampleTime[j] == 0.0
                || SampleTime[j] == 9e12)
            {
                m++;
                continue;
            }
            // calculate overall spread rate
            SampleTime[j] = (m_depths*m_cellSize) / SampleTime[j];
            if (SampleTime[j] > m_maxRosArray[i])
            {
                m_maxRosArray[i] = SampleTime[j];
            }
        }
    } // all combinations are done

    // Release resources
    delete[] SampleTime;
    delete[] ExitTime;
    for (i = 0; i < 2; i++)
    {
        if (m_startDelay[i])
        {
            delete[] m_startDelay[i];
            m_startDelay[i] = 0;
        }
    }
    delete[] m_firstPath;
    delete[] m_newPath;
    delete[] LateralDistances;
    delete[] SpreadRates;
    return;
}

//------------------------------------------------------------------------------
/*! \brief Computes array of starting delays for flanking
 *  within the additional lee-side row of fuels for lateral extensions.
 */

void RandThread::calcStartDelay(long p_laterals, long p_leftRight)
{
    // reverse the spread rate orders
    double *lateralDist = new double[p_laterals];
    double *spreadRates = new double[p_laterals];
    double  separation = m_cellSize;
    double  overlap = m_cellSize / 2.0;

    lateralDist[0] = overlap;
    if (p_leftRight)
    {
        spreadRates[0] = m_latRosArray[p_leftRight * p_laterals];
    }
    else
    {
        spreadRates[0] =
            m_latRosArray[p_leftRight * p_laterals + p_laterals - 1];
    }

    long i;
    for (i = 1; i < p_laterals; i++)
    {
        lateralDist[i] = m_cellSize;
        if (p_leftRight)
        {
            // start from inside out
            spreadRates[i] =
                m_latRosArray[p_leftRight * p_laterals + i];
        }
        else
        {
            // start from inside out
            spreadRates[i] =
                m_latRosArray[p_leftRight * p_laterals + (p_laterals - i - 1)];
        }
    }
    for (i = 0; i < p_laterals; i++)
    {
        m_startDelay[p_leftRight][i] =
            calcFlankingTime(i + 1, separation, overlap, lateralDist,
            spreadRates, REFRACT_LATERAL);
        overlap += m_cellSize;
    }
    delete[] lateralDist;
    delete[] spreadRates;
    return;
}

//------------------------------------------------------------------------------

double RandThread::fastFlankTime(long XStart, long YStart, double Xmid,
    long XEnd, long YEnd, long NumX, double **Ros)
{
    long NumCells, loc, sX, sY, NumVert;
    double TravelTime = 0.0;
    double Theta, TestDir, Fract, num;
    double SinG, CosG, Xc, Yc, aXmid;
    double dX, dY, ExpRate, dist, TravelDist, VertDist;
    double Dist[2] = { 0.0, 0.0 };
    double TDist[2], nX = 0.0, nY = 0.0, ROS;

    // Calculate Travel Angle and total distance through cell (beta)
    dX = (XEnd - XStart) * m_cellSize + Xmid * m_cellSize;
    dY = m_c / m_a * fabs(dX);
    VertDist = (YEnd - YStart) * m_cellSize;
    VertDist -= dY;
    dist = sqrt(pow2(dY) + pow2(dX));
    // will be different than CosB and SinB because of reference angle
    SinG = fabs(dY / dist);
    CosG = fabs(dX / dist);

    // Calculate Theta from Beta
    if (dX < 0.0)
    {
        Theta = -PI / 2.0;
    }
    else
    {
        Theta = PI / 2.0;
    }

    // Find quadrant
    if (Theta < 0.0)
    {
        Theta += PI;
    }
    ExpRate = sqrt(pow2(m_a) + pow2(m_c));

    // Obtain directional signs for sampling offsets from start point
    if (dX < 0.0)
    {
        sX = -1;
    }
    else
    {
        sX = 1;
    }
    if (dY < 0.0)
    {
        sY = -1;
    }
    else
    {
        sY = 1;
    }

    Fract = modf(fabs(dX) / m_cellSize, &Xc);
    Xc -= 1;
    if (Fract > 0.0)
    {
        Xc += 1.0;
    }
    Fract = modf(dY / m_cellSize, &Yc);
    Yc -= 1;
    if (Fract > 0.0)
    {
        Yc += 1.0;
    }
    NumCells = (long)(Xc + Yc) + 1;  //abs(XEnd-XStart)+abs(YEnd-YStart);
    aXmid = fabs(Xmid);

    while ((nX + nY) < NumCells)
    {
        loc = (long)(YStart + nY * sY) * NumX + XStart + ((long)nX) * sX;
        ROS = (*Ros)[loc];
        if (ROS == 0.0)
        {
            ROS = 1e-6;
        }

        if (fabs(CosG) < 1e-9)
        {
            TDist[0] = 1e9;
        }
        else
        {
            TDist[0] = (m_cellSize * (aXmid + nX)) / CosG;    // hypotenuse
        }
        if (fabs(SinG) < 1e-9)
        {
            TDist[1] = 1e9;
        }
        else
        {
            TDist[1] = (m_cellSize * (1 + nY)) / SinG;    // hypotenuse
        }
        if (fabs(TDist[1]) < 1e-9)
        {
            TestDir = 0.5;    // just make if smaller than 1.0
        }
        else
        {
            TestDir = fabs(TDist[0] / TDist[1]);
        }
        if (TestDir < 1.0)
        {
            TDist[1] = sqrt(pow2(TDist[0]) - pow2(m_cellSize * (aXmid + nX)));
            TDist[0] = m_cellSize * (aXmid + nX);
            nX += 1.0;
        }
        else if (TestDir > 1.0)
        {
            TDist[0] = sqrt(pow2(TDist[1]) - pow2(m_cellSize * (1.0 + nY)));
            TDist[1] = m_cellSize * (1.0 + nY);
            nY += 1.0;
        }
        else
        {
            TDist[1] = sqrt(pow2(TDist[0]) - pow2(m_cellSize * (aXmid + nX)));
            TDist[0] = m_cellSize * (aXmid + nX);
            nY += 1.0;
            nX += 1.0;
        }
        Dist[0] = TDist[0] - Dist[0];
        Dist[1] = TDist[1] - Dist[1];

        TravelDist = sqrt(pow2(Dist[0]) + pow2(Dist[1]));
        TravelTime += TravelDist / (ExpRate * ROS);

        // Copy new dimensions to old dimensions
        Dist[0] = TDist[0];
        Dist[1] = TDist[1];
    }

    // vertical stretch
    Fract = modf(VertDist / m_cellSize, &num) * m_cellSize;
    num += 1.0;
    loc = (long)(YStart + nY * sY) * NumX + XStart + ((long)nX) * sX;
    NumVert = 0;
    do
    {
        ROS = (*Ros)[loc];
        if (ROS > 0.0)
        {
            TravelTime += Fract / ROS;
        }
        loc += NumX;
        Fract = m_cellSize;
        NumVert += 1;
    } while (NumVert < ((long)num));

    return(TravelTime);
}

//------------------------------------------------------------------------------

void RandThread::setThreadData(long p_samples, long p_depths, long p_combs,
    double p_lbRatio, double **p_combArray, double **p_rosArray,
    double *p_maxRosArray, long p_start, long p_end, long p_firstSample,
    long p_lastSample, double *p_m_latRosArray, long p_lessIgns)
{
    m_samples = p_samples;
    m_depths = p_depths;
    m_combs = p_combs;
    m_lbRatio = p_lbRatio;
    m_combArray = p_combArray;
    m_rosArray = p_rosArray;
    m_maxRosArray = p_maxRosArray;
    m_start = p_start;
    m_end = p_end;
    m_firstSample = p_firstSample;
    m_lastSample = p_lastSample;
    m_latRosArray = p_m_latRosArray;
    m_lessIgns = p_lessIgns;
    return;
}

//------------------------------------------------------------------------------

double RandThread::spreadTime(long XStart, long YStart, double Xmid,
    long XEnd, long YEnd, long NumX, double **Ros, long Flank)
{
    long   NumCells, loc, sX, sY;
    double TravelTime = 0.0, FlankTime;
    double Beta, Theta, TestDir, aXmid;
    double CosB, SinB, SinB2, CosB2, CosT, SinG, CosG, SinT;
    double dX, dY, ExpRate, dist, TravelDist;
    double Dist[2] = { 0.0, 0.0 };
    double TDist[2];
    double nX = 0.0, nY = 0.0, ROS;

    // Calculate Travel Angle and total distance through cell (beta)
    dX = (XEnd - XStart) * m_cellSize + Xmid * m_cellSize;
    dY = (YEnd - YStart) * m_cellSize;
    dist = sqrt(pow2(dY) + pow2(dX));
    SinG = fabs(dY / dist);  // will be different than CosB and SinB because of reference angle
    CosG = fabs(dX / dist);
    Beta = atan2(dX, dY);
    CosB = cos(Beta);
    SinB = sin(Beta);
    CosB2 = pow2(CosB);
    SinB2 = pow2(SinB);

    // Calculate Theta from Beta
    CosT = (m_a * CosB
        * sqrt(pow2(m_a) * CosB2 + (pow2(m_b) - pow2(m_c)) * SinB2)
        - m_b * m_c * SinB2)
        / (pow2(m_a) * CosB2 + pow2(m_b) * SinB2);
    if (CosT >= 1.0)
    {
        Theta = 0.0;
    }
    else if (CosT <= -1.0)
    {
        Theta = PI;
    }
    else
    {
        Theta = acos(CosT);
    }

    // Find quadrant
    if (Theta < 0.0)
    {
        Theta += PI;
    }
    SinT = sin(Theta);
    ExpRate = sqrt(pow2(m_a * SinT) + pow2(m_b * CosT + m_c));

    // Obtain directional signs for sampling offsets from start point
    if (dX < 0.0)
    {
        sX = -1;
    }
    else
    {
        sX = 1;
    }
    if (dY < 0.0)
    {
        sY = -1;
    }
    else
    {
        sY = 1;
    }

    NumCells = abs(XEnd - XStart) + abs(YEnd - YStart);
    aXmid = fabs(Xmid);
    //  if ( dX > 5.0 && dY != 0.0 )
    //      NumCells -= 1;

    while (nX + nY < NumCells)
    {
        loc = (long)(YStart + nY * sY) * NumX + XStart + ((long)nX) * sX;
        ROS = (*Ros)[loc];

        if (ROS == 0.0)
        {
            ROS = 1e-6;
        }

        if (fabs(CosG) < 1e-9)
        {
            TDist[0] = 1e9;
        }
        else
        {
            TDist[0] = (m_cellSize * (aXmid + nX)) / CosG;  // hyptenuse
        }

        if (fabs(SinG) < 1e-9)
        {
            TDist[1] = 1e9;
        }
        else
        {
            TDist[1] = (m_cellSize * (1 + nY)) / SinG;    // hypotenuse
        }

        if (fabs(TDist[1]) < 1e-9)
        {
            TestDir = 0.5;    // just make if smaller than 1.0
        }
        else
        {
            TestDir = fabs(TDist[0] / TDist[1]);
        }

        if (TestDir<1.0)
        {
            TDist[1] = sqrt(pow2(TDist[0]) - pow2(m_cellSize * (aXmid + nX)));
            TDist[0] = m_cellSize * (aXmid + nX);
            nX += 1.0;
        }
        else if (TestDir > 1.0)
        {
            TDist[0] = sqrt(pow2(TDist[1]) - pow2(m_cellSize * (1.0 + nY)));
            TDist[1] = m_cellSize * (1.0 + nY);
            nY += 1.0;
        }
        else
        {
            TDist[1] = sqrt(pow2(TDist[0]) - pow2(m_cellSize * (aXmid + nX)));
            TDist[0] = m_cellSize * (aXmid + nX);
            nY += 1.0;
            nX += 1.0;
        }
        Dist[0] = TDist[0] - Dist[0];
        Dist[1] = TDist[1] - Dist[1];

        TravelDist = sqrt(pow2(Dist[0]) + pow2(Dist[1]));
        TravelTime += TravelDist / (ExpRate * ROS);

        // Copy new dimensions to old dimensions
        Dist[0] = TDist[0];
        Dist[1] = TDist[1];
    }
    if (Flank && m_lbRatio > 1.0)
    {
        if (fabs(Theta) < PI / 2.0)
        {
            FlankTime =
                fastFlankTime(XStart, YStart, Xmid, XEnd, YEnd, NumX, Ros);
            if (FlankTime < TravelTime)
            {
                TravelTime = FlankTime;
            }
        }
    }

    return(TravelTime);
}

//------------------------------------------------------------------------------
// End of randthread.cpp
//------------------------------------------------------------------------------

