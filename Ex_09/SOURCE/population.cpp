#include "population.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "city.h"
#include "path.h"
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

Population::Population(){   // default constructor
    pop_size=0;
    path_length=0;
    pop=vector<Path>();
}

Population::Population(Population& oldpop){   // copy constructor
    pop_size=oldpop.get_pop_size();
    path_length=oldpop.get_path_length();
    pop=oldpop.get_population();
    rnd=oldpop.get_rnd();
}

void Population :: initialize(int setrank){

    /////////////// Random Generator
    int seed[4];
    int p1, p2;
    ifstream Primes("../INPUT/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();
    ifstream input_seed("../INPUT/seed.in");
    string prop;
    if (input_seed.is_open()){
        while ( !input_seed.eof() ){
            input_seed >> prop;
            if( prop == "RANDOMSEED" ){
                input_seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input_seed.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;


    /////////////// PROPERTIES
    int cities_type;
    int N_cities;
    int population_size;
    double mut_prob;
    int N_gen;
    // default values:
    path_length=1;
    pop_size=1;
    cities={};
    mutation_prob=0.;
    N_generations=0;
    rank=setrank;

    ifstream input("../INPUT/input.dat"); // Start reading ../INPUT/input.dat
    string property;
    ofstream coutf;
    if(rank==0){coutf.open("../OUTPUT/cities.dat");}

    while ( !input.eof() ){
        input >> property;
        if( property == "CITIES_TYPE" ){
            input >> cities_type;
            if(cities_type > 3 || cities_type <0){
                cerr << "PROBLEM: invalid input" << endl;
                cerr << "// Possible CITIES_TYPE values:" << endl;
                cerr << "//  0: equally spaced cities on unit circumference" << endl;
                cerr << "//  1: randomly located cities on unit circumference" << endl;
                cerr << "//  2: randomly located cities inside (0,1)x(0,1) square" << endl;
                cerr << "//  3: read from file coordinates.dat" << endl;
                exit(EXIT_FAILURE);
            }
            if(rank==0){
                if(cities_type == 0)      coutf << "# EQUALLY SPACED CITIES ON UNIT CIRCUMFERENCE"  << endl;
                else if(cities_type == 1) coutf << "# RANDOMLY LOCATED CITIES ON UNIT CIRCUMFERENCE"         << endl;
                else if(cities_type == 2) coutf << "# RANDOMLY LOCATED CITIES INSIDE (0,1)x(0,1) SQUARE" << endl;
                else if(cities_type == 3) coutf << "# CITIES LOCATIONS FROM INPUT FILE" << endl;
            }
        } else if( property == "N_CITIES" ){
            input >> N_cities;
            if(N_cities < 3){
                cerr << "PROBLEM: invalid number of cities" << endl;
                exit(EXIT_FAILURE);
            } else path_length=N_cities;
        } else if( property == "POPULATION_SIZE" ){
            input >> population_size;
            if(population_size < 2){
                cerr << "PROBLEM: invalid number of individuals in population" << endl;
                exit(EXIT_FAILURE);
            } else pop_size=population_size;
        } else if( property == "ENDINPUT" ){
            cout << "Process #" << rank << ": successfully read input.dat" << endl;
            break;
        } else if( property == "MUTATION_PROBABILITY" ){
            input >> mut_prob;
            if(mut_prob < 0 || mut_prob>1.){
                cerr << "PROBLEM: invalid probability of mutations" << endl;
                exit(EXIT_FAILURE);
            } else mutation_prob=mut_prob;
        } else if( property == "N_GENERATIONS" ){
            input >> N_gen;
            if(N_gen < 0){
                cerr << "PROBLEM: invalid number of generations" << endl;
                exit(EXIT_FAILURE);
            } else N_generations=N_gen;
        } else cerr << "PROBLEM: unknown input" << endl;
    }
    input.close();

    // clear output files:
    ofstream outbesthalf;
    outbesthalf.open("../OUTPUT/best_half_" + std::to_string(rank) + ".dat",ios::trunc);
    outbesthalf.close();
    ofstream outbest;
    outbest.open("../OUTPUT/best_individual_" + std::to_string(rank) + ".dat",ios::trunc);
    outbest.close();
    ofstream outbestpath;
    outbestpath.open("../OUTPUT/best_path_steps_" + std::to_string(rank) + ".dat",ios::trunc);
    outbestpath.close();

    ////////////// INITIALIZE CITIES
    if(cities_type==0){
        cities= cities_on_circumference_eq(N_cities);
    } else if(cities_type==1){
        cities= cities_on_circumference_rand(N_cities);
    } else if(cities_type==2){
        cities= cities_inside_square(N_cities);

    } else if(cities_type==3){

        ifstream coordfile("../INPUT/coordinates.dat");
        if(!coordfile){
            cerr << "PROBLEM: file 'coordinates.dat' not found." << endl;
            exit(EXIT_FAILURE);
        }

        string line;
        int counter=0;
        while (getline(coordfile, line)){
            if (!line.empty()) { counter++; }
        }
        if(counter<3){
            cerr << "Invalid number of cities in 'coordinates.dat': must be 3 or more." << endl;
            exit(EXIT_FAILURE);
        }else{
            N_cities=counter;   // overrides value of N_cities from 'input.dat'
        }

        vector<City> cities_temp;
        cities_temp.resize(N_cities);
        coordfile.clear();
        coordfile.seekg(0, std::ios::beg);
        int index = 0;
        double x, y;
        while (getline(coordfile, line)) {
            if (!line.empty()) {
                istringstream iss(line);
                iss >> x >> y;
                cities_temp[index].setposition(x, y);
                index++;
            }
        }

        coordfile.close();

        //// NAMES ////
        ifstream namefile("../INPUT/names.dat");
        if(namefile.good()){   // if file "names.dat" is present, register names of cities
            string name;
            int counternames=0;
            while (getline(namefile, name)){
                if (!name.empty()) { counternames++; }
            }
            if(counternames!=N_cities){
                cerr << "PROBLEM: number of lines in 'names.dat' not compatible with input value." << endl;
                cerr << "Program will ignore cities names." << endl;
            }else{
                namefile.clear();
                namefile.seekg(0, std::ios::beg);
                int indexnames = 0;
                while (getline(namefile, name)) {
                    if (!name.empty()) {
                        cities_temp[indexnames].setname(name);
                        indexnames++;
                    }
                }
            }
        }
        namefile.close();
        //////////////

        cities=cities_temp;
    }

    // write cities in "../OUTPUT/cities.dat":
    if(rank==0){
        for(int i=0; i<N_cities; i++){
            coutf << cities[i].getname() << " " << cities[i].getx() << " " << cities[i].gety() << endl;
        }
    }

    coutf.close();

    ///////////// INITIALIZE POPULATION
    pop.resize(pop_size);
    for(int i=0; i<population_size; i++){
        pop[i].initialize(path_length);   // creates population whose paths are all "1, 2, ..., path_length" 
    }
    // shuffle all paths to create completely random starting population
    shuffle_all();

}

void Population :: quick_initialize(int population_size, int path_size, Random& random){

    rnd=random;

    if(population_size<1){
        cerr << "Invalid population size." << endl;
    }else{
        pop_size=population_size;
    }

    if(path_size<3){
        cerr << "Invalid path length." << endl;
    }else{
        path_length=path_size;
    }

    pop.resize(pop_size);
    for(int i=0; i<population_size; i++){
        pop[i].initialize(path_length);
    }
}

vector<City> Population::cities_on_circumference_eq(int Ncities){
    vector<City> cities;
    cities.resize(Ncities);
    for(int i=0; i<Ncities; i++){
        //double ang=rnd.Rannyu(0, 2*M_PI);
        double ang=i*(2*M_PI/Ncities);   // equally spaced and ordered
        double x=cos(ang);
        double y=sin(ang);
        cities[i].setposition(x, y);
    }
    return cities;
}

vector<City> Population::cities_on_circumference_rand(int Ncities){
    vector<City> cities;
    cities.resize(Ncities);

    vector<double> ang;
    ang.resize(Ncities);
    for(int i=0; i<Ncities; i++){
        ang[i]=rnd.Rannyu(0,2*M_PI);   // generate random angles
    }
    sort(ang.begin(), ang.end());   // sort angles so that the optimal path is 0,1,2,... (or inverse order)

    for(int i=0; i<Ncities; i++){
        double x=cos(ang[i]);
        double y=sin(ang[i]);
        cities[i].setposition(x, y);
    }

    return cities;
}

vector<City> Population::cities_inside_square(int Ncities){
    vector<City> cities;
    cities.resize(Ncities);

    for(int i=0; i<Ncities; i++){
        double x=rnd.Rannyu();
        double y=rnd.Rannyu();
        cities[i].setposition(x, y);
    }

    return cities;
}

void Population::write_all_pop(){
    ofstream coutf;
    coutf.open("../OUTPUT/population.dat", ios::trunc);
    coutf.close();

    coutf.open("../OUTPUT/population.dat", ios::app);
    if(coutf.is_open()){
        for(int i=0; i<pop_size; i++){
            pop[i].print_path(coutf, cities);
        }
    }
}

void Population::write_last_best(){
    ofstream coutf("../OUTPUT/last_best_" + std::to_string(rank) + ".dat");
    if(coutf.is_open()){
        pop[0].print_path(coutf, cities);
    }
    coutf.close();
}

void Population::finalize(){
    rnd.SaveSeed();
    cout << "Process #" << rank << ": all operations completed." << endl;
}

void Population::shuffle_all(){ //shuffles the order in which cities are visited for each single path in a population
    // to be used when trying to create a completely random population of paths
    for(int i=0; i<pop_size; i++){
        pop[i].path_shuffle(rnd);
    }
}

double Population::get_best_dist(){
    return pop[0].tot_dist(cities);
}

double Population::get_mean_dist(){
    double mean=0;
    for(int i=0; i<pop_size; i++){
        mean+=pop[i].tot_dist(cities);
    }
    return mean/pop_size;
}

double Population::get_mean_best_half(){
    double mean=0;
    int counter=0;
    for(int i=0; i<pop_size/2; i++){
        mean+=pop[i].tot_dist(cities);
        counter++;
    }
    return mean/counter;
}

void Population :: write_mean_best_half(){
    ofstream coutf;
    coutf.open("../OUTPUT/best_half_" + std::to_string(rank) + ".dat",ios::app);
    coutf << get_mean_best_half() << endl;
    coutf.close();
}

void Population :: write_best_dist(){
    ofstream coutf;
    coutf.open("../OUTPUT/best_individual_" + std::to_string(rank) + ".dat",ios::app);
    coutf << get_best_dist() << endl;
    coutf.close();
}

void Population :: write_best_path(){
    ofstream coutf;
    coutf.open("../OUTPUT/best_path_steps_" + std::to_string(rank) + ".dat",ios::app);
    pop[0].print_path_steps(coutf);
    coutf.close();
}

void Population::pop_singleswap(int a, int b){
    if(a!=0 && b!=0){
        std::swap(pop[a], pop[b]);
    }
}

bool Population::check_pop(){
    bool check=true;
    for(int i=0; i<pop_size; i++){
        bool temp=pop[i].check_start() && pop[i].check_rep();
        check=check&&temp;
    }
    return check;
}

void Population::fitness_sort() {
    sort(pop.begin(), pop.end(), [&](Path& a, Path& b) {   // ??? but it works (see lambda functions)
        return a.tot_dist(cities) < b.tot_dist(cities);
    });
}

void Population::mutation_pair(double probability){
    for(int i=0; i<pop_size; i++){
        if(rnd.Rannyu()<probability){
            int a=int(rnd.Rannyu(1, path_length));
            int b=int(rnd.Rannyu(1, path_length));
            while(a==b){
                b=int(rnd.Rannyu(1, path_length));
            }
            pop[i].path_swap(a, b);
        }
    }
}

void Population::mutation_inversion(double probability){
    for(int i=0; i<pop_size; i++){
        if(rnd.Rannyu()<probability){
            int begin=int(rnd.Rannyu(0,path_length-1));
            int end=path_length;
            if(path_length-begin>2){
                end=int(rnd.Rannyu(begin+2,path_length+1));
            }
            pop[i].partial_inversion(begin, end);
        }
    }
}

void Population::mutation_permutation(double probability){
}

void Population::mutation_shift(double probability){
    for(int i=0; i<pop_size; i++){
        if(rnd.Rannyu()<probability){
            int end=int(rnd.Rannyu(0,path_length));
            int begin=int(rnd.Rannyu(1,end));
            int step=int(rnd.Rannyu(1, path_length-end));
            pop[i].shift_forward(begin, end, step);
        }
    }
}

void Population::mutate(){
    mutation_pair(mutation_prob);
    mutation_inversion(mutation_prob);
    mutation_shift(mutation_prob);
}

int Population::select(double exponent){   // if population is ordered from most to least fit, p must be set > 1 --> higher chance of selecting fit individuals
    return int(pop_size*pow(rnd.Rannyu(),exponent));
}

void Population::single_crossover(int index1, int index2, int cut_index){
    vector<int> tempv1;
    tempv1.resize(path_length-cut_index);
    vector<int> tempv2;
    tempv2.resize(path_length-cut_index);
    for(int i=cut_index; i<path_length; i++){
        tempv1[i-cut_index]=pop[index1].get_step(i);
        tempv2[i-cut_index]=pop[index2].get_step(i);
    }
    vector<int> sonv1=pop[index2].sort_bycomp(tempv1);
    vector<int> sonv2=pop[index1].sort_bycomp(tempv2);
    for(int i=cut_index; i<path_length; i++){
        pop[index1].set_step(i, sonv1[i-cut_index]);
        pop[index2].set_step(i, sonv2[i-cut_index]);
    }
}

int Population::get_pop_size(){
    return pop_size;
}

int Population::get_path_length(){
    return path_length;
}

int Population::get_N_generations(){
    return N_generations;
}

void Population::set_path(int index, Path& path){
    pop[index]=path;
}

Path Population::get_path(int index){
    return pop[index];
}

vector<Path> Population::get_population(){
    return pop;
}

Random Population::get_rnd(){
    return rnd;
}

void Population::crossover(double exponent){

    // generate a new Population in order to store the new generation
    Population newgen;
    newgen.quick_initialize(pop_size, path_length, rnd);
    newgen.shuffle_all();

    for(int i=0; i<pop_size-1; i=i+2){  // if pop_size is an odd number, the last individual is a randomly generated one
        // select a mother and a father from the old generation
        int father=select(exponent);
        int mother=select(exponent);
        while(mother==father){
            mother=select();
        }
        // copy mother and father to new population
        newgen.set_path(i, pop[father]);
        newgen.set_path(i+1, pop[mother]);
        // perform crossover between mother and father
        newgen.single_crossover(i, i+1, int(rnd.Rannyu(1,path_length)));
        // a new generation is created
    }

    // replace old generation with new generation
    for(int i=0; i<pop_size; i++){
        pop[i]=newgen.get_path(i);
    }
}

/*
void Population::crossover(){

    // generate a new Population in order to store the new generation
    vector<Path> newgen;
    newgen.resize(pop_size);
    for(int i=0; i<pop_size; i++){
        Path newpath;
        newpath.initialize(path_length);
        if(i==pop_size){
            newpath.path_shuffle(rnd);
        }
        newgen[i]=newpath;
    }

    for(int i=0; i<pop_size-1; i=i+2){  // if pop_size is an odd number, the last individual is a randomly generated one
        // select a mother and a father from the old generation
        cout << i << " " << i+1 << endl;
        int father=select();
        int mother=select();
        while(mother==father){
            mother=select();
        }
        // copy mother and father to new population
        newgen[i]= pop[father];
        newgen[i+1]= pop[mother];
        // perform crossover between mother and father
        newgen.single_crossover(i, i+1, int(rnd.Rannyu(1,path_length)));
        // a new generation is created
    }

    // replace old generation with new generation
    for(int i=0; i<pop_size; i++){
        pop[i]=newgen.get_path(i);
    }
}
*/