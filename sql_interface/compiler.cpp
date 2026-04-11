#include "compiler.h"
#include <stdexcept>
namespace DBCPP::SqlInterface 
{
    void Compiler::Error(std::string message, Token token)
    {
        m_compilationError = CompilationError { message, token };
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
             throw std::domain_error("Compilation error!");
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
        node->ColumnList = ColumnList();
        node->From = From();
        node->Where = Where();
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
        return Where_ptr();
    }

    SelectColumnList_ptr Compiler::ColumnList()
    {
        auto node = std::make_unique<SelectColumnList>();
        bool firstColumn = true;
        while(true)
        {
            Token column = Consume(TokenType::Identifier);
            auto name = GetTokenValue(column);
            node->Columns.push_back(std::move(name));

            if(Peek().type == TokenType::From)
            {
                break;
            }
            Consume(TokenType::Comma);
        }
        return node;
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
