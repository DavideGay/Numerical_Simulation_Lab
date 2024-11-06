## Instructions - Ex. 06
This exercise uses the `NSL_SIMULATOR` code supplied in a separate directory.
Instructions on how to compile and execute it are located inside `NSL_SIMULATOR`.

For better usage, a few bash scripts have been provided, which significantly help in the resolution of exercises 4,6 and 7.

To run a certain script, simply use:
```
source script.sh
```

The scripts provided for this exercise are:
- `equilibration.sh` generates the data necessary to analyse the equilibration of the system at 4 different temperatures. It takes as argument the simulation type: 2 for Metropolis, 3 for Gibbs.
- `restart.sh` performs equilibration for the system by performing N (second argument) "coolings" from temperature 2 to 0.5. It takes as first argument the simulation type: 2 for Metropolis, 3 for Gibbs.
- `measures.sh` takes 2 arguments: the simulation type and the value of **h**. It equilibrates the system and then generates all the data relative to equilibrium measurements.
- `run.sh` uses measures.sh to generate the required equilibrium data: internal energy, susceptibility and specifc heat at h=0.0 and magnetization at h=0.02. It takes as argument the simulation type: 2 for Metropolis, 3 for Gibbs.