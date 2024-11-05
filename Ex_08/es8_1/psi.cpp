#include "psi.h"

void Psi::initialize(double starting_sigma, double starting_mu, double starting_temp){
    // set constraints on parameters?
    sigma=starting_sigma;
    mu=starting_mu;
    temp=starting_temp;

    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
}

void Psi::finalize(){
    rnd.SaveSeed();
}

// Potential function V(x)
double Psi::V(double x) {
    return std::pow(x, 4) - 2.5 * std::pow(x, 2); // Modified potential
}

// Wavefunction psi(x)
double Psi::psi(double x) {
    return std::exp(-0.5 * std::pow((x - mu) / sigma, 2)) + std::exp(-0.5 * std::pow((x + mu) / sigma, 2));
}

// Second derivative of the wavefunction psi''(x)
double Psi::psi_double_prime(double x) {
    double term1 = (x - mu) * (x - mu) / (sigma * sigma) - 1;
    double term2 = (x + mu) * (x + mu) / (sigma * sigma) - 1;
    double gauss1 = std::exp(-0.5 * std::pow((x - mu) / sigma, 2));
    double gauss2 = std::exp(-0.5 * std::pow((x + mu) / sigma, 2));
    return (term1 * gauss1 + term2 * gauss2) / (sigma * sigma);
}

// Local energy E_loc(x)
double Psi::local_energy(double x) {
    double kinetic = -0.5 *(psi_double_prime(x) / psi(x));
    double potential = V(x);
    return kinetic + potential;
}

// Metropolis step
double Psi::metropolis_step(double x, double delta) {
    double x_new = x + rnd.Rannyu(-delta, delta);
    double p_accept = std::min(1.0, std::pow(psi(x_new) / psi(x), 2));
    if (rnd.Rannyu() < p_accept) {
        return x_new;
    } else {
        return x;
    }
}