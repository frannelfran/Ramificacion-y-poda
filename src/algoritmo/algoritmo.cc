#include "algoritmo.h"

/**
 * @brief Método para sumar las distancia de los puntos seleccionados
 * @param espacio Espacio vectorial
 * @return Distancia total
 */
double Algoritmo::calcularDistancia(const EspacioVectorial& espacio) {
  double distancia = 0.0;
  size_t n = espacio.getDimension();
  for (size_t i = 0; i < n - 1; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      distancia += espacio[i].calcularDistancia(espacio[j]);
    }
  }
  return distancia;
}