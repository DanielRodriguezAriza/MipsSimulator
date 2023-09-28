#ifndef DRA_TOKENS_H
#define DRA_TOKENS_H

#include <string>
#include <ostream>

/*
	TOKEN_DOT is for section declaration.
	TOKEN_DOLLAR is for registers.
	TOKEN_PERCENT is for macro args.
*/

enum TokenType
{
	TOKEN_UNKNOWN = 0,
	TOKEN_NEWLINE, //newline = statement end.
	TOKEN_NOP,
	TOKEN_LI,TOKEN_MOVE,TOKEN_SYSCALL,
	TOKEN_COMMA,TOKEN_DOT,
	TOKEN_DOLLAR,TOKEN_PERCENT,
	TOKEN_IDENTIFIER,TOKEN_NUMBER,
	TOKEN_L_PAREN,TOKEN_R_PAREN,TOKEN_SEMICOLON,TOKEN_COLON,
	TOKEN_ADD,TOKEN_ADDI,TOKEN_ADDU,TOKEN_ADDIU,
	TOKEN_SUB,TOKEN_SUBI,TOKEN_SUBU,TOKEN_SUBIU,
	TOKEN_MUL,TOKEN_MULT,
	TOKEN_DIV,
	TOKEN_JUMP,TOKEN_JR,TOKEN_JAL,
	TOKEN_EOF,
	TOKEN_COMMENT,
	NUM_TOKENS,
};

char const *const TokenNames[] = {
	"TOKEN_UNKNOWN",
	"TOKEN_NEWLINE",
	"TOKEN_NOP",
	"TOKEN_LI","TOKEN_MOVE","TOKEN_SYSCALL",
	"TOKEN_COMMA","TOKEN_DOT",
	"TOKEN_DOLLAR","TOKEN_PERCENT",
	"TOKEN_IDENTIFIER","TOKEN_NUMBER",
	"TOKEN_L_PAREN","TOKEN_R_PAREN","TOKEN_SEMICOLON","TOKEN_COLON",
	"TOKEN_ADD","TOKEN_ADDI","TOKEN_ADDU","TOKEN_ADDIU",
	"TOKEN_SUB","TOKEN_SUBI","TOKEN_SUBU","TOKEN_SUBIU",
	"TOKEN_MUL","TOKEN_MULT",
	"TOKEN_DIV",
	"TOKEN_JUMP","TOKEN_JR","TOKEN_JAL",
	"TOKEN_EOF",
	"TOKEN_COMMENT",
	"NUM_TOKENS",
};

struct Token
{
	TokenType type;
	std::string lexeme;
	int value;
	
	friend std::ostream &operator<<(std::ostream &os, Token const &token)
	{
		os<<"Token: { Type = " << TokenNames[token.type] << " (" << token.type << "), lexeme = \"" << token.lexeme << "\", value = \"" << token.value << "\" }";
		return os;
	}
};

#endif
