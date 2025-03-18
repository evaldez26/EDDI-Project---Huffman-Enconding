#include "Huffman.h"

struct comp {//esto se puede poner como metodo, pero la logica es super mas compleja. Tambien se puede poner como clase pero no sabia se crear una clase por un metodo.
    bool operator()(Nodo* left, Nodo* right) {//metodo para que el nodo con menor frecuencia vaya primero en la pqueue
        if (left->getFreq() > right->getFreq()) {
            return true;
        }
        return false;
    }
};

void Huffman::encode(Nodo* raiz, string str, unordered_map<char, string>& huffCode)
{
    if (raiz == nullptr) {
        return;
    }

    if (raiz->isLeaf()) {
        huffCode[raiz->getCh()] = str;//cuando es un nodo hoja significa que es un caracter entonces declaramos el codigo del caracter correspondiente
    }

    encode(raiz->getLeft(), str + "0", huffCode);
    encode(raiz->getRight(), str + "1", huffCode);
}

void Huffman::decode(Nodo* raiz, int& indice, string str)
{//para el proceso cuando identifica un caracter 
    if (raiz == nullptr) {
        return; 
    }

    if (raiz->isLeaf()) {
        cout << raiz->getCh();
        return;
    }

    indice++;
    if (str[indice] == '0') {
        decode(raiz->getLeft(), indice, str);
    }
    else {
        decode(raiz->getRight(), indice, str);
    }
}

void Huffman::buildHuffmanTree(string texto)
{
    unordered_map<char, int>frecuencias;//unordered_map es un hashmap que no ordena las claves

    for (char c : texto) {//Recorre el texo y entra cada vez que encuentre el mismo caracter
        frecuencias[c]++;
    }

    priority_queue<Nodo*, vector<Nodo*>, comp> pqueue;
    for (auto pair : frecuencias) {
        pqueue.push(new Nodo(pair.first, pair.second));//anadimos a la pqueue el char y la frecuencia
    }

    while (pqueue.size() > 1) {
        Nodo* left = pqueue.top(); //.top() agarr el primer elemento de la pqueue
        pqueue.pop(); //elimina el primer elemento de la pqueue
        Nodo* right = pqueue.top();
        pqueue.pop();
        int sum = left->getFreq() + right->getFreq();
        pqueue.push(new Nodo('\0', sum, left, right));//creamos el nodo con caracter nulo
    }

    Nodo* raiz = pqueue.top();//el ultimo nodo que se creo es la raiz
    unordered_map<char, string> huffmanCode;
    encode(raiz, "", huffmanCode);

    cout << "Huffman Codes are:\n";
    for (auto pair : huffmanCode) {
        cout << pair.first << " " << pair.second << '\n';//declaramos el codigo de cada caracter
    }

    cout << "\nOriginal string was:\n" << texto << '\n';

    
    string codigoBin = "";
    for (char ch : texto) {
        codigoBin += huffmanCode[ch];//recorremos carfacter por caracter para codificar todo unido el texto original
    }

    cout << "\nEncoded string is:\n" << codigoBin << '\n';

    int indice = -1;
    cout << "\nDecoded string is:\n";
    
    while (indice < (int)codigoBin.size() - 2) {//como empezamos en -1 el indice hacemos un -2 en el .size
        decode(raiz, indice, codigoBin);
    }

    //parte para guardar en un archivo binario, obvio solo se ejecuta si se elige en el menu
    ofstream archivo("huffman.bin", ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    size_t tamanio = codigoBin.size();
    archivo.write(reinterpret_cast<const char*>(&tamanio), sizeof(tamanio));
    archivo.write(codigoBin.c_str(), tamanio);

    guardarArbol(archivo, raiz);
    archivo.close();

}


void Huffman::guardarArbol(ofstream& archivo, Nodo* nodo) { //solo sirve para guardar la estructura del arbol en el archivo binario
    if (!nodo) {
        archivo.put('#'); // indica nodo nulo
        return;
    }
    archivo.put(nodo->getCh()); // guarda char
    guardarArbol(archivo, nodo->getLeft());
    guardarArbol(archivo, nodo->getRight());
}

void Huffman::cargarTexto(string nombreArchivoTexto) {
    ifstream archivo(nombreArchivoTexto);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }
    stringstream buffer;
    buffer << archivo.rdbuf();

    //directamente llama a build con el string obtenido del archivo de texto
    buildHuffmanTree(buffer.str());
}


Nodo* Huffman::reconstruirArbol(ifstream& archivo) {
    char ch;
    archivo.get(ch);
    if (!archivo || ch == '#') {
        return nullptr; //nodo nulo
    }
    Nodo* nodo = new Nodo(ch, 0); //la frequencia no se ocupa
    nodo->setLeft(reconstruirArbol(archivo));
    nodo->setRight(reconstruirArbol(archivo));
    return nodo;
}


void Huffman::cargarCodificado(string nombreArchivoBinario) {
    ifstream archivo(nombreArchivoBinario, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    size_t tamanio;
    archivo.read(reinterpret_cast<char*>(&tamanio), sizeof(tamanio));

    string codigoBin(tamanio, '\0');
    archivo.read(&codigoBin[0], tamanio);

    //crea el arbol huffman
    Nodo* raiz = reconstruirArbol(archivo);
    archivo.close();

    if (!raiz) {
        cerr << "Error al reconstruir el arbol de Huffman." << endl;
        return;
    }

    //decodifica el mensaje
    int indice = -1;
    cout << "\nDecoded string is:\n";
    while (indice < (int)codigoBin.size() - 2) {
        decode(raiz, indice, codigoBin);
    }
}
