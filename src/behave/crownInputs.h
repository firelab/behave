#ifndef CROWNINPUTS_HEADER
#define CROWNINPUTS_HEADER

class CrownInputs
{
public:
    void setCanopyBaseHeight(double canopyBaseHeight);
    void setCanopyBulkDensity(double canopyBulkDensity);
    void setCanopyFlameLength(double canopyUserProvidedFlameLength);
    void setCanopyFirelineIntensity(double canopyUserProvidedFirelineIntensity);
    void setFoliarMoisture(double foliarMoisture);

    double getCanopyBaseHeight() const;
    double getCanopyBulkDensity() const;
    double getCanopyFlameLength() const;
    double getCanopyFirelineIntensity() const;
    double getFoliarMoisture() const;

    void updateCrownInputs(double canopyBaseHeight, double canopyBulkDensity, double foliarMoisture);

private:
    double canopyBaseHeight_;
    double canopyBulkDensity_;
    double canopyUserProvidedFlameLength_;
    double canopyUserProvidedFirelineIntensity_;
    double foliarMoisture_;

};

#endif // CROWNINPUTS_HEADER
