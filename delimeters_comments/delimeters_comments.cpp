#include "../lexer/delimeters_comments.h"
using namespace std;

// Helper: letters, digits and underscore can be part of a word.
// (Named differently from Merum's helpers so the two files never clash.)
static bool isWordCharacter(char ch)
{
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           (ch >= '0' && ch <= '9') ||
           ch == '_';
}

// F01: Checks whether a comment starts at position pos.
bool isCommentStart(const string& code, int pos)
{
    const string marker = "lowkey";
    int totalChars = (int)code.length();
    int markerLength = (int)marker.length();

    if (pos < 0 || pos + markerLength > totalChars)
    {
        return false;
    }

    // The text at pos must be exactly "lowkey" (case-sensitive).
    if (code.compare(pos, markerLength, marker) != 0)
    {
        return false;
    }

    // It must not be the middle of a longer word (e.g. xlowkey).
    if (pos > 0 && isWordCharacter(code[pos - 1]))
    {
        return false;
    }

    // It must be the whole word: lowkeyfoo is an identifier, not a comment.
    int next = pos + markerLength;
    if (next < totalChars && isWordCharacter(code[next]))
    {
        return false;
    }

    return true;
}

// F02: Skips everything from "lowkey" to the end of the line.
void skipComment(const string& code, int &pos, int &line, int &col)
{
    int totalChars = (int)code.length();

    // Stop AT the newline; the main loop will count the new line.
    while (pos < totalChars && code[pos] != '\n')
    {
        pos++;
        col++;
    }
}

// F03: Checks whether the character at pos is a delimiter or punctuation.
bool isDelimiterAt(const string& code, int pos)
{
    if (pos < 0 || pos >= (int)code.length())
    {
        return false;
    }

    char ch = code[pos];

    if (ch == '(' || ch == ')' ||
        ch == '{' || ch == '}' ||
        ch == ';' || ch == ',')
    {
        return true;
    }

    if (ch == ':')
    {
        // ":=" is the initialization operator (the operators part of the
        // lexer handles it), so a ':' followed by '=' is not punctuation.
        if (pos + 1 < (int)code.length() && code[pos + 1] == '=')
        {
            return false;
        }
        return true;
    }

    return false;
}

// F04: Reads one delimiter or punctuation token.
Token readDelimiter(const string& code, int &pos, int &line, int &col)
{
    char ch = code[pos];

    Token tok;
    tok.lexeme = string(1, ch);
    tok.line = line;
    tok.column = col;

    if (ch == '(')      tok.type = "LPAREN";
    else if (ch == ')') tok.type = "RPAREN";
    else if (ch == '{') tok.type = "LBRACE";
    else if (ch == '}') tok.type = "RBRACE";
    else if (ch == ';') tok.type = "SEMICOLON";
    else if (ch == ':') tok.type = "COLON";
    else if (ch == ',') tok.type = "COMMA";
    else                tok.type = "NO CAP BESTIE! INVALID_CHARACTER";

    pos++;
    col++;

    return tok;
}
