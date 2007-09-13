// -*- C++ -*-
// Package:    SiStripChannelGain
// Class:      SiStripGainTickMarkCalculator
// Original Author:  G. Bruno
//         Created:  Mon May 20 10:04:31 CET 2007
// $Id: SiStripGainTickMarkCalculator.cc,v 1.2 2007/07/30 13:41:48 gbruno Exp $

#include "CalibTracker/SiStripChannelGain/plugins/SiStripGainTickMarkCalculator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"



// #include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h" 
// #include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
// #include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
// #include "Geometry/CommonDetUnit/interface/GeomDetType.h"
// #include "Geometry/CommonTopologies/interface/StripTopology.h"
// #include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
// #include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"

#include "DataFormats/SiStripCommon/interface/ConstantsForPresentation.h"

#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"


using namespace cms;
using namespace std;


SiStripGainTickMarkCalculator::SiStripGainTickMarkCalculator(const edm::ParameterSet& iConfig) : ConditionDBWriter<SiStripApvGain>::ConditionDBWriter<SiStripApvGain>(iConfig), histos_(0), runNumber_(0){

  
  edm::LogInfo("SiStripGainTickMarkCalculator::SiStripGainTickMarkCalculator");

  mui_ =  new MonitorUIRoot() ;


  outputFileName_=iConfig.getUntrackedParameter<std::string>("HistoFile", "output.root");

//   if (Mode==std::string("Gaussian")) GaussianMode_=true;
//   else if (IOVMode==std::string("Constant")) ConstantMode_=true;
//   else  edm::LogError("SiStripGainTickMarkCalculator::SiStripGainTickMarkCalculator(): ERROR - unknown generation mode...will not store anything on the DB") << std::endl;

//   detid_apvs_.clear();

//   meanGain_=iConfig.getParameter<double>("MeanGain");
//   sigmaGain_=iConfig.getParameter<double>("SigmaGain");
//   minimumPosValue_=iConfig.getParameter<double>("MinPositiveGain");


  printdebug_ = iConfig.getUntrackedParameter<bool>("printDebug", false);


}


SiStripGainTickMarkCalculator::~SiStripGainTickMarkCalculator(){

  edm::LogInfo("SiStripGainTickMarkCalculator::~SiStripGainTickMarkCalculator");


  if ( histos_ ) { 
    histos_->remove(); 
    delete histos_; 
  }

  if ( mui_ ) { 
    if ( mui_->getBEInterface() ) { 
      mui_->getBEInterface()->setVerbose(0); 
    }
    delete mui_; 
  }
  
}



void SiStripGainTickMarkCalculator::algoAnalyze(const edm::Event & event, const edm::EventSetup& iSetup){


}


SiStripApvGain * SiStripGainTickMarkCalculator::getNewObject() {

  std::cout<<"SiStripGainTickMarkCalculator::getNewObject called"<<std::endl;

  std::vector<std::string> contents;

  //had to comment because method is protected
  mui_->getContents( contents ); 

  histos_ = new OptoScanHistograms( mui_ );

  //get run type from histo content. Maybe unnecessary 
  //  runType_ = histos_->runType( mui_->getBEInterface(), contents );
  //get run number from histo content
  runNumber_ = histos_->runNumber( mui_->getBEInterface(), contents );
  //collate if necessary
  //histos_->createCollations( contents );
  histos_->extractHistograms( contents );


  //if ( mui_ ) { mui_->doSummary(); }

  //perform analysis 
  histos_->histoAnalysis(printdebug_);


  //figure out how to pass enum variables
  //  pair<sistrip::Monitorable, sistrip::Presentation> summ0(OPTO_SCAN_MEASURED_GAIN, HISTO_1D);
  //  pair<std::string,sistrip::Granularity> summ1(sistrip::detectorView_ , APV);


  sistrip::Monitorable mymon = sistrip::OPTO_SCAN_MEASURED_GAIN;
  sistrip::Presentation mypres = sistrip::HISTO_1D;
  //  sistrip::Granularity mygran = sistrip::APV;
  sistrip::Granularity mygran = sistrip::FED_CHANNEL;

  histos_->createSummaryHisto(mymon, mypres, "ControlView", mygran);

  // histos_->createSummaryHisto(summ0, summ1 );

  //save histos
  histos_->save( outputFileName_, runNumber_ ); 
  //  bei_->save();



   SiStripApvGain * obj = new SiStripApvGain();





  //  const std::map<uint32_t,OptoScanAnalysis*> analyses = histos_->getData();



  //  for(std::map<uint32_t,OptoScanAnalysis*>::const_iterator it = analyses.begin(); it != analyses.end(); it++){

    //Generate Gain for det detid
//     std::vector<float> theSiStripVector;
//     for(unsigned short j=0; j<it->second; j++){
//       float gain;

//       //      if(sigmaGain_/meanGain_ < 0.00001) gain = meanGain_;
//       //      else{
//       gain = RandGauss::shoot(meanGain_, sigmaGain_);
//       if(gain<=minimumPosValue_) gain=minimumPosValue_;
//       //      }

//       if (printdebug_)
// 	edm::LogInfo("SiStripGainCalculator") << "detid " << it->first << " \t"
// 					      << " apv " << j << " \t"
// 					      << gain    << " \t" 
// 					      << std::endl; 	    
//       theSiStripVector.push_back(gain);
//     }
    
    
//     SiStripApvGain::Range range(theSiStripVector.begin(),theSiStripVector.end());
//     if ( ! obj->put(it->first,range) )
//       edm::LogError("SiStripGainCalculator")<<"[SiStripGainCalculator::beginJob] detid already exists"<<std::endl;

//   }


//}  
  return obj;


}
