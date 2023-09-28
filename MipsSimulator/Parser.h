#ifndef DRA_MIPS_PARSER_H
#define DRA_MIPS_PARSER_H

#include "Tokens.h"
#include <vector>
#include "VirtualMachine.h"
#include "Symbol.h"

//im calling this the "interpreter" but its more like the "compiler" because it will translate whatever token set the scanner generates into the "machine code" that my virtual machine will later interpret, so i need to restructure this stuff... basically this thing is going to be the bytecode generator... weird structuring for now but whatever.
//btw, another thing, for now the token list generated is just a list of integers that tells me what type of token it has generated, but it does not tell me what text the token contains. For most stuff thats ok, for example i could have a token for every single register, for symbols i dont care about the text, etc... but when it comes to identifiers, i will NEED to store the text somewhere. I need to modify the Token class to contain a string, as well as having a different "addToken" function that allows me to add a token with a symbol optionally.
//in short: this thing is the one that should be generating the bytecode... not the one interpreting it... that is the purpose of the VirtualMachine. Which obviously means i need to rename some stuff for it to make sense.

//the "good" thing about having this as an interpreter is that now i can get the token vector from the scanner and use it to detect errors at this point of the program. In the scanner i can detect unknown symbols and be done with it. Here i can detect orders of symbols that dont make sense, as well as construct the proper bytecode that will be executed later (hence why this makes more sense to be called a bytecode compiler rather than interpreter but whatever). And then the vm is the one in charge of executing the code. If any exceptions or errors or weird stuff happens, i can then analyse it later and its a process completely independent from this. Now, the question is: Does the VM have an interpreter and an scanner or does the interpreter contain the scanner and then the vm contains the interpreter.... etc etc... or what i said in the first paragraph here.... its all stuff i'll have to figure out later.

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
	
	inline std::vector<Instruction> getInstructions() const {return this->instructions;} //again same problem, repetitive copies when returning vector...
	inline std::vector<Token> getTokens() const {return this->tokens;} //this one was added for debugging purposes, but it should be removed for encapsulation in the future.
	
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