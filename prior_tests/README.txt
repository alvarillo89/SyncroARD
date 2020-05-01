Este directorio contiene todos los sketches de Arduino que se realizaron para comprobar 
que, efectivamente, resultaba viable realizar el proyecto en Arduino, así como estudiar
la implementación de la biblioteca:

Incluye:
  - FreeRTOS_test.ino: una prueba de instalar y utilizar FreeRTOS en Arduino.
  - I2C_[Master/Slave].ino: una prueba de comunicación entre dos tarjetas Arduino 
    utilizando I2C y el patrón maestro-esclavo.
  - GPIO_[Master/Slave].ino: una prueba de comunicación entre dos tarjetas Arduino 
    utilizando un pin de entrada/salida de propósito general y el patrón maestro-esclavo.
  - EventGroup_test.ino: una prueba de sincronizar varias tareas de FreeRTOS (en una
    misma tarjeta) utilizando la barrera presente en el complemento de FreeRTOS Event Group.
  - Sync_[Master/Slave].ino: una prueba del código que realizará la biblioteca para la
    sincronización. Este sketch es el que implementa SyncroARD.