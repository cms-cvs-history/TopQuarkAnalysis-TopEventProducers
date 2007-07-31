#ifndef TopAnalysisHisto_H
#define TopAnalysisHisto_H

/** \class TopAnalysisHistograms
 *  Collection of histograms for TopAnalysis test.
 *
 * Author: S.C. Kao  - UC Riverside
 */

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TFile.h"
#include "TString.h"
#include <string>
#include <iostream>

using namespace std;

class HTOP1 {
public:
 
 HTOP1(std::string name_) {
    TString N1 = name_.c_str();
    name=N1;

    //hcsc_q  = new TH2F(N1+"_hcsc_q", " CSC Q from vec - Q from dphi ", 29, -7.25, 7.25, 29, -7.25, 7.25);
    // Kinematics
    hETjet_gen   = new TH1F("hETjet_gen","",40,0,200);
    hETjet_rec   = new TH1F("hETjet_rec","",40,0,200);
    hETjet_cal   = new TH1F("hETjet_cal","",40,0,200);
    hETjet_fit   = new TH1F("hETjet_fit","",40,0,200);
    hEtajet_gen  = new TH1F("hEtajet_gen","",50,-5,5);
    hEtajet_rec  = new TH1F("hEtajet_rec","",50,-5,5);
    hEtajet_cal  = new TH1F("hEtajet_cal","",50,-5,5);
    hEtajet_fit  = new TH1F("hEtajet_fit","",50,-5,5);

    pLJESrec = new TProfile("pLJESrec","",60,0,120,0,4);
    pLJEScal = new TProfile("pLJEScal","",60,0,120,0,4);
    pLJESfit = new TProfile("pLJESfit","",60,0,120,0,4);


 } 

 HTOP1(TString name_, TFile* file) {
    name=name_;
    //hcsc_q  = (TH2F *) file->Get(name+"_hcsc_q");
    hETjet_gen   = (TH1F *) file->Get("hETjet_gen");
    hETjet_rec   = (TH1F *) file->Get("hETjet_rec");
    hETjet_cal   = (TH1F *) file->Get("hETjet_cal");
    hETjet_fit   = (TH1F *) file->Get("hETjet_fit");
    hEtajet_gen  = (TH1F *) file->Get("hEtajet_gen");
    hEtajet_rec  = (TH1F *) file->Get("hEtajet_rec");
    hEtajet_cal  = (TH1F *) file->Get("hEtajet_cal");
    hEtajet_fit  = (TH1F *) file->Get("hEtajet_fit");
 
    pLJESrec  = (TProfile *) file->Get("pLJESrec");
    pLJEScal  = (TProfile *) file->Get("pLJEScal");
    pLJESfit  = (TProfile *) file->Get("pLJESfit");
    
 }

 /// Destructor
 virtual ~HTOP1() {
    //delete hcsc_q;
    delete hETjet_gen;
    delete hETjet_rec;
    delete hETjet_cal;
    delete hETjet_fit;
    delete hEtajet_gen;
    delete hEtajet_rec;
    delete hEtajet_cal;
    delete hEtajet_fit;

    delete pLJESrec;
    delete pLJEScal;
    delete pLJESfit;

 }

 void Fill1a(double j_gen, double h_gen ){
    hETjet_gen->Fill(j_gen);
    hEtajet_gen->Fill(h_gen);
 }
 void Fill1b(double j_rec, double h_rec ){
    hETjet_rec->Fill(j_rec);
    hEtajet_rec->Fill(h_rec);
 }
 void Fill1c(double j_cal, double h_cal){
    hETjet_cal->Fill(j_cal);
    hEtajet_cal->Fill(h_cal);
 }
 void Fill1d(double j_fit, double h_fit){
    hETjet_fit->Fill(j_fit);
    hEtajet_fit->Fill(h_fit);
 }

 void Fill1e(double et_rec, double pJrec, double pJcal){
    pLJESrec->Fill(et_rec, pJrec);
    pLJEScal->Fill(et_rec, pJcal);
 }
 void Fill1f(double et_rec, double pJfit){
    pLJESfit->Fill(et_rec, pJfit);
 }

 void Write() {
     //hcsc_q->Write();
    hETjet_gen->Write();
    hETjet_rec->Write();
    hETjet_cal->Write();
    hETjet_fit->Write();
    hEtajet_gen->Write();
    hEtajet_rec->Write();
    hEtajet_cal->Write();
    hEtajet_fit->Write();

    pLJESrec->Write();
    pLJEScal->Write();
    pLJESfit->Write();
 }

 //TH2F *hcsc_q;
  TH1F *hETjet_gen;
  TH1F *hETjet_rec;
  TH1F *hETjet_cal;
  TH1F *hETjet_fit;
  TH1F *hEtajet_gen;
  TH1F *hEtajet_rec;
  TH1F *hEtajet_cal;
  TH1F *hEtajet_fit;
 
  TProfile *pLJESrec;
  TProfile *pLJEScal;
  TProfile *pLJESfit;

 TString name;

};

class HTOP2 {
public:
 
 HTOP2(std::string name_) {
    TString N1 = name_.c_str();
    name=N1;

   // reconstructed objects masses
    hmthrec      = new TH1F("hmthrec","",50,0,350);
    hmthcal      = new TH1F("hmthcal","",50,0,350);
    hmthfit      = new TH1F("hmthfit","",50,0,350);
    hmtlrec      = new TH1F("hmtlrec","",50,0,350);
    hmtlcal      = new TH1F("hmtlcal","",50,0,350);
    hmtlfit      = new TH1F("hmtlfit","",50,0,350);
    hmWhrec      = new TH1F("hmWhrec","",50,0,150);
    hmWhcal      = new TH1F("hmWhcal","",50,0,150);
    hmWhfit      = new TH1F("hmWhfit","",50,0,150);
    hmWlrec      = new TH1F("hmWlrec","",50,0,150);
    hmWlfit      = new TH1F("hmWlfit","",50,0,150);


 } 

 HTOP2(TString name_, TFile* file) {
    name=name_;

    hmthrec      = (TH1F *) file->Get("hmthrec");
    hmthcal      = (TH1F *) file->Get("hmthcal");
    hmthfit      = (TH1F *) file->Get("hmthfit");
    hmtlrec      = (TH1F *) file->Get("hmtlrec");
    hmtlcal      = (TH1F *) file->Get("hmtlcal");
    hmtlfit      = (TH1F *) file->Get("hmtlfit");
    hmWhrec      = (TH1F *) file->Get("hmWhrec");
    hmWhcal      = (TH1F *) file->Get("hmWhcal");
    hmWhfit      = (TH1F *) file->Get("hmWhfit");
    hmWlrec      = (TH1F *) file->Get("hmWlrec");
    hmWlfit      = (TH1F *) file->Get("hmWlfit");

    
 }

 /// Destructor
 virtual ~HTOP2() {

    delete hmthrec;
    delete hmthcal;
    delete hmthfit;
    delete hmtlrec;
    delete hmtlcal;
    delete hmtlfit;
    delete hmWhrec;
    delete hmWhcal;
    delete hmWhfit;
    delete hmWlrec;
    delete hmWlfit;
 }

 void Fill2(double mth_r, double mth_c, double mth_f, double mtl_r, double mtl_c, double mtl_f, 
            double mWh_r, double mWh_c, double mWh_f, double mWl_r, double mWl_f ){

    hmthrec->Fill(mth_r);
    hmthcal->Fill(mth_c);
    hmthfit->Fill(mth_f);
    hmtlrec->Fill(mtl_r);
    hmtlcal->Fill(mtl_c);
    hmtlfit->Fill(mtl_f);
    hmWhrec->Fill(mWh_r);
    hmWhcal->Fill(mWh_c);
    hmWhfit->Fill(mWh_f);
    hmWlrec->Fill(mWl_r);
    hmWlfit->Fill(mWl_f);
 }


 void Write() {

    hmthrec->Write();
    hmthcal->Write();
    hmthfit->Write();
    hmtlrec->Write();
    hmtlcal->Write();
    hmtlfit->Write();
    hmWhrec->Write();
    hmWhcal->Write();
    hmWhfit->Write();
    hmWlrec->Write();
    hmWlfit->Write();

 }

  TH1F *hmthrec;
  TH1F *hmthcal;
  TH1F *hmthfit;
  TH1F *hmtlrec;
  TH1F *hmtlcal;
  TH1F *hmtlfit;
  TH1F *hmWhrec;
  TH1F *hmWhcal;
  TH1F *hmWhfit;
  TH1F *hmWlrec;
  TH1F *hmWlfit;

 TString name;

};

class HTOP3 {
public:
 
 HTOP3(std::string name_) {
    TString N1 = name_.c_str();
    name=N1;

   // reconstructed objects masses
    hprobchi2    = new TH1F("hprobchi2","",50,0,1);
    hsumDR       = new TH1F("hsumDR","",50,0,5);

 } 

 HTOP3(TString name_, TFile* file) {
    name=name_;

    hprobchi2    = (TH1F *) file->Get("hprobchi2");
    hsumDR       = (TH1F *) file->Get("hsumDR");
    
 }

 /// Destructor
 virtual ~HTOP3() {

    delete hprobchi2;
    delete hsumDR;
 }

 void Fill3a(double probchi2 ){
    hprobchi2->Fill(probchi2);
 }
 void Fill3b(double sumDR ){
    hsumDR->Fill(sumDR);
 }



 void Write() {

    hprobchi2->Write();
    hsumDR->Write();

 }

  TH1F *hprobchi2;
  TH1F *hsumDR;

 TString name;

};


#endif
