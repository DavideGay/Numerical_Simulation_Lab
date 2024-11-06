#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "city.h"
#include "path.h"
#include "population.h"
#include "mpi.h"

using namespace std;
  
int main (int argc, char *argv[]){

  int size, rank;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(size>11){
      cout<<"Too many processes."<<endl;
      return 1;
  }

  
  ///////// INITIALIZATION //////////////////////////////

  Population continent;
  continent.initialize(rank);   // a population of random paths is generated

  // NOTE: in order to have all processes work on the same cities configuration, it is recommended to set
  // CITIES_TYPE as 3 in 'input.dat' and work with a 'coordinates.dat' file. To obtain a certain
  // configuration, one can test run this program with the desired CITIES_TYPE and N_CITIES
  // input and subsequently copy "cities.dat" from "../OUTPUT" to "../INPUT" (renaming it "coordinates.dat").

  int N_migration=20;   // number of generations after which a migration occures

  ///////// EVOLUTION ///////////////////////////////////

  continent.fitness_sort();   // sort population based on fit (total distance)

  continent.write_mean_best_half();
  continent.write_best_dist();

  for(int i=0; i<continent.get_N_generations(); i++){
    continent.crossover();      // create a new generation using selection operator and crossover operator
    continent.mutate();  // apply mutation operators
    continent.fitness_sort();

    //////// MIGRATION ////////////////////////////////
    if(i>1 && i%N_migration==0 && continent.migrate()){

      vector<int> emigrant = continent.get_single_path(0);
      vector<int> immigrant(continent.get_N_cities());
      MPI_Status status;
      if(i<N_migration +2){
        cout << "Process #" << rank << ": migrations applied" << endl;
      }
      
      int dest = (rank + 1) % size;
      int source = (rank - 1 + size) % size;
      MPI_Sendrecv(emigrant.data(), emigrant.size(), MPI_INT, dest, 0, 
                 immigrant.data(), immigrant.size(), MPI_INT, source, 0, 
                 MPI_COMM_WORLD, &status);

      continent.set_single_path(immigrant, 0);

      //continent.print_single_path(outputbest, 0);

      continent.fitness_sort();

    }
    ///////////////////////////////////////////////////

    continent.write_mean_best_half();
    continent.write_best_dist();
    continent.write_best_path();
  }

  continent.write_last_best();

  /////////////////////////////////////////////
  
  continent.finalize();

  MPI_Finalize();
  
  return 0;
}