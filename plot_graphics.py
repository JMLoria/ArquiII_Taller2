import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

CSV_FILE = 'performance_results.csv'
OUTPUT_DIR = 'graphics'

def load_data():
    """Carga los datos y calcula las métricas de rendimiento."""
    try:
        df = pd.read_csv(CSV_FILE)
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {CSV_FILE}. Ejecuta 'make benchmark' primero.")
        return None
    
    # Calcular Speedup y convertir tiempo a microsegundos (us) para mejor lectura
    df['Time_Serial_us'] = df['Time_Serial_ns'] / 1000
    df['Time_SIMD_us'] = df['Time_SIMD_ns'] / 1000
    df['Speedup'] = df['Time_Serial_ns'] / df['Time_SIMD_ns']
    
    return df

def plot_performance_vs_length(df):
    """Gráfico 1: Muestra el tiempo de ejecución vs. la longitud (Log Scale), separado por alineamiento."""
    
    # Promediar el tiempo de ejecución para cada Longitud y Alineamiento (ignorando Alpha_Percent)
    # Esto simula un análisis de escalabilidad puro.
    df_grouped = df.groupby(['Length_Bytes', 'Alignment_Bytes']).mean().reset_index()

    plt.figure(figsize=(12, 6))
    
    # Separar por alineamiento
    df_aligned = df_grouped[df_grouped['Alignment_Bytes'] == 32]
    df_unaligned = df_grouped[df_grouped['Alignment_Bytes'] == 1]

    # Plot para datos Alineados (32 bytes)
    plt.plot(df_aligned['Length_Bytes'], df_aligned['Time_Serial_us'], 
             label='Serial', color='blue', linestyle='-')
    plt.plot(df_aligned['Length_Bytes'], df_aligned['Time_SIMD_us'], 
             label='SIMD (32-Byte Alineado)', color='red', linestyle='-')
    
    # Plot para datos NO Alineados (1 byte)
    plt.plot(df_unaligned['Length_Bytes'], df_unaligned['Time_SIMD_us'], 
             label='SIMD (No Alineado)', color='pink', linestyle='--')
    
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Tamaño de la Cadena (m) [Bytes] - Escala Logarítmica')
    plt.ylabel('Tiempo de Ejecución Promedio [μs] - Escala Logarítmica')
    plt.title('Comparación de Desempeño: Serial vs. SIMD (AVX2)')
    plt.grid(True, which="both", ls="--", c='0.7')
    plt.legend()
    plt.savefig(f'{OUTPUT_DIR}/1_Performance_vs_Length.png')
    plt.close()
    print(f"Gráfico 1 generado: {OUTPUT_DIR}/1_Performance_vs_Length.png")


def plot_speedup_vs_length(df):
    """Gráfico 2: Muestra el Speedup (Aceleración) vs. la longitud (Log Scale)."""
    
    df_grouped = df.groupby(['Length_Bytes', 'Alignment_Bytes']).mean().reset_index()
    
    plt.figure(figsize=(12, 6))
    
    df_aligned = df_grouped[df_grouped['Alignment_Bytes'] == 32]
    df_unaligned = df_grouped[df_grouped['Alignment_Bytes'] == 1]

    plt.plot(df_aligned['Length_Bytes'], df_aligned['Speedup'], 
             label='Speedup (32-Byte Alineado)', color='green', linestyle='-')
    plt.plot(df_unaligned['Length_Bytes'], df_unaligned['Speedup'], 
             label='Speedup (No Alineado)', color='orange', linestyle='--')

    plt.xscale('log')
    plt.xlabel('Tamaño de la Cadena (m) [Bytes] - Escala Logarítmica')
    plt.ylabel('Speedup (Tiempo Serial / Tiempo SIMD)')
    plt.title('Escalabilidad del Speedup vs. Longitud (Sección 4.e)')
    plt.axhline(1.0, color='r', linestyle=':', label='Línea de Base (Speedup = 1)')
    plt.grid(True, which="both", ls="--", c='0.7')
    plt.legend()
    plt.savefig(f'{OUTPUT_DIR}/2_Speedup_vs_Length.png')
    plt.close()
    print(f"Gráfico 2 generado: {OUTPUT_DIR}/2_Speedup_vs_Length.png")


def plot_alpha_impact(df):
    """Gráfico 3: Muestra el tiempo promedio vs. el porcentaje alfabético."""
    
    # Seleccionar un tamaño grande (ej. 8MB o el máximo) para minimizar el ruido del setup
    max_length = df['Length_Bytes'].max()
    df_large = df[df['Length_Bytes'] == max_length]
    
    # Promediar por Porcentaje Alfabético
    df_grouped = df_large.groupby('Alpha_Percent').mean().reset_index()

    plt.figure(figsize=(10, 5))
    
    plt.plot(df_grouped['Alpha_Percent'], df_grouped['Time_Serial_us'], 
             label='Serial', marker='o', color='blue')
    plt.plot(df_grouped['Alpha_Percent'], df_grouped['Time_SIMD_us'], 
             label='SIMD', marker='o', color='red')
    
    
    plt.xlabel('Porcentaje de Caracteres Alfabéticos')
    plt.ylabel('Tiempo de Ejecución Promedio [μs]')
    plt.title(f'Impacto del Contenido vs. Tiempo de Ejecución (Tamaño: {max_length} B)')
    plt.xticks(df_grouped['Alpha_Percent'])
    plt.grid(True, axis='y', ls="--", c='0.7')
    plt.legend()
    plt.savefig(f'{OUTPUT_DIR}/3_Alpha_Percent_Impact.png')
    plt.close()
    print(f"Gráfico 3 generado: {OUTPUT_DIR}/3_Alpha_Percent_Impact.png")


if __name__ == '__main__':
    # Crear directorio para guardar las gráficas
    import os
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)
        
    df = load_data()
    if df is not None:
        plot_performance_vs_length(df)
        plot_speedup_vs_length(df)
        plot_alpha_impact(df)
        print("\nProceso de generación de gráficos finalizado. Revisa la carpeta 'graphics'.")