#!/bin/bash

# Check if the number of arguments is exactly one
if [ "$#" -ne 1 ]; then
  echo "Error: This script requires exactly 1 argument."
  echo "Usage: $0 <sim_type>"
  exit 1
fi

# clean previous data from simulator outputs
rm -f ../NSL_SIMULATOR/OUTPUT/temp_tenergy.dat
rm -f ../NSL_SIMULATOR/OUTPUT/temp_specific_heat.dat
rm -f ../NSL_SIMULATOR/OUTPUT/temp_susceptibility.dat
rm -f ../NSL_SIMULATOR/OUTPUT/temp_magnetization.dat

# perform measurements
source measures.sh $1 0

# export data
if [ "$1" -eq 2 ]; then
  rm -f metropolis/*.dat
  cp ../NSL_SIMULATOR/OUTPUT/temp_tenergy.dat ../es6/metropolis/
  cp ../NSL_SIMULATOR/OUTPUT/temp_specific_heat.dat ../es6/metropolis/
  cp ../NSL_SIMULATOR/OUTPUT/temp_susceptibility.dat ../es6/metropolis/
elif [ "$1" -eq 3 ]; then
  rm -f gibbs/*.dat
  cp ../NSL_SIMULATOR/OUTPUT/temp_tenergy.dat ../es6/gibbs/
  cp ../NSL_SIMULATOR/OUTPUT/temp_specific_heat.dat ../es6/gibbs/
  cp ../NSL_SIMULATOR/OUTPUT/temp_susceptibility.dat ../es6/gibbs/
fi

# MAGNETIZATION
# clean previous data from simulator outputs
rm -f ../NSL_SIMULATOR/OUTPUT/temp_tenergy.dat
rm -f ../NSL_SIMULATOR/OUTPUT/temp_specific_heat.dat
rm -f ../NSL_SIMULATOR/OUTPUT/temp_susceptibility.dat
rm -f ../NSL_SIMULATOR/OUTPUT/temp_magnetization.dat

source measures.sh $1 0.02

# export magnetization data
if [ "$1" -eq 2 ]; then
  cp ../NSL_SIMULATOR/OUTPUT/temp_magnetization.dat ../es6/metropolis/
elif [ "$1" -eq 3 ]; then
  cp ../NSL_SIMULATOR/OUTPUT/temp_magnetization.dat ../es6/gibbs/
fi