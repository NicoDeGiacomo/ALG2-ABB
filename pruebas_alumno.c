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

void prueba_iterar_hash_vacio()
{
    abb_t* hash = abb_crear(NULL, NULL);
    abb_iter_t* iter = abb_iter_in_crear(hash);
    print_test("Prueba hash iter crear iterador hash vacio", (bool) iter);
    print_test("Prueba hash iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba hash iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba hash iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(hash);
}

void pruebas_abb_alumno(){
    prueba_crear_hash_vacio();
    prueba_iterar_hash_vacio();
    //TODO: Seguir agregando estas funciones -> Cupiarlas de las pruebas de catedra para el hash!
    /*prueba_hash_insertar();
    prueba_hash_reemplazar();
    prueba_hash_reemplazar_con_destruir();
    prueba_hash_borrar();
    prueba_hash_clave_vacia();
    prueba_hash_valor_null();
    prueba_hash_volumen(5000, true);
    prueba_hash_iterar();
    prueba_hash_iterar_volumen(5000);*/
}