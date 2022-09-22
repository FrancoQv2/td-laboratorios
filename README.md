# Plantilla de proyectos CIAA

Este repositorio contiene los archivos necesarios para compilar proyectos en lenguaje C que se ejecuten en una placa CIAA. Es una versión simplifciada del firmware original, pensada para se utilizada en ambientes educativos.

## IMPORTANTE

**Este entorno esta en construccion!!!**

## Librerias disponibles

- LPCOpen v3.02.
- FreeRTOS Kernel V10.2.0.
- LPCUSBlib.

## Placas soportadas

- EDU-CIAA-NXP (LPC4337).

## Perifericos soportados

- Placa EDU-CIAA-NXP
	- Leds integrados LED_RDB, LED1, LED2 LED3
	- Teclas integradas TEC1, TEC2, TEC3, TEC4
- Poncho Educativo UNT
	- Indicadores de siete segmentos multiplexados
	- Teclas integradas F1, F2, F3, F4, F5, SI, NO
	- Led RGB con pontencia controlada por PWM en los tres canales
	- Pantalla de LCD 2x20 caracteres
	
## Compiladores soportados

- gcc-arm-none-eabi

## Compilar y grabar un proyecto

Asegurese de tener configurado el compilador `arm-none-eabi-*` en el `PATH`.

- Defina las variables `PROJECT_NAME`  y `PROJECT_PATH` en el archivo `project.mk` de acuerto con el proyecto que quiera compilar. La variable `PROJECT_PATH` es relativa a carpeta principal del repositorio (la carpeta donde se encuentra este archivo).
- Eiminar el resultado de una compilacion previa `make clean`.
- Compilar el proyecto con `make`.

Asegurese de tener configurado `openocd` en el `PATH`.
 
- Grabar el proyecto en la placa con `make download`.

## Crear un nuevo proyecto

Un proyecto esta formado por una carpeta (el nombre no puede contener espacios) que incluye un archivo `config.mk` y dos subcarpetas: una llamada `src` y la otra llamada `inc`. En la carpeta `src` se ubican los archivos cn el codigo fuente (`.c`, `.cpp` o `.s`) y en la carpeta `inc` se ubican los archivos de cabecera (`.h` o `.hpp`). En el archivo `config.mk` se definen variables para modificar el proceso de compilación, por ejemplo indicar que librerias se utilizan en el proyecto, agregar definiciones de macros, etc.

## Ejemplos

Se incluyen los siguientes ejemplos:
 
- `blinking_freertos`: Ejemplo para el poncho educativo de la UNT que utiliza una tarea de FreeRTOS para hacer parpadear el segmento central del utlimo indicador de siete segmentos.

## Agradecimientos

Esta plantilla esta la estrcutura y muchos de los archivos de la plantilla desarrollada por Eric Pernia y Martin Ribelota para [CESE FI-UBA](http://laboratorios.fi.uba.ar/lse/cursos.html) la *Especialización en Sistemas Embebidos* de la *Universidad de Buenos Aires*. El repositorio original puede descargarse de este [enlace](https://github.com/epernia/cese-edu-ciaa-template/).

Este trabajo se apoya en el [Proyecto-CIAA](http://www.proyecto-ciaa.com.ar), un desarrollo nacional impulsador por instrituciones publicas y privadas que busca impulsar la industria argentina.
	

