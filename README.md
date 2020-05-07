[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Generic badge](https://img.shields.io/badge/version-v1.0.0-GREEN.svg)](https://shields.io/)
[![Ask Me Anything !](https://img.shields.io/badge/Ask%20me-anything-1abc9c.svg)](https://GitHub.com/Naereen/ama)

# SyncroARD

###### Proyecto desarrollado para la asignatura Sistemas Críticos del Máster en Ingeniería Informática (UGR).

SyncroARD constituye una biblioteca de Arduino escrita en C++ para sincronizar cualquier tipo de tarea en un sistema de dos tarjetas distribuidas. Proporciona una API de alto nivel para permitir que una tarea definida se ejecute exactamente al mismo tiempo en ambos dispositivos, a la vez que garantiza la confiabilidad (_reliability_) del sistema.

SyncroARD está construida sobre [**FreeRTOS**](https://www.freertos.org/), más concretamente, utiliza por debajo la biblioteca [Arduino_FreeRTOS_Library](https://github.com/feilipu/Arduino_FreeRTOS_Library). Para más información sobre cómo se tomó esta decisión, consulte el [siguiente enlace](https://alvarillo89.github.io/SyncroARD/docs/comparativa).

---

## ¿Cómo funciona?

Por el momento, SyncroARD solo permite sincronizar una única tarea en dos tarjetas, donde una asumirá el rol de Maestro y la otra, de Esclavo. La biblioteca lleva acabo una sincronización basada en eventos, en la que el Maestro avisa al Esclavo del momento exacto en el que debe ejecutarse la tarea. Este aviso se manda a través de dos canales diferentes: un bus I2C y un pulso a través de un Pin de Entrada/Salida de Propósito General (GPIO). La comunicación es crítica para el proceso de sincronización, de ahí que exista redundancia en los canales.

---

## Quickstart

Sigue estos pasos si quieres utilizar SyncroARD:

1. Asegúrate de tener instalada la biblioteca de Arduino _"Arduino FreeRTOS Library"_. Sigue los pasos del punto _Getting Started with FreeRTOS_ del [siguiente enlace](https://feilipu.me/2015/11/24/arduino_freertos/) para instalarla.
2. Clona o descarga este repositorio y coloca el directorio [**SyncroARD/**](https://github.com/alvarillo89/SyncroARD/tree/master/SyncroARD) bajo la ruta en la que se alojen las bibliotecas de Arduino en tu sistema. En Linux por ejemplo, deberás colocarla bajo el directorio `~/Arduino/libraries/`. Puedes consultar más información sobre como instalar una biblioteca manualmente en el [siguiente enlace](https://www.arduino.cc/en/Guide/Libraries#toc5).
3. Conecta ambas tarjetas de la siguiente forma:
   ![](/docs/imgs/Connection.png)

   - La conexión a tierra (GND) entre ambas tarjetas es obligatoria.
   - La conexión entre los pines digitales número 2 son para la comunicación mediante GPIO.
   - La conexión entre los pines analógicos 4 y 5 son para el bus I2C.

4. Para hacer tu código, parte de las plantillas para el Maestro y el Esclavo presentes en el directorio [**templates/**](https://github.com/alvarillo89/SyncroARD/tree/master/templates).

### Master.ino

```cpp
/*
    Master.ino: synchronization template for the
    master card.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

#include <Arduino_FreeRTOS.h>   // FreeRTOS required on master
#include <SyncroARD.h>          // Include SyncroARD Library


// FreeRTOS tasks definitions:
void I2C( void *pvParameters );
void GPIO( void *pvParameters );
void Task( void *pvParameters );


void setup() {
  // Init library. Use SyncroM class for master
  SyncroM::begin();
  // Create the FreeRTOS tasks. They must all have the same priority.
  xTaskCreate(I2C, "I2C", 128, NULL, 1, NULL);    // I2C communication
  xTaskCreate(GPIO, "GPIO", 128, NULL, 1, NULL);  // GPIO communication
  xTaskCreate(Task, "Task", 128, NULL, 1, NULL);  // Your task
  /*
      * Other initialization stuff
      * ...
      * ...
  */
}


// Empty main loop
void loop() { delay(10000); }


// This code block carries out communication via I2C.
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void I2C( void *pvParameters ) {
  (void) pvParameters;
  for(;;) { SyncroM::notifyI2C(); }
}


// This code block carries out communication via GPIO.
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void GPIO( void *pvParameters ) {
  (void) pvParameters;
  for(;;) { SyncroM::notifyGPIO(); }
}


// Your task
void Task( void *pvParameters ) {
  (void) pvParameters;

  for(;;) {
    SyncroM::syncTask();
    /*
        * YOUR TASK CODE GOES HERE
        * ...
        * ...
    */
  }
}
```

### Slave.ino

```cpp
/*
    Slave.ino: synchronization template for the
    slave card.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

// FreeRTOS is optional in Slave
#include <SyncroARD.h>     // Include SyncroARD Library


void setup() {
  // Init library. Use SyncroS class for slave.
  // It receives the functions that manage the interruption
  // of I2C and GPIO respectively. If you don't want to use
  // any of them, pass NULL as parameter
  SyncroS::begin(I2C, GPIO);
  // Tell the library the function of your task. It must
  // return void and not take arguments
  SyncroS::attachTask(Task);
  /*
      * Other initialization stuff
      * ...
      * ...
  */
}


// Keep the main loop like this. This will call your linked task
// at the right time
void loop() {
  SyncroS::doSyncTask();
}


// This code block carries out communication via I2C.
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void I2C(int howMany) {
  SyncroS::receiveI2C();
}


// This code block carries out communication via GPIO.
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void GPIO() {
  SyncroS::receiveGPIO();
}


// Task must return void and not take arguments
void Task() {
  /*
        * YOUR TASK CODE GOES HERE
        * WITHOUT INFINITE LOOPS
        * ...
    */
}
```

También puedes consultar [**este ejemplo**](https://github.com/alvarillo89/SyncroARD/tree/master/examples), el cual hace parpadear el led incorporado de Arduino al mismo tiempo en ambas tarjetas.

---

## Licencia

Este proyecto está licenciado bajo los términos de la licencia GNU General Public License v3.0.
