#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"

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

   
   std::ofstream outputFile("OUTPUT/outFile.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }


   int M=10000;
   int N=100;  //number of blocks
   int L=int(M/N);   //data per block
   double moving_ave =0;
   double moving_ave2=0;

   for(int i=0; i<N; i++){
      double block_ave=0;
      double block_ave2=0;
      double error=0;   //deviazione standard della media ^2
      for(int j=0; j<L; j++){   //inside each block
         block_ave+=(M_PI/2)*(cos(rnd.Rannyu()*(M_PI/2)));;   // calculate function in a random value (uniform between 0,1) and add to block mean
      }
      block_ave/=L; //compute the block mean
      block_ave2=pow(block_ave, 2);  //compute the squared block mean
      moving_ave+=block_ave;
      moving_ave2+=block_ave2;

      if(i>0){
         error= sqrt(((moving_ave2/(i+1))-pow(moving_ave/(i+1), 2))/(i+1));
      }
      outputFile << block_ave << " "<< moving_ave/(i+1) << " " << error << std::endl;   //for each block, outputs moving average
   }

   outputFile.close();



   std::ofstream outputFile2("OUTPUT/outFile2.txt");
   if (!outputFile2) {
      std::cerr << "Error opening file for writing." << std::endl;
      return 1;
   }

   double moving_ave_imp =0;
   double moving_ave2_imp=0;

   for(int i=0; i<N; i++){
      double block_ave_imp=0;
      double block_ave2_imp=0;
      double error_imp=0;   //deviazione standard della media ^2
      for(int j=0; j<L; j++){   //inside each block
         double singledat=1- sqrt(1-rnd.Rannyu());   // random point distributed with p(x)=2(1-x) (normalized)
         block_ave_imp+=(M_PI*cos(singledat*(M_PI/2)))/(4*(1-singledat));;   // calculate function in a random value (uniform between 0,1) and add to block mean
      }
      block_ave_imp/=L; //compute the block mean
      block_ave2_imp=pow(block_ave_imp, 2);  //compute the squared block mean
      moving_ave_imp+=block_ave_imp;
      moving_ave2_imp+=block_ave2_imp;

      if(i>0){
         error_imp= sqrt(((moving_ave2_imp/(i+1))-pow(moving_ave_imp/(i+1), 2))/(i+1));
      }
      outputFile2 << block_ave_imp << " "<< moving_ave_imp/(i+1) << " " << error_imp << std::endl;   //for each block, outputs moving average
   }

   outputFile2.close();

   rnd.SaveSeed();
   return 0;
}