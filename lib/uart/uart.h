#include <Arduino.h>

// Estructura enumerada que engloba los posibles estados del semaforo y teclas
typedef enum {
    BU1_BU2_1,
    BU1_BD2_1,
    BD1_BU2_1,
    BD1_BD2_1,
    rebounds_1,
    normal_verde_1, 
    normal_verde_amarillo_1,
    normal_rojo_1,
    desconectado_amarillo_1,
    desconectado_cero_1,
    alarma_rojo_1, 
    alarma_cero_1
} variable_struct;

// Funcion que muestra en consola el estado actual de la MEF DEL semaforo y teclas
void imprimir(variable_struct variable, float velocidad);

