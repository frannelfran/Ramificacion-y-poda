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

/**
 * @brief Método para ejecutar el algoritmo Voraz
 */
void Voraz::ejecutar() {
  Dato resultado = *dato_;
  EspacioVectorial espacioVectorial = resultado.espacioVectorial;

  // Obtengo el centro de gravedad
  Punto centroGravedad = dato_->espacioVectorial.calcularCentroGravedad();

  do {
    // Calculo el punto más alejado del centro de gravedad
    Punto& puntoMasAlejado = this->puntoMasAlejado(centroGravedad);

    // Agrego el punto más alejado al resultado
    resultado.espacioVectorial.agregarPunto(puntoMasAlejado);

    // Elimino el punto más alejado del espacio vectorial
    resultado.espacioVectorial.eliminarPunto(puntoMasAlejado);

    // Calculo el nuevo centro de gravedad
    centroGravedad = espacioVectorial.calcularCentroGravedad();

  } while (resultado.espacioVectorial.getDimension() != numPuntosAlejados_);
}