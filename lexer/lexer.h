#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"
using namespace std;

// Maximum identifiers we can store in the symbol table
const int MAX_SYMBOLS = 100;

// Maximum allowed characters in a valid identifier
const int MAX_IDENT_LENGTH = 20;

// Helper functions for reserved words and symbol table
bool isKeyword(string word);
int addOrFindSymbol(string name);
void saveSymbolTable(string fileName);
int getSymbolCount();

// Token scanner functions for your task
Token readIdentifierOrKeyword(string code, int &pos, int &line, int &col);
Token readNumber(string code, int &pos, int &line, int &col);
Token readString(string code, int &pos, int &line, int &col);
Token readChar(string code, int &pos, int &line, int &col);

#endif