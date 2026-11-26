#include "lexer.h"
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
int nextAddress = 1000;

// All keywords and special words in a simple string array
string keywords[] = {
    "period", "maincharacter",
    "sus", "lock",
    "sidequests", "spits", "deliver", "heroera",
    "clockit", "cooked", "aurafarming", "grindset", "dip", "skip",
    "spilltea", "lockin",
    "and", "or", "not",
    "true", "false",
    "whole", "decimal", "vibecheck", "letter", "text", "empty",
    "lowkey",
    "from", "to"
};

int totalKeywords = 30;

// Checks if a word is in the keywords list (case-sensitive)
bool isKeyword(string word) {
    for (int i = 0; i < totalKeywords; i++) {
        if (keywords[i] == word) {
            return true;
        }
    }
    return false;
}

// Finds an existing identifier or adds a new one to the symbol table
int addOrFindSymbol(string name) {
    // Check if identifier is already stored
    for (int i = 0; i < symbolCount; i++) {
        if (symbolTable[i].name == name) {
            return symbolTable[i].index;
        }
    }

    // Add new identifier if there is space in the array
    if (symbolCount < MAX_SYMBOLS) {
        symbolTable[symbolCount].index = symbolCount;
        symbolTable[symbolCount].name = name;
        symbolTable[symbolCount].address = nextAddress;
        symbolTable[symbolCount].type = "unknown";
        symbolTable[symbolCount].size = 4;

        nextAddress = nextAddress + 4;
        symbolCount++;
        return symbolTable[symbolCount - 1].index;
    }
    // If the symbolTable array is full, return -1 to indicate it could not be added
    return -1;
}

// Writes the symbol table array into a text file
void saveSymbolTable(string fileName) {
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cout << "Error: Could not open " << fileName << endl;
        return;
    }

    outFile << "SYMBOL TABLE\n";
    outFile << "INDEX\tNAME\t\tADDRESS\tTYPE\tSIZE\n";

    for (int i = 0; i < symbolCount; i++) {
        outFile << symbolTable[i].index << "\t"
                << symbolTable[i].name << "\t\t"
                << symbolTable[i].address << "\t"
                << symbolTable[i].type << "\t"
                << symbolTable[i].size << "\n";
    }

    outFile.close();
}

bool isEnglishLetter(char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        return true;
    }
    return false;
}

// Helper function to check if a character is a digit (0-9)
bool isDigitChar(char ch) {
    if (ch >= '0' && ch <= '9') {
        return true;
    }
    return false;
}

// Reads identifiers (starting with a-z) and keywords
Token readIdentifierOrKeyword(string code, int &pos, int &line, int &col) {
    int startLine = line;
    int startCol = col;
    string word = "";
    int totalChars = static_cast<int>(code.length());

    while (pos < totalChars) {
        char ch = code[pos];

        if (isEnglishLetter(ch) || isDigitChar(ch) || ch == '_') {
            word = word + ch;
            pos = pos + 1;
            col = col + 1;
        } else {
            // Stop when we hit space, operator, punctuation, or newline
            break;
        }
    }

    Token tok;
    tok.lexeme = word;
    tok.line = startLine;
    tok.column = startCol;

    // 1. Check boolean literals first
    if (word == "true" || word == "false") {
        tok.type = "BOOLEAN_LITERAL";
        return tok;
    }

    // 2. Check if the word is a reserved keyword in AERIS
    if (isKeyword(word)) {
        tok.type = "KEYWORD";
        return tok;
    }

    // 3. Check identifier length rule (maximum 20 characters -> L002)
    if ((int)word.length() > MAX_IDENT_LENGTH) {
        tok.type = "NO CAP BESTIE! INVALID_IDENTIFIER";
        return tok;
    }

    // 4. Otherwise it is a valid identifier: add to symbol table and return token
    addOrFindSymbol(word);
    tok.type = "IDENTIFIER";
    return tok;
}

// Reads whole numbers, decimal numbers, and detects malformed numbers
Token readNumber(string code, int &pos, int &line, int &col) {
    int startLine = line;
    int startCol = col;
    string numText = "";
    int dotCount = 0;
    bool malformed = false;

    while (pos < (int)code.length()) {
        char ch = code[pos];

        if (ch >= '0' && ch <= '9') {
            numText = numText + ch;
            pos++;
            col++;
        } else if (ch == '.') {
            dotCount++;
            numText = numText + ch;
            pos++;
            col++;

            // There must be a digit right after '.'
            if (pos >= (int)code.length() || !(code[pos] >= '0' && code[pos] <= '9')) {
                malformed = true;
            }
        } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            // Example like 2score is invalid
            malformed = true;
            numText = numText + ch;
            pos++;
            col++;
        } else {
            break;
        }
    }

    // More than one decimal point is invalid (e.g., 1.2.3)
    if (dotCount > 1) {
        malformed = true;
    }

    Token t;
    t.lexeme = numText;
    t.line = startLine;
    t.column = startCol;

    if (malformed) {
        t.type = "NO CAP BESTIE! INVALID_NUMBER";
    } else if (dotCount == 1) {
        t.type = "DECIMAL_LITERAL";
    } else {
        t.type = "WHOLE_LITERAL";
    }

    return t;
}

// Reads double-quoted string literals and checks for unclosed strings (L004)
Token readString(string code, int &pos, int &line, int &col) {
    int startLine = line;
    int startCol = col;
    string text = "";

    // Move past opening "
    pos++;
    col++;

    while (pos < (int)code.length()) {
        char ch = code[pos];

        // If line ends before closing ", report ERROR
        if (ch == '\n') {
            Token t;
            t.type = "NO CAP BESTIE!_UNCLOSED_STRING";
            t.lexeme = text;
            t.line = startLine;
            t.column = startCol;
            return t;
        }

        // Closing " found
        if (ch == '"') {
            pos++;
            col++;

            Token t;
            t.type = "STRING_LITERAL";
            t.lexeme = text;
            t.line = startLine;
            t.column = startCol;
            return t;
        }

        // Handle escape sequences like \n, \t, \"
        if (ch == '\\') {
            pos++;
            col++;
            if (pos >= (int)code.length() || code[pos] == '\n') {
                Token t;
                t.type = "NO CAP BESTIE!_UNCLOSED_STRING";
                t.lexeme = text;
                t.line = startLine;
                t.column = startCol;
                return t;
            }
            text = text + '\\';
            text = text + code[pos];
            pos++;
            col++;
        } else {
            text = text + ch;
            pos++;
            col++;
        }
    }

    // Reached end of file without closing "
    Token t;
    t.type = "NO CAP BESTIE!_UNCLOSED_STRING";
    t.lexeme = text;
    t.line = startLine;
    t.column = startCol;
    return t;
}

// Reads single-quoted character literals and checks length/closing quote (L005)
Token readChar(string code, int &pos, int &line, int &col) {
    int startLine = line;
    int startCol = col;
    string shown = "'";

    // Move past opening '
    pos++;
    col++;

    // Empty character literal '' is invalid
    if (pos < (int)code.length() && code[pos] == '\'') {
        shown = shown + '\'';
        pos++;
        col++;

        Token t;
        t.type = "NO CAP BESTIE!_INVALID_CHAR_LITERAL";
        t.lexeme = shown;
        t.line = startLine;
        t.column = startCol;
        return t;
    }

    // Missing character before newline or end of file
    if (pos >= (int)code.length() || code[pos] == '\n') {
        Token t;
        t.type = "NO CAP BESTIE!_INVALID_CHAR_LITERAL";
        t.lexeme = shown;
        t.line = startLine;
        t.column = startCol;
        return t;
    }

    string inside = "";

    // Check if it starts with '\' (escaped char like '\n')
    if (code[pos] == '\\') {
        shown = shown + code[pos];
        inside = inside + code[pos];
        pos++;
        col++;

        if (pos >= (int)code.length() || code[pos] == '\n') {
            Token t;
            t.type = "NO CAP BESTIE!_INVALID_CHAR_LITERAL";
            t.lexeme = shown;
            t.line = startLine;
            t.column = startCol;
            return t;
        }

        shown = shown + code[pos];
        inside = inside + code[pos];
        pos++;
        col++;
    } else {
        // Normal single character
        shown = shown + code[pos];
        inside = inside + code[pos];
        pos++;
        col++;
    }

    // Valid character literal must close right now with '
    if (pos < (int)code.length() && code[pos] == '\'') {
        shown = shown + '\'';
        pos++;
        col++;

        Token t;
        t.type = "CHARACTER_LITERAL";
        t.lexeme = inside;
        t.line = startLine;
        t.column = startCol;
        return t;
    }

    // Otherwise too many characters or unclosed quote -> skip to closing ' or space/newline
    while (pos < (int)code.length() && code[pos] != '\'' && code[pos] != '\n' &&
           code[pos] != ' ' && code[pos] != '\t') {
        shown = shown + code[pos];
        pos++;
        col++;
    }
    if (pos < (int)code.length() && code[pos] == '\'') {
        shown = shown + '\'';
        pos++;
        col++;
    }

    Token t;
    t.type = "NO CAP BESTIE!_INVALID_CHAR_LITERAL";
    t.lexeme = shown;
    t.line = startLine;
    t.column = startCol;
    return t;
}