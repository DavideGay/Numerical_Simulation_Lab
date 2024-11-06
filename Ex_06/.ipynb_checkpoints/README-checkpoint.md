## Instructions - Ex. 04
This exercise uses the `NSL_SIMULATOR` code supplied in a separate directory.
Instructions on how to compile and execute it are located inside `NSL_SIMULATOR`.

For better usage, a few bash scripts have been provided, which significantly help in the resolution of exercises 4,6 and 7.

To run a certain script, simply use:
```
source script.sh
```

The scripts provided for this exercise are:
- `equilibration.sh` takes care of producing equilibration data for one phase (argument: 0 solid, 1 liquid, 2 gas).
- `eq_all.sh` uses the above script to produce equilibration data for one phase (argument: 0 solid, 1 liquid, 2 gas), at 3 different temperatures in order to find the one that best approaches the target temperature after equilibration.
- `measures.sh` extracts all necessary measurements for one phase (argument: 0 solid, 1 liquid, 2 gas) after having equilibrated the system.