/**
 * @class Clase para representar un espacio vectorial
 */

#ifndef ESPACIO_VECTORIAL_H
#define ESPACIO_VECTORIAL_H

#include "../punto/punto.h"

class EspacioVectorial {
  public:
    // Constructor
    EspacioVectorial() {}

    // Métodos de la clase
    inline void agregarPunto(const Punto& punto) { espacioVectorial_.push_back(punto); }
    Punto calcularCentroGravedad() const;
    void eliminarPunto(const Punto& punto);

    // Getters
    inline int getDimension() const { return espacioVectorial_.size(); }

    // Sobrecarga de operadores
    Punto& operator[](int i) { return espacioVectorial_[i]; }
    const Punto& operator[](int i) const { return espacioVectorial_[i]; }

    friend ostream& operator<<(ostream& os, const EspacioVectorial& espacio) {
      for (const auto& punto : espacio.espacioVectorial_) {
        os << punto << " ";
      }
      return os;
    }

  private:
    vector<Punto> espacioVectorial_; // Vector de puntos en el espacio vectorial
};

#endif