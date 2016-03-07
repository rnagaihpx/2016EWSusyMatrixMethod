INPUTLIST=${1}
OUTTAG=${2}
echo ==================================
echo Submitting jobs to GRID...
echo ==================================
echo 'Input filelist:' $INPUTLIST
echo 'Output tag    :' $OUTTAG
echo ''
# The number of selection regions for each submission should be less than 10. (Grid output limitation)
SELEC1='-S 2S3B -S 2S3BBveto -S 2S3BZveto -S 2S3BMet -S 2S3BZvetoBvetoMet -S 3S3B -S 3S3BBveto -S 3S3BZveto'
SELEC2='-S 3S3BMet -S 3S3BZvetoBvetoMet -S 3S4B -S 3S4BBveto -S 3S4BZveto -S 3S4BMet -S 3S4BZvetoBvetoMet'
#Making commands
cat ${INPUTLIST} |
while read DS
do 
    echo === Submitting jobs against...
    echo $DS
    echo testRun -o ${OUTTAG}_00 --useGRID -D ${DS} ${SELEC1}
    time testRun -o ${OUTTAG}_00 --useGRID -D ${DS} ${SELEC1}
    echo testRun -o ${OUTTAG}_01 --useGRID -D ${DS} ${SELEC2}
    time testRun -o ${OUTTAG}_01 --useGRID -D ${DS} ${SELEC2}
    echo ''
done
