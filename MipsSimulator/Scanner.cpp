#include "Scanner.h"

#include <fstream>

Scanner::Scanner()
:source{},source_file{},tokens{},current_index{0}
{}

Scanner::~Scanner()
{}

void Scanner::loadSource(std::string const &filepath)
{
	this->clearSource();
	this->clearTokens();
	
	this->source_file = filepath;
	
	std::ifstream file;
	file.open(filepath);
	
	if(file.good())
	{
		std::string current;
		while(std::getline(file,current))
		{
			this->source += current + "\n";
		}
	}
	file.close();
}

void Scanner::scanSource()
{
	while(!isAtEnd())
	{
		switch(getCurrent())
		{
		case ',':
			addToken(TOKEN_COMMA,",");
			break;
		case '.':
			addToken(TOKEN_DOT,".");
			break;
		case '(':
			addToken(TOKEN_L_PAREN,"(");
			break;
		case ')':
			addToken(TOKEN_R_PAREN,")");
			break;
		case ';':
			addToken(TOKEN_SEMICOLON,";");
			break;
		case ':':
			addToken(TOKEN_COLON,":");
			break;
		case '$':
			addToken(TOKEN_DOLLAR,"$");
			//this is done like this because we can write registers as $t0, $v0, etc... as well as using numbers like this: $0, $1, $2, ... $30, etc.
			//so it is important to have a token that indicates that whatever comes after it is going to be either a register identifier or a register number, but that whatever it is, it has to be interpreter as a register. This will help with making this easier, faster, and also catching errors sooner.
			break;
		case '%':
			addToken(TOKEN_PERCENT,"%");
			break;
		case '#':
			scanComment();
			break;
		case '\n':
			addToken(TOKEN_NEWLINE,"__NEWLINE__");
		default:
			if(isAlpha(getCurrent()))
			{
				//scanIdentifier();
				scanAlphanumericToken();
			}
			else
			if(isDigit(getCurrent()))
			{
				scanNumber();
			}
			else
			if(!isWhiteSpace(getCurrent()))
			{
				//we have an error due to an unknown symbol.
				std::string lexeme = "";
				lexeme += getCurrent();
				addToken(TOKEN_UNKNOWN,lexeme);
			}
			break;
		}
		incrementIndex();
	}
	addToken(TOKEN_EOF,"__EOF__");
}

void Scanner::scanNumber()
{
	std::string lexeme;
	size_t start_index = current_index;
	while(!isAtEnd() && (getNext() == '.' || isDigit(getNext())))
	{
		incrementIndex();
	}
	lexeme = source.substr(start_index, current_index - start_index + 1);
	addToken(TOKEN_NUMBER,lexeme);
}

void Scanner::scanAlphanumericToken()
{
	std::string lexeme;
	TokenType type;
	size_t start_index = current_index;
	while(!isAtEnd() && isAlphaNumeric(getNext()))
	{
		incrementIndex();
	}
	lexeme = source.substr(start_index, current_index - start_index + 1);
	
	type = matchInstruction(lexeme);
	
	addToken(type,lexeme);
}

void Scanner::scanComment()
{
	std::string lexeme;
	size_t start_index = current_index;
	while(!isAtEnd() && (getNext() != '\n'))
	{		
		incrementIndex();
	}
	lexeme = source.substr(start_index, current_index - start_index + 1);
	addToken(TOKEN_COMMENT,lexeme);
}

TokenType Scanner::matchInstruction(std::string const &str)
{
	std::string lexeme = toLower(str);
	TokenType ans = TOKEN_IDENTIFIER;
	
	if(lexeme == "nop")
	{
		ans = TOKEN_NOP;
	}
	else
	if(lexeme == "li")
	{
		ans = TOKEN_LI;
	}
	else
	if(lexeme == "add")
	{
		ans = TOKEN_ADD;
	}
	else
	if(lexeme == "addi")
	{
		ans = TOKEN_ADDI;
	}
	else
	if(lexeme == "addu")
	{
		ans = TOKEN_ADDU;
	}
	else
	if(lexeme == "addiu")
	{
		ans = TOKEN_ADDIU;
	}
	else
	if(lexeme == "sub")
	{
		ans = TOKEN_SUB;
	}
	else
	if(lexeme == "subi")
	{
		ans = TOKEN_SUBI;
	}
	else
	if(lexeme == "subu")
	{
		ans = TOKEN_SUBU;
	}
	else
	if(lexeme == "subiu")
	{
		ans = TOKEN_SUBIU;
	}
	else
	if(lexeme == "mul")
	{
		ans = TOKEN_MUL;
	}
	else
	if(lexeme == "mult")
	{
		ans = TOKEN_MULT;
	}
	else
	if(lexeme == "div")
	{
		ans = TOKEN_DIV;
	}
	else
	if(lexeme == "move")
	{
		ans = TOKEN_MOVE;
	}
	else
	if(lexeme == "syscall")
	{
		ans = TOKEN_SYSCALL;
	}
	else
	if(lexeme == "j" || lexeme == "jump" || lexeme == "jmp")
	{
		ans = TOKEN_JUMP;
	}
	else
	if(lexeme == "jr")
	{
		ans = TOKEN_JR;
	}
	else
	if(lexeme == "jal")
	{
		ans = TOKEN_JAL;
	}
	
	return ans;
}

/* original function.
void Scanner::scanIdentifier()
{
	std::string lexeme;
	size_t start_index = current_index;
	while(!isAtEnd() && isAlphaNumeric(getNext()))
	{
		incrementIndex();
	}
	lexeme = source.substr(start_index, current_index - start_index + 1);
	addToken(TOKEN_IDENTIFIER,lexeme);
}
*/