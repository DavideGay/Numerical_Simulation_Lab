#!/bin/bash

# Check if the number of arguments is exactly two
if [ "$#" -ne 2 ]; then
  echo "Error: This script requires exactly 2 arguments."
  echo "Usage: $0 <sim_type> <N>"
  exit 1
fi

# Check if the first argument is either 2 or 3
if [ "$1" -ne 2 ] && [ "$1" -ne 3 ]; then
  echo "Error: The first argument must be 2 (Metropolis) or 3 (Gibbs)."
  exit 1
fi

if [ "$1" -eq 2 ]; then
  rm -f metropolis/equilibration/total_energy_restart_$2.dat
elif [ "$1" -eq 3 ]; then
  rm -f gibbs/equilibration/total_energy_restart_$2.dat
fi

L=0.5,2.0

start=$(echo $L | awk -F ',' '{print $1}')
end=$(echo $L | awk -F ',' '{print $2}')
step=$(echo "scale=10; ($end - $start) / ($2 - 1)" | bc)

t=$end

rm -f ../NSL_SIMULATOR/INPUT/input.dat
cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/ising/equilibration.ising ../NSL_SIMULATOR/INPUT/
mv ../NSL_SIMULATOR/INPUT/equilibration.ising ../NSL_SIMULATOR/INPUT/input.dat

sed -i "1s/.*/SIMULATION_TYPE        $1    1.0    0.0/" ../NSL_SIMULATOR/INPUT/input.dat

sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/input.dat

for ((i=0; i<$2-1; i++)); do

    cd ../NSL_SIMULATOR/SOURCE
    ./simulator.exe
    cd -  # Change back to the original directory

    t=$(echo "scale=10; $t - $step" | bc)  # Update t to decrement

    sed -i "2s/.*/RESTART                1/" ../NSL_SIMULATOR/INPUT/input.dat
    sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/input.dat

    rm -f ../NSL_SIMULATOR/INPUT/CONFIG/config.spin
    cp ../NSL_SIMULATOR/OUTPUT/CONFIG/config.spin ../NSL_SIMULATOR/INPUT/CONFIG/
done

cd ../NSL_SIMULATOR/SOURCE
./simulator.exe
cd -  # Change back to the original directory

if [ "$1" -eq 2 ]; then
    cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es6/metropolis/equilibration/
    mv ../es6/metropolis/equilibration/total_energy.dat ../es6/metropolis/equilibration/total_energy_restart_$2.dat
elif [ "$1" -eq 3 ]; then
    cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es6/gibbs/equilibration/
    mv ../es6/gibbs/equilibration/total_energy.dat ../es6/gibbs/equilibration/total_energy_restart_$2.dat
fi