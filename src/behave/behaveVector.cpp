#include "BehaveVector.h"

BehaveVector::BehaveVector()
    :behaveRun(20, BehaveRun(fuelModels_))
{    
   
}

void BehaveVector::clear()
{
    behaveRun.clear();
}

bool BehaveVector::empty()
{
    return behaveRun.empty();
}

void BehaveVector::resize(int i)
{
    behaveRun.resize(i, behaveRun[0]);
}

const size_t BehaveVector::size()
{
    size_t size = behaveRun.size();
    return size;
}
