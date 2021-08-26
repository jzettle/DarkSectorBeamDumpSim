#!/bin/bash

AREANUM=$1
NUMFILES=$2
FILECOUNTER=0
LOCATION=/pnfs/ldrd/darksectorldrd/persistent/users/jzettle/darksectorsim/test${AREANUM}/
OUTPUTNAME=$3

while [ $FILECOUNTER -lt $NUMFILES ]; do
    echo "On next file!"
    cat ${LOCATION}/beamfiletest_${FILECOUNTER}.txt >> ${OUTPUTNAME}.txt
    let FILECOUNTER=FILECOUNTER+1
done
