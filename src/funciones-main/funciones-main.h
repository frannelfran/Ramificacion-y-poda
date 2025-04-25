/**
 * @brief Funciones para la gestión del main
 */

#ifndef FUNCIONES_MAIN_H
#define FUNCIONES_MAIN_H

#include "../dato/dato.h"
#include "../algoritmo/voraz/voraz.h"
#include <stdexcept> // Para las excepciones
#include <filesystem> // Para la gestión de ficheros
#include <fstream> // Para la lectura de ficheros

namespace fs = filesystem;

vector<Dato> leerFicheros(const string& directorio);
Algoritmo* crearAlgoritmo(const int opcion);
void mostrarMenu();

#endif