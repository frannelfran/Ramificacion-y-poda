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

  private:
    int dimension_; // Dimensión del espacio vectorial
    vector<Punto> espacioVectorial_; // Vector de puntos en el espacio vectorial
};

#endif