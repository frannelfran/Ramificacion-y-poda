/**
 * @brief Funciones para la gestión del main
 */

#ifndef FUNCIONES_MAIN_H
#define FUNCIONES_MAIN_H

#include "../dato/dato.h"
#include <stdexcept> // Para las excepciones
#include <filesystem> // Para la gestión de ficheros
#include <fstream> // Para la lectura de ficheros

namespace fs = filesystem;

vector<Dato> leerFicheros(const string& directorio);

#endif