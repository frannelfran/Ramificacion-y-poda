/**
 * @brief Funciones para la gestión del main
 */

#ifndef FUNCIONES_MAIN_H
#define FUNCIONES_MAIN_H

#include "../dato/dato.h"
#include "../algoritmo/voraz/voraz.h"
#include "../algoritmo/grasp/grasp.h"
#include <stdexcept> // Para las excepciones
#include <filesystem> // Para la gestión de ficheros
#include <fstream> // Para la lectura de ficheros

namespace fs = filesystem;

// Funciones para gestionar los ficheros
vector<Dato> leerFicheros(const string& directorio);
vector<Dato> ordenarDatos(vector<Dato>& datos);
// Funciones para ejecutar los algoritmos
void ejecutarAlgoritmo(vector<Dato>& datos, const int opcion);
void ejecutarVoraz(vector<Dato>& datos, int numPuntosAlejados);
void ejecutarGrasp(vector<Dato>& datos, int numPuntosAlejados);

void mostrarMenu(); // Mostrar el menú de opciones

#endif