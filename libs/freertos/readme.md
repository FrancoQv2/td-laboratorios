# Notas de la implementación
----------------------------

Para la implementación de FreeRTOS V10.2.0 se copío el codigo fuente en la carpeta `source` y se movió la carpeta `includes` sin cambios respecto al archivo comprimido con la distribución oficial descargada del sitio [https://www.freertos.org/a00104.html]()

En la carpeta `portable` se eliminaron los compiladores no utilizados dejando únicamente la carpeta `GCC` y dentro de la misma se eliminaron tambien las portaciones no utilizados dejando únicamente las carpetas `ARM_CM3` y `ARM_CM4F`, todo esto sin realizar ninguna modificación en el contenido de las mismas.

En la carpeta `portable/MemMang` se modificó el archivo `heap_4.c` agregando un atributo a la definición devector ucHeap para ubicar al mismo en el segundo banco de memoria RAM. De esta forma el codigo original 
`static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];` se cambio por `__attribute__ ((section(".data.$RAM2"))) static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];`. 

**Las otras implementación del gestor de memoria dinámica no se modificaron ni se probaron**.

06/03/2019, Esteban Volentini <evolentini@gmail.com>
