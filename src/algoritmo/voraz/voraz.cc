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
  auto start = chrono::high_resolution_clock::now();
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
    centroGravedad = subconjunto.calcularCentroGravedad();

  } while (subconjunto.getDimension() != numPuntosAlejados_);
  auto end = chrono::high_resolution_clock::now();
  resultado.tiempoCPU = chrono::duration<double>(end - start).count(); // Tiempo en segundos

  resultado.espacioVectorial = subconjunto;
  // Agrego el resultado al vector de resultados
  resultados_.push_back(resultado);
}

/**
 * @brief Método para mostrar los resultados del algoritmo Voraz
 * @return void
 */
void Voraz::mostrarResultados() {
  static bool cabeceraMostrada = false;

  if (!cabeceraMostrada) {
    // Cabecera
    cout << "------------------------------------------------------------------" << endl;
    cout << left 
    << setw(20) << "Problema" 
    << setw(6) << "n" 
    << setw(6) << "K" 
    << setw(6) << "m" 
    << setw(12) << "z"
    << setw(12) << "Tiempo CPU" 
    << setw(12) << "S"
    << endl;
    cout << "------------------------------------------------------------------" << endl;
    cabeceraMostrada = true;
  }

  // Muestro los resultados
  for (auto& resultado : resultados_) {
    double distancia = calcularDistancia(resultado.espacioVectorial);
    cout << left 
    << setw(20) << resultado.nombreFichero
    << setw(6) << resultado.numPuntos
    << setw(6) << resultado.tamanio
    << setw(6) << resultado.espacioVectorial.getDimension()
    << setw(12) << fixed << setprecision(2) << distancia
    << setw(12) << fixed << setprecision(5) << resultado.tiempoCPU
    << setw(0) << resultado.espacioVectorial
    << endl;
  }
  cout << "------------------------------------------------------------------" << endl;
}