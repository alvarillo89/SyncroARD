# Desarrollo de la biblioteca

En este documento se describe con detalle los apectos de implementación de `SyncroARD`: el proceso seguido, los problemas encontrados, así como los planes de futuro que se tienen sobre la biblioteca.

**Tabla de contenidos:**

- [1 Mecanismos de comunicación](#1-mecanismos-de-comunicación)
- [2 Time Triggering VS Event Triggering](#2-time-triggering-vs-event-triggering)
- [3 Mecanismos de sincronización](#3-mecanismos-de-sincronización)
  - [3.1 Enfoque 1 (Sincronización de los relojes de ambas tarjetas)](#31-enfoque-1-sincronización-de-los-relojes-de-ambas-tarjetas)
  - [3.2 Enfoque 2 (EventGroup de FreeRTOS)](#32-enfoque-2-eventgroup-de-freertos)
  - [3.3 Enfoque 3 (Comienzo simultáneo y sincronización periódica)](#33-enfoque-3-comienzo-simultáneo-y-sincronización-periódica)
  - [3.4 Enfoque Final (Sincronización basada en Eventos)](#34-enfoque-final-sincronización-basada-en-eventos)
- [4 Redundancia en la comunicación](#4-redundancia-en-la-comunicación)
- [5 Caracterización del tiempo de comunicación](#5-caracterización-del-tiempo-de-comunicación)
- [6 Trabajo futuro](#6-trabajo-futuro)
- [Referencias](#referencias)

## 1 Mecanismos de comunicación

En un sistema distribuido en el que se produzca interacción entre los dispositivos, debe existir un mecanismo que permita mandar información entre ellos, es decir, un mecanismo (o varios) de comunicación. En Arduino, existen tres formas diferentes de comunicar tarjetas:

- **Puerto serie**: Un puerto es el nombre genérico con que se denomina a los interfaces, físicos o virtuales, que permiten la comunicación entre dos ordenadores o dispositivos. Un puerto serie envía la información mediante una secuencia de bits. Para ello, se necesitan al menos dos conectores para realizar la comunicación de datos, RX (recepción) y TX (transmisión). Prácticamente todas las placas Arduino disponen al menos de un puerto serie (también conocido como unidad UART, _universally asynchronous receiver/transmitter_). La placa Arduino UNO dispone de una unidad UART que opera a nivel TTL (_transistor-transistor logic_) 0V / 5V en los pines 0 (RX) y 1 (TX).
- **Bus SPI**: El bus SPI (_Serial Peripheral Interface_) fue desarrollado por Motorola en 1980. Tiene una arquitectura de tipo maestro-esclavo. El dispositivo maestro (master) puede iniciar la comunicación con uno o varios dispositivos esclavos (slave), y enviar o recibir datos de ellos. Los dispositivos esclavos no pueden iniciar la comunicación, ni intercambiar datos entre ellos directamente. La comunicación de datos entre maestros y esclavo se realiza en dos líneas independientes, una del maestro a los esclavos, y otra de los esclavos al maestro. Por tanto la comunicación es Full Duplex, es decir, el maestro puede enviar y recibir datos simultáneamente. Otra característica de SPI es que es síncrono. Los pines asociados a SPI varían de un modelo a otro. Para Arduino Uno son el 10, 11, 12 y 13.
- **Bus I2C**: El estándar I2C (_Inter-Integrated Circuit_) fue desarrollado por Philips en 1982. Requiere únicamente dos cables para su funcionamiento, uno para la señal de reloj (CLK) y otro para el envío de datos (SDA), lo cual es una ventaja frente al bus SPI. Por contra, su funcionamiento es un poco más complejo, así como la electrónica necesaria para implementarlo. En el bus, cada dispositivo dispone de una dirección, que se emplea para acceder a los dispositivos de forma individual. Tiene una arquitectura maestro esclavo, la cual permite más de un maestro (aunque solo uno puede ser maestro cada vez) y también es síncrono. Arduino dispone de soporte I2C por hardware vinculado físicamente a ciertos pines. En Arduino Uno, está vinculado a los pines analógicos 4 y 5.

El puerto serie está pensando especialmente para comunicar una tarjeta de Arduino con un ordenador mediante un USB. Utilizarlo para comunicar dos Arduinos puede resultar engorroso, por lo que será descartado. Esto nos deja dos posible candidatos para la comunicación: SPI e I2C. A continuación se muestran las ventajas e incovenientes de cada uno:

**SPI:**

- Ventajas:
  - Alta velocidad de trasmisión (hasta 8 Mhz en Arduino) y Full Duplex.
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
  - Su velocidad es media-baja.
  - No es full duplex.
  - No hay verificación de que el contenido del mensaje es correcto.

El bus SPI presenta características interesantes, como la comunicación Full Duplex y la velocidad de tranmisión. No obstante, I2C es mucho más sencillo de utilizar (tanto a nivel de conexiones entre las tarjetas, como a nivel de programación). De cara a construir una biblioteca, la sencillez es un aspecto relevante. Además, I2C es el método más popular para conectar tarjetas de Arduino. La propia página oficial de Arduino tiene un tutorial sobre como realizar un [Master Writer/Slave Reader](https://www.arduino.cc/en/Tutorial/MasterWriter) utilizando el bus I2C.

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

**Problema:**

El reloj de FreeRTOS de Arduino es sólo de lectura, no hay forma de modificarlo. Como mucho, se puede consultar el número de ticks de reloj que han pasado desde que se encendió la tarjeta, para cada tarea ([xTaskGetTickCount](https://www.freertos.org/a00021.html#xTaskGetTickCount)). Si no se puede modificar el reloj, esta solución es completamente inviable.

### 3.2 Enfoque 2 (EventGroup de FreeRTOS)

Puesto que no se puede modificar el reloj, la siguiente idea que se pensó para intentar sincronizar las tareas, pasa por el uso de mecanismos clásicos de los sistemas concurrentes y distribuidos: semáforos binarios, mutex, barreras, etcétera. Concretamente, FreeRTOS dispone de una biblioteca complementaria que permite sincronizar varias tareas. Se trata de la biblioteca `event_group.h`.

Este complemento proporciona una barrera. En computación paralela, una barrera, para un grupo de hilos o procesos, significa que todos los que implementen esta barrera deberán parar en ese punto sin poder ejecutar las siguientes líneas de código hasta que todos los restantes hilos/procesos hayan alcanzado esta barrera. `Event_group.h` proporciona el método [xEventGroupSync](https://www.freertos.org/xEventGroupSync.html), una barrera para las tareas de FreeRTOS.

La idea de la sincronización para este enfoque es la siguiente: antes de la ejecución de la tarea en el bucle principal, se coloca la barrera `xEventGroupSync`. De esta forma, las tarjetas deberán esperarse la una a la otra hasta que ambas se encuentren al principio de la tarea. Si esto se hace en cada iteración del bucle principal, las tareas se ejecutarán a la par.

**Problema:**

Para funcionar, `xEventGroupSync` necesita de una estructura de datos global en la que almacenar la información de la barrera (`EventGroupHandle_t`). Dicha estrutura debería ser compartida por ambas tarjetas. Sin embargo, Arduino no dispone de ningún mecanismo de memoria compartida (_Shared Memory_).

### 3.3 Enfoque 3 (Comienzo simultáneo y sincronización periódica)

`xEventGroupSync` no puede utilizarse para sincronizar tareas de tarjetas diferentes, pero sí puede sincronizar tareas de la misma tarjeta. La idea de este tercer enfoque es la siguiente:

1. El maestro tendrá dos tareas de `FreeRTOS` sincronizadas con la barrera de `Event Group`. Una será la tarea que se quiere sincronizar. La otra, se encargará de enviar a través del bus I2C una señal (un byte que no necesita ser leído) a la tarjeta esclavo. La idea es que la tarea comience a ejecutarse al mismo tiempo que se notifica a la tarjeta esclavo.
2. El esclavo estará en espera activa hasta que reciba la notificación del maestro. Cuando la reciba, empezará a ejecutar la tarea en su `loop`.

De esta forma, las tareas comenzarán a ejecutarse a la vez en ambas tarjetas, con una diferencia de tiempo igual al tiempo del transmisión del bus I2C. **Esta sincronización solo se realiza al comienzo, cuando ambas tarjetas se conectan.**

**Problema:**

Las tareas comienzan a ejecutarse al mismo tiempo, pero al cabo del rato, una de las tarjetas acaba desfasándose. Para solucionar esto, cada cierto tiempo, el maestro debería comprobar el desfase (en ticks de reloj o en tiempo) existente entre ambas tarjetas y esperar (u ordenar al esclavo que espere, depende de quien vaya por detrás) tanto tiempo como sea necesario antes de realizar una nueva iteración del bucle principal, para así volver al estado de sincronización. El inconveniente de esta solución es que no existe una fuente temporal común a ambas tarjetas. Cada tiempo es relativo al punto en el que se encendió la tarjeta, lo que hace imposible medir el desfase. 

### 3.4 Enfoque Final (Sincronización basada en Eventos)

El funcionamiento es exactamente el mismo que en el enfoque anterior, solo que ahora, el maestro no sólo notifica al esclavo al principio, sino que lo notifica en cada iteración del bucle principal de la tarea. Por su parte, el esclavo realizará una unica iteración de la tarea cada vez que reciba una notificación del maestro.

Dicho de otra forma, cada iteración en el maestro se sincroniza (usando `Event Group`) con el envío de la notificación al esclavo. Cada vez que el esclavo reciba una notificación por el bus I2C, realizará una sola ejecución de la tarea. 

Se trata de una sincronización basada puramente en _event triggering_. Ahora no se produce nigún desfase, y la diferencia temporal en la ejecución de las tareas depende, exclusivamente, del tiempo de transmisión por el bus I2C.

## 4 Redundancia en la comunicación

La sincronización depende integramente del bus I2C. Si por cualquier motivo la notificación no se envia (o no llega) al esclavo, este no ejecutará la tarea. Para mejorar la _reliability_ del sistema, se añadirá un segundo mencanismo de comunicación (redundancia). 

Podría añadirse un segundo bus I2C, pero la literatura recomienda que cuando se añade redundancia a un sistema crítico, el segundo mecanismo sea de una tecnología diferente. Si fuesen las mismas, el defecto o fallo de una puede replicarse en la otra, anulando completamente la ventaja que proporciona la redundancia. 

Por ejemplo, podría utilizarse el bus SPI. No obstante, al ser un mecanismo de respaldo, quizás SPI sea demasiado complejo. Además, cuanto más sencillos sean los mecanismos/procesos, menor será el riesgo de fallo. Por este motivo, y puesto que realmente no es necesario mandar ninguna información relevante por los canales, solo una señal o notificación, se empleó un Pin de Entrada/Salida de Propósito General (GPIO) como segundo mecanismo de comunicación. Ahora el proceso es el siguiente:

1. El maestro sincroniza tres tareas con la barrera de `Event Group`: la ejecución de la tarea sincronizada, el envío de la notificación por el bus I2C, y el envío de un pulso de voltaje a través del pin 2.
2. El esclavo ejecuta la tarea si ha recibido un byte por el bus I2C o un pulso por el pin 2.

De esta forma, si la notificación por el bus I2C no llega, la tarea se seguirá ejecutando gracias al pulso del GPIO. Además, hay que decir que no es obligatorio el uso de ambos canales. `SyncroARD` está programado para que se pueda utilizar uno, otro o ambos.

## 5 Caracterización del tiempo de comunicación

// TODO

## 6 Trabajo futuro

A continuación se listan algunas ideas que pueden contribuir a mejorar la biblioteca, pero que aún no han sido implementadas:

- Permitir que se puedan sincronizar tantas tarjetas como se desee (ahora está limitado a dos: un maestro y un esclavo).
- Permitir que se pueda sincronizar un número de tareas indeterminado (ahora está limitado a una).
- Hacer que el esclavo aprenda el patrón temporal del maestro. Si el maestro está ejecutando tareas pesadas o críticas, no siempre va a disponer de tiempo de reloj para enviar las notificaciones. En ese caso, no hay redundancia que valga, el esclavo no ejecutará la tarea y se perderá la sincronización. Si se estudia el funcionamiento de la biblioteca, es fácil deducir que el maestro manda una notificación cada cierto tiempo, manteniendo un patrón. Se puede hacer que tras varias iteraciones, el esclavo aprenda cada cuanto tiempo recibe el aviso. De esta forma, cuando el maestro esté ocupado, puede mandar una señal que le indique al esclavo que actue por su cuenta. En ese modo, el esclavo ejecutará la tarea esperando entre iteración e iteración el tiempo que ha aprendido, sin esperar a que llegue la notificación.

## Referencias

- [Comunicación de Arduino con puerto serie](https://www.luisllamas.es/arduino-puerto-serie/)
- [El bus SPI en Arduino](https://www.luisllamas.es/arduino-spi/)
- [El bus I2C en Arduino](https://www.luisllamas.es/arduino-i2c/)
- [Event-Triggered versus Time-Triggered Systems](https://www.e-reading-lib.com/chapter-amp.php/143358/161/andrew-tanenbaum-distributed-operating-systems.html)
- [Barrier (computer science)](https://en.wikipedia.org/wiki/Barrier_(computer_science))