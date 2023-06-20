// Decalracion de Librerias
#include <Arduino.h>
#include <teclas.h>
#include <semaforo.h>
// ****************************** Inputs/Outputs ******************************************
int16_t SW1 = 7; int16_t SW2 = 6;                  // Pulsantes
int16_t LED1=11; int16_t LED2=10; int16_t LED3 = 9; // LEDS 

void setup() {
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  digitalWrite(LED1,false);
  digitalWrite(LED2,false);
  Serial.begin(115200);
}
void loop() {
  bool* resultado = inicializar_MEF(SW1, SW2,40);
  inicializar_MEF_semaforo(resultado[0], resultado[1], LED1, LED2, LED3); 
  free(resultado); 
}