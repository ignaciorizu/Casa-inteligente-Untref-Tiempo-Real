#!/bin/bash

# Script simplificado para ejecutar todos los tests
echo "🏠 EJECUTANDO TODOS LOS TESTS DE CASA INTELIGENTE"
echo "================================================"

# Ejecutar el script principal de build
./test/build_test.sh

# Limpiar archivos objeto
echo ""
read -p "¿Limpiar archivos objeto (*.o)? [y/N]: " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    rm -f *.o
    echo "✅ Archivos objeto limpiados"
else
    echo "📁 Archivos objeto conservados"
fi