import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.categories.append('TopDecaySubset_printTarget')
process.MessageLogger.categories.append('TtDecayChannelSelector_selection')
process.MessageLogger.cerr.TopDecaySubset_printTarget = cms.untracked.PSet(
    limit = cms.untracked.int32(10)
)
process.MessageLogger.cerr.TtDecayChannelSelector_selection = cms.untracked.PSet(
    limit = cms.untracked.int32(10)
)

## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_1_0_pre10/RelValTTbar/GEN-SIM-RECO/IDEAL_31X_v1/0008/CC80B73A-CA57-DE11-BC2F-000423D99896.root'
    )
)
## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
## configure process options
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

## configure geometry & conditions
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_31X_V1::All')

## configure genEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
process.decaySubset.addRadiatedGluons = True
process.genEvt.src = "decaySubset:ME"
## path1
process.genEvtProc = cms.Path(process.makeGenEvt)
               
## configure decay channel selection 
process.load("TopQuarkAnalysis.TopEventProducers.producers.TtDecaySelection_cfi")
process.ttDecaySelection.src = "genEvt"
process.ttDecaySelection.allowedTopDecays.decayBranchA.electron = True
process.ttDecaySelection.allowedTopDecays.decayBranchA.muon     = True
process.ttDecaySelection.allowedTopDecays.decayBranchB.electron = False
process.ttDecaySelection.allowedTopDecays.decayBranchB.muon     = False
process.ttDecaySelection.allowedTauDecays.leptonic              = True
process.ttDecaySelection.allowedTauDecays.oneProng              = True
process.ttDecaySelection.allowedTauDecays.threeProng            = True
## path2
process.ttDecaySelectionProc = cms.Path(process.ttDecaySelection)
