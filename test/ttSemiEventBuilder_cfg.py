import FWCore.ParameterSet.Config as cms

<<<<<<< ttSemiEventBuilder_cfg.py
process = cms.Process("TQAF")
# initialize MessageLogger
=======
##
# test cfg file for semi-leptonic ttbar analyses
##
process = cms.Process("TEST")

## add message logger
>>>>>>> 1.2
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

<<<<<<< ttSemiEventBuilder_cfg.py
# input file
=======
##
# process configuration
##
>>>>>>> 1.2
process.load("TopQuarkAnalysis.Examples.test.RecoInput_cfi")

<<<<<<< ttSemiEventBuilder_cfg.py
# TQAF ###
# TQAF Layer 1 TopObject production
=======
##
# tqaf configuration
##

## std sequence for tqaf layer1
>>>>>>> 1.2
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_full_cff")

<<<<<<< ttSemiEventBuilder_cfg.py
# TQAF Layer 2 for the ttbar semi-leptonic final state
=======
## std sequence to produce the ttGenEvent
>>>>>>> 1.2
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

## std sequence to produce event hypothesis for semi-leptonic ttbar decays
process.load("TopQuarkAnalysis.TopEventProducers.sequences.tqafEventHyp_ttSemiLeptonic_cff")

## process path
process.p = cms.Path(process.tqafLayer1*process.makeGenEvt*process.makeTtSemiEvent)
