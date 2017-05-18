#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;

/* Tipo de función para comparar claves.
 */
typedef int (*abb_comparar_clave_t) (const char *, const char *);

/* Tipo de función para destruir datos.
 */
typedef void (*abb_destruir_dato_t) (void *);



/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

/* Crea un abb.
 * Post: devuelve un nuevo abb vacío. 'destruir_dato' es una función capaz de destruir
 * los datos del abb, o NULL en caso de que no se la utilice
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: El abb fue creado.
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: El abb fue creado.
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: El abb fue creado.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al abb.
 * Pre: El abb fue creado.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: El abb fue creado.
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir_dato para cada dato.
 * Pre: El abb fue creado.
 * Pre: El abb fue destruido.
 */
void abb_destruir(abb_t *arbol);



/* ******************************************************************
 *                    PRIMITIVAS DE LA ITERACION
 * *****************************************************************/

typedef struct abb_iter abb_iter_t;

/* Crea iterador de sentido inorder.
 * Pre: El abb fue creado.
 */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza el iterador.
 * Pre: El iterador fue creado.
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Devuelve clave actual, esa clave no se puede modificar ni liberar.
 * Pre: El iterador fue creado.
 */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Comprueba si terminó la iteración.
 * Pre: El iterador fue creado.
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador.
 * Pre: El iterador fue creado.
 */
void abb_iter_in_destruir(abb_iter_t* iter);



/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

/* Itera sobre el abb y aplica la funcion visitar a cada par (clave, dato).
 * Pasandole a la misma el parametro extra (para uso del usuario).
 * Si la funcion visitar devuelve false se deja de iterar.
 * Pre: El abb fue creado.
 * Post: Se aplicó la funcion visitar a todos los datos.
 */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);



/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del abb.
void pruebas_abb_alumno(void);

#endif // ABB_H