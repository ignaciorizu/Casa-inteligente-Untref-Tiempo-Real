# Firmware – Smart Home CPS

Esta carpeta contiene los archivos de firmware generados por PlatformIO:

- firmware.bin  
- firmware.elf

Estos archivos se generan automáticamente tanto de forma local como mediante la integración continua (CI) en GitHub Actions.

---

## 1. Generación local del firmware

Desde la raíz del proyecto, se puede compilar usando PlatformIO.

### Opción 1: comando estándar

```bash
pio run
```

### Opción 2: usando la ruta directa (Windows PowerShell)

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run
```

Al finalizar la compilación, el script `extra_scripts/copy_firmware.py` copia automáticamente los archivos resultantes a esta carpeta `firmware/`.

---

## 2. Integración Continua (CI)

El repositorio incluye un workflow de GitHub Actions que:

1. Instala PlatformIO.
2. Ejecuta `pio run`.
3. Copia los archivos generados a la carpeta `firmware/`.
4. Publica la carpeta completa como artifact descargable desde la página del workflow.

Los artifacts se encuentran en la pestaña *Actions*, dentro del run correspondiente.

---

## 3. Compilaciones mediante tags

El repositorio puede generar builds “versionadas” utilizando tags de Git.

Para crear un tag:

```bash
git tag v1.0.0
git push origin v1.0.0
```

La CI detecta el tag y genera un artifact asociado a esa versión.
Si se configura un workflow de releases, los mismos archivos `firmware.bin` y `firmware.elf` pueden añadirse automáticamente a la sección de Releases del repositorio.

---

## 4. Versionado en Git

Los archivos de firmware no se incluyen en el repositorio de forma permanente.
Se recomienda mantenerlos en `.gitignore`, ya que pueden regenerarse mediante:

* `pio run` localmente, o
* La descarga del artifact generado por GitHub Actions.
