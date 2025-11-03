#!/bin/bash

# ====================================================================
# SCRIPT DE AUTOMATIZACIÓN DE BENCHMARKS (Sección 5.e)
# CORRECCIÓN: Se utiliza e(x * l(2)) en bc para evitar el warning
# ====================================================================

# Configuración del script
EXECUTABLE="./case_converter"
CSV_FILE="performance_results.csv"
NUM_STEPS=50

# --- Definición de Rangos ---
ALIGNMENTS=(32 1)
ALPHA_PERCENTS=(0 10 20 30 40 50 60 70 80 90 100)

# Tamaños de Cadena (Logarítmico)
MIN_LOG_SIZE=5.0
MAX_LOG_SIZE=23.0
STEP_SIZE_LOG=$(echo "scale=4; ($MAX_LOG_SIZE - $MIN_LOG_SIZE) / ($NUM_STEPS - 1)" | bc)

# Verificar que el ejecutable exista
if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: El ejecutable '$EXECUTABLE' no se encuentra."
    echo "Por favor, compile el código con 'make' primero."
    exit 1
fi

# Preparar el archivo CSV
echo "Length_Bytes,Alignment_Bytes,Alpha_Percent,Time_Serial_ns,Time_SIMD_ns" > $CSV_FILE
TOTAL_MEASUREMENTS=$(($NUM_STEPS * ${#ALIGNMENTS[@]} * ${#ALPHA_PERCENTS[@]}))
echo "Iniciando $NUM_STEPS tamaños x ${#ALIGNMENTS[@]} alineamientos x ${#ALPHA_PERCENTS[@]} porcentajes = $TOTAL_MEASUREMENTS mediciones."
echo "Resultados guardados en $CSV_FILE..."

# Bucle principal de generación de tamaños (logarítmico)
CURRENT_LOG_SIZE=$MIN_LOG_SIZE
for i in $(seq 1 $NUM_STEPS); do
    
    # LÍNEA CORREGIDA: Se usa la fórmula 2^x = e^(x * ln(2)) para evitar el warning 'non-zero scale in exponent'
    # l(2) es el logaritmo natural de 2 en bc -l.
    # Se aumenta la precisión (scale=10) en bc para asegurar un cálculo de punto flotante correcto.
    LENGTH=$(echo "scale=10; e($CURRENT_LOG_SIZE * l(2))" | bc -l | awk '{printf "%.0f\n", $0}')

    for ALIGNMENT in "${ALIGNMENTS[@]}"; do
        for ALPHA_PERCENT in "${ALPHA_PERCENTS[@]}"; do
            
            OUTPUT=$("$EXECUTABLE" "$LENGTH" "$ALIGNMENT" "$ALPHA_PERCENT" 2>/dev/null)
            
            if [ $? -eq 0 ] && [ -n "$OUTPUT" ]; then
                echo "$LENGTH,$ALIGNMENT,$ALPHA_PERCENT,$OUTPUT" >> $CSV_FILE
            fi
        done
    done

    # Aumentar el tamaño logarítmico para el siguiente paso
    CURRENT_LOG_SIZE=$(echo "scale=4; $CURRENT_LOG_SIZE + $STEP_SIZE_LOG" | bc)
done

echo -e "\nMediciones completadas. Resultados guardados en $CSV_FILE."