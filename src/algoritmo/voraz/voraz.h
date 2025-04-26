/**
 * @class Clase para representar un algoritmo Voraz en el sistema
 */

#ifndef C_Voraz_H
#define C_Voraz_H

#include "../algoritmo.h"

class Voraz : public Algoritmo {
  public:
    Voraz() : Algoritmo() {} // Constructor por defecto
    Voraz(Dato& dato, const int numPuntosAlejados) : Algoritmo(dato), numPuntosAlejados_(numPuntosAlejados)  {} // Constructor de la clase

    // Métodos de la clase
    void ejecutar() override;
    void mostrarResultados() override;

    // Setters
    inline void setNumPuntosAlejados(const int numPuntosAlejados) { numPuntosAlejados_ = numPuntosAlejados; }

  private:
    Punto* puntoMasAlejado(EspacioVectorial& espacio, const Punto& centroGravedad) const;
    double calcularDistancia(const EspacioVectorial& espacio);
    // Atributos
    int numPuntosAlejados_; // Número de puntos alejados
};

#endif