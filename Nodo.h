#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
class Nodo
{
private:
    char ch;
    int freq;
    Nodo* left;
    Nodo* right;
public:
    Nodo();
    Nodo(char ch, int freq);
    Nodo(char ch, int freq, Nodo* left, Nodo* right);
    ~Nodo();

    char getCh() const;
    int getFreq() const;
    Nodo* getLeft() const;
    Nodo* getRight() const;

    void setCh(char ch);
    void setFreq(int freq);
    void setLeft(Nodo* left);
    void setRight(Nodo* right);

    bool isLeaf();

};

