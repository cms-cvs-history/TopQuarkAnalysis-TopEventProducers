import FWCore.ParameterSet.Config as cms

#
# tqaf Layer 2 for semi-leptonic event selections
#

## produce ttGenEvt and initialize ttGenEvtFilters
from TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff import *
from TopQuarkAnalysis.TopEventProducers.sequences.ttGenEventFilters_cff import *

## apply objects count selection
from TopQuarkAnalysis.TopEventProducers.sequences.numJetFilter_ttSemiLeptonic_cff import *
from TopQuarkAnalysis.TopEventProducers.sequences.numLeptonFilter_ttSemiLeptonic_cff import *

## produce event hypotheses
from TopQuarkAnalysis.TopEventProducers.producers.TtSemiEvtSolProducer_cfi import *
from TopQuarkAnalysis.TopEventProducers.sequences.tqafEventHyp_ttSemiLeptonic_cff import *

## make tqaf layer2
tqafLayer2_ttSemiLeptonic = cms.Sequence(makeGenEvt*makeTtSemiEvent*solutions)

## make tqaf layer2 filtered for full leptonic decays
tqafLayer2_ttSemiLeptonic_FullLepFilter = cms.Sequence(makeGenEvt*ttFullyLeptonicFilter*solutions)

## make tqaf layer2 filtered for semi-leptonic decays
tqafLayer2_ttSemiLeptonic_SemiLepFilter = cms.Sequence(makeGenEvt*ttSemiLeptonicFilter*makeTtSemiEvent*solutions)

## make tqaf layer2 filtered for full hadronic decays
tqafLayer2_ttSemiLeptonic_FullHadFilter = cms.Sequence(makeGenEvt*ttFullyHadronicFilter*solutions)
