/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Provides an interface for doing multiple Behave runs using a vector
* Author:   William Chatham <wchatham@fs.fed.us>
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

#include "behaveVector.h"
#include <assert.h>

BehaveVector::BehaveVector()
    :behaveRun(20, BehaveRun(fuelModelSet_))
{    
   
}

BehaveVector::~BehaveVector()
{

}

BehaveVector::BehaveVector(const BehaveVector &rhs)
    :behaveRun(rhs.size(), BehaveRun(fuelModelSet_))
{
    fuelModelSet_ = rhs.fuelModelSet_;
    if (size() != rhs.size())
    {
        resize(rhs.size());
    }
    for (int i = 0; i < rhs.size(); i++)
    {
        behaveRun[i] = rhs.behaveRun[i];
        // since behaveRun's assignment operator copys the rhs behaveRun's fuelModelSet_ pointer
        // we need to point the lhs behaveRun back to this object's fuelModelSet_'s memory location
        behaveRun[i].setFuelModelSet(fuelModelSet_);
    }
}

BehaveVector& BehaveVector::operator= (const BehaveVector& rhs)
{
    if (this != &rhs)
    {
        fuelModelSet_ = rhs.fuelModelSet_;
        if (size() != rhs.size())
        {
            resize(rhs.size());
        }
        for (int i = 0; i < rhs.size(); i++)
        {
            behaveRun[i] = rhs.behaveRun[i];
            // since behaveRun's assignment operator copys the rhs behaveRun's fuelModelSet_ pointer
            // we need to point the lhs behaveRun back to this object's fuelModelSet_'s memory location
            behaveRun[i].setFuelModelSet(fuelModelSet_);
        }
    }
    return *this;
}

BehaveRun& BehaveVector::operator[] (const std::vector<BehaveRun>::size_type index)
{ 
    assert(index >= 0 && index < behaveRun.size()); // for bounds checking in debug
    return behaveRun[index]; 
}

BehaveRun&  BehaveVector::at(const std::vector<BehaveRun>::size_type index)
{
    return behaveRun.at(index);
}

const BehaveRun&  BehaveVector::at(const std::vector<BehaveRun>::size_type index) const
{
    return behaveRun.at(index);
}

void BehaveVector::clear()
{
    behaveRun.clear();
}

bool BehaveVector::empty()
{
    return behaveRun.empty();
}

void BehaveVector::resize(std::vector<BehaveRun>::size_type newSize)
{
    behaveRun.resize(newSize, behaveRun[0]);
}

std::vector<BehaveRun>::size_type BehaveVector::size() const
{
    std::vector<BehaveRun>::size_type size = behaveRun.size();
    return size;
}

std::vector <BehaveRun>::iterator BehaveVector::begin()
{
    std::vector <BehaveRun>::iterator myIterator = behaveRun.begin();
    return myIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::begin() const
{
    std::vector <BehaveRun>::const_iterator myConstantIterator = behaveRun.cbegin();
    return myConstantIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::cbegin() const
{
    std::vector <BehaveRun>::const_iterator myConstantIterator = behaveRun.cbegin();
    return myConstantIterator;
}

std::vector <BehaveRun>::iterator BehaveVector::end()
{
    std::vector <BehaveRun>::iterator myIterator = behaveRun.end();
    return myIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::end() const
{
    std::vector <BehaveRun>::const_iterator myConstantIterator = behaveRun.cend();
    return myConstantIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::cend() const
{
    std::vector <BehaveRun>::const_iterator myConstantIterator = behaveRun.cend();
    return myConstantIterator;
}

std::vector <BehaveRun>::reverse_iterator BehaveVector::rbegin()
{
    std::vector <BehaveRun>::reverse_iterator myReverseIterator = behaveRun.rbegin();
    return myReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::rbegin() const
{
    std::vector <BehaveRun>::const_reverse_iterator myConstantReverseIterator = behaveRun.crbegin();
    return myConstantReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::crbegin() const
{
    std::vector <BehaveRun>::const_reverse_iterator myConstantReverseIterator = behaveRun.crbegin();
    return myConstantReverseIterator;
}

std::vector <BehaveRun>::reverse_iterator BehaveVector::rend()
{
    std::vector <BehaveRun>::reverse_iterator myReverseIterator = behaveRun.rend();
    return myReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::rend() const
{
    std::vector <BehaveRun>::const_reverse_iterator myConstantReverseIterator = behaveRun.crend();
    return myConstantReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::crend() const
{
    std::vector <BehaveRun>::const_reverse_iterator myConstantReverseIterator = behaveRun.crend();
    return myConstantReverseIterator;
}
