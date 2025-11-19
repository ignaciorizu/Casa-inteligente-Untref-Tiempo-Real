#!/bin/bash

echo "🏠 EJECUTANDO TODOS LOS TESTS DEL SISTEMA"
echo "=========================================="

# Ejecutar tests de Alarma
echo ""
echo "🔔 EJECUTANDO TESTS DE ALARMA..."
./build_test_alarma.sh
ALARMA_RESULT=$?

# Ejecutar tests de Pantalla LCD
echo ""
echo "🖥️ EJECUTANDO TESTS DE PANTALLA LCD..."
./build_test_pantalla.sh
PANTALLA_RESULT=$?

# Ejecutar tests de Sensor de Temperatura
echo ""
echo "🌡️ EJECUTANDO TESTS DE SENSOR TEMPERATURA..."
./build_test_sensor_temperatura.sh
SENSOR_RESULT=$?

# Ejecutar tests de Sensor de Movimiento
echo ""
echo "🏃 EJECUTANDO TESTS DE SENSOR MOVIMIENTO..."
./build_test_sensor_movimiento.sh
SENSOR_MOV_RESULT=$?

# Ejecutar tests de Sensor de Luz
echo ""
echo "💡 EJECUTANDO TESTS DE SENSOR LUZ..."
./build_test_sensor_luz.sh
SENSOR_LUZ_RESULT=$?

echo ""
echo "=========================================="
echo "📊 RESUMEN FINAL DE TESTS"
echo "=========================================="

if [ $ALARMA_RESULT -eq 0 ]; then
    echo "✅ Tests de Alarma: PASARON"
else
    echo "❌ Tests de Alarma: FALLARON"
fi

if [ $PANTALLA_RESULT -eq 0 ]; then
    echo "✅ Tests de Pantalla LCD: PASARON"
else
    echo "❌ Tests de Pantalla LCD: FALLARON"
fi

if [ $SENSOR_RESULT -eq 0 ]; then
    echo "✅ Tests de Sensor Temperatura: PASARON"
else
    echo "❌ Tests de Sensor Temperatura: FALLARON"
fi

if [ $SENSOR_MOV_RESULT -eq 0 ]; then
    echo "✅ Tests de Sensor Movimiento: PASARON"
else
    echo "❌ Tests de Sensor Movimiento: FALLARON"
fi

if [ $SENSOR_LUZ_RESULT -eq 0 ]; then
    echo "✅ Tests de Sensor Luz: PASARON"
else
    echo "❌ Tests de Sensor Luz: FALLARON"
fi

TOTAL_RESULT=$((ALARMA_RESULT + PANTALLA_RESULT + SENSOR_RESULT + SENSOR_MOV_RESULT + SENSOR_LUZ_RESULT))

echo ""
if [ $TOTAL_RESULT -eq 0 ]; then
    echo "🎉 ¡TODOS LOS TESTS DEL SISTEMA PASARON!"
    echo "🚀 Sistema listo para producción"
else
    echo "💥 ALGUNOS TESTS FALLARON"
    echo "🔧 Revisar las implementaciones"
fi

exit $TOTAL_RESULT