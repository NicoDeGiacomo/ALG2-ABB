#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <string.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct abb{
    struct nodo* raiz;
    size_t cantidad;
    abb_comparar_clave_t comparador;
    abb_destruir_dato_t destructor;
};

struct nodo{
    struct nodo* izq;
    struct nodo* der;
    const char* clave;
    void* dato;
};
typedef struct nodo nodo_t;


//TODO: Hacer una seccion para funciones auxiliares
bool abb_guardar_aux(nodo_t* raiz, nodo_t* nodo, abb_comparar_clave_t comparador, abb_destruir_dato_t destructor);
nodo_t* crear_nodo(const char *clave, void *dato);
void *abb_obtener_aux(const nodo_t *nodo, const char *clave, abb_comparar_clave_t comparador, bool* found);
void abb_destruir_aux(nodo_t *nodo, abb_destruir_dato_t destructor);
bool abb_in_order_aux(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra);
nodo_t* abb_borrar_aux(nodo_t *nodo, const char *clave, abb_comparar_clave_t comparador, abb_destruir_dato_t destructor, void** dato);
/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol)
        return NULL;
    arbol->destructor = destruir_dato;
    arbol->comparador = cmp;
    arbol->raiz = NULL;
    arbol->cantidad = 0;
    return arbol;
}

nodo_t* crear_nodo(const char *clave, void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo)
        return false;

    nodo->clave = malloc(sizeof(char) * (strlen(clave) + 1) );
    if(!nodo->clave){
        free(nodo);
        return false;
    }
    strcpy((char *) nodo->clave, clave);

    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol)
        return NULL;

    nodo_t* nodo = crear_nodo(clave, dato);
    if (!nodo)
        return false;

    if(!arbol->raiz){
        arbol->cantidad++;
        arbol->raiz = nodo;
        return true;
    }
    if(abb_guardar_aux(arbol->raiz, nodo, arbol->comparador, arbol->destructor))
        arbol->cantidad++;
    return true;
}
bool abb_guardar_aux(nodo_t* raiz, nodo_t* nodo, abb_comparar_clave_t comparador, abb_destruir_dato_t destructor){

    //0 -> Iguales. <0 -> 1º mas grande. >0 2º mas grande.
    if(!comparador(raiz->clave, nodo->clave)){
        if (destructor)
            destructor(raiz->dato);
        raiz->dato = nodo->dato;
        free((void *) nodo->clave);
        free(nodo);
        return false;
    }

    if(comparador(raiz->clave, nodo->clave) < 0){
        if(raiz->izq)
            return abb_guardar_aux(raiz->izq, nodo, comparador, destructor);
        raiz->izq = nodo;
    } else{
        if(raiz->der)
            return abb_guardar_aux(raiz->der, nodo, comparador, destructor);
        raiz->der = nodo;
    }
    return true;
}


void *abb_borrar(abb_t *arbol, const char *clave) {
	if(!arbol || !arbol->raiz || !abb_pertenece(arbol, clave))
        return NULL;

    arbol->cantidad--;
    void* dato = NULL;

    arbol->raiz = abb_borrar_aux(arbol->raiz, clave, arbol->comparador, arbol->destructor, &dato);

    return dato;
}
nodo_t* abb_borrar_aux(nodo_t *nodo, const char *clave, abb_comparar_clave_t comparador, abb_destruir_dato_t destructor, void** dato){
    if(!nodo)
        return NULL;

    if(comparador(nodo->clave, clave) < 0){
        nodo->izq = abb_borrar_aux(nodo->izq, clave, comparador, destructor, dato);
        return nodo;
    }
    else if(comparador(nodo->clave, clave) > 0) {
        nodo->der = abb_borrar_aux(nodo->der, clave, comparador, destructor, dato);
        return nodo;
    }

    //No tiene el izq o (o ninguno)
    if(!nodo->izq){
        *dato = nodo->dato;
        nodo_t* tmp = nodo->der;
        free((void *) nodo->clave);
        free(nodo);
        return tmp;
    }
    if(!nodo->der){
        *dato = nodo->dato;
        nodo_t* tmp = nodo->izq;
        free((void *) nodo->clave);
        free(nodo);
        return tmp;
    }
    //TODO: ¡¿Si tiene dos hijos?!
    //Tiene dos hijos -> Busco el mayor de los menores
    nodo_t* reemplazo = nodo->izq;
    while (reemplazo->der)
        reemplazo = reemplazo->der;

    nodo->clave = reemplazo->clave;
    nodo->dato = reemplazo->dato;

    //Eliminar reemplazo (No la clave, ahora la usa 'nodo')
    free(reemplazo);

    return nodo;
}


void *abb_obtener(const abb_t *arbol, const char *clave) {
    if(!arbol || !arbol->raiz)
        return NULL;
    bool found = false;
	return abb_obtener_aux(arbol->raiz, clave, arbol->comparador, &found);
}
void *abb_obtener_aux(const nodo_t *nodo, const char *clave, abb_comparar_clave_t comparador, bool* found){
    if(!nodo)
        return NULL;

    if(!comparador(nodo->clave, clave)){
        *found = true;
        return nodo->dato;
    }

    if(comparador(nodo->clave, clave) < 0)
        return abb_obtener_aux(nodo->izq, clave, comparador, found);

    return abb_obtener_aux(nodo->der, clave, comparador, found);

    return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    bool found = false;
    abb_obtener_aux(arbol->raiz, clave, arbol->comparador, &found);
    return found;
}

size_t abb_cantidad(abb_t *arbol) {
	if(!arbol)
        return 0;

	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    if(!arbol)
        return;
    abb_destruir_aux(arbol->raiz, arbol->destructor);
    free(arbol);
    return;
}
void abb_destruir_aux(nodo_t *nodo, abb_destruir_dato_t destructor){
    if(!nodo)
        return;
    abb_destruir_aux(nodo->izq, destructor);
    abb_destruir_aux(nodo->der, destructor);

    //TODO: Si la clave es "" evalua false ?!
    if (nodo->clave){
        free((void *) nodo->clave);
        if(destructor)
            destructor(nodo->dato);
    }
    free(nodo);
}



/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERACION
 * *****************************************************************/

struct abb_iter {
	pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    if(!arbol)
        return NULL;
	
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter)
		return NULL;

    iter->pila = pila_crear();
    if(!iter->pila){
        free(iter);
        return NULL;
    }

    if(!arbol->raiz)
        return iter;

    pila_apilar(iter->pila, (void *) arbol->raiz);

    nodo_t* nodo_izq = arbol->raiz->izq;
    while(nodo_izq){
        pila_apilar(iter->pila, nodo_izq);
        nodo_izq = nodo_izq->izq;
    }
	
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    nodo_t* desapilado = pila_desapilar(iter->pila);
    if(!desapilado)
        return false;

    //TODO: Verificar este control.
	if (desapilado->der)
		if(!pila_apilar(iter->pila, desapilado->der)) {
			pila_apilar(iter->pila, desapilado);
			return false;
		}
		
    //TODO: Apilo los hijos izqs del desapilado o los hijos izqs del hijo derecho del desapilado ?
    nodo_t* arbol_izq = desapilado->izq;
    while(arbol_izq){
        pila_apilar(iter->pila, arbol_izq);
        arbol_izq = arbol_izq->izq;
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    nodo_t* tope = pila_ver_tope(iter->pila);
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
    if(!arbol || !arbol->raiz)
        return;
    abb_in_order_aux(arbol->raiz, visitar, extra);
    return;
}
bool abb_in_order_aux(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra){
    if (!nodo)
        return false;

    if(!abb_in_order_aux(nodo->izq, visitar, extra))
        return false;

    if(!visitar(nodo->clave, nodo->dato, extra))
        return false;

    if(!abb_in_order_aux(nodo->der, visitar, extra))
        return false;

    return false;
}