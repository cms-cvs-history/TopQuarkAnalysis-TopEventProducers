##
#
# This is an example PAT configuration as a start off for
# a recommended Top PAG PAT tuple; NOTE the it contains all
# information that might be of interest for you. You might
# want to do changes or adaptations to make this sample
# suit to your concrete analysis case.
#
##

## Starting with a skeleton process, which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

##
# Reconfigure the workflow depending on whether you run on data or on MC
##

## Remove MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])

##
# Top PAG specific modifications of the PAT default (this aims for completeness,
# not for disc space optimization)
##

## Exploit MET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')

## Add further jet collections
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5', 'PF',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5', 'PF'),
                 doType1MET   = False, ## there is no use to apply residual jet corrections on CaloMET 
                 doL1Cleaning = True,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = False  ## there is no use of jetID for Pflow jets
                 )

## Produce jpt corrected calo jets, which are not on AOD per default
process.load("PhysicsTools.PatAlgos.recoLayer0.jetPlusTrack_cff")
process.jpt = cms.Path(
    process.jptCaloJets
)

addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'AK5', 'JPT',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = None,  ## jpt is viewed as a jet correction; there is no residual 
                 doType1MET   = False, ## correction on top of that; there is also no MET correction
                 doL1Cleaning = True,
                 doL1Counters = True,                 
                 genJetCollection = cms.InputTag("ak5GenJets"),
                 doJetID      = False
                 )


## Set the right sample for JEC factors here
process.patJetCorrFactors.corrSample      = 'Summer09_7TeV'
process.patJetCorrFactorsAK5PF.corrSample = 'Summer09_7TeV'

## Apply hadron level corrections as derived
## from ttbar instead of dijet qcd events
process.patJetCorrFactors.sampleType      = 'ttbar'
process.patJetCorrFactorsAK5PF.sampleType = 'ttbar'

## Embed calo towers and pflow candidates; you
## might want to keep the caloTowers instead
process.patJets.embedCaloTowers           = True
process.patJetsAK5PF.embedPFCandidates    = True

## Run the pat default dequence
process.p = cms.Path(
  process.patDefaultSequence
)

##
# Configure the event content and output
##

## Define output name
process.out.fileName = "topPAG_ntuple.root"

## Define event content; keep the cleanPatCandidates which are
## the same as selectedPatCandidates PLUS some extra information
## on overlaps across collections.
from PhysicsTools.PatAlgos.patEventContent_cff import *
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += ["drop *_towerMaker_*_*"]
process.out.outputCommands += ["drop l1extraParticles_*_*_*"]

##
# configure input
##

#process.GlobalTag.globaltag =  'ADD_YOUR_FAVORITE_GLOBAL_TAG'
#process.source.fileNames    = ['ADD_YOUR_FAVORITE_INPUT_FILE' ]
process.maxEvents.input     =  1000
