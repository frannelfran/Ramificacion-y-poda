/**
 * @class Clase para repressentar un algoritmo en el sistema
 */

#ifndef C_Algoritmo_H
#define C_Algoritmo_H

#include "../dato/dato.h"
#include <iomanip>
#include <chrono> // Para medir el tiempo de ejecución

class Algoritmo {
  public:
    // Constructor de la clase
    Algoritmo() : dato_(nullptr) {}
    Algoritmo(Dato& dato) : dato_(&dato) {}

    // Destructor de la clase
    ~Algoritmo() {}

    // Métodos de la clase
    virtual void ejecutar() = 0;
    virtual void mostrarResultados() = 0;

    double calcularDistancia(const EspacioVectorial& espacio);

    // Setters
    inline void setDato(Dato& dato) { dato_ = &dato; }
    inline void setNumPuntosAlejados(const int numPuntosAlejados) { numPuntosAlejados_ = numPuntosAlejados; }

  protected:
    // Datos importantes para el algoritmo
    Dato* dato_;
    vector<Dato> resultados_;
    int numPuntosAlejados_;
};

#endif