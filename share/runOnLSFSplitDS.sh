#For the people not using /bin/bash
if [ ! `echo $0 | grep 'bash'` ]; then
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
enRunMM=''
enEffFile=''
enMeasureEff=''
enEleIdBaseline=''
enIsoWP=''
while [ "$2" != "" ]
do
    if [ "$2" = "--runMM" ]; then
        echo 'Enabled --runMM'
        enRunMM='true'
    elif [ "$2" = "--effFile" ]; then
        enEffFile='--effFile '${3}
        echo 'Enabled --effFile:' ${enEffFile}
        shift
    elif [ "$2" = "--measureEff" ]; then
        echo 'Enabled --measureEff'
        enMeasureEff='--measureEff'
    elif [ "$2" = "--eleIdBaseline" ]; then
        enEleIdBaseline='--eleIdBaseline '${3}
        echo 'Enabled --eleIdBaseline:' ${enEleIdBaseline}
        shift
    elif [ "$2" = "--isoWP" ]; then
        enIsoWP='--isoWP '${3}
        echo 'Enabled --isoWP:' ${enIsoWP}
        shift
    else
        TARGETSELECREG='-S '$2' '$TARGETSELECREG
    fi
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
\mkdir lsfoutput/h${tagNum}
\mkdir result/h${tagNum}

###########################################################
# Submitting jobs to the dataset in the target directory 
###########################################################
for TXT in `\ls $TARGETDS`
do
lenFilelistDirName=${#TXT}
#extension check (should be .txt)
extensionStartPos=`expr $lenFilelistDirName - 3`
extensionEndPos=`expr $lenFilelistDirName`
extension=`echo $TXT | cut -c $extensionStartPos-$extensionEndPos`
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
outputDir=`echo $TXT | cut -c $startPos-$outputDirEndPos`
runnum=`echo $TXT | cut -c $startPos-$runnumEndPos`
#echo $TXT
######################################################

#######################################################
## Waiting for #submitted jobs to be less than maxJobs
#maxJobs=800
#while [ $(bjobs | wc -l) -gt $maxJobs  ]
#do
#    echo Currently $(bjobs | wc -l) jobs are runnning. Wait for 10 seconds to keep less running jobs...
#    sleep 10
#done
#######################################################

###################################################################
# Finding MC samples with fake leptons for MM
###################################################################
FakeDSID=("110070" "110071" "110302" "110305" #single top
    "110401" #ttbar
    "147400" "147401" "147402" "147403" "147404" "147405" #W+jets
    "147406" "147407" "147408" #Z+jets
    "161555" "161566" "161577" "161595" "161606" "161617"
    "161646" "161657" "161697" "161827" "161849" #Higgs
    "187150" "187151" "187152" "187153" "187154"
    "187155" "187156" "187157" "187158" #WW
    "187401" "187402" "187403" "187404" "187405" "187406" "187407" "187408" "187409" #gg->WW
    "410000"
)

runMM=''
if [ "$enRunMM" = 'true' ]; then
    for fakeDS in "${FakeDSID[@]}"
    do
        if [ "$runnum" = "$fakeDS" ]; then
            echo 'MC samples for fake leptons was found!!'
            runMM='--runMM'
        fi
    done
fi

###################################################################
# Submitting jobs to LSF
###################################################################
maxEve=-1
queue=1d
echo Starting testRun for DSID=$runnum ...
echo bsub -q ${queue} -J h${tagNum} -e ./lsfoutput/h${tagNum}/${outputDir}_error.log -o ./lsfoutput/h${tagNum}/${outputDir}.log testRun -n $maxEve --FileDirBase $TARGETDS --filelist $TXT -o result/h${tagNum}/$outputDir $TARGETSELECREG ${runMM} ${enMeasureEff} ${enEffFile} ${enEleIdBaseline} ${enIsoWP}
bsub -q ${queue} -J h${tagNum} -e ./lsfoutput/h${tagNum}/${outputDir}_error.log -o ./lsfoutput/h${tagNum}/${outputDir}.log testRun -n $maxEve --FileDirBase $TARGETDS --filelist $TXT -o result/h${tagNum}/$outputDir $TARGETSELECREG ${runMM} ${enMeasureEff}${enEffFile} ${enEleIdBaseline} ${enIsoWP}
echo ''
done

echo =================================================
echo Tag=h${tagNum} was used for this submission. 
echo Output files will appear in result/h${tagNum}.
echo Log files will appear in lsfoutput/h${tagNum}.
echo =================================================
