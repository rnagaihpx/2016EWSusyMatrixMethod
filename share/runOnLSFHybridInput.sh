#For the people not using /bin/bash
if [ $0 != 'bash' ]; then
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
# Finding new output directory (result/hXXX/)
###########################################################
maxTagNum=0
for OUTDIR in `\ls result | grep h`
do
    if [ ${#OUTDIR} -ne 5 ]; then continue; fi
    if [ `echo $OUTDIR | cut -c 1` = 'h' ]; then
        num=`echo $OUTDIR | cut -c 2-5`
        expr "$num" + 1 >/dev/null 2>&1
        if [ $? -lt 2 ]; then #checking if $num is numeric or not
            if [ $num -gt $maxTagNum ]; then
                maxTagNum=`expr $num`
            fi
        fi
    fi
done
tagNum4ThisTime=`expr $maxTagNum + 1`
tagNum=`printf "%04d" $tagNum4ThisTime` #zero padding
echo h$tagNum
\mkdir lsfoutput/h${tagNum}
\mkdir result/h${tagNum}

###########################################################
# Defining function to submit jobs
###########################################################
submit(){
lenFilelistDirName=${#1}
#extension check (should be .txt)
extensionStartPos=`expr $lenFilelistDirName - 3`
extensionEndPos=`expr $lenFilelistDirName`
extension=`echo ${1} | cut -c $extensionStartPos-$extensionEndPos`
if [ "$extension" = ".txt" ]; then
#    echo "matched"
    :
else
#    echo "unmatched"
    continue
fi
#extracting run number
if [ $lenFilelistDirName -eq 10 ]; then
    startPos=`expr $lenFilelistDirName - 9`
    runnumEndPos=`expr $startPos + 5`
    outputDirEndPos=`expr $lenFilelistDirName - 4`
elif [ $lenFilelistDirName -gt 10 ]; then
    startPos=1
    runnumEndPos=`expr $startPos + 5`
    outputDirEndPos=`expr $lenFilelistDirName - 4`
fi
outputDir=`echo ${1} | cut -c $startPos-$outputDirEndPos`
runnum=`echo ${1} | cut -c $startPos-$runnumEndPos`
#echo ${1}
######################################################

######################################################
# Waiting for #submitted jobs to be less than maxJobs
maxJobs=800
while [ $(bjobs | wc -l) -gt $maxJobs  ]
do
    echo Currently $(bjobs | wc -l) jobs are runnning. Wait for 10 seconds to keep less running jobs...
    sleep 10
done
######################################################

if [ "${2}" = "Local" ]; then
    FAXorNot=""
    FilePath="Local"
else
    FAXorNot="--useFAX"
    FilePath="FAXonTOKYO-LCG2"
fi

maxEve=-1
queue=1d
echo Starting testRun for DSID=$runnum ...
echo bsub -q ${queue} -o ./lsfoutput/h${tagNum}/${outputDir}.log testRun -n $maxEve --FileDirBase $TARGETDS/${FilePath} --filelist ${1} -o result/h${tagNum}/$outputDir ${FAXorNot} $TARGETSELECREG
bsub -q ${queue} -o ./lsfoutput/h${tagNum}/${outputDir}.log testRun -n $maxEve --FileDirBase $TARGETDS/${FilePath} --filelist ${1} -o result/h${tagNum}/$outputDir ${FAXorNot} $TARGETSELECREG
echo ''
}

###########################################################
# Submitting jobs to the dataset on GPFS
###########################################################
for TXT in `\ls $TARGETDS/Local`
do
submit $TXT Local
done
###########################################################
# Submitting jobs to the dataset on TOKYO-LCG2 via FAX
###########################################################
for TXT in `\ls $TARGETDS/FAXonTOKYO-LCG2`
do
submit $TXT FAXonTOKYO-LCG2
done

echo =================================================
echo Tag=h${tagNum} was used for this submission. 
echo Output files will appear in result/h${tagNum}.
echo Log files will appear in lsfoutput/h${tagNum}.
echo =================================================
