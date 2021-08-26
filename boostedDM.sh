#!/bin/bash

echo
echo "======== cd to CONDOR_DIR_INPUT ========"
cd $CONDOR_DIR_INPUT
pwd

echo
echo "======== ls ========"
ls

echo
echo "======== UNTARRING... ========"
tar xvfz darksectorsim.tar.gz -C ./ > /dev/null

echo
echo "======== Done untarring. ls ========"
ls

echo
echo "======== SETUP ROOT, GEANT4 and CMAKE ========"
echo "source setup_for_grid.sh"
source setup_for_grid.sh

echo
echo "======== UPDATE to select macro to select input ========"
#INPUT_FILE="${IN_DIR}"
#ifdh cp "${INPUT_FILE}" "$CONDOR_DIR_INPUT/"
#ls

OUTPUT_FILE="darksectorsim_run_$PROCESS.root"
MACROFILE=run_grid_$PROCESS.mac 
echo "Creating $MACROFILE"
#echo "BEAMCONFIG=${BEAMCONFIG}"
echo "PROCESS=$PROCESS"
echo "OUTFILE=$OUTPUT_FILE"

echo
echo "======== BUILDING macro file ========="
echo "/run/verbose 1 " >> $MACROFILE
echo "/inputmodel/inputGDML ../gdml/simple_target_detector/simple.gdml " >> $MACROFILE 
echo "/analysis/setOutputFileName ../${OUTPUT_FILE} " >> $MACROFILE
echo "/analysis/useVoxel true" >> $MACROFILE
echo "/generator/setGenerator BoostedDM" >> $MACROFILE
echo "/run/initialize" >> $MACROFILE
echo "/run/beamOn 150" >> $MACROFILE

ls *.mac

echo
echo "======== EXECUTING simulation ========"
cd build
./darksectorsim -m ../$MACROFILE
#echo "bin/doReweight_dk2nu_original ${NAME_FILE} ${OUTPUT_FILE} ${INPUT_OPTIONS} ${IDET}"
#bin/doReweight_dk2nu_original "${NAME_FILE}" "${OUTPUT_FILE}" "${INPUT_OPTIONS}" "${IDET}"

echo
echo "Moving output to CONDOR_DIR_PPFX: "
echo "=> CONDOR_DIR_PPFX: $CONDOR_DIR_PPFX"
ls ../

#rm g4numi*root
mv ../$OUTPUT_FILE $CONDOR_DIR_PPFX
#mv ../TestJobSub.root /pnfs/icarus/scratch/users/jzettle/
