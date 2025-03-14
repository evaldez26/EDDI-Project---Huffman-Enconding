#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include "Huffman.h"
using namespace std;
int main()
{
    Huffman huff;
    string text;
    cout << "Ingrese el texto: ";
    getline(cin, text);
    huff.buildHuffmanTree(text);
}
