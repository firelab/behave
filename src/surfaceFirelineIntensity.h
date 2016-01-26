#ifndef SURFACEFIREFIRELINEINTENSITY_HEADER
#define SURFACEFIREFIRELINEINTENSITY_HEADER

// dependencies:
//		surfaceFireReactionIntensity,
//		surfaceFireResidenceTime

// optional dependencies
//		surfaceFireForwardSpreadRate,
//		surfaceFireSpreadRateatBeta,
//		surfaceNoWindNoSlopeSpreadRate,
//		surfaceFireBackingSpreadRate

class SurfaceFireFirelineIntensity
{	
public:
	double FBL_SurfaceFireFirelineIntensity(double spreadRate,
			double reactionIntensity, double residenceTime );
	double FireLineIntensityAtHead(double vSurfaceFireResidenceTime, 
			double vSurfaceFireSpreadAtHead, double vSurfaceFireReactionIntensity);
	void FireFlameLengAtVector(void);
private:
};

#endif //SURFACEFIREFIRELINEINTENSITY_HEADER