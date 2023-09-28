#ifndef DRA_MIPS_PARSER_H
#define DRA_MIPS_PARSER_H

#include "Tokens.h"
#include <vector>
#include "VirtualMachine.h"
#include "Symbol.h"

class Parser
{
public:
	
	Parser();
	~Parser();
	
	void parseTokens();
	
	inline bool isAtEnd() const {return current_index >= tokens.size();}
	inline void incrementIndex() {++this->current_index;}
	inline void incrementIndexBy(size_t n) {this->current_index+=n;}
	
	inline bool indexIsValid(size_t index) const {return index < tokens.size();}
	
	inline Token getCurrent() const {return this->tokens[current_index];}
	inline Token getNext() const {return this->tokens[current_index+1];}
	inline Token getAt(size_t index) const {return this->tokens[index];}
	inline Token getAtRelative(size_t index) const {return indexIsValid(current_index + index) ? this->tokens[current_index + index] : Token{TOKEN_EOF,"__out_of_bounds__"};}
	
	inline void setTokens(std::vector<Token> const &newTokens) {this->tokens = newTokens;}
	
	inline std::vector<Instruction> getInstructions() const {return this->instructions;} //problem: repetitive copies when returning vector... implement as ptr or ref later
	inline std::vector<Token> getTokens() const {return this->tokens;} //this one was added for debugging purposes, but it should be removed for encapsulation in the future. Or maybe i can just ignore OOP and do things in a more sensible way...
	
	InstructionType getInstructionType(std::string const &str);
	
	inline void addInstruction(InstructionType type, unsigned int r1 = 0, unsigned int r2 = 0, unsigned int r3 = 0) {this->instructions.push_back(Instruction{type,r1,r2,r3});}
	
	void scanInstruction();
	void scanIdentifier();
	
	void preprocess();
	
	void scanNop();
	
	void scanLi();
	void scanAdd();
	void scanAddi();
	
	void scanMul();
	
	void scanMove();
	
	void scanJump();
	void scanJumpReg();
	
	void scanSyscall();
	
	inline bool isEndOfInstruction(Token token) const {return (token.type == TOKEN_EOF || token.type == TOKEN_NEWLINE || token.type == TOKEN_COMMENT);}
	
private:
	size_t current_index;
	std::vector<Token> tokens;
	std::vector<Instruction> instructions;
	
	SymbolTable table;
	
};

#endif
