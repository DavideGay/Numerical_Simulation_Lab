#ifndef __walker__
#define __walker__

#include <cmath>
#include "random.h"

class Random;

class RandomWalk_3D {

    private:

    protected:
    double x,y,z;
    //Random rnd;
    // stores the current position

    public:
    // Default constructor (and destructor)
    RandomWalk_3D();
    ~RandomWalk_3D();

    // Process a single step in a cubic lattice
    void single_step_discr(Random& );
    // Process multiple steps in a cubic lattice
    void single_step_cont(Random& );
    // Return x coordinate
    double get_x();
    // Return y coordinate
    double get_y();
    // Return z coordinate
    double get_z();
    // Return squared distance from the origin
    double get_r2();
    // Resets the Random Walk: sets position to origin
    void reset_rw();

};



#endif //__walker__