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
    std::vector<BehaveRun>::const_iterator begin() const;
    std::vector<BehaveRun>::const_iterator cbegin() const;
    std::vector<BehaveRun>::iterator end();
    std::vector<BehaveRun>::const_iterator end() const;
    std::vector<BehaveRun>::const_iterator cend() const;
    std::vector<BehaveRun>::reverse_iterator rbegin();
    std::vector<BehaveRun>::const_reverse_iterator rbegin() const;
    std::vector<BehaveRun>::const_reverse_iterator crbegin() const;
    std::vector<BehaveRun>::reverse_iterator rend();
    std::vector<BehaveRun>::const_reverse_iterator rend() const;
    std::vector<BehaveRun>::const_reverse_iterator crend() const;

private:
    FuelModels fuelModels_;
    std::vector<BehaveRun> behaveRun;
};

#endif // BEHAVEVECTOR_HEADER