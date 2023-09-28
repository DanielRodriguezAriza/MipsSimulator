#include "Parser.h"
#include "String.h"

#include <stdio.h>
#include <iostream>

#include "VirtualMachine.h"

Parser::Parser()
:tokens{}, instructions{}, current_index{0}
{}

Parser::~Parser()
{}

void Parser::parseTokens()
{
	
	this->preprocess();
	
	this->instructions.clear();
	while(!isAtEnd())
	{
		switch(getCurrent().type)
		{
		case TOKEN_IDENTIFIER:
			
			if(getNext().type == TOKEN_COLON && table.getSymbolIndex(getCurrent().lexeme) != -1)
			{
				incrementIndex();
			}
			
			break;
		case TOKEN_COMMENT:
			//ignore the comment: no code is generated.
			break;
		case TOKEN_DOT:
			//TODO: Generate section. Maybe this has to be done in a preprocessor step of sorts.
			break;
		case TOKEN_NEWLINE:
		case TOKEN_EOF:
			//do nothing, eof and newlines are not relevant in this case.
			break;
		default:
			scanInstruction();
			//printf("Error: Unexpected token detected during bytecode generation! (%s)\n",TokenNames[getCurrent().type]);
			break;
		}
		
		incrementIndex();
	}
}

void Parser::preprocess() //maybe reimplement as preprocessor class that handles macro code generation and tag replacement.
{
	while(!isAtEnd())
	{
		switch(getCurrent().type)
		{
			case TOKEN_DOT:
				//select corresponding section or create a macro
				break;
			case TOKEN_IDENTIFIER:
				scanIdentifier();
				break;
			case TOKEN_NUMBER:
				this->tokens[current_index].value = std::stoi(getCurrent().lexeme);
				break;
		}
		incrementIndex();
	}
	
	this->current_index = 0; //return to start of the token list to actually start parsing the code after tags have been replaced.
}

void Parser::scanIdentifier()
{
	if(getNext().type == TOKEN_COLON)
	{
		this->table.addSymbol(getCurrent().lexeme, instructions.size());
		std::cout<<"Added the  tag : " << getCurrent().lexeme<<"\n";
		incrementIndex();
	}
	else
	if(getRegisterFromText(getCurrent().lexeme) == -1)
	{
		//emit a new number token with the symbol value that contains its corresponding address and stuff.
		
		std::cout<<"performing lookup for tag: " << getCurrent().lexeme << "\n";
		size_t idx = table.getSymbolIndex(getCurrent().lexeme);
		if(idx != -1)
		{
			this->tokens[current_index].type = TOKEN_NUMBER;
			this->tokens[current_index].value = table.getSymbol(idx).value;
		}
		else
		{
			std::cout<<"Error: unknown tag detected: " << getCurrent().lexeme << "\n";
			exit(-1);
		}
	}
}

void Parser::scanLi()
{
	//li $v0, 69
	if(
	getAtRelative(1).type == TOKEN_DOLLAR &&
	getAtRelative(2).type == TOKEN_IDENTIFIER &&
	getAtRelative(3).type == TOKEN_COMMA &&
	getAtRelative(4).type == TOKEN_NUMBER &&
	isEndOfInstruction(getAtRelative(5))
	)
	{
		addInstruction(LI,getRegisterFromText(getAtRelative(2).lexeme),getAtRelative(4).value);
		incrementIndexBy(5);
	}
	
}

void Parser::scanAdd()
{
	//add $t0, $t1, $t2
	if(
	getAtRelative(1).type == TOKEN_DOLLAR &&
	getAtRelative(2).type == TOKEN_IDENTIFIER &&
	getAtRelative(3).type == TOKEN_COMMA &&
	getAtRelative(4).type == TOKEN_DOLLAR &&
	getAtRelative(5).type == TOKEN_IDENTIFIER &&
	getAtRelative(6).type == TOKEN_COMMA &&
	getAtRelative(7).type == TOKEN_DOLLAR &&
	getAtRelative(8).type == TOKEN_IDENTIFIER &&
	isEndOfInstruction(getAtRelative(9))
	)
	{
		int r1 = getRegisterFromText(getAtRelative(2).lexeme);
		int r2 = getRegisterFromText(getAtRelative(5).lexeme);
		int r3 = getRegisterFromText(getAtRelative(8).lexeme);
		addInstruction(ADD,r1,r2,r3);
		incrementIndexBy(9);
	}
}

void Parser::scanAddi()
{
	//addi $t0, $t1, 100
	if(
	getAtRelative(1).type == TOKEN_DOLLAR &&
	getAtRelative(2).type == TOKEN_IDENTIFIER &&
	getAtRelative(3).type == TOKEN_COMMA &&
	getAtRelative(4).type == TOKEN_DOLLAR &&
	getAtRelative(5).type == TOKEN_IDENTIFIER &&
	getAtRelative(6).type == TOKEN_COMMA &&
	getAtRelative(7).type == TOKEN_NUMBER &&
	isEndOfInstruction(getAtRelative(8))
	)
	{
		int r1 = getRegisterFromText(getAtRelative(2).lexeme);
		int r2 = getRegisterFromText(getAtRelative(5).lexeme);
		int value = getAtRelative(7).value;
		addInstruction(ADDI,r1,r2,value);
		incrementIndexBy(8);
	}
}

void Parser::scanSyscall()
{
	//syscall
	if(
	(getAtRelative(1).type == TOKEN_EOF || getAtRelative(1).type == TOKEN_NEWLINE)
	)
	{
		addInstruction(SYSCALL);
		incrementIndexBy(1);
	}
}

void Parser::scanMove()
{
	//move $t0, $t1
	if(
	getAtRelative(1).type == TOKEN_DOLLAR &&
	getAtRelative(2).type == TOKEN_IDENTIFIER &&
	getAtRelative(3).type == TOKEN_COMMA &&
	getAtRelative(4).type == TOKEN_DOLLAR &&
	getAtRelative(5).type == TOKEN_IDENTIFIER &&
	isEndOfInstruction(getAtRelative(6))
	)
	{
		int r1 = getRegisterFromText(getAtRelative(2).lexeme);
		int r2 = getRegisterFromText(getAtRelative(5).lexeme);
		addInstruction(MOVE,r1,r2);
		incrementIndexBy(6);
	}
}

void Parser::scanJump()
{
	//j value	
	if(
	getAtRelative(1).type == TOKEN_NUMBER &&
	isEndOfInstruction(getAtRelative(2))
	)
	{
		std::cout<<"is not valid!\n";
		int value = getAtRelative(1).value;
		addInstruction(JUMP,value);
		incrementIndexBy(2);
	}
}

void Parser::scanJumpReg()
{
	//jr $ra
	if(
	getAtRelative(1).type == TOKEN_DOLLAR &&
	getAtRelative(2).type == TOKEN_IDENTIFIER &&
	isEndOfInstruction(getAtRelative(3))
	)
	{
		int r1 = getRegisterFromText(getAtRelative(2).lexeme);
		addInstruction(JR,r1);
		incrementIndexBy(3);
	}
}

//important notes about mul vs mult: mul does not modify hi and lo, tho older MIPS CPUs and simulators like mars and spim do so... mul seems to be for when you care about doing a simple 32 bit multiplication. Mult is for when you either want to know what the 64 bit result is ("overflown multiplication")
void Parser::scanMul()
{
	//mul $t0, $t1, $t2
	if(
	getAtRelative(1).type == TOKEN_DOLLAR &&
	getAtRelative(2).type == TOKEN_IDENTIFIER &&
	getAtRelative(3).type == TOKEN_COMMA &&
	getAtRelative(4).type == TOKEN_DOLLAR &&
	getAtRelative(5).type == TOKEN_IDENTIFIER &&
	getAtRelative(6).type == TOKEN_COMMA &&
	getAtRelative(7).type == TOKEN_DOLLAR &&
	getAtRelative(8).type == TOKEN_IDENTIFIER &&
	isEndOfInstruction(getAtRelative(9))
	)
	{
		int r1 = getRegisterFromText(getAtRelative(2).lexeme);
		int r2 = getRegisterFromText(getAtRelative(5).lexeme);
		int r3 = getRegisterFromText(getAtRelative(8).lexeme);
		addInstruction(MUL,r1,r2,r3);
		incrementIndexBy(9);
	}
}

void Parser::scanNop()
{
	//nop
	if(
	isEndOfInstruction(getAtRelative(1))
	)
	{
		addInstruction(NOP);
		incrementIndexBy(1);
	}
}

//TODO: maybe could use this as a bool to get if the instructions are valid or have some kind of pointer to a bool that indicates if the structure is valid so that you can handle errors externally.
/*
void Interpreter::scanInstructionWithStructure(InstructionType type, bool has_r1, bool has_r2, bool has_r3)
{...}
*/


void Parser::scanInstruction()
{
	TokenType type = getCurrent().type;
	
	switch(type)
	{ //modify the scan functions to scan for 0, 1, 2 and 3 argument instructions or custom made ones for macros etc... instead of having an individual but almost identical scan function for each and every single instruction. That means you need to add some "scan R type instruction", "scan J type instruction", etc...
	default:
		std::cout << "Error: Unknown instruction detected : " << getCurrent() << "\n";
		break;
	case TOKEN_NOP:
		scanNop();
		break;
	case TOKEN_LI:
		scanLi();
		break;
	case TOKEN_ADD:
		scanAdd();
		break;
	case TOKEN_ADDI:
		scanAddi();
		break;
	case TOKEN_ADDU:
		//scanAddu();
		break;
	case TOKEN_ADDIU:
		//scanAddiu();
		break;
	case TOKEN_SUB:
		//scanSub();
		break;
	case TOKEN_SUBI:
		//scanSubi();
		break;
	case TOKEN_SUBU:
		//scanSubu();
		break;
	case TOKEN_SUBIU:
		//scanSubiu();
		break;
	case TOKEN_MUL:
		scanMul();
		break;
	case TOKEN_MULT:
		//scanMult();
		break;
	case TOKEN_DIV:
		//scanDiv();
		break;
	case TOKEN_MOVE:
		scanMove();
		break;
	case TOKEN_SYSCALL:
		scanSyscall();
		break;
	case TOKEN_JUMP:
		scanJump();
		break;
	case TOKEN_JR:
		scanJumpReg();
		break;
	}
}

//this one is useless, i should delete it when i cleanup
InstructionType Parser::getInstructionType(std::string const &str)
{
	InstructionType ans = UNKNOWN_INSTRUCTION;
	std::string lexeme = toLower(str);
	
	if(lexeme == "nop")
	{
		ans = NOP;
	}
	else
	if(lexeme == "li")
	{
		ans = LI;
	}
	else
	if(lexeme == "add")
	{
		ans = ADD;
	}
	else
	if(lexeme == "addi")
	{
		ans = ADDI;
	}
	else
	if(lexeme == "addu")
	{
		ans = ADDU;
	}
	else
	if(lexeme == "addiu")
	{
		ans = ADDIU;
	}
	else
	if(lexeme == "sub")
	{
		ans = SUB;
	}
	else
	if(lexeme == "subi")
	{
		ans = SUBI;
	}
	else
	if(lexeme == "subu")
	{
		ans = SUBU;
	}
	else
	if(lexeme == "subiu")
	{
		ans = SUBIU;
	}
	else
	if(lexeme == "mul")
	{
		ans = MUL;
	}
	else
	if(lexeme == "muli")
	{
		ans = MULI;
	}
	else
	if(lexeme == "div")
	{
		ans = DIV;
	}
	else
	if(lexeme == "divi")
	{
		ans = DIVI;
	}
	else
	if(lexeme == "move")
	{
		ans = MOVE;
	}
	else
	if(lexeme == "syscall")
	{
		ans = SYSCALL;
	}
	else
	if(lexeme == "j" || lexeme == "jump" || lexeme == "jmp")
	{
		ans = JUMP;
	}
	else
	if(lexeme == "jr")
	{
		ans = JR;
	}
	
	return ans;
}
