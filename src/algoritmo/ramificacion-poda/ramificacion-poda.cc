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
    sort(indicesOrdenados[v].begin(), indicesOrdenados[v].end(), 
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
double RamificacionPoda::calcularSumaParejas(const set<int>& seleccion) const {
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
double RamificacionPoda::calcularSumaSelVNoSel(int v, const set<int>& seleccion) const {
  double suma = 0.0;
  
  for (const auto& s : seleccion) {
    suma += dato_->espacioVectorial[s].calcularDistancia(dato_->espacioVectorial[v]);
  }
  
  return suma;
}

/**
 * @brief Establece una cota inicial basada en una solución heurística GRASP
 * @return void
 */
void RamificacionPoda::establecerCotaInicial() {
  const size_t m = numPuntosAlejados_;
  const size_t n = dato_->espacioVectorial.getDimension();
  
  // Inicializar límite inferior (LB) usando GRASP
  auto grasp = make_unique<Grasp>();
  grasp->setDato(*dato_);
  grasp->setNumPuntosAlejados(m);
  grasp->setMejoresPuntos(2); // Valor por defecto
  grasp->ejecutar();
  
  // Obtener el resultado del GRASP
  EspacioVectorial espacioGrasp = grasp->getResultados().back().espacioVectorial;
  set<int> seleccionGrasp;
  
  for (size_t i = 0; i < espacioGrasp.getDimension(); ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (espacioGrasp[i].getIndice() == dato_->espacioVectorial[j].getIndice()) {
        seleccionGrasp.insert(j);
        break;
      }
    }
  }
  
  mejorValor_ = calcularSumaParejas(seleccionGrasp);
  mejorSeleccion_ = seleccionGrasp;
}

/**
 * @brief Método para calcular una cota superior para un nodo del árbol
 * @param seleccion Conjunto de puntos ya seleccionados
 * @param nivel Nivel actual en el árbol
 * @return Cota superior para el nodo
 */
double RamificacionPoda::calcularCotaSuperior(const set<int>& seleccion, int nivel) const {
  const size_t n = dato_->espacioVectorial.getDimension();
  const size_t m = numPuntosAlejados_;
  
  // Valor actual de la solución parcial
  double z1 = calcularSumaParejas(seleccion);
  
  // Si ya tenemos una solución completa, la cota es su valor
  if (nivel >= m) {
    return z1;
  }
  
  // Calculamos una cota para los puntos que faltan por seleccionar
  vector<pair<double, int>> zValues;
  
  for (size_t v = 0; v < n; ++v) {
    if (seleccion.find(v) == seleccion.end()) {
      double zSelV = calcularSumaSelVNoSel(v, seleccion);
      
      // Calculamos un estimado para las distancias entre v y otros puntos no seleccionados
      double sumUnsel = 0.0;
      int count = 0;
      for (const auto& u : indicesOrdenados[v]) {
        if (seleccion.find(u) == seleccion.end() && count < m - nivel - 1) {
          sumUnsel += dato_->espacioVectorial[v].calcularDistancia(dato_->espacioVectorial[u]);
          count++;
        }
      }
      
      // Ajustamos para evitar contar cada distancia dos veces
      double zUnselV = 0.5 * sumUnsel;
      double zV = zSelV + zUnselV;
      zValues.push_back({zV, v});
    }
  }
  
  // Ordenamos de mayor a menor
  sort(zValues.begin(), zValues.end(), greater<pair<double, int>>());
  
  // Calculamos la cota para los puntos restantes
  double UB23 = 0.0;
  for (size_t i = 0; i < m - nivel && i < zValues.size(); ++i) {
    UB23 += zValues[i].first;
  }
  
  return z1 + UB23;
}

/**
 * @brief Implementación del algoritmo con pila (enfoque DFS)
 * @return void
 */
void RamificacionPoda::ejecutarConPila() {
  auto start = chrono::high_resolution_clock::now();
  const size_t n = dato_->espacioVectorial.getDimension();
  const size_t m = numPuntosAlejados_;
  
  // Inicializar contadores
  nodosGenerados_ = 1; // El nodo raíz
  nodosPodados_ = 0;
  
  // Establecer cota inicial con GRASP
  establecerCotaInicial();
  
  // Pila para nodos, empezar con nodo raíz (Selección vacía, k=0)
  stack<pair<set<int>, int>> pila;
  pila.push({set<int>(), 0});
  
  while (!pila.empty()) {
    auto nodo = pila.top();
    pila.pop();
    
    set<int> seleccion = nodo.first;
    int k = nodo.second;
    
    if (k == m) {
      // Solución completa
      double z = calcularSumaParejas(seleccion);
      if (z > mejorValor_) {
        mejorValor_ = z;
        mejorSeleccion_ = seleccion;
      }
    } else {
      // Regla de dominancia: podar si hay v no seleccionado con mayor distancia total que algún seleccionado
      if (!seleccion.empty()) {
        double minSel = numeric_limits<double>::infinity();
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
          nodosPodados_++;
          continue; // Podar
        }
      }
      
      // Calcular cota superior
      double UB = calcularCotaSuperior(seleccion, k);
      
      if (UB < mejorValor_) {
        nodosPodados_++;
        continue; // Podar
      }
      
      // Solución heurística: seleccionar los m-k mejores v por z_v
      vector<pair<double, int>> zValues;
      
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
      
      sort(zValues.begin(), zValues.end(), greater<pair<double, int>>());
      
      // Construir solución heurística
      vector<int> topVertices;
      for (size_t i = 0; i < m - k && i < zValues.size(); ++i) {
        topVertices.push_back(zValues[i].second);
      }
      
      set<int> sHeuristic = seleccion;
      for (const auto& v : topVertices) {
        sHeuristic.insert(v);
      }
      
      double zHeuristic = calcularSumaParejas(sHeuristic);
      if (zHeuristic > mejorValor_) {
        mejorValor_ = zHeuristic;
        mejorSeleccion_ = sHeuristic;
      }
      
      // Ramificación: generar nodos hijos
      for (size_t v = 0; v < n; ++v) {
        if (seleccion.find(v) == seleccion.end()) {
          set<int> newSel = seleccion;
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
  
  for (const auto& indice : mejorSeleccion_) {
    subconjunto.agregarPunto(dato_->espacioVectorial[indice]);
  }
  
  resultado.espacioVectorial = subconjunto;
  resultado.nodosGenerados = nodosGenerados_; // Guardar el número de nodos generados
  
  auto end = chrono::high_resolution_clock::now();
  resultado.tiempoCPU = chrono::duration<double>(end - start).count();
  
  resultados_.push_back(resultado);
}

/**
 * @brief Implementación del algoritmo con cola de prioridad (enfoque best-first)
 * @return void
 */
void RamificacionPoda::ejecutarConPrioridad() {
  auto start = chrono::high_resolution_clock::now();
  const size_t n = dato_->espacioVectorial.getDimension();
  const size_t m = numPuntosAlejados_;
  
  // Inicializar contadores
  nodosGenerados_ = 1; // El nodo raíz
  nodosPodados_ = 0;
  
  // Establecer cota inicial con GRASP
  establecerCotaInicial();
  
  // Cola de prioridad para nodos, ordenados por cota superior
  priority_queue<Nodo> cola;
  
  // Crear nodo raíz
  set<int> seleccionInicial;
  double valorInicial = 0.0;
  double cotaInicial = calcularCotaSuperior(seleccionInicial, 0);
  cola.push(Nodo(seleccionInicial, valorInicial, cotaInicial, 0));
  
  while (!cola.empty()) {
    // Extraer nodo con mayor cota superior
    Nodo nodoActual = cola.top();
    cola.pop();
    
    // Si la cota es menor que la mejor solución, podar
    if (nodoActual.cota <= mejorValor_) {
      nodosPodados_++;
      continue;
    }
    
    // Si es una solución completa
    if (nodoActual.nivel == m) {
      if (nodoActual.valor > mejorValor_) {
        mejorValor_ = nodoActual.valor;
        mejorSeleccion_ = nodoActual.seleccion;
      }
      continue;
    }
    
    // Ramificar - probar con cada punto no seleccionado
    for (size_t v = 0; v < n; ++v) {
      if (nodoActual.seleccion.find(v) == nodoActual.seleccion.end()) {
        // Crear nuevo nodo
        set<int> nuevaSeleccion = nodoActual.seleccion;
        nuevaSeleccion.insert(v);
        
        // Calcular nuevo valor (distancias entre todos los pares)
        double nuevoValor = calcularSumaParejas(nuevaSeleccion);
        
        // Calcular cota superior
        double nuevaCota = calcularCotaSuperior(nuevaSeleccion, nodoActual.nivel + 1);
        
        // Si la cota es prometedora, añadir a la cola
        if (nuevaCota > mejorValor_) {
          cola.push(Nodo(nuevaSeleccion, nuevoValor, nuevaCota, nodoActual.nivel + 1));
          nodosGenerados_++;
        } else {
          nodosPodados_++;
        }
      }
    }
  }
  
  // Construir el resultado final
  Dato resultado = *dato_;
  EspacioVectorial subconjunto;
  
  for (const auto& indice : mejorSeleccion_) {
    subconjunto.agregarPunto(dato_->espacioVectorial[indice]);
  }
  
  resultado.espacioVectorial = subconjunto;
  resultado.nodosGenerados = nodosGenerados_; // Guardar el número de nodos generados
  
  auto end = chrono::high_resolution_clock::now();
  resultado.tiempoCPU = chrono::duration<double>(end - start).count();
  
  resultados_.push_back(resultado);
}

/**
 * @brief Método para ejecutar el algoritmo de Ramificación y Poda
 * @return void
 */
void RamificacionPoda::ejecutar() {
  // Realizar precomputaciones
  calcularPrecomputaciones();
  
  // Ejecutar la versión con cola de prioridad (best-first search)
  ejecutarConPrioridad();
  
  // Alternativamente, puedes usar la versión con pila (DFS)
  // ejecutarConPila();
}

/**
 * @brief Método para mostrar los resultados del algoritmo de Ramificación y Poda
 * @return void
 */
void RamificacionPoda::mostrarResultados() {
  static bool cabeceraMostrada = false;

  if (!cabeceraMostrada) {
    // Cabecera
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << left 
    << setw(20) << "Problema" 
    << setw(6) << "n" 
    << setw(6) << "K" 
    << setw(6) << "m" 
    << setw(12) << "z"
    << setw(12) << "Tiempo CPU" 
    << setw(12) << "Nodos" 
    << setw(12) << "S"
    << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    cabeceraMostrada = true;
  }

  // Mostrar los resultados
  for (auto& resultado : resultados_) {
    double distancia = calcularDistancia(resultado.espacioVectorial);
    cout << left 
    << setw(20) << resultado.nombreFichero
    << setw(6) << resultado.numPuntos
    << setw(6) << resultado.tamanio
    << setw(6) << resultado.espacioVectorial.getDimension()
    << setw(12) << fixed << setprecision(2) << distancia
    << setw(12) << fixed << setprecision(5) << resultado.tiempoCPU
    << setw(12) << resultado.nodosGenerados
    << setw(0) << resultado.espacioVectorial
    << endl;
  }
  cout << "---------------------------------------------------------------------------------" << endl;
}