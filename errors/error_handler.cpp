
#include "error_handler.h"
#include <iostream>
#include <fstream>

using namespace std;

#define RED "\033[31m"
#define RESET "\033[0m"


// SH18: Finds the exact position of an error inside a malformed number.
int getNumberErrorOffset(const string& lexeme)
{
    size_t firstDot = lexeme.find('.');

    // L003: A second decimal point is invalid.
    if (firstDot != string::npos)
    {
        size_t secondDot = lexeme.find('.', firstDot + 1);

        if (secondDot != string::npos)
        {
            return (int)secondDot;
        }
    }

    // L003: Finds letters or underscores inside a number.
    for (int i = 0; i < (int)lexeme.length(); i++)
    {
        char ch = lexeme[i];

        if ((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            ch == '_')
        {
            return i;
        }
    }

    // L003: Decimal point without a digit after it.
    if (!lexeme.empty() && lexeme.back() == '.')
    {
        return (int)lexeme.length() - 1;
    }

    // L003: Number beginning with a decimal point, such as .5.
    if (!lexeme.empty() && lexeme[0] == '.')
    {
        return 0;
    }

    return 0;
}


// SH19: Finds the exact position of an invalid character literal.
int getCharErrorOffset(const string& lexeme)
{
    // L005: Empty character literal: ''
    if (lexeme.length() == 2)
    {
        return 1;
    }

    // L005: More than one character: 'AB'
    if (lexeme.length() > 3 &&
        lexeme[0] == '\'' &&
        lexeme[lexeme.length() - 1] == '\'')
    {
        return 2;
    }

    // L005: Missing closing quote: 'A;
    if (lexeme.length() >= 2 &&
        lexeme[0] == '\'' &&
        lexeme[lexeme.length() - 1] != '\'')
    {
        return (int)lexeme.length() - 1;
    }

    return 0;
}


// SH20: Detects lexical errors L001-L005 in tokens received from the lexer.
void ErrorHandler::checkToken(const string& tokenType,
                              const string& lexeme,
                              const string& sourceLine,
                              int line,
                              int column)
{
    int caretColumn = column;
    int caretLength = 1;

    // L001: Invalid Character.
    if (tokenType == "NO CAP BESTIE! INVALID_CHARACTER")
    {
        addError(
            "L001",
            "Invalid Character",
            lexeme,
            "invalid character '" + lexeme + "'",
            sourceLine,
            line,
            column,
            caretColumn,
            caretLength
        );
    }

    // L002: Invalid Identifier.
    else if (tokenType == "NO CAP BESTIE! INVALID_IDENTIFIER")
    {
        int errorOffset = 0;

        // An identifier beginning with uppercase is invalid at its first character.
        if (!lexeme.empty() &&
            lexeme[0] >= 'A' && lexeme[0] <= 'Z')
        {
            errorOffset = 0;
        }
        // An identifier exceeding 20 characters is invalid at character 21.
        else
        {
            errorOffset = 20;
        }

        caretColumn = column + errorOffset;

        addError(
            "L002",
            "Invalid Identifier",
            lexeme,
            "invalid identifier '" + lexeme + "'",
            sourceLine,
            line,
            caretColumn,
            caretColumn,
            caretLength
        );
    }

    // L003: Malformed Number.
    else if (tokenType == "NO CAP BESTIE! INVALID_NUMBER")
    {
        int errorOffset = getNumberErrorOffset(lexeme);

        caretColumn = column + errorOffset;

        addError(
            "L003",
            "Malformed Number",
            lexeme,
            "malformed number '" + lexeme + "'",
            sourceLine,
            line,
            column,
            caretColumn,
            caretLength
        );
    }

    // L004: Unclosed String.
    else if (tokenType == "NO CAP BESTIE!_UNCLOSED_STRING")
    {
        // The error is reported at the opening quotation mark.
        addError(
            "L004",
            "Unclosed String",
            lexeme,
            "unclosed string literal",
            sourceLine,
            line,
            column,
            column,
            caretLength
        );
    }

    // L005: Invalid Character Literal.
    else if (tokenType == "NO CAP BESTIE!_INVALID_CHAR_LITERAL")
    {
        int errorOffset = getCharErrorOffset(lexeme);

        caretColumn = column + errorOffset;

        addError(
            "L005",
            "Invalid Character Literal",
            lexeme,
            "invalid character literal",
            sourceLine,
            line,
            column,
            caretColumn,
            caretLength
        );
    }
}


// SH21: Stores one detected lexical error.
void ErrorHandler::addError(string code, string name, string lexeme,
                            string message, string sourceLine,
                            int line, int column,
                            int caretColumn, int caretLength)
{
    LexicalError error;

    error.code = code;
    error.name = name;
    error.lexeme = lexeme;
    error.message = message;
    error.sourceLine = sourceLine;

    error.line = line;
    error.column = column;

    error.caretColumn = caretColumn;
    error.caretLength = caretLength;

    errors.push_back(error);
}


// SH22: Prints the source line and exact caret position.
void printSourceLocation(const LexicalError& error, ostream& output)
{
    output << error.sourceLine << "\n";

    // Moves to the exact column where the error occurred.
    for (int i = 1; i < error.caretColumn; i++)
    {
        output << " ";
    }

    // Prints carets across the error area.
    int length = error.caretLength;

    if (length < 1)
    {
        length = 1;
    }

    for (int i = 0; i < length; i++)
    {
        output << "^";
    }

    output << "\n";
}


// SH23: Displays all detected lexical errors on the console.
void ErrorHandler::printErrors()
{
    for (const LexicalError& error : errors)
    {
        cout << RED;

        cout << "input_code.txt:"
             << error.line << ":"
             << error.column
             << ": lexical error: "
             << error.message << "\n";

        printSourceLocation(error, cout);

        cout << RESET;
    }

    if (errors.empty())
    {
        cout << "No lexical errors found.\n";
    }
}


// SH24: Saves all detected lexical errors to an output file.
void ErrorHandler::saveErrors(const string& filename)
{
    ofstream fout(filename);

    if (!fout.is_open())
    {
        cerr << "Could not create lexical error file.\n";
        return;
    }

    for (const LexicalError& error : errors)
    {
        fout << "input_code.txt:"
             << error.line << ":"
             << error.column
             << ": lexical error: "
             << error.message << "\n";

        printSourceLocation(error, fout);

        fout << "\n";
    }

    if (errors.empty())
    {
        fout << "No lexical errors found.\n";
    }

    fout.close();
}


// SH25: Checks whether any lexical errors were detected.
bool ErrorHandler::hasErrors()
{
    return !errors.empty();
}


// SH26: Returns the total number of detected lexical errors.
int ErrorHandler::getErrorCount()
{
    return (int)errors.size();
}
