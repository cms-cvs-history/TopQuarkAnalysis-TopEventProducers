import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# tqaf Layer 2 for semi-leptonic event selections
#-------------------------------------------------

## apply objects count selection for leptons
from TopQuarkAnalysis.TopEventProducers.sequences.numLeptonFilter_ttSemiLeptonic_cff import *

## apply objects count selection for jets
from TopQuarkAnalysis.TopEventProducers.sequences.numJetFilter_ttSemiLeptonic_cff import *

## produce mva discriminant for signal selection
from TopQuarkAnalysis.TopEventProducers.sequences.ttSemiLepMVASelection_cff import *

## produce ttSemiEvent
from TopQuarkAnalysis.TopEventProducers.sequences.ttSemiLepEvtBuilder_cff import *

## produce event solution (legacy)
from TopQuarkAnalysis.TopEventProducers.producers.TtSemiEvtSolProducer_cfi import *


## make tqaf layer2
tqafLayer2_ttSemiLeptonic = cms.Sequence(makeTtSemiLepMVASelelection +
                                         makeTtSemiLepEvent +
                                         solutions
                                         )

## make tqaf layer2 filtered for full leptonic decays
tqafLayer2_ttSemiLeptonic_fullLepFilter = cms.Sequence(makeTtSemiLepMVASelelection +
                                                       makeTtSemiLepEvent_fullLepFilter +
                                                       solutions
                                                       )

## make tqaf layer2 filtered for semi-leptonic decays
tqafLayer2_ttSemiLeptonic_semiLepFilter = cms.Sequence(makeTtSemiLepMVASelelection +
                                                       makeTtSemiLepEvent_semiLepFilter +
                                                       solutions
                                                       )

## make tqaf layer2 filtered for full hadronic decays
tqafLayer2_ttSemiLeptonic_fullHadFilter = cms.Sequence(makeTtSemiLepMVASelelection +
                                                       makeTtSemiLepEvent_fullHadFilter +
                                                       solutions
                                                       )

