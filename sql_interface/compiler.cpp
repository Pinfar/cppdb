#include "compiler.h"
#include <stdexcept>
namespace DBCPP::SqlInterface 
{
    void Compiler::Error(std::string message, Token token)
    {
        m_compilationError = CompilationError { message, token };
        throw std::domain_error("Compilation error!");
    }

    static std::string GetTokenValue(Token token)
    {
        return token.source->substr(token.beginOffset, token.length);
    }

    Token Compiler::Consume(TokenType type)
    {
        Token token = Advance();
        if(token.type != type)
        {
             Error("Invalid token type!", token);
        }
        return token;
    }

    Token Compiler::Advance()
    {
        Token token = m_tokens[m_position];
        m_position++;
        return token;
    }

    Token Compiler::Peek()
    {
        return m_tokens[m_position];
    }

    Select_ptr Compiler::Select()
    {
        Consume(TokenType::Select);
        auto node = std::make_unique<SelectNode>();
        node->columnList = ColumnList();
        node->from = From();
        if(Peek().type == TokenType::Where) node->where = Where();
        Consume(TokenType::Eof);
        return node;
    }

    From_ptr Compiler::From()
    {
        Consume(TokenType::From);
        Token table = Consume(TokenType::Identifier);
        std::string tableName = GetTokenValue(table);
        return std::unique_ptr<FromNode>(new FromNode{tableName});
    }

    Where_ptr Compiler::Where()
    {
        auto node = std::make_unique<WhereNode>();
        Consume(TokenType::Where);
        node->condition = Condition();
        return node;
    }

    SelectColumnList_ptr Compiler::ColumnList()
    {
        auto node = std::make_unique<SelectColumnList>();
        bool firstColumn = true;
        while(true)
        {
            Token column = Consume(TokenType::Identifier);
            auto name = GetTokenValue(column);
            node->columns.push_back(std::move(name));

            if(Peek().type == TokenType::From)
            {
                break;
            }
            Consume(TokenType::Comma);
        }
        return node;
    }

    ConditionNode_ptr Compiler::Condition()
    {
        auto node = std::make_unique<ConditionNode>();
        node->lhs = Expression();
        node->oper = Advance(); //This is not correct -> token that is not an operator should not be consumed here.
        node->rhs = Expression();
        return node;
    }

    Expr_ptr Compiler::Expression()
    {
        auto node = std::make_unique<ExpressionNode>();
        Token token = Advance();
        if(token.type == TokenType::Number || token.type == TokenType::Identifier){
            node->token = token;
            return node;
        }
        Error("Expression can be only number or identifier.", token);
        return Expr_ptr();
    }

    Select_ptr Compiler::Compile()
    {
        return Select();
    }

    CompilationError Compiler::GetError()
    {
        if(m_compilationError.has_value()) 
            return m_compilationError.value(); 
        return CompilationError();
    }
}
