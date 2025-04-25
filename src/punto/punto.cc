#include "punto.h"

/**
 * @overload Sobrecarga del operador de salida
 */
ostream& operator<<(ostream& os, const Punto& punto) {
  os << "(";
  for (size_t i = 0; i < punto.punto_.size(); ++i) {
    os << punto.punto_[i];
    if (i < punto.punto_.size() - 1) {
      os << ", ";
    }
  }
  os << ")";
  return os;
}