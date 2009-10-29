import FWCore.ParameterSet.Config as cms

#
# produce ttFullHadEvent structure with all necessary ingredients,
# needs ttGenEvent as input
#

## std sequence to produce the ttFullHadEventHypotheses
from TopQuarkAnalysis.TopEventProducers.sequences.ttFullHadEvtHypotheses_cff import *

## configure ttFullHadEventBuilder
from TopQuarkAnalysis.TopEventProducers.producers.TtFullHadEvtBuilder_cfi import *

## synchronize maxNJets in all hypotheses
ttFullHadJetPartonMatch.maxNJets = ttFullHadEvent.maxNJets

## synchronize jet correction level in all hypotheses
ttFullHadHypGenMatch.jetCorrectionLevel = "abs"

## make ttFullHadEvent
makeTtFullHadEvent = cms.Sequence(makeTtFullHadHypotheses *
                                  ttFullHadEvent
                                  )

