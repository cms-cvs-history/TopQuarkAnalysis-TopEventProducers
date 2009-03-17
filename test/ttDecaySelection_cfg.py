import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
## print original listing of gen particles
## process.MessageLogger.categories.append('TopDecaySubset::printSource')
## print final pruned listing of top decay chain
## process.MessageLogger.categories.append('TopDecaySubset::printTarget')
## print final pruned listing of top decay chain
process.MessageLogger.categories.append('TtDecayChannelSelector::selection')
process.MessageLogger.cout = cms.untracked.PSet(
 INFO = cms.untracked.PSet(
   limit = cms.untracked.int32(0),
   decayChain = cms.untracked.PSet( limit = cms.untracked.int32(10) )
  )
)

## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
   'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_2_X_2008-11-03-STARTUP_V7-AODSIM.100.root'
    )
)

## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

## configure process options
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

## configure geometry
process.load("Configuration.StandardSequences.Geometry_cff")

## configure conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP_V7::All')

## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")

## create ttGenEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
process.decaySubset.addRadiatedGluons = True
process.genEvt.src = "decaySubset:ME"
process.genEvtProc = cms.Path(process.makeGenEvt)
               
## std sequence to produce the ttSemiEvent
process.load("TopQuarkAnalysis.TopEventProducers.producers.TtDecaySelection_cfi")
process.ttDecaySelection.src = "genEvt"
process.ttDecaySelection.allowedTopDecays.decayBranchA.electron = True
process.ttDecaySelection.allowedTopDecays.decayBranchA.muon     = True
process.ttDecaySelection.allowedTopDecays.decayBranchB.electron = False
process.ttDecaySelection.allowedTopDecays.decayBranchB.muon     = False
process.ttDecaySelection.allowedTauDecays.leptonic              = True
process.ttDecaySelection.allowedTauDecays.oneProng              = True
process.ttDecaySelection.allowedTauDecays.threeProng            = True
process.ttDecaySelectionProc = cms.Path(process.ttDecaySelection)
