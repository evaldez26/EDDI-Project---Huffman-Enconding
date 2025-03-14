#include "Nodo.h"

Nodo::Nodo()
{
}

Nodo::Nodo(char ch, int freq)
{
	this->ch = ch;
	this->freq = freq;
	this->left = nullptr;
	this->right = nullptr;
}

Nodo::Nodo(char ch, int freq, Nodo* left, Nodo* right)
{
	this->ch = ch;
	this->freq = freq;
	this->left = left;
	this->right = right;
}

Nodo::~Nodo()
{

}

char Nodo::getCh() const 
{ 
	return this->ch; 
}

int Nodo::getFreq() const 
{ 
	return this->freq;
}

Nodo* Nodo::getLeft() const 
{ 
	return this->left;
}

Nodo* Nodo::getRight() const 
{ 
	return this->right;
}

void Nodo::setCh(char ch) 
{ 
	this->ch = ch;
}

void Nodo::setFreq(int freq) 
{
	this->freq = freq;
}

void Nodo::setLeft(Nodo* left) 
{ 
	this->left = left; 
}

void Nodo::setRight(Nodo* right) 
{
	this->right = right;
}

bool Nodo::isLeaf()
{
	if (!this->getLeft() && !this->getRight()) {
		return true;
	}
	return false;
}
