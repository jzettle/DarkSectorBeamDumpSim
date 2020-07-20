# DarkSectorBeamDumpSim

J. Zettlemoyer, FNAL, jzettle@fnal.gov

This simulation has been tested and verified to run against GEANT4 10.6 with cmake version 3.9.5

To build and compile, with the DarkSectorBeamDumpSim directory

rm -rf build/ (If build directory already exists)

mkdir build

cd build

cmake ../

make

Should compile and build the simulation with an executable called darksectorsim, to run use

./darksectorsim -m </path/to/macro.mac> 

where -m is a input to provide a macro file to run GEANT4
