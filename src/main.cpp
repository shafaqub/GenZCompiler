#include <iostream>
#include <vector>

#include "lexer/lexer.h"
#include "errors/error_handler.h"

using namespace std;

int main()
{
    // 1. Read source code
    // Handled by the lexer workflow.

    // 2. Run lexer
    ErrorHandler errorHandler;
    vector<Token> tokenList = runLexer("input_code.txt", errorHandler);

    // 3. Check lexical errors
    if (tokenList.empty())
    {
        cout << "Lexical analysis could not be completed.\n";
        return 1;
    }

    if (errorHandler.hasErrors())
    {
        cout << "\nCompilation stopped due to lexical errors.\n";
        return 1;
    }

    // 4. Convert lexer tokens into parser tokens
    // To be implemented with the TokenAdapter.

    // 5. Run parser
    // To be implemented.

    // 6. Print AST
    // To be implemented.

    // 7. Run semantic analyzer if parsing succeeds
    // To be implemented.

    // 8. Display final compiler status
    cout << "\nLexical analysis completed successfully.\n";

    return 0;
}