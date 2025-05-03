#include "ramificacion-poda.h"

/**
 * @brief Método para el preprocesamiento de datos
 * @return void
 */
void RamificacionPoda::preprocesamiento() {
  // Preprocesamiento de datos
  matrizDistancias_.resize(dato_->espacioVectorial.getDimension(), vector<double>(dato_->espacioVectorial.getDimension(), 0.0));
  sumatorioDistancias_.resize(dato_->espacioVectorial.getDimension(), 0.0);

  EspacioVectorial espacioVectorial = dato_->espacioVectorial;

  // Calcular la matriz de distancias entre todos los puntos
  for (size_t i = 0; i < espacioVectorial.getDimension(); ++i) {
    for (size_t j = 0; j < espacioVectorial.getDimension(); ++j) {
      if (i != j) {
        matrizDistancias_[i][j] = espacioVectorial[i].calcularDistancia(espacioVectorial[j]);
        sumatorioDistancias_[i] += matrizDistancias_[i][j];
      }
    }
  }
}

/**
 * @brief Método para ejecutar el algoritmo de Ramificación y Poda
 * @return void
 */
void RamificacionPoda::ejecutar() {
  // Preprocesamiento de datos
  preprocesamiento();

  // Implementación del algoritmo de Ramificación y Poda
  // ...
}