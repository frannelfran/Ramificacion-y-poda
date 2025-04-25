/**
 * @class Clase para representar un algoritmo Voraz en el sistema
 */

#ifndef C_Voraz_H
#define C_Voraz_H

#include "../algoritmo.h"

class Voraz : public Algoritmo {
  public:
    Voraz() : Algoritmo() {} // Constructor por defecto
    Voraz(Dato& datos) : Algoritmo(datos) {} // Constructor de la clase

    // Métodos de la clase
    void ejecutar() override;
    void mostrarResultados() override {}

  private:
    inline Punto calcularCentroGravedad() const { return dato_->espacioVectorial.calcularCentroGravedad(); }
    Punto& puntoMasAlejado(const Punto& centroGravedad) const;

};

#endif