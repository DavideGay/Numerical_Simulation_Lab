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

   
   std::ofstream outputFile("../OUTPUT/outFile1.txt");

   // Check if the file is opened successfully
   if (!outputFile) {
      std::cerr << "Error opening file for writing." << std::endl;
      return 1; // Return an error code
   }

   int N=10000; // total number of montecarlo extractions
   int M=100;  // number of data in a block
   int blocks= int(N/M);

   double S_0= 100; // price at time t=0;
   double rate= 0.1;
   double sigma= 0.25; // volatility
   double T= 1;
   double strikeprice=100;

   int time_int_n= 100; // number of time subintervals of (0,T)

   double put_ave=0;
   double call_ave=0;
   double put_ave_discr=0;
   double call_ave_discr=0;

   double err_call_2=0;
   double err_call=0;
   double err_put_2=0;
   double err_put=0;
   double err_call_discr_2=0;
   double err_call_discr=0;
   double err_put_discr_2=0;
   double err_put_discr=0;

   for(int block=0; block< blocks; block++){

      double block_call=0;
      for(int i=0; i<M; i++){
         double S= S_0* exp( (rate- (pow(sigma,2)/2)) *T + sigma*rnd.Gauss(0, T) );
         block_call+=exp(-rate*T) * max(0., S - strikeprice);
      }
      call_ave+=block_call/M;
      err_call_2+=pow( call_ave/(block+1)- (block_call/M) ,2);
      if(block==0){
         err_call=0;
      }else{ err_call=sqrt(err_call_2/(block*(block+1))); }

      double block_put=0;
      for(int i=0; i<M; i++){
         double S= S_0* exp( (rate- (pow(sigma,2)/2)) *T + sigma*rnd.Gauss(0, T) );
         block_put+=exp(-rate*T) * max(0., strikeprice - S);
      }
      put_ave+=block_put/M;
      err_put_2+=pow( put_ave/(block+1)- (block_put/M) ,2);
      if(block==0){
         err_put=0;
      }else{ err_put=sqrt(err_put_2/(block*(block+1))); }

      double block_call_discr=0;
      for(int i=0; i<M; i++){
         double S=S_0;
         for(int time_int=0; time_int<time_int_n; time_int++){
            S*= exp( (rate- (pow(sigma,2)/2)) *(T/time_int_n) + sigma*rnd.Gauss(0,1)*sqrt(T/time_int_n) );
         }
         block_call_discr+=exp(-rate*T) * max(0., S - strikeprice);
      }
      call_ave_discr+=block_call_discr/M;
      err_call_discr_2+=pow( call_ave_discr/(block+1)- (block_call_discr/M) ,2);
      if(block==0){
         err_call_discr=0;
      }else{ err_call_discr=sqrt(err_call_discr_2/(block*(block+1))); }

      double block_put_discr=0;
      for(int i=0; i<M; i++){
         double S=S_0;
         for(int time_int=0; time_int<time_int_n; time_int++){
            S*= exp( (rate- (pow(sigma,2)/2)) *(T/time_int_n) + sigma*rnd.Gauss(0,1)*sqrt(T/time_int_n) );
         }
         block_put_discr+=exp(-rate*T) * max(0., strikeprice - S);
      }
      put_ave_discr+=block_put_discr/M;
      err_put_discr_2+=pow( put_ave_discr/(block+1)- (block_put_discr/M) ,2);
      if(block==0){
         err_put_discr=0;
      }else{ err_put_discr=sqrt(err_put_discr_2/(block*(block+1))); }

      outputFile << call_ave/(block+1) << " " << err_call << " ";
      outputFile << put_ave/(block+1) << " " << err_put << " ";
      outputFile << call_ave_discr/(block+1) << " " << err_call_discr << " ";
      outputFile << put_ave_discr/(block+1) << " " << err_put_discr << endl;
   }

   // check errori !!!

   outputFile.close();



   rnd.SaveSeed();
   return 0;
}