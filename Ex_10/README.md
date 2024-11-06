## Instructions - Ex. 10
SOURCE contains source code for each of the exercises.
- To compile, use the command:
```
mpicxx main.cpp random.cpp city.cpp path.cpp population.cpp -o main
```
- To run:
```
mpiexec -np Number_of_processes main
```
where Number_of_processes must be susbstituted with the desired number of parallel processes to run.