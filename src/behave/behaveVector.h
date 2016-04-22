#ifndef BEHAVEVECTOR_HEADER
#define BEHAVEVECTOR_HEADER

#include "behaveRun.h" 

class BehaveVector
{
public:
    BehaveVector();
    inline BehaveRun& operator[] (const int i) { return behaveRun[i]; };

    void clear();
    bool empty();
    void resize(int i);
    const size_t size();

private:
    FuelModels fuelModels_;
    std::vector <BehaveRun> behaveRun;
};

#endif // BEHAVEVECTOR_HEADER