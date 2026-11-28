// AST.h - Abstract Syntax Tree node definitions for AERIS.
//
// Ownership: every node owns its children through std::unique_ptr, so deleting
// the ProgramNode frees the whole tree and nothing leaks when a syntax error
// aborts a half-built statement.
#pragma once
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include "ParserToken.h"

enum class NodeType {
    Program, Block,
    VariableDeclaration, ConstantDeclaration, Assignment,
    FunctionDeclaration, Parameter,
    IfStatement, WhileStatement, ForStatement,
    PrintStatement, InputStatement, FunctionCall, ReturnStatement,
    BreakStatement, ContinueStatement,
    BinaryExpression, UnaryExpression, Identifier, Literal
};

struct ASTNode {
    NodeType type;
    int line = 0;       // source position of the node's first token
    int column = 0;     // (needed later by the semantic analyzer for error messages)

    explicit ASTNode(NodeType t) : type(t) {}
    virtual ~ASTNode() = default;
    ASTNode(const ASTNode&) = delete;
    ASTNode& operator=(const ASTNode&) = delete;
};

using NodePtr = std::unique_ptr<ASTNode>;

/// Creates a node and stamps it with the position of token `at`.
template <class T, class... Args>
std::unique_ptr<T> makeNode(const ParserToken& at, Args&&... args) {
    auto node = std::make_unique<T>(std::forward<Args>(args)...);
    node->line = at.line;
    node->column = at.column;
    return node;
}

// ---------------------------------------------------------------- expressions
struct IdentifierNode : ASTNode {
    std::string name;
    explicit IdentifierNode(std::string n) : ASTNode(NodeType::Identifier), name(std::move(n)) {}
};

struct LiteralNode : ASTNode {
    std::string value;
    TokenType literalType;   // WHOLE_LITERAL, DECIMAL_LITERAL, STRING_LITERAL, ...
    LiteralNode(std::string v, TokenType t)
        : ASTNode(NodeType::Literal), value(std::move(v)), literalType(t) {}
};

struct UnaryExpressionNode : ASTNode {
    std::string op;          // "not" or "-"
    TokenType opType;
    NodePtr operand;
    UnaryExpressionNode(std::string o, TokenType t, NodePtr e)
        : ASTNode(NodeType::UnaryExpression), op(std::move(o)), opType(t), operand(std::move(e)) {}
};

struct BinaryExpressionNode : ASTNode {
    std::string op;
    TokenType opType;
    NodePtr left, right;
    BinaryExpressionNode(std::string o, TokenType t, NodePtr l, NodePtr r)
        : ASTNode(NodeType::BinaryExpression), op(std::move(o)), opType(t),
          left(std::move(l)), right(std::move(r)) {}
};

/// Used for BOTH `heroera f(x);` (statement) and `... heroera f(x) ...` (expression).
struct FunctionCallNode : ASTNode {
    std::string name;
    std::vector<NodePtr> arguments;
    bool isStatement;
    FunctionCallNode(std::string n, std::vector<NodePtr> args, bool stmt)
        : ASTNode(NodeType::FunctionCall), name(std::move(n)),
          arguments(std::move(args)), isStatement(stmt) {}
};

// ----------------------------------------------------------------- statements
struct BlockNode : ASTNode {
    std::vector<NodePtr> statements;
    BlockNode() : ASTNode(NodeType::Block) {}
};

/// `sus` (variable) and `lock` (constant) declarations share one node type.
struct DeclarationNode : ASTNode {
    bool isConstant;
    std::string name;
    TokenType dataType;      // WHOLE, DECIMAL, TRUTH, LETTER, TEXT, EMPTY
    std::string typeName;    // spelling as written
    NodePtr initializer;
    DeclarationNode(bool constant, std::string n, const ParserToken& typeTok, NodePtr init)
        : ASTNode(constant ? NodeType::ConstantDeclaration : NodeType::VariableDeclaration),
          isConstant(constant), name(std::move(n)), dataType(typeTok.type),
          typeName(typeTok.lexeme), initializer(std::move(init)) {}
};

struct AssignmentNode : ASTNode {
    std::string name;
    NodePtr value;
    AssignmentNode(std::string n, NodePtr v)
        : ASTNode(NodeType::Assignment), name(std::move(n)), value(std::move(v)) {}
};

struct PrintNode : ASTNode {
    NodePtr value;
    explicit PrintNode(NodePtr v) : ASTNode(NodeType::PrintStatement), value(std::move(v)) {}
};

struct InputNode : ASTNode {
    std::string name;
    explicit InputNode(std::string n) : ASTNode(NodeType::InputStatement), name(std::move(n)) {}
};

struct ReturnNode : ASTNode {
    NodePtr value;           // nullptr for a bare `deliver;`
    explicit ReturnNode(NodePtr v) : ASTNode(NodeType::ReturnStatement), value(std::move(v)) {}
};

struct BreakNode : ASTNode {
    BreakNode() : ASTNode(NodeType::BreakStatement) {}
};

struct ContinueNode : ASTNode {
    ContinueNode() : ASTNode(NodeType::ContinueStatement) {}
};

struct IfNode : ASTNode {
    NodePtr condition;
    std::unique_ptr<BlockNode> thenBlock;
    std::unique_ptr<BlockNode> elseBlock;   // nullptr when there is no `cooked`
    IfNode(NodePtr c, std::unique_ptr<BlockNode> t, std::unique_ptr<BlockNode> e)
        : ASTNode(NodeType::IfStatement), condition(std::move(c)),
          thenBlock(std::move(t)), elseBlock(std::move(e)) {}
};

struct WhileNode : ASTNode {
    NodePtr condition;
    std::unique_ptr<BlockNode> body;
    WhileNode(NodePtr c, std::unique_ptr<BlockNode> b)
        : ASTNode(NodeType::WhileStatement), condition(std::move(c)), body(std::move(b)) {}
};

struct ForNode : ASTNode {
    std::string loopVariable;
    NodePtr startValue, endValue;
    std::unique_ptr<BlockNode> body;
    ForNode(std::string v, NodePtr s, NodePtr e, std::unique_ptr<BlockNode> b)
        : ASTNode(NodeType::ForStatement), loopVariable(std::move(v)),
          startValue(std::move(s)), endValue(std::move(e)), body(std::move(b)) {}
};

// ------------------------------------------------------- functions and program
struct ParameterNode : ASTNode {
    TokenType dataType;
    std::string typeName;
    std::string name;
    ParameterNode(const ParserToken& typeTok, std::string n)
        : ASTNode(NodeType::Parameter), dataType(typeTok.type),
          typeName(typeTok.lexeme), name(std::move(n)) {}
};

struct FunctionNode : ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ParameterNode>> parameters;
    TokenType returnType;
    std::string returnTypeName;
    std::unique_ptr<BlockNode> body;
    FunctionNode(std::string n, std::vector<std::unique_ptr<ParameterNode>> params,
                 const ParserToken& retTok, std::unique_ptr<BlockNode> b)
        : ASTNode(NodeType::FunctionDeclaration), name(std::move(n)),
          parameters(std::move(params)), returnType(retTok.type),
          returnTypeName(retTok.lexeme), body(std::move(b)) {}
};

struct ProgramNode : ASTNode {
    std::string programName;
    std::vector<NodePtr> globalDeclarations;
    std::vector<std::unique_ptr<FunctionNode>> functions;
    std::unique_ptr<BlockNode> mainBlock;
    ProgramNode() : ASTNode(NodeType::Program) {}
};

/// Pretty-prints a tree (debugging aid and part of the project's required output).
void printAST(const ASTNode* root, std::ostream& os);
