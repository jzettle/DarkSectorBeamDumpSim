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
TEXT_FILE_ETA="beamfilerun_etagen_$PROCESS.txt"
TEXT_FILE_PIZERO="beamfilerun_pizerogen_$PROCESS.txt"
TEXT_FILE_PIPM="beamfilerun_piplusminusgen_$PROCESS.txt"
TEXT_FILE_KAON="beamfilerun_kaongen_$PROCESS.txt"
TEXT_FILE_EL="beamfilerun_e+e-_gen_$PROCESS.txt"
TEXT_FILE_GAMMA="beamfilerun_gamma_gen_$PROCESS.txt"
TEXT_FILE_BT="beamfilerun_beamtarget_gen_$PROCESS.txt"
TEXT_FILE_EX="beamfilerun_exgamma_$PROCESS.txt"

echo "Creating $MACROFILE"
#echo "BEAMCONFIG=${BEAMCONFIG}"
echo "PROCESS=$PROCESS"
echo "OUTFILE=$OUTPUT_FILE"

echo
echo "======== BUILDING macro file ========="
echo "/run/verbose 1 " >> $MACROFILE
echo "/inputmodel/inputGDML ../gdml/simple_target_detector/TargetShield.gdml " >> $MACROFILE 
echo "/analysis/setOutputFileName ../${OUTPUT_FILE} " >> $MACROFILE
echo "/analysis/useVoxel false" >> $MACROFILE
echo "/analysis/saveBeamPion false" >> $MACROFILE
echo "/analysis/setBeamPiZeroFile ../${TEXT_FILE_PIZERO}" >> $MACROFILE
echo "/analysis/setBeamPiPlusMinusFile ../${TEXT_FILE_PIPM}" >> $MACROFILE
echo "/analysis/setBeamKaonFile ../${TEXT_FILE_KAON}" >> $MACROFILE
echo "/analysis/setBeamEtaFile ../${TEXT_FILE_ETA}" >> $MACROFILE
echo "/analysis/saveALPProd false" >> $MACROFILE
echo "/analysis/setALPProdElFile ../${TEXT_FILE_EL}" >> $MACROFILE
echo "/analysis/setALPProdGammaFile ../${TEXT_FILE_GAMMA}" >> $MACROFILE
echo "/analysis/saveBeamTarget true" >> $MACROFILE
echo "/analysis/setBeamTargetFile ../${TEXT_FILE_BT}" >> $MACROFILE
echo "/analysis/setExGammaFile ../${TEXT_FILE_EX}" >> $MACROFILE
#echo "/run/initialize" >> $MACROFILE
#echo "/generator/setGenerator gps" >> $MACROFILE
#echo "/generator/setGenerator VoxelPhoton" >> $MACROFILE
#echo "/generator/voxelNumR 0" >> $MACROFILE
#echo "/generator/voxelNumZ -100" >> $MACROFILE
echo "/run/initialize" >> $MACROFILE
echo "/generator/setGenerator gps" >> $MACROFILE
echo "/gps/particle proton" >> $MACROFILE
echo "/gps/ene/type Mono" >> $MACROFILE
echo "/gps/ene/mono 2000 MeV" >> $MACROFILE
#echo "/gps/ang/type iso" >> $MACROFILE
echo "/gps/pos/type Beam" >> $MACROFILE
echo "/gps/pos/centre -0.001 0. 0. m" >> $MACROFILE
echo "/gps/pos/halfx 1.0 cm" >> $MACROFILE
echo "/gps/pos/halfy 1.0 cm" >> $MACROFILE
echo "/gps/pos/sigma_x 5.0 cm" >> $MACROFILE
echo "/gps/pos/sigma_y 5.0 cm" >> $MACROFILE
echo "/gps/pos/rot1 0. 0. 1." >> $MACROFILE
echo "/gps/direction 1. 0. 0." >> $MACROFILE
echo "/run/beamOn 10" >> $MACROFILE

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
mv ../$TEXT_FILE_PIZERO $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_PIPM $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_ETA $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_EL $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_GAMMA $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_BT $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_EX $CONDOR_DIR_PPFX
mv ../$TEXT_FILE_KAON $CONDOR_DIR_PPFX
#mv ../TestJobSub.root /pnfs/icarus/scratch/users/jzettle/
