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
    fileNames = cms.untracked.vstring(
   #PAT test sample
   #'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_1_X_2008-07-08_STARTUP_V4-AODSIM.100.root'
   #219 RelVal sample
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/16D75F0F-1186-DD11-80B9-000423D98C20.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/24CD41BB-1A86-DD11-9CDA-000423D98EC8.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/264D79AA-1786-DD11-9F3C-001617C3B6DC.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/327DE1B9-0E86-DD11-B7B1-000423D6C8E6.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/345AE083-1186-DD11-8D43-000423D99658.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/4A0ADB7D-1086-DD11-BD16-000423D98E6C.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/4E31E969-1886-DD11-8398-000423D9989E.root',
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v2/0000/4EEEA6AE-0886-DD11-90F9-000423D94990.root'
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

# Magnetic field now needs to be in the high-level py
process.load("Configuration.StandardSequences.MagneticField_cff")

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

## define tqaf layer1 event content
from TopQuarkAnalysis.TopObjectProducers.tqafLayer1_EventContent_cff import *
tqafLayer1EventContent(process)

#
# more or changed jet collections
#
from PhysicsTools.PatAlgos.tools.jetTools import *

switchJetCollection(process, 
                    'sisCone5CaloJets',      # jet collection; must be already in the event when patLayer0 sequence is executed
                    layers       = [0,1],    # if you're not running patLayer1, set 'layers=[0]' 
                    runCleaner   = "CaloJet",# =None if not to clean
                    doJTA        = True,     # run jet-track association & JetCharge
                    doBTagging   = True,     # run b-tagging
                    jetCorrLabel = 'Scone5', # example jet correction name; set to None for no JEC
                    doType1MET   = True      # recompute Type1 MET using these jets
                    )

## define tqaf layer2 event content
from TopQuarkAnalysis.TopEventProducers.tqafLayer2_EventContent_cff import *
tqafLayer2EventContent(process)


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
