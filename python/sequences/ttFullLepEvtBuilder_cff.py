import FWCore.ParameterSet.Config as cms

#
# produce ttFullLepEvent structure with all necessary ingredients,
# needs ttGenEvent as input
#

## std sequence to produce the ttFullLepEventHypotheses
from TopQuarkAnalysis.TopEventProducers.sequences.ttFullLepEvtHypotheses_cff import *

## configure ttFullLepEventBuilder
from TopQuarkAnalysis.TopEventProducers.producers.TtFullLepEvtBuilder_cfi import *

## synchronize maxNJets in all hypotheses
ttFullLepJetPartonMatch            .maxNJets = ttFullLepEvent.maxNJets
kinSolutionTtFullLepEventHypothesis.maxNJets = ttFullLepEvent.maxNJets

## make ttFullLepEvent
makeTtFullLepEvent = cms.Sequence(makeTtFullLepHypotheses *
                                  ttFullLepEvent
                                 )

