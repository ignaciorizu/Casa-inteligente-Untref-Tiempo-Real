#!/bin/bash

# =============================================
# 🏠 SCRIPT DE TESTING - CASA INTELIGENTE
# =============================================

echo "🏠 COMPILANDO Y EJECUTANDO TESTS DE CASA INTELIGENTE"
echo "==================================================="

# Configuración específica para MSYS2 (igual que tu ejemplo funcionando)
CXX="g++"
CXXFLAGS="-std=c++17 -DUNIT_TEST"
INCLUDES="-I. -I./test -I./test/mocks -IC:/msys64/mingw64/include"
LIBS="-LC:/msys64/mingw64/lib -lgtest -lgtest_main -static"
MOCKS_FILE="test/mocks.cpp"
OUTPUT_PREFIX="test"

# Archivos de componentes principales
COMPONENTS=(
    "Alarma"
)

# Función para imprimir mensajes
print_step() {
    echo ""
    echo "🔧 $1"
}

# Función para verificar archivos
check_file() {
    if [ ! -f "$1" ]; then
        echo "❌ ERROR: Archivo no encontrado: $1"
        return 1
    fi
    echo "✅ Archivo encontrado: $1"
    return 0
}

# Verificar archivos necesarios
print_step "Verificando archivos..."
for comp in "${COMPONENTS[@]}"; do
    check_file "./${comp}.cpp" || exit 1
    check_file "./${comp}.h" || exit 1
done

check_file "./IRCodes.h" || exit 1
check_file "./test/mocks.cpp" || exit 1

# Limpiar compilaciones anteriores
print_step "Limpiando compilaciones anteriores..."
rm -f *.o
rm -f ${OUTPUT_PREFIX}_*

# Compilar mocks (igual que tu ejemplo funcionando)
print_step "Compilando mocks..."
$CXX $CXXFLAGS $INCLUDES -c "./test/mocks.cpp" -o mocks.o
if [ $? -ne 0 ]; then
    echo "❌ Error compilando mocks"
    exit 1
fi
echo "✅ Mocks compilados"

# Compilar componentes principales
OBJECT_FILES="mocks.o"
for comp in "${COMPONENTS[@]}"; do
    print_step "Compilando $comp..."
    $CXX $CXXFLAGS $INCLUDES -c "./${comp}.cpp" -o "${comp}.o"
    if [ $? -ne 0 ]; then
        echo "❌ Error compilando $comp"
        exit 1
    fi
    OBJECT_FILES="$OBJECT_FILES ${comp}.o"
    echo "✅ $comp compilado"
done

# Tests a ejecutar (empezamos solo con test_alarma)
TESTS=(
    "test_alarma"
)

for test_file in "${TESTS[@]}"; do
    print_step "Compilando $test_file..."

    # Compilar test
    $CXX $CXXFLAGS $INCLUDES -c "./test/tests/${test_file}.cpp" -o "${test_file}.o"
    if [ $? -ne 0 ]; then
        echo "❌ Error compilando $test_file"
        continue
    fi
    echo "✅ $test_file compilado"

    # Enlazar (igual que tu ejemplo funcionando)
    print_step "Enlazando $test_file..."
    OUTPUT_FILE="${test_file}.exe"

    $CXX "${test_file}.o" $OBJECT_FILES $LIBS -o "$OUTPUT_FILE"
    if [ $? -ne 0 ]; then
        echo "❌ Error enlazando $test_file"
        echo "Probando enlazado alternativo..."
        # Intentar sin rutas específicas (igual que tu ejemplo)
        $CXX "${test_file}.o" $OBJECT_FILES -lgtest -lgtest_main -o "$OUTPUT_FILE"
        if [ $? -ne 0 ]; then
            echo "💥 Error crítico en enlazado"
            continue
        fi
    fi
    echo "✅ $test_file enlazado"

    # Verificar que el ejecutable se creó
    if [ ! -f "$OUTPUT_FILE" ]; then
        echo "❌ ERROR: No se pudo crear el ejecutable $OUTPUT_FILE"
        continue
    fi

    # Ejecutar test
    echo ""
    echo "🧪 EJECUTANDO: $test_file"
    echo "=========================================="
    chmod +x "$OUTPUT_FILE"
    if ./"$OUTPUT_FILE"; then
        echo "✅ $test_file: PASÓ"
    else
        echo "❌ $test_file: FALLÓ (código: $?)"
    fi
    echo ""
done

# Mostrar resumen final
echo ""
echo "=========================================="
echo "📊 RESUMEN FINAL"
echo "=========================================="
echo "Ejecutables creados:"
ls ${OUTPUT_PREFIX}_* 2>/dev/null || echo "Ninguno"

echo ""
echo "=========================================="
echo "🚀 PROCESO DE TESTING COMPLETADO"