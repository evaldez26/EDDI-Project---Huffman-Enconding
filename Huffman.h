#pragma once
#include <fstream>
#include <sstream>
#include "Nodo.h"
using namespace std;
class Huffman
{

private:
public:
    void encode(Nodo* raiz, string str, unordered_map<char, string>& huffCode);
    void decode(Nodo* raiz, int& indice, string str);
    void buildHuffmanTree(string text);

    void guardarArbol(ofstream& archivo, Nodo* nodo);
    void cargarTexto(string nombreArchivoTexto);
    Nodo* reconstruirArbol(ifstream& archivo);
    void cargarCodificado(string nombreArchivoBinario);
};
