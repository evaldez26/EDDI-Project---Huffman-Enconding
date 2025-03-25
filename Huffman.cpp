#include "Huffman.h"
#include <iomanip>

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
{
    //Hace que la consola use UTF-8 para el output y pueda mostrar caracteres especiales
    SetConsoleOutputCP(65001);

    if (raiz == nullptr) {
        return;
    }

    if (raiz->isLeaf()) {
        cout << raiz->getCh();//para el proceso cuando identifica un caracter 
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

void Huffman::crearHuffmanTree(string texto, string nombreArchivo)
{
    //Hace que la consola use UTF-8 para el output y pueda mostrar caracteres especiales
    SetConsoleOutputCP(65001);
    unordered_map<char, int>frecuencias;//unordered_map es un hashmap que no ordena las claves

    for (char c : texto) {//Recorre el texo y entra cada vez que encuentre el mismo caracter
        frecuencias[c]++;
    }
    priority_queue<Nodo*, vector<Nodo*>, comp> pqueue;//creamos una pqeueu para alamcenar los caracteres y usamos el comparador para que se ordene minheap
    for (auto pair : frecuencias) {
        pqueue.push(new Nodo(pair.first, pair.second));//anadimos a la pqueue el char y la frecuencia
    }
    if (pqueue.size() == 1) {//si solo hay un caracter en el texto
        pqueue.push(new Nodo('\0', 1));//anadimos un caracter nulo
    }

    while (pqueue.size() > 1) {//la pqueue size es de cuantos caracteres existen que no sean repetidos y se deja de leer cuando ya solo quede el padre
        Nodo* left = pqueue.top(); //.top() agarr el primer elemento de la pqueue
        pqueue.pop(); //elimina el primer elemento de la pqueue
        Nodo* right = pqueue.top();
        pqueue.pop();

        int sum = left->getFreq() + right->getFreq();
        pqueue.push(new Nodo('\0', sum, left, right));//creamos el nodo con caracter nulo (esto es como cuando creabamos Xo apuntando a otros caracteres)
    }

    Nodo* raiz = pqueue.top();//el ultimo nodo que se creo es la raiz
    unordered_map<char, string> huffmanCode;//mapa para almacenar el caracter y su respectiva frecuencia
    encode(raiz, "", huffmanCode);

    cout << "Los codigos huffman:\n";
    for (auto pair : huffmanCode) {
        if (pair.first != '\0') {
            cout << pair.first << " " << pair.second << '\n';//declaramos el codigo de cada caracter
        }
    }

    cout << "\nLa cadena original es:\n" << texto << '\n';


    string codigoBin = "";
    for (char ch : texto) {
        codigoBin += huffmanCode[ch];//recorremos carfacter por caracter para codificar todo unido el texto original
    }

    cout << "\nLa cadena codificada es:\n" << codigoBin << '\n';

    nombreArchivo = nombreArchivo.substr(0, nombreArchivo.size() - 4);//quitamos la extension del archivo .txt

    ofstream archivo(nombreArchivo + ".bin", ios::binary);//creamos un archivo binario con el nombre que tiene el archivo de texto 
    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }


    // Calcular el tamaño en bits
    size_t tamanioBits = codigoBin.size(); // Número de bits necesarios

    // Guardar el tamaño en bits en el archivo
    archivo.write(reinterpret_cast<const char*>(&tamanioBits), sizeof(tamanioBits));

    // Convertir el código binario a un vector de bytes
    vector<uint8_t> bytes((tamanioBits + 7) / 8, 0);  // Ajustar el tamaño de bytes

    for (size_t i = 0; i < tamanioBits; ++i) {
        size_t byteIndex = i / 8;
        size_t bitIndex = 7 - (i % 8); // Coloca el bit en la posición correcta
        if (codigoBin[i] == '1') {
            bytes[byteIndex] |= (1 << bitIndex);
        }
    }

    // Escribir los bytes en el archivo
    archivo.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

    // Guardar el árbol de Huffman
    guardarArbol(archivo, raiz);

    archivo.close();

    compararSizeArchivo(nombreArchivo + ".txt", nombreArchivo + ".bin");
}



void Huffman::guardarArbol(ofstream& archivo, Nodo* nodo) { //solo sirve para guardar la estructura del arbol en el archivo binario
    if (nodo==nullptr) {
        archivo.put('#'); //se usa '#' en lugar de '\0' para evitar problemas al leer el archivo ya que se puede confundir a leer el archivo
        //esto se pone cuando se encuentra un caracter leaf y obviamente los nodos hijos son nulos
        return;
    }
    if (nodo->isLeaf()) {
        archivo.put(nodo->getCh()); //guarda el char
    }
    else {
        archivo.put('*'); //guarda el nodo interno q apunta a los demas caracteres o otros nodos internos hasta encontrar una hoja
    }
    guardarArbol(archivo, nodo->getLeft());
    guardarArbol(archivo, nodo->getRight());
}
void Huffman::cargarTexto(string nombreArchivoTexto) {
    ifstream archivo(nombreArchivoTexto);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        return;
    }

    //verifica si el archivo esta vacio
    archivo.seekg(0, ios::end); //seekg mueve el cursor hacia el final del archivo
    if (archivo.tellg() == 0) { //si el cursor aun se encuentra en el inicio (posicion 0) significa que el archivo esta vacio
        cerr << "Error: El archivo de texto está vacio" << endl;
        archivo.close();
        return;
    }
    archivo.seekg(0, ios::beg);

    stringstream buffer;//para almacenar el texto del archivo
    buffer << archivo.rdbuf();//almacena el texto del archivo en el buffer

    //directamente llama a crear el arbol con el string obtenido del archivo de texto
    crearHuffmanTree(buffer.str(), nombreArchivoTexto);//convierte el buffer a string para poder crear el arbol con el contenido del texto
    archivo.close();
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

//metodo original:
/*
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
    cout << "\nLa cadena decodificada es:\n";
    while (indice < (int)codigoBin.size() - 2) {//como empezamos en -1 el indice hacemos un -2 en el .size
        decode(raiz, indice, codigoBin);
    }
}
*/

//entiendo que hace pero ocupo entenderlo bien porque esto es de chatgpt:
void Huffman::cargarCodificado(string nombreArchivoBinario) {
    ifstream archivo(nombreArchivoBinario, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    // Leer el tamaño en bits que se guardó en el archivo
    size_t tamanioBits;
    archivo.read(reinterpret_cast<char*>(&tamanioBits), sizeof(tamanioBits));

    // Calcular cuántos bytes necesitamos para almacenar estos bits
    size_t tamanioBytes = (tamanioBits + 7) / 8; // Redondear hacia arriba para incluir cualquier byte incompleto

    vector<uint8_t> bytes(tamanioBytes);
    archivo.read(reinterpret_cast<char*>(bytes.data()), tamanioBytes);

    string codigoBin = "";
    for (size_t i = 0; i < bytes.size(); ++i) {
        for (size_t bitIndex = 0; bitIndex < 8; ++bitIndex) {
            if (bytes[i] & (1 << (7 - bitIndex))) {
                codigoBin += '1';
            }
            else {
                codigoBin += '0';
            }
        }
    }

    // Ahora tenemos más bits de los necesarios, por lo que recortamos el código binario
    codigoBin = codigoBin.substr(0, tamanioBits);

    cout << endl << "codigoBin: " << codigoBin << endl;

    Nodo* raiz = reconstruirArbol(archivo);
    archivo.close();

    if (!raiz) {
        cerr << "Error al reconstruir el arbol de Huffman." << endl;
        return;
    }

    int indice = -1;
    cout << "\nLa cadena decodificada es:\n";
    while (indice < (int)codigoBin.size() - 2) {
        decode(raiz, indice, codigoBin);
    }
}



void Huffman::compararSizeArchivo(string textFile, string binFile) {
    ifstream archivoTxt(textFile, ios::ate);//ios::ate es para q se vaya directamente al final del archivo
    if (!archivoTxt.is_open()) {
        return;
    }
    ifstream archivoBin(binFile, ios::binary | ios::ate);
    if (!archivoBin.is_open()) {
		return;
	}

    streamsize sizeTxt = archivoTxt.tellg();//tellg() es para obtener el tamano del archivo y streamsize es para almacenar el tamano del archivo en bytes
    streamsize sizeBin = archivoBin.tellg();//

    //convertir los streamsize a int
    int intTxt = static_cast<int>(sizeTxt);
    int intBin = static_cast<int>(sizeBin);

    //convertimos a double todo porque si no se redondea a 0 en algunos casos
    double radio = ((double)intBin / (double)intTxt) * 100;

    cout << "\n\nEl texto original tiene un tamano de " << intTxt << " bytes";
    cout << endl << "El archivo binario tiene un tamano de " << intBin << " bytes";

    cout << endl << "El radio de compresion es de " << fixed << setprecision(2) << radio << "%";
}
