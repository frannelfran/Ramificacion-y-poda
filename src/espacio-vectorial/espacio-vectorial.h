/**
 * @class Clase para representar un espacio vectorial
 */

#ifndef ESPACIO_VECTORIAL_H
#define ESPACIO_VECTORIAL_H

#include "../punto/punto.h"

class EspacioVectorial {
  public:
    // Constructor
    EspacioVectorial() {}

    // Métodos de la clase
    inline void agregarPunto(const Punto& punto) { espacioVectorial_.push_back(punto); }
    Punto calcularCentroGravedad() const;

    // Getters
    inline int getDimension() const { return espacioVectorial_.size(); }

  private:
    vector<Punto> espacioVectorial_; // Vector de puntos en el espacio vectorial
};

#endif