#include <iostream>
#include <cassert>
#include <TROOT.h>
#include <TSystem.h>
#include <Cintex/Cintex.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TProfile.h>
#include <TKey.h>
#include <TF1.h>
#include <TFormula.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TStyle.h>
#include <vector>
#include "FWCore/FWLite/src/AutoLibraryLoader.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"
#include "tdrstyle.C"

using namespace std;


int main() {

  //
  // load framework libraries
  //
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  
  //
  // Set P-TDR plots style
  //
  setTDRStyle();
  
  
  //
  // Read event solutions from events
  //
  TFile fIn("../data/TtSemiMuEvents.root");
  TTree * events = dynamic_cast<TTree*>( fIn.Get( "Events" ) );
  assert( events != 0 );
  TBranch * solsBranch;
  solsBranch = events->GetBranch( "TtSemiEvtSolutions_solutions__TtEventReco.obj" );
  assert( solsBranch != 0 );
  vector<TtSemiEvtSolution> sols;
  solsBranch -> SetAddress( & sols );
  
  
  
  
  //
  // Define some histograms 
  //
  
  // Kinematics
  TH1F *hETjet_gen   = new TH1F("hETjet_gen","",40,0,200);
  TH1F *hETjet_rec   = new TH1F("hETjet_rec","",40,0,200);
  TH1F *hETjet_cal   = new TH1F("hETjet_cal","",40,0,200);
  TH1F *hETjet_fit   = new TH1F("hETjet_fit","",40,0,200);
  TH1F *hEtajet_gen  = new TH1F("hEtajet_gen","",50,-5,5);
  TH1F *hEtajet_rec  = new TH1F("hEtajet_rec","",50,-5,5);
  TH1F *hEtajet_cal  = new TH1F("hEtajet_cal","",50,-5,5);
  TH1F *hEtajet_fit  = new TH1F("hEtajet_fit","",50,-5,5);
  
  // check jet calibration
  TProfile *pLJESrec = new TProfile("pLJESrec","",60,0,120,0,4);
  TProfile *pLJEScal = new TProfile("pLJEScal","",60,0,120,0,4);
  TProfile *pLJESfit = new TProfile("pLJESfit","",60,0,120,0,4);
  
  // reconstructed objects masses
  TH1F *hmthrec      = new TH1F("hmthrec","",50,0,350);
  TH1F *hmthcal      = new TH1F("hmthcal","",50,0,350);
  TH1F *hmthfit      = new TH1F("hmthfit","",50,0,350);
  TH1F *hmtlrec      = new TH1F("hmtlrec","",50,0,350);
  TH1F *hmtlcal      = new TH1F("hmtlcal","",50,0,350);
  TH1F *hmtlfit      = new TH1F("hmtlfit","",50,0,350);
  TH1F *hmWhrec      = new TH1F("hmWhrec","",50,0,150);
  TH1F *hmWhcal      = new TH1F("hmWhcal","",50,0,150);
  TH1F *hmWhfit      = new TH1F("hmWhfit","",50,0,150);
  TH1F *hmWlrec      = new TH1F("hmWlrec","",50,0,150);
  TH1F *hmWlfit      = new TH1F("hmWlfit","",50,0,150);
  
  // other
  TH1F *probchi2    = new TH1F("probchi2","",50,0,1);
  TH1F *hsumDR      = new TH1F("hsumDR","",50,0,5);

  
  // Loop over events and fill histograms
  int nev = events->GetEntries();
  for( int ev = 0; ev < nev; ++ ev ) { 
    solsBranch    -> GetEntry( ev );
            
    if(sols.size()>1){
        
      //get bestSol
      int bestSol = sols[0].getMCBestSol();
    
      // Fill kinematics plots
      hETjet_gen  -> Fill(sols[bestSol].getGenHadp().et());
      hETjet_gen  -> Fill(sols[bestSol].getGenHadq().et());
      hETjet_gen  -> Fill(sols[bestSol].getGenHadb().et());
      hETjet_gen  -> Fill(sols[bestSol].getGenLepb().et());
      hEtajet_gen -> Fill(sols[bestSol].getGenHadp().eta());
      hEtajet_gen -> Fill(sols[bestSol].getGenHadq().eta());
      hEtajet_gen -> Fill(sols[bestSol].getGenHadb().eta());
      hEtajet_gen -> Fill(sols[bestSol].getGenLepb().eta());
      hETjet_rec  -> Fill(sols[bestSol].getRecHadp().et());
      hETjet_rec  -> Fill(sols[bestSol].getRecHadq().et());
      hETjet_rec  -> Fill(sols[bestSol].getRecHadb().et());
      hETjet_rec  -> Fill(sols[bestSol].getRecLepb().et());
      hEtajet_rec -> Fill(sols[bestSol].getRecHadp().eta());
      hEtajet_rec -> Fill(sols[bestSol].getRecHadq().eta());
      hEtajet_rec -> Fill(sols[bestSol].getRecHadb().eta());
      hEtajet_rec -> Fill(sols[bestSol].getRecLepb().eta());
      hETjet_cal  -> Fill(sols[bestSol].getCalHadp().et());
      hETjet_cal  -> Fill(sols[bestSol].getCalHadq().et());
      hETjet_cal  -> Fill(sols[bestSol].getCalHadb().et());
      hETjet_cal  -> Fill(sols[bestSol].getCalLepb().et());
      hEtajet_cal -> Fill(sols[bestSol].getCalHadp().eta());
      hEtajet_cal -> Fill(sols[bestSol].getCalHadq().eta());
      hEtajet_cal -> Fill(sols[bestSol].getCalHadb().eta());
      hEtajet_cal -> Fill(sols[bestSol].getCalLepb().eta());
      if(sols[bestSol].getProbChi2()>0){
        hETjet_fit  -> Fill(sols[bestSol].getFitHadp().et());
        hETjet_fit  -> Fill(sols[bestSol].getFitHadq().et());
        hETjet_fit  -> Fill(sols[bestSol].getFitHadb().et());
        hETjet_fit  -> Fill(sols[bestSol].getFitLepb().et());
        hEtajet_fit -> Fill(sols[bestSol].getFitHadp().eta());
        hEtajet_fit -> Fill(sols[bestSol].getFitHadq().eta());
        hEtajet_fit -> Fill(sols[bestSol].getFitHadb().eta());
        hEtajet_fit -> Fill(sols[bestSol].getFitLepb().eta());
      }
           
      
      // Fill jet calibration check plots
      if(sols[bestSol].getChangeWQ() == 0){
        pLJESrec -> Fill(sols[bestSol].getRecHadp().et(),
                         sols[bestSol].getRecHadp().et()/sols[bestSol].getGenHadp().et());
        pLJESrec -> Fill(sols[bestSol].getRecHadq().et(),
                         sols[bestSol].getRecHadq().et()/sols[bestSol].getGenHadq().et());
        pLJEScal -> Fill(sols[bestSol].getRecHadp().et(),
                         sols[bestSol].getCalHadp().et()/sols[bestSol].getGenHadp().et());
        pLJEScal -> Fill(sols[bestSol].getRecHadq().et(),
                         sols[bestSol].getCalHadq().et()/sols[bestSol].getGenHadq().et());      
        if(sols[bestSol].getProbChi2()>0){
          pLJESfit -> Fill(sols[bestSol].getRecHadp().et(),
                           sols[bestSol].getFitHadp().et()/sols[bestSol].getGenHadp().et());
          pLJESfit -> Fill(sols[bestSol].getRecHadq().et(),
                           sols[bestSol].getFitHadq().et()/sols[bestSol].getGenHadq().et());
        }
      }
      else if(sols[bestSol].getChangeWQ() == 1){
        pLJESrec -> Fill(sols[bestSol].getRecHadp().et(),
                         sols[bestSol].getRecHadp().et()/sols[bestSol].getGenHadq().et());
        pLJESrec -> Fill(sols[bestSol].getRecHadq().et(),
                         sols[bestSol].getRecHadq().et()/sols[bestSol].getGenHadp().et());
        pLJEScal -> Fill(sols[bestSol].getRecHadp().et(),
                         sols[bestSol].getCalHadp().et()/sols[bestSol].getGenHadq().et());
        pLJEScal -> Fill(sols[bestSol].getRecHadq().et(),
                         sols[bestSol].getCalHadq().et()/sols[bestSol].getGenHadp().et());      
        if(sols[bestSol].getProbChi2()>0){
          pLJESfit -> Fill(sols[bestSol].getRecHadp().et(),
                           sols[bestSol].getFitHadp().et()/sols[bestSol].getGenHadq().et());
          pLJESfit -> Fill(sols[bestSol].getRecHadq().et(),
                           sols[bestSol].getFitHadq().et()/sols[bestSol].getGenHadp().et());
        }
      }
      
      
      
      // Fill reconstructed objects masses
      if(sols[bestSol].getProbChi2()>0){
        hmthrec ->Fill(sols[bestSol].getRecHadt().mass());
        hmtlrec ->Fill(sols[bestSol].getRecLept().mass());
        hmWhrec ->Fill(sols[bestSol].getRecHadW().mass());
        hmWlrec ->Fill(sols[bestSol].getRecLepW().mass());
        hmthcal ->Fill(sols[bestSol].getCalHadt().mass());
        hmtlcal ->Fill(sols[bestSol].getCalLept().mass());
        hmWhcal ->Fill(sols[bestSol].getCalHadW().mass());
        hmthfit ->Fill(sols[bestSol].getFitHadt().mass());
        hmtlfit ->Fill(sols[bestSol].getFitLept().mass());
        hmWhfit ->Fill(sols[bestSol].getFitHadW().mass());
        hmWlfit ->Fill(sols[bestSol].getFitLepW().mass());
      }
      
      
      // Fill other histo's
      hsumDR -> Fill(sols[bestSol].getSumDeltaRjp());
      if(sols[bestSol].getProbChi2()>0) probchi2 -> Fill(sols[bestSol].getProbChi2());
    }
  }
  
  //scale the kinematics plots
  hETjet_gen  -> Scale(1./(hETjet_gen->GetEntries()*1.));
  hEtajet_gen -> Scale(1./(hEtajet_gen->GetEntries()*1.));
  hETjet_rec  -> Scale(1./(hETjet_rec->GetEntries()*1.));
  hEtajet_rec -> Scale(1./(hEtajet_rec->GetEntries()*1.));
  hETjet_cal  -> Scale(1./(hETjet_cal->GetEntries()*1.));
  hEtajet_cal -> Scale(1./(hEtajet_cal->GetEntries()*1.));
  hETjet_fit  -> Scale(1./(hETjet_fit->GetEntries()*1.));
  hEtajet_fit -> Scale(1./(hEtajet_fit->GetEntries()*1.));
 




  //
  //  Draw the histograms and save in a ps file
  //
  
  TCanvas d("dummy","",1);
  d.Print("TtEventDummyAnalyser.ps[","Landscape");

  // Draw Kinematics
  TCanvas c1("c1","Kinematics",1);
  c1.Divide(1,2);
  c1.cd(1);
  hETjet_rec->GetXaxis()->SetTitle("jet E_{T} (GeV)");
  hETjet_rec->GetYaxis()->SetTitle("normalised nr. of events");
  hETjet_rec->SetLineColor(2); hETjet_rec->Draw();
  hETjet_gen->SetLineColor(1); hETjet_gen->Draw("same");
  hETjet_cal->SetLineColor(4); hETjet_cal->Draw("same");
  hETjet_fit->SetLineColor(8); hETjet_fit->Draw("same");
  TLegend lkin1(0.65,0.7,0.90,0.90);
  lkin1.AddEntry(hETjet_gen,"parton","l");
  lkin1.AddEntry(hETjet_rec,"recjet","l");
  lkin1.AddEntry(hETjet_cal,"caljet","l");
  lkin1.AddEntry(hETjet_fit,"fitjet","l");
  lkin1.Draw();
  c1.cd(2);
  hEtajet_gen->GetXaxis()->SetTitle("jet #eta");
  hEtajet_gen->GetYaxis()->SetTitle("normalised nr. of events");
  hEtajet_gen->SetLineColor(1); hEtajet_gen->Draw();
  hEtajet_rec->SetLineColor(2); hEtajet_rec->Draw("same");
  hEtajet_cal->SetLineColor(4); hEtajet_cal->Draw("same");
  hEtajet_fit->SetLineColor(8); hEtajet_fit->Draw("same");
  TLegend lkin2(0.65,0.7,0.90,0.90);
  lkin2.AddEntry(hEtajet_gen,"parton","l");
  lkin2.AddEntry(hEtajet_rec,"recjet","l");
  lkin2.AddEntry(hEtajet_cal,"caljet","l");
  lkin2.AddEntry(hEtajet_fit,"fitjet","l");
  lkin2.Draw();
  c1.Print("TtEventDummyAnalyser.ps","Landscape");


  // Draw calibration check plots
  TCanvas c2("c2","Jet Calibration",1);
  pLJESrec->GetXaxis()->SetTitle("rec jet E_{T} (GeV)");
  pLJESrec->GetYaxis()->SetTitle("Relatve shift");
  pLJESrec->SetMaximum(2);
  pLJESrec->SetLineColor(2); pLJESrec->SetMarkerColor(2); pLJESrec->Draw();
  pLJEScal->SetLineColor(4); pLJEScal->SetMarkerColor(4); pLJEScal->Draw("same");
  pLJESfit->SetLineColor(8); pLJESfit->SetMarkerColor(8); pLJESfit->Draw("same");
  TLegend ljes(0.65,0.7,0.90,0.90);
  ljes.AddEntry(pLJESrec,"rec. jet","l");
  ljes.AddEntry(pLJEScal,"cal. jet","l");
  ljes.AddEntry(pLJESfit,"fit. jet","l");
  ljes.Draw();
  c2.Print("TtEventDummyAnalyser.ps","Landscape");


  // Draw reconstructed objects masses
  TCanvas c3("c3","Reconstructed Objects",1);
  c3.Divide(2,2);
  c3.cd(1);
  hmWhrec->GetXaxis()->SetTitle("hadronic W-mass (GeV/c^2)");
  hmWhrec->SetLineColor(2); hmWhrec->Draw();
  hmWhcal->SetLineColor(4); hmWhcal->Draw("same");
  hmWhfit->SetLineColor(8); hmWhfit->Draw("same");
  TLegend lro1(0.65,0.7,0.90,0.90);
  lro1.AddEntry(hmWhrec,"rec.","l");
  lro1.AddEntry(hmWhcal,"cal.","l");
  lro1.AddEntry(hmWhfit,"fit.","l");
  lro1.Draw();
  c3.cd(2);
  hmWlrec->GetXaxis()->SetTitle("leptonic W-mass (GeV/c^2)");
  hmWlrec->SetLineColor(2); hmWlrec->Draw();
  hmWlfit->SetLineColor(8); hmWlfit->Draw("same");
  TLegend lro2(0.65,0.7,0.90,0.90);
  lro2.AddEntry(hmWlrec,"rec.","l");
  lro2.AddEntry(hmWlfit,"fit.","l");
  lro2.Draw();
  c3.cd(3);
  hmthrec->GetXaxis()->SetTitle("hadronic top-mass (GeV/c^2)");
  hmthrec->SetLineColor(2); hmthrec->Draw();
  hmthcal->SetLineColor(4); hmthcal->Draw("same");
  hmthfit->SetLineColor(8); hmthfit->Draw("same");
  TLegend lro3(0.65,0.7,0.90,0.90);
  lro3.AddEntry(hmthrec,"rec.","l");
  lro3.AddEntry(hmthcal,"cal.","l");
  lro3.AddEntry(hmthfit,"fit.","l");
  lro3.Draw();
  c3.cd(4);
  hmtlrec->GetXaxis()->SetTitle("leptonic top-mass (GeV/c^2)");
  hmtlrec->SetLineColor(2); hmtlrec->Draw();
  hmtlcal->SetLineColor(4); hmtlcal->Draw("same");
  hmtlfit->SetLineColor(8); hmtlfit->Draw("same");
  TLegend lro4(0.65,0.7,0.90,0.90);
  lro4.AddEntry(hmtlrec,"rec.","l");
  lro4.AddEntry(hmtlcal,"cal.","l");
  lro4.AddEntry(hmtlfit,"fit.","l");
  lro4.Draw();
  c3.Print("TtEventDummyAnalyser.ps","Landscape");
 

  TCanvas c4("c4","other",1);
  c4.Divide(1,2);
  c4.cd(1);
  probchi2->GetXaxis()->SetTitle("Prob Chi^2 for best solution if converging");
  probchi2->SetLineWidth(2); probchi2->Draw();
  c4.cd(2);
  hsumDR->GetXaxis()->SetTitle("Sum DR(gen,cal) of 4 leading jets");
  hsumDR->Draw();
  c4.Print("TtEventDummyAnalyser.ps","Landscape");
  
  
  //close ps file
  d.Print("TtEventDummyAnalyser.ps]","Landscape");
  
  
  return 0;
}
