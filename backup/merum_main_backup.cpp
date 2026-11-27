#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
using namespace std;

int main() {
    ifstream inFile("input_code.txt");
    if (!inFile.is_open()) {
        cout << "Error: Could not open input_code.txt" << endl;
        return 1;
    }

    ofstream outFile("tokens.txt");
    if (!outFile.is_open()) {
        cout << "Error: Could not open tokens.txt" << endl;
        return 1;
    }

    // Read full source code from file into one string
    string code = "";
    string lineText = "";
    while (getline(inFile, lineText)) {
        code = code + lineText + "\n";
    }
    inFile.close();

    outFile << "LINE\tCOL\tTOKEN_TYPE\t\t\tLEXEME\n";
    outFile << "-------------------------------------------------------------\n";

    int pos = 0;
    int line = 1;
    int col = 1;
    int totalChar = (int)code.length();

    while (pos < totalChar) {
        char ch = code[pos];

        // Track line and column numbers across whitespace
        if (ch == '\n') {
            line++;
            col = 1;
            pos++;
            continue;
        }
        if (ch == ' ' || ch == '\t' || ch == '\r') {
            pos++;
            col++;
            continue;
        }

        Token tok;

        if (ch >= 'a' && ch <= 'z') {
            tok = readIdentifierOrKeyword(code, pos, line, col);
        } else if (ch >= '0' && ch <= '9') {
            tok = readNumber(code, pos, line, col);
        } else if (ch == '.' && (pos + 1) < totalChar &&
                   code[pos + 1] >= '0' && code[pos + 1] <= '9') {
            // Leading dot followed by digit (like .5) -> malformed number
            tok = readNumber(code, pos, line, col);
        } else if (ch == '"') {
            tok = readString(code, pos, line, col);
        } else if (ch == '\'') {
            tok = readChar(code, pos, line, col);
        } else {
            // Placeholder for operators/delimiters handled by other teammates
            tok.type = "OTHER";
            tok.lexeme = string(1, ch);
            tok.line = line;
            tok.column = col;
            pos++;
            col++;
        }

        outFile << tok.line << "\t\t" << tok.column << "\t\t"
                << tok.type << "\t\t" << tok.lexeme << "\n";

        cout << "[" << tok.line << ":" << tok.column << "] "
             << tok.type << " -> " << tok.lexeme << endl;
    }

    saveSymbolTable("symbol_table.txt");
    outFile.close();

    cout << "\nDone! Check tokens.txt and symbol_table.txt." << endl;
    return 0;
}
