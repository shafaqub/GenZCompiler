
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "token.h"
#include "ast.h"

using namespace std;

// Maximum number of syntax errors stored
const int MAX_PARSE_ERRORS = 100;

// Stores information about a syntax error
struct ParseError
{
    string message;
    int line;
    int column;
};

class Parser
{
private:

    // Token array received from the lexer
    Token* tokens;

    // Total number of tokens
    int tokenCount;

    // Current position in the token array
    int currentIndex;

    // Syntax error storage
    ParseError errors[MAX_PARSE_ERRORS];

    int errorCount;

    // Safe EOF token if token access goes out of bounds
    Token eofToken;

public:

    // Constructor
    Parser(Token* tokenArray, int count);

    // =================================
    // TOKEN NAVIGATION FUNCTIONS
    // =================================

    // Returns the current token
    Token current();

    // Looks ahead without consuming
    Token peek(int offset = 1);

    // Moves to the next token
    Token advance();

    // Checks current token type
    bool check(TokenType type);

    // Checks current keyword
    bool checkKeyword(string keyword);

    // Checks and consumes a matching token
    bool match(TokenType type);

    // Checks and consumes a matching keyword
    bool matchKeyword(string keyword);

    // Requires a specific token
    Token consume(TokenType type, string message);

    // Requires a specific keyword
    Token consumeKeyword(string keyword, string message);

    // Checks whether EOF has been reached
    bool isAtEnd();

    // =================================
    // ERROR HANDLING
    // =================================

    void reportError(string message);

    bool hadError();

    int getErrorCount();

    void printErrors();

};

#endif
#pragma once
