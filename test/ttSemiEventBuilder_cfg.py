import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# test cfg file for the production of a 
# ttSemiEvent
#-------------------------------------------------
process = cms.Process("TEST")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

#-------------------------------------------------
# process configuration
#-------------------------------------------------


## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTbar-210p5.1-AODSIM.100.root')
)

## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

## configure process options
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

#-------------------------------------------------
# tqaf configuration
#-------------------------------------------------

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_full_cff")

## std sequence to produce the ttSemiEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttSemiEventBuilder_cff")

## process path
process.p = cms.Path(process.tqafLayer1  *
                     process.makeTtSemiEvent
                     )
