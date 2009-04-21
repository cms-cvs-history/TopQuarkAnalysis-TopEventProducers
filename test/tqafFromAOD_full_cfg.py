import FWCore.ParameterSet.Config as cms

process = cms.Process("TQAF")

## configure message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

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
process.GlobalTag.globaltag = cms.string('IDEAL_V7::All')

# magnetic field now needs to be in the high-level py
process.load("Configuration.StandardSequences.MagneticField_cff")

#-------------------------------------------------
# tqaf configuration; if you want to produce tqaf
# on top of an already existing pat layer1 comment
# the patDefaultSequence from the path
#-------------------------------------------------

## std sequence for pat
process.load("PhysicsTools.PatAlgos.patSequences_cff")
## std sequence for tqaf
process.load("TopQuarkAnalysis.TopEventProducers.tqafSequences_cff")
## switch from icone5 to scone5
##from PhysicsTools.PatAlgos.tools.jetTools import *
##switchJetCollection(process, 
##                    cms.InputTag('sisCone5CaloJets'), # Jet collection; must be already in the event when patDefaultSequence is executed
##                    doJTA           =True,            # Run Jet-Track association & JetCharge
##                    doBTagging      =True,            # Run b-tagging
##                    jetCorrLabel    =('SC5','Calo'),  # example jet correction name; set to None for no JEC
##                    doType1MET      =True,            # recompute Type1 MET using these jets
##                    genJetCollection=cms.InputTag("sisCone5GenJets")
##                    ) 
## default replacements from B22X -> B22X_v2 in other packages
process.ttSemiLepHypGeom.mets               = "layer1METs"
process.ttSemiLepHypKinFit.mets             = "layer1METs"
process.ttSemiLepHypMVADisc.mets            = "layer1METs"
process.ttSemiLepHypGenMatch.mets           = "layer1METs"
process.findTtSemiLepJetCombMVA.mets        = "layer1METs"
process.ttSemiLepHypMaxSumPtWMass.mets      = "layer1METs"
process.ttSemiLepHypWMassMaxSumPt.mets      = "layer1METs"
process.findTtSemiLepSignalSelMVA.METs      = "layer1METs"
process.kinFitTtSemiLepEventSelection.mets  = "layer1METs"
process.kinFitTtSemiLepEventHypothesis.mets = "layer1METs"

## process path
process.p = cms.Path(process.patDefaultSequence *
                     process.tqafTtSemiLeptonic
                     )

## configure output module
process.out = cms.OutputModule("PoolOutputModule",
    fileName     = cms.untracked.string('tqafOutput.fromAOD_full.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring('drop *'),                      
    dropMetaDataForDroppedData = cms.untracked.bool(True)
)
process.outpath = cms.EndPath(process.out)

## pat content
from PhysicsTools.PatAlgos.patEventContent_cff import *
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patEventContentNoLayer1Cleaning
## tqaf content
from TopQuarkAnalysis.TopEventProducers.tqafEventContent_cff import *
process.out.outputCommands += tqafEventContent
