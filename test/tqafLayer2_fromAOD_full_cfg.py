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
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTbar-210p5.1-AODSIM.100.root')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

##
# tqaf configuration
##

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_full_cff")

## std sequence for tqaf layer2 for semi-leptonic decays
process.load("TopQuarkAnalysis.TopEventProducers.tqafLayer2_ttSemiLeptonic_cff")

##
# add tqaf event content
##

## define event content
process.tqafEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *')
)
## define tqaf layer1 event content
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_EventContent_cff")
process.tqafEventContent.outputCommands.extend(process.patLayer1EventContent.outputCommands)

## define tqaf layer2 event content
process.load("TopQuarkAnalysis.TopEventProducers.tqafLayer2_EventContent_cff")
process.tqafEventContent.outputCommands.extend(process.tqafLayer2TtSemiLeptonicEventContent.outputCommands)

##
# define process output
##

## define event selection
process.EventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    )
)

## configure output module
process.out = cms.OutputModule("PoolOutputModule",
    process.EventSelection,
    process.tqafEventContent,
    verbose = cms.untracked.bool(False),
    fileName = cms.untracked.string('TQAFLayer2_Output.fromAOD_full.root')
)

## process path
process.p = cms.Path(process.tqafLayer1*process.tqafLayer2_ttSemiLeptonic)

## output
process.outpath = cms.EndPath(process.out)
