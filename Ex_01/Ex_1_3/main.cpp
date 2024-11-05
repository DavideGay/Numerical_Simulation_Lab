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

   
   std::ofstream outputFile("OUTPUT/outFile1.txt");
   if (!outputFile) {
      std::cerr << "Error opening file for writing." << std::endl;
      return 1;
   }

   /*
   SETTING:
   -Assume distance between parallel lines (their intersections with horizontal axis / x axis) is d = 1
   -Problem is invariant in the vertical dimension (the one parallel to the lines)
   -It is also periodic in the horizontal dimension
   What matters is whether the projection of the needle along the horizontal axis
      crosses the boundaries of a "cell" (i.e. the space between lines)
   APPROACH:
   -I generate a random horizontal coordinate between 0 and 1 for one extreme of the needle,
      exploiting the periodicity along the horizzontal dimension
      (can be viewed as implementation of Periodic Boundary Conditions)
   -I generate a random angle [0,2pi) and with it I extract the horizontal projection of the needle
   -If this projection crosses the boundaries of the unit cell, I update a "counter" variable
   */

   double L= 0.7;  // needle lenght: L < d
   
   double ave=0;
   double ave2=0;

   int totBlocks=100;
   int throwsInBlock=1000;

   for(int block=0; block<totBlocks; block++){
      double counter=0;
      double error=0;
      for(int i=0; i<throwsInBlock; i++){

         double p1 = rnd.Rannyu(); //generate x coordinate of one extreme of the needle

         // generate random angle using accept/reject method
         double x=rnd.Rannyu(-1,1); // Random x-coordinate in [-1, 1]
         double y=rnd.Rannyu(-1,1); // Random y-coordinate in [-1, 1]
         while (x * x + y * y > 1) {  // rejects if point is not within the unit circle
            x=rnd.Rannyu(-1,1);
            y=rnd.Rannyu(-1,1);
         }
         double ang=atan2(y,x);
         double p2 = p1+ (L*cos(ang)); //calculate coordinate of opposite extreme
         //if(x==0){
         //   p2=p1;
         //}else{ p2=p1+ L/sqrt(1+pow(y/x,2)); }
         /*  Formula above allows to avoid the use of trig functions altogether
         Formula is simply a "rescaling": from x and y sampled above (within unit circle) calculates the corresponding point
         on the circumference of radius L which maintains the same proportion y/x
         Alternatively, one can show that cos(atan(x))=1/sqrt(1+x^2) and apply this to the formula that combines
         angle= atan(y/x) and p2= p1 + L*cos(angle)   */

         if(floor(p2)-floor(p1) != 0){
            counter +=1.;
         }

      }
      double pi=(throwsInBlock/counter)*2*L;
      double pi2=pi*pi;
      ave+=pi;
      ave2+=pi2;
      if(block>0){
         error=sqrt((ave2/(block+1)-pow(ave/(block+1),2))/(block));
      }

      outputFile << pi << " " << ave/(block+1) << " " << error << endl;
   }


   outputFile.close();



   rnd.SaveSeed();
   return 0;
}