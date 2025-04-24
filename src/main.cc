#include "funciones-main/funciones-main.h"

/**
 * @brief Función principal
 * @param argc Número de argumentos
 * @param argv Argumentos
 */

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Uso: " << argv[0] << " <directorio>" << endl;
    return 1;
  }
  try {
    string directorio = argv[1];
    vector<Dato> datos = leerFicheros(directorio);

    
  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}