
#include "parser.h"
#include <iostream>

using namespace std;


// =========================================
// 1. CONSTRUCTOR
// =========================================

Parser::Parser(Token* tokenArray, int count)
{
    tokens = tokenArray;
    tokenCount = count;

    currentIndex = 0;
    errorCount = 0;

    // Initialize fallback EOF token
    eofToken.type = TokenType::END_OF_FILE;
    eofToken.lexeme = "$";
    eofToken.line = 0;
    eofToken.column = 0;
}


// =========================================
// 2. CURRENT TOKEN
// =========================================

Token Parser::current()
{
    if (tokens == 0 || tokenCount <= 0)
    {
        return eofToken;
    }

    if (currentIndex >= tokenCount)
    {
        return eofToken;
    }

    return tokens[currentIndex];
}


// =========================================
// 3. LOOKAHEAD / PEEK
// =========================================

Token Parser::peek(int offset)
{
    if (tokens == 0 || tokenCount <= 0)
    {
        return eofToken;
    }

    int nextIndex = currentIndex + offset;

    if (nextIndex < 0 || nextIndex >= tokenCount)
    {
        return eofToken;
    }

    return tokens[nextIndex];
}


// =========================================
// 4. ADVANCE
// =========================================

Token Parser::advance()
{
    Token previous = current();

    // Never move beyond EOF
    if (!isAtEnd())
    {
        currentIndex++;
    }

    return previous;
}


// =========================================
// 5. CHECK TOKEN TYPE
// =========================================

bool Parser::check(TokenType type)
{
    if (isAtEnd())
    {
        return type == TokenType::END_OF_FILE;
    }

    return current().type == type;
}


// =========================================
// 6. CHECK KEYWORD
// =========================================

bool Parser::checkKeyword(string keyword)
{
    Token tok = current();

    return tok.type == TokenType::KEYWORD &&
        tok.lexeme == keyword;
}


// =========================================
// 7. MATCH TOKEN
// =========================================

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }

    return false;
}


// =========================================
// 8. MATCH KEYWORD
// =========================================

bool Parser::matchKeyword(string keyword)
{
    if (checkKeyword(keyword))
    {
        advance();
        return true;
    }

    return false;
}


// =========================================
// 9. CONSUME REQUIRED TOKEN
// =========================================

Token Parser::consume(TokenType type, string message)
{
    if (check(type))
    {
        return advance();
    }

    reportError(message);

    return current();
}


// =========================================
// 10. CONSUME REQUIRED KEYWORD
// =========================================

Token Parser::consumeKeyword(string keyword, string message)
{
    if (checkKeyword(keyword))
    {
        return advance();
    }

    reportError(message);

    return current();
}


// =========================================
// 11. CHECK END OF FILE
// =========================================

bool Parser::isAtEnd()
{
    if (tokens == 0 || tokenCount <= 0)
    {
        return true;
    }

    return currentIndex >= tokenCount ||
        tokens[currentIndex].type == TokenType::END_OF_FILE;
}


// =========================================
// 12. REPORT SYNTAX ERROR
// =========================================

void Parser::reportError(string message)
{
    if (errorCount >= MAX_PARSE_ERRORS)
    {
        return;
    }

    Token tok = current();

    errors[errorCount].message = message;
    errors[errorCount].line = tok.line;
    errors[errorCount].column = tok.column;

    errorCount++;
}


// =========================================
// 13. CHECK WHETHER ERRORS EXIST
// =========================================

bool Parser::hadError()
{
    return errorCount > 0;
}


// =========================================
// 14. GET ERROR COUNT
// =========================================

int Parser::getErrorCount()
{
    return errorCount;
}


// =========================================
// 15. DISPLAY SYNTAX ERRORS
// =========================================

void Parser::printErrors()
{
    if (errorCount == 0)
    {
        cout << "\nNo syntax errors found.\n";
        return;
    }

    cout << "\n========== SYNTAX ERRORS ==========\n";

    for (int i = 0; i < errorCount; i++)
    {
        cout << "Error " << i + 1 << ": "
            << errors[i].message
            << " at line "
            << errors[i].line
            << ", column "
            << errors[i].column
            << endl;
    }

    cout << "===================================\n";
}
