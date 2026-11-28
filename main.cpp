
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer/lexer.h"
#include "errors/error_handler.h"

using namespace std;


int main()
{
    // M01: Opens and reads the source code input file.
    ifstream inFile("input_code.txt");

    if (!inFile.is_open())
    {
        cout << "Error: Could not open input_code.txt\n";
        return 1;
    }

    // M02: Creates the output file for generated tokens.
    ofstream outFile("tokens.txt");

    if (!outFile.is_open())
    {
        cout << "Error: Could not create tokens.txt\n";
        return 1;
    }

    // SH08: Creates the error handler to store and report lexical errors.
    ErrorHandler errorHandler;

    // SH30: Stores all generated tokens for the parser.
    vector<Token> tokenList;

    // M01: Reads the complete source code into one string.
    
    // SH03: Preserves individual source lines for error diagnostics.
    string code = "";
    string lineText;
    vector<string> sourceLines;

    while (getline(inFile, lineText))
    {
        sourceLines.push_back(lineText);
        code += lineText + "\n";
    }

    inFile.close();

    // M02: Writes the token output file header.
    outFile << "LINE\tCOL\tTOKEN_TYPE\t\tLEXEME\n";
    outFile << "-------------------------------------------------------------\n";

    // M03: Initializes source position tracking.
    int pos = 0;
    int line = 1;
    int col = 1;
    int totalChar = (int)code.length();

    // M03: Scans the source code character by character.
    while (pos < totalChar)
    {
        char ch = code[pos];

        // M03: Skips whitespace and updates line and column positions.
        if (ch == '\n')
        {
            line++;
            col = 1;
            pos++;
            continue;
        }

        if (ch == ' ' || ch == '\t' || ch == '\r')
        {
            pos++;
            col++;
            continue;
        }

        Token tok;

        // M04: Calls Merum's existing lexer functions for token recognition.
        if (ch >= 'a' && ch <= 'z')
        {
            tok = readIdentifierOrKeyword(code, pos, line, col);
        }

        // SH04: Detects identifiers beginning with uppercase letters.
        else if (ch >= 'A' && ch <= 'Z')
        {
            tok.type = "NO CAP BESTIE! INVALID_IDENTIFIER";

            tok.line = line;
            tok.column = col;

            // Consumes the complete invalid identifier.
            while (pos < totalChar)
            {
                char current = code[pos];

                if ((current >= 'a' && current <= 'z') ||
                    (current >= 'A' && current <= 'Z') ||
                    (current >= '0' && current <= '9') ||
                    current == '_')
                {
                    tok.lexeme += current;
                    pos++;
                    col++;
                }
                else
                {
                    break;
                }
            }
        }

        else if (ch >= '0' && ch <= '9')
        {
            tok = readNumber(code, pos, line, col);
        }

        // SH05: Handles numbers beginning with a decimal point.
        else if (ch == '.' && pos + 1 < totalChar &&
                 code[pos + 1] >= '0' && code[pos + 1] <= '9')
        {
            tok = readNumber(code, pos, line, col);

            // A decimal must have digits before and after the dot.
            if (!tok.lexeme.empty() && tok.lexeme[0] == '.')
            {
                tok.type = "NO CAP BESTIE! INVALID_NUMBER";
            }
        }

        else if (ch == '"')
        {
            tok = readString(code, pos, line, col);
        }

        else if (ch == '\'')
        {
            tok = readChar(code, pos, line, col);
        }



        // S(Sana), SH(Shafaq): Recognizes operators and delimiters using DFA-inspired classification.
        else
        {
            tok.line = line;
            tok.column = col;

            string twoChar = "";

            if (pos + 1 < code.length())
                twoChar = code.substr(pos, 2);

            // S06: Recognizes the initialization operator :=
            if (twoChar == ":=")
            {
                tok.lexeme = twoChar;
                tok.type = "INITIALIZATION_OPERATOR";

                pos += 2;
                col += 2;
            }

            // SH06: Rejects unsupported operator combination <-
            else if (twoChar == "<-")
            {
                tok.lexeme = twoChar;
                tok.type = "NO CAP BESTIE! INVALID_OPERATOR";

                pos += 2;
                col += 2;
            }

            // S02-S04: Recognizes two-character relational and equality operators.
            else if (twoChar == "<=" || twoChar == ">=" ||
                     twoChar == "==" || twoChar == "!=")
            {
                tok.lexeme = twoChar;

                if (twoChar == "==" || twoChar == "!=")
                    tok.type = "EQUALITY_OPERATOR";
                else
                    tok.type = "RELATIONAL_OPERATOR";

                pos += 2;
                col += 2;
            }

            // S01, S02, S05: Recognizes single-character operators.
            else
            {
                tok.lexeme = string(1, ch);

                switch (ch)
                {
                    // S01: Arithmetic operators
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '%':
                        tok.type = "ARITHMETIC_OPERATOR";
                        break;

                    // S02: Relational operators
                    case '<':
                    case '>':
                        tok.type = "RELATIONAL_OPERATOR";
                        break;

                    // S05: Assignment operator
                    case '=':
                        tok.type = "ASSIGNMENT_OPERATOR";
                        break;

                    // SH06: Standalone ! is not supported
                    case '!':
                        tok.type = "INVALID_OPERATOR";
                        break;

                    // Existing delimiters
                    case '(':
                    case ')':
                    case '{':
                    case '}':
                    case ';':
                    case ':':
                    case ',':
                        tok.type = "DELIMITER";
                        break;

                    // SH06: Reports unsupported characters
                    default:
                        tok.type = "NO CAP BESTIE! INVALID_CHARACTER";
                        break;
                }

                pos++;
                col++;
            }
        }

        // SH07: Skips single-line comments starting with lowkey.
        if (tok.type == "KEYWORD" && tok.lexeme == "lowkey")
        {
            while (pos < totalChar && code[pos] != '\n')
            {
                pos++;
                col++;
            }

            continue;
        }


        // S07: Classifies logical operators recognized by the lexer.
        if (tok.type == "KEYWORD" &&
            (tok.lexeme == "and" ||
             tok.lexeme == "or" ||
             tok.lexeme == "not"))
        {
            tok.type = "LOGICAL_OPERATOR";
        }

        // SH03: Retrieves the original source line for error reporting.
        string sourceLine = "";

        if (tok.line >= 1 && tok.line <= (int)sourceLines.size())
        {
            sourceLine = sourceLines[tok.line - 1];
        }

        // SH27: Passes the recognized token to the separate error handler.
        errorHandler.checkToken(
            tok.type,
            tok.lexeme,
            sourceLine,
            tok.line,
            tok.column
        );

        // SH30: Stores the token in the token list for future parsing.
        tokenList.push_back(tok);


        // M05: Writes the recognized token to the output file.
        outFile << tok.line << "\t\t"
                << tok.column << "\t\t"
                << tok.type << "\t\t"
                << tok.lexeme << "\n";

        // M05: Displays the recognized token on the console.
        cout << "[" << tok.line << ":" << tok.column << "] "
             << tok.type << " -> " << tok.lexeme << "\n";
    }

    // SH30: Adds EOF to mark the end of the token stream.
    Token eofToken;
    eofToken.type = "EOF";
    eofToken.lexeme = "$";
    eofToken.line = line;
    eofToken.column = col;

    tokenList.push_back(eofToken);

    outFile << eofToken.line << "\t\t"
            << eofToken.column << "\t\t"
            << eofToken.type << "\t\t"
            << eofToken.lexeme << "\n";

    cout << "[" << eofToken.line << ":" << eofToken.column << "] "
        << eofToken.type << " -> " << eofToken.lexeme << "\n";

    
    outFile.close();

    // M06: Saves the symbol table generated by Merum's lexer.
    saveSymbolTable("symbol_table.txt");

    // SH28: Displays and saves all detected lexical errors.
    cout << "\n";

    errorHandler.printErrors();
    errorHandler.saveErrors("lexical_errors.txt");

    cout << "\nProcessing complete.\n";
    cout << "Check tokens.txt, symbol_table.txt and lexical_errors.txt.\n";

    return 0;
}
