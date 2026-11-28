
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"

using namespace std;

struct Symbol
{
    string name;
    string type;
    int index;
    int address;
    int size;
};

// Maximum identifiers we can store in the symbol table
const int MAX_SYMBOLS = 100;

// Maximum allowed characters in a valid identifier
const int MAX_IDENT_LENGTH = 20;

// Helper functions for reserved words and symbol table
bool isKeyword(string word);
int addOrFindSymbol(string name);
void saveSymbolTable(string fileName);
int getSymbolCount();

// Token scanner functions
Token readIdentifierOrKeyword(string code, int &pos, int &line, int &col);
Token readNumber(string code, int &pos, int &line, int &col);
Token readString(string code, int &pos, int &line, int &col);
Token readChar(string code, int &pos, int &line, int &col);

// Famia's delimiter and comment handling functions
bool isCommentStart(const string& code, int pos);
void skipComment(const string& code, int &pos, int &line, int &col);
bool isDelimiterAt(const string& code, int pos);
Token readDelimiter(const string& code, int &pos, int &line, int &col);

#endif
