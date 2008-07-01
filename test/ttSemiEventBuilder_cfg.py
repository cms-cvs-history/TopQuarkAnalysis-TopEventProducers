import FWCore.ParameterSet.Config as cms

##
# test cfg file for semi-leptonic ttbar analyses
##
process = cms.Process("TEST")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

##
# process configuration
##
process.load("TopQuarkAnalysis.Examples.test.RecoInput_cfi")

##
# tqaf configuration
##

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_full_cff")

## std sequence to produce the ttGenEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

## std sequence to produce event hypothesis for semi-leptonic ttbar decays
process.load("TopQuarkAnalysis.TopEventProducers.sequences.tqafEventHyp_ttSemiLeptonic_cff")

## process path
process.p = cms.Path(process.tqafLayer1*process.makeGenEvt*process.makeTtSemiEvent)
