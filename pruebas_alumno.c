#include "abb.h"
#include "testing.h"

void prueba_crear_hash_vacio() {
    abb_t* abb = abb_crear(NULL, NULL);

    print_test("Prueba crear abb", (bool) abb);
    print_test("Prueba la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

void pruebas_abb_alumno(){
    prueba_crear_hash_vacio();
    /*prueba_iterar_hash_vacio();
    prueba_hash_insertar();
    prueba_hash_reemplazar();
    prueba_hash_reemplazar_con_destruir();
    prueba_hash_borrar();
    prueba_hash_clave_vacia();
    prueba_hash_valor_null();
    prueba_hash_volumen(5000, true);
    prueba_hash_iterar();
    prueba_hash_iterar_volumen(5000);*/
}