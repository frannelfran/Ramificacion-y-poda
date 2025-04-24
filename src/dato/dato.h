/**
 * @struct Struct para representar el contenido del fichero
 */

#ifndef DATO_H
#define DATO_H

#include "../espacio-vectorial/espacio-vectorial.h"

struct Dato {
  string nombreFichero; // Nombre del fichero

  // Espacio vectorial
  EspacioVectorial espacioVectorial;
};

#endif