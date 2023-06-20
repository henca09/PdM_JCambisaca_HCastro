#include <Arduino.h>
//Variable enum que contienen los posibles estados de la MEF
typedef enum{
    normal_verde, 
    normal_verde_amarillo,
    normal_rojo,
    desconectado_amarillo,
    desconectado_cero,
    alarma_rojo, 
    alarma_cero
}estados;
//Estructura que incluye los Led disponibles y la variable de estado
typedef struct {
    bool SW1; 
    int16_t LED1;
    int16_t LED2;
    int16_t LED3;
    estados es; 
} struct_var;
//Prototipos de funciones
void inicializar_MEF_semaforo(bool SW1, bool SW2, int16_t LED1, int16_t LED2, int16_t LED3);
estados actualizar_MEF_semaforo(struct_var var_struct);
bool tiempo_demora(int16_t delay_1);