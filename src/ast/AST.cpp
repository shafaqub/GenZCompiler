
#include "AST.h"


// ==========================================
// 1. ADD FUNCTION ARGUMENT
// ==========================================

bool FunctionCallExpr::addArgument(Expr* argument) {

    // Check if argument is valid and array has space
    if (argument == 0 || argumentCount >= MAX_ARGUMENTS) {
        return false;
    }

    // Store argument in the next available position
    arguments[argumentCount] = argument;

    // Increase argument count
    argumentCount++;

    return true;
}


// ==========================================
// 2. ADD STATEMENT TO BLOCK
// ==========================================

bool BlockStmt::addStatement(Stmt* statement) {

    // Check if statement is valid and array has space
    if (statement == 0 || statementCount >= MAX_STATEMENTS) {
        return false;
    }

    // Store statement in the next available position
    statements[statementCount] = statement;

    // Increase statement count
    statementCount++;

    return true;
}


// ==========================================
// 3. ADD PARAMETER TO FUNCTION
// ==========================================

bool FunctionDecl::addParameter(const char* name, DataType type) {

    // Check if name is valid and parameter array has space
    if (name == 0 || parameterCount >= MAX_PARAMETERS) {
        return false;
    }

    // Copy parameter name into the array
    int i = 0;

    while (name[i] != '\0' && i < MAX_NAME_LENGTH - 1) {
        parameters[parameterCount].name[i] = name[i];
        i++;
    }

    // Add null terminator
    parameters[parameterCount].name[i] = '\0';

    // Store parameter data type
    parameters[parameterCount].dataType = type;

    // Increase parameter count
    parameterCount++;

    return true;
}


// ==========================================
// 4. ADD GLOBAL DECLARATION TO PROGRAM
// ==========================================

bool Program::addGlobalDeclaration(Stmt* declaration) {

    // Check if declaration is valid and array has space
    if (declaration == 0 || globalCount >= MAX_GLOBALS) {
        return false;
    }

    // Store global declaration
    globalDeclarations[globalCount] = declaration;

    // Increase global declaration count
    globalCount++;

    return true;
}


// ==========================================
// 5. ADD FUNCTION TO PROGRAM
// ==========================================

bool Program::addFunction(FunctionDecl* function) {

    // Check if function is valid and array has space
    if (function == 0 || functionCount >= MAX_FUNCTIONS) {
        return false;
    }

    // Store function
    functions[functionCount] = function;

    // Increase function count
    functionCount++;

    return true;
}


// ==========================================
// 6. BINARY EXPRESSION DESTRUCTOR
// ==========================================

BinaryExpr::~BinaryExpr() {

    delete left;
    delete right;
}


// ==========================================
// 7. UNARY EXPRESSION DESTRUCTOR
// ==========================================

UnaryExpr::~UnaryExpr() {

    delete operand;
}


// ==========================================
// 8. FUNCTION CALL EXPRESSION DESTRUCTOR
// ==========================================

FunctionCallExpr::~FunctionCallExpr() {

    for (int i = 0; i < argumentCount; i++) {
        delete arguments[i];
    }
}


// ==========================================
// 9. VARIABLE DECLARATION DESTRUCTOR
// ==========================================

VariableDecl::~VariableDecl() {

    delete initializer;
}


// ==========================================
// 10. CONSTANT DECLARATION DESTRUCTOR
// ==========================================

ConstantDecl::~ConstantDecl() {

    delete initializer;
}


// ==========================================
// 11. ASSIGNMENT STATEMENT DESTRUCTOR
// ==========================================

AssignmentStmt::~AssignmentStmt() {

    delete value;
}


// ==========================================
// 12. PRINT STATEMENT DESTRUCTOR
// ==========================================

PrintStmt::~PrintStmt() {

    delete expression;
}


// ==========================================
// 13. FUNCTION CALL STATEMENT DESTRUCTOR
// ==========================================

ExprStmt::~ExprStmt() {

    delete functionCall;
}


// ==========================================
// 14. RETURN STATEMENT DESTRUCTOR
// ==========================================

ReturnStmt::~ReturnStmt() {

    delete returnValue;
}


// ==========================================
// 15. BLOCK STATEMENT DESTRUCTOR
// ==========================================

BlockStmt::~BlockStmt() {

    for (int i = 0; i < statementCount; i++) {
        delete statements[i];
    }
}


// ==========================================
// 16. CONDITIONAL STATEMENT DESTRUCTOR
// ==========================================

IfStmt::~IfStmt() {

    delete condition;
    delete thenBlock;
    delete elseBlock;
}


// ==========================================
// 17. WHILE STATEMENT DESTRUCTOR
// ==========================================

WhileStmt::~WhileStmt() {

    delete condition;
    delete body;
}


// ==========================================
// 18. FOR STATEMENT DESTRUCTOR
// ==========================================

ForStmt::~ForStmt() {

    delete startExpression;
    delete endExpression;
    delete body;
}


// ==========================================
// 19. FUNCTION DECLARATION DESTRUCTOR
// ==========================================

FunctionDecl::~FunctionDecl() {

    delete body;
}


// ==========================================
// 20. PROGRAM DESTRUCTOR
// ==========================================

Program::~Program() {

    // Delete global declarations
    for (int i = 0; i < globalCount; i++) {
        delete globalDeclarations[i];
    }

    // Delete functions
    for (int i = 0; i < functionCount; i++) {
        delete functions[i];
    }

    // Delete main block
    delete mainBlock;
}
