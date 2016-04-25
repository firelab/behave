#include "BehaveVector.h"

BehaveVector::BehaveVector()
    :behaveRun(20, BehaveRun(fuelModels_))
{    
   
}

BehaveRun& BehaveVector::operator[] (const size_t i)
{ 
    return behaveRun[i]; 
}

void BehaveVector::clear()
{
    behaveRun.clear();
}

bool BehaveVector::empty()
{
    return behaveRun.empty();
}

void BehaveVector::resize(size_t i)
{
    behaveRun.resize(i, behaveRun[0]);
}

const size_t BehaveVector::size()
{
    size_t size = behaveRun.size();
    return size;
}

std::vector <BehaveRun>::iterator BehaveVector::begin()
{
    std::vector <BehaveRun>::iterator myIterator = behaveRun.begin();
    return myIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::begin() const
{
    std::vector <BehaveRun>::const_iterator myconstantIterator = behaveRun.cbegin();
    return myconstantIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::cbegin() const
{
    std::vector <BehaveRun>::const_iterator myconstantIterator = behaveRun.cbegin();
    return myconstantIterator;
}

std::vector <BehaveRun>::iterator BehaveVector::end()
{
    std::vector <BehaveRun>::iterator myIterator = behaveRun.end();
    return myIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::end() const
{
    std::vector <BehaveRun>::const_iterator myconstantIterator = behaveRun.cend();
    return myconstantIterator;
}

std::vector<BehaveRun>::const_iterator BehaveVector::cend() const
{
    std::vector <BehaveRun>::const_iterator myconstantIterator = behaveRun.cend();
    return myconstantIterator;
}

std::vector <BehaveRun>::reverse_iterator BehaveVector::rbegin()
{
    std::vector <BehaveRun>::reverse_iterator myReverseIterator = behaveRun.rbegin();
    return myReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::rbegin() const
{
    std::vector <BehaveRun>::const_reverse_iterator myconstantReverseIterator = behaveRun.crbegin();
    return myconstantReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::crbegin() const
{
    std::vector <BehaveRun>::const_reverse_iterator myconstantReverseIterator = behaveRun.crbegin();
    return myconstantReverseIterator;
}

std::vector <BehaveRun>::reverse_iterator BehaveVector::rend()
{
    std::vector <BehaveRun>::reverse_iterator myReverseIterator = behaveRun.rend();
    return myReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::rend() const
{
    std::vector <BehaveRun>::const_reverse_iterator myconstantReverseIterator = behaveRun.crend();
    return myconstantReverseIterator;
}

std::vector<BehaveRun>::const_reverse_iterator BehaveVector::crend() const
{
    std::vector <BehaveRun>::const_reverse_iterator myconstantReverseIterator = behaveRun.crend();
    return myconstantReverseIterator;
}
