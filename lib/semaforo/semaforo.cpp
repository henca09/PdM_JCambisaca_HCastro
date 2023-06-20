#include <Arduino.h>
#include <semaforo.h>
#include <uart.h>
float tiempos[] = {2,1,0.5};
float rapidez = 0; 
bool tiempo_demora(int16_t delay_1){
    static bool es = true;
    static unsigned long time_pass = 0; 
    static unsigned long time_pres = 0; 
    bool res = false; 
    
    if (es == true){es = false; time_pass = millis();}
    time_pres = millis();

    if (time_pres-time_pass >delay_1){res = true; es = true;} 
    else {res = false; es=false;}
    return res; 
}

void inicializar_MEF_semaforo(bool SW1, bool SW2, int16_t LED1, int16_t LED2, int16_t LED3){
    static int16_t pos_tiempos = 0; 
    static estados state1 = normal_verde;
    struct_var datos; 
    datos = {SW1,LED1,LED2,LED3,state1};
    if (SW2) pos_tiempos=(pos_tiempos+1)%3;
    rapidez = tiempos[pos_tiempos]; 
    state1 = actualizar_MEF_semaforo(datos); 
}
estados actualizar_MEF_semaforo(struct_var var_struct){
    estados es1;
    switch (var_struct.es){
    case normal_verde:
        if (var_struct.SW1) es1 = desconectado_amarillo; 
        else{
            if (tiempo_demora((int16_t)(3000*rapidez))) es1 = normal_verde_amarillo; 
            else es1 = normal_verde;
        } 
        digitalWrite(var_struct.LED1,HIGH);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,LOW);
        imprimir(normal_verde_1,rapidez);
        break;
    case normal_verde_amarillo: 
        if (var_struct.SW1) es1 = desconectado_amarillo; 
        else{
            if (tiempo_demora((int16_t)(500*rapidez))) es1 = normal_rojo; 
            else es1 = normal_verde_amarillo;
        } 
        digitalWrite(var_struct.LED1,HIGH);
        digitalWrite(var_struct.LED2,HIGH);
        digitalWrite(var_struct.LED3,LOW);
        imprimir(normal_verde_amarillo_1,rapidez);
        break;
    case normal_rojo: 
        if (var_struct.SW1) es1 = desconectado_amarillo; 
        else{
            if (tiempo_demora((int16_t)(2000*rapidez))) es1 = normal_verde; 
            else es1 = normal_rojo;
        } 
        digitalWrite(var_struct.LED1,LOW);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,HIGH);
        imprimir(normal_rojo_1,rapidez);
        break;
    case desconectado_amarillo: 
        if (var_struct.SW1) es1 = alarma_rojo; 
        else{
            if (tiempo_demora((int16_t)(500*rapidez))) es1 = desconectado_cero; 
            else es1 = desconectado_amarillo;
        } 
        digitalWrite(var_struct.LED1,LOW);
        digitalWrite(var_struct.LED2,HIGH);
        digitalWrite(var_struct.LED3,LOW);
        imprimir(desconectado_amarillo_1,rapidez);
        break;
    case desconectado_cero: 
        if (var_struct.SW1) es1 = alarma_rojo; 
        else{
            if (tiempo_demora((int16_t)(500*rapidez))) es1 = desconectado_amarillo; 
            else es1 = desconectado_cero;
        } 
        digitalWrite(var_struct.LED1,LOW);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,LOW);
        imprimir(desconectado_cero_1,rapidez);
        break;
    case alarma_rojo: 
        if (var_struct.SW1) es1 = normal_verde; 
        else{
            if (tiempo_demora((int16_t)(500*rapidez))) es1 = alarma_cero; 
            else es1 = alarma_rojo;
        } 
        digitalWrite(var_struct.LED1,LOW);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,HIGH);
        imprimir(alarma_rojo_1,rapidez);
        break;
    case alarma_cero:
        if (var_struct.SW1) es1 = normal_verde; 
        else{
            if (tiempo_demora((int16_t)(500*rapidez))) es1 = alarma_rojo; 
            else es1 = alarma_cero;
        } 
        digitalWrite(var_struct.LED1,LOW);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,LOW);
        imprimir(alarma_cero_1,rapidez);
        break;
    default:
        es1 = normal_verde;
        digitalWrite(var_struct.LED1,HIGH);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,LOW);
        break;
    }
    return es1;
}