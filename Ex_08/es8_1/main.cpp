#include "random.h"
#include "psi.h"

using namespace std;

int main (int argc, char *argv[]){

   std::ofstream outputMetro("OUTPUT/metro.dat");
   std::ofstream outputEnergy("OUTPUT/energy.dat");
   if (!outputMetro or !outputMetro) {
      std::cerr << "Error opening output files." << std::endl;
      return 1;
   }

   //////// INITIALIZATION ///////////////////////////////////////////////////////

   Psi psi_t;
   double mu = 1.;
   double sigma = 0.5;
   double temp =5.;   // not useful, for now
   psi_t.initialize(sigma, mu, temp);
   cout << "Wavefunction parameters: mu = " << mu << " , sigma = " << sigma << endl;

   //////// METROPOLIS SAMPLING //////////////////////////////////////////////////

   double x = 0.0; // Initial position
   int num_steps=1000000;
   double energy=0.;
   double acceptance=0;

   // Sampling steps
   for (int i = 0; i < num_steps; ++i) {
      if(i%10==0){
         x=-1.*x;   // flipping x every n steps helps the algorithm explore the x space evenly
                    // Note: this does not interfere with the MRT algorithm since the trial Psi is symmetrical
      }
      double x_old=x;
      x = psi_t.metropolis_step(x, 2.);
      if (x==x_old) {acceptance+=1;}  // to keep an eye on the acceptance rate

      outputMetro << x << endl;

      energy += psi_t.local_energy(x);
   }

   cout << "Metropolis acceptance rate: " << acceptance/num_steps << endl;
   cout << "Energy average of sampled function: " << energy/num_steps << endl;

   //////// ENERGY COMPUTATION ///////////////////////////////////////////////////

   int blocks=1000;
   int steps=1000;
   x=0;
   double energy_ave=0;
   double energy_ave_2=0;
   
   int equilibration_steps=0;
   for(int i=0; i<equilibration_steps; i++){   // equilibration steps, if needed
      if(i%10==0){
         x=-1.*x;
      }
      x = psi_t.metropolis_step(x, 2.);
   }

   for(int j=0; j<blocks; j++){
      double energy_block=0;
      double energy_block_2=0;
      double error=0;
      
      for(int i=0; i<steps; i++){
         if(i%10==0){
               x=-1.*x;
         }
         x = psi_t.metropolis_step(x, 2.);
         energy_block+= psi_t.local_energy(x);
      }
      energy_block=energy_block/steps;
      energy_block_2=pow(energy_block, 2);
      energy_ave+=energy_block;
      energy_ave_2+=energy_block_2;
      if(j>0){
      error= sqrt(((energy_ave_2/(j+1))-pow(energy_ave/(j+1), 2))/(j+1));
      }
      outputEnergy << energy_ave/(j+1) << " " << error << endl;
   }

   //////// FINALIZATION /////////////////////////////////////////////////////////

   outputMetro.close();
   outputEnergy.close();
   return 0;
}