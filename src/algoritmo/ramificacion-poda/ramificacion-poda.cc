#include "ramificacion-poda.h"

/**
 * @brief Método para el preprocesamiento de datos
 * @return void
 */
void RamificacionPoda::preprocesamiento() {
  // Obtener el espacio vectorial
  EspacioVectorial espacioVectorial = dato_->espacioVectorial;
  int n = espacioVectorial.getDimension();
  
  // Inicializar la matriz de distancias y el sumatorio de distancias
  matrizDistancias_.resize(n, vector<double>(n, 0.0));
  sumatorioDistancias_.resize(n, 0.0);
  vecinosOrdenados_.resize(n);

  // Calcular la matriz de distancias entre todos los puntos
  for (int i = 0; i < n; ++i) {
    vector<pair<double, int>> vecinos;
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        // Calcular distancia entre puntos
        matrizDistancias_[i][j] = espacioVectorial[i].calcularDistancia(espacioVectorial[j]);
        sumatorioDistancias_[i] += matrizDistancias_[i][j];
        vecinos.push_back({matrizDistancias_[i][j], j});
      }
    }
    
    // Ordenar vecinos por distancia descendente
    sort(vecinos.begin(), vecinos.end(), greater<pair<double, int>>());
    
    // Guardar los índices de los vecinos ordenados
    for (const auto& vecino : vecinos) {
      vecinosOrdenados_[i].push_back(vecino.second);
    }
  }
}

/**
 * @brief Método para calcular la suma de distancias entre los puntos seleccionados
 * @param seleccionados Conjunto de puntos seleccionados
 * @return double Suma de distancias
 */
double RamificacionPoda::calcularSumaDistancias(const set<int>& seleccionados) const {
  double suma = 0.0;
  
  // Calcular la suma de distancias entre todos los pares de puntos seleccionados
  for (auto it1 = seleccionados.begin(); it1 != seleccionados.end(); ++it1) {
    auto it2 = it1;
    ++it2;
    for (; it2 != seleccionados.end(); ++it2) {
      suma += matrizDistancias_[*it1][*it2];
    }
  }
  return suma;
}

/**
 * @brief Método para aplicar la regla de dominancia
 * @param nodo Nodo actual en el árbol de búsqueda
 * @return bool true si el nodo debe ser podado, false en caso contrario
 */
bool RamificacionPoda::aplicarReglaDominancia(const Nodo& nodo) const {
  // Si no hay puntos seleccionados, no aplicar regla de dominancia
  if (nodo.seleccionados.empty()) {
    return false;
  }
  
  // Encontrar el punto seleccionado con menor suma de distancias
  double minSel = numeric_limits<double>::max();
  for (int v : nodo.seleccionados) {
    minSel = min(minSel, sumatorioDistancias_[v]);
  }
  
  // Encontrar el punto no seleccionado con mayor suma de distancias
  double maxNoSel = -1.0;
  for (int i = 0; i < dato_->espacioVectorial.getDimension(); ++i) {
    if (nodo.seleccionados.find(i) == nodo.seleccionados.end()) {
      maxNoSel = max(maxNoSel, sumatorioDistancias_[i]);
    }
  }
  
  // Si el mínimo seleccionado es menor que el máximo no seleccionado, podar
  return minSel < maxNoSel;
}

/**
 * @brief Método para calcular la cota superior para un nodo
 * @param nodo Nodo actual en el árbol de búsqueda
 * @param valoresZ Vector para almacenar los valores z de los candidatos
 * @return double Cota superior para el nodo
 */
double RamificacionPoda::calcularCotaSuperior(const Nodo& nodo, vector<pair<double, int>>& valoresZ) const {
  // Calcular la suma actual de distancias
  double z1 = calcularSumaDistancias(nodo.seleccionados);
  int n = dato_->espacioVectorial.getDimension();
  int m = numPuntosAlejados_;
  int k = nodo.nivel;
  
  // Generar lista de candidatos (puntos no seleccionados)
  vector<int> candidatos;
  for (int i = 0; i < n; ++i) {
    if (nodo.seleccionados.find(i) == nodo.seleccionados.end()) {
      candidatos.push_back(i);
    }
  }
  
  // Calcular la contribución potencial de cada candidato
  valoresZ.clear();
  for (int v : candidatos) {
    // Contribución a puntos ya seleccionados
    double contribucionSel = 0.0;
    for (int s : nodo.seleccionados) {
      contribucionSel += matrizDistancias_[v][s];
    }
    
    // Contribución a puntos no seleccionados
    double contribucionNoSel = 0.0;
    int contador = 0;
    for (int u : vecinosOrdenados_[v]) {
      // Solo considerar puntos no seleccionados y diferentes al candidato
      if (nodo.seleccionados.find(u) == nodo.seleccionados.end() && u != v) {
        contribucionNoSel += matrizDistancias_[v][u];
        contador++;
        if (contador >= m - k - 1) break;
      }
    }
    contribucionNoSel *= 0.5; // Factor de 0.5 según el pseudocódigo
    
    // Valor z para este candidato
    double z_v = contribucionSel + contribucionNoSel;
    valoresZ.push_back({z_v, v});
  }
  
  // Ordenar candidatos por valor z descendente
  sort(valoresZ.begin(), valoresZ.end(), greater<pair<double, int>>());
  
  // Calcular cota superior usando los mejores candidatos
  double UB = z1;
  for (int i = 0; i < min(m - k, static_cast<int>(valoresZ.size())); ++i) {
    UB += valoresZ[i].first;
  }
  
  return UB;
}

/**
 * @brief Método para generar los hijos de un nodo
 * @param nodo Nodo actual en el árbol de búsqueda
 * @param cotaInferior Cota inferior actual
 * @param pila Pila de nodos por explorar
 * @param valoresZ Vector con los valores z de los candidatos
 */
void RamificacionPoda::generarHijos(const Nodo& nodo, double cotaInferior, stack<Nodo>& pila, const vector<pair<double, int>>& valoresZ) {
  // Para cada candidato (ordenados por valor z descendente)
  for (const auto& [z_v, v] : valoresZ) {
    // Generar un nuevo conjunto de seleccionados incluyendo el candidato
    set<int> nuevosSeleccionados = nodo.seleccionados;
    nuevosSeleccionados.insert(v);
    
    // Crear un nuevo nodo con este conjunto
    if (nodo.nivel + 1 <= numPuntosAlejados_) {
      Nodo nuevoNodo(nuevosSeleccionados, nodo.nivel + 1);
      pila.push(nuevoNodo);
      nodosGenerados_++; // Incrementar contador de nodos generados
    }
  }
}

/**
 * @brief Método para ejecutar el algoritmo de Ramificación y Poda
 * @return void
 */
void RamificacionPoda::ejecutar() {
  auto start = chrono::high_resolution_clock::now();
  
  // Inicializar contador de nodos generados y vector de información
  nodosGenerados_ = 0;
  
  // Paso 1: Preprocesamiento
  preprocesamiento();
  
  // Paso 2: Inicialización
  
  // Generar cota inferior inicial usando el algoritmo proporcionado
  Dato datoInicial = *dato_;
  algoritmo_->setDato(datoInicial);
  algoritmo_->setNumPuntosAlejados(numPuntosAlejados_);
  algoritmo_->ejecutar();

  // Calculo el valor de la mejor solución inicial
  if (algoritmo_->getResultados().empty()) {
    mejorSolucion_ = 0.0;
    mejorConjunto_.clear();
  } else {
    // Obtengo la solución generada por el algoritmo (Voraz o GRASP)
    const Dato& solucionInicial = algoritmo_->getResultados().back();
    const EspacioVectorial& espacioSolucion = solucionInicial.espacioVectorial;
    
    // Convierto los puntos de la solución inicial a índices en el espacio original
    mejorConjunto_.clear();
    for (size_t i = 0; i < espacioSolucion.getDimension(); ++i) {
      int indiceOriginal = espacioSolucion[i].getIndice() - 1;  // Ajustar a índice base 0
      mejorConjunto_.insert(indiceOriginal);
    }
    
    // Calculo la suma de distancias entre todos los pares de puntos seleccionados
    mejorSolucion_ = calcularSumaDistancias(mejorConjunto_);
  }
  
  // Inicializar la pila de nodos
  stack<Nodo> pila;
  Nodo nodoRaiz(set<int>(), 0);
  pila.push(nodoRaiz); // Nodo raíz (conjunto vacío, nivel 0)
  
  nodosGenerados_++; // Contar el nodo raíz
  
  // Paso 3: Búsqueda con podas
  while (!pila.empty()) {
    // Extraer nodo actual
    Nodo nodoActual = pila.top();
    pila.pop();
    
    // Si es solución completa
    if (nodoActual.nivel == numPuntosAlejados_) {
      double z = calcularSumaDistancias(nodoActual.seleccionados);
      if (z > mejorSolucion_) {
        mejorSolucion_ = z;
        mejorConjunto_ = nodoActual.seleccionados;
      }
      continue;
    }
    
    // Aplicar regla de dominancia
    if (aplicarReglaDominancia(nodoActual)) {
      continue; // Podar este nodo
    }
    
    // Calcular cota superior
    vector<pair<double, int>> valoresZ;
    double UB = calcularCotaSuperior(nodoActual, valoresZ);
    
    // Poda si la cota no supera la mejor solución
    if (UB <= mejorSolucion_) {
      continue;
    }
    
    // Generar hijos
    generarHijos(nodoActual, mejorSolucion_, pila, valoresZ);
  }
  
  // Construir el resultado
  Dato resultado = *dato_;
  EspacioVectorial subconjunto;
  
  // Añadir los puntos de la mejor solución al subconjunto
  for (int i : mejorConjunto_) {
    subconjunto.agregarPunto(dato_->espacioVectorial[i]);
  }
  
  resultado.espacioVectorial = subconjunto;
  auto end = chrono::high_resolution_clock::now();
  resultado.tiempoCPU = chrono::duration<double>(end - start).count();
  
  // Agregar el resultado
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
    cout << "-------------------------------------------------------------------------------------" << endl;
    cout << left 
    << setw(20) << "Problema" 
    << setw(6) << "n" 
    << setw(6) << "K" 
    << setw(6) << "m" 
    << setw(10) << "z"
    << setw(12) << "Tiempo CPU" 
    << setw(6) << "S"
    << setw(6) << " "
    << setw(6) << "nodos generados"
    << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
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
    << setw(10) << fixed << setprecision(2) << distancia
    << setw(12) << fixed << setprecision(5) << resultado.tiempoCPU
    << setw(0) << resultado.espacioVectorial
    << setw(6) << " "
    << setw(12) << nodosGenerados_
    << endl;
  }
  cout << "-------------------------------------------------------------------------------------" << endl;
}