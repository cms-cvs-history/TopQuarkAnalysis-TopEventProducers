#include "TString.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "TopQuarkAnalysis/TopEventProducers/interface/TopDecaySubset.h"


using namespace std;
using namespace reco;

// maximal number of particles in the original gen
// particle listing to be printed as daughters for 
// debugging
static const unsigned int kMAX=5; 


TopDecaySubset::TopDecaySubset(const edm::ParameterSet& cfg):
  pdg_( cfg.getParameter<unsigned int >( "pdgId" ) ),
  src_( cfg.getParameter<edm::InputTag>( "src" ) )
{
  // produces a set of gep particle collections following
  // the decay branch of top quarks to the first level of 
  // stable decay products
  produces<reco::GenParticleCollection>();
  produces<reco::GenParticleCollection>("beforePS");
  produces<reco::GenParticleCollection>("afterPS" ); 
  produces<reco::GenParticleCollection>("ME"      );
}

TopDecaySubset::~TopDecaySubset()
{
}

void
TopDecaySubset::produce(edm::Event& evt, const edm::EventSetup& setup)
{     
  edm::Handle<reco::GenParticleCollection> src;
  evt.getByLabel(src_, src);
 
  const reco::GenParticleRefProd ref = evt.getRefBeforePut<reco::GenParticleCollection>(); 
  // create target vectors
  std::auto_ptr<reco::GenParticleCollection> selME      ( new reco::GenParticleCollection );
  std::auto_ptr<reco::GenParticleCollection> selStable  ( new reco::GenParticleCollection );
  std::auto_ptr<reco::GenParticleCollection> selBeforePS( new reco::GenParticleCollection );
  std::auto_ptr<reco::GenParticleCollection> selAfterPS ( new reco::GenParticleCollection );

  // fill output vectors with references
  fillOutput(*src, *selME,       ref, kME      );
  fillOutput(*src, *selStable,   ref, kStable  );
  fillOutput(*src, *selBeforePS, ref, kBeforePS);
  fillOutput(*src, *selAfterPS,  ref, kAfterPS );

  // write vectors to event
  evt.put( selME, "ME" );
  evt.put( selStable   );
  evt.put( selBeforePS, "beforePS" );
  evt.put( selAfterPS,  "afterPS"  );
}

Particle::LorentzVector 
TopDecaySubset::p4(const std::vector<reco::GenParticle>::const_iterator top, bool p4Flag, int statusFlag)
{
  // calculate the four vector for top/anti-top quarks from 
  // the W boson and the b quark plain or including all 
  // additional radiation depending on switch 'plain'
  if(statusFlag==TopDecayID::unfrag){
    // return 4 momentum as it is
    return top->p4();
  }
  Particle::LorentzVector vec;
  for(reco::GenParticle::const_iterator p=top->begin(); p!=top->end(); ++p){
    if(!p4Flag){
      if( p->status() == TopDecayID::unfrag ){
	// decend by one level for each
	// status 3 particle on the way
	vec+=p4( p, p4Flag, 2 );
      }
      else{ 
	if( abs(top->pdgId())==TopDecayID::WID ){
	  // in case of a W boson skip the status 
	  // 2 particle to prevent double counting
	  if( abs(p->pdgId())!=TopDecayID::WID ) 
	    vec+=p->p4();
        } 
        else{
	  // add all four vectors for each stable
	  // particle (status 1 or 2) on the way 
	  // else
	  vec+=p->p4();
	}
      }
    }
    else{
      if( p->status() == TopDecayID::unfrag && 
	  (abs(p->pdgId()) == TopDecayID::WID || 
	   abs(p->pdgId()) == TopDecayID::bID)){
        // take only the W boson & the quark; do 
	// not consider any additional radiation
	vec+=p4( p, p4Flag, statusFlag);
      }
    }
  }
  return vec;
}

Particle::LorentzVector 
TopDecaySubset::p4(const reco::GenParticle::const_iterator part,
		      bool p4Flag, int statusFlag)
{
  // calculate the four vector for all top daughters from 
  // their daughters including additional radiation 
  if(statusFlag==TopDecayID::unfrag){
    // return 4 momentum as it is
    return part->p4();
  }
  Particle::LorentzVector vec;
  for(reco::GenParticle::const_iterator p=part->begin(); p!=part->end(); ++p){
    if(!p4Flag){
      if( p->status()<=TopDecayID::stable && p->pdgId() == part->pdgId()){
	// return the four vector of status 1 or status 2 daughters, 
	// if they have the same pdgId as part and break
        vec=p->p4();
        break;
      }
      else{
	// sum uo the p4 of all stable particles 
	// (of status 1 or 2)
        if(p->status()<=TopDecayID::stable) vec+=p->p4();
        else 
	  if( p->status()==TopDecayID::unfrag)
	    // if the particle is unfragmented (i.e.
	    // status 3) decend by one level
	    vec+=p4(p, p4Flag, statusFlag);   
      }
    }
    else{
      // treat the case of the W boson separately
      if( p->status()<=TopDecayID::stable && 
	  abs(p->pdgId()) == TopDecayID::WID){
	vec=p->p4();
	break;
      }
      else{
        if(p->status()<=TopDecayID::stable) vec+=p->p4();
        else 
	  if( p->status()==TopDecayID::unfrag)
	    vec+=p4(p, p4Flag, statusFlag);   
      }
    }
  }
  return vec;
}

void
TopDecaySubset::clearReferences()
{
  // clear vector of references 
  refs_.clear();  
  // set idx for mother particles to a start value
  // of -1 (the first entry will raise it to 0)
  motherPartIdx_=-1;
}

bool 
TopDecaySubset::wInDecayChain(const reco::GenParticleCollection& src, const int& partId)
{
  bool isContained=false;
  for(GenParticleCollection::const_iterator t=src.begin(); t!=src.end(); ++t){
    if( t->status() == TopDecayID::unfrag && t->pdgId()==partId ){ 
      for(GenParticle::const_iterator td=t->begin(); td!=t->end(); ++td){
        if( td->status()==TopDecayID::unfrag && abs( td->pdgId() )==TopDecayID::WID ){ 
	  isContained=true;
	  break;
        }
      }
    }
  }
  if( !isContained ){
    edm::LogWarning( "decayChain" )
      << " W boson is not contained in decay chain in the original gen particle listing.   \n"
      << " A status 2 equivalent W candidate will be re-reconstructed from the W daughters \n"
      << " but the hadronization of the W might be screwed up. Contact an expert for the   \n"
      << " generator in use to assure that what you are doing is ok.";     
  }
  return isContained;
}

void
TopDecaySubset::fillOutput(const reco::GenParticleCollection& src, reco::GenParticleCollection& target, const reco::GenParticleRefProd& ref, FillMode mode)
{
  // clear existing refs
  clearReferences();
  
  // fill output for top branch
  wInDecayChain(src, TopDecayID::tID) ? 
    fillFromFullListing     (src, target, TopDecayID::tID, mode): 
    fillFromTruncatedListing(src, target, TopDecayID::tID, mode);
  // fill output for anti-top branch
  wInDecayChain(src,-TopDecayID::tID) ? 
    fillFromFullListing     (src, target,-TopDecayID::tID, mode): 
    fillFromTruncatedListing(src, target,-TopDecayID::tID, mode);

  // fill references
  fillReferences(ref, target);

  // print decay chain for debugging
  printSource(src,    pdg_);
  printTarget(target, pdg_);
}

void 
TopDecaySubset::fillFromFullListing(const reco::GenParticleCollection& src, reco::GenParticleCollection& target, const int& partId, FillMode mode)
{
  int statusFlag;
  // determine status flag of the new 
  // particle depending on the FillMode
  mode == kME ? statusFlag=3 : statusFlag=2;

  // determine p4 reconstruction flag
  // depending on the FillMode
  bool p4Flag = (mode == kBeforePS);

  for(GenParticleCollection::const_iterator t=src.begin(); t!=src.end(); ++t){
    if( t->status() == TopDecayID::unfrag && t->pdgId()==partId ){ 
      // if particle is top or anti-top 
      auto_ptr<reco::GenParticle> topPtr( new GenParticle( t->threeCharge(), p4( t, p4Flag, statusFlag), t->vertex(), t->pdgId(), statusFlag, false ) );
      target.push_back( *topPtr );
      ++motherPartIdx_;
      // keep the top index for the map to manage the daughter refs
      int iTop=motherPartIdx_; 
      vector<int> topDaughters;
      // define the W boson index (to be set later) for the map to 
      // manage the daughter refs
      int iW = 0;
      vector<int> wDaughters;

      //iterate over top daughters
      for(GenParticle::const_iterator td=t->begin(); td!=t->end(); ++td){
	if( td->status()==TopDecayID::unfrag && abs( td->pdgId() )<=TopDecayID::bID ){ 
	  // if particle is beauty or other quark 
	  if(mode == kAfterPS){
	    addRadiation(motherPartIdx_,td,target); 
	  } 
	  auto_ptr<GenParticle> bPtr( new GenParticle( td->threeCharge(), p4( td, p4Flag, statusFlag ), td->vertex(), td->pdgId(), statusFlag, false ) );
	  target.push_back( *bPtr );	  
	  // increment & push index of the top daughter
	  topDaughters.push_back( ++motherPartIdx_ ); 
	  if(mode == kBeforePS){
	    addRadiation(motherPartIdx_,td,target); 
	  }
	}
	if( td->status()==TopDecayID::unfrag && abs( td->pdgId() )==TopDecayID::WID ){ 
	  // if particle is is W boson
	  if(mode == kAfterPS){
	    addRadiation(motherPartIdx_,td,target); 
	  }

	  auto_ptr<GenParticle> wPtr(  new GenParticle( td->threeCharge(), p4( td, p4Flag, statusFlag), td->vertex(), td->pdgId(), statusFlag, true ) );
	  target.push_back( *wPtr );
	  // increment & push index of the top daughter
	  topDaughters.push_back( ++motherPartIdx_ );
	  // keep the W idx for the map
	  iW=motherPartIdx_; 
	  if(mode == kBeforePS){
	    addRadiation(motherPartIdx_,td,target); 
	  }

	  // iterate over W daughters
	  for(GenParticle::const_iterator wd=td->begin(); wd!=td->end(); ++wd){
	    // make sure the W daughter is of status unfrag and not the W itself
	    if( wd->status()==TopDecayID::unfrag && !(abs(wd->pdgId())==TopDecayID::WID) ) {
	      if(mode == kAfterPS){
		addRadiation(motherPartIdx_,wd,target); 
	      } 
	      auto_ptr<GenParticle> qPtr( new GenParticle( wd->threeCharge(), p4( wd, p4Flag, statusFlag ), wd->vertex(), wd->pdgId(), statusFlag, false) );
	      target.push_back( *qPtr );
	      // increment & push index of the top daughter
	      wDaughters.push_back( ++motherPartIdx_ );
	      if(mode == kBeforePS){
		if(abs(wd->pdgId())<TopDecayID::tID)
		  // restrict to radiation from quarks
		  addRadiation(motherPartIdx_,wd,target); 
              }
	      if( wd->status()==TopDecayID::unfrag && abs( wd->pdgId() )==TopDecayID::tauID ){ 
		// add tau daughters if the particle is a tau pass
		// the daughter of the tau which is of status 2
		addTauDaughters(motherPartIdx_,wd->begin(),target); 
	      }
	    } 
	  }
	}
	if(td->status()==TopDecayID::stable && ( td->pdgId()==TopDecayID::glueID || abs(td->pdgId())<TopDecayID::bID)){
	  // collect additional radiation from the top 
	  auto_ptr<GenParticle> radPtr( new GenParticle( td->threeCharge(), td->p4(), td->vertex(), td->pdgId(), statusFlag, false ) );
	  target.push_back( *radPtr );	
	  // increment & push index of the top daughter  
	  topDaughters.push_back( ++motherPartIdx_ );
	}
      }
      // fill the map for the administration 
      // of daughter indices
      refs_[ iTop ] = topDaughters;
      refs_[ iW   ] = wDaughters; 
    }
  }
}

void 
TopDecaySubset::fillFromTruncatedListing(const reco::GenParticleCollection& src, reco::GenParticleCollection& sel, const int& partId, FillMode mode)
{
  //needed for W reconstruction from daughters
  reco::Particle::Point wVtx;
  reco::Particle::Charge wQ=0;
  reco::Particle::LorentzVector wP4S2,wP4S3,wP4S4;
  for(GenParticleCollection::const_iterator t=src.begin(); t!=src.end(); ++t){
    if( t->status() == TopDecayID::unfrag && t->pdgId()==partId ){ 
      //if source particle is top or topBar     
      //status 2
      GenParticle* candS2 = new GenParticle( t->threeCharge(), p4( t, false, 2), t->vertex(), t->pdgId(), t->status(), false );
      auto_ptr<reco::GenParticle> ptrS2( candS2 );
      sel.push_back( *ptrS2 );
      ++motherPartIdx_;
      //keep top index for the map for 
      //management of the daughter refs
      int iTopS2=motherPartIdx_, iWS2=0;
      //status 3
      GenParticle* candS3 = new GenParticle( t->threeCharge(), t->p4(), t->vertex(), t->pdgId(), t->status(), false );
      auto_ptr<reco::GenParticle> ptrS3( candS3 );
      sel.push_back( *ptrS3 );
      ++motherPartIdx_;
      int iTopS3=motherPartIdx_, iWS3=0;
      //status 4
      GenParticle* candS4 = new GenParticle( t->threeCharge(), p4( t, true, 2 ), t->vertex(), t->pdgId(), t->status(), false );
      auto_ptr<reco::GenParticle> ptrS4( candS4 );
      sel.push_back( *ptrS4 );
      ++motherPartIdx_;
      //keep top index for the map for 
      //management of the daughter refs
      int iTopS4=motherPartIdx_, iWS4=0;
      
      vector<int> topDaughsS2, wDaughsS2;
      vector<int> topDaughsS3, wDaughsS3;
      vector<int> topDaughsS4, wDaughsS4;
      
      //search ISR
      if(t->numberOfMothers()>0 && t->pdgId()==TopDecayID::tID) // to do it only once !
        for(GenParticle::const_iterator ts = t->mother()->begin(); ts!=t->mother()->end(); ++ts){
         if(abs(ts->pdgId())!=TopDecayID::tID){
           //ISR 
           GenParticle* cand = new GenParticle( ts->threeCharge(), ts->p4(), ts->vertex(), ts->pdgId(), ts->status(), false );
           auto_ptr<reco::GenParticle> ptr( cand );
           sel.push_back( *ptr );
           ++motherPartIdx_;
         }
       }

      //iterate over top daughters
      for(GenParticle::const_iterator td=t->begin(); td!=t->end(); ++td){
	if( td->status()==TopDecayID::unfrag && abs( td->pdgId() )==TopDecayID::bID ){ 
	  //is beauty
	  //here there is an unmbiguity for event t->'W'q where q!=b (when R!=1) but cannot be treat properly
	  //status 2
	  GenParticle* candS2 = new GenParticle( td->threeCharge(), p4( td, false, 3 ), td->vertex(), td->pdgId(), td->status(), false );
	  auto_ptr<GenParticle> ptrS2( candS2 );
	  sel.push_back( *ptrS2 );	  
	  topDaughsS2.push_back( ++motherPartIdx_ ); //push index of top daughter
	  //status 3
	  GenParticle* candS3 = new GenParticle( td->threeCharge(), td->p4(), td->vertex(), td->pdgId(), td->status(), false );
	  auto_ptr<GenParticle> ptrS3( candS3 );
	  sel.push_back( *ptrS3 );	  
	  topDaughsS3.push_back( ++motherPartIdx_ ); //push index of top daughter
	  //status 4
	  GenParticle* candS4 = new GenParticle( td->threeCharge(), p4( td, true, 3 ),
					       td->vertex(), td->pdgId(), td->status(), false );
	  auto_ptr<GenParticle> ptrS4( candS4 );
	  sel.push_back( *ptrS4 );	  
	  topDaughsS4.push_back( ++motherPartIdx_ ); //push index of top daughter
          // fill radiations from b status 3
	  addRadiation(motherPartIdx_,td,sel); 
	}
	//count all 4-vectors but the b
	if( td->status()==TopDecayID::unfrag && abs( td->pdgId() )!=TopDecayID::bID ){
	  //is non-beauty
	  //status 2
	  GenParticle* candS2 = new GenParticle( td->threeCharge(), p4( td, false, 3 ), td->vertex(), td->pdgId(), td->status(), false);
	  auto_ptr<GenParticle> ptrS2( candS2 );
	  sel.push_back( *ptrS2 );
	  wDaughsS2.push_back( ++motherPartIdx_ ); //push index of wBoson daughter
	  //status 3
	  GenParticle* candS3 = new GenParticle( td->threeCharge(), td->p4(), td->vertex(), td->pdgId(), td->status(), false);
	  auto_ptr<GenParticle> ptrS3( candS3 );
	  sel.push_back( *ptrS3 );
	  wDaughsS3.push_back( ++motherPartIdx_ ); //push index of wBoson daughter
	  //status 4
	  GenParticle* candS4 = new GenParticle( td->threeCharge(), p4( td, true, 3 ), td->vertex(), td->pdgId(), td->status(), false);
	  auto_ptr<GenParticle> ptrS4( candS4 );
	  sel.push_back( *ptrS4 );
	  wDaughsS4.push_back( ++motherPartIdx_ ); //push index of wBoson daughter
	  
          // fill radiations from quark status 3
	  if(abs(td->pdgId())<TopDecayID::tID)
	    addRadiation(motherPartIdx_,td,sel); 
	  
	  //get w quantities from its daughters; take care of the non-trivial 
	  //charge definition of quarks/leptons and of the non integer charge
	  //of the quarks
	  if( fabs(td->pdgId() )<TopDecayID::tID ) //quark
	    wQ += ((td->pdgId()>0)-(td->pdgId()<0))*abs(candS2->threeCharge());
	  else //lepton
	    wQ +=-((td->pdgId()>0)-(td->pdgId()<0))*abs(candS2->charge());
	  wVtx=candS2->vertex();	      
	  wP4S2+=candS2->p4();
	  wP4S3+=candS3->p4();
	  wP4S4+=candS4->p4();
	  
	  if( td->status()==TopDecayID::unfrag && abs( td->pdgId() )==TopDecayID::tauID ){ 
	    //is tau
	    addTauDaughters(motherPartIdx_,td->begin(),sel); //pass daughter of tau which is of status
	    //2 and by this skip status 3 particle
	  }
	}
	if( (td+1)==t->end() ){
	  //reco of W boson is completed
	  //status 2
	  GenParticle* candS2 = new GenParticle( wQ, wP4S2, wVtx, wQ*TopDecayID::WID, TopDecayID::unfrag, false);
	  auto_ptr<GenParticle> ptrS2( candS2 );
	  sel.push_back( *ptrS2 ); 
	  topDaughsS2.push_back( ++motherPartIdx_ ); //push index of top daughter
	  iWS2=motherPartIdx_; //keep W idx for the map
	  //status 3
	  GenParticle* candS3 = new GenParticle( wQ, wP4S3, wVtx, wQ*TopDecayID::WID, TopDecayID::unfrag, false);
	  auto_ptr<GenParticle> ptrS3( candS3 );
	  sel.push_back( *ptrS3 ); 
	  topDaughsS3.push_back( ++motherPartIdx_ ); //push index of top daughter
	  iWS3=motherPartIdx_; //keep W idx for the map
	  //status 4
	  GenParticle* candS4 = new GenParticle( wQ, wP4S4, wVtx, wQ*TopDecayID::WID, TopDecayID::unfrag, false);
	  auto_ptr<GenParticle> ptrS4( candS4 );
	  sel.push_back( *ptrS4 ); 
	  topDaughsS4.push_back( ++motherPartIdx_ ); //push index of top daughter
	  iWS4=motherPartIdx_; //keep W idx for the map
	  //reset W quantities for next W boson
	  wQ  = 0;
	  wVtx= reco::Particle::Point(0, 0, 0);
	  wP4S2 = reco::Particle::LorentzVector(0, 0, 0, 0);
	  wP4S3 = reco::Particle::LorentzVector(0, 0, 0, 0);
	  wP4S4 = reco::Particle::LorentzVector(0, 0, 0, 0);
	}
	if(td->status()==TopDecayID::unfrag && (abs( td->pdgId()==TopDecayID::glueID || abs( td->pdgId() )<TopDecayID::bID))){
	  //is Top radiation: mainly gluons + qqbar
	  GenParticle* cand = new GenParticle( td->threeCharge(), td->p4(), td->vertex(), td->pdgId(), td->status(), false );
	  auto_ptr<GenParticle> ptr( cand );
	  sel.push_back( *ptr );	  
	  topDaughsS3.push_back( ++motherPartIdx_ ); //push index of top daughter
	}
      }
      refs_[ iTopS2 ]=topDaughsS2;
      refs_[ iTopS3 ]=topDaughsS3;
      refs_[ iTopS4 ]=topDaughsS4;
      refs_[ iWS2   ]=wDaughsS2;
      refs_[ iWS3   ]=wDaughsS3;
      refs_[ iWS4   ]=wDaughsS4;
    }
  }
}

void 
TopDecaySubset::fillReferences(const reco::GenParticleRefProd& ref, reco::GenParticleCollection& sel)
{ 
 GenParticleCollection::iterator p=sel.begin();
 for(int idx=0; p!=sel.end(); ++p, ++idx){
 //find daughter reference vectors in refs_ and add daughters
   map<int, vector<int> >::const_iterator daughters=refs_.find( idx );
   if( daughters!=refs_.end() ){
     vector<int>::const_iterator daughter = daughters->second.begin();
     for( ; daughter!=daughters->second.end(); ++daughter){
       GenParticle* part = dynamic_cast<GenParticle* > (&(*p));
       if(part == 0){
	 throw edm::Exception( edm::errors::InvalidReference, "Not a GenParticle" );
       }
       part->addDaughter( GenParticleRef(ref, *daughter) );
       sel[*daughter].addMother( GenParticleRef(ref, idx) );
     }
   }
 }
}

void 
TopDecaySubset::addTauDaughters(int& idx, const GenParticle::const_iterator part, reco::GenParticleCollection& sel)
{
  vector<int> daughters;
  int idx0 = idx;
  for(GenParticle::const_iterator daughter=part->begin(); daughter!=part->end(); ++daughter){
    GenParticle* cand = new GenParticle( daughter->threeCharge(), daughter->p4(), daughter->vertex(), daughter->pdgId(), daughter->status(), false);
    auto_ptr<GenParticle> ptr( cand );
    sel.push_back( *ptr );
    daughters.push_back( ++idx ); //push index of daughter
    addTauDaughters(idx,daughter,sel);   //continue recursively
  }  
  if(daughters.size()) {
     refs_[ idx0 ] = daughters;
  }
}

void 
TopDecaySubset::addRadiation(int& idx, const GenParticle::const_iterator part, reco::GenParticleCollection& sel)
{
  vector<int> daughters;
  int idx0 = idx;
  for(GenParticle::const_iterator daughter=part->begin(); daughter!=part->end(); ++daughter){
    if(daughter->status()<=TopDecayID::stable && daughter->pdgId()!=part->pdgId()){
      // skip comment lines and make sure that
      // the daughter is not double counted
      GenParticle* cand = new GenParticle( daughter->threeCharge(), daughter->p4(), daughter->vertex(), daughter->pdgId(), daughter->status(), false);
      auto_ptr<GenParticle> ptr( cand );
      sel.push_back( *ptr );
      daughters.push_back( ++idx ); //push index of daughter
    }
  }  
  if(daughters.size()) {
     refs_[ idx0 ] = daughters;
  }
}

void 
TopDecaySubset::printSource(const reco::GenParticleCollection& src, const int& pdgId=0)
{
  unsigned int idx=0;
  std::string linestr;
  for(GenParticleCollection::const_iterator q=src.begin(); q!=src.end(); ++q, ++idx){
    unsigned int jdx=0;
    std::string daugstr; // keeps pdgIds of daughters
    for(GenParticle::const_iterator qd=q->begin(); qd!=q->end(); ++qd, ++jdx){
      if(jdx<kMAX){
	char buffer[5];
	sprintf(buffer, "%i", qd->pdgId());
	daugstr += buffer;
	if(qd+1 != q->end()){
	  daugstr += ",";
	}
      }
      else if(jdx==kMAX){
	char buffer[5];
	sprintf(buffer, "%i", q->numberOfDaughters());
	daugstr += "...(";
	daugstr += buffer;
	daugstr += ")";
      }
    }
    daugstr += "\n";
    char buffer[100];
    sprintf(buffer, "%8i%15i%10i%12f%25s", idx, q->pdgId(), q->status(), q->px(), daugstr.c_str());
    linestr += buffer; 
  }
  edm::LogVerbatim( "inputChain" ) 
    << "\nParticle-idx      pdgId      status        pdgId of Daughters"
    << "\n============================================================="
    << "\n" << linestr
    << "------------------------\n"
    << "'...' means more than" << kMAX;
}


void 
TopDecaySubset::printTarget(reco::GenParticleCollection& sel, const int& pdgId=0)
{
  edm::LogVerbatim log("decayChain");
  log << "\n   idx   pdg   stat      px          py         pz             mass          daughter pdg's  "
      << "\n===========================================================================================\n";

  for(unsigned int t=0; t<sel.size(); ++t){
    if( sel[t].pdgId()==TopDecayID::tID ){
      // restrict to the top in order 
      // to have it shown only once      
      GenParticleCollection::iterator p=sel.begin();
      int idx=0;
      for(GenParticleCollection::iterator p=sel.begin(); p!=sel.end(); ++p, ++idx){
	// loop the top daughters
	log << std::right << std::setw( 5) << idx
	    << std::right << std::setw( 7) << sel[idx].pdgId()
	    << std::right << std::setw( 5) << sel[idx].status() << "  "
	    << std::right << std::setw(10) << std::setprecision( 6 ) << sel[idx].p4().x() << "  "	
	    << std::right << std::setw(10) << std::setprecision( 6 ) << sel[idx].p4().y() << "  "	
	    << std::right << std::setw(10) << std::setprecision( 6 ) << sel[idx].p4().z() << "  "	
	    << std::right << std::setw(15) << std::setprecision( 6 ) << sel[idx].p4().mass() 
	    << "   ";
	// search for potential daughters; if they exits 
	// print the daughter to the screen in the last 
	// column of the table separated by ','
	TString pdgIds;
	map<int, vector<int> >::const_iterator daughters=refs_.find( idx );
	if( daughters!=refs_.end() ){	  
	  for(vector<int>::const_iterator d = daughters->second.begin(); d!=daughters->second.end(); ++d){
	    pdgIds+=sel[*d].pdgId();
	    if(d+1 != daughters->second.end()){
	      pdgIds+= ",";
	    }
	  }
	  log << std::setfill( ' ' ) << std::right << std::setw(15) << pdgIds; 
	  log << "\n";
	}
	else{
	  log << std::setfill( ' ' ) << std::right << std::setw(15) << "-\n";
	}
      }
    }
  }
}
