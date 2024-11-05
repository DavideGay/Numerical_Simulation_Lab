#!/bin/bash

# Check if the number of arguments is exactly two
if [ "$#" -ne 2 ]; then
  echo "Error: This script requires exactly 2 arguments."
  echo "Usage: $0 <sim_type> <h>"
  exit 1
fi

# Check if the first argument is either 2 or 3
if [ "$1" -ne 2 ] && [ "$1" -ne 3 ]; then
  echo "Error: The first argument must be 2 (Metropolis) or 3 (Gibbs)."
  exit 1
fi

L=0.5,2.0
N=20

start=$(echo $L | awk -F ',' '{print $1}')
end=$(echo $L | awk -F ',' '{print $2}')
step=$(echo "scale=10; ($end - $start) / ($N - 1)" | bc)

t=$end

for ((i=0; i<N; i++)); do

    # EQUILIBRATION ///////////////////////////////////
    # replace input.dat with appropriate input file
    rm -f ../NSL_SIMULATOR/INPUT/input.dat
    cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/ising/equilibration.ising ../NSL_SIMULATOR/INPUT/
    mv ../NSL_SIMULATOR/INPUT/equilibration.ising ../NSL_SIMULATOR/INPUT/input.dat

    # modify input
    sed -i "1s/.*/SIMULATION_TYPE        $1    1.0    $2/" ../NSL_SIMULATOR/INPUT/input.dat
    sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/input.dat
    if (( i > 0 )); then
        sed -i "2s/.*/RESTART                1/" ../NSL_SIMULATOR/INPUT/input.dat

        rm -f ../NSL_SIMULATOR/INPUT/CONFIG/config.spin
        cp ../NSL_SIMULATOR/OUTPUT/CONFIG/config.spin ../NSL_SIMULATOR/INPUT/CONFIG/
    fi

    cd ../NSL_SIMULATOR/SOURCE
    ./simulator.exe  # Execute simulator.exe
    cd -  # Change back to the original directory

    # MEASURES ////////////////////////////////////////
    rm -f ../NSL_SIMULATOR/INPUT/input.dat
    cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/ising/input.ising ../NSL_SIMULATOR/INPUT/
    mv ../NSL_SIMULATOR/INPUT/input.ising ../NSL_SIMULATOR/INPUT/input.dat

    # set h=0.02
    sed -i "1s/.*/SIMULATION_TYPE        $1    1.0    $2/" ../NSL_SIMULATOR/INPUT/input.dat
    sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/input.dat
    sed -i "2s/.*/RESTART                1/" ../NSL_SIMULATOR/INPUT/input.dat
    rm -f ../NSL_SIMULATOR/INPUT/CONFIG/config.spin
    cp ../NSL_SIMULATOR/OUTPUT/CONFIG/config.spin ../NSL_SIMULATOR/INPUT/CONFIG/

    cd ../NSL_SIMULATOR/SOURCE
    ./simulator.exe  # Execute simulator.exe
    cd -  # Change back to the original directory

    t=$(echo "scale=10; $t - $step" | bc)  # Update t to decrement
done
