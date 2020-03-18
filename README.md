[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# SyncroARD
###### Proyecto desarrollado para la asignatura Sistemas Críticos del Máster en Ingeniería Informática (UGR).

SyncroARD constituye una biblioteca de Arduino escrita en C para sincronizar cualquier tipo de tarea en un sistema distribuido de tarjetas. Proporciona una API "de alto nivel" para permitir que una tarea definida se ejecute exactamente al mismo tiempo en los dispositivos que se requiera, a la vez que garantiza la confiabilidad (*reliability*) del sistema.

SyncroARD está construida sobre [**FreeRTOS**](https://www.freertos.org/), más concretamente, utiliza por debajo la biblioteca [Arduino_FreeRTOS_Library](https://github.com/feilipu/Arduino_FreeRTOS_Library). Para más información sobre cómo se tomó esta decisión, consulte el [siguiente enlace](https://alvarillo89.github.io/SyncroARD/docs/comparativa).

---

## Licencia

Este proyecto está licenciado bajo los términos de la licencia GNU General Public License v3.0.