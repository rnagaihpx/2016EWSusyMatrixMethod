TAG=$1
if [ $# -lt 1 ]; then
  echo 'Specify a target tag (e.g. h0001, h0002...)'
  return 1
fi
DIRECTORY='result'
DSID=("410000")
#DSID=("110070" "110071" "110302" "110305" #single top
#    "110401" #ttbar
#    "119353" "119355" "119583" #ttV
#    "147400" "147401" "147402" "147403" "147404" "147405" #W+jets
#    "147406" "147407" "147408" #Z+jets
#    "161105" "161155" "161305"
#    "161555" "161566" "161577" "161595" "161606" "161617"
#    "161635" "161646" "161657" "161675" "161686" "161697" "161805" "161827" "161849" #Higgs
#    "174830" "174831" "174832" "174833" #ttV
#    "187150" "187151" "187152" "187153" "187154"
#    "187155" "187156" "187157" "187158" #WW
#    "187160" "187161" "187162" "187163" "187164" "187165"
#    "187166" "187167" "187168" "187170" "187171" "187172"
#    "187173" "187174" "187175" "187176" "187177" "187178" #WZ
#    "187180" "187181" "187182" "187183" "187184" "187185" "187186" "187187" "187188" #ZZ
#    "187401" "187402" "187403" "187404" "187405" "187406" "187407" "187408" "187409" #gg->WW
#    "205048" "205049" "205050" "205051" "205052" "205053"
#    "205054" "205055" "205056" "205128" "205129" "205130" "205131" #Signal(N2C1toWZ)
#)

###################################################
## Preparing available selection regions
###################################################
REGION=()
for fname in `ls -d $DIRECTORY/$TAG/${DSID}_1/data*`
do
    REG=`echo $fname | cut -d'.' -f2`
    REGION+=($REG)
done

###################################################
## Merging files
###################################################
queue=4h
for i in "${DSID[@]}"
do
    for j in "${REGION[@]}"
    do
        echo 'Merging data files of '$i' : SelecReg='$j' for the tag of '$TAG
#        echo time hadd -f ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.root ${DIRECTORY}/${TAG}/${i}_*/data-*.${j}.*/*.root
#        time hadd -f ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.root ${DIRECTORY}/${TAG}/${i}_*/data-*.${j}.*/*.root
        echo bsub -q ${queue} -e ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.err -o ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.log hadd -f ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.root ${DIRECTORY}/${TAG}/${i}_*/data-*.${j}.*/*.root
        bsub -q ${queue} -e ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.err -o ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.log hadd -f ${DIRECTORY}/${TAG}/${i}.${j}.AnaHists.root ${DIRECTORY}/${TAG}/${i}_*/data-*.${j}.*/*.root
    done
done
