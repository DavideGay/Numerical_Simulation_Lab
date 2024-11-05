#include "random.h"
#include "psi.h"

using namespace std;

int main (int argc, char *argv[]){

   std::ofstream outputSA("OUTPUT/SA.dat");
   std::ofstream outputEnergy("OUTPUT/energy.dat");
   std::ofstream outputPsi("OUTPUT/psi.dat");
   if (!outputSA or !outputEnergy or !outputPsi) {
      std::cerr << "Error opening output files." << std::endl;
      return 1;
   }

   //////// INITIALIZATION ///////////////////////////////////////////////////////

   Psi psi_t;
   double mu = 0.8;
   double sigma = 0.6;
   double temperature =2.0;
   double delta =2.;   // metropolis delta
   psi_t.initialize(sigma, mu, temperature, delta);

   //////// SIMULATED ANNEALING //////////////////////////////////////////////////

   double newsigma, newmu;   // variables to store newly proposed parameteres at each SA step
   cout << "Performing Simulated Annealing starting from temperature T=" << temperature << endl;

   while(temperature>=0.01){
      psi_t.set_temp(temperature);

      newsigma=psi_t.propose_sigma();
      newmu=psi_t.propose_mu();
      psi_t.SA_step(newsigma, newmu);

      outputSA << psi_t.get_temp() << " " << psi_t.get_sigma() << " " << psi_t.get_mu() << " " << psi_t.get_energy() << " " << psi_t.get_uncertainty() << endl;

      temperature*=0.997;
   }

   //////// FINALIZATION /////////////////////////////////////////////////////////

   cout << "Final parameters:" << endl << "Sigma     Mu     Energy     Uncertainty" << endl;
   cout << psi_t.get_sigma() << " " << psi_t.get_mu() << " " << psi_t.get_energy() << " " << psi_t.get_uncertainty() << endl;

   psi_t.final_output(outputEnergy, outputPsi);

   outputSA.close();
   outputEnergy.close();
   return 0;
}