#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

// Holds one token found in the AERIS source code
struct Token {
    string type;
    string lexeme;
    int line;
    int column;
};

// Holds one unique identifier in our symbol table
struct Symbol {
    int index;
    string name;
    int address;
    string type;
    int size;
};

#endif