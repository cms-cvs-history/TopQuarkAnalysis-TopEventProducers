import FWCore.ParameterSet.Config as cms

## produce ttGenEvent
from TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff import *

## apply objects count selection for *leptons*
from TopQuarkAnalysis.TopEventProducers.sequences.numLepFilter_ttSemiLeptonic_cff import *

## apply objects count selection for *jets*
from TopQuarkAnalysis.TopEventProducers.sequences.numJetFilter_ttSemiLeptonic_cff import *

## produce event solution (legacy)
from TopQuarkAnalysis.TopEventProducers.producers.TtSemiEvtSolProducer_cfi import *

## make tqaf layer2
tqafLayer2_ttSemiLeptonic_oldStyle = cms.Sequence(makeGenEvt *
                                                  solutions
                                                  )
