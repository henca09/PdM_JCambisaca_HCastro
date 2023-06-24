#include <Arduino.h>
#include <teclas.h>
#include <uart.h>
// Variables Globales 
bool flanco_SW1 = false;                                //Flanco de bajada B1  
bool flanco_SW2 = false;                                //Flanco de bajada B2


bool buttonPressed(int16_t tecla){ 
    static bool prev_state = true; 
    bool act_state = digitalRead(tecla);                //Lectura del estado lógico del pulsante
    bool result = false;
    //Comparacion del estado lógico actual y anterior
    if (prev_state == true && act_state == false){      
        result = true;                                  //Deteccion del flanco de bajada del pulsante
    }
    else result = false;                               
    prev_state = act_state;                          
    return result; 
}

bool buttonReleased(int16_t tecla){
    static bool prev_state = true;                      //Lectura del estado lógico del pulsante
    bool act_state = digitalRead(tecla);
    bool result = true;
    //Comparacion del estado lógico actual y anterior
    if (prev_state == false && act_state == true){
        result = true;                                 //Deteccion del flanco de subida del pulsante
    }
    else result = false;
    prev_state = act_state; 
    return result; 
}

bool* inicializar_MEF(int16_t SW1, int16_t SW2, int16_t delay2){
    static var_estado es = BU1_BU2; 
    struct_actualizar datos;                            
    bool* vector = (bool*)malloc(2*sizeof(bool));       //Reserva dinámica de memoria
    datos = {SW1,SW2,delay2,es};                        
    es = actualizar_MEF(datos);                         //Actualizacion del estado de 
    
    //Devolver el resultado del flanco de bajada
    if (flanco_SW1) vector[0] = flanco_SW1;
    else vector[0] = false;

    if (flanco_SW2) vector[1] = flanco_SW2; 
    else vector[1] = false;

    flanco_SW1 = false; flanco_SW2 = false;
    return vector;
}

var_estado actualizar_MEF(struct_actualizar var_struct){
    // Variables Estáticas de almacenamiento de tiempo de ejecución
    static int16_t time_pass = 0;
    static int16_t time_present = 0;
    // Variables Estáticas de estados
    static bool es_time = false;        //Estado del delay no bloqueante
    static bool es_SW1 = true;          //Estado del B1 
    static bool es_SW2 = true;          //Estado del B2
    
    // Selector del estado de la MEF 
    switch (var_struct.es1){
    case BU1_BU2:
        imprimir(BU1_BU2_1,0);          //Imprime el Estado actual de la MEF        
        // Detección de un flanco de bajada en cualquier pulsante 
        if (buttonPressed(var_struct.SW1) || buttonPressed(var_struct.SW2)){
            es_SW1 = false; 
            es_SW2 = false;
            return rebounds;
        }
        else {
            es_SW1 = true; 
            es_SW2 = true; 
            return BU1_BU2; 
        }
        break;
    case BU1_BD2:
        imprimir(BU1_BD2_1,0);          //Imprime el Estado actual de la MEF    
        // Detección de un flanco de bajada del Pulsante 1 o un flanco de subida del pulsante 2
        if (buttonPressed(var_struct.SW1) || buttonReleased(var_struct.SW2)){
            es_SW1 = false; 
            es_SW2 = true;
            return rebounds;
        }
        else {
            es_SW1 = true; 
            es_SW2 = false; 
            return BU1_BD2; 
        }
        break;
    case BD1_BU2:
        imprimir(BD1_BU2_1,0);          //Imprime el Estado actual de la MEF   
        // Detección de un flanco de subida del Pulsante 1 o un flanco de bajada del pulsante 2
        if (buttonReleased(var_struct.SW1) || buttonPressed(var_struct.SW2)){
            es_SW1 = true; 
            es_SW2 = false;
            return rebounds;
        }
        else {
            es_SW1 = false; 
            es_SW2 = true; 
            return BD1_BU2; 
        }
        break;
    case BD1_BD2:
        //Serial.println("BD1_BD2");
        imprimir(BD1_BD2_1,0);            //Imprime el Estado actual de la MEF      
        // Detección de un flanco de subida del Pulsante 1 o un flanco de subida del pulsante 2
        if (buttonReleased(var_struct.SW1) || buttonReleased(var_struct.SW2)){
            es_SW1 = true; 
            es_SW2 = true;
            return rebounds;
        }
        else {
            es_SW1 = false; 
            es_SW2 = false; 
            return BD1_BD2; 
        } 
        break;
    case rebounds:
        imprimir(rebounds_1,0);         //Imprime el Estado actual de la MEF     
        // Inicio del retardo no bloqueante cuando es_time = false
        if (es_time == false) {time_pass = millis(); es_time = true;}
        time_present = millis();
        // Verificar si se ha cumplido el delay
        if (time_present-time_pass > var_struct.delay1){
            es_time = false;            //Restauración del estado
            bool es_act_SW1, es_act_SW2;
            
            //Verificación del flanco de bajada una vez pasado el delay  
            if (es_SW1 == digitalRead(var_struct.SW1)) {
                if (es_SW1 == false) flanco_SW1 = true;
                es_act_SW1 = es_SW1;
            }
            else es_act_SW1 = !es_SW1;
            
            if (es_SW2 == digitalRead(var_struct.SW2)) {
                if (es_SW2 == false) flanco_SW2 = true;
                es_act_SW2 = es_SW2;
            }
            else es_act_SW2 = !es_SW2;
            //En base al estado actual y anterior de los pulsantes se ubica al siguiente estado 
            if (es_act_SW1 == false){
                if (es_act_SW2 == false) return BD1_BD2;
                else return BD1_BU2;
            }
            else{
                if (es_act_SW2 == false) return BU1_BD2;
                else return BU1_BU2;
            } 
        }
        else return rebounds;       //En caso de que no haya pasado el delay se retorna nuevamente al estado rebounds
        break;       
    default:
        //Programación Defensiva 
        imprimir(BU1_BU2_1,0); 
        es_SW1 = true;
        es_SW2 = true;
        return BU1_BU2; 
        break;
    }
}