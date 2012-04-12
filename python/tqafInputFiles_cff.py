import FWCore.ParameterSet.Config as cms

# from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles
# relValTTbar = pickRelValInputFiles(cmsswVersion  = 'CMSSW_5_0_0_pre6',
#                                    relVal        = 'RelValProdTTbar',
#                                    globalTag     = 'START50_V5',
#                                    dataTier      = 'AODSIM',
#                                    maxVersions   = 1
#                                    )
relValTTbar = cms.untracked.vstring(
  '/store/relval/CMSSW_5_0_0/RelValProdTTbar/AODSIM/START50_V8-v3/0082/38FB16A8-542B-E111-A9E6-003048FFCC1E.root',
  '/store/relval/CMSSW_5_0_0/RelValProdTTbar/AODSIM/START50_V8-v3/0074/E871A010-DD2A-E111-83F5-001A928116CE.root'
  )
