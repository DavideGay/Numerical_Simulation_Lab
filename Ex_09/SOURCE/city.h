#ifndef __City__
#define __City__

#include "random.h"
#include <vector>
#include <string>

using namespace std;

class City {

private:
  //const int _ndim = 2; // Dimensionality of the system
  double x;
  double y;
  std::string name;

public: // Function declarations
  void initialize();
  void setposition(double, double);
  void setname(std::string newname);
  double getx();
  double gety();
  std::string getname();

};

#endif // __City__