// ParserToken.h - the token the PARSER works with.
//
// The lexer's own `Token` (lexer/token.h) carries its type as a string such as
// "KEYWORD" or "OTHER". TokenAdapter.cpp converts those into this enum-based token,
// so the parser can switch on types instead of comparing strings.
#pragma once
#include <string>
#include <vector>

enum class TokenType {
    // program structure
    PERIOD, MAINCHARACTER,
    // declarations
    SUS, LOCK,
    // functions
    SIDEQUESTS, SPITS, DELIVER, HEROERA,
    // control flow
    CLOCKIT, COOKED,
    AURAFARMING,          // the single combined token for "aura farming" / "aurafarming"
    AURA, FARMING,        // only used if the lexer emits the two words separately
    GRINDSET, FROM, TO, DIP, SKIP,
    // input / output
    SPILLTEA, LOCKIN,
    // logical keywords
    AND, OR, NOT,
    TRUE_LITERAL, FALSE_LITERAL,
    // data type keywords
    WHOLE, DECIMAL, TRUTH /* also used for 'vibecheck' */, LETTER, TEXT, EMPTY,
    // identifiers and literals
    IDENTIFIER, WHOLE_LITERAL, DECIMAL_LITERAL, STRING_LITERAL, CHARACTER_LITERAL,
    // arithmetic
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    // relational / equality
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, EQUAL, NOT_EQUAL,
    // assignment: ASSIGNMENT is whichever symbol the language finally uses ('=' or '<-')
    ASSIGNMENT, INITIALIZE /* := */,
    // delimiters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SEMICOLON, COLON, COMMA,
    // a symbol the lexer accepted but the grammar has no use for (e.g. a lone '!')
    UNKNOWN,
    // end of input (named END_OF_FILE because EOF is a C macro)
    END_OF_FILE
};

struct ParserToken {
    TokenType   type   = TokenType::END_OF_FILE;
    std::string lexeme;      // for string/char literals: the content without quotes
    int         line   = 1;
    int         column = 1;
};

inline const char* tokenTypeName(TokenType t) {
    switch (t) {
        case TokenType::PERIOD: return "PERIOD";
        case TokenType::MAINCHARACTER: return "MAINCHARACTER";
        case TokenType::SUS: return "SUS";
        case TokenType::LOCK: return "LOCK";
        case TokenType::SIDEQUESTS: return "SIDEQUESTS";
        case TokenType::SPITS: return "SPITS";
        case TokenType::DELIVER: return "DELIVER";
        case TokenType::HEROERA: return "HEROERA";
        case TokenType::CLOCKIT: return "CLOCKIT";
        case TokenType::COOKED: return "COOKED";
        case TokenType::AURAFARMING: return "AURAFARMING";
        case TokenType::AURA: return "AURA";
        case TokenType::FARMING: return "FARMING";
        case TokenType::GRINDSET: return "GRINDSET";
        case TokenType::FROM: return "FROM";
        case TokenType::TO: return "TO";
        case TokenType::DIP: return "DIP";
        case TokenType::SKIP: return "SKIP";
        case TokenType::SPILLTEA: return "SPILLTEA";
        case TokenType::LOCKIN: return "LOCKIN";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::TRUE_LITERAL: return "TRUE";
        case TokenType::FALSE_LITERAL: return "FALSE";
        case TokenType::WHOLE: return "WHOLE";
        case TokenType::DECIMAL: return "DECIMAL";
        case TokenType::TRUTH: return "TRUTH";
        case TokenType::LETTER: return "LETTER";
        case TokenType::TEXT: return "TEXT";
        case TokenType::EMPTY: return "EMPTY";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::WHOLE_LITERAL: return "WHOLE_LITERAL";
        case TokenType::DECIMAL_LITERAL: return "DECIMAL_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::CHARACTER_LITERAL: return "CHARACTER_LITERAL";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::ASSIGNMENT: return "ASSIGNMENT";
        case TokenType::INITIALIZE: return "INITIALIZE";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::END_OF_FILE: return "EOF";
    }
    return "?";
}

inline bool isTypeKeyword(TokenType t) {
    switch (t) {
        case TokenType::WHOLE: case TokenType::DECIMAL: case TokenType::TRUTH:
        case TokenType::LETTER: case TokenType::TEXT: case TokenType::EMPTY:
            return true;
        default:
            return false;
    }
}
