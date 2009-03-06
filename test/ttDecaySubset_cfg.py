import FWCore.ParameterSet.Config as cms

#-------------------------------------------------
# test cfg file for the production of a 
# ttSemiEvent
#-------------------------------------------------
process = cms.Process("TEST")

## add message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
## print original listing of gen particles
process.MessageLogger.categories.append('TopDecaySubset::printSource')
## print final pruned listing of top decay chain
process.MessageLogger.categories.append('TopDecaySubset::printTarget')
process.MessageLogger.cout = cms.untracked.PSet(
 INFO = cms.untracked.PSet(
   limit = cms.untracked.int32(0),
   decayChain = cms.untracked.PSet( limit = cms.untracked.int32(10) )
  )
)

#-------------------------------------------------
# process configuration
#-------------------------------------------------

## define input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    fileNames = cms.untracked.vstring(
  # PAT test sample for 2.2.X
   'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_2_X_2008-11-03-STARTUP_V7-AODSIM.100.root'
  # PAT test sample for 2.1.X
  #'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_1_X_2008-07-08_STARTUP_V4-AODSIM.100.root'
    )
)

## define maximal number of events to loop over
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2)
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


#-------------------------------------------------
# tqaf configuration
#-------------------------------------------------

## std sequence to produce the ttSemiEvent
process.load("TopQuarkAnalysis.TopEventProducers.producers.TopDecaySubset_cfi")

## process path
process.p = cms.Path(process.decaySubset)
