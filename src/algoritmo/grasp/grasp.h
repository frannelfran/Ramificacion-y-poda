/**
 * @class Clase que representa un algoritmo GRASP en el sistema
 */
#ifndef C_Grasp_H
#define C_Grasp_H

#include "../algoritmo.h"
#include "busqueda-local/busqueda-local.h"
#include <random>

class Grasp : public Algoritmo {
  public:
    // Constructores de la clase
    Grasp() : Algoritmo() {} // Constructor por defecto

    // Métodos de la clase
    void ejecutar() override;
    void mostrarResultados() override;

    // Setters
    inline void setMejoresPuntos(const int mejoresPuntos) { mejoresPuntos_ = mejoresPuntos; }
    
  private:
    Punto* puntoMasAlejado(EspacioVectorial& espacio, const Punto& centroGravedad) const;
    // Atributos
    int mejoresPuntos_ = 2;
};

#endif