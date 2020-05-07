# Desarrollo de la biblioteca

En este documento se describe con detalle los apectos de implementación de `SyncroARD`: el proceso seguido, los problemas encontrados, así como los planes de futuro que se tienen sobre la biblioteca.

**Tabla de contenidos:**

- [1 Mecanismos de comunicación](#1-mecanismos-de-comunicacion)
- [1 Time Triggering VS Event Triggering](#2-time-triggering-vs-event-triggering)

## 1 Mecanismos de comunicación

En un sistema distribuido en el que se produzca interacción entre los dispositivos, debe existir un mecanismo que permita mandar información entre ellos, es decir, un mecanismo (o varios) de comunicación.

## 2 Time Triggering VS Event Triggering

El principal objetivo de la sincronización es lograr que una determinada tarea o procedimiento se ejecute al mismo tiempo en un conjunto de dispositivos. En consecuencia, debe de existir un mecanismo que dispare (_triggering_) la ejecución de la tarea en cada una de las tarjetas. En sistemas de tiempo real, este _triggering_ puede ser de dos tipos: **temporal** o **por eventos**:

- Disparo temporal (_time triggering_): en este tipo de sistemas, se produce una interrupción del reloj cada _ΔT_ milisegundos. Durante esas interrupciones, se muestrean los sensores y se accionan ciertos actuadores. El _ΔT_ debe ser elegido con extremo cuidado. Si es demasiado pequeño, el sistema experimentará muchas interrupciones de reloj y perderá demasiado tiempo manejándolas. Si es demasiado grande, los eventos pueden no ser detectados. Además, la decisión sobre qué sensores comprobar en cada tick del reloj es crítica.
- Disparo por eventos (_event triggering_): en este tipo de sistemas, las interrupciones se producen a causa de un evento externo, algo detectado por un sensor. Para sistemas de tiempo real suaves con mucha potencia de computación, este enfoque es simple, funciona bien y sigue siendo ampliamente utilizado. Su principal inconveniente es que pueden fallar en condiciones de mucha carga, es decir, cuando se producen muchos eventos simultáneos.

En definitiva, los sistemas activados por eventos dan una respuesta más rápida cuando la carga de trabajo es baja, pero si la carga es alta, tiene más _overhead_ y posibilidades de fallo. Los sistemas activados por tiempo tienen las propiedades opuestas, y sólo son adecuados en entornos relativamente estáticos en los que se sabe mucho sobre el comportamiento del sistema de antemano. Cuál es mejor depende de la aplicación.

Para `SyncroARD`, tras varias pruebas, se utilizó finalmente _event triggering_ para la implementación : el maestro manda a través de los mecanismos de comunicación una señal que genera una interrupción encargada de ejecutar la tarea en el esclavo . Pese a que esta es la decisión final, inicialmente se optó por un enfoque híbrido, aunque los problemas encontrados lo hicieron inviable. Todo esto se describe en el siguiente apartado.

## 3 Mecanismos de sincronización

### 3.1 Enfoque 1 (Sincronización de los relojes de ambas tarjetas)

### 3.2 Enfoque 2 (EventGroup de FreeRTOS)

### 3.3 Enfoque 3 (Comienzo simultáneo y sincronización periódica)

### 3.4 Enfoque Final (Sincronización basada en Eventos)

## 4 Redundancia en la comunicación

## 5 Caracterización del tiempo de comunicación

## 6 Trabajo futuro

## Referencias

- [Event-Triggered versus Time-Triggered Systems](https://www.e-reading-lib.com/chapter-amp.php/143358/161/andrew-tanenbaum-distributed-operating-systems.html)
