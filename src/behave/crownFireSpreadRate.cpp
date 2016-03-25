#include "crownFireSpreadRate.h"

#include <iostream> // Test

CrownFireSpreadRate::CrownFireSpreadRate()
{
	
}

CrownFireSpreadRate::~CrownFireSpreadRate()
{

}

//------------------------------------------------------------------------------
/*! \brief Calculates the crown fire spread rate.
*
*  This uses Rothermel's 1991 crown fire correlation.
*
*  \param windAt20Ft   Wind speed at 20 ft above the canopy (mi/h).
*  \param mc1          Dead 1-h fuel moisture content (lb water/lb fuel).
*  \param mc10         Dead 10-h fuel moisture content (lb water/lb fuel).
*  \param mc100        Dead 100-h fuel moisture content (lb water/lb fuel).
*  \param mcWood       Live wood fuel moisture content (lb water/lb fuel).
*
*  \return Crown fire average spread rate (ft/min).
*/

double CrownFireSpreadRate::FBL_CrownFireSpreadRate(double windAt20Ft, double mc1, double mc10,
	double mc100, double mcWood )

{
	// TODO: Work out a way to better abstract away implementation details shared between the objects used 
	// in this method. Object cohesion is very bad at this point, we are slowly moving away from the globals
	// that ran rampant in the legacy non-OOD system. - WMC 08/2015

	//--------------------------------------------------------------------------
	// Step 1: Create the crown fuel model (fire behavior fuel model 10)
	//--------------------------------------------------------------------------

	// Objects needed for calculation of spread rate
	// These objects are allocated on the stack, thus they live and die all within one call to this method
	// These eliminates the need to worry about the two pesky problems of manual memory manangement
	// as well as the issue of side-effects introduced from the persistence of state between method calls
	// - WMC 08/2015

	/*SurfaceFuelbedIntermediates surfaceFuelbedIntermediates;
	SurfaceFireNoWindNoSlopeSpreadRate 	surfaceFireNoWindNoSlopeSpreadRate;
	SurfaceFireReactionIntensity surfaceFireReactionIntensity;
	SurfaceFirePropogatingFlux surfaceFirePropagatingFlux;
	SurfaceFireForwardSpreadRate surfaceFireForwardSpreadRate;*/

	// Local variables
	// TODO: Consider grouping the following constants together into their own object to provide
	// better cohesion and encapsulation. This new object could then be used an input and output
	// parameter within the "pipeline" calculations in this method - WMC 08/2015
	//double m_slopeK;		//!< Slope constant K (see Rothermel 1972)
	//double m_windB;		//!< Wind constant B (see Rothermel 1972)
	//double m_windE;		//!< Wind constant E (see Rothermel 1972)
	//double m_windK;		//!< Wind constant K (see Rothermel 1972)

	static double depth = 1.0;
	static double deadFuelMext = 0.25;
	static int    particles = 4;
	static int    life[4] = { 0, 0, 0, 2 };
	static double load[4] = { 0.138, 0.092, 0.230, 0.092 };
	static double savr[4] = { 2000., 109., 30., 1500. };
	static double heat[4] = { 8000., 8000., 8000., 8000. };
	static double dens[4] = { 32.0, 32.0, 32.0, 32.0 };
	static double stot[4] = { .0555, .0555, .0555, .0555 };
	static double seff[4] = { .0100, .0100, .0100, .0100 };
	
	double mois[4];
	mois[0] = mc1;
	mois[1] = mc10;
	mois[2] = mc100;
	mois[3] = mcWood;

	double ros = 0;
	double sigma = 0;

	//--------------------------------------------------------------------------
	// Step 2: this is a major hack ....
	// Since the crown fire spread may be calculated as part of a linked
	// simulation with surface fire spread, we have to save and then restore
	// state for the surface fire variables.
	// This is the save part
	//
	// I think this hack may no longer needed thanks to the new object oriented 
	// architecture (Hopefully) - WMC 08/2015
	//--------------------------------------------------------------------------
	/*
	int    x_particles = m_particles;	//!< Number of fuel particles
	double x_liveMextK = m_liveMextK;	//!< Live moisture of extinction
	double x_slopeK = m_slopeK;			//!< Slope constant K (see Rothermel 1972)
	double x_windB = m_windB;			//!< Wind constant B (see Rothermel 1972)
	double x_windE = m_windE;			//!< Wind constant E (see Rothermel 1972)
	double x_windK = m_windK;			//!< Wind constant K (see Rothermel 1972)
	int    x_life[MAX_PARTS];			//!< Fuel particle life category
	double x_aWtg[MAX_PARTS];			//!< Fuel particle area weighting factor
	double x_load[MAX_PARTS];			//!< Fuel particle fuel load (lb/ft2)
	double x_sigK[MAX_PARTS];			//!< Fuel particle surface area-to-volume ratio (ft2/ft3)
	double x_lifeAwtg[MAX_CATS];		//!< Life category weighting factor
	double x_lifeFine[MAX_CATS];		//!< Fine fuel ratio by life category
	double x_lifeRxK[MAX_CATS];			//!< Reaction intensity constant by life category
	int i;
	for (i = 0; i<MAX_PARTS; i++)
	{
		x_life[i] = m_life[i];
		x_aWtg[i] = m_aWtg[i];
		x_load[i] = m_load[i];
		x_sigK[i] = m_sigK[i];		// WAS m_sigK[MAX_PARTS];
	}
	for (i = 0; i<MAX_CATS; i++)
	{
		x_lifeAwtg[i] = m_lifeAwtg[i];
		x_lifeFine[i] = m_lifeFine[i];
		x_lifeRxK[i] = m_lifeRxK[i];
	}
	*/

	////--------------------------------------------------------------------------
	//// Step 3: Determine fire behavior.
	////--------------------------------------------------------------------------
	//// TODO: Improve the way the intermediate variables are communicated between producers and consumers -WMC 08/2015
	//double fuelBedBulkDensity = 0.0;	// Returned by FBL_SurfaceFuelBedIntermediates()
	//double fuelBedPackingRatio = 0.0;   // Returned by FBL_SurfaceFuelBedIntermediates()
	//double fuelBedBetaRatio = 0.0;		// Returned by FBL_SurfaceFuelBedIntermediates()

	//sigma = surfaceFuelbedIntermediates.FBL_SurfaceFuelBedIntermediates(depth, deadFuelMext,
	//	particles, life, load, savr, heat, dens, stot, seff);

	//fuelBedBulkDensity = surfaceFuelbedIntermediates.Get_vSurfaceFuelBedBulkDensity();
	//fuelBedPackingRatio = surfaceFuelbedIntermediates.Get_vSurfaceFuelBedPackingRatio();
	//fuelBedBetaRatio = surfaceFuelbedIntermediates.Get_vSurfaceFuelBedBetaRatio();

	//// DEBUG
	//std::cout << "sigma is " << sigma << std::endl;

	//// TODO: Remove the need for the implemention details of m_liferxk to be exposed -WMC 08/2015
	//double m_lifeRxK[MAX_CATS];
	//m_lifeRxK[DEAD_CAT] = surfaceFuelbedIntermediates.Get_m_lifeRxKForCategory(DEAD_CAT);
	//m_lifeRxK[LIVE_CAT] = surfaceFuelbedIntermediates.Get_m_lifeRxKForCategory(LIVE_CAT);
	//
	//// TODO: Remove the need for the implemention details of Rothermal constants to be exposed -WMC 08/2015
	//m_slopeK = surfaceFuelbedIntermediates.Get_m_slopeK();
	//m_windB = surfaceFuelbedIntermediates.Get_m_windB();
	//m_windE = surfaceFuelbedIntermediates.Get_m_windE();
	//m_windK = surfaceFuelbedIntermediates.Get_m_windK();

	//double deadFuelMois = 0.0;  // Returned by FBL_SurfaceFuelBedHeatSink()
	//double liveFuelMois = 0.0;  // Returned by FBL_SurfaceFuelBedHeatSink()
	//double liveFuelMext = 0.0;  // Returned by FBL_SurfaceFuelBedHeatSink()

	//double heatSink = surfaceFuelbedIntermediates.FBL_SurfaceFuelBedHeatSink(fuelBedBulkDensity, deadFuelMext, mois);
	//double reactionIntensity = surfaceFireReactionIntensity.FBL_SurfaceFireReactionIntensity(
	//	deadFuelMois, deadFuelMext, liveFuelMois, liveFuelMext, m_lifeRxK);
	//double propagatingFlux = surfaceFirePropagatingFlux.FBL_SurfaceFirePropagatingFlux(
	//	fuelBedPackingRatio, sigma);
	//double ros0 = surfaceFireNoWindNoSlopeSpreadRate.FBL_SurfaceFireNoWindNoSlopeSpreadRate(reactionIntensity,
	//	propagatingFlux, heatSink);

	//// DEBUG
	//std::cout << "ros0 is " << ros0 << std::endl;

	//double slopeFraction = 0.0;
	//double windDirFromUpslope = 0.0;
	//double midflameWindSpeed = 0.4 * windAt20Ft;
	//double maxDirFromUpslope = 0.0;		// Returned by FBL_SurfaceFireForwardSpreadRate()
	//double effectiveWindSpeed = 0.0;    // Returned by FBL_SurfaceFireForwardSpreadRate()
	//double windSpeedLimit = 0.0;		// Returned by FBL_SurfaceFireForwardSpreadRate()
	//int    windLimitExceeded = 0;		// Returned by FBL_SurfaceFireForwardSpreadRate()
	//double windFactor = 0.0;			// Returned by FBL_SurfaceFireForwardSpreadRate()
	//double slopeFactor = 0.0;			// Returned by FBL_SurfaceFireForwardSpreadRate()

	//double 
	//
	//ros = surfaceFireForwardSpreadRate.FBL_SurfaceFireForwardSpreadRate(m_slopeK, m_windB, m_windE, m_windK,
	//	ros0, reactionIntensity, slopeFraction, midflameWindSpeed, windDirFromUpslope);

	//std::cout << "ros is " << ros << std::endl;
	
	// Rothermel 1991
	double crownRos = 3.34 * ros;

	//double residenceTime = FBL_SurfaceFireResidenceTime( sigma );
	//double firelineIntensity = FBL_SurfaceFireFirelineIntensity( ros,
	//    reactionIntensity, residenceTime );
	//double flameLength = FBL_SurfaceFireFlameLength( firelineIntensity );

	//--------------------------------------------------------------------------
	// Step 4: this is the rest of the major hack .... restore the state.
	//--------------------------------------------------------------------------
	/*
	// I think this hack is no longer needed thanks to the new object oriented
	// architecture - WMC 08/2015
	//
	m_particles = x_particles;
	m_liveMextK = x_liveMextK;
	m_slopeK = x_slopeK;
	m_windB = x_windB;
	m_windE = x_windE;
	m_windK = x_windK;
	for (i = 0; i<MAX_PARTS; i++)
	{
		m_life[i] = x_life[i];
		m_aWtg[i] = x_aWtg[i];
		m_load[i] = x_load[i];
		m_sigK[i] = x_sigK[i];		// WAS x_sigK[MAX_PARTS];
	}
	for (i = 0; i<MAX_CATS; i++)
	{
		m_lifeAwtg[i] = x_lifeAwtg[i];
		m_lifeFine[i] = x_lifeFine[i];
		m_lifeRxK[i] = x_lifeRxK[i];
	}
	//fprintf( stderr, "ros0=%f,  ros=%f,  crownRos=%f\n", ros0, ros, crownRos );
	*/
	return(crownRos);
}