#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "random.h"
#include "walker.h"

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;
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

   
   std::ofstream outputDiscrete("OUTPUT/output_discrete.dat");
   std::ofstream outputContinous("OUTPUT/output_continous.dat");
   if (!outputContinous || !outputDiscrete) {
      std::cerr << "Problem opening output files." << std::endl;
      return 1;
   }


   int M=10000;  // total number of random walks
   int N=100;  // number of random walks in each block
   int tot_steps=100;  // length of each random walk

   vector<RandomWalk_3D> walkers;
   walkers.resize(M);   // generate M random walks objects, initially of length 0


   // Discrete step RWs
   for(int step=0; step<tot_steps; step++){
      double ave_r2=0;        // mean value of r^2 at each step
      double ave_r2squared=0;
      double error=0;

      for(int block=0; block<M/N; block++){
         double block_r2=0;      // mean value over a single block
         double block_r2squared=0;
         
         for(int i=0; i<N; i++){  // update all random walks in a block and compute the mean of r^2
            walkers[block*N +i].single_step_discr(rnd);
            block_r2+=walkers[block*N + i].get_r2();
         }

         block_r2/=N;
         block_r2squared=pow(block_r2, 2);
         ave_r2+=block_r2;   // inside each block, update the overall mean value
         ave_r2squared+=block_r2squared;
      }
      error= sqrt(((ave_r2squared/(M/N))-pow(ave_r2/(M/N), 2))/(M/N));
      outputDiscrete << ave_r2/(M/N) << " " << error << endl;
   }

   // Resets all RWs back to starting point
   for(int i=0; i<M; i++){
      walkers[i].reset_rw();
   }

   // Continous RWs
   for(int step=0; step<tot_steps; step++){
      double ave_r2=0;        // mean value of r^2 at each step
      double ave_r2squared=0;
      double error=0;
      for(int block=0; block<M/N; block++){
         double block_r2=0;      // mean value over a single block
         double block_r2squared=0;
         for(int i=0; i<N; i++){  // update all random walks in a block and compute the mean of r^2
            walkers[block*N +i].single_step_cont(rnd);
            block_r2+=walkers[block*N + i].get_r2();
         }
         block_r2/=N;
         block_r2squared=pow(block_r2, 2);
         ave_r2+=block_r2;   // inside each block, update the overall mean value
         ave_r2squared+=block_r2squared;
      }      
      error= sqrt(((ave_r2squared/(M/N))-pow(ave_r2/(M/N), 2))/(M/N));
      outputContinous << ave_r2/(M/N) << " " << error << endl;
   }


   outputDiscrete.close();
   outputContinous.close();


   rnd.SaveSeed();
   return 0;
}