#include "busqueda-local.h"

/**
 * @brief Método para calcular el delta Z de la mejora
 * @param i Índice del primer punto
 * @param j Índice del segundo punto
 * @return void
 */
double BusquedaLocal::calcularDeltaZ(int i, int j) const {
  const Punto& punto_i = subconjunto_->operator[](i);
  const Punto& punto_j = espacioVectorial_[j];
  return punto_i.calcularDistancia(punto_j);
}


/**
 * @brief Método que realiza las búsquedas locales
 * @return void
 */
void BusquedaLocal::swap1LocalSearch() {
  // Inicializar variables
  double mejor_delta = 0;
  pair<int, int> mejor_swap = {-1, -1}; // Inicializa el mejor swap como ninguno
  
  vector<pair<int, int>> realizados; // Para evitar swaps repetidos
  
  do {
    mejor_delta = 0;
    mejor_swap = {-1, -1}; // Reinicia el mejor swap

    for (size_t i = 0; i < subconjunto_->getDimension(); ++i) {
      for (size_t j = 0; j < espacioVectorial_.getDimension(); ++j) {
        if (subconjunto_->operator[](i) != espacioVectorial_[j] && 
            find(realizados.begin(), realizados.end(), make_pair(static_cast<int>(i), static_cast<int>(j))) == realizados.end()) {
          double delta_z = calcularDeltaZ(i, j); // Mejora de intercambiar i por j

          if (delta_z > mejor_delta) {
            mejor_delta = delta_z;
            mejor_swap = {i, j};
          }
        }
      }
    }
    if (mejor_delta > 0) {
      // Aplicar el mejor swap
      realizados.push_back(mejor_swap); // Registrar el swap realizado
      swap(subconjunto_->operator[](mejor_swap.first), espacioVectorial_[mejor_swap.second]);
    }
  } while (mejor_delta > 0);
}

/**
 * @brief Método para realizar la búsqueda local 1-swap Local Search
 * @return void
*/
void BusquedaLocal::mejorarRutas() {
  // Realizar la búsqueda local
  swap1LocalSearch();
}
//Entrada: solución inicial S (conjunto de m elementos), conjunto total U
//Salida: solución S mejorada (óptimo local)
//
//Repetir:
//    mejor_delta ← 0
//    mejor_swap ← (ninguno)
//
//    Para cada elemento i ∈ S:
//        Para cada elemento j ∈ (U \ S):   // elementos que no están en S
//            calcular Δz = mejora de intercambiar i por j
//
//            Si Δz > mejor_delta:
//                mejor_delta ← Δz
//                mejor_swap ← (i, j)
//
//    Si mejor_delta > 0:
//        aplicar mejor_swap: 
//            eliminar i de S, añadir j a S
//    Sino:
//        terminar (óptimo local alcanzado)
//
//Devolver S