// For now this class is contained within SurfaceFuelbedIntermediates - WMC 10/2015

//#include "surfaceFuelbedHeatSink.h"
//#include <math.h>
//
//SurfaceFuelbedHeatSink::SurfaceFuelbedHeatSink(const SurfaceFuelbedIntermediates& surfaceFuelbedIntermediates)
//{
//	surfaceFuelbedIntermediates_ = &surfaceFuelbedIntermediates;
//}
//
//double SurfaceFuelbedHeatSink::calculateHeatSink()
//{
//	double qigLive[MAX_SIZES]; // Heat of preigintion for live fuels
//	double qigDead[MAX_SIZES]; // Heat of preigintion for dead fuels
//	
//	// Initialize variables
//	heatSink_ = 0;
//	for (int i = 0; i < MAX_SIZES; i++)
//	{
//		qigLive[i] = 0.0;
//		qigDead[i] = 0.0;
//	}
//
//	for (int i = 0; i < MAX_SIZES; i++)
//	{
//		if (surfaceFuelbedIntermediates_->savrDead_[i] > 1e-6)
//		{
//			qigDead[i] = 250.0 + 1116.0 *  surfaceFuelbedIntermediates_->moistureDead_[i];
//			heatSink_ += surfaceFuelbedIntermediates_->areaWeightingFactorForLifeState_[DEAD] * surfaceFuelbedIntermediates_->areaWeightingFactorDead_[i] 
//				* qigDead[i] * exp(-138.0 / surfaceFuelbedIntermediates_->savrDead_[i]);
//		}
//		if (surfaceFuelbedIntermediates_->savrLive_[i] > 1e-6)
//		{
//			qigLive[i] = 250.0 + 1116.0 *  surfaceFuelbedIntermediates_->moistureLive_[i];
//			heatSink_ += surfaceFuelbedIntermediates_->areaWeightingFactorForLifeState_[LIVE] * surfaceFuelbedIntermediates_->areaWeightingFactorLive_[i] 
//				* qigLive[i] * exp(-138.0 / surfaceFuelbedIntermediates_->savrLive_[i]);
//		}
//	}
//	heatSink_ *= surfaceFuelbedIntermediates_->bulkDensity_;
//	return heatSink_;
//}