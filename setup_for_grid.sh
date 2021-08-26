#!/bin/bash

setup_for_grid(){

    # the default mode is NuMI
    export MODE="NUMI"

    # set package to be used
    source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh
    setup login
    setup jobsub_client
    #source /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setup
    source /cvmfs/larsoft.opensciencegrid.org/products/setup
    setup root v6_18_04 -q e17:prof
    setup geant4 v4_10_6_p01 -q e19:prof
    setup cmake v3_9_5
    #GRID
    #source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh
    #setup jobsub_client
    #setup ifdhc #v2_2_3
    #export IFDH_GRIDFTP_EXTRA="-st 10" #set ifdh cp stall timeout to 10 sec
    #export IFDH_CP_MAXRETRIES=2

    # gets the full path to the location of setup.sh
    export PPFX_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
    echo "setting PPFX_DIR=${PPFX_DIR}"

    export LD_LIBRARY_PATH=$PPFX_DIR/lib:$LD_LIBRARY_PATH
    echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
}
HOST=$(hostname -f)
echo $HOST
if echo "$HOST" | grep 'dune';then
    echo "This is not a dune machine. Try setup_for_dune.sh <MODE>"
else 
    echo "executing for the $HOST"
    setup_for_grid
fi

