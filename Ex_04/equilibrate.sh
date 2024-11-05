#!/bin/bash

# Check if exactly one argument is passed
if [ $# -ne 1 ]; then
  echo "Error: You must provide exactly one argument."
  echo "Usage: $0 [0|1|2]"
  exit 1
fi

# Check if the argument is an integer between 0 and 2
if [[ "$1" =~ ^[0-2]$ ]]; then
  # replace input.dat with appropriate input file
  rm -f ../NSL_SIMULATOR/INPUT/input.dat
  case "$1" in
    0)
      echo "Performing solid phase equilibration."
      cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/SOLID.dat ../NSL_SIMULATOR/INPUT/
      mv ../NSL_SIMULATOR/INPUT/SOLID.dat ../NSL_SIMULATOR/INPUT/input.dat
      ;;
    1)
      echo "Performing liquid phase equilibration."
      cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/LIQUID.dat ../NSL_SIMULATOR/INPUT/
      mv ../NSL_SIMULATOR/INPUT/LIQUID.dat ../NSL_SIMULATOR/INPUT/input.dat
      ;;
    2)
      echo "Performing gas phase equilibration."
      cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/GAS.dat ../NSL_SIMULATOR/INPUT/
      mv ../NSL_SIMULATOR/INPUT/GAS.dat ../NSL_SIMULATOR/INPUT/input.dat
      ;;
  esac
  # replace config.xyz with config.fcc
  rm -f ../NSL_SIMULATOR/INPUT/CONFIG/config.xyz
  rm -f ../NSL_SIMULATOR/INPUT/CONFIG/velocities.in
  cp ../NSL_SIMULATOR/INPUT/CONFIG/STARTING_CONFIG/config.fcc ../NSL_SIMULATOR/INPUT/CONFIG/
  mv ../NSL_SIMULATOR/INPUT/CONFIG/config.fcc ../NSL_SIMULATOR/INPUT/CONFIG/config.xyz

  cd ../NSL_SIMULATOR/SOURCE
  ./simulator.exe  # Execute simulator.exe
  cd -  # Change back to the original directory

  # replace config.xyz with equilibrated config, move and rename velocities.out
  rm -f ../NSL_SIMULATOR/INPUT/CONFIG/config.xyz
  cp ../NSL_SIMULATOR/OUTPUT/CONFIG/config.xyz ../NSL_SIMULATOR/INPUT/CONFIG/
  cp ../NSL_SIMULATOR/OUTPUT/CONFIG/velocities.out ../NSL_SIMULATOR/INPUT/CONFIG/
  mv ../NSL_SIMULATOR/INPUT/CONFIG/velocities.out ../NSL_SIMULATOR/INPUT/CONFIG/velocities.in
else
  echo "Error: Argument must be an integer between 0 and 2."
  exit 1
fi