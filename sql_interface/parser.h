#pragma once
#include "scanner.h"
#include <memory>
#include <optional>
#include <variant>
#include <vector>

namespace DBCPP::SqlInterface
{
struct LiteralExpression;
struct BinaryExpression;
struct LogicalExpression;
using AnyExpression = std::variant<LiteralExpression, BinaryExpression, LogicalExpression>;
using Expr_ptr = std::unique_ptr<AnyExpression>;

struct LiteralExpression
{
    Token token;
};

struct BinaryExpression
{
    Expr_ptr lhs;
    Token oper;
    Expr_ptr rhs;
};

struct LogicalExpression
{
    Expr_ptr lhs;
    Token oper;
    Expr_ptr rhs;
};

// Node defitinitions
struct WhereNode
{
    Expr_ptr condition;
};

using Where_ptr = std::unique_ptr<WhereNode>;

struct FromNode
{
    std::string tableName;
};

using From_ptr = std::unique_ptr<FromNode>;

struct SelectColumnList
{
    std::vector<Expr_ptr> columns;
};

using SelectColumnList_ptr = std::unique_ptr<SelectColumnList>;

struct SelectNode
{
    SelectColumnList_ptr columnList;
    From_ptr from;
    Where_ptr where;
};

using Select_ptr = std::unique_ptr<SelectNode>;

struct ParsingError
{
    std::string message;
    Token token;
};

class Parser
{
    std::vector<Token> m_tokens;
    std::optional<ParsingError> m_compilationError;
    int m_position = 0;

    void Error(std::string message, Token token);
    Token Consume(TokenType type);
    Token Advance();
    Token Peek();
    Select_ptr Select();
    From_ptr From();
    Where_ptr Where();
    SelectColumnList_ptr ColumnList();
    Expr_ptr MakeExpression();
    Expr_ptr MakeAndExpression();
    Expr_ptr MakeBinaryExpression();
    Expr_ptr MakeLiteralExpression();
    Expr_ptr MakeArithmeticExpression();

  public:
    Parser(std::vector<Token> &tokens) : m_tokens(std::move(tokens))
    {
    }

    Select_ptr Parse();
    ParsingError GetError();
};
} // namespace DBCPP::SqlInterface