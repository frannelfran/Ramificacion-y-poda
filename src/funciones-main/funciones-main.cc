#include "funciones-main.h"

/**
 * @brief Función para leer los ficheros de datos
 * @param directorio Directorio donde se encuentran los ficheros
 * @return vector<Dato> Vector de estructuras Dato con los datos leídos
 */
vector<Dato> leerFicheros(const string& directorio) {
  vector<Dato> datos;

  // Recorro los ficheros del directorio
  for (const auto& entrada : fs::directory_iterator(directorio)) {
    // Verifico que la entrada es un fichero
    if (fs::is_regular_file(entrada)) {
      Dato dato;
      int numPuntos, dimension;
      string nombreFichero = entrada.path().filename().string();
      ifstream fichero(nombreFichero); // Abro el fichero

      fichero >> numPuntos >> dimension;
      for (int i = 0; i < numPuntos; i++) {
        vector<double> punto(dimension);
        for (int j = 0; j < dimension; j++) {
          fichero >> punto[j];
        }
        dato.espacioVectorial.agregarPunto(Punto(punto));
      }
    }
  }
  return datos;
}