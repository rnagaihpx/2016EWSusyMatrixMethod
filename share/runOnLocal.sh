#For the people not using /bin/bash
if [ $SHELL != '/bin/bash' ]; then
    echo 'Please change your shell to /bin/bash to use this script!!'
    return
fi

#default target directory
TARGETDS=$1
if [ $# -lt 1 ]; then
  echo '========================================================================='
  echo 'This script execute "testRun" against all the dataset in [Dataset path].'
  echo 'Usage :'
  echo '$source runall.sh [Dataset path] [selction region1] [selection region2],,,'
  echo '========================================================================='
  echo 'Exitting ...'
  return 1
fi

#check if there is a specified target selection region
TARGETSELECREG=''
if [ $# -lt 2 ]; then
  echo 'Specify at least one target selection region (e.g. test,3lep...)'
  return 1
fi

#additional target selection region
while [ "$2" != "" ]
do
  TARGETSELECREG='-S '$2' '$TARGETSELECREG
  shift
done

echo Target DS directory     = $TARGETDS
echo Target selection region = $TARGETSELECREG

for DatasetDir in `\ls $TARGETDS`
do
#echo $DatasetDir
############################################
# Finding "data type" and "run number"
############################################
runnumStartPos=-1
runnumEndPos=-1
matched=-1
if [ $matched = -1 ]; then dataType=`echo $DatasetDir | cut -c 1-9` ;fi
if [ $dataType = "mc14_8TeV" ]; then    
    runnumStartPos=11
    runnumEndPos=16
    runnum=`echo $DatasetDir | cut -c $runnumStartPos-$runnumEndPos`
    matched=1
fi
if [ $matched = -1 ]; then dataType=`echo $DatasetDir | cut -c 1-10` ;fi
if [ $dataType = "mc14_13TeV" ]; then
    runnumStartPos=12
    runnumEndPos=17
    runnum=`echo $DatasetDir | cut -c $runnumStartPos-$runnumEndPos`
    matched=1
fi
if [ $matched = -1 ]; then dataType=`echo $DatasetDir | cut -c 1-12` ;fi
if [ $dataType = "mc15_13TeV" ]; then
    runnumStartPos=12
    runnumEndPos=17
    runnum=`echo $DatasetDir | cut -c $runnumStartPos-$runnumEndPos`
    matched=1
fi
if [ $matched = -1 ]; then dataType=`echo $DatasetDir | cut -c 1-12` ;fi
if [ $dataType = "mc15_14TeV" ]; then
    runnumStartPos=12
    runnumEndPos=17
    runnum=`echo $DatasetDir | cut -c $runnumStartPos-$runnumEndPos`
    matched=1
fi
if [ $matched = -1 ]; then dataType=`echo $DatasetDir | cut -c 1-12` ;fi
if [ $dataType = "data14_13TeV" ]; then
    runnumStartPos=13
    runnumEndPos=19
    runnum=`echo $DatasetDir | cut -c $runnumStartPos-$runnumEndPos`
    matched=1
fi
#echo $dataType
#echo $runnum

maxEve=10
echo Starting testRun for DSID=$runnum '('$DatasetDir')'...
echo '$testRun -n '$maxEve' --FileDirBase '$TARGETDS' -D '$DatasetDir' -o result/'$runnum'_00 '$TARGETSELECREG
time testRun -n $maxEve --FileDirBase $TARGETDS -D $DatasetDir -o result/${runnum}_00 $TARGETSELECREG 2>&1 | tee log/$runnum_00.log
echo ''
done
