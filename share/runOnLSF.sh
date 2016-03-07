#For the people not using /bin/bash
if [ $SHELL != '/bin/bash' ]; then
    echo 'Please change your shell to /bin/bash to use this script!!'
    return
fi

#default target directory
TARGETDS=$1
if [ $# -lt 1 ]; then
  echo '============================================================================'
  echo 'This script execute "testRun" against all the dataset in [Dataset path].'
  echo 'Works only on the nodes'
  echo 'Usage :'
  echo '$source brunall.sh [Dataset path] [selction region1] [selection region2],,,'
  echo '============================================================================'
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

###########################################################
# Deleting old logfiles in ./lsfoutput
###########################################################
\rm -f lsfoutput/*.log~* #delete logfiles with "~" first
for logfile in `\ls ./lsfoutput`
do
    \cp -p --force lsfoutput/$logfile lsfoutput/$logfile~
    \rm -f lsfoutput/$logfile
done

###########################################################
# Submitting jobs to the dataset in the target directory 
###########################################################
for DatasetDir in `\ls $TARGETDS`
do
######################################################
# Finding "data type" and "run number"
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
#echo $DatasetDir
#echo $dataType
#echo $runnum
######################################################

######################################################
# Waiting for #submitted jobs to be less than maxJobs
maxJobs=150
while [ $(bjobs | wc -l) -gt $maxJobs  ]
do
    echo Currently $(bjobs | wc -l) jobs are runnning. Wait for 10 seconds to keep less running jobs...
    sleep 10
done
######################################################

maxEve=-1
echo Starting testRun for DSID=$runnum '('$DatasetDir')'...
echo 'bsub -q 1d -o ./lsfoutput/'$runnum'_00.log -e ./lsfoutput/'$runnum'_error_00.log testRun -n '$maxEve' --FileDirBase '$TARGETDS' -D '$DatasetDir' -o result/'$runnum'_00 '$TARGETSELECREG
bsub -q 1d -o ./lsfoutput/${runnum}_00.log -e ./lsfoutput/${runnum}_error_00.log testRun -n $maxEve --FileDirBase $TARGETDS -D $DatasetDir -o result/${runnum}_00 $TARGETSELECREG
echo ''
done
