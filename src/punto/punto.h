/**
 * @class Clase para representar un punto en el espacio
 */

#ifndef PUNTO_H
#define PUNTO_H

#include <iostream>
#include <vector>

using namespace std;

class Punto {
  public:
    // Constructor
    Punto() {}
    Punto(const vector<double>& punto) : punto_(punto) {}
    

  private:
    vector<double> punto_; // Vector que representa el punto en el espacio
};

#endif