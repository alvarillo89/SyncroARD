# Comparativa de microkernels y dispositivos

## 1. Microkernels

Para que la biblioteca funcione apropiadamente, debe tener por debajo una capa que proporcione utilidades de tiempo real. Puesto que los dispositivos con los que se trabajará serán empotrados y no realizarán más de una o dos tareas, tenemos suficiente con un microkernel.

> Un microkernel, es un tipo de kernel de un sistema operativo que provee un conjunto de primitivas o llamadas mínimas al sistema para implementar servicios básicos como espacios de direcciones, comunicación entre procesos y planificación básica.

De entre todos los microkernels existenes para tiempo real, se han tomado en consideración los siguientes:

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
- Biblotecas modulares.
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

Evaluando las diferentes alternativas, parece ser que la que más destaca por sus características es `µC/OS`. No obstante, no se trata de una comparativa justa. Este es el unico kernel "comercial" que se muestra en la tabla y, posiblemente, si se tuviésen en cuenta las licencias comerciales de los otros kernels, no habría tanta diferencia. Además, dada la naturaleza de código abierto de `SyncroC`, usar un software privativo no es aceptable. 

Dentro de los kernels open source, podemos descartar `F9-kernel`. Este está aún en un desarrollo y las releases existentes son experimentales. Además, todavía no se ha evaluado su confiabilidad (*reliability*).

Esto nos deja con dos opciones: `FreeRTOS` o `RTEMS`, los dos kernels de tiempo real de código abierto por excelencia. Ambos proporcionan más o menos las mismas características, son compatibles con múltiples arquitecturas y están orientados a dispositivos empotrados. Para decidir, se tendrán en cuenta únicamente la popularidad del mismo y el estado de la documentación.

Según estos dos criterios, el claro ganador es `FreeRTOS`, que será el microkernel que se empleará para el desarrollo de `SyncroC`.

---

## 2. Dispositivos

`FreeRTOS` y `SyncroC` deben instalarse sobre alguna tarjeta. Para las pruebas realizadas durante el desarrollo, debemos descantarnos por utilizar un dispositivo concreto. Se han tomado en cuenta las siguientes alternativas:

- Raspberry Pi (https://www.raspberrypi.org/)
- Zybo Zynq (https://store.digilentinc.com/zybo-zynq-7000-arm-fpga-soc-trainer-board/)


---
## 3. Referencias

- https://www.freertos.org/RTOS.html
- https://es.wikipedia.org/wiki/RTEMS
- https://lists.rtems.org/pipermail/users/2004-September/010702.html
- http://www.microkernel.info/
- https://www.silabs.com/about-us/legal/micrium-software-evaluation-license