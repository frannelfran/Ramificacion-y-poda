/**
 * @class Clase para realizar las busquedas locales
 */

#ifndef BUSQUEDA_LOCAL_H
#define BUSQUEDA_LOCAL_H

#include "../grasp.h"

class BusquedaLocal {
  public:
    // Constructor
    BusquedaLocal(const EspacioVectorial& espacioVectorial, EspacioVectorial* subconjunto)
      : espacioVectorial_(espacioVectorial), subconjunto_(subconjunto) {}

    // Método para realizar la búsqueda local
    void mejorarRutas();
    double calcularDeltaZ(int i, int j) const;

    // Búsqueda local
    void swap1LocalSearch();
    
  private:
    EspacioVectorial* subconjunto_;
    EspacioVectorial espacioVectorial_;
};

#endif