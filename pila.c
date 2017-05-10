//Nombre: Nicolás De Giácomo
//Padron: 99702
//Corrector: Matias C

#include "pila.h"
#include <stdlib.h>

#define SIZE 10 //Capacidad inicial de la pila
//TODO: poner esto como porcentaje y pasar a size_t al multiplicar
#define REALLOC_WHEN 4 //Porcentage al cual la capacidad de la pila se reduce
#define REALLOC_TO 2 //Porcentaje de reduccion y ampliacion de la capacidad de la pila

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

bool redimensionar(pila_t* pila, size_t size);

pila_t* pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if (!pila)
		return NULL;
	pila->cantidad = 0;
	pila->capacidad = SIZE;
	pila->datos = malloc(SIZE * sizeof(void*));
	if (!pila->datos)
		return NULL;
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
	return;
}

bool pila_esta_vacia(const pila_t *pila){
	return !(pila->cantidad);
}

bool pila_apilar(pila_t* pila, void* valor){
	//Si ya esta al maximo, redimensiono antes de apilar
	if (pila->cantidad == pila->capacidad-1)
		if (!redimensionar(pila, (pila->capacidad)*REALLOC_TO))
			return false;
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (!(pila->cantidad))
		return NULL;
	return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t *pila){
	if (!(pila->cantidad))
		return NULL;
	pila->cantidad--;
	void* dato = pila->datos[pila->cantidad];
	//Si baja del minimo, redimensiono luego de desapilar
	if (pila->cantidad <= (pila->capacidad / REALLOC_WHEN))
		redimensionar(pila, (pila->capacidad / REALLOC_TO));
	return dato;
}

bool redimensionar(pila_t* pila, size_t size){
	void** tmp = realloc(pila->datos, size*sizeof(void*));
	if (!tmp)
		return false;
	pila->capacidad = size;
	pila->datos = tmp;
	return true;
}
