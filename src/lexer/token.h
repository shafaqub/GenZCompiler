#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

enum class TokenType
{
    KEYWORD,
    IDENTIFIER,

    BOOLEAN_LITERAL,
    WHOLE_LITERAL,
    DECIMAL_LITERAL,
    STRING_LITERAL,
    CHARACTER_LITERAL,

    ARITHMETIC_OPERATOR,
    RELATIONAL_OPERATOR,
    EQUALITY_OPERATOR,
    ASSIGNMENT_OPERATOR,
    INITIALIZATION_OPERATOR,
    LOGICAL_OPERATOR,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COLON,
    COMMA,

    INVALID_CHARACTER,
    INVALID_IDENTIFIER,
    INVALID_NUMBER,
    INVALID_OPERATOR,
    UNCLOSED_STRING,
    INVALID_CHAR_LITERAL,

    END_OF_FILE,
    UNKNOWN
};

struct Token
{
    TokenType type = TokenType::UNKNOWN;
    string lexeme;
    int line = 0;
    int column = 0;
};

inline string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::BOOLEAN_LITERAL: return "BOOLEAN_LITERAL";
        case TokenType::WHOLE_LITERAL: return "WHOLE_LITERAL";
        case TokenType::DECIMAL_LITERAL: return "DECIMAL_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::CHARACTER_LITERAL: return "CHARACTER_LITERAL";

        case TokenType::ARITHMETIC_OPERATOR: return "ARITHMETIC_OPERATOR";
        case TokenType::RELATIONAL_OPERATOR: return "RELATIONAL_OPERATOR";
        case TokenType::EQUALITY_OPERATOR: return "EQUALITY_OPERATOR";
        case TokenType::ASSIGNMENT_OPERATOR: return "ASSIGNMENT_OPERATOR";
        case TokenType::INITIALIZATION_OPERATOR: return "INITIALIZATION_OPERATOR";
        case TokenType::LOGICAL_OPERATOR: return "LOGICAL_OPERATOR";

        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";

        case TokenType::INVALID_CHARACTER: return "INVALID_CHARACTER";
        case TokenType::INVALID_IDENTIFIER: return "INVALID_IDENTIFIER";
        case TokenType::INVALID_NUMBER: return "INVALID_NUMBER";
        case TokenType::INVALID_OPERATOR: return "INVALID_OPERATOR";
        case TokenType::UNCLOSED_STRING: return "UNCLOSED_STRING";
        case TokenType::INVALID_CHAR_LITERAL: return "INVALID_CHAR_LITERAL";

        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

#endif