#include <iostream>
#include <math.h>
#include "city.h"
#include "path.h"

using namespace std;

void Path :: initialize(int Ncities){
    path_length=Ncities;
    if(Ncities<2){
        path_length=2;
        cout << "Path size must be 2 or greater !" << endl;
    }
    steps.resize(path_length);
    steps[0]=0;
    for(int i=1; i<path_length; i++){
        steps[i]=i;
    }
}

int Path::get_path_length(){
    return path_length;
}

int Path::get_step(int index){
    return steps[index];
}

void Path::set_step(int index, int stepvalue){   // use with caution !! Can mess up the no repetitions constraint
    if(index!=0 && stepvalue>0 && stepvalue<path_length){
        steps[index]=stepvalue;
    }
}

vector<int> Path::get_steps(){
    return steps;
}

void Path::set_steps(vector<int>& st){   // use with caution !! Can mess up the no repetitions constraint
    steps=st;
}

bool Path::check_rep(){
    bool rep=true;
    for(int i=1; i<path_length; i++){
        int t= steps[i];
        for(int j=i+1; j<path_length; j++){
            if(steps[j]==t){rep=false;}
        }
    }
    return rep;
}

bool Path::check_start(){
    bool start=false;
    if(steps[0]==0){start=true;}
    return start;
}

void Path::path_swap(int a, int b){
    if(a< path_length && b<path_length && a >0 && b>0){
        int t=steps[a];
        steps[a]=steps[b];
        steps[b]=t;
    }
}

void Path::path_shuffle(Random& rnd){ // shuffles path using Fisher-Yates algorithm, leaves the first element untouched
    for(int i=path_length-1; i>1; i--){
        double r=rnd.Rannyu(1,i+1);
        int j= int(r);
        //cout << "rand indeces: " << i << " " << r << " " << j << endl;
        path_swap(i,j);
    }
}

void Path::print_path(ofstream& outfile, vector<City>& cities){
    for(int i=0; i<path_length; i++){
        outfile << steps[i] << " ";
    }
    outfile << "total distance: " << tot_dist(cities);
    outfile << endl;
}

void Path::print_path_steps(ofstream& outfile){
    for(int i=0; i<path_length; i++){
        outfile << steps[i] << " ";
    }
    outfile <<  endl;
}


void Path::out_path(){
    for(int i=0; i<path_length; i++){
        cout << steps[i] << " ";
    }
    cout << endl;
}

double Path::tot_dist(vector<City>& cities){
    double total_dist=0;

    for (int i = 0; i < path_length - 1; ++i){
      total_dist += sqrt(pow(cities[steps[i + 1]].getx()-cities[steps[i]].getx(), 2) + pow(cities[steps[i + 1]].gety()-cities[steps[i]].gety(),2));
      //total_dist += abs(cities[steps[i + 1]].getx()-cities[steps[i]].getx()) + abs(cities[steps[i + 1]].getx()-cities[steps[i]].getx());
    }
    // Add the distance from the last city back to the first city
    total_dist += sqrt(pow(cities[steps[0]].getx()-cities[steps[path_length - 1]].getx(), 2) + pow(cities[steps[0]].gety()-cities[steps[path_length - 1]].gety(),2));
    //total_dist += abs(cities[steps[0]].getx()-cities[steps[path_length - 1]].getx()) + abs(cities[steps[0]].getx()-cities[steps[path_length - 1]].getx());

    return total_dist;
}

void Path::partial_inversion(int begin, int end){
    if(end-begin>1 && end<=path_length && begin>=0){
        for(int i=0; i<(int((end-begin-1)/2) + 1); i++){
            path_swap(begin+i, end-1-i);
        }
    }
}

void Path::shift_forward(int begin, int end, int step){
    if(begin>0 && end-begin>0 && step>0 && end+step<=path_length){
        for(int i=0;i<step; i++){
            for(int j=end-1+i; j>=begin+i; j--){
                path_swap(j,j+1);
            }
        }
    }
}

void Path::shift_backward(int begin, int end, int step){
    if(end<path_length && end-begin>0 && step>0 && begin-step>0){
        for(int i=0;i<step; i++){
            for(int j=begin-i; j<end-i; j++){
                path_swap(j,j-1);
            }
        }
    }
}

vector<int> Path::sort_bycomp(vector<int>& to_be_sorted){
    if(get_path_length()>=int(to_be_sorted.size())){
        vector<int> sorted;
        sorted.resize(int(to_be_sorted.size()));
        int counter=0;
        for(int i=0; i<get_path_length(); i++){
            for(int j=0; j<int(to_be_sorted.size()); j++){
            if(get_step(i)==to_be_sorted[j]){
                sorted[counter]=get_step(i);
                counter++;
            }
            }
        }
        return sorted;
    }else{return vector<int>();}
}