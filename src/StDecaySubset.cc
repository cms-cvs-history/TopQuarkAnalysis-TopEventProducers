#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"
#include "TopQuarkAnalysis/TopEventProducers/interface/StDecaySubset.h"

using namespace std;
using namespace reco;

namespace StDecayID{
  static const int status = 3;
  static const int tID = 6;
  static const int bID = 5;
  static const int WID =24;
}

StDecaySubset::StDecaySubset(const edm::ParameterSet& cfg):
  src_ ( cfg.getParameter<edm::InputTag>( "src" ) )
{
  switchOption    = cfg.getParameter<int>("SwitchChainType");
  produces<reco::CandidateCollection>();
}

StDecaySubset::~StDecaySubset()
{
}

void
StDecaySubset::produce(edm::Event& evt, const edm::EventSetup& setup)
{     
  edm::Handle<reco::CandidateCollection> src;
  evt.getByLabel(src_, src);
 
  const reco::CandidateRefProd ref = evt.getRefBeforePut<reco::CandidateCollection>(); 
  std::auto_ptr<reco::CandidateCollection> sel( new reco::CandidateCollection );

  //fill output collection
  fillOutput( *src, *sel );
  //fill references
  fillRefs( ref, *sel );

  evt.put( sel );
}

Particle::LorentzVector StDecaySubset::fourVector(const reco::Candidate& p)
{
  Particle::LorentzVector vec;
  Candidate::const_iterator pd=p.begin();
  for( ; pd!=p.end(); ++pd){
    if( status( *pd )==StDecayID::status ){
      vec+=fourVector( *pd );
    }
    else{
      //skip W with status 2 to
      //prevent double counting
      if( abs(pd->pdgId())!=StDecayID::WID )
	vec+=pd->p4();
    }
  }
  return vec;
}

void StDecaySubset::fillOutput(const reco::CandidateCollection& src, reco::CandidateCollection& sel)
{
  if (switchOption==1) { // standard option: look for top and W, and navigate through the decay chains
    CandidateCollection::const_iterator t=src.begin();
    for(int idx=-1; t!=src.end(); ++t){
      if( status( *t )==StDecayID::status && abs( t->pdgId() )==StDecayID::tID ){ //is top      
	GenParticleCandidate* cand = new GenParticleCandidate( t->charge(), fourVector( *t ), 
							       t->vertex(), t->pdgId(), status( *t ) );
	auto_ptr<reco::Candidate> ptr( cand );
	sel.push_back( ptr );
	++idx;

	//keep top index for the map for 
	//management of the daughter refs 
	int iTop=idx, iW=0;
	vector<int> topDaughs, wDaughs;
	
	//iterate over top daughters
	Candidate::const_iterator td=t->begin();
	for( ; td!=t->end(); ++td){
	  if( status( *td )==StDecayID::status && abs( td->pdgId() )==StDecayID::bID ){ //is beauty	  
	    GenParticleCandidate* cand = new GenParticleCandidate( td->charge(), fourVector( *td ), 
								   td->vertex(), td->pdgId(), status( *td ) );
	    auto_ptr<Candidate> ptr( cand );
	    sel.push_back( ptr );	  
	    topDaughs.push_back( ++idx ); //push index of top daughter
	  }
	  if( status( *td )==StDecayID::status && abs( td->pdgId() )==StDecayID::WID ){ //is W boson
	    GenParticleCandidate* cand = new GenParticleCandidate( td->charge(), fourVector( *td ), 
								   td->vertex(), td->pdgId(), status( *td ) );
	    auto_ptr<Candidate> ptr( cand );
	    sel.push_back( ptr );
	    topDaughs.push_back( ++idx ); //push index of top daughter
	    
	    //keep W idx 
	    //for the map
	    iW=idx;

	    //iterate over W daughters
	    Candidate::const_iterator wd=td->begin();
	    for( ; wd!=td->end(); ++wd){
	      if (wd->pdgId() != td->pdgId()) {
		GenParticleCandidate* cand = new GenParticleCandidate( wd->charge(), fourVector( *wd ), 
								       wd->vertex(), wd->pdgId(), status( *wd ) );	      auto_ptr<Candidate> ptr( cand );
		sel.push_back( ptr );
		wDaughs.push_back( ++idx ); //push index of wBoson daughter
	      }
	    }
	  }
	}
	refs_[ iTop ]=topDaughs;
	refs_[ iW ]=wDaughs;
      }
    }
  } else if (switchOption==2) { // this is needed, for example, for the SingleTop generator, since it doesn't save the intermediate particles (lepton, neutrino and b are directly daughters of the incoming partons)

    int iP;
    vector<int> ipDaughs;

    CandidateCollection::const_iterator ip1=src.begin();
    CandidateCollection::const_iterator ip2=src.begin();
    for(int idx=-1; ip1!=src.end(); ++ip1){
      for(; ip2!=src.end(); ++ip2){

	//iterate over the daughters of both
	Candidate::const_iterator td1=ip1->begin();
	Candidate::const_iterator td2=ip2->begin();
	for( ; td1!=ip1->end(); ++td1){
	  for( ; td2!=ip2->end(); ++td2){
	    if (td1 == td2) { // daughter of both initial state partons

	      //	      ++idx;
	      //	      iP=idx;

	      GenParticleCandidate* cand = new GenParticleCandidate( td2->charge(), fourVector( *td2 ), 
								     td2->vertex(), td2->pdgId(), status( *td2 ) );
	      auto_ptr<Candidate> ptr( cand );
	      sel.push_back( ptr );	  
	      ipDaughs.push_back( ++idx ); //push index of daughter
	      iP=idx;
	    }
	    refs_[ iP ]=ipDaughs;
	  }
	}// end of double loop on daughters

      }
    }
  }

}

void StDecaySubset::fillRefs(const reco::CandidateRefProd& ref, reco::CandidateCollection& sel)
{ 
  CandidateCollection::iterator p=sel.begin();
  for(int idx=0; p!=sel.end(); ++p, ++idx){
    //find daughter reference vectors in refs_ and add daughters
    map<int, vector<int> >::const_iterator daughters=refs_.find( idx );
    if( daughters!=refs_.end() ){
      vector<int>::const_iterator daughter = daughters->second.begin();
      for( ; daughter!=daughters->second.end(); ++daughter){
	GenParticleCandidate* part = dynamic_cast<GenParticleCandidate* > (&(*p));
	if(part == 0){
	  throw edm::Exception( edm::errors::InvalidReference, "Not a GenParticleCandidate" );
	}
	part->addDaughter( CandidateRef(ref, *daughter) );
      }
    }
  }
}