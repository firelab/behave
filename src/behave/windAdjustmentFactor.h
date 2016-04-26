#ifndef WINDADJUSTMENTFACTOR_HEADER
#define WINDADJUSTMENTFACTOR_HEADER

#include "surfaceEnums.h"

class WindAjustmentFactor
{
public:
    WindAjustmentFactor();
    double calculateWindAdjustmentFactor(double canopyCover, double canopyHeight,
        double crownRatio, double fuelBedDepth);
    double getCanopyCrownFraction() const;
    WindAdjustmentFactorMethod::WindAdjustmentFactorMethodEnum getWindAdjustmentFactorMethod() const;

private:
    double	windAdjustmentFactor_;
    double	canopyCrownFraction_;
    WindAdjustmentFactorMethod::WindAdjustmentFactorMethodEnum windAdjustmentFactorMethod_;
};

#endif //WINDADJUSTMENTFACTOR_HEADER
