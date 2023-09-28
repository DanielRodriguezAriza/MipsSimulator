#ifndef DRA_SCANNER_H
#define DRA_SCANNER_H

#include "Tokens.h"
#include <string>
#include <vector>

#include "String.h"

class Scanner
{
public:
	
	Scanner();
	~Scanner();
	
	void loadSource(std::string const &filepath);
	void scanSource();
	
	inline char getCurrent() const {return this->source[current_index];}
	inline char getNext() const {return this->source[current_index+1];}
	
	inline bool isAtEnd() const {return current_index >= source.size();}
	inline bool hasNext() const {return current_index + 1 < source.size();}
	
	inline void incrementIndex() {++this->current_index;}
	
	inline void addToken(TokenType t, std::string l) {this->tokens.push_back(Token{t,l});}
	inline void clearTokens() {this->tokens.clear();}
	
	inline void clearSource() {this->source_file = ""; this->source = "";}
	
	void scanNumber();
	void scanAlphanumericToken();
	void scanComment();
	
	inline std::vector<Token> getTokens() const {return this->tokens;} //this sucks dick and feels dirty because it performs a copy of the vector...
	
	//TokenType getTokenType(std::string const &str);
	
	TokenType matchInstruction(std::string const &str);
	
private:
	
	std::string source,source_file;
	std::vector<Token> tokens;
	size_t current_index;
	//std::vector<Instruction> instructions; //is this a good idea?
};

#endif