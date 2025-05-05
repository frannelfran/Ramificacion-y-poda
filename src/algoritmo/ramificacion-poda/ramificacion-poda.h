/**
 * @class Clase que representa un algoritmo de Ramificación y Poda en el sistema
 */

#ifndef C_RamificacionPoda_H
#define C_RamificacionPoda_H

#include "../algoritmo.h"
#include "../grasp/grasp.h"
#include <stack>
#include <queue>
#include <algorithm>
#include <set>
#include <vector>
#include <limits>
#include <utility>

// Estructura para representar un nodo en el árbol de búsqueda
struct Nodo {
  std::set<int> seleccion;        // Puntos seleccionados
  double valor;                   // Valor objetivo actual
  double cota;                    // Cota superior
  int nivel;                      // Nivel en el árbol (número de puntos seleccionados)
  
  Nodo(const std::set<int>& s, double v, double c, int n) 
    : seleccion(s), valor(v), cota(c), nivel(n) {}
  
  // Operador para la cola de prioridad (ordenar por cota de mayor a menor)
  bool operator<(const Nodo& otro) const {
    return cota < otro.cota; // Para que la cola de prioridad ordene por mayor cota
  }
};

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
    double calcularCotaSuperior(const std::set<int>& seleccion, int nivel) const;
    void establecerCotaInicial();
    
    // Implementaciones de estrategias de poda
    void ejecutarConPila();
    void ejecutarConPrioridad();
    
    // Atributos para precomputaciones
    std::vector<double> distanciasTotal;
    std::vector<std::vector<int>> indicesOrdenados;
    
    // Contadores para estadísticas
    size_t nodosGenerados_;
    size_t nodosPodados_;
    
    // Mejor solución encontrada
    std::set<int> mejorSeleccion_;
    double mejorValor_;
};

#endif