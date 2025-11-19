#!/bin/bash

# =============================================
# 🌡️ SCRIPT DE TESTING - SENSOR DE TEMPERATURA
# =============================================

echo "🌡️ COMPILANDO Y EJECUTANDO TESTS DE SENSOR DE TEMPERATURA"
echo "========================================================="

# Configuración
CXX="g++"
CXXFLAGS="-std=c++17 -DUNIT_TEST"
INCLUDES="-I./lib -I./lib/Interfaces -IC:/msys64/mingw64/include"
LIBS="-LC:/msys64/mingw64/lib -lgtest -lgtest_main -static"
MOCKS_FILE="test/mocks.cpp"
SENSOR_CPP="lib/Sensores/SensorTemperatura.cpp"
TEST_FILE="test/test_sensor_temperatura_gtest.cpp"
OUTPUT="test_sensor_temperatura_gtest.exe"

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
check_file "$MOCKS_FILE" || exit 1
check_file "$SENSOR_CPP" || exit 1
check_file "$TEST_FILE" || exit 1

# Limpiar compilaciones anteriores
print_step "Limpiando compilaciones anteriores..."
rm -f *.o
rm -f "$OUTPUT"

# Compilar mocks
print_step "Compilando mocks..."
$CXX $CXXFLAGS $INCLUDES -c "$MOCKS_FILE" -o mocks.o
if [ $? -ne 0 ]; then
    echo "❌ Error compilando mocks"
    exit 1
fi
echo "✅ Mocks compilados"

# Compilar implementación de SensorTemperatura
print_step "Compilando implementación de SensorTemperatura..."
$CXX $CXXFLAGS $INCLUDES -c "$SENSOR_CPP" -o SensorTemperatura.o
if [ $? -ne 0 ]; then
    echo "❌ Error compilando SensorTemperatura.cpp"
    exit 1
fi
echo "✅ SensorTemperatura.cpp compilado"

# Compilar tests
print_step "Compilando tests..."
$CXX $CXXFLAGS $INCLUDES -c "$TEST_FILE" -o test_sensor_temperatura_gtest.o
if [ $? -ne 0 ]; then
    echo "❌ Error compilando tests"
    exit 1
fi
echo "✅ Tests compilados"

# Enlazar todo
print_step "Enlazando ejecutable..."
$CXX test_sensor_temperatura_gtest.o SensorTemperatura.o mocks.o $LIBS -o "$OUTPUT"
if [ $? -ne 0 ]; then
    echo "❌ Error enlazando"
    echo "Probando enlazado alternativo..."
    # Intentar sin rutas específicas
    $CXX test_sensor_temperatura_gtest.o SensorTemperatura.o mocks.o -lgtest -lgtest_main -o "$OUTPUT"
    if [ $? -ne 0 ]; then
        echo "💥 Error crítico en enlazado"
        exit 1
    fi
fi
echo "✅ Ejecutable enlazado"

# Verificar que el ejecutable se creó
if [ ! -f "$OUTPUT" ]; then
    echo "❌ ERROR: No se pudo crear el ejecutable"
    exit 1
fi

# Ejecutar tests
echo ""
echo "========================================================="
echo "🧪 EJECUTANDO TESTS DE SENSOR DE TEMPERATURA"
echo "========================================================="
chmod +x "$OUTPUT"
./"$OUTPUT"

# Capturar el código de salida
TEST_RESULT=$?
echo ""
echo "========================================================="

# Mostrar resultado final
if [ $TEST_RESULT -eq 0 ]; then
    echo "🎉 ¡TODOS LOS TESTS PASARON!"
    echo "✅ Sensor de Temperatura funcionando correctamente"
else
    echo "💥 ALGUNOS TESTS FALLARON (código: $TEST_RESULT)"
    echo "❌ Revisar la implementación"
fi

echo ""
echo "📊 Resumen:"
echo "   - Tests ejecutados: ./$OUTPUT"
echo "   - Archivos objeto: *.o"
echo "   - Ejecutable: $OUTPUT"

# Limpiar archivos objeto (opcional)
echo ""
read -p "¿Limpiar archivos objeto (*.o)? [y/N]: " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    rm -f *.o
    echo "✅ Archivos objeto limpiados"
else
    echo "📁 Archivos objeto conservados para debugging"
fi

echo ""
echo "========================================================="
echo "🚀 PROCESO DE TESTING COMPLETADO"
exit $TEST_RESULT