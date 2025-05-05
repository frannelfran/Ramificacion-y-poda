/**
 * @class Clase que representa un algoritmo de Ramificación y Poda en el sistema
 */

#ifndef C_RamificacionPoda_H
#define C_RamificacionPoda_H

#include "../algoritmo.h"
#include "../grasp/grasp.h"
#include <stack>
#include <algorithm>
#include <set>
#include <vector>
#include <limits>
#include <utility>

class RamificacionPoda : public Algoritmo {
  public:
    // Constructor por defecto
    RamificacionPoda() : Algoritmo() {}

    // Métodos
    void ejecutar() override;
    void mostrarResultados() override;

  private:
    // Métodos auxiliares
    void calcularPrecomputaciones();
    double calcularSumaParejas(const std::set<int>& seleccion) const;
    double calcularSumaSelVNoSel(int v, const std::set<int>& seleccion) const;
    
    // Atributos para precomputaciones
    std::vector<double> distanciasTotal;
    std::vector<std::vector<int>> indicesOrdenados;
    
    // Contador de nodos
    size_t nodosGenerados_;
};

#endif