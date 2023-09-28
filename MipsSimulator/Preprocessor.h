#ifndef DRA_PREPROCESSOR_H
#define DRA_PREPROCESSOR_H

#include <vector>
#include "Tokens.h"

//TODO:
//this class generates a symbol table with symbols that contain the tag name and tag value.
//example:
/*
	loop:
		addi $t0, $t1, 100
		bne $t0, $zero, loop
*/
//the symbol "loop" has the value of the address of the instruction that goes after it ("addi $t0, $t1, 100").

//this class will have to look for values located only in the .data segment.
//any tags created in the .text segment will simply be identifiers, thus their value is added during "compilation" into bytecode.


class Preprocessor
{
public:
	
	
	
private:

	size_t current_index;
	std::vector<Token> tokens;
	std::vector<Instruction> instructions;

};

#endif
