#include "String.h"

bool isWhiteSpace(char c) {return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v';}
bool isDigit(char c) {return c>='0' && c<='9';}
bool isAlpha(char c) {return (c>='A' && c<='Z') || (c>='a' && c<='z') || c=='_';}
bool isAlphaNumeric(char c) {return isDigit(c) || isAlpha(c);}

char toLower(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return c - ('A'-'a');
	}
	return c;
}

std::string toLower(std::string const &input)
{
	std::string ans = input;
	for(size_t i = 0; i < input.size(); ++i)
	{
		ans[i] = toLower(ans[i]);
	}
	return ans;
}