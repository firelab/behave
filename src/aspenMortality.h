#ifndef ASPENMORTALITY_HEADER
#define ASPENMORTALITY_HEADER

//dependencies: surfaceFireFlameLength

class AspenMortality
{	
public:
	AspenMortality(); // Default Constructor
	~AspenMortality(); // Default Destructor

	double CalculateAspenMortality(int severity, double flameLength, double diameterBreastHeight);

private:
};
#endif //ASPENMORTALITY_HEADER