/**
 * @struct Struct para representar el contenido del fichero
 */

#ifndef DATO_H
#define DATO_H

#include "../espacio-vectorial/espacio-vectorial.h"

struct Dato {
  string nombreFichero; // Nombre del fichero

  int numPuntos; // Número de puntos
  int tamanio; // Dimensión de los puntos

  EspacioVectorial espacioVectorial; // Espacio vectorial
  double tiempoCPU; // Tiempo de CPU
};

#endif