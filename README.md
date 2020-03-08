# SyncroC
###### Proyecto desarrollado para la asignatura Sistemas Críticos del Máster en Ingeniería Informática (UGR).

SyncroC consituye una biblioteca *header-only* escrita en C para sincronizar cualquier tipo de tarea en un sistema distribuido de dispositivos empotrados. Proporciona una API "de alto nivel" para permitir que una tarea definida se ejecute exactamente al mismo tiempo en los dispositivos que se requiera, a la vez que garantiza la confiabilidad (*reliability*) del sistema.

SyncroC está construida sobre [`FreeRTOS`](https://www.freertos.org/). Para más información sobre cómo se tomo esta decisión, consulte el [siguiente enlace](https://github.com/alvarillo89/SyncroC/blob/master/docs/comparativa.md).