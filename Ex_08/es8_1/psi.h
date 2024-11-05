#ifndef __Psi__
#define __Psi__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "random.h"

using namespace std;

class Psi {

private:
    double sigma;
    double mu;
    double temp;
    Random rnd;

public:
    void initialize(double starting_sigma, double starting_mu, double starting_temp);
    void finalize();

    double V(double x);
    double psi(double x);
    double psi_double_prime(double x);
    double local_energy(double x);

    double metropolis_step(double x, double delta);
};

#endif // __Psi__