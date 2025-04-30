#include "punto.h"

/**
 * @brief Método para calcular la distancia entre 2 puntos
 * @param otro Otro punto
 * @details Para calcular la distancia entre 2 puntos, se utiliza la fórmula de la distancia euclidiana
 * @return Distancia entre los 2 puntos
 */
double Punto::calcularDistancia(const Punto& otro) const {
  if (punto_.size() != otro.punto_.size()) {
    throw invalid_argument("Los puntos deben tener la misma dimensión");
  }
  double suma = 0;
  for (size_t i = 0; i < punto_.size(); ++i) {
    suma += pow(punto_[i] - otro.punto_[i], 2);
  }
  return sqrt(suma);
}

/**
 * @overload Sobrecarga del operador de salida
 */
ostream& operator<<(ostream& os, const Punto& punto) {
  os << punto.identificador_;
  return os;
}