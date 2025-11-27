#!/bin/bash

# =============================================
# 🏠 SCRIPT DE TESTING - CASA INTELIGENTE
# =============================================

echo "🏠 COMPILANDO Y EJECUTANDO TESTS DE CASA INTELIGENTE"
echo "==================================================="

# Detectar el sistema operativo y configurar rutas
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    CXX="${CXX:-g++}"
    INCLUDES="-I. -I./test -I./test/mocks -I/usr/include"
    LIBS="-lgtest -lgtest_main -pthread"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    # Windows (MSYS2)
    CXX="${CXX:-g++}"
    INCLUDES="-I. -I./test -I./test/mocks -IC:/msys64/mingw64/include"
    LIBS="-LC:/msys64/mingw64/lib -lgtest -lgtest_main -static"
else
    # macOS u otros
    CXX="${CXX:-g++}"
    INCLUDES="-I. -I./test -I./test/mocks"
    LIBS="-lgtest -lgtest_main -pthread"
fi

CXXFLAGS="-std=c++17 -DUNIT_TEST -g -Wall"
MOCKS_FILE="test/mocks.cpp"

# Archivos de componentes principales (puedes expandir esta lista)
COMPONENTS=(
    "Alarma"
    "PantallaLCD"
    "ConfigManager"
    "SensorTemperatura"
    "SensorLuz"
    "SensorMovimiento"
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
    check_file "./src/${comp}.cpp" || exit 1
    check_file "./include/${comp}.h" || exit 1
done

check_file "./IRCodes.h" || exit 1
check_file "./test/mocks.cpp" || exit 1

# Limpiar compilaciones anteriores
print_step "Limpiando compilaciones anteriores..."
rm -f *.o
rm -f test_*

# Compilar mocks
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

# Tests a ejecutar
TESTS=(
    "test_alarma"
    "test_pantalla"
    "test_sensores"
)

TEST_RESULTS=()
for test_file in "${TESTS[@]}"; do
    print_step "Compilando $test_file..."

    # Compilar test
    $CXX $CXXFLAGS $INCLUDES -c "./test/tests/${test_file}.cpp" -o "${test_file}.o"
    if [ $? -ne 0 ]; then
        echo "❌ Error compilando $test_file"
        TEST_RESULTS+=("${test_file}:COMPILATION_FAILED")
        continue
    fi
    echo "✅ $test_file compilado"

    # Enlazar
    print_step "Enlazando $test_file..."
    OUTPUT_FILE="test_${test_file}"

    $CXX "${test_file}.o" $OBJECT_FILES $LIBS -o "$OUTPUT_FILE"
    if [ $? -ne 0 ]; then
        echo "❌ Error enlazando $test_file"
        TEST_RESULTS+=("${test_file}:LINK_FAILED")
        continue
    fi
    echo "✅ $test_file enlazado"

    # Ejecutar test
    print_step "Ejecutando $test_file..."
    if ./"$OUTPUT_FILE" --gtest_output="xml:${test_file}_results.xml"; then
        echo "✅ $test_file: PASÓ"
        TEST_RESULTS+=("${test_file}:PASS")
    else
        echo "❌ $test_file: FALLÓ (código: $?)"
        TEST_RESULTS+=("${test_file}:FAIL")
    fi
done

# Mostrar resumen final
echo ""
echo "=========================================="
echo "📊 RESUMEN FINAL DE TESTS"
echo "=========================================="
for result in "${TEST_RESULTS[@]}"; do
    echo "  $result"
done

echo ""
echo "=========================================="
echo "🚀 PROCESO DE TESTING COMPLETADO"

# Retornar código de error si algún test falló
for result in "${TEST_RESULTS[@]}"; do
    if [[ $result == *":FAIL"* || $result == *":COMPILATION_FAILED"* || $result == *":LINK_FAILED"* ]]; then
        exit 1
    fi
done

exit 0