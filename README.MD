# Simulación de Partículas en OpenGL

Esta es un proyecto de muestra que implementa un sistema de partículas básico renderizado con OpenGL. Las partículas son creadas y simuladas en la CPU, ofreciendo una posible mayor versatilidad y facilidad de la implementación a cambio de rendimiento.

La aplicación es **multiplataforma**, escrita en **C++ 23** y probada en Windows y Linux (Wayland y X11). Utiliza **GLFW** para la gestión de ventanas y entrada.

## Características

- **Simulación de partículas**:
  - Renderización de partículas utilizando OpenGL 4.2 Core.
  - Actualización y creación de partículas gestionada íntegramente en la CPU.

- **Interacción en tiempo real**:
  - Es posible pausar la simulación en cualquier instante, permitiendo observar el estado actual de las partículas.

- **Diseño multiplataforma**:
  - Utiliza CMake y GLFW, permitiendo así una gran compatibilidad entre diversas plataformas y entornos de escritorio.

- **Piscina de partículas**:
  - Utiliza una piscina de partículas para simplificar la creación y actualización de esta. Al ser reiniciadas y no destruidas, usa un vector en vez de una lista libre ([_free list_](https://en.wikipedia.org/wiki/Free_list)), mucho más eficaz a la hora de crear y destruir objetos.

## Controles

- **Movimiento de la cámara**:
    - `A`: Mueve la cámara hacia la izquierda.
    - `D`: Mueve la cámara hacia la derecha.
    - `W`: Acerca la cámara.
    - `S`: Aleja la cámara.
    - `E`: Sube la cámara.
    - `Q`: Baja la cámara.

- **Control de simulación**:
    - Presionar y mantener `Espacio` pausa temporalmente la simulación.

## Objetivo del Proyecto

El objetivo de esta aplicación es mostrar una implementación básica de un sistema de una cantidad arbitraria de partículas usando una piscina. Los conceptos expuestos se pueden extrapolar fácilmente a la GPU con sombreadores de cómputo (disponibles a partir de OpenGL 4.3), siendo la simulación en la GPU mucho más eficaz al evitar la cara sincronización y copia de memoria entre CPU y GPU.

El manejo de errores y excepciones es muy básico, ya que se trata únicamente de una prueba de concepto y no de una aplicación de calidad de producción. La salida de depuración de OpenGL está activada si se compila con la depuración activada.
