import FWCore.ParameterSet.Config as cms

process = cms.Process("TQAF")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("TopQuarkAnalysis.Examples.test.RecoInput_cfi")

process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_full_cff")

process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

process.load("TopQuarkAnalysis.TopEventProducers.sequences.tqafEventHyp_ttSemiLeptonic_cff")

process.p = cms.Path(process.tqafLayer1*process.makeGenEvt*process.makeTtSemiEvent)
process.MessageLogger.cerr.threshold = 'INFO'

