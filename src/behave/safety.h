/******************************************************************************
*
* Project:  CodeBlocks
* Purpose:  Class for calculating the safety zone size, separation distance, 
*           radius
*          
* Author:   William Chatham <wchatham@fs.fed.us>
* Credits:  Some of the code in the corresponding cpp file is, in part or in
*           whole, from BehavePlus5 source originally authored by Collin D.
*           Bevins and is used with or without modification.
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

#ifndef SAFETY_H
#define SAFETY_H

class Safety
{
public:
    void setFlameHeight(double flameHeight);
    void setNumberOfPersonnel(double numberOfPersonnel);
    void setAreaPerPerson(double areaPerPerson);
    void setNumberOfEquipment(double numberOfEquipment);
    void setAreaPerEquipment(double areaPerEquipment);

    void updateSafetyInputs(double flameHeight, double numberOfPersonnel, double areaPerPerson,
        double numberOfEquipment, double areaPerEquipment);
    void calculateSafetyZoneRadius();
    void calculateSafetyZoneSeparationDistance();
private:
    //Inputs
    double flameHeight_;        // flame height (ft)
    double numberOfPersonnel_;
    double areaPerPerson_;      // Mean area required per person within safety zone (ft^2)
    double numberOfEquipment_;  
    double areaPerEquipment_;   // Mean area required per piece of equipment within safety zone (ft^2)

    // Outputs
    double separationDistance_;
    double safetyZoneRadius_;
    double safetyZoneArea_;
};

#endif  // SAFETY_H
