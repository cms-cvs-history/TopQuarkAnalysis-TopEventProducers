import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# test cfg file for tqaflayer1 & 2 production from
# fullsim for semi-leptonic ttbar events 
#-------------------------------------------------
process = cms.Process("TEST")

## configure message logger
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
# tqaf configuration; if you want just to produce 
# tqafLayer2 on top of an already existing
# tqafLayer1 just comment the standard tqafLayer1
# production sequence
#-------------------------------------------------

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_full_cff")

## std sequence for tqaf layer2 for semi-leptonic decays
process.load("TopQuarkAnalysis.TopEventProducers.tqafLayer2_ttSemiLeptonic_cff")

## process path
process.p = cms.Path(process.tqafLayer1 *
                     process.tqafLayer2_ttSemiLeptonic
                     )

#-------------------------------------------------
# tqaf event content; first ALL objects are
# dropped in this process; then tqafLayer1 and
# tqafLayer2 concent is added
#-------------------------------------------------

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

#-------------------------------------------------
# process output; first the event selection is
# defined: only those events that have passed the
# full production path are selected and written
# to file; the event content has been defined
# above
#-------------------------------------------------

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

#-------------------------------------------------
# output paths; in order not to write the
# persistent output to file comment the the output
# path
#-------------------------------------------------

## output
process.outpath = cms.EndPath(process.out)
