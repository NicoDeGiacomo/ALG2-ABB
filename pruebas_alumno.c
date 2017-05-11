#include <string.h>
#include "abb.h"
#include "testing.h"

void prueba_crear_abb_vacio(){
    abb_t* abb = abb_crear(NULL, NULL);

    print_test("Prueba crear abb", (bool) abb);
    print_test("Prueba la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

void prueba_iterar_abb_vacio(){
    abb_t* abb = abb_crear(NULL, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", (bool) iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

int comparar_clave (const char * clave1, const char * clave2){
    return strcmp(clave1, clave2);
}
void prueba_abb_insertar(){
    abb_t* abb = abb_crear(comparar_clave, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

void pruebas_abb_alumno(){
    prueba_crear_abb_vacio();
    prueba_iterar_abb_vacio();
    //TODO: Seguir agregando estas funciones -> Cupiarlas de las pruebas de catedra para el abb!
    /*prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_volumen(5000, true);
    prueba_abb_iterar();
    prueba_abb_iterar_volumen(5000);*/
}