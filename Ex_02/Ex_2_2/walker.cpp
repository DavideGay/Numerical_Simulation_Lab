#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include "walker.h"

RandomWalk_3D :: RandomWalk_3D(){
    x=0;
    y=0;
    z=0;
}
// Default constructor, sets origin as starting point

RandomWalk_3D :: ~RandomWalk_3D(){}

void RandomWalk_3D :: single_step_discr(Random& rnd) {
    // This function processes a single step in a cubic lattice
    double r=rnd.Rannyu()*6 -3;
    if(r<0){
        if(r<-2){ x-=1; }
        else if(r>-1){ y-=1; }
        else{ z-=1; }
    }
    else{
        if(r<1){ x+=1;}
        else if(r>2){ y+=1;}
        else{ z+=1;}
    }
}

void RandomWalk_3D :: single_step_cont(Random& rnd) {
    // This function processes a single step in 3D space
    double newx=rnd.Gauss(0,1);
    double newy=rnd.Gauss(0,1);
    double newz=rnd.Gauss(0,1);
    while(newx==0 and newy==0 and newz==0){
        newx=rnd.Gauss(0,1);
        newy=rnd.Gauss(0,1);
        newz=rnd.Gauss(0,1);
    }
    double normalization=sqrt(newx*newx + newy*newy + newz*newz);
    newx/=normalization;
    newy/=normalization;
    newz/=normalization;

    x+=newx;
    y+=newy;
    z+=newz;
}

double RandomWalk_3D :: get_x(){
    // This function returns the x coordinate
    return x;
}

double RandomWalk_3D :: get_y(){
    // This function returns the y coordinate
    return y;
}

double RandomWalk_3D :: get_z(){
    // This function returns the z coordinate
    return z;
}

double RandomWalk_3D :: get_r2(){
    // This function returns the squared distance from the origin
    return pow(x, 2.) + pow(y, 2.) + pow(z, 2.);
}

void RandomWalk_3D :: reset_rw(){
    x=0; y=0; z=0;
}