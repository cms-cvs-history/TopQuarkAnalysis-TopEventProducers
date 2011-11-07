import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles
relValTTbar = pickRelValInputFiles(cmsswVersion  = 'CMSSW_4_2_9_HLT1',
                                   relVal        = 'RelValTTbar',
                                   globalTag     = 'MC_42_V14A',
                                   numberOfFiles = 1
                                   )
