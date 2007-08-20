// -*- C++ -*-
//
// Package:    TtEventDummyAnalysis
// Class:      TtEventDummyAnalysis
// 
/**\class TtEventDummyAnalysis TtEventDummyAnalysis.cc TopQuarkAnalysis/TtEventDummyAnalysis/src/TtEventDummyAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Shih-Chuan Kao
//         Created:  Thu Jul 26 13:59:47 CEST 2007
// $Id: TtEventDummyAnalysis.cc,v 1.2 2007/08/07 10:14:37 tsirig Exp $
//
//


// system include files
#include <memory>

// user include files
#include "TopQuarkAnalysis/TopEventProducers/test/TtEventDummyAnalysis.h"

#include "FWCore/Framework/interface/MakerMacros.h"


//
// constructors and destructor
//
TtEventDummyAnalysis::TtEventDummyAnalysis(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  debug             = iConfig.getUntrackedParameter<bool>   ("debug");
  rootFileName      = iConfig.getUntrackedParameter<string> ("rootFileName");
  leptonFlavour     = iConfig.getParameter<std::string>   ("leptonFlavour"); 
  muonSrc           = iConfig.getParameter<edm::InputTag> ("muonSource");
  electronSrc       = iConfig.getParameter<edm::InputTag> ("electronSource");
  metSrc            = iConfig.getParameter<edm::InputTag> ("metSource");
  bjetSrc           = iConfig.getParameter<edm::InputTag> ("bjetSource");
  evtsols           = iConfig.getParameter<edm::InputTag> ("EvtSolution");
  

  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");
  theFile->mkdir("Jets");
  theFile->cd();
  theFile->mkdir("Mass");
  theFile->cd();
  theFile->mkdir("Other");
  theFile->cd();


  h_Jet   = new HTOP1("Jets_");
  h_Mass  = new HTOP2("Mass_");
  h_Other = new HTOP3("Other_");

}


TtEventDummyAnalysis::~TtEventDummyAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   if (debug) cout << "[SeedQualityAnalysis] Destructor called" << endl;
   theFile->cd();
   theFile->cd("Jets");
   h_Jet->Write();

   theFile->cd();
   theFile->cd("Mass");
   h_Mass->Write();

   theFile->cd();
   theFile->cd("Other");
   h_Other->Write();

   //Release the memory
   delete h_Jet;
   delete h_Mass;
   delete h_Other;
 
   //Close the Root file
   theFile->Close();
   if (debug) cout << "************* Finished writing histograms to file" << endl;

}

void
TtEventDummyAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

/*
   // retrieve the reco-objects
   edm::Handle< std::vector<TopMuon> > muons;
   iEvent.getByLabel(muonSrc, muons);

   edm::Handle< std::vector<TopElectron> > electrons;
   iEvent.getByLabel(electronSrc, electrons);

   edm::Handle< std::vector<TopMET> > met;
   iEvent.getByLabel(metSrc, met);
   
   edm::Handle< std::vector<TopJet> > bjets;
   iEvent.getByLabel(bjetSrc, bjets);
*/
   // get the event solution
   edm::Handle< std::vector<TtSemiEvtSolution> > eSols; 
   iEvent.getByLabel(evtsols, eSols);

   std::vector<TtSemiEvtSolution> sols;
   sols = *eSols;
  
   // Initial the histograms
   HTOP1 *histo1 = 0;
   HTOP2 *histo2 = 0;
   HTOP3 *histo3 = 0;

   // Jets information
   double jET_g[4]={0.0};
   double jh_g[4]={0.0};
   double jET_r[4]={0.0};
   double jh_r[4]={0.0};
   double jET_f[4]={0.0};
   double jh_f[4]={0.0};
   double jET_c[4]={0.0};
   double jh_c[4]={0.0};
   // Mass information
   double mth[3]={0.0};
   double mtl[3]={0.0};
   double mWh[3]={0.0};
   double mWl[2]={0.0};

   if ( sols.size() > 0 ) 

   if ( ((sols[0].getGenEvent()).isSemiLeptonic()     ) && 
        ((sols[0].getGenEvent()).numberOfBQuarks()==2 ) ){ 

      int bestSol = sols[0].getMCBestJetComb();
      histo1 = h_Jet;
      histo3 = h_Other;

      jET_g[0] = ((sols[bestSol]).getGenHadp())->et();
      jET_g[1] = ((sols[bestSol]).getGenHadq())->et();
      jET_g[2] = ((sols[bestSol]).getGenHadb())->et();
      jET_g[3] = ((sols[bestSol]).getGenLepb())->et();
      jh_g[0]  = ((sols[bestSol]).getGenHadp())->eta();
      jh_g[1]  = ((sols[bestSol]).getGenHadq())->eta();
      jh_g[2]  = ((sols[bestSol]).getGenHadb())->eta();
      jh_g[3]  = ((sols[bestSol]).getGenLepb())->eta();

      jET_r[0] = ((sols[bestSol]).getRecHadp()).et();
      jET_r[1] = ((sols[bestSol]).getRecHadq()).et();
      jET_r[2] = ((sols[bestSol]).getRecHadb()).et();
      jET_r[3] = ((sols[bestSol]).getRecLepb()).et();
      jh_r[0]  = ((sols[bestSol]).getRecHadp()).eta();
      jh_r[1]  = ((sols[bestSol]).getRecHadq()).eta();
      jh_r[2]  = ((sols[bestSol]).getRecHadb()).eta();
      jh_r[3]  = ((sols[bestSol]).getRecLepb()).eta();

      jET_c[0] = ((sols[bestSol]).getCalHadp()).et();
      jET_c[1] = ((sols[bestSol]).getCalHadq()).et();
      jET_c[2] = ((sols[bestSol]).getCalHadb()).et();
      jET_c[3] = ((sols[bestSol]).getCalLepb()).et();
      jh_c[0]  = ((sols[bestSol]).getCalHadp()).eta();
      jh_c[1]  = ((sols[bestSol]).getCalHadq()).eta();
      jh_c[2]  = ((sols[bestSol]).getCalHadb()).eta();
      jh_c[3]  = ((sols[bestSol]).getCalLepb()).eta();

      for (int i=0; i<4; i++){
          histo1->Fill1a(jET_g[i],jh_g[i]);
          histo1->Fill1b(jET_r[i],jh_r[i]);
          histo1->Fill1c(jET_c[i],jh_c[i]);
      }

      if ( sols[bestSol].getProbChi2()>0 ) {
         jET_f[0] = ((sols[bestSol]).getFitHadp()).et();
         jET_f[1] = ((sols[bestSol]).getFitHadq()).et();
         jET_f[2] = ((sols[bestSol]).getFitHadb()).et();
         jET_f[3] = ((sols[bestSol]).getFitLepb()).et();
         jh_f[0]  = ((sols[bestSol]).getFitHadp()).eta();
         jh_f[1]  = ((sols[bestSol]).getFitHadq()).eta();
         jh_f[2]  = ((sols[bestSol]).getFitHadb()).eta();
         jh_f[3]  = ((sols[bestSol]).getFitLepb()).eta();
	 for (int i=0; i<4; i++){
             histo1->Fill1d(jET_f[i],jh_f[i]);
         }
         histo3->Fill3a( (sols[bestSol]).getProbChi2() );
      }
      histo3->Fill3b( (sols[bestSol]).getMCBestSumAngles() );

      mth[0] = ((sols[bestSol]).getRecHadt()).mass();
      mth[1] = ((sols[bestSol]).getCalHadt()).mass();
      mth[2] = ((sols[bestSol]).getFitHadt()).mass();

      mtl[0] = ((sols[bestSol]).getRecLept()).mass();
      mtl[1] = ((sols[bestSol]).getCalLept()).mass();
      mtl[2] = ((sols[bestSol]).getFitLept()).mass();

      mWh[0] = ((sols[bestSol]).getRecHadW()).mass();
      mWh[1] = ((sols[bestSol]).getCalHadW()).mass();
      mWh[2] = ((sols[bestSol]).getFitHadW()).mass();

      mWl[0] = ((sols[bestSol]).getRecLepW()).mass();
      mWl[1] = ((sols[bestSol]).getFitLepW()).mass();
      histo2 = h_Mass;
      histo2->Fill2(mth[0],mth[1],mth[2],mtl[0],mtl[1],mtl[2],mWh[0],mWh[1],mWh[2],mWl[0],mWl[1]);

      // Fill jet calibration check plots
      if( (sols[bestSol].getMCChangeWQ() == 0) || (sols[bestSol].getMCChangeWQ() == 1) ){
 
        histo1->Fill1e(jET_r[0],jET_r[0]/jET_g[0],jET_c[0]/jET_g[0]);
        histo1->Fill1e(jET_r[1],jET_r[1]/jET_g[1],jET_c[1]/jET_g[1]);

        if(sols[bestSol].getProbChi2()>0){
          histo1->Fill1f(jET_r[0],jET_f[0]/jET_g[0]);
          histo1->Fill1f(jET_r[1],jET_f[1]/jET_g[1]);
        }
      }

   }
   /*
   for (std::vector<TopMuon>::const_iterator mu_i = muons->begin(); mu_i != muons->end(); mu_i++)
   {
       cout <<"mu eta = "<<(*mu_i).eta()<<" mother = "<<(*mu_i).mother()<<endl;
   }

   for (std::vector<TopElectron>::const_iterator e_i = electrons->begin(); e_i != electrons->end(); e_i++)
   {
       cout <<"e eta = "<<(*e_i).eta()<<" mother = "<<(*e_i).mother()<<endl;
   }

   for (std::vector<TopMET>::const_iterator met_i = met->begin(); met_i != met->end(); met_i++)
   {
       cout <<"met eta = "<<(*met_i).eta()<<" mother = "<<(*met_i).mother()<<endl;
   }

   for (std::vector<TopJet>::const_iterator jet_i = bjets->begin(); jet_i != bjets->end(); jet_i++)
   {
       cout <<"bjet eta = "<<(*jet_i).eta()<<" mother = "<<(*jet_i).mother()<<endl;
   }

   cout <<" ============================ " <<endl;
   */
}


//define this as a plug-in
DEFINE_FWK_MODULE(TtEventDummyAnalysis);
