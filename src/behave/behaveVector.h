/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Provides an interface for doing multiple Behave runs using a vector
* Author:   William Chatham <wchatham@fs.fed.us>
*
******************************************************************************
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
*****************************************************************************/

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