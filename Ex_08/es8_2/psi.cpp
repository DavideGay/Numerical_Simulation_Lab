#include "psi.h"

void Psi::initialize(double starting_sigma, double starting_mu, double starting_temp, double metro_delta){
    sigma=starting_sigma;
    mu=starting_mu;
    temp=starting_temp;
    delta=metro_delta;
    energy=0;

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

    tie(energy, uncertainty)= this->energy_unc();   // compute energy and uncertainty

    cout << "Wavefunction initialized with parameters: mu = " << mu << " , sigma = " << sigma << endl;
}

void Psi::finalize(){
    rnd.SaveSeed();
}

double Psi::get_sigma(){
    return sigma;
}

double Psi::get_mu(){
    return mu;
}

double Psi::get_temp(){
    return temp;
}

void Psi::set_sigma(double newsigma){
    sigma=newsigma;
}

void Psi::set_mu(double newmu){
    mu=newmu;
}

void Psi::set_temp(double newtemp){
    temp=newtemp;
}

double Psi::get_energy(){
    return energy;
}

double Psi::get_uncertainty(){
    return uncertainty;
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

// compute energy & uncertainty as function of blocks + plot Psi
void Psi::final_output(ofstream& outEnergy, ofstream& outPsi){
    int blocks=1000;
    int steps=1000;
    double x=0;
    double energy_ave=0;
    double energy_ave_2=0;
    
    int equilibration_steps=0;
    for(int i=0; i<equilibration_steps; i++){   // equilibration steps
        if(i%10==0){
            x=-1.*x;
        }
        x = this->metropolis_step(x,delta);
    }

    for(int j=0; j<blocks; j++){
        double energy_block=0;
        double energy_block_2=0;
        double error=0;
        
        for(int i=0; i<steps; i++){
            if(i%10==0){
                x=-1.*x;   // flipping x every n steps helps the algorithm explore the x space evenly
                // Note: this does not interfere with the MRT algorithm since the trial Psi is symmetrical
            }
            x = this->metropolis_step(x, delta);   // update position with metropolis
            outPsi << x << endl;
            energy_block+= this->local_energy(x);   // add contribution to energy calculated in x
        }
        energy_block=energy_block/steps;
        energy_block_2=pow(energy_block, 2);
        energy_ave+=energy_block;
        energy_ave_2+=energy_block_2;
        if(j>0){
        error= sqrt(((energy_ave_2/(j+1))-pow(energy_ave/(j+1), 2))/(j+1));
        }
        outEnergy << j+1 << " " << energy_ave/(j+1) << " " << error << endl;
    }
}

// compute energy & uncertainty
std::tuple<double, double> Psi::energy_unc(){
    int blocks=1000;
    int steps=100;
    double x=0;
    double energy_ave=0;
    double energy_ave_2=0;
    double uncertainty=0;
    
    int equilibration_steps=0;
    for(int i=0; i<equilibration_steps; i++){   // equilibration steps
        if(i%10==0){
            x=-1.*x;
        }
        x = this->metropolis_step(x,delta);
    }

    for(int j=0; j<blocks; j++){
        double energy_block=0;
        double energy_block_2=0;
        double error=0;
        
        for(int i=0; i<steps; i++){
            if(i%10==0){
                x=-1.*x;   // flipping x every n steps helps the algorithm explore the x space evenly
                // Note: this does not interfere with the MRT algorithm since the trial Psi is symmetrical
            }
            x = this->metropolis_step(x, delta);   // update position with metropolis
            energy_block+= this->local_energy(x);   // add contribution to energy calculated in x
        }
        energy_block=energy_block/steps;
        energy_block_2=pow(energy_block, 2);
        energy_ave+=energy_block;
        energy_ave_2+=energy_block_2;
        if(j>0){
        error= sqrt(((energy_ave_2/(j+1))-pow(energy_ave/(j+1), 2))/(j+1));
        }
        uncertainty=error;
    }

    return  std::make_tuple(energy_ave / blocks, uncertainty);
}

// SA step
void Psi::SA_step(double newsigma, double newmu) {

    double oldsigma=sigma;
    sigma=newsigma;
    double oldmu= mu;
    mu=newmu;

    double newenergy, newunc;
    tie(newenergy, newunc)= this->energy_unc();

    double p_accept = std::min(1., exp(-(newenergy-energy)/temp));
    if (rnd.Rannyu() < p_accept) {   // if move is accepted, update energy and uncertainty
        energy= newenergy;
        uncertainty= newunc;
    }else{                           // if move is rejected, restore previous wavefunction parameters
        sigma=oldsigma;
        mu=oldmu;
    }
}

// propose updated mu for SA
double Psi::propose_mu(){
    return mu+ rnd.Rannyu(-1,1) * 0.05;
}

// propose updated sigma for SA
double Psi::propose_sigma(){
    return sigma+ rnd.Rannyu(-1,1) * 0.05;
}