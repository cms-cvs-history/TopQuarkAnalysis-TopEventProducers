\import FWCore.ParameterSet.Config as cms

#
# produce ttFullHadLep event hypotheses
#

## genMatch hypothesis
from TopQuarkAnalysis.TopJetCombination.TtFullHadHypGenMatch_cff import *

## make all considered event hypotheses
makeTtFullHadHypotheses = cms.Sequence(makeHypothesis_genMatch)

