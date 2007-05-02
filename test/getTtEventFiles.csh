#!/bin/tcsh -f

set inputdir = "/becms/heyninck/CMSSW_1_2_0_TtEvents/"
rm TtSemiEvents.cfi

touch TtSemiEvents.cfi
echo "//////////////////////// \n// RECO Input (digi) //\n////////////////////////"  >> TtSemiEvents.cfi
echo "\n\n\nsource = PoolSource {"  >> TtSemiEvents.cfi
echo "  untracked vstring fileNames = ">> TtSemiEvents.cfi
echo "    {\c">> TtSemiEvents.cfi

set i = 0 
foreach f(`ls -X /pnfs/iihe$inputdir|grep root`) 
  if ($i>0) echo ",\c" >>TtSemiEvents.cfi
  echo "\n      \042${inputdir}${f}\042\c" >>TtSemiEvents.cfi
  @ i = $i + 1
end

echo "\n    }">> TtSemiEvents.cfi
echo "  untracked int32   maxEvents = 5">> TtSemiEvents.cfi
echo "  untracked uint32 skipEvents = 0">> TtSemiEvents.cfi
echo "}">> TtSemiEvents.cfi

echo collected input of `grep "$inputdir" TtSemiEvents.cfi|wc -l` files
