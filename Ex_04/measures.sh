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
        # equilibration
        cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/SOLID.dat ../NSL_SIMULATOR/INPUT/
        mv ../NSL_SIMULATOR/INPUT/SOLID.dat ../NSL_SIMULATOR/INPUT/input.dat
        sed -i "3s/.*/TEMP                   1.55/" ../NSL_SIMULATOR/INPUT/input.dat
        source equilibrate.sh 0
        # measures
        rm -f ../es4/DATA/SOLID/*.dat
        rm -f ../NSL_SIMULATOR/INPUT/input.dat
        cp ../es4/DATA/SOLID/input.in ../NSL_SIMULATOR/INPUT/
        mv ../NSL_SIMULATOR/INPUT/input.in ../NSL_SIMULATOR/INPUT/input.dat
        cd ../NSL_SIMULATOR/SOURCE
        echo "Computing solid phase properties."
        ./simulator.exe  # Execute simulator.exe
        cd -  # Change back to the original directory
        # copy output
        cp ../NSL_SIMULATOR/OUTPUT/temperature.dat ../es4/DATA/SOLID/
        cp ../NSL_SIMULATOR/OUTPUT/potential_energy.dat ../es4/DATA/SOLID/
        cp ../NSL_SIMULATOR/OUTPUT/pressure.dat ../es4/DATA/SOLID/
        cp ../NSL_SIMULATOR/OUTPUT/kinetic_energy.dat ../es4/DATA/SOLID/
        cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es4/DATA/SOLID/
      ;;
    1)
        # equilibration
        cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/LIQUID.dat ../NSL_SIMULATOR/INPUT/
        mv ../NSL_SIMULATOR/INPUT/LIQUID.dat ../NSL_SIMULATOR/INPUT/input.dat
        sed -i "3s/.*/TEMP                   1.98/" ../NSL_SIMULATOR/INPUT/input.dat
        source equilibrate.sh 1
        # measures
        rm -f ../es4/DATA/LIQUID/*.dat
        rm -f ../NSL_SIMULATOR/INPUT/input.dat
        cp ../es4/DATA/LIQUID/input.in ../NSL_SIMULATOR/INPUT/
        mv ../NSL_SIMULATOR/INPUT/input.in ../NSL_SIMULATOR/INPUT/input.dat
        cd ../NSL_SIMULATOR/SOURCE
        echo "Computing liquid phase properties."
        ./simulator.exe  # Execute simulator.exe
        cd -  # Change back to the original directory
        # copy output
        cp ../NSL_SIMULATOR/OUTPUT/temperature.dat ../es4/DATA/LIQUID/
        cp ../NSL_SIMULATOR/OUTPUT/potential_energy.dat ../es4/DATA/LIQUID/
        cp ../NSL_SIMULATOR/OUTPUT/pressure.dat ../es4/DATA/LIQUID/
        cp ../NSL_SIMULATOR/OUTPUT/kinetic_energy.dat ../es4/DATA/LIQUID/
        cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es4/DATA/LIQUID/
      ;;
    2)
        # equilibration
        cp ../NSL_SIMULATOR/INPUT/INPUT_EXAMPLES/equilibration/GAS.dat ../NSL_SIMULATOR/INPUT/
        mv ../NSL_SIMULATOR/INPUT/GAS.dat ../NSL_SIMULATOR/INPUT/input.dat
        sed -i "3s/.*/TEMP                   1/" ../NSL_SIMULATOR/INPUT/input.dat
        source equilibrate.sh 2
        # measures
        rm -f ../es4/DATA/GAS/*.dat
        rm -f ../NSL_SIMULATOR/INPUT/input.dat
        cp ../es4/DATA/GAS/input.in ../NSL_SIMULATOR/INPUT/
        mv ../NSL_SIMULATOR/INPUT/input.in ../NSL_SIMULATOR/INPUT/input.dat
        cd ../NSL_SIMULATOR/SOURCE
        echo "Computing gas phase properties."
        ./simulator.exe  # Execute simulator.exe
        cd -  # Change back to the original directory
        # copy output
        cp ../NSL_SIMULATOR/OUTPUT/temperature.dat ../es4/DATA/GAS/
        cp ../NSL_SIMULATOR/OUTPUT/potential_energy.dat ../es4/DATA/GAS/
        cp ../NSL_SIMULATOR/OUTPUT/pressure.dat ../es4/DATA/GAS/
        cp ../NSL_SIMULATOR/OUTPUT/kinetic_energy.dat ../es4/DATA/GAS/
        cp ../NSL_SIMULATOR/OUTPUT/total_energy.dat ../es4/DATA/GAS/
      ;;
  esac
else
  echo "Error: Argument must be an integer between 0 and 2."
  exit 1
fi