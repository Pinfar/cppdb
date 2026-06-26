#include "parser.h"
#include <stdexcept>
namespace DBCPP::SqlInterface
{
void Parser::Error(std::string message, Token token)
{
    m_compilationError = ParsingError{message, token};
    throw std::domain_error("Parsing error! " + message);
}

static std::string GetTokenValue(Token token)
{
    return token.source->substr(token.beginOffset, token.length);
}

Token Parser::Consume(TokenType type)
{
    Token token = Advance();
    if (token.type != type)
    {
        Error("Invalid token type!", token);
    }
    return token;
}

Token Parser::Advance()
{
    Token token = m_tokens[m_position];
    m_position++;
    return token;
}

Token Parser::Peek()
{
    return m_tokens[m_position];
}

Select_ptr Parser::Select()
{
    Consume(TokenType::Select);
    auto node = std::make_unique<SelectNode>();
    node->columnList = ColumnList();
    node->from = From();
    if (Peek().type == TokenType::Where)
        node->where = Where();
    Consume(TokenType::Eof);
    return node;
}

From_ptr Parser::From()
{
    Consume(TokenType::From);
    Token table = Consume(TokenType::Identifier);
    std::string tableName = GetTokenValue(table);
    return std::unique_ptr<FromNode>(new FromNode{tableName});
}

Where_ptr Parser::Where()
{
    auto node = std::make_unique<WhereNode>();
    Consume(TokenType::Where);
    node->condition = MakeExpression();
    return node;
}

SelectColumnList_ptr Parser::ColumnList()
{
    auto node = std::make_unique<SelectColumnList>();
    while (true)
    {
        auto name = MakeExpression();
        node->columns.push_back(std::move(name));

        if (Peek().type == TokenType::From)
        {
            break;
        }
        Consume(TokenType::Comma);
    }
    return node;
}

Expr_ptr Parser::MakeExpression()
{
    auto node = MakeAndExpression();
    if (Peek().type != TokenType::Or)
        return node;
    node = std::make_unique<AnyExpression>(LogicalExpression{std::move(node), Advance(), MakeExpression()});
    return node;
}

Expr_ptr Parser::MakeAndExpression()
{
    auto node = MakeBinaryExpression();
    if (Peek().type != TokenType::And)
        return node;
    node = std::make_unique<AnyExpression>(LogicalExpression{std::move(node), Advance(), MakeAndExpression()});
    return node;
}

Expr_ptr Parser::MakeBinaryExpression()
{
    auto node = MakeArithmeticExpression();
    switch (Peek().type)
    {
    case TokenType::Eq:
    case TokenType::Neq:
    case TokenType::Gt:
    case TokenType::Lt:
        return std::make_unique<AnyExpression>(
            BinaryExpression{std::move(node), Advance(), MakeArithmeticExpression()});
    default:
        return node;
    }
}

Expr_ptr Parser::MakeArithmeticExpression()
{
    auto node = MakeLiteralExpression();
    switch (Peek().type)
    {
    case TokenType::Plus:
    case TokenType::Minus:
    case TokenType::Star:
    case TokenType::Slash:
        return std::make_unique<AnyExpression>(
            BinaryExpression{std::move(node), Advance(), MakeArithmeticExpression()});
    default:
        return node;
    }
}

Expr_ptr Parser::MakeLiteralExpression()
{
    Token token = Advance();
    if (token.type == TokenType::Number || token.type == TokenType::Identifier || token.type == TokenType::String)
    {
        return std::make_unique<AnyExpression>(LiteralExpression{token});
    }
    Error("Expression can be only number or identifier or string.", token);
    return Expr_ptr();
}

Select_ptr Parser::Parse()
{
    return Select();
}

ParsingError Parser::GetError()
{
    if (m_compilationError.has_value())
        return m_compilationError.value();
    return ParsingError();
}
} // namespace DBCPP::SqlInterface
