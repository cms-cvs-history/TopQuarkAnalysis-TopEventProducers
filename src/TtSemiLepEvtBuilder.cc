#include "FWCore/Framework/interface/MakerMacros.h"
#include "TopQuarkAnalysis/TopEventProducers/interface/TtSemiLepEvtBuilder.h"


TtSemiLepEvtBuilder::TtSemiLepEvtBuilder(const edm::ParameterSet& cfg) :
  hyps_    (cfg.getParameter<std::vector<edm::InputTag> >("hyps")),
  keys_    (cfg.getParameter<std::vector<edm::InputTag> >("keys")),
  matches_ (cfg.getParameter<std::vector<edm::InputTag> >("matches")),
  kinFit_  (cfg.getParameter<edm::ParameterSet>("kinFit")),
  decay_   (cfg.getParameter<int>("decay")),
  genEvt_  (cfg.getParameter<edm::InputTag>("genEvent"))
{
  if( cfg.exists("kinFit") ) {
    // get parameter subsets for kinFit
    fitChi2_=kinFit_.getParameter<edm::InputTag>("chi2");
    fitProb_=kinFit_.getParameter<edm::InputTag>("prob");
  }
  // get parameter subsets for genMatch
  if( cfg.exists("genMatch") ) {
    genMatch_= cfg.getParameter<edm::ParameterSet>("genMatch");
    sumPt_=genMatch_.getParameter<edm::InputTag>("sumPt");
    sumDR_=genMatch_.getParameter<edm::InputTag>("sumDR");
  }
  // get parameter subsets for mvaDisc
  if( cfg.exists("mvaDisc") ) {
    mvaDisc_= cfg.getParameter<edm::ParameterSet>("mvaDisc");
    meth_=mvaDisc_.getParameter<edm::InputTag>("meth");
    disc_=mvaDisc_.getParameter<edm::InputTag>("disc");
  }
  // produces an TtSemiLeptonicEvent from hypothesis
  // and associated extra information
  produces<TtSemiLeptonicEvent>();
}

TtSemiLepEvtBuilder::~TtSemiLepEvtBuilder()
{
}

void
TtSemiLepEvtBuilder::produce(edm::Event& evt, const edm::EventSetup& setup)
{
  TtSemiLeptonicEvent event;

  // set decay
  event.setDecay((TtSemiLeptonicEvent::Decay&)decay_);

  // set genEvent
  edm::Handle<TtGenEvent> genEvt;
  evt.getByLabel(genEvt_, genEvt);
  event.setGenEvent(genEvt);

  // set eventHypos
  typedef std::vector<edm::InputTag>::const_iterator EventHypo;
  for(EventHypo h=hyps_.begin(), k=keys_.begin(); h!=hyps_.end() && k!=keys_.end(); ++h, ++k){
    edm::Handle<int> key; 
    evt.getByLabel(*k, key);

    edm::Handle<reco::CompositeCandidate> hyp; 
    evt.getByLabel(*h, hyp);

    event.addEventHypo((TtSemiLeptonicEvent::HypoKey&)*key, *hyp);
  }

  // set jetMatch extras
  for(EventHypo k=keys_.begin(), m=matches_.begin(); k!=keys_.end() && m!=matches_.end(); ++k, ++m){
    edm::Handle<int> key; 
    evt.getByLabel(*k, key);

    edm::Handle<std::vector<int> > match;
    evt.getByLabel(*m, match);

    event.addJetMatch((TtSemiLeptonicEvent::HypoKey&)*key, *match);
  }

  // set kinFit extras
  edm::Handle<double> fitChi2;
  evt.getByLabel(fitChi2_, fitChi2);
  event.setFitChi2(*fitChi2);

  edm::Handle<double> fitProb;
  evt.getByLabel(fitProb_, fitProb);
  event.setFitProb(*fitProb);

  // set genMatch extras
  edm::Handle<double> sumPt;
  evt.getByLabel(sumPt_, sumPt);
  event.setGenMatchSumPt(*sumPt);  

  edm::Handle<double> sumDR;
  evt.getByLabel(sumDR_, sumDR);
  event.setGenMatchSumDR(*sumDR);  

  // set mvaDisc extras
  edm::Handle<std::string> meth;
  evt.getByLabel(meth_, meth);

  edm::Handle<double> disc;
  evt.getByLabel(disc_, disc);
  event.setMvaDiscAndMethod((std::string&)*meth, *disc);

  // feed out 
  std::auto_ptr<TtSemiLeptonicEvent> pOut(new TtSemiLeptonicEvent);
  *pOut=event;
  evt.put(pOut);
}
