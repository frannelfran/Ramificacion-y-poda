#include "espacio-vectorial.h"

/**
 * @brief Método para calcular el centro de gravedad del espacio vectorial
 * @details Se suman las coordenadas de todos los puntos y se divide por el número de puntos
 * @return Punto centro de gravedad
 */
Punto EspacioVectorial::calcularCentroGravedad() const {
  vector<double> centroGravedad(espacioVectorial_[0].getDimension(), 0.0);
  for (size_t i = 0; i < espacioVectorial_.size(); ++i) {
    for (size_t j = 0; j < espacioVectorial_[i].getDimension(); ++j) {
      centroGravedad[j] += espacioVectorial_[i].getCoordenada(j);
    }
  }
  for (size_t j = 0; j < centroGravedad.size(); ++j) {
    centroGravedad[j] /= espacioVectorial_.size();
  }
  return Punto(centroGravedad);
}