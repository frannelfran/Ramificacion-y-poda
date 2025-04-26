#include "grasp.h"

/**
 * @brief Método para calcular el punto más alejado del centro de gravedad
 * @param espacio Espacio vectorial
 * @param centroGravedad Centro de gravedad
 * @details Se escoge al azar entre los n puntos más alejados del centro de gravedas
 * @return Punto más alejado del centro de gravedad
 */
Punto* Grasp::puntoMasAlejado(EspacioVectorial& espacio, const Punto& centroGravedad) const {
  double maxDistancia = -1;
  vector<Punto*> puntosMasAlejados;

  for (size_t i = 0; i < espacio.getDimension(); ++i) {
    double distancia = espacio[i].calcularDistancia(centroGravedad);
    if (distancia > maxDistancia && puntosMasAlejados.size() < mejoresPuntos_) {
      Punto* punto = &espacio[i];
      maxDistancia = distancia;
      // Compruebo si el punto ya está en la lista de puntos más alejados
      if (find(puntosMasAlejados.begin(), puntosMasAlejados.end(), punto) == puntosMasAlejados.end()) {
        puntosMasAlejados.push_back(punto);
      }
    }
  }

  // Escoger un punto aleatorio entre los n puntos más alejados
  static std::random_device rd; // Obtiene un número aleatorio
  static std::mt19937 gen(rd()); // Generador de números aleatorios
  std::uniform_int_distribution<> dis(0, puntosMasAlejados.size() - 1);
  
  return puntosMasAlejados[dis(gen)];
}

/**
 * @brief Método para ejecutar el algoritmo GRASP	
 * @return void
 */
void Grasp::ejecutar() {
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

