# extra_scripts/copy_firmware.py
from pathlib import Path
from shutil import copy2

Import("env")

def copy_firmware(source, target, env):
    project_dir = Path(env["PROJECT_DIR"])

    # Carpeta destino dentro del repo
    firmware_dir = project_dir / "firmware"
    firmware_dir.mkdir(exist_ok=True)

    # BUILD_DIR suele ser: .pio/build/esp-wrover-kit
    build_dir = Path(env.subst("$BUILD_DIR"))

    # Rutas origen del bin y elf generados por PlatformIO
    bin_src = build_dir / "firmware.bin"
    elf_src = build_dir / "firmware.elf"

    # Rutas destino fijas
    bin_dst = firmware_dir / "firmware.bin"
    elf_dst = firmware_dir / "firmware.elf"

    # Copiar (si existen)
    if bin_src.exists():
        copy2(bin_src, bin_dst)
        print(f"[copy_firmware] Copiado {bin_src} -> {bin_dst}")
    else:
        print(f"[copy_firmware] WARNING: No se encontró {bin_src}")

    if elf_src.exists():
        copy2(elf_src, elf_dst)
        print(f"[copy_firmware] Copiado {elf_src} -> {elf_dst}")
    else:
        print(f"[copy_firmware] WARNING: No se encontró {elf_src}")

# Se ejecuta después de la tarea 'buildprog'
env.AddPostAction("buildprog", copy_firmware)
