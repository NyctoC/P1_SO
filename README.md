# P1_SO
Parcial 1 Sistemas Operativos - Compresión

# SircComp v2.0

**SircComp** es una herramienta de compresión, descompresión, encriptación y desencriptación de archivos de texto, implementada utilizando el algoritmo LZW (Lempel–Ziv–Welch) y El Teorema de Euler. Esta herramienta está diseñada para usuarios que necesitan manejar archivos de manera eficiente en sistemas Linux y se ofrece tanto como una aplicación de línea de comandos como con una interfaz gráfica simple (Hasta la fecha solo funciona para comprimir).

## Características

- **Compresión LZW**: Utiliza el algoritmo *LZW* para comprimir archivos de texto.
- **Descompresión**: Descomprime archivos comprimidos utilizando *LZW*.
- **Bonus: Encriptación y Desencriptación**: Permite encriptar y desencriptar archivos de texto con una implementación basica del *Teorema de Euler*.
- **Interfaz de Línea de Comandos (CLI)**: Permite interactuar con el programa mediante opciones de línea de comandos.
- **Interfaz Gráfica de Usuario (GUI)**: Proporciona una interfaz simple con botones para abrir archivos y comprimirlos.

## Requisitos

- Sistema operativo Linux.
- C++11 o superior.
- Bibliotecas estándar de C++ (Y gtk si planea usar la interfaz visual).

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

```

## Interfaz Gráfica de Usuario (GUI)

La GUI permite al usuario interactuar con el programa a través de dos botones principales:

1. Abrir un archivo de texto: Permite seleccionar y cargar un archivo ```.txt``` para su compresión.
2. Comprimir: Comprime el archivo seleccionado utilizando el algoritmo LZW y lo guarda con una extensión ```.sirc```.