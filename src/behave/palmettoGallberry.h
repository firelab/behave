#ifndef PALMETTOGALLBERRY_HEADER
#define PALMETTOGALLBERRY_HEADER

class PalmettoGallberry
{
public:
    PalmettoGallberry();
    PalmettoGallberry(const PalmettoGallberry &rhs);
    PalmettoGallberry& operator= (const PalmettoGallberry& rhs);

    void initialize();

    double calculatePalmettoGallberyDeadOneHourLoad(double ageOfRough, double heightOfUnderstory);
    double calculatePalmettoGallberyDeadTenHourLoad(double ageOfRough, double palmettoCoverage);
    double calculatePalmettoGallberyDeadFoliageLoad(double ageOfRough, double palmettoCoverage);
    double calculatePalmettoGallberyFuelBedDepth(double heightOfUnderstory);
    double calculatePalmettoGallberyLitterLoad(double ageOfRough, double overstoryBasalArea);
    double calculatePalmettoGallberyLiveOneHourLoad(double ageOfRough, double heightOfUnderstory);
    double calculatePalmettoGallberyLiveTenHourLoad(double ageOfRough, double heightOfUnderstory);
    double calculatePalmettoGallberyLiveFoliageLoad(double ageOfRough, double palmettoCoverage, double heightOfUnderstory);

    double getMoistureOfExtinctionDead() const;
    double getHeatOfCombustionDead() const;
    double getHeatOfCombustionLive() const;
    double getPalmettoGallberyDeadOneHourLoad() const;
    double getPalmettoGallberyDeadTenHourLoad() const;
    double getPalmettoGallberyDeadFoliageLoad() const;
    double getPalmettoGallberyFuelBedDepth() const;
    double getPalmettoGallberyLitterLoad() const;
    double getPalmettoGallberyLiveOneHourLoad() const;
    double getPalmettoGallberyLiveTenHourLoad() const;
    double getPalmettoGallberyLiveFoliageLoad() const;

private:
    double moistureOfExtinctionDead_;
    double heatOfCombustionDead_;
    double heatOfCombustionLive_;
    double palmettoGallberyDeadOneHourLoad_;
    double palmettoGallberyDeadTenHourLoad_;
    double palmettoGallberyDeadFoliageLoad_;
    double palmettoGallberyFuelBedDepth_;
    double palmettoGallberyLitterLoad_;
    double palmettoGallberyLiveOneHourLoad_;
    double palmettoGallberyLiveTenHourLoad_;
    double palmettoGallberyLiveFoliageLoad_;
};

#endif //PALMETTOGALLBERRY_HEADER
