#include <iostream>
#include <math.h>
#include "city.h"

using namespace std;

void City :: initialize(){
    x=0;
    y=0;
    name="";
}

void City :: setposition(double posx, double posy){
    x=posx;
    y=posy;
}

void City:: setname(std::string newname){
    name=newname;
}

double City :: getx(){
    return x;
}

double City :: gety(){
    return y;
}

std::string City :: getname(){
    return name;
}