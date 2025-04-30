#include "funciones-main.h"

/**
 * @brief Función para ordenar los datos
 * @param datos Vector de estructuras Dato con los datos a ordenar
 * @details Datos ordenados por el nombre del fichero
 * @return vector<Dato> Vector de estructuras Dato ordenado
 */
vector<Dato> ordenarDatos(vector<Dato>& datos) {
  sort(datos.begin(), datos.end(), [](const Dato& a, const Dato& b) {
    return a.nombreFichero < b.nombreFichero;
  });
  return datos;
}

/**
 * @brief Función para leer los ficheros de datos
 * @param directorio Directorio donde se encuentran los ficheros
 * @return vector<Dato> Vector de estructuras Dato con los datos leídos
 */
vector<Dato> leerFicheros(const string& directorio) {
  vector<Dato> datos;
  for (const auto& entrada : fs::directory_iterator(directorio)) {
    // Verifico que la entrada es un fichero
    if (fs::is_regular_file(entrada)) {
      Dato dato;
      int numPuntos, tamanio;
      string nombreFichero = entrada.path().filename().string();
      ifstream fichero(entrada.path().string()); // Abro el fichero con la ruta completa

      dato.nombreFichero = nombreFichero;

      fichero >> numPuntos >> tamanio;
      dato.numPuntos = numPuntos;
      dato.tamanio = tamanio;

      if (tamanio <= 0) {
        throw invalid_argument("La dimensión debe ser mayor que cero");
      }
      if (numPuntos <= 0) {
        throw invalid_argument("El número de puntos debe ser mayor que cero");
      }
    
      for (int i = 0; i < numPuntos; i++) {
        vector<double> punto(tamanio);
        for (int j = 0; j < tamanio; j++) {
          fichero >> punto[j];
        }
        dato.espacioVectorial.agregarPunto(Punto(i + 1, punto));
      }
      datos.push_back(dato);
      fichero.close();
    }
  }
  return ordenarDatos(datos);
}

/**
 * @brief Función para mostrar el menú de opciones
 * @return void
 */
void mostrarMenu() {
  cout << "Seleccione el algoritmo a ejecutar:" << endl;
  cout << "1. Algoritmo Voraz" << endl;
  cout << "2. Algoritmo GRASP" << endl;
  cout << "Seleccione una opción: ";
}

/**
 * @brief Función para ejecutar el algoritmo seleccionado
 * @param datos Vector de estructuras Dato con los datos a procesar
 * @param opcion Opción seleccionada por el usuario
 * @return void
 */
void ejecutarAlgoritmo(vector<Dato>& datos, const int opcion) {
  switch (opcion) {
    case 1: 
      ejecutarVoraz(datos, 5);
      break;
    case 2:
      ejecutarGrasp(datos, 5);
      break;
    default:
      invalid_argument("Opción no válida");
      break;
  }
}

/**
 * @brief Función para ejecutar el algoritmo Voraz
 * @param datos Vector de estructuras Dato con los datos a procesar
 * @param numPuntosAlejados Número de puntos alejados a seleccionar
 * @return void
 */
void ejecutarVoraz(vector<Dato>& datos, int numPuntosAlejados) {
  for (auto& dato : datos) {
    auto voraz = make_unique<Voraz>();
    voraz->setDato(dato);
    for (int i = 2; i <= numPuntosAlejados; i++) {
      voraz->setNumPuntosAlejados(i);
      voraz->ejecutar();
    }
    voraz->mostrarResultados();
  }
  cout << "Algoritmo Voraz ejecutado con éxito." << endl;
}

/**
 * @brief Función para ejecutar el algoritmo GRASP
 * @param datos Vector de estructuras Dato con los datos a procesar
 * @param numPuntosAlejados Número de puntos alejados a seleccionar
 * @return void
 */
void ejecutarGrasp(vector<Dato>& datos, int numPuntosAlejados) {
  for (auto& dato : datos) {
    auto grasp = make_unique<Grasp>();
    grasp->setDato(dato);
    for (int i = 2; i <= numPuntosAlejados; i++) {
      grasp->setNumPuntosAlejados(i);
      for (int j = 1; j <= 2; j++) {
        for (int k = 2; k <= 3; k++) {
          grasp->setMejoresPuntos(k);
          grasp->ejecutar();
        }
      }
    }
    grasp->mostrarResultados();
  }
  cout << "Algoritmo GRASP ejecutado con éxito." << endl;
}