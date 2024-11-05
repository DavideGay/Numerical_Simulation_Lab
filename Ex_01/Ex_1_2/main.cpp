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

   std::ofstream outputFile("OUTPUT/outFile1_2.txt");
   if (!outputFile) {
      std::cerr << "Error opening file for writing." << std::endl;
      return 1;
   }


   
   int N[4]={1, 2, 10, 100};
   for(int j=0; j<10000; j++){
      for(int k=0; k<4; k++){
         double ave=0;
         double ave_exp=0;
         double ave_lor=0;
         for(int i=0; i<N[k]; i++){
            ave+= rnd.Rannyu();
            ave_exp+= rnd.Exp(1);
            ave_lor+=rnd.Lorentz(0, 1);
         }
         outputFile << ave/N[k] << " " << ave_exp/N[k] << " " << ave_lor/N[k] << " ";
      }
      outputFile << std::endl;
   }

   outputFile.close();

   rnd.SaveSeed();
   return 0;
}