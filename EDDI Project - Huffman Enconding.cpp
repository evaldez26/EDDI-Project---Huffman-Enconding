#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
unordered_map<char, int> calcularFrecuencia(const string texto) {
    unordered_map<char, int>frecuencias;//unordered_map es un hashmap que no ordena las claves

    for (char c : texto) {//Recorre el texo y entra cada vez que encuentre el mismo caracter
        frecuencias[c]++;
    }
    return frecuencias;
 }
int main()
{
    string texto;
    cout << "Ingresa un texto: ";
    getline(cin, texto);
    unordered_map<char, int> frecuencias = calcularFrecuencia(texto);
    for (const auto elemento : frecuencias) {
        cout << elemento.first << ": " << elemento.second<<endl;
    }
    return 0;
}
