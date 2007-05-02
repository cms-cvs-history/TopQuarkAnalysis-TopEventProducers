#!/bin/tcsh -f

set inputdir = "/becms/heyninck/CMSSW_1_2_0_RECO/"
rm TtRecoFiles.cfi

touch TtRecoFiles.cfi
echo "//////////////////////// \n// RECO Input (digi) //\n////////////////////////"  >> TtRecoFiles.cfi
echo "\n\n\nsource = PoolSource {"  >> TtRecoFiles.cfi
echo "  untracked vstring fileNames = ">> TtRecoFiles.cfi
echo "    {\c">> TtRecoFiles.cfi

set i = 0 
foreach f(`ls -X /pnfs/iihe$inputdir|grep root`) 
  if ($i>0) echo ",\c" >>TtRecoFiles.cfi
  echo "\n      \042${inputdir}${f}\042\c" >>TtRecoFiles.cfi
  @ i = $i + 1
end

echo "\n    }">> TtRecoFiles.cfi
echo "  untracked int32   maxEvents = 5">> TtRecoFiles.cfi
echo "  untracked uint32 skipEvents = 0">> TtRecoFiles.cfi
echo "}">> TtRecoFiles.cfi

echo collected input of `grep "$inputdir" TtRecoFiles.cfi|wc -l` files
