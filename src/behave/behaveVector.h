#ifndef BEHAVEVECTOR_HEADER
#define BEHAVEVECTOR_HEADER

#include <vector>
#include "behaveRun.h" 

class BehaveVector
{
public:
    BehaveVector();
    BehaveRun& operator[] (const size_t i);
    
    void clear();
    bool empty();
    void resize(size_t i);
    const size_t size();

    std::vector<BehaveRun>::iterator begin();
    std::vector<BehaveRun>::iterator end();
    std::vector<BehaveRun>::reverse_iterator rbegin();
    std::vector<BehaveRun>::reverse_iterator rend();

private:
    FuelModels fuelModels_;
    std::vector<BehaveRun> behaveRun;
};

#endif // BEHAVEVECTOR_HEADER