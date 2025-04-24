/**
 * @struct Struct para representar el contenido del fichero
 */

#ifndef DATO_H
#define DATO_H

#include "../espacio-vectorial/espacio-vectorial.h"

struct Dato {
  // Valores para definir el número de puntos y el tamaño del espacio vectorial
  int numPuntos;
  int dimension;

  // Espacio vectorial
  EspacioVectorial espacioVectorial;
};

#endif