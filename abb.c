#include "abb.h"
#include "pila.h"
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

        strcpy((char *) arbol->clave, clave);
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

//TODO: Se repite codigo en las siguientes tres funciones.
void *abb_borrar(abb_t *arbol, const char *clave) {
	if(!arbol || !arbol->dato)
        return NULL;
	
	if(!arbol->comparador(arbol->clave, clave)){
		void *buffer = arbol->dato;
		free((void *) arbol->clave);
		free(arbol);
        return buffer;
    }
	else if(arbol->comparador(arbol->clave, clave) < 0)
		return (abb_borrar(arbol->izq, clave));
	else 
		return (abb_borrar(arbol->der, clave));
	
	return NULL;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	if(!arbol || !arbol->dato)
        return NULL;
	
	if(!arbol->comparador(arbol->clave, clave)){
        return arbol->dato;
    }
	else if(arbol->comparador(arbol->clave, clave) < 0) 
		return (abb_obtener(arbol->izq, clave));
	else 
		return (abb_obtener(arbol->der, clave));
	
	return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	if(!arbol || !arbol->dato)
        return false;
	
	if(!arbol->comparador(arbol->clave, clave)){
        return true;
    }
	else if(arbol->comparador(arbol->clave, clave) < 0) 
		return (abb_pertenece(arbol->izq, clave));
	else 
		return (abb_pertenece(arbol->der, clave));
	
	return false;
}

size_t abb_cantidad(abb_t *arbol) {
	if(!arbol || !arbol->dato)
        return 0;
	if(!arbol->izq && !arbol->der)
		return 1;

	return abb_cantidad(arbol->izq) + abb_cantidad(arbol->der);
}

void abb_destruir(abb_t *arbol){
    if(!arbol)
        return;
	abb_destruir(arbol->izq);
	abb_destruir(arbol->der);

    if (arbol->dato){
        free((void *) arbol->clave);
        arbol->destructor(arbol->dato);

    }
	free(arbol);
}



/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERACION
 * *****************************************************************/

struct abb_iter {
	pila_t* pila;
};
typedef struct abb_iter abb_iter_t;

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter)
		return NULL;

    iter->pila = pila_crear();
    if(!iter->pila){
        free(iter);
        return NULL;
    }

    if(!arbol->dato)
        return iter;

    pila_apilar(iter->pila, (void *) arbol);

    abb_t* arbol_izq = arbol->izq;
    while(arbol_izq){
        pila_apilar(iter->pila, arbol_izq);
        arbol_izq = arbol_izq->izq;
    }
	
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    abb_t* desapilado = pila_desapilar(iter->pila);
    if(!desapilado)
        return false;

    //TODO: Hay que controllar que no falle al apilar ?
    if (desapilado->der)
        pila_apilar(iter->pila, desapilado->der);

    //TODO: Apilo los hijos izqs del desapilado o los hijos izqs del hijo derecho del desapilado ?
    abb_t* arbol_izq = desapilado->izq;
    while(arbol_izq){
        pila_apilar(iter->pila, arbol_izq);
        arbol_izq = arbol_izq->izq;
    }

    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    abb_t* tope = pila_ver_tope(iter->pila);
    if (!tope)
        return NULL;
	return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}



/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    if(!arbol || !arbol->dato)
        return;
    abb_in_order(arbol->izq, visitar, extra);
    //TODO: Esto no corta de verdad la iteracion (La funcion que llamo a esta va a seguir con el nodo correspondiente)
    if (!visitar(arbol->clave, arbol->dato, extra))
        return;
    abb_in_order(arbol->der, visitar, extra);
}
