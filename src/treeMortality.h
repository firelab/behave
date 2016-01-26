#ifndef TREEMORTALITY_HEADER
#define TREEMORTALITY_HEADER

#include <stddef.h>


//dependencies: none

//optional dependencies: surfaceFirelineIntensity

class TreeMortality
{
	typedef struct _fofemSpecies
	{
		char	*abbrev;		// Genus-species abbreviation
		int		mortEq;			// Index to mortality equation (base 1)
		int		barkEq;			// Index to single bark thickness equation (base 1)
		int		regions;		// Region list (any combination of 1, 2, 3, and/or 4)
		char	*scientific;	// Scientific name
		char	*common;		// Common name
	}FofemSpeciesStruct;

public:
	TreeMortality(); //Default Constructor
	//~TreeMortality(); // Default Destructor

	double CalculateSurfaceFireScorchHeight(double firelineIntensity, double windSpeed,
			double airTemperature);
	double FBL_TreeBarkThicknessFofem(int speciesIndex, double dbh);
	double FBL_TreeCrownScorch(double treeHt, double crownRatio, double scorchHt, 
			double *crownLengthScorched, double *crownLengthFractionScorched);
	double FBL_TreeMortalityFofem(int speciesIndex, double barkThickness,
			double crownVolScorched, double scorchHt);
	double FBL_TreeMortalityHood(int speciesIndex, double dbh, 
			double crownLengScorched, double crownVolScorched );
	double TreeMortalityRateFofemHoodAtVector( int speciesIndex, double dbh, 
			double crownLengFractionScorched, double crownVolScorched, 
			double barkThickness, double surfaceFireScorchHt);
	double FireScorchHtFromFlameLengAtVector(double airTemp, double windSpeed, double flameLength, 
		double firelineIntensity);

private:
	const double SMIDGEN = 1.0e-07; // Number used to test for "close enough to zero" to prevent divide - by - zero, sqrt(0), etc
	static const int NUM_SPECIES = 210; // Number of tree species
	FofemSpeciesStruct FofemSpeciesArray[NUM_SPECIES];
	double Fofem_Sbt[40];
};

#endif //TREEMORTALITY_HEADER
