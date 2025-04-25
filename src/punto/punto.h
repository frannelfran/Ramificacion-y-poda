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

    // Getters
    inline int getDimension() const { return punto_.size(); }
    inline double getCoordenada(int i) const { return punto_[i]; }

    // Sobrecarga del operador de salida
    friend ostream& operator<<(ostream& os, const Punto& punto);
    

  private:
    vector<double> punto_; // Vector que representa el punto en el espacio
};

#endif