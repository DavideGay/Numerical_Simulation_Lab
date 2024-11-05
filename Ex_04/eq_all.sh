#!/bin/bash

# Check if exactly one argument is passed
if [ $# -ne 1 ]; then
  echo "Error: You must provide exactly one argument."
  echo "Usage: $0 [0|1|2]"
  exit 1
fi

# Check if the argument is an integer between 0 and 2
if [[ "$1" =~ ^[0-2]$ ]]; then
  case "$1" in
    0)
      rm -f ../es4/DATA/SOLID/equilibration/*.dat
        # Loop over the values of t
        for t in 1.5 1.6 1.55
        do
            # change temperature in input file
            sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/SOLID.dat
            
            source equilibrate.sh 0

            # move output files
            cp ../NSL_SIMULATOR/OUTPUT/temperature.dat ../es4/DATA/SOLID/equilibration/
            mv ../es4/DATA/SOLID/equilibration/temperature.dat ../es4/DATA/SOLID/equilibration/temperature_${t}.dat
        done
        ;;
    1)
      rm -f ../es4/DATA/LIQUID/equilibration/*.dat
        # Loop over the values of t
        for t in 1.95 2 1.98
        do
            # change temperature in input file
            sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/LIQUID.dat
            
            source equilibrate.sh 1

            # move output files
            cp ../NSL_SIMULATOR/OUTPUT/temperature.dat ../es4/DATA/LIQUID/equilibration/
            mv ../es4/DATA/LIQUID/equilibration/temperature.dat ../es4/DATA/LIQUID/equilibration/temperature_${t}.dat
        done
        ;;
    2)
      rm -f ../es4/DATA/GAS/equilibration/*.dat
        # Loop over the values of t
        for t in 0.9 1.1 1
        do
            # change temperature in input file
            sed -i "3s/.*/TEMP                   $t/" ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/GAS.dat
            
            source equilibrate.sh 2

            # move output files
            cp ../NSL_SIMULATOR/OUTPUT/temperature.dat ../es4/DATA/GAS/equilibration/
            mv ../es4/DATA/GAS/equilibration/temperature.dat ../es4/DATA/GAS/equilibration/temperature_${t}.dat
        done
        ;;
  esac
else
  echo "Error: Argument must be an integer between 0 and 2."
  exit 1
fi