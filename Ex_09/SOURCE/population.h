#ifndef __Population__
#define __Population__

#include "path.h"

using namespace std;

class Population {

private:
  int pop_size;
  int path_length;
  vector<Path> pop;
  Random rnd;
  vector<City> cities;
  double mutation_prob;
  int N_generations;
  int rank;

public:
  Population();   // default constructor
  Population(Population&);   // copy constructor
  void initialize(int setrank=0);   // standard initialization
  void quick_initialize(int population_size, int path_size, Random& random);   // "quick" initialization. To be used inside crossover method
  void finalize();

  vector<City> cities_on_circumference_eq(int Ncities); // creates Ncities equally spaced on the unit circle
  vector<City> cities_on_circumference_rand(int Ncities); // creates Ncities randomly on the unit circle (ordered)
  vector<City> cities_inside_square(int Ncities); // creates Ncities randomly distributed inside (0,1)x(0,1) square
  // vector<City> cities_from_file(ifstream&); // reads cities from input file

  int get_pop_size();
  int get_path_length();
  int get_N_generations();
  vector<Path> get_population();
  Random get_rnd();
  void set_path(int index, Path& path);
  Path get_path(int index);

  double get_best_dist();
  double get_best_path();
  double get_mean_dist();            // calculates mean distance across all paths
  double get_mean_best_half();            // calculates mean distance across best half of population
  void write_mean_best_half();
  void write_best_dist();
  void write_best_path();
  void write_all_pop();       // prints out the entire population along with single distances
  void write_last_best();

  void shuffle_all();                // shuffles each single path. To be used when creating a totally random population
  void pop_singleswap(int a, int b); // swaps the position of two elements inside the population
  bool check_pop();                  // checks if every element of the population satisfies restraints
  void fitness_sort();   // sorts all individual based on their fit (total distance) from best to worst

  void mutation_pair(double probability);       // swap two cities inside a path, across the population with set probability
  void mutation_inversion(double probability);  // invert the order of m cities inside a path
  void mutation_permutation(double probability);        // not implemented
  void mutation_shift(double probability);      // shift forward m cities by s steps inside a path
  void mutate();              // perform all mutations across the population with set probability

  int select(double exponent=5);      // selection operator: selects random individual with preference to most fit
  void single_crossover(int index1, int index2, int cut_index); // performs crossover between two "parents" to create two "children"
  void crossover(double exponent=5);  // creates an offspring by selecting random parents and performing a crossover

};

#endif // __Population__