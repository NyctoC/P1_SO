#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

using namespace std;

#define VERSION "1.2"

void show_help() {
    cout << "Uso: ./SircComp [opciones] <archivo>\n";
    cout << "Opciones:\n";
    cout << "  -h, --help          Muestra este mensaje\n";
    cout << "  -v, --version       Muestra la versión del programa\n";
    cout << "  -c, --compress      Comprime el archivo indicado\n";
    cout << "  -x, --decompress    Descomprime el archivo indicado\n";
    cout << "  -e, --encrypt       Encripta el archivo indicado\n";
    cout << "  -d, --decrypt       Desencripta el archivo indicado\n";
}

void show_version() {
    cout << "SircComp v" << VERSION << "\n";
}

string read_file(const string& filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        cerr << "Error: No se pudo abrir el archivo " << filename << "\n";
        exit(1);
    }

    vector<char> buffer(1024);
    string content;
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer.data(), buffer.size())) > 0) {
        content.append(buffer.data(), bytesRead);
    }

    if (bytesRead == -1) {
        cerr << "Error: No se pudo leer el archivo " << filename << "\n";
        close(fd);
        exit(1);
    }

    close(fd);
    return content;
}

vector<int> lzw_compress(const string& input) {
    unordered_map<string, int> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[string(1, i)] = i;
    }

    string P;
    vector<int> compressed;
    int code = 256;

    for (char C : input) {
        string PC = P + C;
        if (dictionary.find(PC) != dictionary.end()) {
            P = PC;
        } else {
            compressed.push_back(dictionary[P]);
            dictionary[PC] = code++;
            P = string(1, C);
        }
    }

    if (!P.empty()) {
        compressed.push_back(dictionary[P]);
    }

    return compressed;
}

void save_compressed_file(const string& filename, const vector<int>& compressed) {
    ofstream output_file(filename, ios::binary);
    for (int code : compressed) {
        while (code >= 0x80) { // Si tienes mas de 7 bits (128 en dec)
            output_file.put((code & 0x7F) | 0x80); // Extrae los 7 bits menos significativos de code // con un and 01111111 Luego agregamos 1000000 con 0x80 con un or
            code >>= 7; // Desplaza code 7 bits a la derecha 
        }
        output_file.put(code & 0x7F); // Agrega los 7 bits menos significativos de code
    }
    output_file.close();
}

vector<int> read_compressed_file(const string& filename) {
    ifstream input_file(filename, ios::binary);
    vector<int> compressed;
    int code = 0;
    int shift = 0;
    char byte;

    while (input_file.get(byte)) {
        code |= (byte & 0x7F) << shift; // Se extraen los 7 bits menos significativos de byte y se desplazan shift bits a la izquierda
        if (byte & 0x80) { // Si hay mas bytes, acumulamos mas bits pa leer (Aumentamos el shift)
            shift += 7;
        } else { // Si no, guardamos el codigo y reiniciamos
            compressed.push_back(code);
            code = 0;
            shift = 0;
        }
    }

    input_file.close();
    return compressed;
}

string lzw_decompress(const vector<int>& compressed) {
    unordered_map<int, string> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, i);
    }

    int code = 256;
    string P = dictionary[compressed[0]];
    string decompressed = P;

    for (size_t i = 1; i < compressed.size(); i++) {
        int K = compressed[i];
        string entry;
        if (dictionary.find(K) != dictionary.end()) {
            entry = dictionary[K];
        } else if (K == code) {
            entry = P + P[0];
        } else {
            cerr << "Error: Código inválido durante la descompresión\n";
            exit(1);
        }

        decompressed += entry;
        dictionary[code++] = P + entry[0];
        P = entry;
    }

    return decompressed;
}

void save_decompressed_file(const string& filename, const string& content) {
    ofstream output_file(filename);
    output_file << content;
    output_file.close();
}

// Encryption with euler's theorem
void encrypt_file(const string& filename, long long e, long long n) {
    string content = read_file(filename);
    vector<int> compressed = lzw_compress(content);

    vector<int> encrypted;
    for (int code : compressed) {
        long long c = 1;
        for (int i = 0; i < e; i++) {
            c = (c * code) % n;
        }
        encrypted.push_back(c);
    }

    string output_filename = string(filename) + ".enc";
    save_compressed_file(output_filename, encrypted);

    cout << "Archivo encriptado guardado como " << output_filename << "\n";
}

// Decryption with euler's theorem
void decrypt_file(const string& filename, long long d, long long n) {
    vector<int> encrypted = read_compressed_file(filename);

    vector<int> decrypted;
    for (int code : encrypted) {
        long long m = 1;
        for (int i = 0; i < d; i++) {
            m = (m * code) % n;
        }
        decrypted.push_back(m);
    }

    string decompressed = lzw_decompress(decrypted);
    string output_filename = string(filename) + "_d";

    save_decompressed_file(output_filename, decompressed);

    cout << "Archivo desencriptado guardado como " << output_filename << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: No se proporcionaron argumentos. Use -h para ayuda.\n";
        return 1;
    }

    string option = argv[1];
    if (option == "-h" || option == "--help") {
        show_help();
    } else if (option == "-v" || option == "--version") {
        show_version();
    } else if ((option == "-c" || option == "--compress") && argc == 3) {
        cout << "Compresión de archivo: " << argv[2] << "\n";
        string content = read_file(argv[2]);
        string output_filename = string(argv[2]) + ".sirc";

        vector<int> compressed = lzw_compress(content);
        save_compressed_file(output_filename, compressed);

        cout << "Archivo comprimido guardado como " << output_filename << "\n";
    } else if ((option == "-x" || option == "--decompress") && argc == 3) {
        cout << "Descompresión de archivo: " << argv[2] << "\n";
        vector<int> compressed = read_compressed_file(argv[2]);
        string decompressed = lzw_decompress(compressed);
        string output_filename = string(argv[2]) + "_d";

        save_decompressed_file(output_filename, decompressed);

        cout << "Archivo descomprimido guardado como " << output_filename << "\n";
    } else if ((option == "-e" || option == "--encrypt") && argc == 3) {
        long long e = 65537, n = 3233;  // Valores arbitrarios para pruebas
        cout << "Encriptación de archivo: " << argv[2] << "\n";
        encrypt_file(argv[2], e, n);
    } else if ((option == "-d" || option == "--decrypt") && argc == 3) {
        long long d = 2753, n = 3233;  // Valores arbitrarios para pruebas
        cout << "Desencriptación de archivo: " << argv[2] << "\n";
        decrypt_file(argv[2], d, n);
    } else {
        cerr << "Error: Opción inválida o falta de argumentos. Use -h para ayuda.\n";
        return 1;
    }

    return 0;
}
