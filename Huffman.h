#pragma once
#include <fstream>
#include <sstream>
#include <windows.h>
#include "Nodo.h"
using namespace std;
class Huffman
{

private:
public:
    void encode(Nodo* raiz, string str, unordered_map<char, string>& huffCode);
    void decode(Nodo* raiz, int& indice, string str);
    void crearHuffmanTree(string text, string nombreArchivo);

    void guardarArbol(ofstream& archivo, Nodo* nodo);
    void cargarTexto(string nombreArchivoTexto);
    Nodo* reconstruirArbol(ifstream& archivo);
    void cargarCodificado(string nombreArchivoBinario);

    void compararSizeArchivo(string textFile, string binFile);
};
