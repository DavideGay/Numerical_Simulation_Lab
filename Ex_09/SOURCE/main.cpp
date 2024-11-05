#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "city.h"
#include "path.h"
#include "population.h"

using namespace std;
  
int main (int argc, char *argv[]){

  
  ///////// INITIALIZATION //////////////////////////////

  Population generation;
  generation.initialize();   // a population of random paths is generated

  ///////// EVOLUTION ///////////////////////////////////

  generation.fitness_sort();   // sort population based on fit (total distance)

  generation.write_mean_best_half();
  generation.write_best_dist();

  for(int i=0; i<generation.get_N_generations(); i++){
    generation.crossover();      // create a new generation using selection operator and crossover operator
    generation.mutate();  // apply mutation operators
    generation.fitness_sort();

    generation.write_mean_best_half();
    generation.write_best_dist();
    generation.write_best_path();
  }

  generation.write_last_best();

  /////////////////////////////////////////////
  
  generation.finalize();
  
  return 0;
}