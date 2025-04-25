#include "espacio-vectorial.h"

/**
 * @brief Método para calcular el centro de gravedad del espacio vectorial
 * @details Se suman las coordenadas de todos los puntos y se divide por el número de puntos
 * @return Punto centro de gravedad
 */
Punto EspacioVectorial::calcularCentroGravedad() const {
  vector<double> centroGravedad(espacioVectorial_[0].getDimension(), 0.0);
  for (const auto& punto : espacioVectorial_) {
    for (size_t i = 0; i < centroGravedad.size(); ++i) {
      centroGravedad[i] += punto.getCoordenada(i);
    }
  }
  // Dividir por el número de puntos
  for (size_t i = 0; i < centroGravedad.size(); ++i) {
    centroGravedad[i] /= espacioVectorial_.size();
  }
  return Punto(centroGravedad);
}