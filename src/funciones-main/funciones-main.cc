#include "funciones-main.h"

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
        dato.espacioVectorial.agregarPunto(Punto(punto));
      }
      datos.push_back(dato);
      fichero.close();
    }
  }
  return datos;
}

/**
 * @brief Función para crear el algoritmo a ejecutar
 * @param opcion Opción seleccionada por el usuario
 * @return Algoritmo* Puntero al algoritmo creado
 */
Algoritmo* crearAlgoritmo(const int opcion) {
  Algoritmo* algoritmo = nullptr;
  switch (opcion) {
    case 1: {
      int numPuntosAlejados;
      cout << "Ingrese el número de puntos alejados: ";
      cin >> numPuntosAlejados;
      if (numPuntosAlejados <= 0) {
        throw invalid_argument("El número de puntos alejados debe ser mayor que cero");
      }
      algoritmo = new Voraz();
      break;
    }
    default:
      throw invalid_argument("Opción no válida");
  }
  return algoritmo;
}

/**
 * @brief Función para mostrar el menú de opciones
 * @return void
 */
void mostrarMenu() {
  cout << "Seleccione el algoritmo a ejecutar:" << endl;
  cout << "1. Algoritmo Voraz" << endl;
}