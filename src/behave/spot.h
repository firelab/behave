#ifndef SPOT_HEADER
#define SPOT_HEADER



class Spot
{
public:
    Spot();
    ~Spot();

    double spotCriticalCoverHeight(double firebrandHeight, double coverHeightt);
    double spotDistanceMountainTerrain(double flatDistance, int location, double ridgeToValleyDistance, 
        double ridgeToValleyElevation);
    double spotDistanceFlatTerrain(double firebrandHeight, double coverHeightt, double windSpeedAtTwentyFeet);
    double spotDistanceFromSurfaceFire(int location, double ridgeToValleyDistance, double ridgeToValleyElevation,
        double coverHeight, double windSpeedAtTwentyFeet, double flameLength);
    double spotDistanceFromBurningPile(int location, double ridgeToValleyDistance, double ridgeToValleyElevation,
        double coverHeight, double windSpeedAtTwentyFeet, double flameHeight);
    double spotDistanceFromTorchingTrees(int location, double ridgeToValleyDistance, double ridgeToValleyElevation,
        double coverHeight, double windSpeedAtTwentyFeet, double torchingTrees, double treeDBH, double treeHeight,
        int treeSpecies);

    double getCoverHeightUsed();
    double getFlameHeight();
    double getFlameRatio();
    double getFlameDuration();
    double getMaxFirebrandHeight();
    double getMaxFlatTerrainSpottingDistance();
    double getMaxMountainTerrainSpottingDistance();   

private:
    static const int NUM_SPECIES = 14;
    static const int NUM_COLS = 2;
    static const int NUM_FIREBRAND_ROWS = 4;

    double speciesFlameHeightParameters_[NUM_SPECIES][NUM_COLS];
    double speciesFlameDurationParameters_[NUM_SPECIES][NUM_COLS];
    double firebrandHeightFactors_[NUM_FIREBRAND_ROWS][NUM_COLS];

    double heightUsed_;         // Actual tree / vegetation ht used(ft).
    double flameHeight_;        // Steady state flame ht(ft).
    double flameRatio_;         // Ratio of tree height to steady flame height(ft / ft).
    double firebrandDrift_;     // Maximum firebrand drift(mi).
    double flameDuration_;      // Flame duration(dimensionless).
    double firebrandHeight_;    // Initial maximum firebrand height(ft).
    double flatDistance_;       // Maximum spotting distance over flat terrain(mi).
    double mountainDistance_;   // Maximum spotting distance over mountain terrain(mi).
};

#endif // SPOT_HEADER
