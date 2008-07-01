import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.TopEventProducers.producers.TopInitSubset_cfi import *
from TopQuarkAnalysis.TopEventProducers.producers.TopDecaySubset_cfi import *
from TopQuarkAnalysis.TopEventProducers.producers.StGenEvtProducer_cfi import *
makeGenEvt = cms.Sequence(initSubset*decaySubset*genEvt_SingleTop)

