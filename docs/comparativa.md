# Comparativa de microkernels y dispositivos

En este documento encontrará información acerca de por qué se decidió utilizar FreeRTOS y por qué se escogió Arduino como plataforma objetivo para el desarrollo de este proyecto.

## 1. Microkernels

Para que la biblioteca funcione apropiadamente, debe tener por debajo una capa que proporcione utilidades de tiempo real. Las tareas deben ejecutarse en el momento que determine el protocolo de sincronización, ni antes ni después. Están sometidas a restricciones de tiempo. 

Puesto que los dispositivos con los que se trabajará serán empotrados y no realizarán más de una o dos tareas, tenemos suficiente con un microkernel.

> Un microkernel, es un tipo de kernel de un sistema operativo que provee un conjunto de primitivas o llamadas mínimas al sistema para implementar servicios básicos como espacios de direcciones, comunicación entre procesos y planificación básica.

De entre todos los microkernels existentes para tiempo real, se han tomado en consideración los siguientes:

- `FreeRTOS` (https://www.freertos.org/)
- `µC/OS` (https://www.micrium.com/rtos/kernels/)
- `RTEMS` (https://www.rtems.org/)
- `F9-kernel` (https://github.com/f9micro/f9-kernel)

---

### 1.1 FreeRTOS

Una de las alternativas más conocidas y utilizadas dentro del ámbito de los RTOS (Real Time Operative System). Incluye un kernel y un amplio conjunto de bibliotecas listas para emplearse en múltiples sectores industriales.

Está específicamente diseñado para funcionar con pequeños microprocesadores y microcontroladores (según ellos mismos, constituyen el estándar de facto para este tipo de dispositivos). Entre sus principales características destacan:

- Open Source (MIT License). También proporciona una versión comercial.
- Ligero (con un footprint de tan solo 9KB).
- Modo de ahorro de energía para ciertas arquitecturas.
- Soporte para más de 40 arquitecturas.
- Bibliotecas modulares.
- Incluye conectividad en la nube e integraciones de referencia para IoT.

---

### 1.2 µC/OS

Dentro de esta familia se incluyen dos kernels diferentes: µC/OS-II y µC/OS-III (una versión más reciente del anterior). Ambos son preventivos, portables y altamente escalables. Constituyen la base para el sistema operativo de tiempo real con el mismo nombre: µC/OS.

Sus características principales son:

- Portable: se proporciona el código fuente y una documentación a fondo para múltiples arquitecturas de procesadores.
- Escalable: Tareas ilimitadas y objetos del kernel ilimitados.
- El footprint suele estar entre los 6 y los 25KB (incluyendo código y datos).
- Confiable (*reliable*): proporcionan un amplio rango de verificaciones, que incluye verificaciones de los punteros pasados en las llamadas a la API, servicios a nivel de tarea de ISR, argumentos dentro de un rango permitido y opciones específicas válidas.
- Eficiente: proporciona estadísticas útiles para detectar cuellos de botella y optimizar el consumo de energía.
- Se trata de un kernel multitarea, preventivo y de tiempo real, que además proporciona un scheduling opcional de Round Robin.
- Certificable para *safety-critical applications*.

---

### 1.3 RTEMS

RTEMS significa *Real-Time Executive for Multiprocessor Systems* (ejecutivo de tiempo real para sistemas multiprocesador). Se trata de un sistema operativo de tiempo real que soporta APIs estándares y abiertas, como por ejemplo, POSIX. Ha sido utilizado en vuelos espaciales y en aplicaciones médicas entre otros.

- Open Source (MIT License). También proporciona una versión comercial.
- Soporte para varias arquitecturas de procesadores y dispositivos empotrados.
- Amplia documentación.
- Kernel con capacidad multitarea y asignación de memoria dinámica.
- Compatible con la familia de procolos de internet como UDP, TCP, FTP, etc.
- Compatible con protocolos de comunicación USB, tarjetas SD/MMC, entre otros.

---

### 1.4 F9-kernel

Se trata de un microkernel experimental utilizado para construir sistemas empotrados flexibles e inspirado en la [familia de microkernels L4](https://en.wikipedia.org/wiki/L4_microkernel_family). Su objetivo es desplegar mediante técnicas modernas, un kernel para soportar la ejecución de aplicaciones de tiempo real y de tiempo compartido (por ejemplo, comunicaciones inalámbricas), con eficiencia y seguridad.

- Diseñado para los procesadores de la serie [ARM Cortex-M](https://en.wikipedia.org/wiki/ARM_Cortex-M).
- Sigue los principios fundamentales de los microkernels, incluyendo espacio de direcciones, gestión de hebras e IPC (inter-proccess communicacion) en el kernel privilegiado.
- Planificación eficiente de energía y temporizador sin tick, que permite al procesador levantarse solo cuando es necesario.
- Instrumentación dinámica inspirada en el kernel de Linux.
- Proporciona llamadas al sistema para la gestión de direcciones de memoria.
- IPC síncrono.
- Proporciona mecanismos de depuración.

--- 

### 1.5 Tabla comparativa

|                           |         FreeRTOS         |         µC/OS        |    RTEMS    |  F9-kernel  |
|:-------------------------:|:------------------------:|:--------------------:|:-----------:|:-----------:|
|        Open Source        |            Sí            |          No          |      Sí     |      Sí     |
|           Gratis          |            Sí            | Free Trial (45 días) |      Sí     |      Sí     |
|      Multiplataforma      |            Sí            |          Sí          |      Sí     |      No     |
|     Ahorro de energía     |  En algunas plataformas  |          Sí          |      No     |      Sí     |
|         Footprint         |            9KB           |        6~25KB        |   64~128K   | Desconocida |
|       Documentación       | Abundante y estructurada |    En profundidad    |  Abundante  |    Mínima   |
| Existencia de bibliotecas |            Sí            |    Otros productos   |      Sí     |      No     |
|    Última actualización   |        Marzo, 2020       |      Desconocido     | Marzo, 2020 |  Enero 2020 |
|         Confiable         |            Sí            |   Sí (certificable)  |      Sí     | No evaluado |

---

### 1.6 Conclusión

Evaluando las diferentes alternativas, parece ser que la que más destaca por sus características es `µC/OS`. No obstante, no se trata de una comparativa justa. Este es el unico kernel "comercial" que se muestra en la tabla y, posiblemente, si se tuviésen en cuenta las licencias comerciales de los otros kernels, no habría tanta diferencia. Además, dada la naturaleza de código abierto de `SyncroARD`, usar un software privativo no es aceptable. 

Dentro de los kernels open source, podemos descartar `F9-kernel`. Este está aún en un desarrollo y las releases existentes son experimentales. Además, todavía no se ha evaluado su confiabilidad (*reliability*).

Esto nos deja con dos opciones: `FreeRTOS` o `RTEMS`, los dos kernels de tiempo real de código abierto por excelencia. Ambos proporcionan más o menos las mismas características, son compatibles con múltiples arquitecturas y están orientados a dispositivos empotrados. Para decidir, se tendrán en cuenta únicamente la popularidad del mismo y el estado de la documentación.

Según estos dos criterios, el claro ganador es `FreeRTOS`, que será el microkernel que se empleará para el desarrollo de `SyncroARD`.

---

## 2. Dispositivos

`FreeRTOS` y `SyncroARD` deben instalarse sobre alguna tarjeta (además de que `FreeRTOS` es muy dependiente del dispositivo). Durante el desarrollo, debemos decantarnos por utilizar un dispositivo concreto. Se han tomado en cuenta las siguientes alternativas:

- Raspberry Pi (https://www.raspberrypi.org/)
- Zybo Zynq (https://store.digilentinc.com/zybo-zynq-7000-arm-fpga-soc-trainer-board/)
- Arduino (https://www.arduino.cc/)

---

### 2.1 Raspberry Pi

Posiblemente, el ordenador de placa reducida y bajo costo más popular del mundo de la electrónica y la informática. Su objetivo inicial fue fomentar la enseñanza de la informática en las escuelas, aunque se ha aplicado en muchos más campos fuera de se su mercado objetivo, como por ejemplo la robótica. Su primer lanzamiento fue el 29 de Febrero de 2012 y el último modelo, la Raspberry Pi 4 B, fue anunciado en junio de 2019.

 Incluye un procesador Broadcom, memoria RAM, GPU, puertos USB, HDMI, Ethernet, 40 pines GPIO y un conector para cámara. Ninguna de sus ediciones incluye memoria, siendo esta en su primera versión una tarjeta SD y en ediciones posteriores una tarjeta MicroSD.

 En [este enlace](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/specifications/) se pueden consultar las especificaciones del último modelo.

 **Pros:**
 - Barata.
 - Fácilmente configurable.
 - Amplia documentación.
 - Miles de tutoriales y referencias en la web.
 - Utilizada para sistemas empotrados.

 **Contras:**
 - No dispone de un reloj de tiempo real.
 - No es aconsejable para sistemas de tiempo real.
 - FreeRTOS no dispone de un port oficial. Aunque sí de un [port hecho por terceros](https://github.com/jameswalmsley/RaspberryPi-FreeRTOS)  muy básico.

--- 

### 2.2 Zybo Zynq

Se trata de una plataforma de desarrollo de circuitos digitales y software empotrado de nivel básico, rica en funciones y lista para usar, construida alrededor del miembro más pequeño de la familia Xilinx Zynq-7000, el Z-7010. Este procesador se basa en la arquitectura Xilinx All Programmable System-on-Chip (AP SoC), que integra estrechamente un procesador ARM Cortex-A9 de doble núcleo con la *Field Programmable Gate Array* (FPGA) Xilinx serie 7.

Incluye memorias integradas, E/S de video y audio, USB de doble función, Ethernet y una ranura SD. Además, incorpora seis conectores Pmod.

En el [siguiente enlace](https://store.digilentinc.com/zybo-zynq-7000-arm-fpga-soc-trainer-board/) se incluyen las características.

**Pros:**
- Específica para sistemas empotrados.
- Incluye un [port oficial](https://www.freertos.org/a00090.html#XILINX) de FreeRTOS.
- Es útil para sistemas de tiempo real.

**Contras:**
- Más cara.
- El modelo está descontinuado.
- Documentación prácticamente nula.
- Es imposible encontrar comunidad que haya trabajado con ella.
- Cargar una imagen en la tarjeta es bastante complejo.

---

### 2.3 Arduino

Arduino es una compañía de desarrollo de software y hardware libres, así como una comunidad internacional que diseña y manufactura placas de desarrollo de hardware para construir dispositivos digitales y dispositivos interactivos que puedan detectar y controlar objetos del mundo real. Arduino se enfoca en acercar y facilitar el uso de la electrónica y programación de sistemas embebidos en proyectos multidisciplinarios. Los diseños de las placas Arduino usan diversos microcontroladores y microprocesadores. Generalmente el hardware consiste de un microcontrolador Atmel AVR, conectado bajo la configuración de "sistema mínimo" sobre una placa de circuito impreso a la que se le pueden conectar placas de expansión (shields) a través de la disposición de los puertos de entrada y salida presentes en la placa seleccionada.

Arduino presenta una gran variedad de modelos de placas. Para esta comparativa, nos centraremos en el modelo [Arduino Uno R3](https://www.infootec.net/arduino/). Incluye un procesador `ATmega328P` con 35KB de memoria flash y 14 pines digitales de entrada y salida.

Además, existen placas con las mismas especificaciones pero de otras marcas, haciéndolas mucho más económicas de lo que ya es de por sí Arduino (como por ejemplo, las placas de `ELEGOO`).

**Pros:**
- Muy ecónomica.
- Amplia comunidad y soporte.
- Miles de tutoriales y referencias en la web.
- Es sencillo comunicar dos tarjetas entre sí.
- El proceso de cargar una imagen en la tarjeta es extremadamente sencillo.

**Contras:**
- No existe un port oficial de FreeRTOS para Arduino. No obstante, sí que existe una [bibloteca de Arduino de FreeRTOS](https://github.com/feilipu/Arduino_FreeRTOS_Library) hecha por terceros.
- Escasa memoria.

---

### 2.4 Conclusión

Del estudio anterior, pueden extraerse los siguientes puntos:

- La `Raspberry Pi` no es apropiada para sistemas de tiempo real. La versión de FreeRTOS no es oficial y además no incluye todas las características del microkernel. No parece la mejor decisión para un proyecto de este tipo. 
- La `Zybo Zynq` resulta demasiado compleja de utilizar, en gran parte, debido a la descontinuación del modelo por parte de los fabricantes y a la falta de referencias y comunidad tras ella. Es cierto que es la única de esta lista con un port oficial de FreeRTOS, pero su uso ralentizará enormemente el desarrollo.
- Las ventajas de `Arduino` con respecto al resto son bastante claras:
    - Tiene tanta comunidad como la Raspberry Pi.
    - A diferencia de la Raspberry Pi, Arduino si que incorpora un reloj que puede ser utilizado para el `system_tick` de FreeRTOS (un watchdog timer).
    - Aunque el port de Arduino tampoco es oficial, este si que incorpora todas las funcionalidades originales de FreeRTOS.
    - Programar el Arudino es infinitamente más sencillo que programar la Zybo.
    - Con respecto al inconveniente de la memoria, se han realizado pruebas para comprobar cuánto espacio consume FreeRTOS de los 32KB disponibles. El microkernel solo consume un 20% del espacio, quedando libre un 80% para el resto de código necesario.
    - Utilizando el bus [I2C](https://www.luisllamas.es/arduino-i2c/) resulta sencillo conectar dos tarjetas Arduino y compartir información entre ambas.

En definitiva, para el desarrollo de este proyecto, se utilizará como dispositivo la placa `Arduino Uno R3`. En el directorio [**prior_tests/**](https://github.com/alvarillo89/SyncroARD/tree/master/prior_tests) puede consultar todos los sketches de Arduino empleados para verificar la viabilidad de esta decisión. 

---
## 3. Referencias

- https://www.freertos.org/RTOS.html
- https://es.wikipedia.org/wiki/RTEMS
- https://lists.rtems.org/pipermail/users/2004-September/010702.html
- http://www.microkernel.info/
- https://www.silabs.com/about-us/legal/micrium-software-evaluation-license
- https://www.raspberrypi.org/forums/viewtopic.php?t=199307
- https://www.socallinuxexpo.org/sites/default/files/presentations/Steven_Doran_SCALE_13x.pdf
- https://www.arduino.cc/en/Tutorial/MasterWriter
- https://github.com/feilipu/Arduino_FreeRTOS_Library
- https://feilipu.me/2015/11/24/arduino_freertos/