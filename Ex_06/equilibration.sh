#!/bin/bash

# Check if the number of arguments is exactly one
if [ "$#" -ne 1 ]; then
  echo "Error: This script requires exactly 1 argument."
  echo "Usage: $0 <sim_type>"
  exit 1
fi

if [ "$1" -eq 2 ]; then
  rm -rf metropolis/equilibration/*.dat
elif [ "$1" -eq 3 ]; then
  rm -rf gibbs/equilibration/*.dat
fi

for t in 2 1.5 1 0.5
do
    rm -f ../NSL_SIMULATOR/INPUT/input.dat
    cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/ising/equilibration.ising ../NSL_SIMULATOR/INPUT/
    mv ../NSL_SIMULATOR/INPUT/equilibration.ising ../NSL_SIMULATOR/INPUT/input.dat

    sed -i "1s/.*/SIMULATION_TYPE        $1    1.0    0.0/" ../NSL_SIMULATOR/INPUT/input.dat
    sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/input.dat

    cd ../NSL_SIMULATOR/SOURCE
    ./simulator.exe
    cd -  # Change back to the original directory

    if [ "$1" -eq 2 ]; then
        cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es6/metropolis/equilibration/
        mv ../es6/metropolis/equilibration/total_energy.dat ../es6/metropolis/equilibration/total_energy_${t}.dat
    elif [ "$1" -eq 3 ]; then
        cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es6/gibbs/equilibration/
        mv ../es6/gibbs/equilibration/total_energy.dat ../es6/gibbs/equilibration/total_energy_${t}.dat
    fi
done