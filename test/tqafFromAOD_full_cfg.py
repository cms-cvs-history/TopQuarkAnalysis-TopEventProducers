import FWCore.ParameterSet.Config as cms

process = cms.Process("TQAF")

## configure message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_1_0_pre6/RelValTTbar/GEN-SIM-RECO/IDEAL_31X_v1/0002/50D4BADB-FA32-DE11-BA01-000423D98DC4.root'    
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

## configure geometry & conditions
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_31X::All')


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

# replacements currently needed to make the taus work
process.allLayer1Taus.addTauID = False

## process path
process.p = cms.Path(process.patDefaultSequence *
                     process.tqafTtSemiLeptonic
                     )

## configure output module
process.out = cms.OutputModule("PoolOutputModule",
    fileName       = cms.untracked.string('tqafOutput.fromAOD_full.root'),
    SelectEvents   = cms.untracked.PSet(SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring('drop *'),                      
    dropMetaData   = cms.untracked.string("DROPPED")  ## NONE    for none
                                                      ## DROPPED for drop for dropped data
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
