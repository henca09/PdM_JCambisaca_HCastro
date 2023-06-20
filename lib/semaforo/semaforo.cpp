#include <Arduino.h>
#include <semaforo.h>
#include <uart.h>
//Vector de que contiene el ajuste de velocidad
float tiempos[] = {2,1,0.5};
//Inicializacion de la variable de velocidad
float rapidez = 0; 
//Funcion de retardo no bloqueante
bool tiempo_demora(int16_t delay_1){
    //Bandera que cambia su estado cuando el tiempo fue cumplido
    static bool es = true;
    //Variable que almacena el tiempo pasado
    static unsigned long time_pass = 0;
    //Variable que almacena el tiempo presente
    static unsigned long time_pres = 0;
    //Salida de la funcion
    bool res = false; 
    //Inicializacion luego de cumplir el tiempo
    if (es == true){es = false; time_pass = millis();}
    time_pres = millis();
    //Condicional de comparacion
    if (time_pres-time_pass >delay_1){res = true; es = true;} 
    else {res = false; es=false;}
    return res; 
}

//Funcion de inicializacion de la maquina de estados para controlar la secuencia de LEDs
void inicializar_MEF_semaforo(bool SW1, bool SW2, int16_t LED1, int16_t LED2, int16_t LED3){
    //Inicializacion de variables
    static int16_t pos_tiempos = 0;
    static estados state1 = normal_verde;
    struct_var datos;
    //Vector de LEDs
    datos = {SW1,LED1,LED2,LED3,state1};
    //Contador modulo 3 (Cambia de velocidad)
    if (SW2) pos_tiempos=(pos_tiempos+1)%3;
    rapidez = tiempos[pos_tiempos];
    state1 = actualizar_MEF_semaforo(datos);
}
//Funcion para actualizar la maquina de estados del semaforo
estados actualizar_MEF_semaforo(struct_var var_struct){
    estados es1; //Variable de estado
    switch (var_struct.es){
    case normal_verde:
    //Encender el LED verde 3 s
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
    //Encender el LED verde y LED amarillo 500 ms
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
    //Encender el LED rojo durante 2 segundos
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
    //Led amarillo intermitente cada 500 ms
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
    //Apaga todos los Led cuando esta en estado desconectado
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
    //Estado de alarma cambia al siguiente estado en caso de cumplir la condicion
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
    //Apaga todos los Led cuando estra en estado de alarma
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
    //Valores por defecto en caso de falla
        es1 = normal_verde;
        digitalWrite(var_struct.LED1,HIGH);
        digitalWrite(var_struct.LED2,LOW);
        digitalWrite(var_struct.LED3,LOW);
        break;
    }
    return es1;
}