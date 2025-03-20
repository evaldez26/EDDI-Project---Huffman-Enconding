#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include "Huffman.h"
using namespace std;
int main()
{
    //para probar cargar un archivo de texto creen uno con la frase que quieran y lo ponen en el directorio del proyecto, y ponen el nombre del archivo en el menu
    //si prueban cargar un archivo binario sin haber corrido el programa obvio les va a decir que hubo error, tienen que correrlo primero
    Huffman huff;
    int opcion;
    string nombreArchivoTexto;
    string nombreArchivoBinario;
    string nombreArchivo;
    cout << "Seleccione una opcion:\n";
    cout << "1. Cargar texto para codificar\n";
    cout << "2. Cargar archivo binario codificado\n";
    cin >> opcion;
    do {
        switch (opcion) {
        case 1:
            cout << "Ingrese el nombre del archivo de texto a cargar: "; //estaba usando input.txt para el nombre del archivo, se podria cambiar y aqui poner como se llama
            cin >> nombreArchivoTexto;
            nombreArchivo = nombreArchivoTexto;
            huff.cargarTexto(nombreArchivoTexto);
            break;
        case 2:
            cout << "Ingrese el nombre del archivo binario a cargar: "; //el programa crea el archivo binario con el nombre "huffman.bin"
            cin >> nombreArchivoBinario;
            huff.cargarCodificado(nombreArchivoBinario);
            break;
        default:
            cout << "Opcion no valida\n";
			break;
        }
    
        cout << "\n\nSeleccione una opcion:\n";
        cout << "1. Cargar texto para codificar\n";
        cout << "2. Cargar archivo binario codificado\n";
        cout << "3. Salir\n";
        cin >> opcion;
    } while (opcion != 3);

    return 0;
}
