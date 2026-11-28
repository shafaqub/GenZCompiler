#include "lexer.h"
#include "../errors/error_handler.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Runs the complete lexical analysis process.
vector<Token> runLexer(const string& filename, ErrorHandler& errorHandler)
{
    // 1. Read source code.
    ifstream inFile(filename);

    if (!inFile.is_open())
    {
        cerr << "Error: Could not open " << filename << "\n";
        return vector<Token>();
    }

    string code = "";
    string lineText;
    vector<string> sourceLines;

    while (getline(inFile, lineText))
    {
        sourceLines.push_back(lineText);
        code += lineText + "\n";
    }

    inFile.close();

    // 2. Create token output file.
    ofstream outFile("tokens.txt");

    if (!outFile.is_open())
    {
        cerr << "Error: Could not create tokens.txt\n";
        return vector<Token>();
    }

    outFile << "LINE\tCOL\tTOKEN_TYPE\t\tLEXEME\n";
    outFile << "-------------------------------------------------------------\n";

    // 3. Initialize scanner position.
    int pos = 0;
    int line = 1;
    int col = 1;
    int totalChar = (int)code.length();

    vector<Token> tokenList;

    // 4. Scan the source code character by character.
    while (pos < totalChar)
    {
        char ch = code[pos];

        // Skip newlines.
        if (ch == '\n')
        {
            line++;
            col = 1;
            pos++;
            continue;
        }

        // Skip whitespace.
        if (ch == ' ' || ch == '\t' || ch == '\r')
        {
            pos++;
            col++;
            continue;
        }

        // Skip single-line comments.
        if (isCommentStart(code, pos))
        {
            skipComment(code, pos, line, col);
            continue;
        }

        Token tok;

        // Identifiers and lowercase keywords.
        if (ch >= 'a' && ch <= 'z')
        {
            tok = readIdentifierOrKeyword(code, pos, line, col);
        }

        // Invalid identifiers beginning with uppercase letters.
        else if (ch >= 'A' && ch <= 'Z')
        {
            tok.type = TokenType::INVALID_IDENTIFIER;
            tok.line = line;
            tok.column = col;

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

        // Whole and decimal numbers.
        else if (ch >= '0' && ch <= '9')
        {
            tok = readNumber(code, pos, line, col);
        }

        // Numbers beginning with a decimal point.
        else if (ch == '.' && pos + 1 < totalChar &&
                 code[pos + 1] >= '0' && code[pos + 1] <= '9')
        {
            tok = readNumber(code, pos, line, col);

            // A decimal must have digits before and after the dot.
            if (!tok.lexeme.empty() && tok.lexeme[0] == '.')
            {
                tok.type = TokenType::INVALID_NUMBER;
            }
        }

        // String literals.
        else if (ch == '"')
        {
            tok = readString(code, pos, line, col);
        }

        // Character literals.
        else if (ch == '\'')
        {
            tok = readChar(code, pos, line, col);
        }

        // Delimiters and punctuation.
        else if (isDelimiterAt(code, pos))
        {
            tok = readDelimiter(code, pos, line, col);
        }

        // Operators and invalid characters.
        else
        {
            tok.line = line;
            tok.column = col;

            string twoChar = "";

            if (pos + 1 < code.length())
            {
                twoChar = code.substr(pos, 2);
            }

            // Initialization operator :=
            if (twoChar == ":=")
            {
                tok.lexeme = twoChar;
                tok.type = TokenType::INITIALIZATION_OPERATOR;

                pos += 2;
                col += 2;
            }

            // Unsupported operator combination <-
            else if (twoChar == "<-")
            {
                tok.lexeme = twoChar;
                tok.type = TokenType::INVALID_OPERATOR;

                pos += 2;
                col += 2;
            }

            // Two-character relational and equality operators.
            else if (twoChar == "<=" || twoChar == ">=" ||
                     twoChar == "==" || twoChar == "!=")
            {
                tok.lexeme = twoChar;

                if (twoChar == "==" || twoChar == "!=")
                {
                    tok.type = TokenType::EQUALITY_OPERATOR;
                }
                else
                {
                    tok.type = TokenType::RELATIONAL_OPERATOR;
                }

                pos += 2;
                col += 2;
            }

            // Single-character operators.
            else
            {
                tok.lexeme = string(1, ch);

                switch (ch)
                {
                    // Arithmetic operators.
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '%':
                        tok.type = TokenType::ARITHMETIC_OPERATOR;
                        break;

                    // Relational operators.
                    case '<':
                    case '>':
                        tok.type = TokenType::RELATIONAL_OPERATOR;
                        break;

                    // Assignment operator.
                    case '=':
                        tok.type = TokenType::ASSIGNMENT_OPERATOR;
                        break;

                    // Unsupported standalone !
                    case '!':
                        tok.type = TokenType::INVALID_OPERATOR;
                        break;

                    // Invalid characters.
                    default:
                        tok.type = TokenType::INVALID_CHARACTER;
                        break;
                }

                pos++;
                col++;
            }
        }

        // Convert logical keywords into logical operator tokens.
        if (tok.type == TokenType::KEYWORD &&
            (tok.lexeme == "and" ||
             tok.lexeme == "or" ||
             tok.lexeme == "not"))
        {
            tok.type = TokenType::LOGICAL_OPERATOR;
        }

        // Retrieve the source line for error reporting.
        string sourceLine = "";

        if (tok.line >= 1 && tok.line <= (int)sourceLines.size())
        {
            sourceLine = sourceLines[tok.line - 1];
        }

        // Check the token for lexical errors.
        errorHandler.checkToken(
            tokenTypeToString(tok.type),
            tok.lexeme,
            sourceLine,
            tok.line,
            tok.column
        );

        // Store the token for the parser.
        tokenList.push_back(tok);

        // Write the token to the output file.
        outFile << tok.line << "\t\t"
                << tok.column << "\t\t"
                << tokenTypeToString(tok.type) << "\t\t"
                << tok.lexeme << "\n";

        // Display the token.
        cout << "[" << tok.line << ":" << tok.column << "] "
             << tokenTypeToString(tok.type) << " -> "
             << tok.lexeme << "\n";
    }

    // 5. Add the EOF token.
    Token eofToken;
    eofToken.type = TokenType::END_OF_FILE;
    eofToken.lexeme = "$";
    eofToken.line = line;
    eofToken.column = col;

    tokenList.push_back(eofToken);

    outFile << eofToken.line << "\t\t"
            << eofToken.column << "\t\t"
            << tokenTypeToString(eofToken.type) << "\t\t"
            << eofToken.lexeme << "\n";

    cout << "[" << eofToken.line << ":" << eofToken.column << "] "
         << tokenTypeToString(eofToken.type) << " -> "
         << eofToken.lexeme << "\n";

    outFile.close();

    // 6. Save the symbol table.
    saveSymbolTable("symbol_table.txt");

    // 7. Display and save lexical errors.
    cout << "\n";
    errorHandler.printErrors();
    errorHandler.saveErrors("lexical_errors.txt");

    cout << "\nProcessing complete.\n";
    cout << "Check tokens.txt, symbol_table.txt and lexical_errors.txt.\n";

    return tokenList;
}