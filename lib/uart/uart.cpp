#include <Arduino.h>
#include <uart.h>


void imprimir(variable_struct variable,float velocidad){
    static variable_struct alm1 = BD1_BU2_1; 
    static int16_t alm2 = 1;
    static float alm_velcidad = 2; 
    int16_t verificacion = 1; 

    // Segmentacion de los estados en base  a la entrada de la funcion 
    if (velocidad != 0) {
        if (alm_velcidad != velocidad){
            verificacion = 3; 
        }
        else if (variable == normal_rojo_1 ||variable == normal_verde_1 || variable == normal_verde_amarillo_1){
            verificacion = 0; 
        }
        else if (variable == desconectado_amarillo_1 || variable == desconectado_cero_1){
            verificacion = 1; 
        }
        else {
            verificacion = 2;
        }
    }
    else verificacion = alm2; 

    // En base a la entrada se asigna una salida a la consola 
    // Esto siempre y cuando anteriormente no se repita la misma entrada (Según a la MEF)
    if ((alm1 != variable && (variable == BU1_BU2_1 ||variable == BU1_BD2_1 || variable == BD1_BD2_1 || variable == BD1_BU2_1 || variable == rebounds_1)) ||  alm2 != verificacion){
        switch (variable){
        case BU1_BU2_1:
            Serial.println("SW1: OFF");
            Serial.println("SW2: OFF"); 
            break;
        case BD1_BU2_1:
            Serial.println("SW1: ON");
            Serial.println("SW2: OFF"); 
            break;
        case BU1_BD2_1:
            Serial.println("SW1: OFF");
            Serial.println("SW2: ON"); 
            break;
        case BD1_BD2_1:
            Serial.println("SW1: ON");
            Serial.println("SW2: ON"); 
            break;
        case rebounds_1: 
            Serial.println("Estado: Rebounds"); 
            break;
        case normal_verde_1: 
            Serial.print("Modo: Normal "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        case normal_verde_amarillo_1: 
            Serial.print("Modo: Normal "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        case normal_rojo_1: 
            Serial.print("Modo: Normal "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        case desconectado_amarillo_1: 
            Serial.print("Modo: Desconectado "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        case desconectado_cero_1: 
            Serial.print("Modo: Desconectado "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        case alarma_rojo_1: 
            Serial.print("Modo: Alarma "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        case alarma_cero_1: 
            Serial.print("Modo: Alarma "); 
            Serial.print("Velocidad: X");
            Serial.println(velocidad); 
            break;
        default:
            Serial.println(" "); 
            break;
        }
    Serial.println("*******************************************************");
    }
    //Almacenamiento del estado anterior 
    if (variable == BU1_BU2_1 || variable == BD1_BU2_1 || variable == BU1_BD2_1 || variable == BD1_BD2_1 || variable == rebounds_1) {
        alm1 = variable;
    }
    else {alm2 = verificacion; alm_velcidad = velocidad;}    
    
}