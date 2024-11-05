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

   /// PART 1 //////////////////////////////

   std::ofstream outputFile("OUTPUT/outFile1.txt");
   if (!outputFile) {
      std::cerr << "Error opening file for writing." << std::endl;
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
         block_ave+=rnd.Rannyu();  // calculate function in a random value (uniform between 0,1) and add to block mean
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

   /// PART 2 //////////////////////////////

   std::ofstream outputFile2("OUTPUT/outFile2.txt");
    if (!outputFile2) {
        std::cerr << "Error opening file for writing." << std::endl;
    }

   moving_ave =0;
   moving_ave2=0;

   for(int i=0; i<N; i++){
      double block_ave=0;
      double block_ave2=0;
      double error=0;   //deviazione standard della media ^2
      for(int j=0; j<L; j++){   //inside each block
         block_ave+=pow(rnd.Rannyu()-0.5,2.);  // calculate function in a random value (uniform between 0,1) and add to block mean
      }
      block_ave/=L; //compute the block mean
      block_ave2=pow(block_ave, 2);  //compute the squared block mean
      moving_ave+=block_ave;
      moving_ave2+=block_ave2;

      if(i>0){
         error= sqrt(((moving_ave2/(i+1))-pow(moving_ave/(i+1), 2))/(i+1));
      }
      outputFile2 << block_ave << " "<< moving_ave/(i+1) << " " << error << std::endl;   //for each block, outputs moving average
   }

   outputFile2.close();

   /// PART 3 //////////////////////////////

   std::ofstream outputFile3("OUTPUT/outFile3.txt");
   if (!outputFile3) {
      std::cerr << "Error opening file for writing." << std::endl;
   }

   int K= 100;   //divide [0,1] in K identical sub-intervals
   int n=1000;


   for(int j=0; j<100; j++){
      double chi2=0;

      double counter[K] ={0};

      for(int i=0; i<n; i++){
         counter[int(floor(rnd.Rannyu()*100))]+=1;
      }

      for(int i=0; i<K; i++){

         chi2+= pow((counter[i] - (n/K)),2) / (n/K);
      }

      outputFile3 << chi2 << std::endl;   // e con questi valori di chi2 farò istogramma su py
   }

   outputFile3.close();



   rnd.SaveSeed();
   return 0;
}