#include "voraz.h"

/**
 * @brief Método para calcular el punto más alejado del centro de gravedad
 * @param espacio Espacio vectorial
 * @param centroGravedad Centro de gravedad
 * @return Punto más alejado del centro de gravedad
 */
Punto* Voraz::puntoMasAlejado(EspacioVectorial& espacio, const Punto& centroGravedad) const {
  double maxDistancia = -1;
  Punto* puntoMasAlejado = nullptr;

  for (size_t i = 0; i < espacio.getDimension(); ++i) {
    double distancia = espacio[i].calcularDistancia(centroGravedad);
    if (distancia > maxDistancia) {
      maxDistancia = distancia;
      puntoMasAlejado = &espacio[i];
    }
  }
  return puntoMasAlejado;
}

/**
 * @brief Método para ejecutar el algoritmo Voraz
 */
void Voraz::ejecutar() {
  Dato resultado = *dato_;
  EspacioVectorial subconjunto;

  // Obtengo el centro de gravedad
  Punto centroGravedad = dato_->espacioVectorial.calcularCentroGravedad();

  do {
    // Calculo el punto más alejado del centro de gravedad
    Punto* puntoMasAlejado = this->puntoMasAlejado(resultado.espacioVectorial, centroGravedad);

    // Agrego el punto más alejado al resultado
    subconjunto.agregarPunto(*puntoMasAlejado);

    // Elimino el punto más alejado del espacio vectorial
    resultado.espacioVectorial.eliminarPunto(*puntoMasAlejado);

    // Calculo el nuevo centro de gravedad
    centroGravedad = resultado.espacioVectorial.calcularCentroGravedad();

  } while (subconjunto.getDimension() != numPuntosAlejados_);

  resultado.espacioVectorial = subconjunto;
  // Agrego el resultado al vector de resultados
  resultados_.push_back(resultado);
}