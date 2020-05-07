# Desarrollo de la biblioteca

En este documento se describe con detalle los apectos de implementación de `SyncroARD`: el proceso seguido, los problemas encontrados, así como los planes de futuro que se tienen sobre la biblioteca.

**Tabla de contenidos:**

- [1 Mecanismos de comunicación](#1-mecanismos-de-comunicación)
- [2 Time Triggering VS Event Triggering](#2-time-triggering-vs-event-triggering)
- [3 Mecanismos de sincronización](#3-mecanismos-de-sincronización)
  - [3.1 Enfoque 1 (Sincronización de los relojes de ambas tarjetas)](#3.1-enfoque-1-(sincronización-de-los-relojes-de-ambas-tarjetas))

## 1 Mecanismos de comunicación

En un sistema distribuido en el que se produzca interacción entre los dispositivos, debe existir un mecanismo que permita mandar información entre ellos, es decir, un mecanismo (o varios) de comunicación. En Arduino, existen tres formas diferentes de comunicar tarjetas:

- **Puerto serie**: Un puerto es el nombre genérico con que se denomina a los interfaces, físicos o virtuales, que permiten la comunicación entre dos ordenadores o dispositivos. Un puerto serie envía la información mediante una secuencia de bits. Para ello, se necesitan al menos dos conectores para realizar la comunicación de datos, RX (recepción) y TX (transmisión). Prácticamente todas las placas Arduino disponen al menos de un puerto serie (también conocido como unidad UART, _universally asynchronous receiver/transmitter_). La placa Arduino UNO dispone de una unidad UART que opera a nivel TTL (_transistor-transistor logic_) 0V / 5V en los pines 0 (RX) y 1 (TX).
- **Bus SPI**: El bus SPI (_Serial Peripheral Interface_) fue desarrollado por Motorola en 1980. Tiene una arquitectura de tipo maestro-esclavo. El dispositivo maestro (master) puede iniciar la comunicación con uno o varios dispositivos esclavos (slave), y enviar o recibir datos de ellos. Los dispositivos esclavos no pueden iniciar la comunicación, ni intercambiar datos entre ellos directamente. La comunicación de datos entre maestros y esclavo se realiza en dos líneas independientes, una del maestro a los esclavos, y otra de los esclavos al maestro. Por tanto la comunicación es Full Duplex, es decir, el maestro puede enviar y recibir datos simultáneamente. Otra característica de SPI es que es síncrono. Los pines asociados a SPI varían de un modelo a otro. Para Arduino Uno son el 10, 11, 12 y 13.
- **Bus I2C**: El estándar I2C (_Inter-Integrated Circuit_) fue desarrollado por Philips en 1982. Requiere únicamente dos cables para su funcionamiento, uno para la señal de reloj (CLK) y otro para el envío de datos (SDA), lo cual es una ventaja frente al bus SPI. Por contra, su funcionamiento es un poco más complejo, así como la electrónica necesaria para implementarla. En el bus, cada dispositivo dispone de una dirección, que se emplea para acceder a los dispositivo de forma individual. Tiene una arquitectura maestro esclavo, la cual permite más de un maestro (aunque solo uno puede ser maestro cada vez) y también es síncrono. Arduino dispone de soporte I2C por hardware vinculado físicamente a ciertos pines. En Arduino Uno, está vinculado a los pines analógicos 4 y 5.

El puerto serie está pensando especialmente para comunicar una tarjeta de Arduino con un ordenador mediante un USB. Utilizarlo para comunicar dos Arduinos puede resultar engorroso, por lo que será descartado. Esto nos deja dos posible candidatos para la comunicación: SPI e I2C. A continuación se muestran las ventajas e incovenientes de cada uno:

**SPI:**

- Ventajas:
  - Alta velocidad de trasmisión (hasta 8 Mhz en Arduino) y Full Duplex
  - Los dispositivos necesarios son sencillos y baratos, lo que hace que esté integrado en muchos dispositivos.
  - Puede mandar secuencias de bit de cualquier tamaño, sin dividir y sin interrupciones.
- Inconvenientes:
  - Se requiere 3 cables (SCK, MOSI y MISO) + 1 cable adicional (SS) por cada dispositivo esclavo.
  - Solo es adecuado a corta distancias (unos 30cm).
  - No se dispone de ningún mecanismo de control, es decir, no podemos saber si el mensaje ha sido recibido y menos si ha sido recibido correctamente.
  - La longitud de los mensajes enviados y recibidos tiene que ser conocida por ambos dispositivos.
  - La programación es engorrosa.

**I2C:**

- Ventajas:
  - Mas sencillo de programar.
  - Requiere pocos cables.
  - Dispone de mecanismos para verificar que la señal hay llegado.
- Inconvenientes:
  - Su velocidad es media-baja
  - No es full duplex
  - No hay verificación de que el contenido del mensaje es correcto

El bus SPI presenta características interesantes, como la comunicación Full Duplex y la velocidad de tranmisión. No obstante, I2C es mucho más sencillo de utilizar (tanto a nivel de conexiones entre las tarjetas, como a nivel de programación). De cara a construir una biblioteca, la sencillez es un aspecto relevante. Además, I2C es el método más popular para conectar tarjetas de Arduino. La propia página oficial de Arduino tiene un tutorial sobre como realizar un Master Writer/Slave reader utilizando el bus I2C.

En definitiva, pese a tener una valocidad de transmisión más lenta, **I2C** será el principal **mecanismo de comunicación** de `SycroARD` (aún así, en la [caracterízación del tiempo](#5-caracterizacion-del-tiempo-de-comunicacion) se observa que el tiempo de transmisión es más que razonable).

## 2 Time Triggering VS Event Triggering

El principal objetivo de la sincronización es lograr que una determinada tarea o procedimiento se ejecute al mismo tiempo en un conjunto de dispositivos. En consecuencia, debe de existir un mecanismo que dispare (_triggering_) la ejecución de la tarea en cada una de las tarjetas. En sistemas de tiempo real, este _triggering_ puede ser de dos tipos: **temporal** o **por eventos**:

- Disparo temporal (_time triggering_): en este tipo de sistemas, se produce una interrupción del reloj cada _ΔT_ milisegundos. Durante esas interrupciones, se muestrean los sensores y se accionan ciertos actuadores. El _ΔT_ debe ser elegido con extremo cuidado. Si es demasiado pequeño, el sistema experimentará muchas interrupciones de reloj y perderá demasiado tiempo manejándolas. Si es demasiado grande, los eventos pueden no ser detectados. Además, la decisión sobre qué sensores comprobar en cada tick del reloj es crítica.
- Disparo por eventos (_event triggering_): en este tipo de sistemas, las interrupciones se producen a causa de un evento externo, algo detectado por un sensor. Para sistemas de tiempo real suaves con mucha potencia de computación, este enfoque es simple, funciona bien y sigue siendo ampliamente utilizado. Su principal inconveniente es que pueden fallar en condiciones de mucha carga, es decir, cuando se producen muchos eventos simultáneos.

En definitiva, los sistemas disparados por eventos dan una respuesta más rápida cuando la carga es baja, pero si la carga es alta, tiene más _overhead_ y posibilidades de fallo. Los sistemas disparados por tiempo tienen las propiedades opuestas, y sólo son adecuados en entornos relativamente estáticos en los que se sabe de antemano mucho sobre el comportamiento del sistema. Cuál es mejor depende de la aplicación.

Para `SyncroARD`, tras varias pruebas, se utilizó finalmente **event triggering** para la implementación : el maestro manda a través de los mecanismos de comunicación una señal que genera una interrupción encargada de ejecutar la tarea en el esclavo . Pese a que esta es la decisión final, inicialmente se optó por un enfoque híbrido, aunque los problemas encontrados lo hicieron inviable. Todo esto se describe en el siguiente apartado.

## 3 Mecanismos de sincronización

En este apartado se describen todos los enfoques que se han ido adoptado durante el desarrollo de la biblioteca para afrontar la sincronización entre las tarjetas, cuáles fueron sus inconvenientes y por qué se desecharon hasta alcanzar la solución final.

### 3.1 Enfoque 1 (Sincronización de los relojes de ambas tarjetas)

En este primer enfoque, la sincronización funcionaría de la siguiente forma:

1. Se sincronizan la frecuencia y los ticks de reloj de las CPU de ambas tarjetas. Para ello, el tick y frecuencia del maestro se asignan al tick y frecuencia de la tarjeta esclavo.
2. Puesto que las frecuencias y los ticks están sincronizados, simplemente la tarea debe comenzar en el mismo tick en ambas tarjetas.
3. Periódicamemente, se vuelve a comprobar que la frecuencia y los ticks de ambas tarjetas sigan sincronizados. En caso de que no, se vuelven a reasignar.

**Problemas:**

- El reloj de FreeRTOS de Arduino es sólo de lectura, no hay forma de modificarlo. Como mucho, se puede consultar el número de ticks de reloj que han pasado desde que se encendió la tarjeta, para cada tarea ([xTaskGetTickCount](https://www.freertos.org/a00021.html#xTaskGetTickCount)). Si no se puede modificar el reloj, esta solución es completamente inviable.

### 3.2 Enfoque 2 (EventGroup de FreeRTOS)

### 3.3 Enfoque 3 (Comienzo simultáneo y sincronización periódica)

### 3.4 Enfoque Final (Sincronización basada en Eventos)

## 4 Redundancia en la comunicación

## 5 Caracterización del tiempo de comunicación

## 6 Trabajo futuro

## Referencias

- [Comunicación de Arduino con puerto serie](https://www.luisllamas.es/arduino-puerto-serie/)
- [El bus SPI en Arduino](https://www.luisllamas.es/arduino-spi/)
- [El bus I2C en Arduino](https://www.luisllamas.es/arduino-i2c/)
- [Event-Triggered versus Time-Triggered Systems](https://www.e-reading-lib.com/chapter-amp.php/143358/161/andrew-tanenbaum-distributed-operating-systems.html)
