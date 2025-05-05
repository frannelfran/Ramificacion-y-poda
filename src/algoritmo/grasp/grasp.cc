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
    if (puntosMasAlejados.size() == mejoresPuntos_) break;
    if (distancia > maxDistancia) {
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

  resultado.espacioVectorial = subconjunto;
  // Realizo la búsqueda local
  BusquedaLocal busquedaLocal(dato_->espacioVectorial, &resultado.espacioVectorial);
  busquedaLocal.mejorarRutas();
  auto end = chrono::high_resolution_clock::now();
  resultado.tiempoCPU = chrono::duration<double>(end - start).count(); // Tiempo en segundos

  // Agrego el resultado al vector de resultados
  resultados_.push_back(resultado);
}

/**
 * @brief Método para mostrar los resultados del algoritmo GRASP
 * @return void
 */
void Grasp::mostrarResultados() {
  static bool cabeceraMostrada = false;

  if (!cabeceraMostrada) {
    // Cabecera
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << left 
    << setw(20) << "Problema" 
    << setw(6) << "n" 
    << setw(6) << "K" 
    << setw(6) << "m" 
    << setw(10) << "|LRC|"
    << setw(12) << "z"
    << setw(12) << "Tiempo CPU" 
    << setw(6) << "S"
    << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cabeceraMostrada = true;
  }

  // Muestro los resultados
  int LRC = 2;
  for (auto& resultado : resultados_) {
    if (LRC > 3) LRC = 2;
    cout << left 
    << setw(20) << resultado.nombreFichero 
    << setw(6) << resultado.numPuntos
    << setw(6) << resultado.tamanio
    << setw(6) << resultado.espacioVectorial.getDimension()
    << setw(10) << LRC++
    << setw(12) << fixed << setprecision(2) << calcularDistancia(resultado.espacioVectorial)
    << setw(12) << fixed << setprecision(5) << resultado.tiempoCPU
    << setw(0) << resultado.espacioVectorial
    << endl;
  }
  cout << "-----------------------------------------------------------------------------" << endl;
}