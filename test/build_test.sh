#!/bin/bash

# =============================================
# 🏠 SCRIPT DE TESTING - CASA INTELIGENTE
# =============================================

echo "🏠 COMPILANDO Y EJECUTANDO TESTS DE CASA INTELIGENTE"
echo "==================================================="

# Detectar el sistema operativo - ESTO FUNCIONA PARA AMBOS ENTORNOS
if [[ "$OSTYPE" == "linux-gnu"* || "$RUNNER_OS" == "Linux" ]]; then
    # Linux (GitHub Actions o local)
    CXX="${CXX:-g++}"
    INCLUDES="-I. -I./test -I./test/mocks -I/usr/include"
    LIBS="-lgtest -lgtest_main -pthread"
    echo "🔧 Sistema: Linux"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" || "$RUNNER_OS" == "Windows" ]]; then
    # Windows (solo local en tu máquina)
    CXX="${CXX:-g++}"
    INCLUDES="-I. -I./test -I./test/mocks -IC:/msys64/mingw64/include"
    LIBS="-LC:/msys64/mingw64/lib -lgtest -lgtest_main -static"
    echo "🔧 Sistema: Windows (local)"
else
    # macOS u otros
    CXX="${CXX:-g++}"
    INCLUDES="-I. -I./test -I./test/mocks"
    LIBS="-lgtest -lgtest_main -pthread"
    echo "🔧 Sistema: Otro ($OSTYPE)"
fi

# Mostrar información del compilador
echo "🔧 Compilador: $CXX"
$CXX --version || echo "⚠️ No se pudo obtener versión del compilador"

CXXFLAGS="-std=c++17 -DUNIT_TEST -g -Wall"
MOCKS_FILE="test/mocks.cpp"

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
)

TEST_RESULTS=()
for test_file in "${TESTS[@]}"; do
    print_step "Compilando $test_file..."

    # Verificar que existe el archivo de test
    if [ ! -f "./test/tests/${test_file}.cpp" ]; then
        echo "❌ Archivo de test no encontrado: ./test/tests/${test_file}.cpp"
        TEST_RESULTS+=("${test_file}:FILE_NOT_FOUND")
        continue
    fi

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
    if [ -f "./$OUTPUT_FILE" ]; then
        if ./"$OUTPUT_FILE" --gtest_output="xml:${test_file}_results.xml"; then
            echo "✅ $test_file: PASÓ"
            TEST_RESULTS+=("${test_file}:PASS")
        else
            echo "❌ $test_file: FALLÓ (código: $?)"
            TEST_RESULTS+=("${test_file}:FAIL")
        fi
    else
        echo "❌ $test_file: Ejecutable no encontrado"
        TEST_RESULTS+=("${test_file}:EXE_NOT_FOUND")
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
    if [[ $result == *":FAIL"* || $result == *":COMPILATION_FAILED"* || $result == *":LINK_FAILED"* || $result == *":FILE_NOT_FOUND"* || $result == *":EXE_NOT_FOUND"* ]]; then
        echo "❌ ALGUNOS TESTS FALLARON"
        exit 1
    fi
done

echo "✅ TODOS LOS TESTS PASARON"
exit 0