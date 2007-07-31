void TtEventDummyAnalysisPlotter(){

  /* Macro to plot histograms produced by TtEventDummyAnalysis.cc
   * You may need to update the TFile name, and will need to
   * input the segtype as shown below.
   *
   * Author:  Shih-Chuan Kao - UCR
   */


TFile *file0 = TFile::Open("TtEventDummyAnalysis.root");

// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...
TString suffixps = ".eps";


 TString plot01 = "Kinematics"+suffixps;
 TString plot02 = "Jet_Calibration"+suffixps;
 TString plot03 = "Reco_Objects"+suffixps;
 TString plot04 = "Others"+suffixps;

// ********************************************************************
// Pointers to histograms
// ********************************************************************
    hETjet_gen  = (TH1F *) file0->Get("Jets/hETjet_gen");
    hETjet_rec  = (TH1F *) file0->Get("Jets/hETjet_rec");
    hETjet_cal  = (TH1F *) file0->Get("Jets/hETjet_cal");
    hETjet_fit  = (TH1F *) file0->Get("Jets/hETjet_fit");
    hEtajet_gen  = (TH1F *) file0->Get("Jets/hEtajet_gen");
    hEtajet_rec  = (TH1F *) file0->Get("Jets/hEtajet_rec");
    hEtajet_cal  = (TH1F *) file0->Get("Jets/hEtajet_cal");
    hEtajet_fit  = (TH1F *) file0->Get("Jets/hEtajet_fit");
 
    pLJESrec = (TProfile *) file0->Get("Jets/pLJESrec");
    pLJEScal = (TProfile *) file0->Get("Jets/pLJEScal");
    pLJESfit = (TProfile *) file0->Get("Jets/pLJESfit");

    hmthrec      = (TH1F *) file0->Get("Mass/hmthrec");
    hmthcal      = (TH1F *) file0->Get("Mass/hmthcal");
    hmthfit      = (TH1F *) file0->Get("Mass/hmthfit");
    hmtlrec      = (TH1F *) file0->Get("Mass/hmtlrec");
    hmtlcal      = (TH1F *) file0->Get("Mass/hmtlcal");
    hmtlfit      = (TH1F *) file0->Get("Mass/hmtlfit");
    hmWhrec      = (TH1F *) file0->Get("Mass/hmWhrec");
    hmWhcal      = (TH1F *) file0->Get("Mass/hmWhcal");
    hmWhfit      = (TH1F *) file0->Get("Mass/hmWhfit");
    hmWlrec      = (TH1F *) file0->Get("Mass/hmWlrec");
    hmWlfit      = (TH1F *) file0->Get("Mass/hmWlfit");

    probchi2     = (TH1F *) file0->Get("Other/hprobchi2");
    hsumDR       = (TH1F *) file0->Get("Other/hsumDR");
   
    
// *****************************************************************
// Drawing Setups
// *****************************************************************

  /// scale the plots  
  hETjet_gen  -> Scale(1./(hETjet_gen->GetEntries()*1.));
  hEtajet_gen -> Scale(1./(hEtajet_gen->GetEntries()*1.));
  hETjet_rec  -> Scale(1./(hETjet_rec->GetEntries()*1.));
  hEtajet_rec -> Scale(1./(hEtajet_rec->GetEntries()*1.));
  hETjet_cal  -> Scale(1./(hETjet_cal->GetEntries()*1.));
  hEtajet_cal -> Scale(1./(hEtajet_cal->GetEntries()*1.));
  hETjet_fit  -> Scale(1./(hETjet_fit->GetEntries()*1.));
  hEtajet_fit -> Scale(1./(hEtajet_fit->GetEntries()*1.));


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
  c1.Print(plot01,"Landscape");

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
  c2.Print(plot02,"Landscape");

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
  c3.Print(plot03,"Landscape");

  // others
  TCanvas c4("c4","other",1);
  c4.Divide(1,2);
  c4.cd(1);
  probchi2->GetXaxis()->SetTitle("Prob Chi^2 for best solution if converging");
  probchi2->SetLineWidth(2); probchi2->Draw();
  c4.cd(2);
  hsumDR->GetXaxis()->SetTitle("Sum DR(gen,cal) of 4 leading jets");
  hsumDR->Draw();
  c4.Print(plot04,"Landscape");


 file0->Close();
 //gROOT->ProcessLine(".q");

}


