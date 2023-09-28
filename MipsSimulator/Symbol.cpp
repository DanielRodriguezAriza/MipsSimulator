#include "Symbol.h"

SymbolTable::SymbolTable()
:symbols{}
{}

SymbolTable::~SymbolTable()
{}

size_t SymbolTable::getSymbolIndex(std::string str)
{
	for(size_t i = 0; i < this->symbols.size(); ++i)
	{
		if(str == symbols[i].identifier)
		{
			return i;
		}
	}
	return -1;
}
