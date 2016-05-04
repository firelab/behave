#include "crownInputs.h"

double CrownInputs::getCanopyBaseHeight() const
{
    return canopyBaseHeight_;
}

double CrownInputs::getCanopyBulkDensity() const
{
    return canopyBulkDensity_;
}

double CrownInputs::getCanopyFlameLength() const
{
    return canopyUserProvidedFlameLength_;
}

double CrownInputs::getCanopyFirelineIntensity() const
{
    return canopyUserProvidedFirelineIntensity_;
}

double CrownInputs::getFoliarMoisture() const
{
    return foliarMoisture_;
}

void CrownInputs::setCanopyBaseHeight(double canopyBaseHeight)
{
    canopyBaseHeight_ = canopyBaseHeight;
}

void CrownInputs::setCanopyBulkDensity(double canopyBulkDensity)
{
    canopyBulkDensity_ = canopyBulkDensity;
}

void CrownInputs::setCanopyFlameLength(double canopyUserProvidedFlameLength)
{
    canopyUserProvidedFlameLength_ = canopyUserProvidedFlameLength;
}

void CrownInputs::setCanopyFirelineIntensity(double canopyUserProvidedFirelineIntensity)
{
    canopyUserProvidedFirelineIntensity_ = canopyUserProvidedFirelineIntensity;
}

void CrownInputs::setFoliarMoisture(double foliarMoisture)
{
    foliarMoisture_ = foliarMoisture;
}

void CrownInputs::updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture)
{
    canopyBaseHeight_ = canopyBaseHeight;
    canopyBulkDensity_ = canopyBulkDensity;
    foliarMoisture_ = foliarMoisture;
}
