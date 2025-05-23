#ifndef CROWNFIREBRANDPROCESSOR_H
#define CROWNFIREBRANDPROCESSOR_H

/*!	\class Sem::Fire::Crown::Firebrand::Processor
  \brief Determines the height and drift distance of a firebrand of
  specified diameter from an active crown fire.

  This class is based upon Frank Albini's	'Program dist', a "program for
  predicting spotting distance from an active crown fire in uniformly
  forested flat terrain".
*/
class CrownFirebrandProcessor
{
public:
    CrownFirebrandProcessor(double canopyHt=0.,
                            double fireInt=0.,
                            double windSpeed=0.,
                            double windHt=0.,
                            double emberDiam=1.,
                            double ds=0.2);
    virtual ~CrownFirebrandProcessor(){}

    // These are the primary output property accessors
    double getFirebrandDistance( void ) { update(); return m_spotDist; }
    double getFirebrandHeight( void ) { update(); return m_loftHt; }
    double getFlameHeight( void ) { update(); return m_flameHt; }
    double getWindSpeedAtCanopy( void ) { update(); return m_canopyWind; }

    // These are the intermediate output property accessors
    double getDiameterFactor( void ) { update(); return m_eta; };
    double getFavreU( void ) { update(); return m_uf; }
    double getFavreV( void ) { update(); return m_vf; }
    double getFavreW( void ) { update(); return m_wf; }
    int    getLoftLayer( void ) { update(); return m_layer; }
    double getNormalizedDriftDist( void ) { update(); return m_driftX; }
    double getNormalizedFlameHeight( void ) { update(); return m_hf; }
    double getNormalizedLoftDistance( void ) { update(); return m_loftX; }
    double getNormalizedLoftHeight( void ) { update(); return m_loftZ; }
    double getNormalizedWindSpeed( void ) { update(); return m_uc; }
    double getPlumeAngle( void ) { update(); return m_ang; }
    double getPlumeMassDensity( void ) { update(); return m_rhof; }
    double getPlumeMassFlux( void ) { update(); return m_fm; }
    double getPlumeWidth( void ) { update(); return m_bf; }
    double getQRequired( void ) { update(); return m_qreq; }
    double getWn( void ) { update(); return m_wn; }

    // These are input property mutators
    void set( double canopyHt=0., double fireInt=0., double windSpeed=0.,
              double windHt=0., double emberDiam=1., double ds=0.2 ) ;
    void setCanopyHeight( double canopyHt=0. ) ;
    void setDeltaStep( double deltaStep=0.2 ) ;
    void setEmberDiameter( double emberDiam=1. ) ;
    void setFirelineIntensity( double fireInt=0. ) ;
    void setWindSpeed( double windSpeed=0. ) ;
    void setWindSpeedHeight( double windHt=0. ) ;

private:
    void process( void ) ;
    void processCanopyWindSpeed( void ) ;
    void processFirebrandDrift( void ) ;
    void processFirebrandLoft( void ) ;
    void processFlameBoundaryConditions( void ) ;
    void processFlameHeight( void ) ;
    void reset( void ) ;
    void update( void ) ;

private:
    // Housekeeping
    bool   m_isDirty;	// TRUE if any inputs have changed since last request
    // Input properties
    double m_canopyHt;	// Mean height of the active crown fire canopy (m)
    double m_deltaStep;	// Albini's plume layer data step size
    double m_emberDiam;	// Diameter of a viable firebrand ember when alighting at the fuel surface (mm)
    double m_fireInt;	// Mean flame height above the canopy top (m)
    double m_windHt;	// Height at which the wind speed was measured (m)
    double m_windSpeed;	// Mean windspeed at the top of the canopy (km/h)
    // Primary output properties
    double m_canopyWind;// Mean wind speed at the canopy top (m/s)
    double m_flameHt;	// Mean flame height above the canopy top (m)
    double m_loftHt;	// Maximum firebrand loft height (m)
    double m_spotDist;	// Maximum spotting distance (m)
    // Intermediate output properties (these use Albini's variable names, with a 'm_' prefix)
    double m_ang;		// Angle of the plume centerline above horizontal
    double m_bf;		// Plume width normal to the plume centerline
    double m_driftX;	// Normalized downwind drift distance (m / m_canopyHt)
    double m_eta;		// Safety factor for the ember diameter on impact ( m_eta > 1. )
    double m_fm;		// Mass flux in plume flow per unit length of fire edge
    double m_hf;		// Normalized flame height above crown top (i.e., m_flameHt / m_canopyHt)
    int    m_layer;		// Plume layer (base 0) into which the ember is lofted
    double m_loftX;		// Normalized coordinate firebrand loft height (m / m_canopyHt)
    double m_loftZ;		// Normalized coordinate firebrand loft height (m / m_canopyHt)
    double m_qfac;		// Subroutine drop(), constant for a given m_uc
    double m_qreq;		// The required q (normalized dynamic pressure of vertical flow component) for the m_emberDiam
    double m_rhof;		// Mean mass density of the plume flow
    double m_uc;		// Normalized wind speed at the crown top (i.e., m_canopyWind / m_wn );
    double m_uf;		// Horizontal component of Favre avereage plume flow velocity
    double m_vf;		// Favre avereage plume flow velocity
    double m_wf;		// Vertical component of Favre avereage plume flow velocity
    double m_wn;		// Factor used to adjust wind speed to canopy (i.e., log( g * m_canopyHt ) )
};

#endif
