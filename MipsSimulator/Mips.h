#ifndef DRA_MIPS_H
#define DRA_MIPS_H

#include <string>
#include <ostream>

#include "VirtualMachine.h"
#include "Parser.h"
#include "Scanner.h"

class MipsSimulator
{
public:
	
	MipsSimulator();
	~MipsSimulator();
	
	void loadSource(std::string const &src);
	void runProgram();
	
	std::vector<Token> getTokens();
	std::vector<Instruction> getInstructions();
	
private:
	
	Scanner scanner;
	Parser parser;
	VirtualMachine vm;
	
};

#endif