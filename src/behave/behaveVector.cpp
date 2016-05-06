#include "BehaveVector.h"
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
    for (int i = 0; i < rhs.size(); i++)
    {
        behaveRun[i] = rhs.behaveRun[i];
    }
}

BehaveVector& BehaveVector::operator = (const BehaveVector& rhs)
{
    if (this != &rhs)
    {
        for (int i = 0; i < rhs.size(); i++)
        {
            behaveRun[i] = rhs.behaveRun[i];
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
