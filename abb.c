#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct abb{
    abb_comparar_clave_t comparador;
    abb_destruir_dato_t destructor;
    void* dato;
    const char* clave;
    abb_t* izq;
    abb_t* der;
};
typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);



/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol)
        return NULL;
    arbol->destructor = destruir_dato;
    arbol->comparador = cmp;
    arbol->dato = NULL;
    arbol->clave = NULL;
    arbol->der = NULL;
    arbol->izq = NULL;
    return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){

    if (!arbol->clave){
        arbol->clave = malloc(sizeof(char) * (strlen(clave) + 1) );
        if (!arbol->clave)
            return false;

        strcpy(arbol->clave, clave);
        arbol->dato = dato;
        return true;
    }

    //0 -> Iguales. <0 -> 1ยบ mas grande. >0 2ยบ mas grande.
    if(!arbol->comparador(arbol->clave, clave)){
        arbol->destructor(arbol->dato);
        arbol->dato = dato;
        return true;
    }

    if(arbol->comparador(arbol->clave, clave) < 0){
        if (!arbol->izq){
            arbol->izq = abb_crear(arbol->comparador, arbol->destructor);
            if (!arbol->izq)
                return false;
        }
        return abb_guardar(arbol->izq, clave, dato);
    }
    else{
        if (!arbol->der){
            arbol->der = abb_crear(arbol->comparador, arbol->destructor);
            if (!arbol->der)
                return false;
        }
        return abb_guardar(arbol->der, clave, dato);
    }

}

//TODO: Se repite código en las siguientes tres funciones. Me duele pero es comportamiento binario, so...? What do?
void *abb_borrar(abb_t *arbol, const char *clave) {
	if(!arbol)
        return NULL;
	
	if(!arbol->comparador(arbol->clave, clave)){
		void *buffer = arbol->dato;
		free((void *) arbol->clave);
		arbol->destructor(arbol->dato);
		free(arbol);
        return buffer;
    }
	else if(arbol->comparador(arbol->clave, clave) < 0) {
		return (abb_borrar(arbol->izq, clave));
	}
	else {
		return (abb_borrar(arbol->der, clave));
	}
	
	return NULL;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	if(!arbol)
        return NULL;
	
	if(!arbol->comparador(arbol->clave, clave)){
        return arbol->dato;
    }
	else if(arbol->comparador(arbol->clave, clave) < 0) {
		return (abb_obtener(arbol->izq, clave));
	}
	else {
		return (abb_obtener(arbol->der, clave));
	}
	
	return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	if(!arbol)
        return false;
	
	if(!arbol->comparador(arbol->clave, clave)){
        return true;
    }
	else if(arbol->comparador(arbol->clave, clave) < 0) {
		return (abb_pertenece(arbol->izq, clave));
	}
	else {
		return (abb_pertenece(arbol->der, clave));
	}
	
	return false;
}

size_t abb_cantidad(abb_t *arbol) {
	if(!arbol)
        return 0;
	
	if(!arbol->izq && !arbol->der)
		return 1;
	
	size_t cantidad = 0;
    cantidad += abb_cantidad(arbol->izq);
    cantidad += abb_cantidad(arbol->der);
	
	return cantidad;
}

void abb_destruir(abb_t *arbol){
    if(!arbol)
        return;
    abb_destruir(arbol->izq);
    abb_destruir(arbol->der);

    free((void *) arbol->clave);
    arbol->destructor(arbol->dato);
    free(arbol);
}



/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERACION
 * *****************************************************************/

 //TODO: Creo que le falta algo mas. Pero por las dudas tiene lo mismo que cualquier nodo (+ raiz + actual).
typedef struct abb_iter abb_iter_t {
	abb_comparar_clave_t comparador;
    abb_destruir_dato_t destructor;
    void* dato;
    const char* clave;
	abb_t* raiz;
	abb_t* actual;
    abb_t* izq;
    abb_t* der;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

bool abb_iter_in_avanzar(abb_iter_t *iter);

const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

bool abb_iter_in_al_final(const abb_iter_t *iter);

void abb_iter_in_destruir(abb_iter_t* iter);



/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);
