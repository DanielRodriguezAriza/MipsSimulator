#include "Mips.h"

MipsSimulator::MipsSimulator()
:scanner{}, parser{}, vm{}
{}

MipsSimulator::~MipsSimulator()
{}

void MipsSimulator::loadSource(std::string const &src)
{
	this->scanner.loadSource(src);
	this->scanner.scanSource();
	this->parser.setTokens(this->scanner.getTokens());
	this->parser.parseTokens();
}

void MipsSimulator::runProgram()
{
	if(this->scanner.getTokens().size() <= 0)
	{
		return;
	}
	
	this->vm.RunProgram(this->parser.getInstructions());
}

std::vector<Token> MipsSimulator::getTokens()
{
	return this->scanner.getTokens();
}

std::vector<Instruction> MipsSimulator::getInstructions()
{
	return this->parser.getInstructions();
}
