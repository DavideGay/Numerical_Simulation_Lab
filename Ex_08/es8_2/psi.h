#ifndef __Psi__
#define __Psi__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <tuple>
#include "random.h"

using namespace std;

class Psi {

private:
    double sigma;
    double mu;
    double temp;   // temperature for SA
    double delta;   // metropolis algorithm delta (i.e. step size) for energy computation
    double energy;   // (estimated) energy of wavefunction
    double uncertainty;   // uncertainty of energy measurement
    Random rnd;

public:
    void initialize(double starting_sigma, double starting_mu, double starting_temp, double metro_delta);
    void finalize();

    // return or set Psi parameters/values
    double get_sigma();
    double get_mu();
    double get_temp();
    void set_sigma(double newsigma);
    void set_mu(double newmu);
    void set_temp(double newtemp);
    double get_energy();
    double get_uncertainty();

    double V(double x);   // potential
    double psi(double x);   // wavefunction Psi
    double psi_double_prime(double x);   // second derivative of Psi
    double local_energy(double x);   // kinetic + potential energy

    double metropolis_step(double x, double delta);   // update position using metropolis acceptance
    std::tuple<double, double> energy_unc();   // energy average and uncertainty with data blocking
    void final_output(ofstream& outEnergy, ofstream& outPsi);   // output energy and uncertainty as function of block # and plot wavefunction

    void SA_step(double newsigma, double newmu);   // update mu & sigma using metropolis acceptance
    double propose_mu();   // propose new value of mu with random change
    double propose_sigma();   // propose new value of sigma with random change
};

#endif // __Psi__