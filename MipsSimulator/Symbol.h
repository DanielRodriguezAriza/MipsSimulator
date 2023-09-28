#ifndef DRA_SYMBOL_H
#define DRA_SYMBOL_H

#include "Tokens.h"
#include <vector>

struct Symbol
{
	std::string identifier;
	size_t value;
};

class SymbolTable
{
public:

	SymbolTable();
	~SymbolTable();
	
	size_t getSymbolIndex(std::string str);
	
	inline Symbol getSymbol(size_t index) {return this->symbols[index];}
	
	inline void addSymbol(std::string str, size_t value) {this->symbols.push_back(Symbol{str,value});}

private:

	std::vector<Symbol> symbols;
	
};

#endif
