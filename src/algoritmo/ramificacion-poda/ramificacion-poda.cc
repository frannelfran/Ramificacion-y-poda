#include "ramificacion-poda.h"

/**
 * @brief Método para calcular precomputaciones necesarias para el algoritmo
 * @return void
 */
void RamificacionPoda::calcularPrecomputaciones() {
  const size_t n = dato_->espacioVectorial.getDimension();
  distanciasTotal.resize(n, 0.0);
  indicesOrdenados.resize(n);
  
  // Calcular la distancia total para cada punto
  for (size_t v = 0; v < n; ++v) {
    for (size_t u = 0; u < n; ++u) {
      if (v != u) {
        double dist = dato_->espacioVectorial[v].calcularDistancia(dato_->espacioVectorial[u]);
        distanciasTotal[v] += dist;
      }
    }
    
    // Crear índices ordenados por distancia descendente
    for (size_t u = 0; u < n; ++u) {
      if (v != u) {
        indicesOrdenados[v].push_back(u);
      }
    }
    
    // Ordenar índices de mayor a menor distancia
    std::sort(indicesOrdenados[v].begin(), indicesOrdenados[v].end(), 
      [this, v](int a, int b) {
        double distA = dato_->espacioVectorial[v].calcularDistancia(dato_->espacioVectorial[a]);
        double distB = dato_->espacioVectorial[v].calcularDistancia(dato_->espacioVectorial[b]);
        return distA > distB;
    });
  }
}

/**
 * @brief Método para calcular la suma de distancias entre pares de puntos seleccionados
 * @param seleccion Conjunto de índices de puntos seleccionados
 * @return Suma de distancias
 */
double RamificacionPoda::calcularSumaParejas(const std::set<int>& seleccion) const {
  double suma = 0.0;
  
  for (auto i = seleccion.begin(); i != seleccion.end(); ++i) {
    auto j = i;
    ++j;
    for (; j != seleccion.end(); ++j) {
      suma += dato_->espacioVectorial[*i].calcularDistancia(dato_->espacioVectorial[*j]);
    }
  }
  
  return suma;
}

/**
 * @brief Método para calcular la suma de distancias entre un punto v y los puntos seleccionados
 * @param v Índice del punto
 * @param seleccion Conjunto de índices de puntos seleccionados
 * @return Suma de distancias
 */
double RamificacionPoda::calcularSumaSelVNoSel(int v, const std::set<int>& seleccion) const {
  double suma = 0.0;
  
  for (const auto& s : seleccion) {
    suma += dato_->espacioVectorial[s].calcularDistancia(dato_->espacioVectorial[v]);
  }
  
  return suma;
}

/**
 * @brief Método para ejecutar el algoritmo de Ramificación y Poda
 * @return void
 */
void RamificacionPoda::ejecutar() {
  auto start = std::chrono::high_resolution_clock::now();
  const size_t n = dato_->espacioVectorial.getDimension();
  const size_t m = numPuntosAlejados_;
  
  // Inicializar contador de nodos
  nodosGenerados_ = 1; // El nodo raíz
  
  // Realizar precomputaciones
  calcularPrecomputaciones();
  
  // Inicializar límite inferior (LB) usando GRASP
  auto grasp = std::make_unique<Grasp>();
  grasp->setDato(*dato_);
  grasp->setNumPuntosAlejados(m);
  grasp->setMejoresPuntos(2); // Valor por defecto
  grasp->ejecutar();
  
  // Obtener el resultado del GRASP
  EspacioVectorial espacioGrasp = grasp->getResultados().back().espacioVectorial;
  std::set<int> seleccionGrasp;
  for (size_t i = 0; i < espacioGrasp.getDimension(); ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (espacioGrasp[i].getIndice() == dato_->espacioVectorial[j].getIndice()) {
        seleccionGrasp.insert(j);
        break;
      }
    }
  }
  
  double LB = calcularSumaParejas(seleccionGrasp);
  
  // Inicializar espacio resultado con el mejor resultado hasta ahora (GRASP)
  std::set<int> mejorSeleccion = seleccionGrasp;
  
  // Pila para nodos, empezar con nodo raíz (Selección vacía, k=0)
  std::stack<std::pair<std::set<int>, int>> pila;
  pila.push({std::set<int>(), 0});
  
  while (!pila.empty()) {
    auto nodo = pila.top();
    pila.pop();
    
    std::set<int> seleccion = nodo.first;
    int k = nodo.second;
    
    if (k == m) {
      // Solución completa
      double z = calcularSumaParejas(seleccion);
      if (z > LB) {
        LB = z;
        mejorSeleccion = seleccion;
      }
    } else {
      // Regla de dominancia: podar si hay v no seleccionado con mayor distancia total que algún seleccionado
      if (!seleccion.empty()) {
        double minSel = std::numeric_limits<double>::infinity();
        for (const auto& s : seleccion) {
          if (distanciasTotal[s] < minSel) {
            minSel = distanciasTotal[s];
          }
        }
        
        double maxNoSel = -1.0;
        for (size_t v = 0; v < n; ++v) {
          if (seleccion.find(v) == seleccion.end() && distanciasTotal[v] > maxNoSel) {
            maxNoSel = distanciasTotal[v];
          }
        }
        
        if (minSel < maxNoSel) {
          continue; // Podar
        }
      }
      
      // Calcular cota superior (z1 + UB23)
      double z1 = calcularSumaParejas(seleccion);
      std::vector<std::pair<double, int>> zValues;
      
      for (size_t v = 0; v < n; ++v) {
        if (seleccion.find(v) == seleccion.end()) {
          double zSelV = calcularSumaSelVNoSel(v, seleccion);
          
          double sumUnsel = 0.0;
          int count = 0;
          for (const auto& u : indicesOrdenados[v]) {
            if (seleccion.find(u) == seleccion.end() && count < m - k - 1) {
              sumUnsel += dato_->espacioVectorial[v].calcularDistancia(dato_->espacioVectorial[u]);
              count++;
            }
          }
          
          double zUnselV = 0.5 * sumUnsel;
          double zV = zSelV + zUnselV;
          zValues.push_back({zV, v});
        }
      }
      
      std::sort(zValues.begin(), zValues.end(), std::greater<std::pair<double, int>>());
      
      double UB23 = 0.0;
      for (size_t i = 0; i < m - k && i < zValues.size(); ++i) {
        UB23 += zValues[i].first;
      }
      
      double UB = z1 + UB23;
      
      if (UB < LB) {
        continue; // Podar
      }
      
      // Solución heurística: seleccionar los m-k mejores v por z_v
      std::vector<int> topVertices;
      for (size_t i = 0; i < m - k && i < zValues.size(); ++i) {
        topVertices.push_back(zValues[i].second);
      }
      
      std::set<int> sHeuristic = seleccion;
      for (const auto& v : topVertices) {
        sHeuristic.insert(v);
      }
      
      double zHeuristic = calcularSumaParejas(sHeuristic);
      if (zHeuristic > LB) {
        LB = zHeuristic;
        mejorSeleccion = sHeuristic;
      }
      
      // Ramificación: generar nodos hijos
      for (size_t v = 0; v < n; ++v) {
        if (seleccion.find(v) == seleccion.end()) {
          std::set<int> newSel = seleccion;
          newSel.insert(v);
          pila.push({newSel, k + 1});
          nodosGenerados_++; // Incrementar contador de nodos
        }
      }
    }
  }
  
  // Construir el resultado final
  Dato resultado = *dato_;
  EspacioVectorial subconjunto;
  
  for (const auto& indice : mejorSeleccion) {
    subconjunto.agregarPunto(dato_->espacioVectorial[indice]);
  }
  
  resultado.espacioVectorial = subconjunto;
  resultado.nodosGenerados = nodosGenerados_; // Guardar el número de nodos generados
  
  auto end = std::chrono::high_resolution_clock::now();
  resultado.tiempoCPU = std::chrono::duration<double>(end - start).count();
  
  resultados_.push_back(resultado);
}

/**
 * @brief Método para mostrar los resultados del algoritmo de Ramificación y Poda
 * @return void
 */
void RamificacionPoda::mostrarResultados() {
  static bool cabeceraMostrada = false;

  if (!cabeceraMostrada) {
    // Cabecera
    std::cout << "---------------------------------------------------------------------------------" << std::endl;
    std::cout << std::left 
    << std::setw(20) << "Problema" 
    << std::setw(6) << "n" 
    << std::setw(6) << "K" 
    << std::setw(6) << "m" 
    << std::setw(12) << "z"
    << std::setw(12) << "Tiempo CPU" 
    << std::setw(12) << "Nodos" 
    << std::setw(12) << "S"
    << std::endl;
    std::cout << "---------------------------------------------------------------------------------" << std::endl;
    cabeceraMostrada = true;
  }

  // Mostrar los resultados
  for (auto& resultado : resultados_) {
    double distancia = calcularDistancia(resultado.espacioVectorial);
    std::cout << std::left 
    << std::setw(20) << resultado.nombreFichero
    << std::setw(6) << resultado.numPuntos
    << std::setw(6) << resultado.tamanio
    << std::setw(6) << resultado.espacioVectorial.getDimension()
    << std::setw(12) << std::fixed << std::setprecision(2) << distancia
    << std::setw(12) << std::fixed << std::setprecision(5) << resultado.tiempoCPU
    << std::setw(12) << resultado.nodosGenerados
    << std::setw(0) << resultado.espacioVectorial
    << std::endl;
  }
  std::cout << "---------------------------------------------------------------------------------" << std::endl;
}