import FWCore.ParameterSet.Config as cms

#
# produce ttFullHadEvent structure with all necessary ingredients,
# needs ttGenEvent as input
#

## std sequence to produce the ttFullHadEventHypotheses
from TopQuarkAnalysis.TopEventProducers.sequences.ttFullHadEvtHypotheses_cff import *

## configure ttFullHadEventBuilder
from TopQuarkAnalysis.TopEventProducers.producers.TtFullHadEvtBuilder_cfi import *

## make ttFullHadEvent
makeTtFullHadEvent = cms.Sequence(makeTtFullHadHypotheses *
                                  ttFullHadEvent
                                  )


################################################################################
## helper functions
## (examples of usage can be found in the ttFullHadEvtBuilder_cfg.py)
################################################################################


## set a specific attribute for all hypotheses to a given value
## -> this works for "jets", "maxNJets", "jetCorrectionLevel"
def setForAllTtFullHadHypotheses(process, attribute, value):
    modules = ["ttFullHadJetPartonMatch",
               "ttFullHadHypGenMatch"]
    for obj in range(len(modules)):
        object = getattr(process, modules[obj])
        if hasattr(object, attribute):
            setattr(object, attribute, value)
