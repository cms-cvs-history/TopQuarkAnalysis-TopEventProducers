##
#
# This is an example PAT configuration as a start off for
# a recommended Top PAG PAT tuple
#
##

## Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

##
# Reconfigure the workflow depending on whether you run on data or on MC
##

## remove MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])

##
# Top PAG specific modifications of the PAT default (this aims for completeness,
# not doe disc space optimization)
##

## exploit MET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')

## add further jet collections
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'PF',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5', 'PF'),
                 doType1MET   = False,
                 doL1Cleaning = True,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = False
                 )

## produce jpt corrected calo jets, which are not on AOD per default
process.load("PhysicsTools.PatAlgos.recoLayer0.jetPlusTrack_cff")
process.jpt = cms.Path(
    process.jptCaloJets
)

addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'JPT',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = None,
                 doType1MET   = False,
                 doL1Cleaning = True,
                 doL1Counters = True,                 
                 genJetCollection = cms.InputTag("ak5GenJets"),
                 doJetID      = False
                 )


## intermediate fixes
process.jetCorrFactors.corrLevels.L7Parton = "L7Parton_AK5"
process.jetCorrFactorsPF.corrLevels.L7Parton = "L7Parton_AK5"

## run the pat default dequence
process.p = cms.Path(
    process.patDefaultSequence
    )

##
# configure the event content and output
##

## define output name
process.out.fileName = "topPAG_ntuple.root"

## define event content
from PhysicsTools.PatAlgos.patEventContent_cff import *
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent

##
# configure input
##

#process.GlobalTag.globaltag =  'ADD_YOUR_FAVORITE_GLOBAL_TAG'
#process.source.fileNames    = ['ADD_YOUR_FAVORITE_INPUT_FILE' ]
#process.maxEvents.input     =  -1
