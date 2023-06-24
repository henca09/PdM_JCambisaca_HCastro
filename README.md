# PdM_JCambisaca_HCastro
## Organización de Archivos 
- **Programa Principal: ** [/src/main.cpp](/src/)
- **MEF Pulsantes: ** [/lib/teclas/](/PdM_JCambisaca_HCastro/lib/teclas/)
- **MEF Semáforo: ** [/lib/semaforo/](/PdM_JCambisaca_HCastro/lib/semaforo/)
- **UART: ** [/lib/uart/](/PdM_JCambisaca_HCastro/lib/uart/)
## Máquina de Estados
**Maquina de estados para el funcionamiento del Semaforo**
La figura muestra una máquina de estados de tipo More, siendo que la entrada de cada estado sea tiempo o un flanco de bajada de un pulsante. El cambio de estado por tiempo esta definido de tal forma que se cumple con la secuencia definida en los requerimientos de la práctica. Por otro lado, el flanco de bajada del pulsante indica el cambio de secuencia, ya sea de modo Normal, Desconectado y Alarma. 

![screen](/MEF_Semaforo.png?lang=es)
**Maquina de estados para el control de dos pulsantes**

La idea principal de esta máquina de estados es que ante cualquier cambio en los estados lógicos de los pulsantes se dirige al estado *Rebotes*. El estado *Rebotes* es un retardo no bloqueante, de modo de que una vez haya pasado 40ms se compara el estado lógico actual y anterior de cada pulsante para identificar el siguiente estado. La figura que se muestra debajo cumple con la descripción mencionada. 

![screen](/MEF_Pulsantes.jpeg?lang=es)
