
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <vector>

using namespace std;

// SH09: Stores all information about one lexical error.
struct LexicalError
{
    string code;
    string name;
    string lexeme;
    string message;
    string sourceLine;

    int line;
    int column;

    int caretColumn;
    int caretLength;
};

// SH10: Handles lexical error detection, storage and reporting.
class ErrorHandler
{
private:
    // SH11: Stores all detected lexical errors.
    vector<LexicalError> errors;

public:
    // SH12: Checks a token for lexical errors L001-L005.
    void checkToken(const string& tokenType,
                    const string& lexeme,
                    const string& sourceLine,
                    int line,
                    int column);

    // SH13: Stores a detected lexical error.
    void addError(string code, string name, string lexeme,
                  string message, string sourceLine,
                  int line, int column,
                  int caretColumn, int caretLength);

    // SH14: Displays detected lexical errors.
    void printErrors();

    // SH15: Saves detected lexical errors to a file.
    void saveErrors(const string& filename);

    // SH16: Checks whether any lexical errors exist.
    bool hasErrors();

    // SH17: Returns the number of detected lexical errors.
    int getErrorCount();
};

#endif
