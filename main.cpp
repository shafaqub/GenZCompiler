
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer/lexer.h"
#include "errors/error_handler.h"
#include "lexer/delimeters_comments.h"

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

        // F05: Skips single-line comments that starts with lowkey
        if (isCommentStart (code, pos))
        {
            skipComment (code, pos, line, col);
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


        // F06: recoginizes delimeters and punctuations () {} : ; , 
        else if (isDelimiterAt (code, pos))
        {
            tok =readDelimiter (code, pos, line, col);
        }
        // SH06: Identifies valid symbols and reports unsupported characters.
        else
        {
            string validSymbols = "+-*/%<>=!():;,{}";

            tok.lexeme = string(1, ch);
            tok.line = line;
            tok.column = col;

            if (validSymbols.find(ch) != string::npos)
            {
                tok.type = "OTHER";
            }
            else
            {
                tok.type = "NO CAP BESTIE! INVALID_CHARACTER";
            }

            pos++;
            col++;
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

        // M05: Writes the recognized token to the output file.
        outFile << tok.line << "\t\t"
                << tok.column << "\t\t"
                << tok.type << "\t\t"
                << tok.lexeme << "\n";

        // M05: Displays the recognized token on the console.
        cout << "[" << tok.line << ":" << tok.column << "] "
             << tok.type << " -> " << tok.lexeme << "\n";
    }

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
