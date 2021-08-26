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
TEXT_FILE="beamfiletest_$PROCESS.txt"
echo "Creating $MACROFILE"
#echo "BEAMCONFIG=${BEAMCONFIG}"
echo "PROCESS=$PROCESS"
echo "OUTFILE=$OUTPUT_FILE"

echo
echo "======== BUILDING macro file ========="
echo "/run/verbose 1 " >> $MACROFILE
echo "/inputmodel/inputGDML ../gdml/simple_target_detector/Target.gdml " >> $MACROFILE 
echo "/analysis/setOutputFileName ../${OUTPUT_FILE} " >> $MACROFILE
echo "/analysis/useVoxel false" >> $MACROFILE
echo "/analysis/saveBeamPion true" >> $MACROFILE
echo "/analysis/setBeamPionFile ../${TEXT_FILE}" >> $MACROFILE
#echo "/run/initialize" >> $MACROFILE
#echo "/generator/setGenerator gps" >> $MACROFILE
#echo "/generator/setGenerator VoxelPhoton" >> $MACROFILE
#echo "/generator/voxelNumR 0" >> $MACROFILE
#echo "/generator/voxelNumZ -100" >> $MACROFILE
echo "/run/initialize" >> $MACROFILE
echo "/generator/setGenerator gps" >> $MACROFILE
echo "/gps/particle proton" >> $MACROFILE
echo "/gps/ene/type Mono" >> $MACROFILE
echo "/gps/ene/mono 800.0 MeV" >> $MACROFILE
#echo "/gps/ang/type iso" >> $MACROFILE
echo "/gps/pos/type Beam" >> $MACROFILE
echo "/gps/pos/centre -3.601 0. 0. m" >> $MACROFILE
echo "/gps/pos/halfx 1.0 cm" >> $MACROFILE
echo "/gps/pos/halfy 1.0 cm" >> $MACROFILE
echo "/gps/pos/sigma_x 4.5 cm" >> $MACROFILE
echo "/gps/pos/sigma_y 4.5 cm" >> $MACROFILE
echo "/gps/pos/rot1 0. 0. 1." >> $MACROFILE
echo "/gps/direction 1. 0. 0." >> $MACROFILE
echo "/run/beamOn 5000" >> $MACROFILE

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
mv ../$TEXT_FILE $CONDOR_DIR_PPFX
#mv ../TestJobSub.root /pnfs/icarus/scratch/users/jzettle/
