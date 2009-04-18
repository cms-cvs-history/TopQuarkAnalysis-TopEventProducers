import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# test cfg file for the production of a 
# ttSemiEvent
#-------------------------------------------------
process = cms.Process("TEST")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.categories.append('TopGenEvent')
#process.MessageLogger.categories.append('TopDecaySubset_printSource')
#process.MessageLogger.categories.append('TopDecaySubset_printTarget')
#process.MessageLogger.categories.append('KinFitter')
process.MessageLogger.categories.append('TtSemiLeptonicEvent')
process.MessageLogger.categories.append('TtSemiLepKinFitter')

process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default                    = cms.untracked.PSet( limit = cms.untracked.int32( 1) ),
#   TopGenEvent                = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
#   TopDecaySubset_printSource = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
#   TopDecaySubset_printTarget = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    TtSemiLeptonicEvent        = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    TtSemiLepKinFitter         = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
#   KinFitter           = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)

#-------------------------------------------------
# process configuration
#-------------------------------------------------

## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
   #PAT test sample for 2.2.X
    'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_2_X_2008-11-03-STARTUP_V7-AODSIM.100.root'
   #PAT test sample for 2.1.X
   #'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_1_X_2008-07-08_STARTUP_V4-AODSIM.100.root'    
    ),
    skipEvents = cms.untracked.uint32(0)
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
process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')

## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")


#-------------------------------------------------
# tqaf configuration
#-------------------------------------------------

## std sequence for tqaf layer1
process.load("TopQuarkAnalysis.TopObjectProducers.tqafLayer1_cff")

## std sequence to produce the ttGenEvt
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

## std sequence to produce the ttSemiLepEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttSemiLepEvtBuilder_cff")
process.ttSemiLepEvent.verbosity = 1

## change maximum number of jets taken into account per event (default: 4)
#process.ttSemiLepEvent.maxNJets = 5
## change maximum number of jet combinations taken into account per event (default: 1)
#process.findTtSemiLepJetCombMVA       .maxNComb = -1
#process.kinFitTtSemiLepEventHypothesis.maxNComb = -1

## necessary fixes to run 2.2.X on 2.1.X data
## comment this when running on samples produced
## with 22X
## from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
## run22XonSummer08AODSIM(process)

## process path
process.p = cms.Path(process.tqafLayer1  *
                     process.makeGenEvt  *
                     process.makeTtSemiLepEvent
                     )

## define event selection
process.EventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    )
)

## define tqaf layer1 event content
from TopQuarkAnalysis.TopObjectProducers.tqafLayer1_EventContent_cff import *
makeTqafLayer1EventContent(process)
## define tqaf layer2 event content
from TopQuarkAnalysis.TopEventProducers.tqafLayer2_EventContent_cff import *
makeTqafLayer2EventContent(process)

## configure output module
process.out = cms.OutputModule("PoolOutputModule",
    process.EventSelection,                      
    process.tqafEventContent,
    fileName = cms.untracked.string('ttSemiLepEvtBuilder.root')
)

## output path
## in order not to write the persistent output to
## file, comment the following outpath
process.outpath = cms.EndPath(process.out)
