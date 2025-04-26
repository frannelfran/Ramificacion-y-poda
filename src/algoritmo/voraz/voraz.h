/**
 * @class Clase para representar un algoritmo Voraz en el sistema
 */

#ifndef C_Voraz_H
#define C_Voraz_H

#include "../algoritmo.h"

class Voraz : public Algoritmo {
  public:
    Voraz() : Algoritmo() {} // Constructor por defecto

    // Métodos de la clase
    void ejecutar() override;
    void mostrarResultados() override;

  private:
    Punto* puntoMasAlejado(EspacioVectorial& espacio, const Punto& centroGravedad) const;
    double calcularDistancia(const EspacioVectorial& espacio);
};

#endif