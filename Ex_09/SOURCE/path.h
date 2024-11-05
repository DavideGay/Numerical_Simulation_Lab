#ifndef __Path__
#define __Path__

#include "city.h"
#include <iomanip>
#include <fstream>

using namespace std;

class Path {

private:
  int path_length;
  vector<int> steps;
  //Random rnd;

public: // Function declarations
  //void initialize();
  void initialize(int);
  //void randominit(int);
  int get_path_length();
  int get_step(int index);
  void set_step(int index, int stepvalue);
  double tot_dist(vector<City>&);
  bool check_rep();
  bool check_start();
  void path_swap(int, int);
  void partial_inversion(int, int);
  void shift_forward(int, int, int);
  void shift_backward(int, int, int);
  void path_shuffle(Random& rnd);
  void print_path(ofstream&, vector<City>&);
  void print_path_steps(ofstream&);
  void out_path();
  vector<int> sort_bycomp(vector<int>& to_be_sorted);

};

#endif // __Path__