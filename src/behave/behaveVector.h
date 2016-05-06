#ifndef BEHAVEVECTOR_HEADER
#define BEHAVEVECTOR_HEADER

#include <vector>
#include "behaveRun.h" 
#include "fuelModelSet.h"

class BehaveVector
{
public:
    BehaveVector();
    ~BehaveVector();
    BehaveVector(const BehaveVector &rhs);
    BehaveVector& operator= (const BehaveVector& rhs);

    // Overloaded element access methods for member vector<BehaveRun> behaveRun
    BehaveRun& operator[] (const std::vector<BehaveRun>::size_type index);
    BehaveRun& at(const std::vector<BehaveRun>::size_type index);
    const BehaveRun& at(const std::vector<BehaveRun>::size_type index) const;

    // Overloaded capacity methods for member vector<BehaveRun> behaveRun
    void clear();
    bool empty();
    void resize(std::vector<BehaveRun>::size_type newSize);
    std::vector<BehaveRun>::size_type size() const;

    // Overloaded iterator methods for member vector<BehaveRun> behaveRun
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
    FuelModelSet fuelModelSet_;
    std::vector<BehaveRun> behaveRun;
};

#endif // BEHAVEVECTOR_HEADER