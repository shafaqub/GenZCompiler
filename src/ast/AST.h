
#ifndef AST_H
#define AST_H

// ==========================================
// 1. CONSTANTS
// ==========================================

const int MAX_STATEMENTS = 100;
const int MAX_GLOBALS = 50;
const int MAX_FUNCTIONS = 50;
const int MAX_PARAMETERS = 20;
const int MAX_ARGUMENTS = 20;
const int MAX_NAME_LENGTH = 100;
const int MAX_TEXT_LENGTH = 256;


// ==========================================
// 2. DATA TYPES
// ==========================================

enum DataType {
    TYPE_WHOLE,
    TYPE_DECIMAL,
    TYPE_VIBECHECK,
    TYPE_LETTER,
    TYPE_TEXT,
    TYPE_EMPTY
};


// ==========================================
// 3. SOURCE LOCATION
// ==========================================

struct SourceLocation {
    int line;
    int column;

    SourceLocation() {
        line = 0;
        column = 0;
    }

    SourceLocation(int l, int c) {
        line = l;
        column = c;
    }
};


// ==========================================
// 4. BASE AST NODE
// ==========================================

class ASTNode {
public:
    SourceLocation location;

    ASTNode() {}

    ASTNode(int line, int column) {
        location.line = line;
        location.column = column;
    }

    virtual ~ASTNode() {}
};


// ==========================================
// 5. BASE EXPRESSION AND STATEMENT NODES
// ==========================================

class Expr : public ASTNode {
public:
    Expr() {}
};


class Stmt : public ASTNode {
public:
    Stmt() {}
};


// ==========================================
// 6. EXPRESSION NODES
// ==========================================


// ---------- Literal Expression ----------

enum LiteralType {
    LITERAL_WHOLE,
    LITERAL_DECIMAL,
    LITERAL_VIBECHECK,
    LITERAL_LETTER,
    LITERAL_TEXT
};


class LiteralExpr : public Expr {
public:
    LiteralType literalType;

    long long wholeValue;
    double decimalValue;
    bool vibecheckValue;
    char letterValue;
    char textValue[MAX_TEXT_LENGTH];

    LiteralExpr() {
        literalType = LITERAL_WHOLE;

        wholeValue = 0;
        decimalValue = 0.0;
        vibecheckValue = false;
        letterValue = '\0';
        textValue[0] = '\0';
    }
};


// ---------- Identifier Expression ----------

class IdentifierExpr : public Expr {
public:
    char name[MAX_NAME_LENGTH];

    IdentifierExpr() {
        name[0] = '\0';
    }
};


// ---------- Binary Expression ----------

enum BinaryOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULUS,

    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,

    OP_EQUAL,
    OP_NOT_EQUAL,

    OP_AND,
    OP_OR
};


class BinaryExpr : public Expr {
public:
    Expr* left;
    BinaryOp operation;
    Expr* right;

    BinaryExpr() {
        left = 0;
        right = 0;
        operation = OP_ADD;
    }

    ~BinaryExpr();
};


// ---------- Unary Expression ----------

enum UnaryOp {
    OP_NEGATE,
    OP_NOT
};


class UnaryExpr : public Expr {
public:
    UnaryOp operation;
    Expr* operand;

    UnaryExpr() {
        operand = 0;
        operation = OP_NEGATE;
    }

    ~UnaryExpr();
};


// ---------- Function Call Expression ----------

class FunctionCallExpr : public Expr {
public:
    char functionName[MAX_NAME_LENGTH];

    Expr* arguments[MAX_ARGUMENTS];
    int argumentCount;

    FunctionCallExpr() {
        functionName[0] = '\0';
        argumentCount = 0;

        for (int i = 0; i < MAX_ARGUMENTS; i++) {
            arguments[i] = 0;
        }
    }

    ~ FunctionCallExpr();
    bool addArgument(Expr* argument);
};


// ==========================================
// 7. DECLARATION NODES
// ==========================================


// ---------- Variable Declaration ----------

class VariableDecl : public Stmt {
public:
    char name[MAX_NAME_LENGTH];
    DataType dataType;
    Expr* initializer;

    VariableDecl() {
        name[0] = '\0';
        dataType = TYPE_WHOLE;
        initializer = 0;
    }

    ~VariableDecl();
};


// ---------- Constant Declaration ----------

class ConstantDecl : public Stmt {
public:
    char name[MAX_NAME_LENGTH];
    DataType dataType;
    Expr* initializer;

    ConstantDecl() {
        name[0] = '\0';
        dataType = TYPE_WHOLE;
        initializer = 0;
    }

    ~ ConstantDecl();
};


// ==========================================
// 8. BASIC STATEMENT NODES
// ==========================================


// ---------- Assignment Statement ----------

class AssignmentStmt : public Stmt {
public:
    char variableName[MAX_NAME_LENGTH];
    Expr* value;

    AssignmentStmt() {
        variableName[0] = '\0';
        value = 0;
    }

    ~AssignmentStmt();
};


// ---------- Print Statement ----------

class PrintStmt : public Stmt {
public:
    Expr* expression;

    PrintStmt() {
        expression = 0;
    }

    ~PrintStmt();
};


// ---------- Input Statement ----------

class InputStmt : public Stmt {
public:
    char variableName[MAX_NAME_LENGTH];

    InputStmt() {
        variableName[0] = '\0';
    }
};


// ---------- Function Call Statement ----------

class ExprStmt : public Stmt {
public:
    FunctionCallExpr* functionCall;

    ExprStmt() {
        functionCall = 0;
    }
    ~ExprStmt();
};


// ---------- Return Statement ----------

class ReturnStmt : public Stmt {
public:
    Expr* returnValue;

    ReturnStmt() {
        returnValue = 0;
    }

    ~ReturnStmt();
};


// ---------- Break Statement ----------

class BreakStmt : public Stmt {
};


// ---------- Continue Statement ----------

class ContinueStmt : public Stmt {
};


// ==========================================
// 9. BLOCK NODE
// ==========================================

class BlockStmt : public Stmt {
public:
    Stmt* statements[MAX_STATEMENTS];
    int statementCount;

    BlockStmt() {
        statementCount = 0;

        for (int i = 0; i < MAX_STATEMENTS; i++) {
            statements[i] = 0;
        }
    }

    ~BlockStmt();
    bool addStatement(Stmt* statement);
};


// ==========================================
// 10. CONTROL FLOW NODES
// ==========================================


// ---------- Conditional Statement ----------

class IfStmt : public Stmt {
public:
    Expr* condition;
    BlockStmt* thenBlock;
    BlockStmt* elseBlock;

    IfStmt() {
        condition = 0;
        thenBlock = 0;
        elseBlock = 0;
    }

    ~IfStmt();
};


// ---------- While Statement ----------

class WhileStmt : public Stmt {
public:
    Expr* condition;
    BlockStmt* body;

    WhileStmt() {
        condition = 0;
        body = 0;
    }

    ~WhileStmt();
};


// ---------- For Statement ----------

class ForStmt : public Stmt {
public:
    char variableName[MAX_NAME_LENGTH];

    Expr* startExpression;
    Expr* endExpression;

    BlockStmt* body;

    ForStmt() {
        variableName[0] = '\0';

        startExpression = 0;
        endExpression = 0;
        body = 0;
    }
    ~ForStmt();
};


// ==========================================
// 11. FUNCTION NODES
// ==========================================

struct Parameter {
    char name[MAX_NAME_LENGTH];
    DataType dataType;
};


class FunctionDecl : public ASTNode {
public:
    char functionName[MAX_NAME_LENGTH];

    Parameter parameters[MAX_PARAMETERS];
    int parameterCount;

    DataType returnType;

    BlockStmt* body;

    FunctionDecl() {
        functionName[0] = '\0';

        parameterCount = 0;
        returnType = TYPE_EMPTY;

        body = 0;

        // Initialize all parameter entries
        for (int i = 0; i < MAX_PARAMETERS; i++) {
            parameters[i].name[0] = '\0';
            parameters[i].dataType = TYPE_WHOLE;
        }
    }

    ~FunctionDecl();

    bool addParameter(const char* name, DataType type);
};


// ==========================================
// 12. PROGRAM ROOT NODE
// ==========================================

class Program : public ASTNode {
public:
    char programName[MAX_NAME_LENGTH];

    Stmt* globalDeclarations[MAX_GLOBALS];
    int globalCount;

    FunctionDecl* functions[MAX_FUNCTIONS];
    int functionCount;

    BlockStmt* mainBlock;

    Program() {
        programName[0] = '\0';

        globalCount = 0;
        functionCount = 0;
        mainBlock = 0;

        for (int i = 0; i < MAX_GLOBALS; i++) {
            globalDeclarations[i] = 0;
        }

        for (int i = 0; i < MAX_FUNCTIONS; i++) {
            functions[i] = 0;
        }
    }

    ~Program();

    bool addGlobalDeclaration(Stmt* declaration);
    bool addFunction(FunctionDecl* function);
};

#endif
