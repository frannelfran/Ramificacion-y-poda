#include "voraz.h"

/**
 * @brief Método para calcular el punto más alejado del centro de gravedad
 * @param centroGravedad Centro de gravedad
 * @return Punto más alejado del centro de gravedad
 */
Punto& Voraz::puntoMasAlejado(const Punto& centroGravedad) const {
  double maxDistancia = -1;
  Punto* puntoMasAlejado = nullptr;

  for (size_t i = 0; i < dato_->espacioVectorial.getDimension(); ++i) {
    double distancia = dato_->espacioVectorial[i].calcularDistancia(centroGravedad);
    if (distancia > maxDistancia) {
      maxDistancia = distancia;
      puntoMasAlejado = &dato_->espacioVectorial[i];
    }
  }
  return *puntoMasAlejado;
}