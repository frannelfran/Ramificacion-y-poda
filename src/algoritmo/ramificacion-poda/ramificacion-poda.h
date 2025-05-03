/**
 * @class Clase que representa el algoritmo de Ramificación y Poda
 */

#ifndef C_RamificacionPoda_H
#define C_RamificacionPoda_H

#include "../algoritmo.h"

class RamificacionPoda : public Algoritmo {
  public:
    // Constructor de la clase
    RamificacionPoda() : Algoritmo() {}
    
    // Destructor de la clase
    ~RamificacionPoda() {}

    // Métodos heredados
    void ejecutar() override;
    void mostrarResultados() override {}

    // Setters
    inline void setAlgoritmo(Algoritmo* algoritmo) { algoritmo_ = algoritmo; }

  private:
    // Métodos
    void preprocesamiento();

    // Atributos clave
    vector<vector<double>> matrizDistancias_; // Matriz de distancias entre todos los puntos
    vector<double> sumatorioDistancias_; // Suma de distancias para cada punto
    Algoritmo* algoritmo_; // Algoritmo utilizado

};

#endif