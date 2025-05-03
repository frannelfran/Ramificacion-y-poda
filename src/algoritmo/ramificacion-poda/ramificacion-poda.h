/**
 * @class Clase que representa el algoritmo de Ramificación y Poda
 */

#ifndef C_RamificacionPoda_H
#define C_RamificacionPoda_H

#include "../algoritmo.h"
#include <stack>
#include <set>
#include <algorithm>
#include <utility>

// Estructura para representar un nodo en el árbol de búsqueda
struct Nodo {
  set<int> seleccionados;  // Conjunto de puntos seleccionados
  int nivel;               // Nivel en el árbol (número de puntos seleccionados)
  
  Nodo(const set<int>& sel, int lvl) : seleccionados(sel), nivel(lvl) {}
};

class RamificacionPoda : public Algoritmo {
  public:
    // Constructor de la clase
    RamificacionPoda() : Algoritmo() {}
    
    // Destructor de la clase
    ~RamificacionPoda() {}

    // Métodos heredados
    void ejecutar() override;
    void mostrarResultados() override;

    // Setters
    inline void setAlgoritmo(Algoritmo* algoritmo) { algoritmo_ = algoritmo; }

  private:
    // Métodos privados
    void preprocesamiento();
    double calcularSumaDistancias(const set<int>& seleccionados) const;
    double calcularCotaSuperior(const Nodo& nodo, vector<pair<double, int>>& valoresZ) const;
    bool aplicarReglaDominancia(const Nodo& nodo) const;
    void generarHijos(const Nodo& nodo, double cotaInferior, stack<Nodo>& pila, const vector<pair<double, int>>& valoresZ);

    // Atributos clave
    vector<vector<double>> matrizDistancias_;          // Matriz de distancias entre todos los puntos
    vector<double> sumatorioDistancias_;               // Suma de distancias para cada punto
    vector<vector<int>> vecinosOrdenados_;             // Vecinos ordenados por distancia para cada punto
    Algoritmo* algoritmo_;                             // Algoritmo utilizado para obtener cota inicial
    double mejorSolucion_;                             // Mejor solución encontrada hasta el momento
    set<int> mejorConjunto_;                           // Mejor conjunto de puntos encontrado
    int nodosGenerados_;                               // Contador de nodos generados
};

#endif