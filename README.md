# P1_SO
Parcial 1 Sistemas Operativos - Compresión

# SircComp v1.2

**SircComp** es una herramienta de compresión, descompresión, encriptación y desencriptación de archivos de texto, implementada utilizando el algoritmo LZW (Lempel–Ziv–Welch). Esta herramienta está diseñada para usuarios que necesitan manejar archivos de manera eficiente en sistemas Linux y se ofrece tanto como una aplicación de línea de comandos como con una interfaz gráfica simple.

## Características

- **Compresión LZW**: Utiliza el algoritmo LZW para comprimir archivos de texto.
- **Descompresión**: Descomprime archivos comprimidos utilizando LZW.
- **Encriptación y Desencriptación**: Permite encriptar y desencriptar archivos de texto (en la versión actual, se guarda el archivo comprimido como encriptado).
- **Interfaz de Línea de Comandos (CLI)**: Permite interactuar con el programa mediante opciones de línea de comandos.
- **Interfaz Gráfica de Usuario (GUI)**: Proporciona una interfaz simple con botones para abrir archivos y comprimirlos.

## Requisitos

- Sistema operativo Linux.
- C++11 o superior.
- Bibliotecas estándar de C++.

## Instalación

1. **Clonar el repositorio**:
    ```bash
    git clone https://github.com/tu-usuario/SircComp.git
    ```

2. **Compilar el código**:
    Asegúrate de tener instalado un compilador de C++ que soporte C++11 o superior.
    ```bash
    cd SircComp
    g++ -o SircComp SircComp.cpp
    ```

## Uso

### Línea de Comandos

Las opciones disponibles en la línea de comandos son:

```bash
Uso: ./SircComp [opciones] <archivo>

Opciones:
  -h, --help          Muestra este mensaje
  -v, --version       Muestra la versión del programa
  -c, --compress      Comprime el archivo indicado
  -x, --decompress    Descomprime el archivo indicado
  -e, --encrypt       Encripta el archivo indicado
  -d, --decrypt       Desencripta el archivo indicado