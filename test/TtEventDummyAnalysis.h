#ifndef TtEventDummyAnalysis_H
#define TtEventDummyAnalysis_H

/** \class TtEventDummyAnalysis
 *
 *  Author: S.C. Kao  - UC Riverside
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "AnalysisDataFormats/TopObjects/interface/TopLepton.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopObject.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopParticle.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopMET.h" 
#include "AnalysisDataFormats/TopObjects/interface/TopJet.h" 
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h" 
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h" 

#include "TtEventDummyAnalysisHisto.h"

#include "TFile.h"
#include <vector>
#include <map>
#include <string>
#include <utility> 


class TtEventDummyAnalysis : public edm::EDAnalyzer {

  public:

    /// Constructor
    TtEventDummyAnalysis(const edm::ParameterSet& pset);

    /// Destructor
    virtual ~TtEventDummyAnalysis();

    /// Perform the real analysis
    void analyze(const edm::Event & iEvent, const edm::EventSetup& iSetup);


  private: 

    // Histograms
    HTOP1 *h_Jet;
    HTOP2 *h_Mass;
    HTOP3 *h_Other;
    // TNtuple1 *tr_muon;

    // The file which will store the histos
    TFile *theFile;

    // Switch for debug output
    bool debug;

    std::string rootFileName;
    std::string leptonFlavour;
    edm::InputTag muonSrc;
    edm::InputTag electronSrc;
    edm::InputTag metSrc;
    edm::InputTag bjetSrc;
    edm::InputTag bjetObj;

    edm::InputTag evtsols;

};


#endif
