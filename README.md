# Casa Inteligente - CPS con FreeRTOS

Este proyecto implementa un sistema ciberfísico (CPS) que simula el funcionamiento de una casa inteligente utilizando un ESP32 con FreeRTOS. El sistema integra sensores, actuadores, control por IR, una interfaz LCD, persistencia de configuración y tareas concurrentes.

La versión actual del proyecto soporta ejecución local mediante PlatformIO y también simulación completa dentro de Visual Studio Code utilizando el plugin oficial de Wokwi.

---

## Uso desde Wokwi (VS Code)

El proyecto puede ejecutarse completamente desde el entorno de Wokwi sin necesidad de hardware físico.

Requisitos:

1. Tener instalado el **Wokwi for VS Code**.  
2. Activar la licencia gratuita dentro del plugin (un clic, sin tarjetas).  
3. Ejecutar la simulación directamente (RUN) o compilar con `pio run`.

Para mayor detalle sobre la generación del firmware y el flujo completo, consultar el archivo:

```
firmware/README.md
```

Versión online del proyecto (vista en el navegador):  
https://wokwi.com/projects/448016145560939521

---

## Objetivo del Sistema

El sistema automatiza las siguientes funciones del hogar:

| Subsistema    | Función                                          | Sensores                  | Actuadores             |
| ------------- | ------------------------------------------------ | ------------------------- | ---------------------- |
| Climatización | Mantener temperatura dentro de un rango definido | 3 sensores de temperatura | Termostato             |
| Iluminación   | Encender luces según nivel lumínico              | 3 sensores de luz         | Lámparas               |
| Seguridad     | Detectar eventos y activar alarma                 | 3 sensores de movimiento  | Sirena y LED indicador |

---

## Funcionamiento General

Cada funcionalidad del sistema está implementada como una tarea independiente de FreeRTOS:

| Tarea                  | Responsabilidad                                                                 |
| ---------------------- | ------------------------------------------------------------------------------- |
| Lectura de sensores    | Lectura de temperatura, luz y movimiento                                        |
| Control del termostato | Ajuste automático según umbrales configurables                                  |
| Control de luces       | Encendido automático basado en intensidad lumínica                              |
| Control de alarma      | Activación de sirena y LED ante movimiento en modo armado                       |
| Interfaz LCD           | Visualización de estado del sistema y avisos                                    |
| Control remoto IR      | Manejo de cambios de parámetros y acciones del usuario                          |

---

## Configuración mediante LCD + Control Remoto

El usuario puede modificar parámetros del sistema en tiempo real.

Ingreso al menú:
- Presionar **MENU** en el control remoto.

Controles principales:
- **Arriba/Abajo**: Cambiar valores.
- **MENU**: Salir del menú sin guardar.
- **POWER (fuera del menú)**: Reiniciar el sistema y recargar configuración persistida.

Parámetros configurables:
- Temperatura mínima
- Temperatura máxima
- Umbral de luz

Tras confirmar valores, la configuración se guarda en EEPROM.

---

## Persistencia (EEPROM)

El sistema almacena y carga automáticamente:
- `tempMin`
- `tempMax`
- `umbralLuz`

Si la memoria contiene datos corruptos, se restauran valores por defecto.

---

## Subsistema de Seguridad

- La alarma se activa ante detección de movimiento si el sistema está armado.
- Se utilizan una sirena y un LED indicador.
- La desactivación se hace mediante el botón asignado en el control remoto.
- Los avisos visuales se gestionan exclusivamente desde la tarea del LCD.

---

## Pruebas Unitarias

El proyecto incluye un entorno de pruebas con mocks que simulan el comportamiento del entorno Arduino.


Ejecución:

```
./build_test.sh
```

Mocks disponibles en:

```
test/mocks/
```

---
## Integración Continua (CI)

El repositorio incluye un workflow de GitHub Actions que:

* Compila el proyecto con PlatformIO
* Ejecuta las pruebas unitarias
* Genera los archivos `firmware.bin` y `firmware.elf`
* Publica estos archivos como artifacts descargables

Al publicar un tag, la CI genera una versión con su propio firmware compilado.

Configuración disponible en:

```
.github/workflows/
```
---

## Funcionalidades Extra Implementadas

| Funcionalidad adicional        | Estado        |
| ------------------------------ | ------------- |
| Persistencia en EEPROM         | Implementada  |
| Control remoto por IR          | Implementada  |
| Interfaz LCD                   | Implementada  |
| Menú configurable              | Implementada  |
| Pruebas unitarias con mocks    | Implementadas |
| Integración continua           | Implementada  |
| Simulación con Wokwi (VS Code) | Implementada  |