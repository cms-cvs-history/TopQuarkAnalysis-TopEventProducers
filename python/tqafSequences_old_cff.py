import FWCore.ParameterSet.Config as cms

## produce ttGenEvent
from TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff import *

## produce event solution
from TopQuarkAnalysis.TopEventProducers.producers.TtSemiEvtSolProducer_cfi import *

## make tqaf layer2
tqafLayer2_ttSemiLeptonic_oldStyle = cms.Sequence(makeGenEvt * solutions)
