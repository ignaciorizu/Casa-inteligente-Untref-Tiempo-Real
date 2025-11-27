# Casa Inteligente - CPS con FreeRTOS

Este proyecto implementa un sistema ciberfísico (CPS) que simula el funcionamiento de una casa inteligente utilizando un ESP32 con FreeRTOS. El sistema integra sensores físicos, actuadores y una interfaz de usuario para configurar parámetros de funcionamiento.

El proyecto incluye control de temperatura, regulación de iluminación, monitoreo de seguridad con alarma, persistencia de configuración y pruebas automatizadas.

---

## Objetivo del Sistema

El sistema busca automatizar las siguientes funciones del hogar:

| Subsistema    | Función                                          | Sensores                  | Actuadores             |
| ------------- | ------------------------------------------------ | ------------------------- | ---------------------- |
| Climatización | Mantener temperatura entre un rango configurable | 3 sensores de temperatura | Termostato             |
| Iluminación   | Activar luz según el nivel lumínico del ambiente | 3 sensores de luz         | Lámparas               |
| Seguridad     | Detectar intrusos y activar alarma               | 3 sensores de movimiento  | Sirena y LED indicador |

---

## Funcionamiento General

El sistema utiliza múltiples tareas de FreeRTOS, cada una con una responsabilidad específica:

| Tarea                  | Responsabilidad                                                                 |
| ---------------------- | ------------------------------------------------------------------------------- |
| Lectura de sensores    | Obtención de valores de temperatura, luz y movimiento                           |
| Control del termostato | Ajuste de calefacción según el promedio de temperatura y umbrales configurables |
| Control de luces       | Activación de iluminación si se detecta poca luz                                |
| Control de alarma      | Gestión de sirena y señal luminosa ante detección de intrusión                  |
| Interfaz LCD           | Visualización del estado del sistema, alertas y confirmaciones                  |
| Control remoto IR      | Interpretación de comandos y configuración de parámetros                        |

---

## Configuración mediante LCD + Control Remoto

El usuario puede modificar parámetros del sistema en tiempo real utilizando un control remoto infrarrojo.

### Ingreso al modo de configuración

* Presionar el botón **MENU** del control.
* La pantalla LCD mostrará el menú de configuración.

### Navegación dentro del menú

| Botón del Control        | Acción                                                             |
| ------------------------ | ------------------------------------------------------------------ |
| **Arriba / Abajo**       | Cambia el valor de la configuración actual                         |
| **MENU nuevamente**      | Sale del menú sin guardar cambios                                  |
| **POWER fuera del menú** | Reinicia el sistema (útil para cargar valores guardados en EEPROM) |

### Parámetros configurables

| Parámetro          | Descripción                                   |
| ------------------ | --------------------------------------------- |
| Temperatura mínima | Umbral para activar calefacción               |
| Temperatura máxima | Temperatura límite antes de apagarla          |
| Umbral de luz      | Nivel mínimo de lux para encender iluminación |

Al finalizar la configuración, los datos son guardados automáticamente y la pantalla muestra un mensaje de confirmación.

---

## Persistencia de Valores (EEPROM)

El sistema guarda en memoria no volátil los siguientes valores:

* `tempMin`
* `tempMax`
* `umbralLuz`

Estos se cargan automáticamente al iniciar el sistema.
Si la EEPROM contiene valores inválidos, el software restaura valores por defecto.

---

## Subsistema de Seguridad

* La alarma se activa si se detecta movimiento mientras el sistema está en modo armado.
* La sirena y un LED se encienden hasta que el usuario desactive la alarma.
* La alarma se desactiva presionando el botón asignado en el control remoto.

Para evitar conflictos de actualización con la pantalla, los mensajes de aviso son manejados únicamente por la tarea del LCD.

---

## Pruebas Unitarias

El proyecto incluye pruebas unitarias con mocks para simular el entorno Arduino.

### Dependencias necesarias

Instalación recomendada (Debian/Ubuntu):

```bash
sudo apt install arduino-cli
sudo apt install libgtest-dev
```

Ejecución de pruebas (según el script incluido):

```bash
./build_test.sh
```

Los mocks se encuentran en:

```
test/mocks/
```

---

## Integración Continua

El proyecto incluye integración continua con GitHub Actions. La CI ejecuta:

* Compilación
* Pruebas unitarias
* Validación previa a merges

Configuración disponible en:

```
.github/workflows/cpp-tests.yml
```

---

## Funcionalidades Extra Implementadas

| Funcionalidad adicional             | Estado        |
| ----------------------------------- | ------------- |
| Persistencia en EEPROM              | Implementada  |
| Control remoto infrarrojo           | Implementado  |
| Interfaz gráfica mediante LCD       | Implementada  |
| Modificación dinámica de parámetros | Implementada  |
| Pruebas unitarias con mocks         | Implementadas |
| Integración continua                | Implementada  |
