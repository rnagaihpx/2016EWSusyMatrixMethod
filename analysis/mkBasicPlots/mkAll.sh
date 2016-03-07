TAG=$1
if [ $# -lt 1 ]; then
  echo 'Specify a target tag (e.g. h0001, h0002...)'
  return 1
fi
DIRECTORY=${ROOTCOREBIN}/../result/
echo $DIRECTORY

###################################################
## Preparing available selection regions
###################################################
REGION=()
for fname in `ls -d $DIRECTORY/$TAG/110401_1/data*`
do
    REG=`echo $fname | cut -d'.' -f4`
    REGION+=($REG)
done

###################################################
## Merging files
###################################################
queue=30m
for reg in "${REGION[@]}"
do
#    echo root -q -b 'mkPlots.C("'${TAG}'","'${reg}'")'
#    root -q -b 'mkPlots.C("'${TAG}'","'${reg}'")'
    echo bsub -q ${queue} -e ./plots/${TAG}.${reg}.err -o ./plots/${TAG}.${reg}.log root -q -b 'mkPlots.C("'${TAG}'","'${reg}'")'
    bsub -q ${queue} -e ./plots/${TAG}.${reg}.err -o ./plots/${TAG}.${reg}.log root -q -b 'mkPlots.C("'${TAG}'","'${reg}'")'
done
