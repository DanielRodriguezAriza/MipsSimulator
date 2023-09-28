#ifndef DRA_STRING_H
#define DRA_STRING_H

#include <string>

bool isWhiteSpace(char c);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);

char toLower(char c);
std::string toLower(std::string const &input);

#endif
