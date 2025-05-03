/**
 * @class Clase para representar un punto en el espacio
 */

#ifndef PUNTO_H
#define PUNTO_H

#include <iostream>
#include <vector>
#include <cmath> // Para calcular la distancia

using namespace std;

class Punto {
  public:
    // Constructor
    Punto() {}
    Punto(const int& id, const vector<double>& punto) : identificador_(id), punto_(punto) {}
    Punto(const vector<double>& punto) : identificador_(-1), punto_(punto) {}

    // Métodos de la clase
    double calcularDistancia(const Punto& otro) const;

    // Getters
    inline int getDimension() const { return punto_.size(); }
    inline double getCoordenada(int i) const { return punto_[i]; }
    inline int getIndice() const { return identificador_; }

    // Sobrecarga del operador de salida
    friend ostream& operator<<(ostream& os, const Punto& punto);
    friend inline bool operator==(const Punto& p1, const Punto& p2) {
      return p1.punto_ == p2.punto_;
    }
    
  private:
    int identificador_; // Identificador del punto
    vector<double> punto_; // Vector que representa el punto en el espacio
};

#endif